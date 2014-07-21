#include "include.h"

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
    alpha_info = Read(&tree, hash, n, l, &hash_val);
    TreeConsistency(tree);

    //SetValueForArg(alpha_info[0], alpha_info[1], hash_val, hash, n, l);

    
    //fprintf(stdout, "Original sentence in original form:\n\t");
    Negate(&((*tree) -> left));
    tree_print(&((*tree) -> left));
    Negate(&((*tree) -> left));
    fprintf(stdout, " implies ");

    //fprintf(stdout, "Statement to be inferred:\n\t");
    tree_print(&((*tree) -> right));
    fprintf(stdout, ": ");

    //fprintf(stdout, "Full tree in original form:\n\t");
    //tree_print(tree);
    //fprintf(stdout, "\n");

    d = 3;
    for (i = 0; i < d; i++)
        CNF(tree);

    is_cnf = IsCNF(tree);
    //fprintf(stdout, "Tree is in CNF: %d\n", is_cnf);
    //fprintf(stdout, "Full tree in conjunctive normal form:\n\t");
    //tree_print(tree);
    //fprintf(stdout, "\n\n");

    //tree_val = GetTreeValue(tree, hash, n, l);
    //fprintf(stdout, "Truth value of full tree after conversion to conjuctive normal form: %d\n", tree_val);

    infered = Infer(tree, alpha_info[0], alpha_info[1]);
    if (infered)
        fprintf(stdout, "True\n");
    else fprintf(stdout, "False\n");

    return 0;
}
