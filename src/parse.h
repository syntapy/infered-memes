#ifndef PARSE_H
#define PARSE_H

#include "prps.h"

typedef struct Tokens
{   // Used in parsing non-quantifier text
    //    and existential quantifiers
    char *token;
    struct Tokens *next;
} Tokens;

typedef struct Args
{   // Used in implementing universal quantifiers
    char *token;
    Tokens *token_ptr;
    struct Args *next;
} Args;

typedef struct ArgNmbr
{
    int s, t;
    struct ArgNmbr *next;
} ArgNmbr;

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

void GetPrps(char **prps, char *input, int *i);
void GetArg(char ***arg, char *input, int *i, int *arg_n);
int IncrementConditional(Tokens **arg_list, Args **u_args_ptr, char quant);

#endif
