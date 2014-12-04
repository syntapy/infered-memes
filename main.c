//#include <check.h>
#include "include.h"
//#include "test/unit_fixtures.h"

int main()
{
    const char **ptr = NULL;
    char *symbols[] = {"A", "B", "C", "D"};
    char *args[] = {"a", "b", "c", "d"};

    // n: # of symbols
    // m: max # chars in each symbol
    // l: # of args
    int m = 2, n = 4, l = 4;
    int i, d;
    int hash_val;
    int infered = 0;
    int is_true;

    char **alpha_info = NULL;

    PrpsTree **tree = NULL, **alpha = NULL;
    PrpsTree **tree_tmp = NULL;
    PrpsTree **left = NULL;
    PrpsTree **KB, **alpha_cpy;

    //HashTable **hash = NULL;

    int *array = NULL, *array_ptr = NULL;
    int is_cnf, tree_val = -1;

    //hash = GenerateAllocatedHashTable(n, l);
    Read(&tree, n, l, &hash_val);
    return 0;
    TreeConsistency(tree);

    //KB = CopySubTree(&((*tree) -> left));

    printf("Original Formula:\n\t"); tree_print(tree); printf("\n");
    d = 5;

    CNF(tree, d);

    // Checks if tree is in conjunctive normal form
    is_cnf = IsCNF(tree);
    if (is_cnf)
    {
        printf("\nTree has been converted"
            " to conjunctive normal form:\n\t");
        tree_print(tree);
        fprintf(stdout, "\n\n");
    }

    is_true = Resolution(tree);
    if (is_true)
        printf("\nInference takes place\n");
    else
        printf("Inference does not take place\n");

    return 0;
}
