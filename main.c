#include "include.h"

int main()
{
    //char **symbols = NULL;
    //char **args = NULL;
    //char *buffer = NULL;
    const char **ptr = NULL;
    char *symbols[] = {"A", "B", "C", "D"};
    char *args[] = {"a", "b", "c", "d"};

    // n: # of symbols
    // m: max # chars in each symbol
    // l: # of args
    int m = 2, n = 4, l = 4;
    int i, d;

    PrpsTree **tree = NULL, **alpha = NULL;
    PrpsTree **tree_tmp = NULL;

    HashTable **hash = NULL;

    int *array = NULL, *array_ptr = NULL;

    //array = calloc(10, sizeof(int));

    //if (array == NULL)
    //    MallocErr("main 1");

    //array_ptr = array + 3*sizeof(int);

    //if (array_ptr != array)
    //    DeathErr("main 2");

    int is_cnf, tree_val = -1;

    //tree = calloc(1, sizeof(PrpsTree *));
    //alpha = calloc(1, sizeof(PrpsTree *));
    //if (tree == NULL || alpha == NULL)
    //    MallocErr("main 2");

    //tree_mpt = calloc(1, sizeof(PrpsTree *));
    //if (tree == NULL)
    //    MallocErr("main 3");

    //(*tree) = GenerateEmpty();
    //(*alpha) = GenerateEmpty();

    //(*tree_tmp) = GenerateEmpty();

    hash = GenerateAllocatedHashTable(n, l);
    tree = Read(hash, n, l);
    TreeConsistency(tree);

    fprintf(stdout, "Tree in original form:\t");
    tree_print(tree);
    fprintf(stdout, "\n");

    d = 2;
    for (i = 0; i < d; i++)
        CNF(tree);

    is_cnf = IsCNF(tree);
    fprintf(stdout, "Tree is in CNF: %d\n", is_cnf);
    fprintf(stdout, "Tree in CNF:\t");
    tree_print(tree);
    fprintf(stdout, "\n");

    tree_val = GetTreeValue(tree, hash, n, l);
    fprintf(stdout, "Truth value of tree: %d\n", tree_val);
    fprintf(stdout, "1 = True, 0 = False, -1 = Unknown\n");

    //GenSymbolArray(&symbols, n, m);
    //GenSymbolArray(&args, l, 0);

    //AllocateAsPrps(tree, (const char *) symbols[0], (const char *) args[0]);

    //ptr = (const char **) symbols;
    //agr = (const char **) args;
    //srand(5);

    //GenSkewedTree((const char **) symbols, (const char **) args, n, l, tree, d);
    //GenAlphaTree((const char **) symbols, (const char **) args, &alpha, n, l, d);

    //tree = OprtrWrap(NOT, tree, IS, (const char *) symbols[1], 
    //    (const char *) args[0], IS, AND);
    //tree = OprtrWrap(IS, tree, IS, (const char *) symbols[2], 
    //    (const char *) args[1], IS, IMP);
    //tree = OprtrWrap(IS, tree, NOT, (const char *) symbols[2], 
    //    (const char *) args[2], IS, IMP);

    //AllocateAsPrps(tree_tmp, (const char *) symbols[0], (const char *) args[0]);
    //tree_tmp = OprtrWrap(IS, tree_tmp, IS, (const char *) symbols[2], (const char *) args[1], 
    //IS, IMP);

    //SetValueForArg(symbols[0], args[0], 0, hash, n, l);
    //SetValueForArg(symbols[1], args[0], 1, hash, n, l);
    //SetValueForArg(symbols[2], args[1], 1, hash, n, l);
    //fprintf(stdout, "Val not in hash: %d\n", GetValueForArg(symbols[2], args[2], hash, n, l));

    //fprintf(stdout, "%s[%s] = %d\n", symbols[0], args[0], 
    //    GetValueForArg(symbols[0], args[0], hash, n, l));

    //fprintf(stdout, "%s[%s] = %d\n", symbols[1], args[0], 
    //    GetValueForArg(symbols[1], args[0], hash, n, l));

    //fprintf(stdout, "%s[%s] = %d\n", symbols[2], args[1], 
    //    GetValueForArg(symbols[2], args[1], hash, n, l));
    //for (i = 0; i < d; i++)
    //    CNF(tree);

    //hash = GenerateAllocatedHashTable(n, l);
    //FillHashTable(hash, symbols, args, n, l, 7);

    return 0;
}

    //printf("%d\n", GetValueForArg("E", "F", hash, n, l));

    //hash -> node_ptr = hash -> nodes + 5 * sizeof(HashNode);
    //(*hash) -> node_ptr = ((*hash) -> nodes) + 5 * sizeof(HashNode);
    //(*hash) -> node_ptr -> name;

    //setPointerToProp("A", &hash, n, l);

    //for (i = 2; i < n; i++)
    //{
    //    hash -> node_ptr = hash -> nodes + i * sizeof(HashNode);

    //    fprintf(stdout, "%p\n", hash -> node_ptr -> name);
    //}

    //SetValueForArg("A", "x", 1, &hash, n, l);

    //

    //tree_print(alpha);
    //fprintf(stdout, "\n");


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
//}
