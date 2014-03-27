int test_cnf(PrpsTree **tree, int d)
{
    int return_val = 0;

    return_val = IsCNF(tree);

    if (return_val == 0)
        for (i = 0; i < 2*d; i++)
            CNF(tree);

    return_val = IsCNF(tree);
}
//int test_everything()
//{
//    /* Return 1 if everything seems to work
//     * Else returns 0
//     */
//
//    int return_val = 1;
//
//    char **symbols = NULL;
//    char **args = NULL;
//    //char *buffer = NULL;
//    const char **ptr = NULL;
//
//    // n: # of symbols
//    // m: max # chars in each symbol
//    int m = 15, n = 8, l = 5;
//    int i, d;
//
//    PrpsTree **tree = NULL, **alpha = NULL;
//    PrpsTree **tree_tmp = NULL;
//
//    HashTable *hash = NULL;
//
//    hash = malloc(sizeof(HashTable));
//
//    if (hash == NULL)
//        MallocErr("main 1");
//
//    hash -> nodes = malloc(n * sizeof(HashNode));
//
//    if (hash -> nodes == NULL)
//        MallocErr("main 2");
//
//    tree = calloc(1, sizeof(PrpsTree *));
//
//    d = 9;
//
//    GenSymbolArray(&symbols, n, m);
//    GenSymbolArray(&args, l, 0);
//    ptr = (const char **) symbols;
//    srand(3);
//
//    GenSkewedTree(ptr, n, tree, d);
//
//    return_val = IsCNF(tree);
//    //fprintf(stdout, "Before operation: is_cnf = %d\n\n", is_cnf);
//    //fprintf(stdout, "\n");
//    for (i = 0; i < 2*d; i++)
//        CNF(tree);
//
//
//    //tree_print(tree);
//    //fprintf(stdout, "\n");
//
//    return_val = IsCNF(tree);
//    
//
//    if (return_val == 1)
//    {
//        
//    }
//    
//    return return_val;
//}
//
//int test_resolution();
