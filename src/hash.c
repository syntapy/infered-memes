#include "prps.h"
#include "hash.h"

#include "_hash_check.h"

HashTable **generateEmptyHashTable()
{   /* This generates a hash table with m symbols and 
     * n possible arguments for each symbol.
     */

    HashTable **hash_ptr = NULL;

    hash_ptr = (HashTable **) calloc(1, sizeof(HashTable *));

    if (hash_ptr == NULL) 
        MallocErr("HashTable *generateEmptyHashTable() 1 ");

    (*hash_ptr) = (HashTable *) calloc(1, sizeof(HashTable));

    if ((*hash_ptr) == NULL)
        MallocErr("HashTable *generateEmptyHashTable() 1 ");

    (*hash_ptr) -> m = NULL;
    (*hash_ptr) -> n = NULL;
    (*hash_ptr) -> node_ptr = NULL;
    (*hash_ptr) -> nodes = NULL;

    return hash_ptr;
}

HashTable **GenerateAllocatedHashTable(int m, int n)
{   /* Generates a hash table with an array of m linked lists
     * of propositional symbols
     * 
     * Each propositional symbol has n arguments
     */

    HashTable **hash = generateEmptyHashTable();

    if ((*hash) -> m != NULL || (*hash) -> n != NULL 
            || (*hash) -> nodes != NULL || (*hash) -> node_ptr != NULL) 
        DeathErr("HashTable *GenerateAllocatedHas(int m, int n) 1 ");

    (*hash) -> m = (int *) malloc(sizeof(int));
    (*hash) -> n = (int *) malloc(sizeof(int));
    (*hash) -> nodes = (HashNode *) calloc(m, sizeof(HashNode));

    if ((*hash) -> m == NULL || (*hash) -> n == NULL || (*hash) -> nodes == NULL) 
        MallocErr("HashTable *GenerateAllocatedHas(int m, int n) 2 ");

    *((*hash) -> m) = m;
    *((*hash) -> n) = n;

    return hash;
}

#include "_hash_alloc.h"

int GetPropIndex(char prop[], int m)
{
    /* This function takes a string and returns a 
     * hash table index. 
     *
     * m is the number of symbols in the hash table.
     */

    unsigned int i, N, s = 0;

    N = strlen(prop);

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
    addArg(prop, arg, hash, m, n);
    arg_ptr = (*hash) -> node_ptr -> arg_ptr;

    if (arg_ptr -> arg == NULL)
        InconsistencyErr("void setValueForArg(char arg, HashTable *hash, int m, int n, int value) 1 ");

    *((*hash) -> node_ptr -> arg_ptr -> result) = value;
}

int GetValueForArg(char prop[], char arg[], HashTable **hash, int m, int n)
{   
    /*  returns the value for the function denoted by prop[]
        when passed argument denoted by arg[]

        if either prop[] isn't in the hash table, or arg[] isn't in the hash
        table pointed to by node containing prop[], then function returns -1
     */


    int return_val;
    int prp, rgu;
    checkHashTableNonEmpty(hash);

    prp = setPointerToProp(prop, hash, m, n);

    if (prp == -1)
        return_val = -1;
    else if (prp == 1)
        return_val = -1;
    else if (prp == 0)
    {   rgu = setPointerToArg(arg, hash, m, n);
        if (rgu == -1)
            return_val = -1;
        else if (rgu == 1)
            return_val = -1;
        else if (rgu == 0)
            return_val = *((*hash) -> node_ptr -> arg_ptr -> result);
        else return_val = -1;
    } 
    else return_val = -1;

    return return_val;
}
