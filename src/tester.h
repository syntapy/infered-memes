#ifndef TESTER_H
#define TESTER_H
//#include "prps.h"
// 65 - 90: A - Z
// 97 - 122: a - z
typedef struct FindList
{
    char *prps, **arg;
    struct FindList *next;
    int is, is_not;
    int n_args;
} FindList;

typedef struct ClauseList
{
    FindList **clause;
    struct ClauseList *prev, *next;
} ClauseList;

char **CopyArgs(PrpsTree **node, int *n_args);
char *GenPrps(char *input, int *i);
char **GenArgs(char *input, int *i);

#endif
