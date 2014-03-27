void allocateNonEmptyString(MyString **arg_ptr_ptr, int s)
{   (*arg_ptr_ptr) -> s = (int *) calloc(1, sizeof(int));
    (*arg_ptr_ptr) -> stc = (char *) calloc(s + 1, sizeof(char));

    checkStringNonEmpty(arg_ptr_ptr);

    *((*arg_ptr_ptr) -> s) = s;
}

void allocateEmptyHashNode(HashNode **node_ptr_ptr)
{   /* Sets all the pointers in HashNode, pointed to 
     * by pointer pointed to by node_ptr_ptr, to NULL
     */

    //node_ptr_ptr = (HashNode **) malloc(sizeof(HashNode *));
    if ((*node_ptr_ptr) == NULL) 
    {   *node_ptr_ptr = (HashNode *) malloc(sizeof(HashNode));
        if ((*node_ptr_ptr) == NULL) 
            MallocErr("void allocateEmptyHashNode(HashNode **node_ptr_ptr) 1 ");
    } if ((*node_ptr_ptr) == NULL) 
        NoMallocErr("void allocateEmptyHashNode(HashNode **node_ptr_ptr) 2 ");

    (*node_ptr_ptr) -> n = NULL;
    (*node_ptr_ptr) -> array = NULL;
    (*node_ptr_ptr) -> name = NULL;
    (*node_ptr_ptr) -> next = NULL;
    (*node_ptr_ptr) -> head = NULL;
}

void allocateNonEmptyHashNode(HashNode **node_ptr_ptr, int s, int n)
{   /* Allocates memory for the pointers in the struct 
     * that holds the array corresponding to the result
     * of the propositional function for each argument.
     *
     * node_ptr_ptr is a pointer-pointer to a HashNode
     * struct in hash -> array.
     * 
     * s is the length of the MyString
     */

    // Sets all pointers in the HashNode to NULL
    if (node_ptr_ptr == NULL)
        InconsistencyErr("void allocateNonEmptyHashNode(HashNode **node_ptr_ptr, int n) 1 ");
    if ((*node_ptr_ptr) == NULL)
        allocateEmptyHashNode(node_ptr_ptr);
    if (*node_ptr_ptr == NULL) 
        NoMallocErr("void allocateNonEmptyHashNode(HashNode **node_ptr_ptr, int n) 2 ");
    //else checkHashNodeNonEmpty(node_ptr_ptr);

    //printf("outside function: node_ptr_ptr = %p\n", node_ptr_ptr);
    //printf("outsiie function: (*node_ptr_ptr) = %p\n", *node_ptr_ptr);

    // Check if not yet allocated
    //if (node_ptr_ptr == NULL) 
    //    NoMallocErr("void allocateNonEmptyHashNode(HashNode **node_ptr_ptr, int n) 1 ");

    checkHashNodeEmpty(node_ptr_ptr);

    // Allocates memory for pointers in (HashNode **)
    (*node_ptr_ptr) -> array = (ArgNode *) calloc(n, sizeof(ArgNode));
    (*node_ptr_ptr) -> name = (MyString *) calloc(1, sizeof(MyString));
    (*node_ptr_ptr) -> n = (int *) calloc(1, sizeof(int));
    //(*node_ptr_ptr) -> next = (HashNode *) calloc

    checkHashNodeNonEmpty(node_ptr_ptr);

    checkStringEmpty(&((*node_ptr_ptr) -> name));
    allocateNonEmptyString(&((*node_ptr_ptr) -> name), s);
    checkStringNonEmpty(&((*node_ptr_ptr) -> name));

    *((*node_ptr_ptr) -> n) = n;
}

void allocateEmptyArgNode(ArgNode **arg_ptr_ptr)
{   /* Allocates memory for the ArgNode struct containing
     * the argument of interest. It does not set any values.
     * This function should be run after setting the 
     * arg_ptr_ptr -> node_ptr -> arg_ptr pointer to the appropriate
     * value
     */

    //ArgNode **arg_ptr;

    //arg_ptr = (ArgNode **) malloc(sizeof(ArgNode *));
    if (!arg_ptr_ptr) DeathErr("...wait, what?");

    //checkHashNodeNonEmpty2(arg_ptr_ptr);
    //checkHashNodeNonEmpty(arg_ptr_ptr -> node_pt

    //(*arg_ptr) = (*arg_ptr_ptr) -> node_ptr -> arg_ptr;

    if ((*arg_ptr_ptr) == NULL)
    {   (*arg_ptr_ptr) = (ArgNode *) malloc(sizeof(ArgNode));
        if ((*arg_ptr_ptr) == NULL)
            MallocErr("void allocateEmptyArgNode(HashTable **arg_ptr_ptr) 1 ");
    }

    (*arg_ptr_ptr) -> arg = NULL;
    (*arg_ptr_ptr) -> result = NULL;
    (*arg_ptr_ptr) -> next = NULL;
}

void allocateNonEmptyArgNode(ArgNode **arg_ptr_ptr, int s, int n)
{   // s is the length of the MyString

    if (!arg_ptr_ptr) DeathErr("");
    allocateEmptyArgNode(arg_ptr_ptr);

    checkArgNodeEmpty(arg_ptr_ptr);
    (*arg_ptr_ptr) -> arg = (MyString *) calloc(1, sizeof(MyString));
    (*arg_ptr_ptr) -> result = (int *) calloc(n, sizeof(int));
    checkArgNodeNonEmpty(arg_ptr_ptr);

    checkStringEmpty(&((*arg_ptr_ptr)-> arg));
    allocateNonEmptyString(&((*arg_ptr_ptr) -> arg), s);
    checkStringNonEmpty(&((*arg_ptr_ptr) -> arg));
}

void allocatePropNode(char prop[], HashNode **node_ptr_ptr, int prp, int s, int n)
{

    if (prp == -1 || prp == 1)
    {   allocateNonEmptyHashNode(node_ptr_ptr, s, n);
        allocateEmptyHashNode(&((*node_ptr_ptr) -> next));
    } 
    
    //else if (prp == 1)
    //{   allocateNonEmptyHashNode(node_ptr_ptr, s, n);
    //    allocateEmptyHashNode(&((*node_ptr_ptr) -> next));
    //} 
    //else if (prp == 0)
    //    InconsistencyErr("void allocatePropNode(args...)");
    if ((*node_ptr_ptr) -> name -> stc == NULL)
        DeathErr("void setPointerToProp(char prop[], HashTable **hash, int m, int n)");
}

void allocateArgNode(char arg[], ArgNode **arg_ptr_ptr, int rgu, int s, int n)
{   if (rgu == -1)
    {   allocateNonEmptyArgNode(arg_ptr_ptr, s, n);
        allocateEmptyArgNode(&((*arg_ptr_ptr) -> next));
    } else if (rgu == 1)
    {   allocateNonEmptyArgNode(arg_ptr_ptr, s, n);
        allocateEmptyArgNode(&((*arg_ptr_ptr) -> next));
    } 
    
    //else if (rgu == 0)
     //   InconsistencyErr("void allocatePropNode(args...)");
    if ((*arg_ptr_ptr) -> arg == NULL)
        DeathErr("void setPointerToProp(char prop[], HashTable **hash, int m, int n)");
}

int setPointerToProp(char prop[], HashTable **hash, int m, int n)
{   
    /*
        returns -1, 1, or 0
        I can't see how this would retun -1
        returns 1 if node pointed to is NULL, and thus hasn't been allocated yet

        returns 0 if the prpositional symbol is already in the hash table

    */

    if (*hash == NULL) 
        NoMallocErr("void setPointerToProp(char prop[], HashTable **hash, int m, int n) 1 ");

    int index = GetPropIndex(prop, m);
    HashNode *tail_ptr;

    //(*hash) -> node_ptr = (*hash) -> nodes + index * sizeof(HashNode);
    (*hash) -> node_ptr = &(((*hash) -> nodes)[index]);

    while (1)
    {   if ((*hash) -> node_ptr == NULL) 
            return -1;
        if ((*hash) -> node_ptr -> name == NULL) 
            return 1; 
        if ((*hash) -> node_ptr -> name -> stc == NULL)
            DeathErr("void setPointerToProp(char prop[], HashTable **hash, int m, int n)");

        if (strcmp((*hash) -> node_ptr -> name -> stc, prop) == 0) 
            return 0;

        (*hash) -> node_ptr = (*hash) -> node_ptr -> next;
    }
}

int setPointerToArg(char arg[], HashTable **hash, int m, int n)
{   int index = GetArgIndex(arg, n);
    ArgNode *arg_ptr = NULL;

    checkHashTableNonEmpty(hash);
    checkHashNodeNonEmpty(&((*hash) -> node_ptr));

    // This function requires that hash -> node_ptr has already been appropriately set
    //(*hash) -> node_ptr -> arg_ptr = (*hash) -> node_ptr -> array + index * sizeof(ArgNode);
    (*hash) -> node_ptr -> arg_ptr = &(((*hash) -> node_ptr -> array)[index]);
    arg_ptr = (*hash) -> node_ptr -> arg_ptr;

    while (1)
    {   if (arg_ptr == NULL) 
            return -1;
        if (arg_ptr -> arg == NULL) 
            return 1;
        if (arg_ptr -> arg -> stc == NULL)
            DeathErr("void setPointerToArg(args...)");
        if (strcmp(arg_ptr -> arg -> stc, arg) == 0) 
            return 0;

        arg_ptr = arg_ptr -> next;
        (*hash) -> node_ptr -> arg_ptr = arg_ptr;
    }
}

void addSymbol(char prop[], HashTable **hash, int m, int n)
{   /* Adds the string prop[] to the proposition table if not
     * already present. Otherwise, the pointer (*hash) -> node_ptr
     * is set to point to the HashNode containing prop[]
     *
     * m is the number of Propositional functions in 
     * the knowledge base, in case youve forgotten.
     *
     * I'll let you guess what n is...
     */

    int i, index, prp;

    checkHashTableNonEmpty(hash);

    if (*((*hash) -> m) != m) 
        InconsistencyErr("void addSymbol(char prop[], int m, int n, HashTable *hash) 3 ");

    // Get index to put symbol in
    index = GetPropIndex(prop, m);

    if (index >= m || index < 0) 
    {   fprintf(stderr, "m = %d\n", m);
        InconsistencyErr("void addSymbol(char prop[], int m, int n, HashTable *hash) 5 ");
    }

    prp = setPointerToProp(prop, hash, m, n);
    allocatePropNode(prop, &((*hash) -> node_ptr), prp, strlen(prop), n);

    //if (*((*hash) -> node_ptr -> name -> s) != strlen(prop))
    //    InconsistencyErr("void addSymbol(char prop[], HashTable **hash, int m, int n)");

    if (strcmp((*hash) -> node_ptr -> name -> stc, prop) != 0)
        strcpy((*hash) -> node_ptr -> name -> stc, prop);
}

void addArg(char prop[], char arg[], HashTable **hash, int m, int n)
{   int rgu;
    ArgNode *arg_ptr = NULL;
    //checkHashTableNonEmpty(hash);
    addSymbol(prop, hash, m, n);
    checkHashNodeNonEmpty(&((*hash) -> node_ptr));
    //setPointerToProp(prop, hash, m, n);
    rgu = setPointerToArg(arg, hash, m, n);
    allocateArgNode(arg, &((*hash) -> node_ptr -> arg_ptr), rgu, strlen(arg), n);

    arg_ptr = (*hash) -> node_ptr -> arg_ptr;

    if (*(arg_ptr -> arg -> s) != strlen(arg))
        InconsistencyErr("void addArg(char prop[], char arg[], HashTable **hash, int m, int n)");
    if (strcmp(arg_ptr -> arg -> stc, arg) != 0)
        strcpy(arg_ptr -> arg -> stc, arg);
    //*(arg_ptr -> result) = -1;
}
