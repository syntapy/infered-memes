void AddToken(Tokens **head, char *token)
{
    /*  Adds the token (char *) token to 
     *  the linked list (Tokens **) head
     */

    char buf[2];
    int n;

    if (head == NULL || *head == NULL)
        NoMallocErr("AddToken 1");

    // allocates new node in (Tokens **) head
    n = strlen(token) + 1;
    (*head) -> token = calloc(n, sizeof(char));
    if ((*head) -> token == NULL)
        MallocErr("AddToken 2");
    strcpy((*head) -> token, token);
    (*head) -> next = calloc(1, sizeof(Tokens));

    if ((*head) -> next == NULL)
        MallocErr("AddTokens 3");

    *head = (*head) -> next;
}

int T_Contains(Tokens **args, char *arg)
{
   /*   Searches (Tokens **) args for string (char *) arg
    *
    *   Returns 1 if found
    *   Otherwise returns 0
    *
    *   ARGUMENTS:
    *       (Tokens **) args: linked list of strings
    *       (char *) arg: string to be searched for
    */

    int return_val = 0;
    Tokens *args_ptr = NULL;

    if (args != NULL)
    {
        args_ptr = (*args);
        while (args_ptr != NULL)
        {
            if (strcmp((const char *) args_ptr -> token, arg) == 0)
            {
                return_val = 1;
                break;
            }

            args_ptr = args_ptr -> next;
        }
    }

    return return_val;
}

Args *A_Contains(Args **args, char *arg)
{
    /*  Returns true if (Tokens **) args contains a node
     *  with character (char) arg
     *
     *  Otherwise returns false
     */

    Args *return_val = NULL;
    Args *args_ptr = NULL;

    if (args != NULL)
    {
        args_ptr = (*args);
        while (args_ptr != NULL)
        {
            if (strcmp((const char *) args_ptr -> token, arg) == 0)
            {
                return_val = args_ptr;
                break;
            }

            args_ptr = args_ptr -> next;
        }
    }

    return return_val;
}

int OprtrContains(OprtrArgs *tail, char *arg)
{
    /*  Checks if string (char *) arg is contained
     *  inside the linked list (OprtrArgs *) tail
     *
     *  ARGUMENTS:
     *
     *      (OprtrArgs *) tail: a linked list of linked lists
     *          or argument strings
     *
     *      (char *) arg: string argument
     */

    OprtrArgs **head = NULL;
    int return_val = 0;
    head = &tail;

    // Search all nodes in (OprtrArgs *) tail
    while (head != NULL && (*head) != NULL)

        // (*head) -> args is a linked list of strings.
        // 
        // Searches (*head) -> args for (char *) arg
        if (!T_Contains(&((*head) -> args), arg))
            head = &((*head) -> next);
        else
        {
            return_val = 1;
            break;
        }

    return return_val;
}

char *List(Tokens **tokens, int index)
{
    /*  Returns the char at node (int) index of (Tokens **) tokens.
     *  (int) index is a zero-based index
     */

    int i;
    char *return_val = NULL;
    Tokens **head = NULL;

    head = calloc(1, sizeof(Tokens *));
    if (head == NULL)
        MallocErr("List 1");

    if (tokens != NULL)
    {
        (*head) = (*tokens);
        for (i = 0; i < index; i++)
        {
            if (*head == NULL)
                break;
            (*head) = (*head) -> next;
        }

        if (i == index)
            strcpy(return_val, (*head) -> token);
    }

    return return_val;
}

int ListLen(Args **args)
{
    int return_val = 0;
    Args **args_ptr = args;
    while (args_ptr != NULL && (*args_ptr) != NULL)
    {
        args_ptr = &((*args_ptr) -> next);
        return_val++;
    }

    return return_val;
}

int E_GetIndex(Tokens **tokens, char *token)
{
    int return_val = -1, index = 0;

    Tokens **head = NULL;

    head = calloc(1, sizeof(Tokens *));
    if (head == NULL)
        MallocErr("GetIndex 1");

    if (tokens != NULL)
    {
        (*head) = (*tokens);
        while ((*head) != NULL && strcmp(token, (*head) -> token) != 0)
        {
            (*head) = (*head) -> next;
            index++;
        }

        if ((*head) != NULL && strcmp((*head) -> token, token) == 0)
            return_val = index;
    }

    return return_val;
}

int U_GetIndex(Args **args, char *arg)
{
    int return_val = -1, index = 0;

    Args **head = NULL;

    head = calloc(1, sizeof(Args *));
    if (head == NULL)
        MallocErr("GetIndex 1");

    if (args != NULL)
    {
        (*head) = (*args);
        while ((*head) != NULL && strcmp(arg, (*head) -> token) != 0)
        {
            (*head) = (*head) -> next;
            index++;
        }

        if ((*head) != NULL && strcmp((*head) -> token, arg) == 0)
            return_val = index;
    }

    return return_val;
}

void E_AddArg(Tokens **e_args, char *arg, int depth)
{
    /*  Ensures that (char *) arg is in linked list
     *  pointed to by (Tokens **) e_args  
     *
     *  If (char *) arg is already in the list, nothing is done
     *
     *  ARGUMENTS:
     *
     *      (Tokens **) e_args: Linked list of string
     *      (char *) arg: string being searched for
     *      (int) depth: depth of nested quantifiers 
     */

    Tokens **e_args_ptr = e_args;

    if (e_args_ptr == NULL)
        NoMallocErr("E_AddArg 1");

    // Searches (Tokens **) e_args for node containing 
    // string (char *) arg, and points (Tokens **) e_args_ptr
    // to it. If (char *) arg not in list, points 
    // (Tokens **) e_args_ptr to head of list.
    while ((*e_args_ptr) != NULL)
    {
        if (strcmp((*e_args_ptr) -> token, arg) == 0)
            break;
        else
            e_args_ptr = &((*e_args_ptr) -> next);
    }

    // Add string (char *) arg to linked list if it wasn't found
    if ((*e_args_ptr) == NULL)
    {
        (*e_args_ptr) = calloc(1, sizeof(Tokens));
        if ((*e_args_ptr) == NULL)
            MallocErr("E_AddArg 2");

        (**e_args_ptr).token = calloc(strlen(arg)+1, sizeof(char));
        if ((**e_args_ptr).token == NULL)
            MallocErr("E_AddArg 1");

        strcpy((**e_args_ptr).token, (const char *) arg);
        (**e_args_ptr).depth = depth;
    }
}

Tokens **E_CopyTokens(Tokens **e_args)
{
    Tokens **return_val = NULL, **arg_ptr = NULL, **return_ptr = NULL;
    int s = 0;

    if (e_args != NULL)
    {
        return_val = calloc(1, sizeof(Tokens *));
        if (return_val == NULL)
            MallocErr("CopyUTokens 1");

        arg_ptr = e_args;
        return_ptr = return_val;

        while (*arg_ptr != NULL)
        {
            (*return_ptr) = calloc(1, sizeof(Tokens));
            if ((*return_ptr) == NULL)
                MallocErr("CopyUTokens 2");

            if ((**arg_ptr).token != NULL)
            {
                s = strlen((**arg_ptr).token);
                (**return_ptr).token = calloc(s, sizeof(char));
                if ((**return_ptr).token == NULL)
                    MallocErr("CopyUTokens 3");
                strcpy((**return_ptr).token, (**arg_ptr).token);
            }

            (**return_ptr).depth = (**arg_ptr).depth;
            arg_ptr = &((*arg_ptr) -> next);
            return_ptr = &((*return_ptr) -> next);
        }
    }

    return return_val;
}

Args **U_CopyArgs(Args **u_args, Tokens **arg_list)
{
    Args **return_val = NULL, **arg_ptr = NULL, **return_ptr = NULL;
    int s = 0;

    if (u_args != NULL)
    {
        return_val = calloc(1, sizeof(Args *));
        if (return_val == NULL)
            MallocErr("CopyUArgs 1");

        arg_ptr = u_args;
        return_ptr = return_val;

        while (*arg_ptr != NULL)
        {
            (*return_ptr) = calloc(1, sizeof(Args));
            if ((*return_ptr) == NULL)
                MallocErr("CopyUArgs 2");

            if ((**arg_ptr).token != NULL)
            {
                s = strlen((**arg_ptr).token);
                (**return_ptr).token = calloc(s, sizeof(char));
                if ((**return_ptr).token == NULL)
                    MallocErr("CopyUArgs 3");
                strcpy((**return_ptr).token, (**arg_ptr).token);
            }

            if ((**arg_ptr).token_ptr != NULL)
                (**return_ptr).token_ptr = (*arg_list);
            
            (**return_ptr).depth = (**arg_ptr).depth;
            arg_ptr = &((*arg_ptr) -> next);
            return_ptr = &((*return_ptr) -> next);
        }
    }

    return return_val;
}

void U_FreeTop(Args **u_args)
{
    Args **arg_ptr = u_args;

    if (u_args != NULL)
    {
        if (*u_args != NULL)
        {
            while ((*arg_ptr) -> next != NULL)
                arg_ptr = &((*arg_ptr) -> next);
            free((**arg_ptr).token);
            (**arg_ptr).token_ptr = NULL;
            free(*arg_ptr); *arg_ptr = NULL;
        }
    }
}

void E_FreeTop(Tokens **e_args)
{
    Tokens **token_ptr = e_args;

    if (e_args != NULL)
    {
        if (*e_args != NULL)
        {
            while ((*token_ptr) -> next != NULL)
                token_ptr = &((*token_ptr) -> next);
            free((**token_ptr).token);
            free(*token_ptr); *token_ptr = NULL;
        }
    }

}

void U_AddArg(Tokens **arg_list, Args **u_args, char *arg, int depth)
{
    /*  Searches the stack (Args **) u_args for the string string (char *) arg.
     *  If found, does nothing, else pushes the string onto the stack.
     *
     *  ARGUMENTS:
     *      (Tokens **) arg_list: linked list of literal argument strings
     *      (Args **) u_args: linked list stack of universally quantified
     *          arguments (if any). (*arg_list) should be NULL if stack is empty
     *      (char *) arg: argument string
     *      (int) depth: depth into quantifier statements
     */

    Args **u_args_ptr = u_args;

    if (arg_list == NULL || u_args_ptr == NULL)
        NoMallocErr("U_AddArg 1");

    // Search stack
    while ((*u_args_ptr) != NULL)
    {
        if (strcmp((*u_args_ptr) -> token, arg) == 0)
            break;
        else
            u_args_ptr = &((*u_args_ptr) -> next);
    }

    // Insert into stack
    if ((*u_args_ptr) == NULL)
    {
        (*u_args_ptr) = calloc(1, sizeof(Args));
        if ((*u_args_ptr) == NULL)
            MallocErr("U_AddArg 2");

        (*u_args_ptr) -> token = calloc(strlen(arg)+1, sizeof(char));
        if ((*u_args_ptr) -> token == NULL)
            MallocErr("U_AddArg 3");
        strcpy((*u_args_ptr) -> token, arg);
        (*u_args_ptr) -> token_ptr = (*arg_list);
        (**u_args_ptr).depth = depth;
    }
}

void FreeTokens(Tokens **tokens)
{
    if (tokens == NULL)
        InconsistencyErr("FreeTokens 1");

    if ((*tokens) != NULL)
        FreeTokens(&((*tokens) -> next));

    free(*tokens); (*tokens) = NULL;
}

void FreeArgs(Args **args)
{
    if (args == NULL)
        InconsistencyErr("FreeArgs 1");

    if ((*args) != NULL)
        FreeArgs(&((*args) -> next));

    free(*args); (*args) = NULL;
}

void FreeOprtrArgs(OprtrArgs **args)
{
    if (args == NULL)
        InconsistencyErr("FreeArgs 1");

    if ((*args) != NULL)
        FreeOprtrArgs(&((*args) -> next));

    free(*args); (*args) = NULL;
}

void SetArg(Tokens **arg_list, Tokens **e_args, Args **u_args, 
    char **arg, char *token, int depth, char quant)
{
    /*  Generates the literal argument string from the token.
     *  (char *) token is an argument string as encountered
     *  in the logical sentence file.
     *  Depending on whether (char *) token has a quantifier
     *  associated with it, and if so, which quantifier,
     *  either skolimization (universal quantifier),
     *  argument generation (existential quantifier),
     *  of simply using the literal value of token string
     *  (no quantifier), is done.
     *
     *  ARGUMENTS:
     *      (Tokens **) arg_list: list of non-quantified arguments
     *      (Tokens **) e_args: linked list stack of existentially
     *          quantified argument strings
     *      (Args **) u_args: linked list stack of universally
     *          quantified argument strings
     *      (char **) arg: array of string pointers to argument strings.
     *      (int) subtree_negate: is 1 if prps node is to be negated
     *                            is 0 otherwise
     *      (char *) token: token arguemnt string from list of tokens
     *      (int) depth: depth inside nested quantifier sentences
     *      (char) quant: is '3' for existential,
     *          and '4' for universal, quantifier.
     */

    int found, i, j, arg_size = 1;
    Args *arg_ptr = NULL;
    Tokens **arg_list_head = NULL;
    if (arg == NULL)
        InconsistencyErr("SetArg 1");

    arg_ptr = A_Contains(u_args, token);
    if ((e_args == NULL && u_args == NULL) || (!arg_ptr && !T_Contains(e_args, token)))
    {
        if (*arg != NULL)
            InconsistencyErr("SetArg 2.25");
        *arg = calloc(strlen(token)+1, sizeof(char));
        if (*arg == NULL)
            MallocErr("SetArg 2.375");
        strcpy(*arg, token);
    }

    // Generate new literal symbol that is
    // not one of the non-quantified (or even quantified)
    // argument strings.
    //
    // i.e. this is how you deal with existentially
    // quantified arguments.
    else if (T_Contains(e_args, token))
    {
        found = 0;
        while (!found)
        {
            if (*arg != NULL)
            {
                free(*arg); (*arg) = NULL;
                arg_size++; // Increment number of characters in new argument str
            }

            *arg = calloc(arg_size + 1, sizeof(char));
            if (*arg == NULL)
                MallocErr("SetArg 2.5");

            // initialize new symbol to "aaa..."
            // however many characters it is (arg_size)
            for (i = 0; i < arg_size; i++)
                (*arg)[i] = LOWER_CASE_FIRST;
            (*arg)[arg_size] = '\0';

            j = -1; 
            
            // This makes sure that that symbol does not already exist
            while (!found)
                if (T_Contains(arg_list, *arg) || strcmp(*arg, "v") == 0)
                {
                    if (j == 0)
                        // Symbol cannot be generated
                        // with only arg_size characters
                        break;
                    j = arg_size - 1;
                    (*arg)[j] += 1;
                    while ((*arg)[j] == LOWER_CASE_LAST + 1 && j > 0)
                    {
                        (*arg)[j] = LOWER_CASE_FIRST;
                        j -= 1;
                        (*arg)[j] += 1;
                    }
                }

                else
                    found = 1;
        }
    }

    // token is among the list of universally quantified arguments
    else if (arg_ptr != NULL)
    {
        if (*arg != NULL)
            InconsistencyErr("SetArg 2.75");

        if (arg_ptr -> token_ptr != NULL)
        {
            // This is where the skolemization is done! Yay!
            *arg = calloc(strlen(arg_ptr -> token_ptr -> token)+1, sizeof(char));
            if (*arg == NULL)
                MallocErr("SetArg 3");
            strcpy(*arg, arg_ptr -> token_ptr -> token);
        }
        else
        {   // This is done when there is no non-quantified arguments
            *arg = calloc(strlen(arg_ptr -> token)+1, sizeof(char));
            if (*arg == NULL)
                MallocErr("SetArg 3.5");
            strcpy(*arg, arg_ptr -> token);
        }
    }

    else
        InconsistencyErr("SetArg 4");
}
