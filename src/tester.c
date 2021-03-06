#include "tester.h"
#include <stdarg.h>

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

    if ((*symbols) == NULL)
        MallocErr("GenSymbolArray 2");
    if (symbolStore == NULL)
        MallocErr("GenSymbolArray 3");

    for (i = 0; i < n; i++)
    {   (*symbols)[i] = &(symbolStore[i * (m + 1)]);
        GenSequentialSymbol(&((*symbols)[i]), i+1, m);
    }
}

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

char **CopyArgs(PrpsTree **node, int *n_args)
{
    char **args = NULL;
    int i;
    if (node != NULL && (*node) != NULL)
    {
        if ((*node) -> argmnt != NULL)
        {
            (*n_args) = (*node) -> n_args;
            args = calloc(*n_args, sizeof(char *));
            if (args == NULL)
                MallocErr("CopyNode 1");
            
            for (i = 0; i < *n_args; i++)
            {   args[i] = calloc(
                    strlen(((*node) -> argmnt)[i] -> stc)+1, sizeof(char));
                if (args[i] == NULL)
                    MallocErr("CopyArgs 1");
                strcpy(args[i], ((*node) -> argmnt)[i] -> stc);
            }
        }
    }

    else
        InconsistencyErr("CopyArgs 1");

    return args;
}

char *GenPrps(char *input, int *i)
{
    char *return_val = NULL;
    int j, n_chars;

    if (IsUpperCase(input[*i]))
    {
        j = *i;
        do
        {   j++;
        } while (IsLowerCase(input[j]));

        n_chars = j - *i;

        return_val = calloc(n_chars, sizeof(char));
        if (return_val == NULL)
            MallocErr("GenPrps 1");

        strncpy(return_val, &(input[*i]), n_chars * sizeof(char));

        *i += n_chars;
    }

    else
        InconsistencyErr("GenPrps 2");

    return return_val;
}

char **GenArgs(char *input, int *i)
{
    char **return_val = NULL;
    int ref = *i, j, k, n_args, *arg_sizes = NULL;

    n_args = 0;
    while (input[ref] != ']')
        if (input[ref++] == ',')
            n_args++;
    n_args++;

    ref = *i;
    arg_sizes = calloc(n_args, sizeof(int));
    return_val = calloc(n_args, sizeof(char *));
    if (arg_sizes == NULL || return_val == NULL)
        MallocErr("GenArgs 1");

    j = *i; k = 0;
    for (ref = 0; ref < n_args; ref++)
    {
        while (IsLowerCase(input[*i + k++]));

        arg_sizes[ref] = k;
        strncpy(return_val[ref], &(input[*i]), k * sizeof(char));
        while (!IsLowerCase(input[*i + k]) && input[*i + k] != ']')
            k++;

        *i = *i + k;
        k = 0;
    }
}

int CompareArgs(char **args_a, char **args_b, int n_args_a, int n_args_b)
{
    /*
        returns 1 if (char **) args_a and (char **) args_b 
        have the same arguments in the same order

        otherwise returns 0
    */

    int return_val = 1, i;

    if (n_args_a != n_args_b)
        return_val = 0;

    else
    {
        for (i = 0; i < n_args_a; i++)
        {
            if (strcmp((const char *) args_a[i],
                       (const char *) args_b[i]) != 0)
            {
                return_val = 0;
                break;
            }
        }
    }

    return return_val;
}

int Complimentary(FindList **clause, FindList **literal)
{
    FindList **clause_ptr = clause;
    char *c_prps = NULL, **c_arg = NULL, *l_prps, **l_arg = NULL;
    int c_len = 0, l_len = 0;
    int c_is, c_is_not, l_is, l_is_not;
    int c_n_args = 0, l_n_args = 0;
    int return_val = 0;

    l_prps = (**literal).prps;
    l_arg = (**literal).arg;
    l_is = (*literal) -> is;
    l_is_not = (*literal) -> is_not;
    l_n_args = (*clause) -> n_args;

    //while (clause_ptr != NULL && (*clause_ptr) != NULL)
    //{
    c_prps = (**clause).prps;
    c_arg = (**clause).arg;
    c_is = (*clause) -> is;
    c_is_not = (*clause) -> is_not;
    c_n_args = (*clause) -> n_args;

    // Check: equal prps && equal args, oposite negations
    if (strcmp(c_prps, l_prps) == 0)
        if (CompareArgs(c_arg, l_arg, c_n_args, l_n_args))
            if (c_is == !l_is && c_is_not == !l_is_not)
                return_val = 1;

    return return_val;
}

void AddStmnt(FindList **list, char *prps, char **arg, 
        int n_args, int is, int is_not)
{
    /*  Adds the literal, denote by prps, arg, and neg, to list, a 
     *  Linked List of literals in the clause
     */

    FindList **head = NULL, **tail = NULL;
    int prps_len = 0;

    head = list;
    if (head != NULL)
        while ((*head) != NULL)
        {
            if (strcmp((*head) -> prps, prps) == 0)
            {
                if (CompareArgs((*head) -> arg, arg, (*head) -> n_args, n_args))
                {
                    if ((*head) -> is == is && (*head) -> is_not == is_not)
                        break;
                    else
                    {
                        if (tail == NULL)
                        {
                            head = &((*head) -> next);
                            free(*list);
                            *list = *head;
                        }

                        else
                        {
                            head = &((*head) -> next);
                            free((*tail) -> next);
                            (*tail) -> next = *head; continue;
                        }
                    }
                }
            }

            tail = head;
            head = &((*head) -> next);
        }

    if ((*head) == NULL)
    {
        (*head) = calloc(1, sizeof(FindList));
        if ((*head) == NULL)
            MallocErr("AddStmnt 1");

        (*head) -> is = 0;
        (*head) -> is_not = 0;

        (*head) -> is = is;
        (*head) -> is_not = is_not;

        prps_len = strlen(prps) + 1;

        (*head) -> prps = calloc(prps_len, sizeof(char));
        (*head) -> arg = calloc(n_args, sizeof(char *));
        if ((*head) -> prps == NULL || (*head) -> arg == NULL)
            MallocErr("AddSmnt 2");

        strcpy(((**head).prps), prps);
        (*head) -> arg = arg;
        (*head) -> n_args = n_args;
    }
}

void DepthFirstTraversal(PrpsTree **clause, FindList **list)
{
    /*
     */
    PrpsTree **left, **right;
    int is, is_not, n_args;
    char **args = NULL;
    
    if (clause != NULL)
    {   if ((*clause) != NULL)
        {   if (OprtrNodeType(clause, IS, OR))
            {   left = &((*clause) -> left);
                right = &((*clause) -> right);

                DepthFirstTraversal(left, list);
                DepthFirstTraversal(right, list);
            }

            else if (PrpsNode(clause))
            {
                args = CopyArgs(clause, &n_args);
                is = !(*((*clause) -> neg));
                is_not = *((*clause) -> neg);
                AddStmnt(list, (*clause) -> stmnt -> stc, args, n_args, is, is_not);
            }
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

void GetClauses(PrpsTree **tree, ClauseList **head, ClauseList **tail, 
        int *index, int num_literals)
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

int IdenticalLiterals(int exactness, FindList **Pa, FindList **Pb)
{
    int return_val = 0;
    char *a_prps, *b_prps, **a_arg = NULL, **b_arg = NULL;
    int a_is, b_is, a_is_not, b_is_not;
    int a_n_args = 0, b_n_args = 0;

    a_prps = ((*Pa) -> prps);
    a_arg = (**Pa).arg;
    a_is = ((*Pa) -> is);
    a_is_not = ((*Pa) -> is_not);
    a_n_args = (**Pa).n_args;

    b_prps = ((*Pb) -> prps);
    b_arg = (**Pb).arg;
    b_is = (*Pb) -> is;
    b_is_not = (*Pb) -> is_not;
    b_n_args = (**Pb).n_args;


    if (strcmp(b_prps, a_prps) == 0)
        if (CompareArgs(a_arg, b_arg, a_n_args, b_n_args))
        {   if (exactness)
            {   if (b_is == a_is && b_is_not == a_is_not)
                    return_val = 1;
            }

            else
                return_val = 1;
        }

    return return_val;
}

int FindIdenticalLiteral(int exactness, FindList **C_new, FindList **clause_ptr)
{
    FindList **C_ptr = C_new;
    int return_val = 0;

    while (C_ptr != NULL && (*C_ptr) != NULL)
    {
        return_val = IdenticalLiterals(exactness, C_ptr, clause_ptr);
        if (return_val == 1)
            break;

        C_ptr = &((*C_ptr) -> next);
    }

    return return_val;
}

int EqualClauses(int exactness, FindList **C_new, FindList **clause)
{
    int return_val = 1;

    FindList **C_new_ptr = C_new;
    FindList **clause_ptr = clause;
    while (clause_ptr != NULL && (*clause_ptr) != NULL)
    {
        return_val = FindIdenticalLiteral(exactness, C_new, clause_ptr);
        if (!return_val)
            break;

        clause_ptr = &((*clause_ptr) -> next);
    }

    C_new_ptr = C_new;
    clause_ptr = clause;
    if (return_val)
    {
        while (C_new_ptr != NULL && (*C_new_ptr) != NULL)
        {
            return_val = FindIdenticalLiteral(exactness, clause, C_new_ptr);
            if (!return_val)
                break;

            C_new_ptr = &((*C_new_ptr) -> next);
        }
    }

    return return_val;
}

int ContainsClause(char type, int exactness, ...)
{
    /*  Arguments are as follows:
     *      first argument is (char) type
     *      second is (int) exactness
     *          exactness == 0 means ignore negations in front of literals
     *          exactness == 1 means literals have to have same negations to be deemed equal
     *          
     *      third is (FindList **) P_new
     *          This represents an actual clause, not just a single literal
     *      fourth:
     *          if type == 'l' (ClauseList **) Q_nmp_ptr
     *              Checks to see if clause represented by third arg is equivalent to clause 
     *              represented by fourth arg
     *          or a (FindList **)
     *              Checks to see if literal represented by third arg is contained in any 
     *              of clauses held in fourth arg
     */

    int return_val = 0;
    va_list argp;
    FindList **P_new = NULL, **P_new_ptr = NULL;
    FindList **P_nmp_ptr = NULL, **P_nmp = NULL;
    ClauseList **Q_nmp_ptr = NULL, **Q_nmp = NULL;
    va_start(argp, exactness);

    // THIRD ARG
    P_new = va_arg(argp, FindList **);

    switch (type)
    {
        // Checks if equivalent of clause P_new is contained in clause list Q_nmp_ptr
        case 'c':

            // FOURTH ARG
            Q_nmp_ptr = va_arg(argp, ClauseList **);
            while (Q_nmp_ptr != NULL && (*Q_nmp_ptr) != NULL && (*Q_nmp_ptr) -> clause != NULL)
            {
                return_val = ContainsClause('e', exactness, (*Q_nmp_ptr) -> clause, P_new);
                if (return_val)
                    break;
                Q_nmp_ptr = &((*Q_nmp_ptr) -> next);
            }

            break;

        // Checks if clause P_new is equivalent to clause P_nmp_ptr
        case 'e':

            // FOURTH ARG
            P_nmp_ptr = va_arg(argp, FindList **);
            while (P_nmp_ptr != NULL && (*P_nmp_ptr) != NULL)
            {
                return_val = EqualClauses(exactness, P_nmp_ptr, P_new);
                if (return_val)
                    break;
                P_nmp_ptr = &((*P_nmp_ptr) -> next);
            }

            break;

        // Checks if exact literals in P_nmp_ptr are in P_new, regardless or order
        case 'l':

            P_nmp_ptr = va_arg(argp, FindList **);
            while (P_nmp_ptr != NULL && (*P_nmp_ptr) != NULL)
            {
                return_val = IdenticalLiterals(exactness, P_new, P_nmp_ptr);
                if (return_val == 1)
                    break;
                P_nmp_ptr = &((*P_nmp_ptr) -> next);
            }

            break;
    }

    return return_val;
}

void print_clause_list(ClauseList **clause_list, int a, int b)
{
    int i, x = 0, n_args = 0;
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
        (**tmp).n_args = (**((*clause_list_head) -> clause)).n_args; 
        if (x == a || x == b)
            printf("X");
        while (tmp != NULL && (*tmp) != NULL)
        {
            n_args = (**tmp).n_args;
            if ((*tmp) -> is > 0)
            {
                printf("\t%s[", ((*tmp) -> prps));
                for (i = 0; i < (**tmp).n_args - 1; i++)
                    printf("%s, ", ((*tmp) -> arg)[i]);
                printf("%s]\t", ((*tmp) -> arg)[n_args - 1]);
            }

            if ((*tmp) -> is_not > 0)
            {
                printf("\t!%s[", ((*tmp) -> prps));
                for (i = 0; i < (**tmp).n_args - 1; i++)
                    printf("%s, ", ((*tmp) -> arg)[i]);
                printf("%s]\t", ((*tmp) -> arg)[n_args - 1]);
            }

            (*tmp) = (*tmp) -> next;
        }

        printf("\n");

        (*clause_list_head) = (*clause_list_head) -> next;
        x++;
    }
}

int PL_Resolve_2(FindList **Pa, FindList **Pb, ClauseList ***ptr_head, ClauseList **clause_list, int a, int b, int *added)
{
    /*  returns 0 if not contradictory
     *  returns 1 if contradictory
     */

    int return_val = 0, passes_added = 0, passes_tried = 0, contains_clause;
    int is_complimentary, complimentary_clause = 0;
    int is, is_not;
    int contained;
    char **args = NULL;
    FindList **Pa_head = NULL, **Pb_head = NULL;
    FindList **C_cmp = NULL, **C_new = NULL;
    FindList **C_cmp_ptr = NULL;
    ClauseList **clause_ptr = NULL;

    *added = 0;

    C_cmp = calloc(1, sizeof(FindList *));
    Pa_head = Pa; Pb_head = Pb;

    while ((*Pa_head) != NULL)
    {
        Pb_head = Pb;
        while ((*Pb_head) != NULL)
        {
            is_complimentary = Complimentary(Pb_head, Pa_head);
            if (is_complimentary)
            {
                complimentary_clause = 1;
                is = (*Pa_head) -> is; is_not = (*Pa_head) -> is_not;
                AddStmnt(C_cmp, (*Pa_head) -> prps, (*Pa_head) -> arg, 
                    (*Pa_head) -> n_args, -1, -1);
                passes_added++;
            }

            passes_tried++;
            Pb_head = &((*Pb_head) -> next);
        }

        Pa_head = &((*Pa_head) -> next);
    }

    if (complimentary_clause == 1 && passes_added == passes_tried)
        return_val = 1; // IS Contradictory

    else if (complimentary_clause == 1)
    {
        Pa_head = Pa; Pb_head = Pb;
        C_new = calloc(1, sizeof(FindList *));
        C_cmp_ptr = C_cmp;
        if (C_new == NULL)
            MallocErr("PL_Resolve_2 3");

        while ((*Pa_head) != NULL)
        {
            contained = ContainsClause('l', 0, Pa_head, C_cmp);
            if (!contained)
            {
                is = (*Pa_head) -> is; is_not = (*Pa_head) -> is_not;
                AddStmnt(C_new, (*Pa_head) -> prps, (*Pa_head) -> arg, 
                    (*Pa_head) -> n_args, is, is_not);
            }

            Pa_head = &((*Pa_head) -> next);
        }

        while ((*Pb_head) != NULL)
        {
            contained = ContainsClause('l', 0, Pb_head, C_cmp);
            if (!contained)
            {
                is = (*Pb_head) -> is; is_not = (*Pb_head) -> is_not;
                AddStmnt(C_new, (*Pb_head) -> prps, (*Pb_head) -> arg, 
                    (*Pb_head) -> n_args, is, is_not);
            }

            Pb_head = &((*Pb_head) -> next);
        }

        clause_ptr = clause_list;

        while (clause_ptr != NULL && (*clause_ptr) != NULL && (*clause_ptr) -> clause != NULL)
        {
            contained = EqualClauses(1, C_new, (*clause_ptr) -> clause);
            if (contained)
                break;

            clause_ptr = &((*clause_ptr) -> next);
        }

        if (!contained)
        {
            (**ptr_head) -> next = calloc(1, sizeof(ClauseList *));
            if ((**ptr_head) -> next == NULL)
                MallocErr("PL_Resolves_2 4");

            (**ptr_head) -> next -> prev = (**ptr_head);
            (*ptr_head) = &((**ptr_head) -> next);

            (**ptr_head) -> clause = C_new;
            *added = 1;
        }
    }

    return return_val;
}

int PL_Resolve(FindList **Pa, FindList **Pb, ClauseList ***ptr_head)
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
    int is = 0, is_not = 0;

    P_new = calloc(1, sizeof(FindList *));
    if (!P_new)
        MallocErr("PL_Resolve 1");

    num_passes = 0;
    Pa_head = Pa; Pb_head = Pb;
    while ((*Pa_head) != NULL)
    {
        while ((*Pb_head) != NULL)
        {
            // Test that prps and arg are equal
            if ((*Pa_head) == NULL)
            {
                equal_literals = 0;
                complimentary = 0;
                different_literals = 1;
            }

            else
            {
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
                    ((*Pb_head) -> arg)[0] != ((*Pa_head) -> arg)[0]);
            }

            if ((equal_literals && complimentary))
            {
                num_passes++;
                had_complimentary = 1;
                Pa_head = &((*Pa_head) -> next);
                Pb_head = Pb;
            }

            else if (different_literals || equal_literals)
            {
                num_passes++;
                had_complimentary = 0;

                if ((*Pa_head) != NULL && !equal_literals)
                {
                    if ((*Pa_head) -> is > 0 && (*Pa_head) -> is_not > 0)
                        InconsistencyErr("PL_Resolve 3");

                    is = (*Pa_head) -> is;
                    is_not = (*Pa_head) -> is_not;

                    AddStmnt(P_new, (*Pa_head) -> prps, (*Pa_head) -> arg,
                        (*Pa_head) -> n_args, is, is_not);
                }

                if ((*Pb_head) -> is > 0 && (*Pb_head) -> is_not > 0)
                    InconsistencyErr("PL_Resolve 4");

                is = (*Pb_head) -> is;
                is_not = (*Pb_head) -> is_not;

                AddStmnt(P_new, (*Pb_head) -> prps, (*Pb_head) -> arg,
                    (*Pb_head) -> n_args, is, is_not);

                Pb_head = &((*Pb_head) -> next);
            }
        }

        if ((*Pa_head) != NULL)
            Pa_head = &((*Pa_head) -> next);
        Pb_head = Pb;
    }

    if (equal_literals && complimentary && num_passes == 1)
    {
        free(P_new);
        P_new = NULL;
        return_val = 1;
    }

    else if (num_passes > 0)
    {
        (**ptr_head) = calloc(1, sizeof(ClauseList));
        if (!(**ptr_head))
            MallocErr("PL_Resolve 4");

        (**ptr_head) -> clause = P_new;
        (*ptr_head) = &((**ptr_head) -> next);
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
    int a = 0, b = 1;
    int added;

    ClauseList **ptr_tail = NULL, **ptr_head = NULL, **ptr_mid = NULL;

    //  Clause pairs to be resolved
    //ptr_tail = calloc(1, sizeof(ClauseList *));
    //ptr_mid = calloc(1, sizeof(ClauseList *));

    //  Where new clauses are put
    //ptr_head = calloc(1, sizeof(ClauseList *)); 

    //if (!ptr_tail || !ptr_head || !ptr_mid)
    //    MallocErr("Resolve 1");

    ptr_tail = clause_list;
    ptr_head = clause_list;

    //  Birng head to head of clause list
    while ((*ptr_head) != NULL && (*ptr_head) -> next != NULL)
    {   (*ptr_head) -> next -> prev = *ptr_head;
        ptr_head = &((*ptr_head) -> next);
    }

    //  Makre sure there's more than one clause
    if ((*ptr_tail) != NULL)
    {
        ptr_mid = &((*ptr_tail) -> next);

        // If only one clause in statement
        // Though this will never happen...
        if ((*ptr_mid) == NULL)
            // DefaultsToTrue is NEGATED here
            return_val = !DefaultsToTrue((*ptr_tail) -> clause);

        else
            while ((*ptr_tail) != NULL && (*ptr_tail) -> next != NULL && !done)
            {
                ptr_mid = &((*ptr_tail) -> next);
                while ((*ptr_mid) != NULL)
                {
                    contradictory = PL_Resolve_2((*ptr_tail) -> clause, 
                            (*ptr_mid) -> clause, &ptr_head, clause_list, a, b, &added);
                    printf("==============\n");
                    print_clause_list(clause_list, a, b);

                    if (contradictory)
                    {
                        return_val = 1;     //  Inference takes place
                        done = 1;
                        break;
                    }

                    ptr_mid = &((*ptr_mid) -> next);
                    b++;

                    if (added)
                    {
                        ptr_tail = clause_list;
                        a = 0;
                    }
                }

                ptr_tail = &((*ptr_tail) -> next);
                ptr_mid = &((*ptr_tail) -> next);
                a++; b = a+1;
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

    return_val = Resolve(clause_list_ptr);
    printf("Clauses:\n");
    print_clause_list(clause_list_ptr, -1, -1);

    return return_val;
}
