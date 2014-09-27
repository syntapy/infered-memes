void AddToken(Tokens **head, char *token)
{
    char buf[2];
    sprintf(buf, "%s", token);

    if (head == NULL || *head == NULL)
        NoMallocErr("AddToken 1");

    (*head) -> token = token;
    (*head) -> next = calloc(1, sizeof(Tokens));

    if ((*head) -> next == NULL)
        MallocErr("AddTokens 2");

    *head = (*head) -> next;
}

int T_Contains(Tokens **args, char *arg)
{
   /*   Returns true if (Tokens **) args contains a noe
    *   with character (char) arg
    *
    *   Otherwise returns false
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

char *List(Tokens **tokens, int index)
{
    /*  Returns the char at node (int) index of (Tokens **) tokens.
        (int) index is a zero-based index
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

void E_AddArg(Tokens **e_args, char *arg)
{
    /*  Adds (char) arg to (Tokens **) e_args, and sets
        (char *) token_ptr to the first element in
        (Tokens **) e_arg_list_tail

        If (char *) arg is already in the list, nothing is done
    */

    Tokens **e_args_ptr = e_args;

    if (e_args_ptr == NULL)
        NoMallocErr("E_AddArg 1");

    while ((*e_args_ptr) != NULL)
    {
        if (strcmp((*e_args_ptr) -> token, arg) == 0)
            break;
        else
            e_args_ptr = &((*e_args_ptr) -> next);
    }

    if ((*e_args_ptr) == NULL)
    {
        (*e_args_ptr) = calloc(1, sizeof(Tokens));
        if ((*e_args_ptr) == NULL)
            MallocErr("E_AddArg 2");

        
        (**e_args_ptr).token = calloc(strlen(arg)+1, sizeof(char));
        if ((**e_args_ptr).token == NULL)
            MallocErr("E_AddArg 1");

        strcpy((**e_args_ptr).token, (const char *) arg);
    }
}

void U_AddArg(Tokens **arg_list, Args **u_args, char *arg)
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
        if (strcmp((*u_args_ptr) -> token, arg) == 0)
            break;
        else
            u_args_ptr = &((*u_args_ptr) -> next);
    }

    if ((*u_args_ptr) == NULL)
    {
        (*u_args_ptr) = calloc(1, sizeof(Args));
        if ((*u_args_ptr) == NULL)
            MallocErr("U_AddArg 2");

        strcpy((*u_args_ptr) -> token, arg);
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
    char **arg, char *token, int depth, char quant)
{
    /*  (int) depth represents the levels of nested 
     *  universal / existential quantifiers this is in, so as
     *  to determine whether to take the (char) token literally
     *  or as an existential / universal quantifer symbol
     */

    int found, i, j, arg_size = 1;
    Args *arg_ptr = NULL;
    Tokens **arg_list_head = NULL;
    if (arg == NULL)
        InconsistencyErr("SetArg 1");

    //(*arg) = calloc(2, sizeof(char));
    //if ((*arg) == NULL)
    //    MallocErr("SetArg 2");

    //(*arg_list_head) = (*arg_list);
    arg_ptr = A_Contains(u_args, token);
    if (quant == '\0' ||
            ((e_args != NULL && !T_Contains(e_args, token)) && arg_ptr == NULL))
    {

        if (*arg != NULL)
            InconsistencyErr("SetArg 2.25");
        *arg = calloc(strlen(token)+1, sizeof(char));
        if (*arg == NULL)
            MallocErr("SetArg 2.375");
        strcpy(*arg, token);
    }

    else if (e_args != NULL && quant == '3' && T_Contains(e_args, token))
    {

        found = 0;
        while (!found)
        {
            if (*arg != NULL)
            {
                free(*arg); (*arg) = NULL;
                arg_size++;
            }

            *arg = calloc(arg_size + 1, sizeof(char));
            if (*arg == NULL)
                MallocErr("SetArg 2.5");

            for (i = 0; i < arg_size; i++)
                (*arg)[i] = LOWER_CASE_FIRST;
            (*arg)[arg_size] = '\0';

            j = -1; 
            
            while (!found)
                if (T_Contains(arg_list, *arg) || strcmp(*arg, "v") == 0)
                {
                    if (j == 0)
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

    else if (u_args != NULL && quant == '4' && arg_ptr != NULL)
    {
        if (*arg != NULL)
            InconsistencyErr("SetArg 2.75");

        if (arg_ptr -> token_ptr != NULL)
        {
            *arg = calloc(strlen(arg_ptr -> token_ptr -> token)+1, sizeof(char));
            if (*arg == NULL)
                MallocErr("SetArg 3");
            strcpy(*arg, arg_ptr -> token_ptr -> token);
        }
        else
        {   *arg = calloc(strlen(arg_ptr -> token)+1, sizeof(char));
            if (*arg == NULL)
                MallocErr("SetArg 3.5");
            strcpy(*arg, arg_ptr -> token);
        }
    }

    else
        InconsistencyErr("SetArg 4");
}
