/* E R R O R - C H E C K I N G - F U N C T I O N S */
void CheckOprtrNode(PrpsTree **tree)
{   if (tree == NULL || (*tree) == NULL)
        InconsistencyErr("CheckLeafNode");
    //if ((*tree) -> type == NULL || *((*tree) -> type) != OPRTR)
    //    InconsistencyErr("CheckLeafNode 1");
    if ((*tree) -> stmnt != NULL || (*tree) -> argmnt != NULL)
        InconsistencyErr("CheckLeafNode 2");
    if ((*tree) -> oprtr == NULL)
        InconsistencyErr("CheckLeafNode 3");
    if ((*tree) -> left == NULL)
        InconsistencyErr("CheckLeafNode 3");
    if ((*tree) -> right == NULL)
        InconsistencyErr("CheckLeafNode 3");
}

void CheckPrpsTreeNode(PrpsTree **tree)
{   if (tree == NULL || (*tree) == NULL)
        InconsistencyErr("CheckPrpsTreeNode");
    //if ((*tree) -> type == NULL || *((*tree) -> type) != PRPS)
    //    InconsistencyErr("CheckPrpsTreeNode");
    if ((*tree) -> stmnt == NULL || (*tree) -> argmnt == NULL 
            || (*tree) -> stmnt -> stc == NULL
            || (*tree) -> stmnt -> s == NULL
            || (*tree) -> argmnt -> stc == NULL
            || (*tree) -> argmnt -> s == NULL)
        InconsistencyErr("CheckPrpsTreeNode");
    if (strlen((*tree) -> stmnt -> stc) != *((*tree) -> stmnt -> s))
        InconsistencyErr("CheckPrpsTreeNode");
    if (strlen((*tree) -> argmnt -> stc) != *((*tree) -> argmnt -> s))
        InconsistencyErr("CheckPrpsTreeNode");
    if ((*tree) -> oprtr != NULL)
        InconsistencyErr("CheckPrpsTreeNode");
    if ((*tree) -> left != NULL)
        InconsistencyErr("CheckPrpsTreeNode");
    if ((*tree) -> right != NULL)
        InconsistencyErr("CheckPrpsTreeNode");
}

void CheckConsistency(PrpsTree **tree)
{   if (!tree || !(*tree))
        DeathErr("CheckConsistency");

	if (OprtrNode(tree))
    {
        if ((*tree) -> left == NULL || (*tree) -> right == NULL)
            InconsistencyErr("CheckConsistency 1");
        if (!((*tree) -> left -> p == (*tree) && (*tree) -> right -> p == (*tree)))
            InconsistencyErr("CheckConsistency 2");
    }

    else if (PrpsNode(tree))
    {
        if ((*tree) -> left != NULL || (*tree) -> right != NULL)
            InconsistencyErr("CheckConsistency 3");
        if ((*tree) -> p != NULL)
            if (!OprtrNode(&((*tree) -> p)))
                InconsistencyErr("CheckConsistency 4");
    }

    else InconsistencyErr("CheckConsitency 5");
}

void CheckChildrenOprtr(PrpsTree **tree, int oprtr)
{   if (tree == NULL || (*tree) == NULL)
        DeathErr("ConvertIMP");
    //if ((*tree) -> type == NULL)
    //    InconsistencyErr("ConvertIMP");
    if (*((*tree) -> oprtr) != oprtr)
        InconsistencyErr("ConvertIMP");
    if ((*tree) -> right == NULL ||
            (*tree) -> left == NULL)
        NoMallocErr("ConvertIMP");
    if ((*tree) -> right -> neg == NULL ||
            (*tree) -> left -> neg == NULL)
        NoMallocErr("ConvertIMP");
}

void print_oprtr(int oprtr)
{
    if (oprtr == AND)
        fprintf(stdout, " ^ ");
    else if (oprtr == OR)
        fprintf(stdout, " v ");
    else if (oprtr == IMP)
        fprintf(stdout, " -> ");
    else if (oprtr == TAU)
        fprintf(stdout, " == ");
}

void tree_print(PrpsTree **tree)
{
    //CheckConsistency(tree);

    if ((*tree) -> left != NULL && (*tree) -> right != NULL)
    {
        if (*((*tree) -> neg) == NOT)
            fprintf(stdout, "~");
        fprintf(stdout, "(");
        tree_print(&((*tree) -> left));

        print_oprtr(*((*tree) -> oprtr));

        tree_print(&((*tree) -> right));

        fprintf(stdout, ")");
    }

    else if ((*tree) -> left == NULL && (*tree) -> right == NULL)
    {
        if (*(*tree) -> neg == NOT)
            fprintf(stdout, "~");
        fprintf(stdout, "%s[%s]", (*tree) -> stmnt -> stc, (*tree) -> argmnt -> stc);
    }

    else 
        InconsistencyErr("tree_print 1");
}
