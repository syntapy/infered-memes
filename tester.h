//#include "prps.h"
// 65 - 90: A - Z
// 97 - 122: a - z

void GetLeftMostByte(int *f, int *nmbr)
{
    *nmbr = *f;
    *f = *f >> 8;
    *f = *f << 8;

    *nmbr = *nmbr & ~(*f);
}

void GenSequentialSymbol(char **symbol, int f, int m)
{
    /* This function wou
        m is the number of chars needed in the symbol
     */
    int i, j, value;
    unsigned long int max_val;
    if (symbol == NULL)
        NoMallocErr("GenSequentialSymbols 1");

    if ((*symbol) == NULL)
    {   (*symbol) = calloc(m, sizeof(char));
        if ((*symbol) == NULL)
            MallocErr("GenSequentialSymbol 2");
    }

    max_val = ((unsigned long int) powl(2, 8*m + 1)) - 1;

    if (f > max_val)
        InconsistencyErr("GenSequentialSymbol 3");

    i = 0;
    while (f > 0)
    {
        GetLeftMostByte(&f, &value);
        value += 65;
        (*symbol)[i++] = value;
    }

    (*symbol)[i] = '\0';
}

void GenSymbol(char **symbol, int n)
{
    int i, value;
    if (symbol == NULL)
        NoMallocErr("GenSymbols 1");

    if ((*symbol) == NULL)
    {
        (*symbol) = calloc(n, sizeof(char));
        if ((*symbol) == NULL)
            MallocErr("GenSymbol 2");
    }

    for (i = 0; i < n - 1; i++)
    {   value = rand() % 26;
        value += 65;
        (*symbol)[i] = value;
    }

    (*symbol)[n-1] = '\0';
}

void GenSymbolArray(char ***symbols, unsigned long int n, unsigned int m)
{   /*  Generate an array of n strings
     *
     *  Not sure if the m argument (arg 2) is useful. Supposedly it is 
     *  the max # of chars per string, but if its too small it gets set 
     *  to the min amount
     */

    unsigned long int i, j;
    //unsigned long int m;
    char *symbolStore = NULL;
    char *buffer = NULL;
    unsigned long int min_m = (unsigned int) ceil(log2l(n) / 3);

    if (m < min_m)
        m = min_m;

    if (symbols == NULL)
        NoMallocErr("GenSymbolArray");

    symbolStore = calloc((m + 1) * n, sizeof(char));
    (*symbols) = calloc(n, sizeof(char *));
    //buffer = calloc(n, sizeof(char));

    if ((*symbols) == NULL)
        MallocErr("GenSymbolArray 2");
    if (symbolStore == NULL)
        MallocErr("GenSymbolArray 3");
    //if (buffer == NULL)
    //    MallocErr("GenSymbolArray 4");

    for (i = 0; i < n; i++)
    {   (*symbols)[i] = &(symbolStore[i * (m + 1)]);
        GenSequentialSymbol(&((*symbols)[i]), i+1, m);
    }
}

//void GenArgArray(char ***args, unsigned long int n, unsigned long int M)
//{
//
//}

void FreeSymbolArray(char ***symbols, unsigned long int n, unsigned int m)
{
    unsigned long int i, j;

    if (symbols != NULL)
    {
        free(**symbols);
        free(*symbols);

        **symbols = NULL;
        *symbols = NULL;
    }
}

void GenSkewedTree(const char **symbols, int m, PrpsTree **tree, int d)
{   int i, j, index;
    const char *ptr = NULL;
    long double nmbr;
    PrpsTree **left = NULL, **right = NULL;
    //if (tree == NULL)

    if ((*tree) == NULL)
    {   (*tree) = GenerateEmpty();
        index = rand() % 4;
        ptr = symbols[index];

        if (d > 0)
        {
            index = rand() % 4;
        	AllocateAsOprtr(tree, index);

            left = calloc(1, sizeof(PrpsTree *));
            right = calloc(1, sizeof(PrpsTree *));

            if (!left || !right)
                MallocErr("GenSkewedTree 1");

            nmbr = (long double) rand();
            nmbr = (d - 1) * ((int) rintl(logl(nmbr) / logl((long double) RAND_MAX)));

            GenSkewedTree(symbols, m, left, d - 1);
            GenSkewedTree(symbols, m, right, d - 1 - nmbr);

            SetLeftChild(tree, left);
            SetRightChild(tree, right);

            free(left);
            free(right);

            left = NULL;
            right = NULL;
        }

        else
        {   
            j = rand() % m;
            ptr = symbols[j];
            AllocateAsPrps(tree, ptr);

            //for (i = 0; i < d; i++)
            //{   
            //    tree = Oprtr2
            //           (rand() % 3, tree,
            //            rand() % 3, ptr,
            //            rand() % 3, rand() % 4);
            //}
        }

        if (rand() > RAND_MAX / 2)
            Negate(tree);
    }
}

void GenAlphaTree(const char **symbols, PrpsTree ***alpha, int m, int n, int d)
{
    int i, j;
    const char *symbol_tmp = NULL;
    //PrpsTree **alpha = NULL;

    if (alpha == NULL)
        NoMallocErr("GenAlphaTree 1");

    if ((*alpha) != NULL)
        InconsistencyErr("GenAlphaTree 2");

    (*alpha) = calloc(1, sizeof(PrpsTree *));
    if ((*alpha) == NULL)
        MallocErr("GenAlphaTree 3");
    (**alpha) = GenerateEmpty();

    i = rand() % n;

    symbol_tmp = symbols[i];
    AllocateAsPrps(*alpha, symbol_tmp);
}

void FillHashTable(HashTable **hash, char **symbols, char **args, int m, int n, int seed)
{
    int i, j, value;
    int a, b;
    int iterations;

    srand(seed);

    for (i = 0; i < m; i++)
    {
        iterations = rand() % n;

        for (j = 0; j < iterations; j++)
        {
            value = rand() % 2; 
            a = rand() % m;
            b = rand() % n;

            SetValueForArg(symbols[a], args[b], value, hash, m, n);
        }
    }
}
