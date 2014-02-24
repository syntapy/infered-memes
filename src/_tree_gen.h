/* T R E E - G E N E R A T I O N / A L L O C A T I O N - F U N C T I O N S */
PrpsTree *GenerateEmpty()
{   
    /* Generates a pointer to an empty propositional tree.
     * All this does is allocate the memory for a signle node tree 
     * which is a struct consisting of pointers
     */
 
    PrpsTree **tree = NULL;

	tree = (PrpsTree **) calloc(1, sizeof(PrpsTree *));
    if (tree == NULL) 
        MallocErr("PrpsTree *GenerateEmpty()");

    *tree = (PrpsTree *) calloc(1, sizeof(PrpsTree));
    if ((*tree) == NULL)
        MallocErr("PrpsTree *GenerateEmpty()");

    //(*tree) -> type = NULL;
    //(*tree) -> op = NULL;
    //(*tree) -> stmnt = NULL;
    //(*tree) -> p = NULL;
    //(*tree) -> left = NULL;
    //(*tree) -> right = NULL;

    return *tree;
}

void AllocateAsPrps(PrpsTree **node, const char *prps)
{   /* memory for the pointer in the PrpsTree node (**node)
     *
     * i.e. it allocates memory for both 
     * (*node) -> type and (*node) -> stmnt, setting the former
     * to PRPS (which is 0) and the latter to the string prps[]
     */

    int s = strlen(prps);

    if (!node || !(*node)) 
        NoMallocErr("void AllocateAsPrps(args...) 1");
    if ((*node) -> oprtr != NULL) 
        InconsistencyErr("AllocateAsPrps");

    if (/*(*node) -> type != NULL ||*/ (*node) -> stmnt != NULL)
        InconsistencyErr("AllocateAsPrps");
    (*node) -> stmnt = (MyString *) calloc(1, sizeof(MyString));
    //(*node) -> type = (int *) calloc(1, sizeof(int));
    (*node) -> neg = (int *) calloc(1, sizeof(int));

    if (/*(*node) -> type == NULL ||*/ (*node) -> stmnt == NULL ||
            (*node) -> neg == NULL) 
        MallocErr("void AllocateAsPrps(PrpsTree **node, char prps[]");

    checkStringEmpty(&((*node) -> stmnt));
    allocateNonEmptyString(&((*node) -> stmnt), s);
    checkStringNonEmpty(&((*node) -> stmnt));

    //*((*node) -> type) = PRPS;
    strcpy((*node) -> stmnt -> stc, prps);
    *((*node) -> stmnt -> s) = s;
    //*((*node) -> neg) = neg;
}

void AllocateAsOprtr(PrpsTree **node, int oprtr)
{   if (node == NULL || (*node) == NULL) DeathErr("AllocateAsOprtr");
    if ((*node) -> stmnt != NULL) InconsistencyErr("AllocateAsOprtr");
    if ((*node) -> oprtr != NULL /*|| (*node) -> type != NULL*/)
        InconsistencyErr("AllocateAsOprtr");

    (*node) -> oprtr = (int *) calloc(1, sizeof(int));
    //(*node) -> type = (int *) calloc(1, sizeof(int));
    (*node) -> neg = (int *) calloc(1, sizeof(int));
    if ((*node) -> oprtr == NULL /*|| (*node) -> type == NULL*/) 
        MallocErr("void AllocateAsOprtr");
    if ((*node) -> neg == NULL)
        MallocErr("void AllocateAsOprtr");
    *((*node) -> oprtr) = oprtr;
    //*((*node) -> type) = OPRTR;
    //*((*node) -> neg) = neg;
}

void DeallocateFromPrps(PrpsTree **node)
{
    //CheckPrpsTreeNode(node);
    free((*node) -> stmnt -> s);
    free((*node) -> stmnt -> stc);

    (*node) -> stmnt -> s = NULL;
    (*node) -> stmnt -> stc = NULL;

    free((*node) -> stmnt);
    (*node) -> stmnt = NULL;

    //(*node) -> stmnt -> s = NULL;
    //(*node) -> stmnt -> stc = NULL;
    //free((*node) -> type);
    //(*node) -> type = NULL;
}

void DeallocateFromOprtr(PrpsTree **node)
{
    //CheckOprtrNode(PrpsTree **tree);
    free((*node) -> oprtr);
    free((*node) -> neg);
    //free((*node) -> type);

    (*node) -> oprtr = NULL;
    (*node) -> neg = NULL;
    //(*node) -> type = NULL;
}

//void FreeOprtrNode(PrpsTree **node)
//{
//    if (OprtrNode(node))
//    {
//        DeallocateFromPrps(node);
//        
//    }
//}
