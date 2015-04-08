void checkStringEmpty(MyString **arg_ptr_ptr, int arg_n)
{   
    int i;

    if (arg_ptr_ptr == NULL)
        DeathErr("Something wrong with compiler");

    for (i = 0; i < arg_n; i++)
    {
        if (arg_ptr_ptr[i] == NULL)
            DeathErr("checkStringEmpty");
        if (arg_ptr_ptr[i] -> stc != NULL)
            DeathErr("checkStringEmpty");
        if (arg_ptr_ptr[i] -> s != NULL)
            DeathErr("checkStringEmpty");
    }
}

void checkStringNonEmpty(MyString **arg_name)
{   if ((*arg_name) == NULL)
        NoMallocErr("checkStringEmpty");
    if ((*arg_name) -> stc == NULL)
        NoMallocErr("checkStringEmpty");
    if ((*arg_name) -> s == NULL)
        NoMallocErr("checkStringEmpty");
}

void allocateNonEmptyString(MyString **arg_ptr_ptr, int s)
{   (*arg_ptr_ptr) -> s = (int *) calloc(1, sizeof(int));
    (*arg_ptr_ptr) -> stc = (char *) calloc(s + 1, sizeof(char));

    checkStringNonEmpty(arg_ptr_ptr);

    *((*arg_ptr_ptr) -> s) = s;
}

/* T R E E - G E N E R A T I O N / A L L O C A T I O N - F U N C T I O N S */
PrpsTree *GenerateEmpty()
{   
    /* Generates a pointer to an empty propositional tree.
     * All this does is allocate the memory for a signle node tree 
     * which is a struct consisting of pointers
     */
 
    PrpsTree *tree = NULL;

	tree = (PrpsTree *) calloc(1, sizeof(PrpsTree));
    if (tree == NULL) 
        MallocErr("PrpsTree *GenerateEmpty()");

    return tree;
}

void AllocateAsPrps(PrpsTree **node, char *prps, char **arg, int n_args)
{   /* memory for the pointer in the PrpsTree node (**node)
     *
     * i.e. it allocates memory for both 
     * (*node) -> type and (*node) -> stmnt, setting the former
     * to PRPS (which is 0) and the latter to the string prps[]
     */

    int i, s = strlen(prps), *t = NULL, s_store = 0;
    MyString *argmnt_store = NULL;

    t = calloc(n_args, sizeof(int));
    if (t == NULL)
        MallocErr("void AllocateAsPrps 0");

    for (i = 0; i < n_args; i++)
    {
        t[i] = strlen(arg[i]);
        s_store += strlen(arg[i]) + 1;
    }

    if (!node || !(*node)) 
        NoMallocErr("void AllocateAsPrps(args...) 1");
    if ((*node) -> oprtr != NULL) 
        InconsistencyErr("AllocateAsPrps");

    if ((*node) -> argmnt != NULL || (*node) -> stmnt != NULL)
        InconsistencyErr("AllocateAsPrps");
    (*node) -> stmnt = (MyString *) calloc(1, sizeof(MyString));
    (*node) -> argmnt = (MyString **) calloc(n_args, sizeof(MyString *));

    (*node) -> neg = (int *) calloc(1, sizeof(int));

    if ((*node) -> argmnt == NULL || (*node) -> stmnt == NULL ||
            (*node) -> neg == NULL) 
        MallocErr("void AllocateAsPrps(PrpsTree **node, char prps[]");

    checkStringEmpty(&((*node) -> stmnt), 1);
    allocateNonEmptyString(&((*node) -> stmnt), s);
    checkStringNonEmpty(&((*node) -> stmnt));

    for (i = 0; i < n_args; i++)
    {
        ((*node) -> argmnt)[i] = calloc(1, sizeof(MyString));
        if (((*node) -> argmnt)[i] == NULL)
            MallocErr("AllocateAsPrps 2");

        allocateNonEmptyString(&(((*node) -> argmnt)[i]), t[i]);
        checkStringNonEmpty(&(((*node) -> argmnt)[i]));

        strcpy(((*node) -> argmnt)[i] -> stc, arg[i]);
        *(((*node) -> argmnt)[i] -> s) = t[i];
    }

    strcpy((*node) -> stmnt -> stc, prps);
    *((*node) -> stmnt -> s) = s;
    (**node).n_args = n_args;
}

void AllocateAsOprtr(PrpsTree **node, int oprtr)
{   if (node == NULL || (*node) == NULL) DeathErr("AllocateAsOprtr");
    if ((*node) -> stmnt != NULL) InconsistencyErr("AllocateAsOprtr");
    if ((*node) -> argmnt != NULL) InconsistencyErr("AllocateAsOprtr");
    if ((*node) -> oprtr != NULL )
        InconsistencyErr("AllocateAsOprtr");

    (*node) -> oprtr = (int *) calloc(1, sizeof(int));
    (*node) -> neg = (int *) calloc(1, sizeof(int));
    if ((*node) -> oprtr == NULL) 
        MallocErr("void AllocateAsOprtr");
    if ((*node) -> neg == NULL)
        MallocErr("void AllocateAsOprtr");
    *((*node) -> oprtr) = oprtr;
}

void DeallocateFromPrps(PrpsTree **node)
{
    int i, n_args;
    free((*node) -> stmnt -> s);
    free((*node) -> stmnt -> stc);

    n_args = (*node) -> n_args;
    for (i = 0; i < n_args; i++)
    {
        free(((*node) -> argmnt)[i] -> s);
        free(((*node) -> argmnt)[i] -> stc);
        ((*node) -> argmnt)[i] -> s = NULL;
        ((*node) -> argmnt)[i] -> stc = NULL;
    }

    (*node) -> stmnt -> s = NULL;
    (*node) -> stmnt -> stc = NULL;

    free((*node) -> stmnt);
    (*node) -> stmnt = NULL;

    free((*node) -> argmnt);
    (*node) -> argmnt = NULL;
}

void DeallocateFromOprtr(PrpsTree **node)
{
    free((*node) -> oprtr);
    free((*node) -> neg);

    (*node) -> oprtr = NULL;
    (*node) -> neg = NULL;
}
