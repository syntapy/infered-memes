#include <check.h>
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
    int infered;

    char **alpha_info = NULL;

    PrpsTree **tree = NULL, **alpha = NULL;
    PrpsTree **tree_tmp = NULL;

    HashTable **hash = NULL;

    int *array = NULL, *array_ptr = NULL;
    int is_cnf, tree_val = -1;

    hash = GenerateAllocatedHashTable(n, l);
    alpha = Read(&tree, hash, n, l, &hash_val);
    TreeConsistency(tree);

    Negate(&((*tree) -> left));
    tree_print(&((*tree) -> left));
    Negate(&((*tree) -> left));
    fprintf(stdout, " implies ");

    tree_print(&((*tree) -> right));
    fprintf(stdout, "\n\n");

    d = 3;

    // Convert tree to conjuctive normal form
    for (i = 0; i < d; i++)
        CNF(tree, d);

    // Checks if tree is in conjunctive normal form
    is_cnf = IsCNF(tree);
    if (is_cnf)
        printf("Tree has been converted to conjunctive normal form\n\n");
    fprintf(stdout, "Full tree in conjunctive normal form: \n");
    tree_print(tree);
    fprintf(stdout, "\n\n");

    infered = Infer(tree);

    if (infered)
        printf("Tree defaults to True, and thus the statement is infered\n");
    else
        printf("Tree does NOT default to True, so no inference can take place here\n");

    return 0;
}
