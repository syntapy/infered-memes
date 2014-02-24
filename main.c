#include "include.h"

int main()
{
    //char **symbols = NULL;
    //char **args = NULL;
    //char *buffer = NULL;
    //const char **ptr = NULL;

    // n: # of symbols
    // m: max # chars in each symbol
    int m = 15, n = 5, l = 5;
    int i, d;

    //PrpsTree **tree = NULL, **alpha = NULL;
    //PrpsTree **tree_tmp = NULL;

    HashTable *hash = NULL;

    hash = malloc(sizeof(HashTable));

    if (hash == NULL)
        MallocErr("main 1");

    hash -> nodes = malloc(n * sizeof(HashNode));

    if (hash -> nodes == NULL)
        MallocErr("main 2");

    //hash = calloc(1, sizeof(HashTable *));
    //if (!hash)
    //    MallocErr("main 1");

    int is_cnf;

    //tree = calloc(1, sizeof(PrpsTree *));

    d = 5;

    //GenSymbolArray(&symbols, n, m);
    //GenSymbolArray(&args, l, 0);
    //ptr = (const char **) symbols;
    //srand(3);

    //GenSkewedTree(ptr, m, tree, d);
    //GenAlphaTree((const char **) symbols, &alpha, m, n, d);

    //tree_tmp = tree;
    //tree = Oprtr(NOT, tree_tmp, IS, alpha, IS, AND);
    //free(tree_tmp); tree_tmp = NULL;

    //(*hash) = GenerateAllocatedHashTable(n, l);
    //setPointerToProp("A", &hash, n, l);

    //for (i = 2; i < n; i++)
    //{
    //    hash -> node_ptr = hash -> nodes + i * sizeof(HashNode);

    //    fprintf(stdout, "%p\n", hash -> node_ptr -> name);
    //}

    //SetValueForArg("A", "x", 1, &hash, n, l);

    //FillHashTable(&hash, symbols, args, n, l, 7);

    //tree_print(alpha);
    //fprintf(stdout, "\n");
    //TreeConsistency(tree);

    //is_cnf = IsCNF(tree);
    //fprintf(stdout, "Before operation: is_cnf = %d\n\n", is_cnf);
    //fprintf(stdout, "\n");
    //for (i = 0; i < d; i++)
    //    CNF(tree);


    //tree_print(tree);
    //fprintf(stdout, "\n");

    //is_cnf = IsCNF(tree);
    //if (is_cnf != 1)
    //    fprintf(stdout, "After operation: is_cnf = %d!\n", is_cnf);
    //else fprintf(stdout, "Good\n");

    //for (i = 0; i < m; i++)
    //    fprintf(stdout, "%s\n", symbols[i]);

    //HashTable *hash = NULL;
    //PrpsTree **tree = NULL, **tree_ptr_a = NULL, **tree_ptr_b = NULL;
    //PrpsTree **tree_a = NULL, **tree_b = NULL;
    //PrpsTree **left_a = NULL, **left_b = NULL, **right_a = NULL, **right_b = NULL,
    //         **right = NULL, **left = NULL;
    //PrpsTree *a = NULL, *b = NULL, *c = NULL;
    //PrpsTree **op_a = NULL;
    //PrpsTree **root = NULL;
    //PrpsTree **p = NULL;
    //int is_cnf;

    //left_a = (PrpsTree **) calloc(1, sizeof(PrpsTree *));
    //left_b = (PrpsTree **) calloc(1, sizeof(PrpsTree *));
    //right_a = (PrpsTree **) calloc(1, sizeof(PrpsTree *));
    //right_b = (PrpsTree **) calloc(1, sizeof(PrpsTree *));

    //left = calloc(1, sizeof(PrpsTree *));
    //right = calloc(1, sizeof(PrpsTree *));
    //root = (PrpsTree **) calloc(1, sizeof(PrpsTree *));
    //if (!left_a || !left_b) MallocErr("main");
    //if (!right_a || !right_b) MallocErr("main");
    //if (!left || !right) MallocErr("main");

    //(*left_a) = GenerateEmpty();
    //(*left_b) = GenerateEmpty();
    //(*right_a) = GenerateEmpty();
    //(*right_b) = GenerateEmpty();

    //(*left) = GenerateEmpty();
    //(*right) = GenerateEmpty();

    //tree = Oprtr2(NOT, tree, NOT, "LightWeight", IS, AND);

    //AllocateAsPrps(left_a, "Elephant");
    //AllocateAsPrps(left_b, "Old");

    //AllocateAsPrps(right_a, "Rhino");
    //AllocateAsPrps(right_b, "Wrinkly");

    //left = Oprtr(IS, left_a, IS, left_b, IS, AND);
    //right = Oprtr(IS, right_a, IS, right_b, IS, AND);

    //tree = Oprtr(IS, left_a, IS, right_a, IS, TAU);
    //CNF(tree);
    //is_cnf = IsCNF(tree);
    //fprintf(stdout, "IsCNF(tree) = %d\n", is_cnf);

    return 0;
}
