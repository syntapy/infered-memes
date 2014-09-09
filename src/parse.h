#include <string.h>
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
{   // Used in parsing non-quantifier text
    //    and existential quantifiers
    char token;
    struct Tokens *next;
} Tokens;

typedef struct Args
{   // Used in implementing universal quantifiers
    char token;
    Tokens *token_ptr;
    struct Args *next;
} Args;

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

int IncrementConditional(Tokens **arg_list, Args **u_args_ptr, char quant);

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

Tokens **ReadArgs(char *input, int n)
{
    int i = -1, j = 0;
    Tokens **head = NULL, *tail = NULL;
    Tokens **return_val = NULL;
    int quantifier = 0, parentheses = 0, quant_parent = 0;
    Tokens **oprtrs = NULL, **oprtrs_tail = NULL;

    head = calloc(1, sizeof(Tokens *));
    oprtrs = calloc(1, sizeof(Tokens *));
    //tail = calloc(1, sizeof(Tokens));
    if (head == NULL || oprtrs == NULL)
        MallocErr("ReadArgs 1");

    while (++i < n)
    {
        head = &tail;
        if (input[i] == '3' || input[i] == '4')
        {
            quantifier++;
            while (1)
            {
                if (IsLowerCase(input[i]))
                    E_AddArg(oprtrs, input[i++]);
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

        if (input[i] >= LOWER_CASE_FIRST && 
            input[i] <= LOWER_CASE_LAST && 
            input[i] != 'v')
        {

            if (quantifier > 0)
            {
                if (!T_Contains(oprtrs, input[i]))
                    E_AddArg(head, input[i]);
            }
            else if (quantifier == 0)
                E_AddArg(head, input[i]);
        }
    }

    return_val = calloc(1, sizeof(Tokens *));
    if (return_val == NULL)
        MallocErr("ReadArgs 2");
    (*return_val) = tail;
    //print_tokens(tail);

    return return_val;
}

PrpsTree **Read(PrpsTree ***tree, HashTable **hash, 
    int h, int k, int *hash_val)
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
    LearnKB(hash, kb_string, m, h, k);

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
