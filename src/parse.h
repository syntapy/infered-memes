// Requires string.h library !!! //


//***********************************//
//      S Y N T A X - R U L E S      //
//***********************************//
//
//      Forall
//      Exists
//
//      Arguments: 
//          Any number of chars: a - z
//              All Lower Case
//          
//
//      Proposition functions:
//          1st char: A - Z
//              Upper Case
//          Any number of lower case letters: a - z
//          Left (
//          One Argument                     .  
//          Right)                                          
//                           .                     ***********
//      Operators:                                 ***********
//          and             .   .                  ** **  * **
//          or                 *                   **  *  * **
//          implies                                *****  * **
//          equivalent        .        .           *****  * **
//                                                        * **
//                                                        * **
//                                                       ** **
//*************************************************************
//**************************************************************

typedef struct Tokens
{
    char token;
    struct Tokens *next;
} Tokens;

typedef struct TreeList
{
    PrpsTree **tree;
    struct TreeList *next;
} TreeList;

typedef struct OprtrList
{
    int oprtr;
    struct OprtrList *next;
} OprtrList;

int IsLowerCase(char c)
{   int return_val = FALSE;
    if ((int) c <= LOWER_CASE_LAST && (int) c >= LOWER_CASE_FIRST)
        return_val = TRUE;
    return return_val;
}

int IsUpperCase(char c)
{   int return_val = FALSE;
    if ((int) c <= UPPER_CASE_LAST && (int) c >= UPPER_CASE_FIRST)
        return_val = TRUE;
    return return_val;
}

int GetOprtr(const char *symbol)
{
    if (strcmp(symbol, "^") == 0)
        return AND;
    else if (strcmp(symbol, "v") == 0)
        return OR;
    else if (strcmp(symbol, "=") == 0)
        return TAU;
    else if (strcmp(symbol, ">") == 0)
        return IMP;
    else return -1;
}

int IsOprtr(const char *symbol)
{
    int oprtr;
    int return_val = FALSE;

    oprtr = GetOprtr(symbol);

    if (oprtr == AND || oprtr == OR || oprtr == IMP || oprtr == TAU)
        return_val = TRUE;   

    return return_val;
}

char **ReadKB(int m, const char *filename)
{
    char **kb_string;
    FILE *f = fopen(filename, "r");
    int n[m];
    int i;

    kb_string = calloc(m, sizeof(char *));
    if (kb_string == NULL)
        MallocErr("ReadKB 1");

    for (i = 0; i < m; i++)
        getline(&(kb_string[i]), &(n[i]), f);

    fclose(f);

    return kb_string;
}

void LearnKB(HashTable **hash, char **kb_string, int m, int h, int k)
{
    int i = 0, j;
    int n = strlen(kb_string[0]);
    int hash_val;

    char **args = NULL, **prps = NULL;
    args = calloc(n-2, sizeof(char *));
    prps = calloc(m-1, sizeof(char *));

    if (args == NULL || prps == NULL)
        MallocErr("LearnKB 1");

    for (j = 1; j < n-1; j++)
    {   args[j-1] = calloc(2, sizeof(char));
        if (args[j-1] == NULL)
            MallocErr("LearnKB 3");
        args[j-1][0] = kb_string[0][j];
        args[j-1][1] = '\0';
    }

    for (j = 0; j < m-1; j++)
    {   
        prps[j] = calloc(2, sizeof(char));
        if (prps[j] == NULL)
            MallocErr("LearnKB 2");

        prps[j][0] = kb_string[j+1][0];
        prps[j][1] = '\0';
    }

    for (i = 1; i < m; i++)
        for (j = 1; j < n-1; j++)
        {
            if (kb_string[i][j] == ' ')
                hash_val = -1;
            else if (kb_string[i][j] == '1')
                hash_val = 1;
            else if (kb_string[i][j] == '0')
                hash_val = 0;

            SetValueForArg(prps[i-1], args[j-1], hash_val, hash, h, k);
        }
}

char *ReadPRPS(const char *filename)
{
    char *prps = NULL;
    int n;

    FILE *f = fopen(filename, "r");

    getline(&prps, &n, f);
    
    fclose(f);

    return prps;
}

char *ReadALPHA(const char *filename)
{
    char *alpha = NULL;
    int n;

    FILE *f = fopen(filename, "r");

    getline(&alpha, &n, f);

    fclose(f);

    return alpha;
}

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
        if (input[i] == SPACE)
        {
            i++;
            continue;
        }

        if (input[i] == '\n')
            break;

        if (input[i] == '~' || input[i] == '(' || input[i] == ')')
        {
            AddToken(&head, input[i++]);
            continue;
        }

        if (input[i] >= UPPER_CASE_FIRST && input[i] <= UPPER_CASE_LAST)
        {
            AddToken(&head, input[i++]);

            if (input[i] == '[')
            {
                i++;
                if (input[i] >= LOWER_CASE_FIRST && input[i] <= LOWER_CASE_LAST)
                {
                    AddToken(&head, input[i++]);

                    if (input[i] == ']')
                    { 	i++;
                        continue;
                    }
                }
            }
        }

        else if (input[i] == '^')
        {
            AddToken(&head, input[i++]);
            continue;
        }

        else if (input[i] == 'v')
        {
            AddToken(&head, input[i++]);
            continue;
        }

        else if (input[i] == '|')
        {
            i++;
            if (input[i] == '|')
            {
                AddToken(&head, input[i++]);
                continue;
            }
        }

        else if (input[i] == '=')
        {
            AddToken(&head, input[i++]);
            continue;
        }

        else if (input[i] == '-')
        {
            i++;
            if (input[i] == '>')
            {
                AddToken(&head, input[i++]);
                continue;
            }
        }

        ParseErr("Unexpected symbol");
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

void print_tokens(Tokens *tokens)
{
    Tokens *head = tokens;

    while (head -> next != NULL);
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

PrpsTree **TokensToTree(Tokens **tokens, int global_negate)
{
    int subtree_negate = 0, oprtr = -1;
    char *prps = NULL, *arg = NULL;
    char token;

    PrpsTree **tree_tmp = NULL, **return_tree = NULL;
    TreeList *tree_list = NULL, **tree_list_head = NULL;
    OprtrList *oprtr_list = NULL, **oprtr_list_head = NULL;

    tree_list = calloc(1, sizeof(TreeList));
    tree_list_head = &tree_list; //calloc(1, sizeof(TreeList *));

    oprtr_list = calloc(1, sizeof(OprtrList));
    oprtr_list_head = &oprtr_list; //calloc(1, sizeof(OprtrList *));

    if (tree_list == NULL || oprtr_list == NULL ||
        tree_list_head == NULL || oprtr_list_head == NULL)
        MallocErr("TokensToTree 1");

    (*tree_list_head) = tree_list; (*oprtr_list_head) = oprtr_list;

    while ((*tokens) -> next != NULL) // && (*tokens) -> token != ')')
    {   
        token = (*tokens) -> token;

        if (token == '~')
        {
            (*tokens) = (*tokens) -> next;
            token = (*tokens) -> token;
            subtree_negate = !subtree_negate;
        }

        if (token == '(')
        {
            (*tokens) = (*tokens) -> next;
            tree_tmp = TokensToTree(tokens, subtree_negate);
            (*tree_list_head) -> tree = tree_tmp;
            tree_list_head = &((*tree_list_head) -> next);
            (*tree_list_head) = calloc(1, sizeof(TreeList));
            if ((*tree_list_head) == NULL)
                MallocErr("TokensToTree 1");
            tree_tmp = NULL;
            continue;
        }

        token = (*tokens) -> token;

        if (IsUpperCase(token))
        {
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
        }

        if (token == '^' || token == 'v' || token == '=' || token == '>')
        {
            if (token == 'v')
                oprtr = OR;
            else if (token == '^')
                oprtr = AND;
            else if (token == '=')
                oprtr = TAU;
            else if (token == '>')
                oprtr = IMP;

            (*oprtr_list_head) -> oprtr = oprtr;
            oprtr_list_head = &((*oprtr_list_head) -> next);
            (*oprtr_list_head) = calloc(1, sizeof(OprtrList));

            if ((*oprtr_list_head) == NULL)
                MallocErr("TokensToTree 10");

            oprtr = -1;
            (*tokens) = (*tokens) -> next;
        }
    }

    return_tree = TreeListToTree(tree_list, oprtr_list);

    return return_tree;
}

char **Read(PrpsTree ***tree, HashTable **hash, int h, int k, int *hash_val)
{   /* This reads the Prpsositional sentence char *sentence[]
     * and turns it into a knowledge base representation.
     * This knowledge base consists of a prpsositional 
     * tree and a hash table that can be used to look up
     * the values the prpsositional functions give for 
     * each of the arguments.
     *
     * This is a recursive function. 
     * i is the index in the prpsosition sentence that it
     * is currently looking at. The sentence is basically 
     * an array of strings. Each string has a maximum length
     * defined by the value of MAXCHARS in the ops.h file.
     * 
     * n is the number of strings in the array of strings.
     */

    char **kb_string = NULL;
    char *alpha_string = NULL;
    char *prps = NULL;
    int m = 4;
    int nn = 0;
    //int hash_val;

    char *a_prps = NULL, *a_arg = NULL;
    const char *filename_kb = "KB.txt";
    const char *filename_prps = "PRPS.txt";
    const char *filename_alpha = "ALPHA.txt";

    char **return_val = NULL;

    return_val = calloc(2, sizeof(char *));
    if (return_val == NULL)
        MallocErr("Read -1");

    return_val[0] = calloc(2, sizeof(char));
    return_val[1] = calloc(2, sizeof(char));

    if (return_val[0] == NULL || return_val[1] == NULL)
        MallocErr("Read -2");

    PrpsTree **tree_tmp = NULL, **alpha = NULL,  **return_tree = NULL;
    Tokens **tokens = NULL;

    a_prps = calloc(2, sizeof(char)); a_arg = calloc(2, sizeof(char));
    if (a_prps == NULL || a_arg == NULL)
        MallocErr("Read 0");

    tokens = calloc(1, sizeof(Tokens *));
    if (tokens == NULL)
        MallocErr("Read 1");

    kb_string = ReadKB(m, filename_kb);
    prps = ReadPRPS(filename_prps);
    alpha_string = ReadALPHA(filename_alpha);

    LearnKB(hash, kb_string, m, h, k);
    (*tokens) = PrpsTokenizer(prps, &nn);

    tree_tmp = TokensToTree(tokens, 0);
    alpha = AlphaToTree(alpha_string, hash_val);

    return_val[0][0] = ((*alpha) -> stmnt -> stc)[0];
    return_val[1][0] = ((*alpha) -> argmnt -> stc)[0];
    return_val[0][1] = '\0'; return_val[1][1] = '\0';

    (*tree) = Oprtr(NOT, tree_tmp, IS, alpha, IS, OR);

    free(tree_tmp); free(alpha);
    tree_tmp = NULL; alpha = NULL;

    //Negate((*tree));

    //print_tokens((*tokens));
    //tree_print(return_tree);
    //printf("\n");

    return return_val;
}
