#ifndef PRPS_H
#define PRPS_H

typedef struct MyString
{   int *s;
    char *stc;
} MyString;

typedef struct PrpsTree
{   
    /* struct representing a propositional node */
    //int *type;          // Bool var: 1 if type
    int *oprtr;         // var indicating type of logic operator
    int *neg;           // = 1 if negated, otherwise 0
    int value;          // Truth value of subtree
    int n_args;
    MyString *stmnt, **argmnt;

    //struct PrpsTree **root;   // ptr_ptr to the root of tree. *root is constant
    struct PrpsTree *p;         // parent node
    struct PrpsTree *left;      // left child
    struct PrpsTree *right;     // right child
} PrpsTree;

#include "parse.h"

#include "tester.h"

void ListAddTree(TreeList ***tree_list_head, PrpsTree **tree_tmp, int depth);

void AddOprtr(OprtrList ***oprtr_list_head, int oprtr);

PrpsTree **QuantifierToTree(Tokens **tokens, Tokens **arg_list,
    Tokens **e_args, Args **u_args, int depth, int global_negate);

PrpsTree **U_E_QuantifierToTree(Tokens **tokens, Tokens **arg_list, 
    Tokens **e_args, Args **u_args, int depth, char quant, int global_negate);

void AddTree(Tokens **tokens, Tokens **arg_list,
    TreeList ***tree_list_head, TreeList **tree_list,
    Tokens **e_args, Args **u_args, 
    int depth, char quant, int subtree_negate);

//void GetArg(char **arg, char *input, int *i, int *arg_n);

#endif
