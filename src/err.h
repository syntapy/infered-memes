void MallocErr(const char *msg) 
{   fprintf(stderr, "In function: %s:", msg);
    fprintf(stderr, "Problem Allocating Memory\n");
    exit(2);
}

void NoMallocErr(const char *msg)
{   fprintf(stderr, "In function: %s:", msg);
    fprintf(stderr, "No Memory Allocated for pointer\n");
    exit(3);
}

void AlreadyMallocErr(const char *msg)
{   fprintf(stderr, "In function: %s:", msg);
    fprintf(stderr, "Memory Already Allocated for pointer\n");
    exit(4);
}

void InconsistencyErr(const char *msg)
{   fprintf(stderr, "In function: %s:", msg);
    fprintf(stderr, "Inconsistent parameremters\n");
    //while (1)
    //    sleep(2);
    exit(5);
}

void ParseErr(const char *msg)
{   fprintf(stderr, "In function: %s:", msg);
    fprintf(stderr, "Problem Parsing Statment\n");
    exit(1);
}

void DeathErr(const char *msg)
{   fprintf(stderr, "In function: %s:", msg);
    fprintf(stderr, "We're all gonna die......\n");
    
    //while (1)
    //    sleep(2);
    
    exit(666);
}

void TestErr(const char *msg)
{   fprintf(stderr, "In function: %s:", msg);
    fprintf(stderr, "TESTING\n");
    exit(667);
}

void NoArgWarn(const char *msg)
{   fprintf(stderr, "WARNING In function: %s:", msg);
    fprintf(stderr, "No such agrument....yet\n");
    //exit(911);
}

void MaxCharsExceededWarn(const char *chars)
{   fprintf(stderr, "WARNING: ");
    fprintf(stderr, "String length exceeds MAXCHARS: %s\n", chars);
}
