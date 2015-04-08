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
