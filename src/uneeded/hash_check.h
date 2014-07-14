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
