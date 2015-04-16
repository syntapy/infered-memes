#include <string.h>
#include "parse.h"
#include "prps.h"
// Requires string.h library !!! //


//***********************************//
//      S Y N T A X - R U L E S      //
//***********************************//
//
//      Forall: 4
//      Exists: 3
//
//      Arguments: 
//          Any number of chars: a - z
//              All Lower Case
//              Separated by commas
//          
//
//      Proposition functions:
//          1st char: A - Z
//              Upper Case
//          If any, any number of lower case letters: a - z
//          Left {
//          One Argument                     .  
//          Right}                                          
//                           .                     ***********
//      Operators:                                 ***********
//          and: ^          .   .                  ** **  * **
//          or: v              *                   **  *  * **
//          implies: ->                            *****  * **
//          equivalent: =     .        .           *****  * **
//                                                        * **
//                                                        * **
//                                                       ** **
//*************************************************************
//**************************************************************

int IsLowerCase(char c)
{   int return_val = FALSE;
    if ((int) c <= LOWER_CASE_LAST && (int) c >= LOWER_CASE_FIRST && c != 'v')
        return_val = TRUE;
    return return_val;
}

int IsUpperCase(char c)
{   int return_val = FALSE;
    if ((int) c <= UPPER_CASE_LAST && (int) c >= UPPER_CASE_FIRST)
        return_val = TRUE;
    return return_val;
}

int GetOprtr(char symbol)
{
    if (symbol == '^')
        return AND;
    else if (symbol == 'v')
        return OR;
    else if (symbol == '=')
        return TAU;
    else if (symbol == '>')
        return IMP;
    else return -1;
}

int IsOprtr(char token)
{
    int oprtr;
    int return_val = FALSE;

    oprtr = GetOprtr(token);

    if (oprtr == AND || oprtr == OR || oprtr == IMP || oprtr == TAU)
        return_val = TRUE;   

    return return_val;
}

#include "_read.h"

#include "_list.h"

#include "_tree_synth.h"

int IncrementConditional(Tokens **arg_list, Args **u_args_ptr, int depth)
{
    /*  THERE MIGHT BE A SUBTLE BUG IN THIS: Duplication of arguments
     *  
     *  This is a recursive function that increments the pointers in the
     *  stack of universally quantified arguments.
     *
     *  Alright, here's the deal: the pointers in the (Args **) u_args_ptr stack
     *  will, at different points in the skolemization process, point to
     *  a different combination of the arguments in the (Tokens **) arg_list
     *  linked list.
     *
     *  So, at the beginning of the skolemization process, all the pointers
     *  in the (Args **) u_args_ptr stack will point to the first node in 
     *  the (Tokens **) arg_list linked list.
     *  So, lets say (Args **) u_args_ptr has ["x", "y", "z"], three nodes
     *  in the stack. And suppose the arguments in (Tokens **) arg_list
     *  are ["cat", "dog", "chicken"].
     *
     *  Now, "x", "y", "z" are all nodes in the u_args_ptr, and they each
     *  have a pointer to one of the nodes in arg_list.
     *  At the beginning of the skolemization process, each node in u_args_ptr
     *  stack would point to "cat", and when the literal argument names are 
     *  generated then "x", "y", and "z" would become "cat".
     *  
     *  Run this IncrementConditional(...) once and then "z"'s pointer
     *  is pointing to "dog" instead, the rest unchanged.
     "  Run this again and "z"'s pointer points to "chicken"
     *  Run this again, "z"'s pointer goes back to "cat" but "y"'s pointer
     *  goes to "dog"
     *
     *  This function always returns 1, except when it is called AFTER
     *  the pointers of "x", "y", and "z" are all pointing to "chicken",
     *  meaninng that all combinations of u_args_ptr to arg_list pointings
     *  have been traversed.
     */

    int return_val = 1, next = 0;

    if ((*u_args_ptr) != NULL && (*u_args_ptr) -> token_ptr != NULL)
    {
        if ((**u_args_ptr).depth < depth)
        {
            return_val = 0;
            next = IncrementConditional(arg_list, &((*u_args_ptr) -> next), depth);

            return_val = return_val || next;
        }

        else
        {
            (*u_args_ptr) -> token_ptr = (*u_args_ptr) -> token_ptr -> next;

            if ((*u_args_ptr) -> token_ptr == NULL)
            {
                (*u_args_ptr) -> token_ptr = (*arg_list);
                return_val = 0;
                if ((*u_args_ptr) -> next != NULL)
                    next = IncrementConditional(arg_list, &((*u_args_ptr) -> next), depth);
                return_val = return_val || next;
            }
        }
    }

    else
        return_val = 0;

    return return_val;
}

void GetPrps(char **prps, char *input, int *i)
{
    int j = *i, s;
    if (prps == NULL)
        DeathErr("GetPrps 1");

    (*prps) = NULL;
    if (IsUpperCase(input[j]))
        j++;
    else
        InconsistencyErr("GetPrps 2");

    while (IsLowerCase(input[j]) || input[j] == 'v')
        j++; j--;

    s = j - *i + 1;
    *prps = calloc(s+1, sizeof(char));
    if ((*prps) == NULL)
        MallocErr("GetPrps 1.5");

    for (j = 0; j < s; j++)
        (*prps)[j] = input[*i + j];
    (*prps)[j] = '\0';
    *i += s;
}

void GetArg(char ***arg, char *input, int *i, int *arg_n)
{
    /*  Collects all the arguments of a prps function into an
     *  array of strings, and increments (int *) i to point 
     *  input[*i] to the next '{' or ']' character, whichever is 
     *  closer.
     *
     *  For instance:
     *      Loves[human, cat]
     *
     *  would allocate arg to have 2 (char *) elements, and one points to
     *  the "human" string, the other points to the "cat" string.
     */

    int j = *i, k, s, t;

    // Temp linked list that holds the arguments
    // This will be a memory leak source as its not deallocated
    ArgNmbr *tail = NULL, **head = &tail;

    if (arg == NULL)
        DeathErr("GetArg 1");

    *arg_n = 0; (*arg) = NULL;

    // Collect argument strings into linked list
    while (input[j] != ']' && input[j] != '{' && input[j] != '}')
    {
        k = 0;  // k is used get str length of current argument

        // Read past one argument
        while (input[j + k] != ',' && input[j + k] != ']' && input[j + k] != '{')
            k++;

        *arg_n += 1;
        *head = calloc(1, sizeof(ArgNmbr));
        if (*head == NULL)
            MallocErr("GetArg 1.25");

        (**head).s = k;

        // Read pasts comma and spaces
        while (input[j + k] != ']' && input[j + k] != '{' && input[j + k] != '}' && 
            (input[j + k] == ',' || input[j + k] == ' '))

            k++;

        (**head).t = k - (**head).s;
        head = &((*head) -> next);

        j += k; k = 0;
    }

    *arg = calloc(*arg_n, sizeof(char *));
    if ((*arg) == NULL)
        MallocErr("GetArg 1.5");


    // Collect arguments from linked list into (char ***) arg array of strings
    head = &tail;
    for (j = 0; j < *arg_n; j++)
    {
        s = (**head).s;
        t = (**head).t;
        (*arg)[j] = calloc(s+1, sizeof(char));
        if ((*arg)[j] == NULL)
            MallocErr("GetArg 2");

        for (k = 0; k < s; k++)
            (*arg)[j][k] = input[*i + k];

        (*arg)[j][k] = '\0';
        for (k = s; k < s + t; k++);

        *i += k;
        
        head = &((*head) -> next);
    }
}

void FreeTopOprtrArg(OprtrArgs **oprtr_tail, OprtrArgs ***oprtr_head)
{
    /*  
     */

    OprtrArgs **oprtr_tmp = NULL;

    if (*oprtr_head == NULL)
        InconsistencyErr("FreeTopOprtrArg 1");

    if (**oprtr_head != NULL)
    {
        FreeTokens(&((**oprtr_head) -> args));
        (**oprtr_head) -> args = NULL;
        (**oprtr_head) -> quant = '\0';

        if ((**oprtr_head) -> prev != NULL)
        {
            oprtr_tmp = calloc(1, sizeof(OprtrArgs *));
            if (oprtr_tmp == NULL)
                MallocErr("FreeTopOprtrArgs 1");

            (*oprtr_tmp) = (**oprtr_head) -> prev;
            free(**oprtr_head);
            **oprtr_head = NULL;
            free(*oprtr_head);
            *oprtr_head = NULL;
            (*oprtr_head) = oprtr_tmp;
            (**oprtr_head) -> next = NULL;
        }

        else
        {
            free(**oprtr_head);
            **oprtr_head = NULL;
        }
    }
}

void ReadArgs_Quantifier(Tokens **head, char ***arg, char *input, int *i, int *arg_n,
        OprtrArgs ***oprtr_head)
{
    /*  Each quantifier statement is formed as follows:
     *      4 animal, rock, car {statements...}
     *              or
     *      3 penquin, stallion, choochootrain {statements...}
     *
     *  Where there could be any number of lower case characters
     *  after the quantifier
     *
     *  This function changes the (int *) i pointer to point to the
     *  '{' character
     *  Meanwhile it has to keep track of the variable names it has encountered,
     *  placing them in the (OprtrArgs ***) oprtr_head linked list
     *
     *  ARGUMENTS:
     *
     *      (Tokens **) head: a linked list of the non-quantified argument strings.
     *          Each node contains a unique string corresponding to one such argument.
     *
     *      (char ***) arg: 
     *
     *      (int *) i: the pointer to the current index.
     *
     *      (int *) arg_n: this is set to the number of arguments
     *
     *      (Tokens **) oprtr_tail: the base of a linked list consisting of the
     *          quantified arguments. Each node 
     *
     *      (Tokens ***) oprtr_head is the head of the linked list containing
     */

    // quant is '3' or '4', denoting which quantifier
    int quant = input[*i], j;
    OprtrArgs **oprtr_tmp = NULL;
    while (input[*i] != '{')
    {
        if (IsLowerCase(input[*i]))
        {
            if (**oprtr_head == NULL)
            {
                (**oprtr_head) = calloc(1, sizeof(OprtrArgs));
                if ((**oprtr_head) == NULL)
                    MallocErr("ReadArgsQuantifier 1.5");
            }

            else
            {
                (***oprtr_head).next = calloc(1, sizeof(OprtrArgs));
                if ((***oprtr_head).next == NULL)
                    MallocErr("ReadArgsQuantifier 1.6");

                (*((***oprtr_head).next)).prev = (**oprtr_head);
                oprtr_tmp = calloc(1, sizeof(OprtrArgs *));
                if (oprtr_tmp == NULL)
                    MallocErr("ReadArgsQuantifier 1.7");

                (*oprtr_tmp) = (**oprtr_head) -> next;
                (*oprtr_head) = oprtr_tmp;
            }

            (***oprtr_head).quant = quant;

            // Read args and add them to list of existentiall 
            // quantified arguments
            GetArg(arg, input, i, arg_n);
            for (j = 0; j < *arg_n; j++)
            {
                E_AddArg(&((**oprtr_head) -> args), (*arg)[j], -1);
                free((*arg)[j]); (*arg)[j] = NULL;
            }
        }

        else
            (*i)++;
    }
}

void ReadArgs(Tokens **head, char ***arg, char *input, int *i, int n, char quantifier,
        OprtrArgs **oprtr_tail, OprtrArgs ***oprtr_head)
{
    /*  Parses the string (char *) input to fill a linked list, (Tokens **) head,
     *  with the arguments that do not have universal or existential quantification
     *  applied to them
     *
     *  This is a recursive function. The first call parses the entire line, while subsequent calls
     *  will parse (char *) input from one '{' to the corresponding '}'
     *
     *  ARGUMENTS:
     *
     *      (Tokens **) head: a linked list of the non-quantified argument strings.
     *          Each node contains a unique string corresponding to one such argument.
     *
     *      (char *) input: the string containing the entire sentence. 
     *
     *      (int *) i: the pointer to the current index.
     *
     *      (int) n: the length of (char *) input
     *
     *      (char) quantifier: depth in nested quantifiers
     *
     *      (Tokens **) oprtr_tail: the base of a linked list of linked lists.
     *          It consists of the quantified arguments corresponding to each
     *          quantifier statement. For instance, in the following statement:
     *              4 x, y, z{3 a, b {statements...}}
     *          (OprtrArgs **) oprtr_tail would become a two node list,
     *          the first node containing {"x", "y", "z"} and the second containing
     *          {"a", "b"}
     *      
     *      (Tokens ***) oprtr_head: the head of the linked list containing
     */

    int j = 0, arg_n = 0;
    char quant = '\0';
    Tokens **return_val = NULL;
    if (head == NULL)
        MallocErr("ReadArgs 1");

    while (*i < n)
    {
        // Reads quantified portion of the sentence
        if (input[*i] == '3' || input[*i] == '4')
        {
            ReadArgs_Quantifier(head, arg, input, i, &arg_n, oprtr_head);
            ReadArgs(head, arg, input, i, n, quantifier+1, oprtr_tail, oprtr_head);
            FreeTopOprtrArg(oprtr_tail, oprtr_head);
        }

        if (input[*i] == '}')
        {
            (*i)++;
            break;
        }

        // Moves past propositional function
        if (IsUpperCase(input[*i]))
            while (input[++(*i)] != '[');

        // Adds args, keeping track of whether
        // they are quantified arguments
        if (IsLowerCase(input[*i]))
        {
            GetArg(arg, input, i, &arg_n);
            
            if (quantifier > 0)
            {
                for (j = 0; j < arg_n; j++)
                    if (!OprtrContains(*oprtr_tail, (*arg)[j]))
                    {
                        E_AddArg(head, (*arg)[j], -1);
                        free((*arg)[j]); (*arg)[j] = NULL;
                    }
            }

            else if (quantifier == 0)
                for (j = 0; j < arg_n; j++)
                {
                    E_AddArg(head, (*arg)[j], -1);
                    free((*arg)[j]); (*arg)[j] = NULL;
                }

            (*i)++;
        }

        else if (input[*i] == '\0')
            break;

        else
            (*i)++;
    }
}

void Read(PrpsTree ***tree, int h, int k, int *hash_val)
{
    /* This reads PRPS.txt and ALPHA.txt and produces the binary tree
     * representing the logical sentence. It includes the use of 
     * skolemization to deal with universal operators, and a simpler
     * way of dealing with existential operators
     */

    char **kb_string = NULL;
    char **alpha_string = NULL;
    char *prps = NULL, *alpha = NULL, *mid = NULL, *prps_alpha = NULL;
    char **arg = NULL;
    int arg_n = 0;
    int m = 4, n;
    int nn = 0, mm = 0;
    int i = 0;

    char quantifier = '\0';
    char *a_prps = NULL, *a_arg = NULL;
    const char *filename_kb = "KB.txt";
    const char *filename_prps = "PRPS.txt";
    const char *filename_alpha = "ALPHA.txt";

    OprtrArgs *oprtr_tail = NULL, **oprtr_mid = &oprtr_tail;
    OprtrArgs **oprtr_head = &oprtr_tail;

    PrpsTree **tree_tmp = NULL, **alpha_tree = NULL,  **return_tree = NULL;
    Tokens **tokens_prps_alpha = NULL, **tokens_alpha = NULL;

    Tokens *tail = NULL;
    Tokens **arg_list = &tail, **exist_arg_list = NULL;

    a_prps = calloc(2, sizeof(char)); a_arg = calloc(2, sizeof(char));
    if (a_prps == NULL || a_arg == NULL)
        MallocErr("Read 0");

    tokens_prps_alpha = calloc(1, sizeof(Tokens *));
    if (tokens_prps_alpha == NULL)
        MallocErr("Read 1");

    //kb_string = ReadKB(m, filename_kb); // UNNEEDED: Reads KB.txt
    prps = ReadPRPS(filename_prps);     // Reads PRPS.txt
    alpha = ReadPRPS(filename_alpha);   // Reads ALPHA.txt

    fprintf(stdout, "KB: %s\n", prps);
    fprintf(stdout, "alpha: %s\n", alpha);

    // Combines the strings from PRPS.txt and ALPHA.txt to from a single sentence
    prps_alpha = calloc(strlen(prps) + strlen(" ^ !") + strlen(alpha) + 1, sizeof(char));
    if (prps_alpha == NULL)
        MallocErr("Read 3");

    strcat(prps_alpha, prps);
    strcat(prps_alpha, " ^ !");
    strcat(prps_alpha, alpha);

    n = strlen((const char *) prps_alpha);

    // Get a list of all hardcoded arguments (without universal/existential operators applied)
    ReadArgs(arg_list, &arg, prps_alpha, &i, n, quantifier, &oprtr_tail, &oprtr_head);

    exist_arg_list = calloc(1, sizeof(Tokens *));
    if (exist_arg_list == NULL)
        MallocErr("Read 2");

    (*tokens_prps_alpha) = PrpsTokenizer(prps_alpha, &nn);

    // THIS IS WHERE IT GETS COMPLICATED
    (*tree) = TokensToTree(tokens_prps_alpha, arg_list, 0);
    printf("Initial Tree: ");
    tree_print(*tree);
    printf("\n\n");
}
