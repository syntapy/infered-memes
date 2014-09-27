#ifndef HASH_H
#define HASH_H

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

    MyString *name;     // String corresponding to the name
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

char **CopyArgs(PrpsTree **node, int *n_args);
char *GenPrps(char *input, int *i);
char **GenArgs(char *input, int *i);
int CompareArgs(char **args_a, char **args_b, int n_args_a, int n_args_b);

#endif
