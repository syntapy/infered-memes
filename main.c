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

    int *array = NULL, *array_ptr = NULL;
    int is_cnf, tree_val = -1;

    Read(&tree, n, l, &hash_val);
    TreeConsistency(tree);

    printf("Original Formula:\n\t"); tree_print(tree); printf("\n");
    d = 5;

    CNF(tree, d);

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
        fprintf(stderr, "\nInference takes place\n");
    else
        fprintf(stderr, "Inference does not take place\n");

    return 0;
}
