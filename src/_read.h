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
