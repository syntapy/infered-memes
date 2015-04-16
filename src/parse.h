#ifndef PARSE_H
#define PARSE_H

#include "prps.h"

typedef struct Tokens
{
    /*  Used in parsing non-quantifier text
     *  and existential quantifiers
     *
     *  FIELDS:
     *      (char *) token: 
     *      (int) depth: integer describing depth in 
     *          nested quantifier statements
     *      (Tokens *) next: pointer for linked list
     */

    char *token;
    int depth;
    struct Tokens *next;
} Tokens;

typedef struct Args
{
    /*  Used in implementing universal quantifiers.
     *  It is a linked list stack, each node contains the literal argument
     *  string when it was encountered right after the quantifer
     *  token, and a pointer to an argument in the linked list of 
     *  non-quantified argument strings.
     */

    char *token;        // Literal quantified argument
    Tokens *token_ptr;  // Pointer to non-quantified argument 
                        //    in another linked list
    int depth;          // Depth into nested quantied sentences
    struct Args *next;
} Args;

typedef struct ArgNmbr
{
    int s, t;
    struct ArgNmbr *next;
} ArgNmbr;

typedef struct TreeList
{
    /*  This is a linked list of logical sentences
     *  in binary tree form.
     */

    PrpsTree **tree;
    struct TreeList *next;
} TreeList;

typedef struct OprtrList
{
    int oprtr;
    struct OprtrList *next;
} OprtrList;

typedef struct OprtrArgs
{
    /*  This is a struct designed to be a linked list.
     *  Its used to keep track of arguments read in a logical sentence
     *  string when dealing with universal and existential quantifers.
     *  
     *  FIELDS:
     *      (char) quant: indicates which quantifier
     *          is associated with args in (Tokens *) args
     *      (Tokens *) args: a linked list
     *      (OprtrArgs *) prev, next: doubly linked list pointers
     */

    char quant;
    Tokens *args;
    struct OprtrArgs *prev, *next;
} OprtrArgs;

void GetPrps(char **prps, char *input, int *i);
void GetArg(char ***arg, char *input, int *i, int *arg_n);
int IncrementConditional(Tokens **arg_list, Args **u_args_ptr, int depth);

#endif
