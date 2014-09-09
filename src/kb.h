//void AddSymbol(char *symbol, char *arg, HashTable **hash)
//{
//    
//}

int GetNodeValue(PrpsTree **node, HashTable **hash, int m, int n)
{
    int return_val = -1;
    if (PrpsNode(node))
    {
        return_val = GetValueForArg((*node) -> stmnt -> stc, 
            (*node) -> argmnt -> stc, hash, m, n);
        if (*((*node) -> neg) == 1)
            return_val = !return_val;
    }

    return return_val;
}

int KB_And(int left_val, int right_val)
{
    int return_val = -1;

    if (left_val != -1 && right_val != -1)
        return_val = left_val && right_val;
    
    return return_val;
}

int KB_Or(int left_val, int right_val)
{
    int return_val = -1;

    if (left_val == 1 || right_val == 1)
        return_val = 1;
    else if (left_val == 0 && right_val == 0)
        return_val = 0;

    return return_val;
}

int KB_Tau(int left_val, int right_val)
{
    int return_val = -1;

    if (left_val == 0 || left_val == 1)
        if (right_val == 0 || right_val == 1)
            return_val = (left_val == right_val);
    
    return return_val;
}

int KB_Not(int val)
{
    int return_val = -1;

    if (val == 1 || val == 0)
        return_val = !val;

    return return_val;
}

int GetTreeValue(PrpsTree **tree, HashTable **hash, int m, int n)
{   int return_val = -1;
    int left_val = -1, right_val = -1;

    if (PrpsNode(tree))
        return_val = GetNodeValue(tree, hash, m, n);

    else if (OprtrNode(tree))
    {
        left_val = GetTreeValue(&((*tree) -> left), hash, m, n);
        right_val = GetTreeValue(&((*tree) -> right), hash, m, n);

        if (left_val != 0 && left_val != 1 && left_val != -1)
            InconsistencyErr("GetTreeValue 2");

        if (right_val != 0 && right_val != 1 && right_val != -1)
            InconsistencyErr("GetTreeValue 2");

        if (*((*tree) -> oprtr) == AND)
            return_val = KB_And(left_val, right_val);

        else if (*((*tree) -> oprtr) == OR)
            return_val = KB_Or(left_val, right_val);

        else if (*((*tree) -> oprtr) == IMP)
            return_val = KB_Or(KB_Not(left_val), right_val);

        else if (*((*tree) -> oprtr) == TAU)
            return_val = KB_Tau(left_val, right_val);

        else
            InconsistencyErr("GetTreeValue 1 ");
    }

    return return_val;
}
