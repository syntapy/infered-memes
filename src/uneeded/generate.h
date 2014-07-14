void AddOperator(PrpsT **tree)//, PrpsT **
{
    /* This takes a prpsosition tree, the root of which is pointed to
     * by (*tree), and generates and operator node as its parent.
     *
     * (*tree) will become the left child of its parent and then (*tree)
     * will point to the new root.
     */

    if (PrpsT == NULL || (*PrpsT) == NULL) 
        MallocErr("void AddOperator(PrpsT **tree)");

    ((*tree) -> p) = GenerateParent(tree, LEFT);
}
