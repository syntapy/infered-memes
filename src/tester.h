//#include "prps.h"
// 65 - 90: A - Z
// 97 - 122: a - z
typedef struct FindList
{
    char *prps, *arg;
    struct FindList *next;
    int is, is_not;
} FindList;

typedef struct ClauseList
{   //PrpsTree **clause;
    FindList **clause;
    struct ClauseList *prev, *next;
} ClauseList;

void GetLeftMostByte(int *f, int *nmbr)
{
    *nmbr = *f;
    *f = *f >> 8;
    *f = *f << 8;

    *nmbr = *nmbr & ~(*f);
}

void GenSequentialSymbol(char **symbol, int f, int m)
{
    /* This function wou
        m is the number of chars needed in the symbol
     */

    int i, j, value;
    unsigned long int max_val;
    if (symbol == NULL)
        NoMallocErr("GenSequentialSymbols 1");

    if ((*symbol) == NULL)
    {   (*symbol) = calloc(m, sizeof(char));
        if ((*symbol) == NULL)
            MallocErr("GenSequentialSymbol 2");
    }

    max_val = ((unsigned long int) powl(2, 8*m + 1)) - 1;

    if (f > max_val)
        InconsistencyErr("GenSequentialSymbol 3");

    i = 0;
    while (f > 0)
    {
        GetLeftMostByte(&f, &value);
        value += 65;
        (*symbol)[i++] = value;
    }

    (*symbol)[i] = '\0';
}

void GenSymbol(char **symbol, int n)
{
    int i, value;
    if (symbol == NULL)
        NoMallocErr("GenSymbols 1");

    if ((*symbol) == NULL)
    {
        (*symbol) = calloc(n, sizeof(char));
        if ((*symbol) == NULL)
            MallocErr("GenSymbol 2");
    }

    for (i = 0; i < n - 1; i++)
    {   value = rand() % 26;
        value += 65;
        (*symbol)[i] = value;
    }

    (*symbol)[n-1] = '\0';
}

void GenSymbolArray(char ***symbols, unsigned long int n, unsigned int m)
{   
    /*  Generate an array of n strings
     *
     *  Not sure if the m argument (arg 2) is useful. Supposedly it is 
     *  the max # of chars per string, but if its too small it gets set 
     *  to the min amount
     */

    unsigned long int i, j;
    //unsigned long int m;
    char *symbolStore = NULL;
    char *buffer = NULL;
    unsigned long int min_m = (unsigned int) ceil(log2l(n) / 3);

    if (m < min_m)
        m = min_m;

    if (symbols == NULL)
        NoMallocErr("GenSymbolArray");

    symbolStore = calloc((m + 1) * n, sizeof(char));
    (*symbols) = calloc(n, sizeof(char *));
    //buffer = calloc(n, sizeof(char));

    if ((*symbols) == NULL)
        MallocErr("GenSymbolArray 2");
    if (symbolStore == NULL)
        MallocErr("GenSymbolArray 3");
    //if (buffer == NULL)
    //    MallocErr("GenSymbolArray 4");

    for (i = 0; i < n; i++)
    {   (*symbols)[i] = &(symbolStore[i * (m + 1)]);
        GenSequentialSymbol(&((*symbols)[i]), i+1, m);
    }
}

//void GenArgArray(char ***args, unsigned long int n, unsigned long int M)
//{
//
//}

void FreeSymbolArray(char ***symbols, unsigned long int n, unsigned int m)
{
    unsigned long int i, j;

    if (symbols != NULL)
    {
        free(**symbols);
        free(*symbols);

        **symbols = NULL;
        *symbols = NULL;
    }
}

void GenSkewedTree(const char **symbols, const char **args, int m, int n, PrpsTree **tree, int d)
{

    int i, j, index;
    const char *ptr = NULL, *arg = NULL;
    long double nmbr;
    PrpsTree **left = NULL, **right = NULL;
    //if (tree == NULL)

    if ((*tree) == NULL)
    {   (*tree) = GenerateEmpty();
        index = rand() % 4;
        ptr = symbols[index];

        if (d > 0)
        {
            index = rand() % 4;
        	AllocateAsOprtr(tree, index);

            left = calloc(1, sizeof(PrpsTree *));
            right = calloc(1, sizeof(PrpsTree *));

            if (!left || !right)
                MallocErr("GenSkewedTree 1");

            nmbr = (long double) rand();
            nmbr = (d - 1) * ((int) rintl(logl(nmbr) / logl((long double) RAND_MAX)));

            GenSkewedTree(symbols, args, m, n, left, d - 1);
            GenSkewedTree(symbols, args, m, n, right, d - 1 - nmbr);

            SetLeftChild(tree, left);
            SetRightChild(tree, right);

            free(left);
            free(right);

            left = NULL;
            right = NULL;
        }

        else
        {   
            j = rand() % m;
            ptr = symbols[j];
            j = rand() % n;
            arg = args[j];

            AllocateAsPrps(tree, ptr, arg);

            //for (i = 0; i < d; i++)
            //{   
            //    tree = Oprtr2
            //           (rand() % 3, tree,
            //            rand() % 3, ptr,
            //            rand() % 3, rand() % 4);
            //}
        }

        if (rand() > RAND_MAX / 2)
            Negate(tree);
    }
}

void GenAlphaTree(const char **symbols, const char **args, PrpsTree ***alpha, int m, int n, int d)
{
    int i, j;
    const char *symbol_tmp = NULL, *arg_tmp;
    //PrpsTree **alpha = NULL;

    if (alpha == NULL)
        NoMallocErr("GenAlphaTree 1");

    if ((*alpha) != NULL)
        InconsistencyErr("GenAlphaTree 2");

    (*alpha) = calloc(1, sizeof(PrpsTree *));
    if ((*alpha) == NULL)
        MallocErr("GenAlphaTree 3");
    (**alpha) = GenerateEmpty();

    i = rand() % m;
    symbol_tmp = symbols[i];

    i = rand() % n;
    arg_tmp = args[i];

    AllocateAsPrps(*alpha, symbol_tmp, arg_tmp);
}

void FillHashTable(HashTable **hash, char **symbols, char **args, int m, int n, int seed)
{
    int i, j, value, value_retrieved;
    int a, b;
    int iterations;

    srand(seed);

    for (i = 0; i < m*50; i++)
    {
        iterations = 50 * n; //rand() % n;

        for (j = 0; j < iterations; j++)
        {
            value = rand() % 2; 
            a = rand() % m;
            b = rand() % n;

            SetValueForArg(symbols[a], args[b], value, hash, m, n);

            value_retrieved = GetValueForArg(symbols[a], args[b], hash, m, n);

            if (value_retrieved != value)
                InconsistencyErr("FillHashTable 1: Unit Test! ");
        }
    }
}

void AddStmnt(FindList **list, char *prps, char *arg, int neg)
{
    /*  Adds the literal, denote by prps, arg, and neg, to list, a 
     *  Linked List of literals in the clause
     */
    FindList **head = NULL;
    int return_val = 0;

    head = list;
    while (head != NULL && (*head) != NULL)
    {
        if (strcmp((*head) -> prps, prps) == 0 && strcmp((*head) -> arg, arg) == 0)
        {
            if (neg == 0)
                (*list) -> is += 1;
            else if (neg == 1)
                (*list) -> is_not += 1;
            break;
        }

        head = &((*head) -> next);
    }

    if ((*head) == NULL)
    {
        (*head) = calloc(1, sizeof(FindList));
        if ((*head) == NULL)
            MallocErr("AddStmnt 1");

        (*head) -> is = 0;
        (*head) -> is_not = 0;

        if (neg == 0)
            (*head) -> is = 1;
        else if (neg == 1)
            (*head) -> is_not = 1;

        (*head) -> prps = calloc(2, sizeof(char));
        (*head) -> arg = calloc(2, sizeof(char));
        if ((*head) -> prps == NULL || (*head) -> arg == NULL)
            MallocErr("AddSmnt 2");

        ((*head) -> prps)[0] = prps[0];
        ((*head) -> arg)[0] = arg[0];
        ((*head) -> prps)[1] = '\0';
        ((*head) -> arg)[1] = '\0';
    }
}

void DepthFirstTraversal(PrpsTree **clause, FindList **list)
{
    /*
     */
    PrpsTree **left, **right;
    
    if (clause != NULL)
    {   if ((*clause) != NULL)
        {   if (OprtrNodeType(clause, IS, OR))
            {   left = &((*clause) -> left);
                right = &((*clause) -> right);

                DepthFirstTraversal(left, list);
                DepthFirstTraversal(right, list);
            }
            else if (PrpsNode(clause))
                AddStmnt(
                    list, 
                    (*clause) -> stmnt -> stc, 
                    (*clause) -> argmnt -> stc, 
                    *((*clause) -> neg));
        }
    }
}

int DefaultsToTrue(FindList **list)
{
    int return_val = 0;

    FindList **head = list;

    if (list != NULL)
        while ((*head) != NULL)
        {
            if ((*list) -> is && (*list) -> is_not)
            {
                return_val = 1;
                break;
            }

            head = &((*head) -> next);
        }

    return return_val;
}

/*FindList ***GetLiterals(PrpsTree **tree, FindList ***clause_array, int *k)
{
    FindList **list = NULL, **head = NULL;
    int left, right;
    FindList **return_val;

    if (OprtrNodeType(tree, IS, AND))
    {
        GetLiterals(&((*tree) -> left), clause_array, k);
        GetLiterals(&((*tree) -> right), clause_array, k);
    }

    if (OprtrNodeType(tree, IS, OR))
    {
        list = calloc(1, sizeof(FindList *));
        if (list == NULL)
            MallocErr("GetLiterals 0");

        DepthFirstTraversal(tree, list);
        return_val = DefaultsToTrue(list);
    }

    else
        InconsistencyErr("GetLiterals 1");

    return list;
}*/

/*int CountLiterals(PrpsTree **tree)
{
    int return_val = 0;

    if (OprtrNode(tree))
    {
        return_val += CountLiterals(&((*tree) -> left));
        return_val += CountLiterals(&((*tree) -> right));
    }
    else if (PrpsNode(tree))
        return_val = 1;

    return return_val;
}*/

//void AddLiterals(PrpsTree **sub_tree, ClauseList **head, int *index)
//{
    /*  Adds all the symbols of all literals in the clause which 
     *  PrpsTree **sub_tree is a subtree of, to the clause_array 
     *  array of head
     */

/*    char neg = ' ';
    if (OprtrNodeType(sub_tree, IS, OR))
    {
        AddLiterals(&((*sub_tree) -> left), head, index);
        AddLiterals(&((*sub_tree) -> right), head, index);
    }

    else if (PrpsNode(sub_tree))
    {
        ((*head) -> clause_array)[*index] = calloc(3, sizeof(char *));

        if (((*head) -> clause_array)[*index] == NULL)
            MallocErr("AddLiterals 1");

        if (*((*sub_tree) -> neg) == 1)
            neg = '~';

        ((*head) -> clause_array)[*index][0] = calloc(2, sizeof(char));
        if (((*head) -> clause_array)[*index][0] == NULL)
            MallocErr("AddLiterals 2");

        ((*head) -> clause_array)[*index][0][0] = neg;
        ((*head) -> clause_array)[*index][0][1] = '\0';
        ((*head) -> clause_array)[*index][1] = (*sub_tree) -> stmnt -> stc;
        ((*head) -> clause_array)[*index][2] = (*sub_tree) -> argmnt -> stc;
        *index += 1;
    }
}*/

void GetClauses(PrpsTree **tree, ClauseList **head, ClauseList **tail, int *index, int num_literals)
{
    /*
        Produces a linked list of subtrees, each of which is a clause
        The ClauseList ** is the linked list
    */

    int inc_array = 0;
    ClauseList **tmp = NULL;
    if (OprtrNode(tree) || PrpsNode(tree))
    {
        if (OprtrNodeType(tree, IS, AND))
        {   GetClauses(&((*tree) -> left), head, tail, index, num_literals);
            GetClauses(&((*tree) -> right), head, tail, index, num_literals);
        }

        else if (OprtrNodeType(tree, IS, OR) || PrpsNode(tree))
        {
            if ((*head) == NULL)
            {
                (*head) = calloc(1, sizeof(ClauseList));
                if ((*head) == NULL)
                    MallocErr("GetClauses 2");
                (*tail) = (*head);
            }

            else
            {
                (*head) -> next = calloc(1, sizeof(ClauseList));
                if ((*head) -> next == NULL)
                    MallocErr("GetClauses 3");
                (*head) = (*head) -> next;
            }

            //num_literals = CountLiterals(tree);
            ((*head) -> clause) = calloc(1, sizeof(FindList *));
            if ((*head) -> clause == NULL)
                MallocErr("GetClauses 4");
            DepthFirstTraversal(tree, (*head) -> clause);
            //AddLiterals(tree, head, index);
            //*index = 0;
        }

        else
            InconsistencyErr("GetClauses 5");
    }
}


/*void GetClauses(PrpsTree **tree, ClauseList **head, ClauseList **tail, int *index, int num_literals)
{

    int inc_array = 0;
    ClauseList **tmp = NULL;
    if (OprtrNode(tree) || PrpsNode(tree))
    {
        if (OprtrNodeType(tree, IS, AND))
        {   GetClauses(&((*tree) -> left), head, tail, index, num_literals);
            GetClauses(&((*tree) -> right), head, tail, index, num_literals);
        }

        else if (OprtrNodeType(tree, IS, OR) || PrpsNode(tree))
        {   
            if ((*head) == NULL)
            {   
                (*head) = calloc(1, sizeof(ClauseList));
                if ((*head) == NULL)
                    MallocErr("GetClauses 2");
                (*tail) = (*head);
            }

            else
            {
                (*head) -> next = calloc(1, sizeof(ClauseList));
                if ((*head) -> next == NULL)
                    MallocErr("GetClauses 3");
                (*head) = (*head) -> next;
            }

            num_literals = CountLiterals(tree);
            ((*head) -> clause_array) = calloc(num_literals, sizeof(char **));
            (*head) -> array_len = num_literals;
            AddLiterals(tree, head, index);
            *index = 0;
        }
        else
            InconsistencyErr("GetClauses 3");
    }
}*/


void print_clause_list(ClauseList **clause_list)
{
    int i;
    FindList **tmp = NULL;
    ClauseList **clause_list_head = NULL;
    tmp = calloc(1, sizeof(FindList *));
    clause_list_head = calloc(1, sizeof(ClauseList *));
    if (!tmp || !clause_list_head)
        MallocErr("print_clauses 1");

    (*clause_list_head) = (*clause_list);
    while ((*clause_list_head) != NULL)
    {   
        (*tmp) = *((*clause_list_head) -> clause);
        while (tmp != NULL && (*tmp) != NULL)
        {
            if ((*tmp) -> is > 0)
                printf("%c[%c]\t", ((*tmp) -> prps)[0], ((*tmp) -> arg)[0]);
            if ((*tmp) -> is_not > 0)
                printf("!%c[%c]\t", ((*tmp) -> prps)[0], ((*tmp) -> arg)[0]);

            (*tmp) = (*tmp) -> next;
        }

        printf("\n");

        (*clause_list_head) = (*clause_list_head) -> next;
    }
}

int PL_Resolve(FindList **Pa, FindList **Pb, ClauseList **ptr_head)
    /*  Returns the resolution of clauses Pa and Pb
     *  If the result is an empty clause, return_val
     *  will be null.
     */
{
    FindList **Pa_head = NULL, **Pb_head = NULL;
    FindList **P_new = NULL;
    int complimentary, equal_literals, different_literals;
    int had_complimentary = 0, num_passes;
    int return_val = 0, done = 0;

    Pa_head = calloc(1, sizeof(FindList *));
    Pb_head = calloc(1, sizeof(FindList *));
    P_new = calloc(1, sizeof(FindList *));
    if (!Pa_head || !Pb_head || !P_new)
        MallocErr("PL_Resolve 1");

    //return_val = calloc(1, sizeof(FindList));
    //if (return_val == NULL)
    //    MallocErr("PL_Resolve 2");

    num_passes = 0;
    (*Pa_head) = (*Pa); (*Pb_head) = (*Pb);
    while ((*Pa_head) != NULL)
    {
        while ((*Pb_head) != NULL)
        {
            // Test that prps and arg are equal
            equal_literals = 

                ((*Pa_head) -> prps)[0] == ((*Pb_head) -> prps)[0] &&
                ((*Pb_head) -> arg)[0] == ((*Pb_head) -> arg)[0];

            // Test for complimentary literals across the two clauses
            complimentary = 

                (   ((*Pb_head) -> is > 0 && (*Pb_head) -> is_not == 0)    &&
                    ((*Pa_head) -> is == 0 && (*Pa_head) -> is_not > 0)
                )
                    ||

                (   ((*Pa_head) -> is > 0 && (*Pa_head) -> is_not == 0)    &&
                    ((*Pb_head) -> is == 0 && (*Pb_head) -> is_not > 0)
                );

            // Test that Pa and Pb are non-identical clauses
            different_literals = 

                (((*Pb_head) -> prps)[0] != ((*Pa_head) -> prps)[0]     || 
                ((*Pb_head) -> arg)[0] != ((*Pb_head) -> arg)[0])       ||

                (((*Pb_head) -> prps)[0] == ((*Pa_head) -> prps)[0]     ||
                ((*Pb_head) -> arg)[0] == ((*Pa_head) -> arg)[0]);

            /*if (equal_literals && complimentary)
            {
                return_val = 1;
                done = 1;
                break;
            }*/

            if ((equal_literals && complimentary) || different_literals)
            {
                num_passes++;

                if (equal_literals && complimentary)
                    had_complimentary = 1;

                if ((*Pa_head) -> is > 0 && (*Pa_head) -> is_not > 0)
                    InconsistencyErr("PL_Resolve 3");
                if ((*Pa_head) -> is > 0)
                    AddStmnt(P_new, (*Pa_head) -> prps, (*Pa_head) -> arg, IS);
                if ((*Pa_head) -> is_not > 0)
                    AddStmnt(P_new, (*Pa_head) -> prps, (*Pa_head) -> arg, NOT);


                if ((*Pb_head) -> is > 0 && (*Pb_head) -> is_not > 0)
                    InconsistencyErr("PL_Resolve 4");
                if ((*Pb_head) -> is > 0)
                    AddStmnt(P_new, (*Pb_head) -> prps, (*Pb_head) -> arg, IS);
                if ((*Pb_head) -> is_not > 0)
                    AddStmnt(P_new, (*Pb_head) -> prps, (*Pb_head) -> arg, NOT);
            }

            (*Pb_head) = (*Pb_head) -> next;
        }

        (*Pa_head) = (*Pa_head) -> next;

        if ((*Pa_head) == NULL)
            break;
        (*Pb_head) = (*Pa_head) -> next;
    }

    if (equal_literals && complimentary && num_passes == 1)
    {
        free(P_new);
        P_new = NULL;
        return_val = 1;
    }

    else if (num_passes > 0)
    {
        (*ptr_head) = calloc(1, sizeof(ClauseList));
        if (!(*ptr_head))
            MallocErr("PL_Resolve 4");

        (*ptr_head) -> clause = P_new;
        (*ptr_head) = (*ptr_head) -> next;
    }

    return return_val;
}

int Resolve(ClauseList **clause_list)
{
    /* Resolves the clauses in clause_list.
     */

    int return_val = 0;
    int done = 0;
    int contradictory = 0;

    ClauseList **ptr_tail = NULL, **ptr_head = NULL, **ptr_mid = NULL;

    //  Clause pairs to be resolved
    ptr_tail = calloc(1, sizeof(ClauseList *));
    ptr_mid = calloc(1, sizeof(ClauseList *));

    //  Where new clauses are put
    ptr_head = calloc(1, sizeof(ClauseList *)); 

    if (!ptr_tail || !ptr_head || !ptr_mid)
        MallocErr("Resolve 1");

    *ptr_tail = *clause_list;
    *ptr_head = *clause_list;

    //  Birng head to head of clause list
    while ((*ptr_head) != NULL)
        (*ptr_head) = (*ptr_head) -> next;

    //  Makre sure there's more than one clause
    if ((*ptr_tail) != NULL)
    {
        (*ptr_mid) = (*ptr_tail) -> next;

        // If only one clause in statement
        // Though this will never happen...
        if ((*ptr_mid) == NULL)
            // DefaultsToTrue is NEGATED here
            return_val = !DefaultsToTrue((*ptr_tail) -> clause);

        else
            while ((*ptr_tail) != NULL && (*ptr_tail) -> next != NULL && !done)
            {
                (*ptr_mid) = (*ptr_tail) -> next;
                while ((*ptr_mid) != NULL)
                {
                    contradictory = PL_Resolve((*ptr_tail) -> clause, (*ptr_mid) -> clause, ptr_head);

                    if (contradictory)
                    {
                        return_val = 1;     //  Inference takes place
                        done = 1;
                        break;
                    }
                    else if ((*ptr_head) != NULL)
                        (*ptr_head) = (*ptr_head) -> next;

                    (*ptr_mid) = (*ptr_mid) -> next;
                }

                (*ptr_tail) = (*ptr_tail) -> next;
                //if ((*ptr_tail) == NULL)
                //    break;
                (*ptr_mid) = (*ptr_tail) -> next;
            }
    }

    return return_val;
}

void ElliminateClause(FindList **clause)
{
    if ((*clause) -> next != NULL)
        ElliminateClause(&((*clause) -> next));
    free((*clause) -> prps); free((*clause) -> arg);
    (*clause) -> prps = NULL; (*clause) -> arg = NULL;
    (*clause) -> next = NULL;
    free((*clause)); (*clause) = NULL;
}

int ElliminateDefaultTrues(ClauseList **clause_list)
{
    ClauseList **clause_ptr = NULL, **clause_ptr_drag;
    ClauseList **clause_ptr_tmp = NULL;
    FindList **literal_ptr = NULL;

    clause_ptr = calloc(1, sizeof(ClauseList *));
    clause_ptr_drag = calloc(1, sizeof(ClauseList *));
    clause_ptr_tmp = calloc(1, sizeof(ClauseList *));
    literal_ptr = calloc(1, sizeof(FindList *));

    if (!clause_ptr || !clause_ptr_drag || !clause_ptr_tmp || !literal_ptr)
        MallocErr("ElliminateDefaultTrues 1");

    (*clause_ptr) = (*clause_list);
    (*literal_ptr) = *((*clause_ptr) -> clause);
    
    while ((*clause_ptr) != NULL)
    {
        (*literal_ptr) = *((*clause_ptr) -> clause);
        while ((*literal_ptr) != NULL)
        {
            if ((*literal_ptr) -> is > 0 && (*literal_ptr) -> is_not > 0)
            {
                ElliminateClause((*clause_ptr) -> clause);
                if ((*clause_ptr) -> prev == NULL)
                {
                    free(clause_list);
                    clause_list = &((*clause_ptr) -> next);
                    (*clause_ptr) -> next = NULL;
                }

                else
                {
                    (*clause_ptr) -> prev -> next = (*clause_ptr) -> next;
                    (*clause_ptr) -> next = (*clause_ptr) -> next -> prev;
                    (*clause_ptr_tmp) = (*clause_ptr);
                    (*clause_ptr) = (*clause_ptr) -> next;
                    free(*clause_ptr_tmp);
                    (*literal_ptr) = *((*clause_ptr) -> clause);
                    continue;
                }
            }

            (*literal_ptr) = (*literal_ptr) -> next;
        }

        (*clause_ptr) = (*clause_ptr) -> next;
    }
}

int Resolution(PrpsTree **tree)
{
    /*

     */

    int return_val = 0, index = 0, num_literals = -1;
    ClauseList **clause_list_ptr = NULL;
    ClauseList **tail_ptr = NULL;

    clause_list_ptr = calloc(1, sizeof(ClauseList *));
    tail_ptr = calloc(1, sizeof(ClauseList *));
    if (clause_list_ptr == NULL || tail_ptr == NULL)
        MallocErr("PL_Resolution 1");

    GetClauses(tree, clause_list_ptr, tail_ptr, &index, num_literals);
    (*clause_list_ptr) = (*tail_ptr);

    ElliminateDefaultTrues(clause_list_ptr);

    return_val = Resolve(tail_ptr);
    printf("Clauses:\n");
    print_clause_list(clause_list_ptr);

    return return_val;
}
