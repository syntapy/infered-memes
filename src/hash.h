typedef struct MyString
{   int *s;
    char *stc;
} MyString;

typedef struct ArgNode
{   MyString *arg; // Argument name
    int *result;        // Result of passing argument arg to proposition

    struct ArgNode *next;   // Next hash node.
} ArgNode;

// Node in hash table
typedef struct HashNode
{   int *n;             // Length of array held by node
    ArgNode *arg_ptr;   // Pointer variable used to set and retriev 
                        // values correspoinding to arguments

    ArgNode *array;     // Array holding values corresponding to the 
                        // different arguments to the proposition

    MyString *name;// String corresponding to the name
                        // of the proposition
    struct HashNode *next;
    struct HashNode *head;
} HashNode;

// Hash Table
typedef struct HashTable
{   /* This hash table avoids collisions by implementing 
     * an array of linked lists.
     *
     * Each element in the array corresponds to a propositional
     * statement. Each array contained in each linked list node
     * corresponds to the values of the proposition given each argument.
     */

    int *m;             // Number of hash nodes
    int *n;             // Number of elements per hash node
    //char *args;       // Array of argument names
    HashNode *node_ptr; // Pointer so that gcc will let me allocate 
                        // structs in the array
    HashNode *nodes;    // Array of HashNode structs
} HashTable;

#include "_hash_check.h"

HashTable *generateEmptyHashTable()
{   /* This generates a hash table with m symbols and 
     * n possible arguments for each symbol.
     */

    HashTable *hash_ptr = NULL;

    hash_ptr = (HashTable *) calloc(1, sizeof(HashTable));

    if (hash_ptr == NULL) 
        MallocErr("HashTable *generateEmptyHashTable() 1 ");

    //hash_ptr = NULL;
    //hash_ptr = (HashTable *) malloc(sizeof(HashTable));
    //if ((*hash_ptr) == NULL) 
    //    MallocErr("HashTable *generateEmptyHashTable() 2 ");

    hash_ptr -> m = NULL;
    hash_ptr -> n = NULL;
    hash_ptr -> node_ptr = NULL;
    hash_ptr -> nodes = NULL;

    return hash_ptr;
}

HashTable *GenerateAllocatedHashTable(int m, int n)
{   /* Generates a hash table with an array of m linked lists
     * of propositional symbols
     * 
     * Each propositional symbol has n arguments
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

#include "_hash_alloc.h"

int GetPropIndex(char prop[], int m)
{   /* This function takes a string and returns a 
     * hash table index. 
     *
     * m is the number of symbols in the hash table.
     */

    unsigned int i, N, s = 0;

    N = strlen(prop);

    //if (N > MAXCHARS)
    //    MaxCharsExceededWarn(prop);

    srand(677);

    for (i = 0; i < N; i++)
    {   s += (int) prop[i];   
        s += rand();
    }
    s %= m;
    return s;
}

int GetArgIndex(char arg[], int n)
{   /* This function takes a char and returns
     * a hash table index.
     *
     * n is the number of arguments to each function
     */

    unsigned int i, N, s = 0;

    N = strlen(arg);

    //if (N > MAXCHARS)
    //    MaxCharsExceededWarn(arg);

    srand(967);

    for (i = 0; i < N; i++)
    {   s += (int) arg[i];
        s += rand();
    }
    s %= n;
    return s;
}

void SetValueForArg(char prop[], char arg[], int value, HashTable **hash, int m, int n)
{   /* This function actually sets the value returned when you
     * pass argument arg to propositional function pointed to
     * by hash -> node_ptr -> name
     */

    ArgNode *arg_ptr;
    //setPointerToArg(arg, hash, m, n);
    addArg(prop, arg, hash, m, n);
    arg_ptr = (*hash) -> node_ptr -> arg_ptr;

    if (arg_ptr -> arg == NULL)
        InconsistencyErr("void setValueForArg(char arg, HashTable *hash, int m, int n, int value) 1 ");

    *((*hash) -> node_ptr -> arg_ptr -> result) = value;
}

int GetValueForArg(char prop[], char arg[], HashTable **hash, int m, int n)
{   int prp, rgu;
    //ArgNode *arg_ptr = NULL;
    checkHashTableNonEmpty(hash);
    //checkHashNodeNonEmpty(&((*hash) -> node_ptr));

    prp = setPointerToProp(prop, hash, m, n);
    if (prp == -1)
        return -1;
    if (prp == 1)
        return -1;
    if (prp == 0)
    {   rgu = setPointerToArg(arg, hash, m, n);
        if (rgu == -1)
            return -1;
        if (rgu == 1)
            return -1;
        if (rgu == 0)
            return *((*hash) -> node_ptr -> arg_ptr -> result);
        else return -1;
    } else return -1;

    //allocatePropNode(prop, &((*hash) -> node_ptr), prp, strlen(prop), n);
    //rgu = setPointerToArg(arg, hash, m, n);
    //allocateArgNode(arg, &((*hash) -> node_ptr -> arg_ptr), rgu, strlen(arg), n);

    //arg_ptr = (*hash) -> node_ptr -> arg_ptr;

    //return *(arg_ptr -> result);
}
