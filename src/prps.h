#ifndef PRPS_H
#define PRPS_H

typedef struct MyString
{   /* Beefed up string data type.
     * Not sure why I used this.
     * Just to prevent using strlen()
     * ...I guess.
     */

    int *s;     // String length
    char *stc;  // String
} MyString;

typedef struct PrpsTree
{   
    /*  Node data type in a binary tree to represent
     *  logical sentences.
     *
     *  Each node can be either a propositional node (leaf node)
     *  or an operator node (internal node).
     */

    int *oprtr;         // var indicating type of logic operator
    int *neg;           // = 1 if negated, otherwise 0
    int value;          // Truth value of subtree
    int n_args;         // Number of arguments to propositional function

    // stmnt: propositional function string
    // argmnt: array of argument strings
    MyString *stmnt, **argmnt;

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

#endif
