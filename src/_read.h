char **ReadKB(int m, const char *filename)
{
    char **kb_string;
    FILE *f = fopen(filename, "r");
    int n[m];
    int i;

    kb_string = calloc(m, sizeof(char *));
    if (kb_string == NULL)
        MallocErr("ReadKB 1");

    for (i = 0; i < m; i++)
        getline(&(kb_string[i]), &(n[i]), f);

    fclose(f);

    return kb_string;
}

/*void LearnKB(HashTable **hash, char **kb_string, int m, int h, int k)
{
    int i = 0, j;
    int n = strlen(kb_string[0]);
    int hash_val;

    char **args = NULL, **prps = NULL;
    args = calloc(n-2, sizeof(char *));
    prps = calloc(m-1, sizeof(char *));

    if (args == NULL || prps == NULL)
        MallocErr("LearnKB 1");

    for (j = 1; j < n-1; j++)
    {   args[j-1] = calloc(2, sizeof(char));
        if (args[j-1] == NULL)
            MallocErr("LearnKB 3");
        args[j-1][0] = kb_string[0][j];
        args[j-1][1] = '\0';
    }

    for (j = 0; j < m-1; j++)
    {   
        prps[j] = calloc(2, sizeof(char));
        if (prps[j] == NULL)
            MallocErr("LearnKB 2");

        prps[j][0] = kb_string[j+1][0];
        prps[j][1] = '\0';
    }

    for (i = 1; i < m; i++)
        for (j = 1; j < n-1; j++)
        {
            if (kb_string[i][j] == ' ')
                hash_val = -1;
            else if (kb_string[i][j] == '1')
                hash_val = 1;
            else if (kb_string[i][j] == '0')
                hash_val = 0;

            SetValueForArg(prps[i-1], args[j-1], hash_val, hash, h, k);
        }
}*/

char *ReadPRPS(const char *filename)
{
    char *prps = NULL, *return_val = NULL;
    int n = 0, size;

    FILE *f = fopen(filename, "r");

    size = getline(&prps, &n, f);

    return_val = calloc(size, sizeof(char));
    if (return_val == NULL)
        MallocErr("ReadPRPS 1");

    memcpy(return_val, prps, (size-1)*sizeof(char));
    return_val[size-1] = '\0';
    free(prps); prps = NULL;
    fclose(f);

    return return_val;
}
