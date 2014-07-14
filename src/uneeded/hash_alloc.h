typedef struct MyString
{   int *s;
    char *stc;
} MyString;

typedef struct ArgNode
{   MyString *arg; // Argument name
    int *result;        // Result of passing argument arg to prpsosition

    struct ArgNode *next;   // Next hash node.
} ArgNode;

// Node in hash table
typedef struct HashNode
{   int *n;             // Length of array held by node
    ArgNode *arg_ptr;   // Pointer variable used to set and retriev 
                        // values correspoinding to arguments

    ArgNode *array;     // Array holding values corresponding to the 
                        // different arguments to the prpsosition

    MyString *name;// String corresponding to the name
                        // of the prpsosition
    struct HashNode *next;
    struct HashNode *head;
} HashNode;

// Hash Table
typedef struct HashTable
{   /* This hash table avoids collisions by implementing 
     * an array of linked lists.
     *
     * Each element in the array corresponds to a prpsositional
     * statement. Each array contained in each linked list node
     * corresponds to the values of the prpsosition given each argument.
     */

    int *m;             // Number of hash nodes
    int *n;             // Number of elements per hash node
    //char *args;       // Array of argument names
    HashNode *node_ptr; // Pointer so that gcc will let me allocate 
                        // structs in the array
    HashNode *nodes;    // Array of HashNode structs
} HashTable;

void checkHashTableEmpty(HashTable **hash)
{   if ((*hash) == NULL)
        DeathErr("void checkHashTableEmpty(HashTable **hash) 1 ");
    if ((*hash) -> nodes != NULL)
        DeathErr("void checkHashTableEmpty(HashTable **hash) 2 ");
    if ((*hash) -> m != NULL)
        DeathErr("void checkHashTableempty(HashTable **hash) 3 ");
    if ((*hash) -> n != NULL)
        DeathErr("void checkHashTableempty(HashTable **hash) 4 ");
}

void checkHashTableNonEmpty(HashTable **hash)
{   if (hash == NULL)
        DeathErr("Something wrong with compiler");
    if (*hash == NULL)
        DeathErr("void checkHashTableEmpty(HashTable **hash) 2 ");
    if ((*hash) -> nodes == NULL)                                    
        DeathErr("void checkHashTableEmpty(HashTable **hash) 3 ");
    if ((*hash) -> m == NULL)
        DeathErr("void checkHashTableempty(HashTable **hash) 4 ");
    if ((*hash) -> n == NULL)
        DeathErr("void checkHashTableempty(HashTable **hash) 5 ");
}

void checkHashNodeEmpty(HashNode **node_ptr_ptr)
{   // If the DeathErr() triggers, you're doomed
    if (node_ptr_ptr == NULL)
        NoMallocErr("void checkHashNodeEmpty(HashNode **node_ptr_ptr) a ");
    if ((*node_ptr_ptr) == NULL)
        NoMallocErr("void checkHashNodeEmpty(HashNode **node_ptr_ptr) b ");
    if ((*node_ptr_ptr) -> n != NULL) 
        DeathErr("void checkHashNodeEmpty(HashNode **node_ptr_ptr, int n) 1 ");
    if ((*node_ptr_ptr) -> array != NULL) 
        DeathErr("void checkHashNodeEmpty(HashNode **node_ptr_ptr, int n) 2 ");
    if ((*node_ptr_ptr) -> name != NULL) 
        DeathErr("void checkHashNodeNameEmpty(HashNode **node_ptr_ptr, int n) 3 ");
    //if ((*node_ptr_ptr) -> next != NULL) 
    //    DeathErr("void checkHashNodeEmpty(HashNode **node_ptr_ptr, int n) 4 ");
    //if ((*node_ptr_ptr) -> head != NULL) 
    //    DeathErr("void checkHashNodeEmpty(HashNode **node_ptr_ptr, int n) 5 ");
}

void checkHashNodeNonEmpty(HashNode **node_ptr_ptr)
{   // If the DeathErr() triggers, you're doomed
    if ((node_ptr_ptr) == NULL)
        DeathErr("Something wrong with compiler");
    if ((*node_ptr_ptr) == NULL)
        DeathErr("void chechHashNodeNonEmpty(HashNode **node_ptr_ptr, int n) 1 "); 
    if ((*node_ptr_ptr) -> n == NULL) 
        DeathErr("void chechHashNodeNonEmpty(HashNode **node_ptr_ptr, int n) 2 ");
    if ((*node_ptr_ptr) -> array == NULL) 
        DeathErr("void chechHashNodeNonEmpty(HashNode **node_ptr_ptr, int n) 3 ");
    if ((*node_ptr_ptr) -> name == NULL) 
        DeathErr("void chechHashNodeNonEmpty(HashNode **node_ptr_ptr, int n) 4 ");
    //if ((*node_ptr_ptr) -> next == NULL) 
    //    DeathErr("void chechHashNodeNonEmpty(HashNode **node_ptr_ptr, int n) 4 ");
    //if ((*node_ptr_ptr) -> head == NULL) 
    //    DeathErr("void chechHashNodeNonEmpty(HashNode **node_ptr_ptr, int n) 5 ");
}

void checkHashNodeNonEmpty2(HashNode **node_ptr_ptr)
{   if ((node_ptr_ptr) == NULL)
        DeathErr("Prepare to die.....");
    if ((*node_ptr_ptr) == NULL)
        NoMallocErr("void allocateEmptyArgNode(HashTable **node_ptr_ptr) 1 ");
    if ((*node_ptr_ptr) -> name == NULL)
        NoMallocErr("void allocateEmptyArgNode(HashTable **node_ptr_ptr) 3 ");
    if ((*node_ptr_ptr) -> array == NULL)
        NoMallocErr("void allocateEmptyArgNode(HashTable **node_ptr_ptr) 4 ");
    //if ((*node_ptr_ptr) -> node_ptr -> arg_ptr == NULL)
    //    DeathErr("void allocateEmptyArgNode(HashTable **node_ptr_ptr) 5 ");
}

void checkArgNodeNonEmpty(ArgNode **arg_ptr)
{   if (arg_ptr == NULL)
        DeathErr("Prepare to die.....");
    if ((*arg_ptr) == NULL)
        NoMallocErr("void allocateNonEmptyArgNode(ArgNode **arg_ptr) 1 ");
    if ((*arg_ptr) -> arg == NULL)
        NoMallocErr("void allocateNonEmptyArgNode(ArgNode **arg_ptr) 2 ");
    if ((*arg_ptr) -> result == NULL)
        NoMallocErr("void allocateNonEmptyArgNode(ArgNode **arg_ptr) 3 ");
}

void checkArgNodeEmpty(ArgNode **arg_ptr)
{   if (arg_ptr == NULL)
        DeathErr("Your compiler is on drugs");
    //if ((*arg_ptr) != NULL)
    //    NoMallocErr("void allocateEmptyArgNode(ArgNode **arg_ptr) 1 ");
    if ((*arg_ptr) -> result != NULL)
        NoMallocErr("void allocateEmptyArgNode(ArgNode **arg_ptr) 2 ");
    if ((*arg_ptr) -> next != NULL)
        NoMallocErr("void allocateEmptyArgNode(ArgNode **arg_ptr) 3 ");
}

void checkArgNodeEmpty2(ArgNode **arg_ptr)
{   //if ((*arg_ptr) != NULL)
    //    NoMallocErr("void allocateEmptyArgNode(ArgNode **arg_ptr) 1 ");
    if (arg_ptr == NULL)
        DeathErr("Your compiler is on drugs");
    if ((*arg_ptr) -> result != NULL)
        NoMallocErr("void allocateEmptyArgNode(ArgNode **arg_ptr) 2 ");
    if ((*arg_ptr) -> next != NULL)
        NoMallocErr("void allocateEmptyArgNode(ArgNode **arg_ptr) 3 ");
}

void checkStringEmpty(MyString **arg_ptr_ptr)
{   if (arg_ptr_ptr == NULL)
        DeathErr("Something wrong with compiler");
    if ((*arg_ptr_ptr) == NULL)
        DeathErr("checkStringEmpty");
    if ((*arg_ptr_ptr) -> stc != NULL)
        DeathErr("checkStringEmpty");
    if ((*arg_ptr_ptr) -> s != NULL)
        DeathErr("checkStringEmpty");
}

void checkStringNonEmpty(MyString **arg_name)
{   if ((*arg_name) == NULL)
        NoMallocErr("checkStringEmpty");
    if ((*arg_name) -> stc == NULL)
        NoMallocErr("checkStringEmpty");
    if ((*arg_name) -> s == NULL)
        NoMallocErr("checkStringEmpty");
}
HashTable *generateEmptyHashTable()
{   /* This generates a hash table with m symbols and 
     * n possible arguments for each symbol.
     */

    HashTable **hash_ptr = NULL;

    hash_ptr = (HashTable **) malloc(sizeof(HashTable *));

    if (hash_ptr == NULL) 
        MallocErr("HashTable *generateEmptyHashTable() 1 ");

    (*hash_ptr) = NULL;
    (*hash_ptr) = (HashTable *) malloc(sizeof(HashTable));
    if ((*hash_ptr) == NULL) 
        MallocErr("HashTable *generateEmptyHashTable() 2 ");

    (*hash_ptr) -> m = NULL;
    (*hash_ptr) -> n = NULL;
    (*hash_ptr) -> node_ptr = NULL;
    (*hash_ptr) -> nodes = NULL;

    return *hash_ptr;
}

HashTable *GenerateAllocatedHashTable(int m, int n)
{   /* Generates a hash table with an array of m linked lists
     * of prpsositional symbols
     * 
     * Each prpsositional symbol has n arguments
     */

    HashTable *hash = generateEmptyHashTable();

    if (hash -> m != NULL || hash -> n != NULL 
            || hash -> nodes != NULL || hash -> node_ptr != NULL) 
        DeathErr("HashTable *GenerateAllocatedHas(int m, int n) 1 ");

    hash -> m = (int *) malloc(sizeof(int));
    hash -> n = (int *) malloc(sizeof(int));
    hash -> nodes = (HashNode *) calloc(m, sizeof(HashNode));

    if (hash -> m == NULL || hash -> n == NULL || hash -> nodes == NULL) 
        MallocErr("HashTable *GenerateAllocatedHas(int m, int n) 2 ");

    *(hash -> m) = m;
    *(hash -> n) = n;

    return hash;
}

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
     * of the prpsositional function for each argument.
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
     * arg_ptr_ptr -> node_ptr -> arg_ptr pointer to the apprpsriate
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

void allocatePrpsNode(char prps[], HashNode **node_ptr_ptr, int prp, int s, int n)
{

    if (prp == -1)
    {   allocateNonEmptyHashNode(node_ptr_ptr, s, n);
        allocateEmptyHashNode(&((*node_ptr_ptr) -> next));
    } else if (prp == 1)
    {   allocateNonEmptyHashNode(node_ptr_ptr, s, n);
        allocateEmptyHashNode(&((*node_ptr_ptr) -> next));
    } else if (prp == 0)
        InconsistencyErr("void allocatePrpsNode(args...)");
    if ((*node_ptr_ptr) -> name -> stc == NULL)
        DeathErr("void setPointerToPrps(char prps[], HashTable **hash, int m, int n)");
}

void allocateArgNode(char arg[], ArgNode **arg_ptr_ptr, int rgu, int s, int n)
{   if (rgu == -1)
    {   allocateNonEmptyArgNode(arg_ptr_ptr, s, n);
        allocateEmptyArgNode(&((*arg_ptr_ptr) -> next));
    } else if (rgu == 1)
    {   allocateNonEmptyArgNode(arg_ptr_ptr, s, n);
        allocateEmptyArgNode(&((*arg_ptr_ptr) -> next));
    } else if (rgu == 0)
        InconsistencyErr("void allocatePrpsNode(args...)");
    if ((*arg_ptr_ptr) -> arg == NULL)
        DeathErr("void setPointerToPrps(char prps[], HashTable **hash, int m, int n)");
}

int setPointerToPrps(char prps[], HashTable **hash, int m, int n)
{   if (*hash == NULL) 
        NoMallocErr("void setPointerToPrps(char prps[], HashTable **hash, int m, int n) 1 ");

    int index = GetPrpsIndex(prps, m);
    HashNode *tail_ptr;

    (*hash) -> node_ptr = (*hash) -> nodes + index * sizeof(HashNode);

    while (1)
    {   if ((*hash) -> node_ptr == NULL) return -1;
        if ((*hash) -> node_ptr -> name == NULL) return 1; 
        if ((*hash) -> node_ptr -> name -> stc == NULL)
            DeathErr("void setPointerToPrps(char prps[], HashTable **hash, int m, int n)");

        if (strcmp((*hash) -> node_ptr -> name -> stc, prps) == 0) return 0;

        (*hash) -> node_ptr = (*hash) -> node_ptr -> next;
    }
}

int setPointerToArg(char arg[], HashTable **hash, int m, int n)
{   int index = GetArgIndex(arg, n);
    ArgNode *arg_ptr = NULL;

    checkHashTableNonEmpty(hash);
    checkHashNodeNonEmpty(&((*hash) -> node_ptr));

    // This function requires that hash -> node_ptr has already been apprpsriately set
    (*hash) -> node_ptr -> arg_ptr = (*hash) -> node_ptr -> array + index * sizeof(ArgNode);
    arg_ptr = (*hash) -> node_ptr -> arg_ptr;

    while (1)
    {   if (arg_ptr == NULL) return -1;
        if (arg_ptr -> arg == NULL) return 1;
        if (arg_ptr -> arg -> stc == NULL)
            DeathErr("void setPointerToArg(args...)");
        if (strcmp(arg_ptr -> arg -> stc, arg) == 0) return 0;

        arg_ptr = arg_ptr -> next;
        (*hash) -> node_ptr -> arg_ptr = arg_ptr;
    }
}

void addSymbol(char prps[], HashTable **hash, int m, int n)
{   /* Adds the string prps[] to the prpsosition table if not
     * already present. Otherwise, the pointer (*hash) -> node_ptr
     * is set to point to the HashNode containing prps[]
     *
     * m is the number of Prpsositional functions in 
     * the knowledge base, in case youve forgotten.
     *
     * I'll let you guess what n is...
     */

    int i, index, prp;

    checkHashTableNonEmpty(hash);

    if (*((*hash) -> m) != m) 
        InconsistencyErr("void addSymbol(char prps[], int m, int n, HashTable *hash) 3 ");

    // Get index to put symbol in
    index = GetPrpsIndex(prps, m);

    if (index >= m || index < 0) 
    {   fprintf(stderr, "m = %d\n", m);
        InconsistencyErr("void addSymbol(char prps[], int m, int n, HashTable *hash) 5 ");
    }

    prp = setPointerToPrps(prps, hash, m, n);
    allocatePrpsNode(prps, &((*hash) -> node_ptr), prp, strlen(prps), n);

    if (*((*hash) -> node_ptr -> name -> s) != strlen(prps))
        InconsistencyErr("void addSymbol(char prps[], HashTable **hash, int m, int n)");

    if (strcmp((*hash) -> node_ptr -> name -> stc, prps) != 0)
        strcpy((*hash) -> node_ptr -> name -> stc, prps);
}

void addArg(char prps[], char arg[], HashTable **hash, int m, int n)
{   int rgu;
    ArgNode *arg_ptr = NULL;
    //checkHashTableNonEmpty(hash);
    addSymbol(prps, hash, m, n);
    checkHashNodeNonEmpty(&((*hash) -> node_ptr));
    //setPointerToPrps(prps, hash, m, n);
    rgu = setPointerToArg(arg, hash, m, n);
    allocateArgNode(arg, &((*hash) -> node_ptr -> arg_ptr), rgu, strlen(arg), n);

    arg_ptr = (*hash) -> node_ptr -> arg_ptr;

    if (*(arg_ptr -> arg -> s) != strlen(arg))
        InconsistencyErr("void addArg(char prps[], char arg[], HashTable **hash, int m, int n)");
    if (strcmp(arg_ptr -> arg -> stc, arg) != 0)
        strcpy(arg_ptr -> arg -> stc, arg);
    //*(arg_ptr -> result) = -1;
}
