#include <string.h>
#include "parse.h"
#include "prps.h"
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

/*char *ReadALPHA(const char *filename)
{
    char **alpha = NULL;
    int m, n;

    alpha = calloc(2, sizeof(char *));
    if (alpha == NULL)
        MallocErr("ReadALPHA 1");

    FILE *f = fopen(filename, "r");

    getline(&(alpha[0]), &m, f);
    getline(&(alpha[1]), &n, f);

    fclose(f);

    return alpha;
}*/

#include "_read.h"

#include "_list.h"

#include "_tree_synth.h"

int IncrementConditional(Tokens **arg_list, Args **u_args_ptr, char quant)
{
    /*  returns 0 if (*u_args_ptr) -> token_ptr -> next == NULL
     *  i.e. it is at end of linked list (Tokens **) arg_list 
     */

    int return_val = 1;

    if (quant == '4')
    {
        if (arg_list == NULL || u_args_ptr == NULL ||
            (*arg_list) == NULL || (*u_args_ptr) == NULL)
            return_val = 0;

        else if ((*u_args_ptr) -> token_ptr -> next == NULL)
        {
            (*u_args_ptr) -> token_ptr = (*arg_list);
            return_val = 0;
        }

        else if (IncrementConditional(arg_list, 
                    &((*u_args_ptr) -> next), quant) == 0)
            (*u_args_ptr) -> token_ptr =
                (*u_args_ptr) -> token_ptr -> next;
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

    while (IsLowerCase(input[j]))
        j++; j--;

    s = j - *i + 1;
    *prps = calloc(s, sizeof(char));
    if ((*prps) == NULL)
        MallocErr("GetPrps 1.5");

    for (j = 0; j < s; j++)
        (*prps)[j] = input[*i + j];
    (*prps)[j] = '\0';
    *i += s;
}

void GetArg(char ***arg, char *input, int *i, int *arg_n)
{
    /*  Collects all the arguments to a prps function into an
     *  array of arrays.
     *
     *  For instance:
     *      Loves[human, cat]
     *
     *  would allocate arg to have 2 (char *) elements, and one points to
     *  the "human" string, the other points to the "cat" string.
     */

    int j = *i, k, s, t;
    ArgNmbr *tail = NULL, **head = &tail;

    if (arg == NULL)
        DeathErr("GetArg 1");

    *arg_n = 0; (*arg) = NULL;
    while (input[j] != ']' && input[j] != '{' && input[j] != '}')
    {
        k = 0;
        while (input[j + k] != ',' && input[j + k] != ']')
            k++;

        *arg_n += 1;
        *head = calloc(1, sizeof(ArgNmbr));
        if (*head == NULL)
            MallocErr("GetArg 1.25");

        (**head).s = k;

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

    head = &tail;
    for (j = 0; j < *arg_n; j++)
    {
        (*arg)[j] = calloc(s+1, sizeof(char));
        if ((*arg)[j] == NULL)
            MallocErr("GetArg 2");

        s = (**head).s;
        t = (**head).t;
        for (k = 0; k < s; k++)
            (*arg)[j][k] = input[*i + j + k];

        for (k = s; k < s + t; k++);

        *i += k;
    }

    (*arg)[j] = '\0';
    *i += *arg_n;
}

Tokens **ReadArgs(char *input, int n)
{
    int i = 0, j = 0, arg_n = 0;
    char **arg = NULL, quant = '\0';
    Tokens **head = NULL, *tail = NULL;
    Tokens **return_val = NULL;
    int quantifier = 0, parentheses = 0, quant_parent = 0;
    Tokens **oprtrs = NULL, **oprtrs_tail = NULL;

    head = calloc(1, sizeof(Tokens *));
    oprtrs = calloc(1, sizeof(Tokens *));
    //tail = calloc(1, sizeof(Tokens));
    if (head == NULL || oprtrs == NULL)
        MallocErr("ReadArgs 1");

    while (i < n)
    {
        head = &tail;
        if (input[i] == '3' || input[i] == '4')
        {
            quant = input[i++];
            quantifier++;
            while (1)
            {
                if (IsLowerCase(input[i]))
                {
                    GetArg(&arg, input, &i, &arg_n);

                    for (j = 0; j < arg_n; j++)
                        E_AddArg(oprtrs, arg[j]);

                    free(arg); arg = NULL;
                }

                else if (input[i] == '{')
                {
                    i++;
                    break;
                }

                else
                    i++;
            }
        }

        if (input[i] == '}')
        {
            quantifier--;
            i++;
        }

        if (IsUpperCase(input[i]))
            while (input[++i] != '[');

        if (IsLowerCase(input[i]))
        {
            GetArg(&arg, input, &i, &arg_n);
            if (quantifier > 0)
            {
                for (j = 0; j < arg_n; j++)
                    if (!T_Contains(oprtrs, arg[j]))
                        E_AddArg(head, arg[j]);
            }

            else if (quantifier == 0)
                for (j = 0; j < arg_n; j++)
                    E_AddArg(head, arg[j]);
            i++;
        }

        else if (input[i] == '\n')
            break;

        else
            i++;
    }

    return_val = calloc(1, sizeof(Tokens *));
    if (return_val == NULL)
        MallocErr("ReadArgs 2");
    (*return_val) = tail;
    //print_tokens(tail);

    return return_val;
}

PrpsTree **Read(PrpsTree ***tree, int h, int k, int *hash_val)
{
    char **kb_string = NULL;
    char **alpha_string = NULL;
    char *prps = NULL, *alpha = NULL, prps_alpha = NULL;
    int m = 4, n;
    int nn = 0, mm = 0;
    //int hash_val;

    char *a_prps = NULL, *a_arg = NULL;
    const char *filename_kb = "KB.txt";
    const char *filename_prps = "PRPS.txt";
    const char *filename_alpha = "ALPHA.txt";

    PrpsTree **return_val = NULL;

    //return_val = calloc(2, sizeof(char *));
    //if (return_val == NULL)
    //    MallocErr("Read -1");

    //return_val[0] = calloc(2, sizeof(char));
    //return_val[1] = calloc(2, sizeof(char));

    //if (return_val[0] == NULL || return_val[1] == NULL)
    //    MallocErr("Read -2");

    PrpsTree **tree_tmp = NULL, **alpha_tree = NULL,  **return_tree = NULL;
    Tokens **tokens_prps = NULL, **tokens_alpha = NULL;

    //  List of argument symbols, and list of symbols generated
    //  by existential quantifiers
    Tokens **arg_list = NULL, **exist_arg_list = NULL;

    a_prps = calloc(2, sizeof(char)); a_arg = calloc(2, sizeof(char));
    if (a_prps == NULL || a_arg == NULL)
        MallocErr("Read 0");

    tokens_prps = calloc(1, sizeof(Tokens *));
    tokens_alpha = calloc(1, sizeof(Tokens *));
    if (tokens_prps == NULL || tokens_alpha == NULL)
        MallocErr("Read 1");

    kb_string = ReadKB(m, filename_kb);
    prps = ReadPRPS(filename_prps);
    alpha = ReadPRPS(filename_alpha);

    n = strlen((const char *) prps);
    arg_list = ReadArgs(prps, n);

    exist_arg_list = calloc(1, sizeof(Tokens *));
    if (exist_arg_list == NULL)
        MallocErr("Read 2");

    (*tokens_prps) = PrpsTokenizer(prps, &nn);
    (*tokens_alpha) = PrpsTokenizer(alpha, &mm);

    //print_tokens((*tokens_prps));
    //LearnKB(hash, kb_string, m, h, k);

    tree_tmp = TokensToTree(tokens_prps, arg_list, 0);
    tree_print(tree_tmp);
    printf("\n\n");

    alpha_tree = TokensToTree(tokens_alpha, arg_list, 0);
    return_val = CopySubTree(alpha_tree);
    //return_val[1][0] = ((*alpha_tree) -> argmnt -> stc)[0];
    //return_val[0][1] = '\0'; return_val[1][1] = '\0';

    (*tree) = Oprtr(IS, tree_tmp, NOT, alpha_tree, IS, AND);
    free(tree_tmp); free(alpha_tree);
    tree_tmp = NULL; alpha_tree = NULL;

    //Negate((*tree));

    //print_tokens((*tokens));
    //tree_print(return_tree);
    //printf("\n");

    return return_val;
}
