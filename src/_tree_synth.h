void ListAddTree(TreeList ***tree_list_head, PrpsTree **tree_tmp);
void AddOprtr(OprtrList ***oprtr_list_head, int oprtr);
PrpsTree **QuantifierToTree(Tokens **tokens, Tokens **arg_list,
    Tokens **e_args, Args **u_args, int depth, int global_negate);
PrpsTree **U_E_QuantifierToTree(Tokens **tokens, Tokens **arg_list, 
    Tokens **e_args, Args **u_args, int depth, char quant, int global_negate);
void AddTree(Tokens **tokens, Tokens **arg_list,
    TreeList ***tree_list_head, TreeList **tree_list,
    Tokens **e_args, Args **u_args, 
    int depth, char quant, int subtree_negate);

void FreeOprtrList(OprtrList **oprtr_list)
{
    if (oprtr_list != NULL)
    {
        if ((*oprtr_list) -> next == NULL)
        {
            free(*oprtr_list);
            (*oprtr_list) = NULL;
        }

        else 
            FreeOprtrList(&((*oprtr_list) -> next));
    }
}

void FreeTreeList(TreeList **tree_list)
{
    if (tree_list != NULL)
    {
        if ((*tree_list) -> next == NULL)
        {
            free(*tree_list);
            (*tree_list) = NULL;
        }

        else 
            FreeTreeList(&((*tree_list) -> next));
    }
}

Tokens *PrpsTokenizer(char *input, int *n)
{
    /*  char *input: raw input
        char **sentence[]: tokenized version of input; passed by reference
        int *n: number of tokens in output; passed by reference
    */
    int i, j;
    int space;
    *n = strlen(input);

    Tokens *tokens = NULL, *head = NULL;
    tokens = calloc(1, sizeof(Tokens));

    if (tokens == NULL)
        MallocErr("");

    head = tokens;

    i = 0; j = 0;
    space = 0;

    while (i < *n)
    {
        if (input[i] != ',' && input[i] != ' ' &&
            input[i] != '[' && input[i] != ']')
            AddToken(&head, input[i++]);
        else if (input[i] == '\n')
            break;
        else
            i++;
    }

    return tokens;
}

PrpsTree **AlphaToTree(char *input, int *hash_val)
{
    char *prps = NULL, *arg = NULL;
    int i = 0, val = -1;

    PrpsTree **alpha = NULL;

    while (input[i] != '\0')
    {
        if (IsUpperCase(input[i]))
        {
            prps = calloc(2, sizeof(char));
            if (prps == NULL)
                MallocErr("AlphaToTree 1");
            prps[0] = input[i];
            prps[1] = '\0';
        }

        else if (IsLowerCase(input[i]))
        {
            arg = calloc(2, sizeof(char));
            if (arg == NULL)
                MallocErr("AlphaToTree 2");
            arg[0] = input[i];
            arg[1] = '\0';
        }

        else if (input[i] == '0')
            val = 0;
        else if (input[i] == '1')
            val = 1;

        i++;
    }

    if (prps == NULL || arg == NULL || val == -1)
        InconsistencyErr("AlphaToTree 3");

    alpha = calloc(1, sizeof(PrpsTree *));
    if (alpha == NULL)
        MallocErr("AlphaToTree 4");

    (*alpha) = GenerateEmpty();
    AllocateAsPrps(alpha, (const char *) prps, (const char *) arg);

    if (val == 0)
        *((*alpha) -> neg) = 1;
    else if (val == 1)
        *((*alpha) -> neg) = 0;

    (*hash_val) = val;

    return alpha;
}

void print_args(Args *args)
{
    Args *head = args;

    while (head != NULL)
    {
        fprintf(stdout, "%c", head -> token);
        head = head -> next;
    } 

    fprintf(stdout, "\n");
}

void print_tokens(Tokens *tokens)
{
    Tokens *head = tokens;

    while (head != NULL)
    {
        fprintf(stdout, "%c", head -> token);
        head = head -> next;
    } 

    fprintf(stdout, "\n");
}

void AdvanceToClosingParenthese(Tokens **tokens)
{
    int count = 0;

    count = 1;

    do {
        (*tokens) = (*tokens) -> next;
        if ((*tokens) -> token == '(')
            count++;
        else if ((*tokens) -> token == ')')
            count--;

    } while (count > 0);

    (*tokens) = (*tokens) -> next;
}

PrpsTree **TreeListToTree(TreeList *tree_list, OprtrList *oprtr_list)
{
    PrpsTree **tree = NULL, **tree_tmp = NULL, **tree_new = NULL;

    TreeList *tree_list_head = NULL;
    OprtrList *oprtr_list_head = NULL;
    int oprtr = -1;
    char *prps = NULL;
    //tree = calloc(1, sizeof(PrpsTree *));
    //if (tree == NULL)
    //    MallocErr("TreeListToTree 1");
    //(*tree) = Generate
    tree = tree_list -> tree;
    tree_list_head = tree_list -> next;
    oprtr_list_head = oprtr_list;

    while (tree_list_head != NULL && tree_list_head -> tree != NULL)
    {
        oprtr = oprtr_list_head -> oprtr;
        tree_tmp = tree_list_head -> tree;
        tree_new = Oprtr(IS, tree, IS, tree_tmp, IS, oprtr);
        free(tree); tree = tree_new; tree_new = NULL;
        tree_list_head = tree_list_head -> next;
        oprtr_list_head = oprtr_list_head -> next;
    }

    return tree;
}

/*  Universal / Existential Quantifier processing routines  */
PrpsTree **U_E_QuantifierToTree(Tokens **tokens, Tokens **arg_list, 
    Tokens **e_args, Args **u_args, int depth, char quant, int global_negate)
{
    /*  This starts at the opening curly brace, 
        after the opening portion, eg: 4x 
    */

    TreeList *tree_list = NULL, **tree_list_head = NULL; 
    TreeList *tree_list_tmp = NULL, **tree_list_head_tmp = NULL;
    OprtrList *oprtr_list = NULL, **oprtr_list_head = NULL;
    OprtrList *oprtr_list_tmp = NULL, **oprtr_list_head_tmp = NULL;
    //PrpsTree **tree = NULL, **tree_tmp = NULL;
    int i, index, subtree_negate = 0, will_continue = 1;
    char token, prps, arg, node_arg;
    int oprtr;
    PrpsTree **tree = NULL, **return_val = NULL;

    if (tokens == NULL || arg_list == NULL || u_args == NULL)
        NoMallocErr("U_E_QuantifierToTree 1");

    //tree = calloc(1, sizeof(PrpsTree *));
    //**arg_array = NULL,  
    Tokens **head = NULL, **tail = NULL;

    //arg_array = calloc(depth, sizeof(Tokens *));
    head = calloc(1, sizeof(Tokens *));
    tail = calloc(1, sizeof(Tokens *));
    if (!head || !tail)
        MallocErr("U_E_QuantifierToTree 2");

    //for (i = 0; i < depth; i++)
    //    arg_array[i] = (*arg_list);

    tree_list = calloc(1, sizeof(TreeList));
    oprtr_list = calloc(1, sizeof(OprtrList));
    if (tree_list == NULL || oprtr_list == NULL)
        MallocErr("U_E_QuantifierToTree 3");

    tree_list_head = calloc(1, sizeof(TreeList *));
    oprtr_list_head = calloc(1, sizeof(TreeList *));
    if (!tree_list_head || !oprtr_list_head)
        MallocErr("U_E_QuantifierToTree 4");

    tree_list_head = &tree_list;
    oprtr_list_head = &oprtr_list;
    (*tail) = (*tokens);
    while (will_continue)
    {
        (*head) = (*tail);
        token = (*head) -> token;

        tree_list_tmp = calloc(1, sizeof(TreeList));
        oprtr_list_tmp = calloc(1, sizeof(OprtrList));
        if (tree_list_tmp == NULL || oprtr_list_tmp == NULL)
            MallocErr("U_E_QuantifierToTree 4");

        tree_list_head_tmp = calloc(1, sizeof(TreeList *));
        oprtr_list_head_tmp = calloc(1, sizeof(TreeList *));
        if (!tree_list_head_tmp || !oprtr_list_head_tmp)
            MallocErr("U_E_QuantifierToTree 5");

        tree_list_head_tmp = &tree_list_tmp;
        oprtr_list_head_tmp = &oprtr_list_tmp;
        while (token != '}')
        {
            if (token == '{')
            {
                (*head) = (*head) -> next;
                token = (*head) -> token;
            }

            else if (token == '}')
            {
                (*head) = (*head) -> next;
                token = (*head) -> token;
                break;
            }

            else if ((*head) == NULL)
                InconsistencyErr("U_E_QuantifierToTree 6");

            else if (token == '!')
            {
                subtree_negate = !subtree_negate;
                (*head) = (*head) -> next;
                token = (*head) -> token;
            }

            else if (IsUpperCase(token))
            {
                //  This function needs modification so as to reference
                //      variables from the arg_list instead
                AddTree(head, arg_list, &tree_list_head_tmp, 
                        &tree_list_tmp, e_args, u_args, 
                        depth + 1, quant, subtree_negate);
                token = (*head) -> token;

                //GeneratePrpsNode(tokens, arg_list, q_arg_list, 
                //      depth, global_negate);
            }

            else if (IsOprtr(token))
            {
                oprtr = GetOprtr(token);
                AddOprtr(&oprtr_list_head_tmp, oprtr);
                (*head) = (*head) -> next;
                token = (*head) -> token;
            }

            else if (token == '4' || token == '3')
                QuantifierToTree(head, arg_list, e_args, u_args, 
                    depth, global_negate);
            else
            {
                (*head) = (*head) -> next;
                token = (*head) -> token;
            }

                //U_E_QuantifierToTree(tokens, arg_list, e_args, u_args);
        }

        if ((*u_args) != NULL)
            will_continue = IncrementConditional(arg_list, u_args, quant);
        else
            will_continue = 0;

        tree = TreeListToTree(tree_list_tmp, oprtr_list_tmp);
        ListAddTree(&tree_list_head, tree);

        if (will_continue)
            AddOprtr(&oprtr_list_head, AND);

        FreeOprtrList(&oprtr_list_tmp);
        FreeTreeList(&tree_list_tmp);
        oprtr_list_head_tmp = NULL;
        tree_list_head_tmp = NULL;
    }

    //  Will need to clear (Tokens **) e_args and (Args **) u_args
    //      after finished with this function
    (*tokens) = (*head);

    return_val = TreeListToTree(tree_list, oprtr_list);
    FreeTreeList(&tree_list);
    FreeOprtrList(&oprtr_list);
    tree_list = NULL; oprtr_list = NULL;
    tree_list_head = NULL; oprtr_list_head = NULL;

    if (global_negate)
        Negate(return_val);

    return return_val;
}

//PrpsTree **E_QuantifierToTree(Tokens **tokens, Tokens **arg_list,
//    Tokens **e_args, Args **u_args, int global_negate)
//{
//    TreeList *tree_list = NULL, **tree_list_head = NULL; 
//    TreeList *tree_list_tmp = NULL, **tree_list_head_tmp = NULL;
//    OprtrList *oprtr_list = NULL, **oprtr_list_head = NULL;
//    OprtrList *oprtr_list_tmp = NULL, **oprtr_list_head_tmp = NULL;
//    //PrpsTree **tree = NULL, **tree_tmp = NULL;
//    int i, index, negate = 0, will_continue = 1;
//    char token, prps, arg, node_arg;
//    int oprtr;
//    PrpsTree **tree = NULL, **return_val = NULL;
//
//    if (tokens == NULL || arg_list == NULL || u_args == NULL)
//        NoMallocErr("E_QuantifierToTree 1");
//
//    //tree = calloc(1, sizeof(PrpsTree *));
//    //**arg_array = NULL,  
//    Tokens **head = NULL, **tail = NULL;
//
//    //arg_array = calloc(depth, sizeof(Tokens *));
//    head = calloc(1, sizeof(Tokens *));
//    tail = calloc(1, sizeof(Tokens *));
//    if (!head || !tail)
//        MallocErr("E_QuantifierToTree 2");
//
//    //for (i = 0; i < depth; i++)
//    //    arg_array[i] = (*arg_list);
//
//    tree_list = calloc(1, sizeof(TreeList));
//    oprtr_list = calloc(1, sizeof(OprtrList));
//    if (tree_list == NULL || oprtr_list == NULL)
//        MallocErr("E_QuantifierToTree 3");
//
//    tree_list_head = &tree_list;
//    oprtr_list_head = &oprtr_list;
//
//    //while (will_continue)
//    {
//        (*tail) = (*tokens); (*head) = (*tail);
//        token = (*head) -> token;
//
//        tree_list_tmp = calloc(1, sizeof(TreeList));
//        oprtr_list_tmp = calloc(1, sizeof(OprtrList));
//        if (tree_list_tmp == NULL || oprtr_list_tmp == NULL)
//            MallocErr("E_QuantifierToTree 3");
//
//        tree_list_head_tmp = &tree_list_tmp;
//        oprtr_list_head_tmp = &oprtr_list_tmp;
//        while (token != '}')
//        {
//            if ((*head) == NULL)
//                InconsistencyErr("E_QuantifierToTree 4");
//
//            else if (IsUpperCase(token))
//            {
//                //  This function needs modification so as to reference
//                //      variables from the arg_list instead
//                AddTree(head, arg_list, &tree_list_head_tmp, tree_list,
//                    e_args, u_args, depth + 1, subtree_negate);
//
//                //GeneratePrpsNode(tokens, arg_list, q_arg_list, depth, global_negate);
//            }
//
//            else if (IsOprtr(token))
//            {
//                oprtr = GetOprtr(token);
//                AddOprtr(oprtr_list_head, oprtr);
//            }
//
//            else if (token == '4' || token == '3')
//                QuantifierToTree(head, arg_list, e_args, u_args, global_negate);
//                //E_QuantifierToTree(tokens, arg_list, e_args, u_args);
//        }
//
//        if ((*u_args_ptr) != NULL)
//            will_continue = IncrementConditional(arg_list, u_args_ptr);
//        else
//            will_continue = 0;
//
//        tree = TreeListToTree(tree_list_tmp, oprtr_list_tmp);
//        AddTree(tree_list_head, tree);
//
//        if (will_continue)
//            AddOprtr(oprtr_list_head, AND);
//
//        FreeOprtrList(&oprtr_list_tmp);
//        FreeTreeList(&tree_list_tmp);
//        oprtr_list_head_tmp = NULL;
//        tree_list_head_tmp = NULL;
//    }
//
//    //  Will need to clear (Tokens **) e_args and (Args **) u_args
//    //      after finished with this function
//    return_val = TreeListToTree(tree_list, oprtr_list);
//    FreeTreeList(&tree_list);
//    FreeOprtrList(&oprtr_list);
//    tree_list = NULL; oprtr_list = NULL;
//    tree_list_head = NULL; oprtr_list_head = NULL;
//
//    return return_val;
//
//
//}

PrpsTree **QuantifierToTree(Tokens **tokens, Tokens **arg_list, 
    Tokens **e_args, Args **u_args, int depth, int global_negate)
{
    /*  Returns a PrpsTree from the 
            existential / universal quantifier statement
        Uses skolemization based on args from arg_list

        q_arg_list: list of args that should be 'skolemized'
            So, if an arg is encountered that is not in q_arg_list
            it is taken literaly
    */

    char token, quant = '\0';
    PrpsTree **return_val = NULL;

    if (tokens == NULL || arg_list == NULL)
        NoMallocErr("QuantifierToTree 1");

    token = (*tokens) -> token;

    if (token == '4')
    {
        (*tokens) = (*tokens) -> next;
        token = (*tokens) -> token;

        if (!global_negate)
        {
            quant = '4';
            while (token != '{')
            {
                if (IsLowerCase(token))
                    U_AddArg(arg_list, u_args, token);

                (*tokens) = (*tokens) -> next;
                token = (*tokens) -> token;
            }

            depth++;

            //return_val = U_E_QuantifierToTree(tokens, arg_list, e_args, u_args, 
            //    depth + 1, global_negate);
        }

        else if (global_negate)
        {
            quant = '3';
            while (token != '{')
            {
                if (IsLowerCase(token))
                    E_AddArg(e_args, token);

                (*tokens) = (*tokens) -> next;
                token = (*tokens) -> token;
            }

            //return_val = E_QuantifierToTree(tokens, arg_list, e_args, u_args, 
            //    depth, global_negate);
        }
    }

    else if (token == '3')
    {
        (*tokens) = (*tokens) -> next;
        token = (*tokens) -> token;

        if (!global_negate)
        {
            quant = '3';
            while (token != '{')
            {
                if (IsLowerCase(token))
                    E_AddArg(e_args, token);

                (*tokens) = (*tokens) -> next;
                token = (*tokens) -> token;
            }

            //return_val = E_QuantifierToTree(tokens, arg_list, e_args, u_args,
            //    depth, global_negate);
        }

        else if (global_negate)
        {
            quant = '4';
            while (token != '{')
            {
                if (IsLowerCase(token))
                    U_AddArg(arg_list, u_args, token);

                (*tokens) = (*tokens) -> next;
                token = (*tokens) -> token;
            }

            depth++;
            //return_val = U_E_QuantifierToTree(tokens, arg_list, 
            //    e_args, u_args, depth + 1, global_negate);
        }
    }

    return_val = U_E_QuantifierToTree(tokens, arg_list, 
                    e_args, u_args, depth, quant, global_negate);

    return return_val;
}

void AddTreeNode(TreeList ***tree_list_head, 
    char *prps, char *arg, int subtree_negate)
{
    
    PrpsTree **tree_tmp = NULL;
    tree_tmp = calloc(1, sizeof(PrpsTree *));
    if (tree_tmp == NULL)
        MallocErr("AddTreeNode 1");
    (*tree_tmp) = GenerateEmpty();
    AllocateAsPrps(tree_tmp, (const char *) prps, (const char *) arg);

    (**tree_list_head) -> tree = tree_tmp;
    if (subtree_negate)
        Negate((**tree_list_head) -> tree);
    (*tree_list_head) = &((***tree_list_head).next);
    (**tree_list_head) = calloc(1, sizeof(TreeList)); 
    if ((*tree_list_head) == NULL)
        MallocErr("AddTreeNode 2");
}

/*  Token processing    */
void AddTree(Tokens **tokens, Tokens **arg_list,
    TreeList ***tree_list_head, TreeList **tree_list,
    Tokens **e_args, Args **u_args, 
    int depth, char quant, int subtree_negate)
{
    /*  This function may need fixing, as it adds each 
     *  statement into the list of prps trees
     *  It may be fine, but harder to debug, and less
     *  elegant than if one function adds the tree
     *  for each variable combination in the quantifer
     */

    PrpsTree **tree_tmp = NULL;
    char token, *prps = NULL, *arg = NULL;
    char *arg_array = NULL;
    int will_continue = 1, list_len = ListLen(u_args);
    int i = 0;

    Args **u_args_ptr = NULL;
    Tokens **tokens_ptr = NULL;

    if (tokens == NULL || (*tokens) == NULL/* ||
        e_args == NULL || u_args == NULL*/)
        InconsistencyErr("AddTree 1");

    tokens_ptr = calloc(1, sizeof(Tokens *));
    if (tokens_ptr == NULL)
        MallocErr("AddTree 1");

    (*tokens_ptr) = (*tokens);
    u_args_ptr = u_args;
    token = (*tokens) -> token;

    if (IsUpperCase(token))
    {
        if (prps == NULL)
        {
            prps = calloc(2, sizeof(char));
            if (prps == NULL)
                MallocErr("AddTree 2");

            prps[0] = token; prps[1] = '\0';
        }

        else 
            InconsistencyErr("AddTree 3");

        (*tokens_ptr) = (*tokens_ptr) -> next;
        token = (*tokens_ptr) -> token;

        if (IsLowerCase(token))
        {
            SetArg(arg_list, e_args, u_args, &arg, token, depth, quant);
            AddTreeNode(tree_list_head, prps, arg, subtree_negate);
            free(prps); free(arg);
            prps = NULL; arg = NULL;
        }

        else InconsistencyErr("AddTree 4");

        (*tokens_ptr) = (*tokens_ptr) -> next;
        token = (*tokens_ptr) -> token;
        if (token == ')')
            (*tokens_ptr) = (*tokens_ptr) -> next;
        //if (quant != '\0')
        (*tokens) = (*tokens_ptr);
    }
}

void AddOprtr(OprtrList ***oprtr_list_head, int oprtr)
{
    (**oprtr_list_head) -> oprtr = oprtr;
    (*oprtr_list_head) = &((**oprtr_list_head) -> next);
    (**oprtr_list_head) = calloc(1, sizeof(OprtrList));

    if ((**oprtr_list_head) == NULL)
        MallocErr("AddOprtr 1");
}

void ListAddTree(TreeList ***tree_list_head, PrpsTree **tree_tmp)
{
    (**tree_list_head) -> tree = tree_tmp;
    (*tree_list_head) = &((**tree_list_head) -> next);
    (**tree_list_head) = calloc(1, sizeof(TreeList));
    if ((**tree_list_head) == NULL)
        MallocErr("TokensToTree 1");
}

PrpsTree **TokensToTree(Tokens **tokens, Tokens **arg_list, 
    int global_negate)
{
    int subtree_negate = 0, oprtr = -1, depth = 0;
    char *prps = NULL, *arg = NULL;
    char token, quant = '\0';      // stores currently passed char

    PrpsTree **tree_tmp = NULL, **return_tree = NULL;
    TreeList *tree_list = NULL, **tree_list_head = NULL;
    OprtrList *oprtr_list = NULL, **oprtr_list_head = NULL;

    tree_list = calloc(1, sizeof(TreeList));
    tree_list_head = &tree_list; //calloc(1, sizeof(TreeList *));

    oprtr_list = calloc(1, sizeof(OprtrList));
    oprtr_list_head = &oprtr_list; //calloc(1, sizeof(OprtrList *));

    Args **u_args = NULL;
    Tokens **e_args = NULL;
    if (tree_list == NULL || oprtr_list == NULL ||
        tree_list_head == NULL || oprtr_list_head == NULL)
        MallocErr("TokensToTree 1");

    (*tree_list_head) = tree_list; (*oprtr_list_head) = oprtr_list;
    while ((*tokens) -> next != NULL) // && (*tokens) -> token != ')')
    {
        token = (*tokens) -> token;
        if (token == '{' || token == '-')
        {
            (*tokens) = (*tokens) -> next;
            token = (*tokens) -> token;
        }

        else if (token == '!')
        {
            (*tokens) = (*tokens) -> next;
            token = (*tokens) -> token;
            subtree_negate = !subtree_negate;
        }

        else if (token == '(')
        {
            (*tokens) = (*tokens) -> next;
            tree_tmp = TokensToTree(tokens, arg_list, subtree_negate);
            ListAddTree(&tree_list_head, tree_tmp);
            tree_tmp = NULL;
        }

        else if (token == ')' || token == '}')
        {
            (*tokens) = (*tokens) -> next;
            //break;
        }

        else if (IsUpperCase(token))
            AddTree(tokens, arg_list, &tree_list_head, &tree_list, 
                e_args, u_args, 0, quant, subtree_negate);

        else if (token == '4' || token == '3')
        {
            quant = token;
            if (u_args == NULL)
            {
                u_args = calloc(1, sizeof(Tokens *));
                if (u_args == NULL)
                    MallocErr("TokensToTree 10");
            }

            if (e_args == NULL)
            {
                e_args = calloc(1, sizeof(Args *));
                if (e_args == NULL)
                    MallocErr("TokensToTree 11");
            }

            //AddToken(u_args, token)
            if (quant == '4')
                depth++;
            tree_tmp = QuantifierToTree(tokens, arg_list,
                e_args, u_args, 1, global_negate);
            ListAddTree(&tree_list_head, tree_tmp);

            FreeArgs(u_args); FreeTokens(e_args);
            free(u_args); free(e_args);
            u_args = NULL; e_args = NULL;
            quant = '\0';
        }

        else if (IsOprtr(token))
        {
            oprtr = GetOprtr(token);
            AddOprtr(&oprtr_list_head, oprtr);
            (*tokens) = (*tokens) -> next;
            oprtr = -1;
        }

        else if (token == '\n')
            break;
    }

    return_tree = TreeListToTree(tree_list, oprtr_list);
    return return_tree;
}


// Goes under the Add Tree line

            /*
            if (prps == NULL)
            {
                prps = calloc(2, sizeof(char));
                if (prps == NULL)
                    MallocErr("TokensToTree 2");
                prps[0] = token; prps[1] = '\0';
            }
            else InconsistencyErr("TokensToTree 3");

            (*tokens) = (*tokens) -> next;
            token = (*tokens) -> token;

            if (IsLowerCase(token))
            {
                if (arg == NULL)
                {
                    arg = calloc(2, sizeof(char));
                    if (arg == NULL)
                        MallocErr("TokensToTree 4");

                    arg[0] = token; arg[1] = '\0';
                    (*tokens) = (*tokens) -> next;
                }
                else InconsistencyErr("TokensToTree 5");

                if (tree_tmp == NULL)
                {
                    tree_tmp = calloc(1, sizeof(PrpsTree *));
                    if (tree_tmp == NULL)
                        MallocErr("TokensToTree 6");
                    (*tree_tmp) = GenerateEmpty();
                    AllocateAsPrps(tree_tmp, prps, arg);
                    prps = NULL; arg = NULL;

                    (*tree_list_head) -> tree = tree_tmp;
                    if (subtree_negate)
                        Negate(tree_tmp);
                    tree_list_head = &((*tree_list_head) -> next);
                    (*tree_list_head) = calloc(1, sizeof(TreeList)); 
                    if ((*tree_list_head) == NULL)
                        MallocErr("TokensToTree 7");

                    tree_tmp = NULL;
                }

                else InconsistencyErr("TokensToTree 8");
            }
            else InconsistencyErr("TokensToTree 9");

            token = (*tokens) -> token;
            if (token == ')')
            {
                (*tokens) = (*tokens) -> next;
                break;
            }
            */





/*PrpsTree **GeneratePrpsNode(Tokens **tokens, Tokens **arg_list, 
        Tokens **e_args, Args **u_args,
        int depth, int global_negate)
{*/
    /*  Not sure if this function is needed. Its supposed to
     *  take a statement, such as P[x], and turn it into a
     *  (PrpsTree **) type node.
     *
     *  I think another function is purported to be doing 
     *  the same thing.
     */

/*    int index = 0;
    char token, *prps, *arg, *node_arg;
    PrpsTree **return_val = NULL;
    Tokens **head = NULL;

    if (tokens == NULL || (*tokens) == NULL)
        InconsistencyErr("GeneratePrpsNode 1");

    head = calloc(1, sizeof(Tokens *));
    if (head == NULL)
        MallocErr("GeneratePrpsNode 2");

    prps = calloc(2, sizeof(char));
    arg = calloc(2, sizeof(char));
    node_arg = calloc(2, sizeof(char));
    if (!prps || !arg || !node_arg)
        MallocErr("GeneratePrpsNode 2");

    prps[1] = '\0'; arg[1] = '\0'; node_arg[1] = '\0';
    token = (*tokens) -> token;
    if (IsUpperCase(token))
    {
        prps[0] = token;
        (*head) = (*head) -> next;
        token = (*head) -> token;

        if (IsLowerCase(token) && token != 'v')
        {
            SetArg(arg_list, e_args, u_args, &arg, token, depth);
            (*head) = (*head) -> next;

            if (Contains(u_args, arg))
            {
                index = U_GetIndex(u_args, arg[0]);
                node_arg[0] = arg_array[index] -> token;

                if (return_val == NULL)
                {
                    return_val = calloc(1, sizeof(PrpsTree *));
                    if (return_val == NULL)
                        MallocErr("U_E_QuantifierToTree 3");

                    if ((*return_val) == NULL)
                        (*return_val) = GenerateEmpty();
                    AllocateAsPrps(return_val, prps, node_arg);
                }
            }
        }
    }

    return return_val;
}*/
