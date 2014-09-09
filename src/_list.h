void AddToken(Tokens **head, char token)
{
    char buf[2];
    sprintf(buf, "%c", token);

    if (head == NULL || *head == NULL)
        NoMallocErr("AddToken 1");

    (*head) -> token = token;
    (*head) -> next = calloc(1, sizeof(Tokens));

    if ((*head) -> next == NULL)
        MallocErr("AddTokens 2");

    *head = (*head) -> next;
}

int T_Contains(Tokens **args, char arg)
{
    /*  Returns true if (Tokens **) args contains a noe
        with character (char) arg

        Otherwise returns false
    */
    int return_val = 0;
    Tokens *args_ptr = NULL;

    if (args != NULL)
    {
        args_ptr = (*args);
        while (args_ptr != NULL)
        {
            if (args_ptr -> token == arg)
            {
                return_val = 1;
                break;
            }

            args_ptr = args_ptr -> next;
        }
    }

    return return_val;
}

Args *A_Contains(Args **args, char arg)
{
    /*  Returns true if (Tokens **) args contains a noe
        with character (char) arg

        Otherwise returns false
    */
    Args *return_val = NULL;
    Args *args_ptr = NULL;

    if (args != NULL)
    {
        args_ptr = (*args);
        while (*args != NULL)
        {
            if (args_ptr -> token == arg)
            {
                return_val = args_ptr;
                break;
            }

            args_ptr = args_ptr -> next;
        }
    }

    return return_val;
}

char List(Tokens **tokens, int index)
{
    /*  Returns the char at node (int) index of (Tokens **) tokens.
        (int) index is a zero-based index
    */

    int i;
    char return_val = 0;
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
        {
            return_val = (*head) -> token;
        }
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

int E_GetIndex(Tokens **tokens, char token)
{
    int return_val = -1, index = 0;

    Tokens **head = NULL;

    head = calloc(1, sizeof(Tokens *));
    if (head == NULL)
        MallocErr("GetIndex 1");

    if (tokens != NULL)
    {
        (*head) = (*tokens);
        while ((*head) != NULL && token != (*head) -> token)
        {
            (*head) = (*head) -> next;
            index++;
        }

        if ((*head) != NULL && (*head) -> token == token)
            return_val = index;
    }

    return return_val;
}

int U_GetIndex(Args **args, char arg)
{
    int return_val = -1, index = 0;

    Args **head = NULL;

    head = calloc(1, sizeof(Args *));
    if (head == NULL)
        MallocErr("GetIndex 1");

    if (args != NULL)
    {
        (*head) = (*args);
        while ((*head) != NULL && arg != (*head) -> token)
        {
            (*head) = (*head) -> next;
            index++;
        }

        if ((*head) != NULL && (*head) -> token == arg)
            return_val = index;
    }

    return return_val;
}

Args *GetArg(Args **args, char arg)
{
    
}

void E_AddArg(Tokens **e_args, char arg)
{
    /*  Adds (char) arg to (Tokens **) e_args, and sets
        (char *) token_ptr to the first element in
        (Tokens **) e_arg_list_tail
    */

    Tokens **e_args_ptr = e_args;

    if (e_args_ptr == NULL)
        NoMallocErr("E_AddArg 1");

    while ((*e_args_ptr) != NULL)
    {
        if ((*e_args_ptr) -> token == arg)
            break;
        else
            e_args_ptr = &((*e_args_ptr) -> next);
    }

    if ((*e_args_ptr) == NULL)
    {
        (*e_args_ptr) = calloc(1, sizeof(Tokens));
        if ((*e_args_ptr) == NULL)
            MallocErr("E_AddArg 2");

        (*e_args_ptr) -> token = arg;
    }
}

void U_AddArg(Tokens **arg_list, Args **u_args, char arg)
{
    /*  Adds (char) arg to (Args **) u_args, and sets
     *  (char *) token_ptr to the first element in
     *  (Tokens **) arg_list
     */

    Args **u_args_ptr = u_args;

    if (arg_list == NULL || u_args_ptr == NULL)
        NoMallocErr("U_AddArg 1");

    while ((*u_args_ptr) != NULL)
    {
        if ((*u_args_ptr) -> token == arg)
            break;
        else
            u_args_ptr = &((*u_args_ptr) -> next);
    }

    if ((*u_args_ptr) == NULL)
    {
        (*u_args_ptr) = calloc(1, sizeof(Args));
        if ((*u_args_ptr) == NULL)
            MallocErr("U_AddArg 2");

        (*u_args_ptr) -> token = arg;
        (*u_args_ptr) -> token_ptr = (*arg_list);
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

void SetArg(Tokens **arg_list, Tokens **e_args, Args **u_args, 
    char **arg, char token, int depth, char quant)
{
    /*  (int) depth represents the levels of nested 
     *  universal / existential quantifiers this is in, so as
     *  to determine whether to take the (char) token literally
     *  or as an existential / universal quantifer symbol
     */

    Args *arg_ptr = NULL;
    Tokens **arg_list_head = NULL;
    if (arg == NULL)
        InconsistencyErr("SetArg 1");

    (*arg) = calloc(2, sizeof(char));
    if ((*arg) == NULL)
        MallocErr("SetArg 2");

    //(*arg_list_head) = (*arg_list);
    arg_ptr = A_Contains(u_args, token);
    if (quant == '\0' ||
            ((e_args != NULL && !T_Contains(e_args, token)) && arg_ptr == NULL))
    {
        //if (!T_Contains(arg_list, token))
        //    InconsistencyErr("SetArg 2.1");

        (*arg)[0] = token;
        (*arg)[1] = '\0';
    }

    else if (e_args != NULL && quant == '3' && T_Contains(e_args, token))
    {
        (*arg)[0] = LOWER_CASE_FIRST; (*arg)[1] = '\0';
        
        while (T_Contains(arg_list, (*arg)[0]) || (*arg)[0] == 'v')
            (*arg)[0] += 1;
        if ((*arg)[0] > LOWER_CASE_LAST)
            InconsistencyErr("SetArg 3");
    }

    else if (u_args != NULL && quant == '4' && arg_ptr != NULL)
    {
        if (arg_ptr -> token_ptr != NULL)
            (*arg)[0] = arg_ptr -> token_ptr -> token;
        else
            (*arg)[0] = arg_ptr -> token;
        (*arg)[1] = '\0';
    }

    else
        InconsistencyErr("SetArg 4");
}
