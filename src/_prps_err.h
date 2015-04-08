/* E R R O R - C H E C K I N G - F U N C T I O N S */
void CheckOprtrNode(PrpsTree **tree)
{   if (tree == NULL || (*tree) == NULL)
        InconsistencyErr("CheckLeafNode");
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
{   int i, n_args;

    if (tree == NULL || (*tree) == NULL)
        InconsistencyErr("CheckPrpsTreeNode");
    if ((*tree) -> stmnt == NULL || (*tree) -> argmnt == NULL 
            || (*tree) -> stmnt -> stc == NULL
            || (*tree) -> stmnt -> s == NULL
            || ((*tree) -> argmnt)[0] -> stc == NULL
            || ((*tree) -> argmnt)[0] -> s == NULL)
        InconsistencyErr("CheckPrpsTreeNode");
    if (strlen((*tree) -> stmnt -> stc) != *((*tree) -> stmnt -> s))
        InconsistencyErr("CheckPrpsTreeNode");

    n_args = (**tree).n_args;
    for (i = 0; i < n_args; i++)
        if (strlen(((*tree) -> argmnt)[i] -> stc) != (*((*tree) -> argmnt)[i] -> s))
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
    int i, n_args;

    if (tree != NULL && (*tree) != NULL)
    {
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
            n_args = (**tree).n_args;

            if (*(*tree) -> neg == NOT)
                fprintf(stdout, "~");
            fprintf(stdout, "%s[", (*tree) -> stmnt -> stc);

            for (i = 0; i < n_args; i++)
            {
                if (i > 0)
                    fprintf(stdout, "%s", ", ");
                fprintf(stdout, "%s", ((*tree) -> argmnt)[i] -> stc);
            }

            fprintf(stdout, "]");
        }

        else
            InconsistencyErr("tree_print 1");
    }
}

void tp(PrpsTree **tree)
{
    tree_print(tree);
    printf("\n");
}

void p_tl(TreeList *list)
{
    TreeList *ptr = list;

    while (ptr != NULL && (*ptr).tree != NULL)
    {
        tp((*ptr).tree);
        ptr = (*ptr).next;
    }
}
