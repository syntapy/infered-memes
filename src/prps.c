#include "prps.h"

//#include "hash.h"

#include "_prps_err.h"

#include "_tree_gen.h"

/* T R E E - O P E R A T I O N - F U N C T I O N S */
void SetLeftChild(PrpsTree **parent, PrpsTree **child)
{   /* Sets (*parent) -> left to point to (*child) */
    if (parent == NULL || *parent == NULL || child == NULL || 
            (*child) == NULL/* || (*child) -> p == NULL*/) 
        MallocErr("void SetLeftChild(PrpsTree **child, PrpsTree **parent");
    if ((*parent) -> left == NULL)
    {   (*parent) -> left = *child;
        if ((*child) -> p == NULL)
            (*child) -> p = *parent;
        else InconsistencyErr("SetLeftChild");
        //free(child);
        //child = NULL;
    }

    else InconsistencyErr("void SetLeftChild(PrpsTree **child, PrpsTree **parent");

    //printf("&(**child) == (*parent) -> left: %d\n", &(**child) == (*parent) -> left);
    //while (1);
}

void SetRightChild(PrpsTree **parent, PrpsTree **child)
{   /* Sets (*parent) -> right to point to (*child) */

    if (parent == NULL || *parent == NULL || child == NULL || 
            *child == NULL/* || (*child) -> p == NULL*/) 
        MallocErr("void SetRightChild(PrpsTree **child, PrpsTree **parent");

    if ((*parent) -> right == NULL)
    {
        (*parent) -> right = *child;
        if ((*child) -> p == NULL)
             (*child) -> p = *parent;
        else InconsistencyErr("SetRightChild");
    }

    else MallocErr("void SetRightChild(PrpsTree **child, PrpsTree **parent");
}

void SetChild(PrpsTree **parent, PrpsTree **child, int dir)
{
    if (parent != NULL)
    {   if (dir == LEFT)
    	{
            SetLeftChild(parent, child);
            //child = NULL;
    	}

        else if (dir == RIGHT)
        {
            SetRightChild(parent, child);
            //child = NULL;
        }
        //else if (dir != NONE)
        //    InconsistencyErr("SetChild 1");
    }
}

PrpsTree **Oprtr(int neg_a, PrpsTree **tree_a, int neg_b, PrpsTree **tree_b, 
    int neg_oprtr, int oprtr)
{
    /* Combines propositional trees tree_a and tree_b to produce:
     *      neg_oprtr [ (neg_a tree_a) oprtr (neg_b tree_b) ]
     *
     *  Has potential for memory leak as it is right now.
     */

    PrpsTree **new_root = NULL;
    new_root = calloc(1, sizeof(PrpsTree *));
    if (new_root == NULL) 
        MallocErr("Oprtr");
    (*new_root) = GenerateEmpty();

    AllocateAsOprtr(new_root, oprtr);
    SetChild(new_root, tree_a, LEFT);
    SetChild(new_root, tree_b, RIGHT);

    if ((*tree_a) -> neg == NULL) 
        NoMallocErr("Oprtr: Negate");
    if ((*tree_b) -> neg == NULL) 
        NoMallocErr("Oprtr: Negate Right");
    if ((*new_root) -> neg == NULL)
        NoMallocErr("Oprtr: Negate Center");

    *((*tree_a) -> neg) += neg_a;
    *((*tree_b) -> neg) += neg_b;
    *((*new_root) -> neg) += neg_oprtr;

    *((*tree_a) -> neg) %= 2;
    *((*tree_b) -> neg) %= 2;
    *((*new_root) -> neg) %= 2;

    return new_root;
}

PrpsTree **Oprtr2(int neg_a, PrpsTree **tree, int neg_b, char *prps, char **arg, int n_args, int neg_oprtr, int oprtr)
{   /* Basically a wrapper function for Oprtr(args...) so that you can just combine a current tree
     * with a string statement corresponding to statement prps[]
     */

    PrpsTree *new_right_child = NULL; // This is set as the 
    new_right_child = GenerateEmpty();
    AllocateAsPrps(&new_right_child, prps, arg, n_args);
    tree = Oprtr(neg_a, tree, neg_b, &new_right_child, neg_oprtr, oprtr);
    return tree;
}

PrpsTree **OprtrWrap(int neg_a, PrpsTree **tree, int neg_b, char *prps, char **arg, 
        int n_args, int neg_oprtr, int oprtr)
{
    PrpsTree **tree_tmp = NULL;
    tree_tmp = tree;

    tree = Oprtr2(neg_a, tree_tmp, neg_b, prps, arg, n_args, neg_oprtr, oprtr);

    free(tree_tmp);

    return tree;
}

int LeafNode(PrpsTree **tree)
{   
    /* Returns TRUE if node pointed to by (*node) is
     * a leaf node, i.e. a propositional statement
     */

    int return_val = TRUE;
    //else if (/*(*tree) -> type == NULL ||*/ *((*tree) -> type) != PRPS)
    //    return_val = FALSE;
    if ((*tree) -> stmnt == NULL)// || (*tree) -> stmnt -> stc == NULL
            //|| (*tree) -> stmnt -> s == NULL)
        return_val = FALSE;
    else if ((*tree) -> argmnt == NULL)
        return_val = FALSE;
    //else if (strlen((*tree) -> stmnt -> stc) != *((*tree) -> stmnt -> s))
    //    InconsistencyErr("LeafNode 1");
    else if ((*tree) -> oprtr != NULL)
        return_val = FALSE;

    return return_val;
}

int OprtrNode(PrpsTree **tree)
{   //CheckConsistency(tree);
    int return_val = TRUE;
    //if (*((*tree) -> type) == PRPS)
    //    return_val = FALSE;
    //if (LeafNode(tree) == TRUE)
    //    return_val = FALSE;
    if ((*tree) -> stmnt != NULL)
        return_val = FALSE;
    else if ((*tree) -> argmnt != NULL)
        return_val = FALSE;
    else if ((*tree) -> oprtr == NULL)
        return_val = FALSE;

    return return_val;
}

int OprtrNodeType(PrpsTree **node, int neg, int oprtr)
{   int return_val = TRUE;
    int is_oprtr = OprtrNode(node);
    if (!is_oprtr)
        return_val = FALSE;
    else if (*((*node) -> neg) != neg)
        return_val = FALSE;
    else if (*((*node) -> oprtr) != oprtr)
        return_val = FALSE;

    return return_val;
}

int PrpsNode(PrpsTree **node)
{   int return_val = TRUE;
    //CheckConsistency(node);
    if ((*node) -> oprtr != NULL)
        return_val = FALSE;
    //if (*((*node) -> type) == OPRTR)
    //    return_val = FALSE;
    return return_val;
}

int DeepestOprtr(PrpsTree **node)
{   
    /* returns true if and only if 
     * a oprtr b, where a, b are prpositions (leaf nodes)
     * and oprtr is some logical operator: AND, OR, IMP, or TAU
     */

    int return_val = TRUE;
    int oprtr = OprtrNode(node);
    CheckConsistency(node);

    if (oprtr == FALSE)
    {   return_val = FALSE;
        if (!((*node) -> left == NULL && (*node) -> right == NULL))
            InconsistencyErr("DeepestOprtr 1");
    }

    else if (oprtr == TRUE)
    {   //CheckConsistency(&((*node) -> left));
        //CheckConsistency(&((*node) -> right));

        // DOUBLE CHECK THIS LINE
        if (!(PrpsNode(&((*node) -> left)) && PrpsNode(&((*node) -> right))))
            return_val = FALSE;
    }
    else InconsistencyErr("");
    
    return return_val;
}

int IsLeftChild(PrpsTree **tree)
{   int return_val = TRUE;

    if (tree == NULL || (*tree) == NULL)
        return_val = FALSE;
    else if ((*tree) -> p == NULL)
        return_val = FALSE;
    else if ((*tree) -> p -> left == NULL)
        return_val = FALSE;
    else if ((*tree) != (*tree) -> p -> left)
        return_val = FALSE;
    
    return return_val;
}

int IsRightChild(PrpsTree **tree)
{   int return_val = TRUE;

    if (tree == NULL || (*tree) == NULL)
        return_val = FALSE;
    else if ((*tree) -> p == NULL || (*tree) -> p -> right == NULL)
        return_val = FALSE;
    else if ((*tree) != (*tree) -> p -> right)
        return_val = FALSE;
    
    return return_val;
}

// Next two probably redundant with DetatchChild(args...)
PrpsTree **DetatchParentLeft(PrpsTree **tree)
{   
    /* if node (*tree) is a left child, this function
     * detatches the parent from the left child and returns
     * a ptr_ptr to the parent
     */

    PrpsTree **p = NULL;
    if (IsLeftChild(tree))
    {   
        p = calloc(1, sizeof(PrpsTree *));
        if (p == NULL) 
            MallocErr("DetatchParentLeft");

        (*p) = (*tree) -> p;
        (*tree) -> p -> left = NULL;
        (*tree) -> p = NULL;
    }

    else
        p = NULL;

    return p;
}

PrpsTree **DetatchParentRight(PrpsTree **tree)
{
    /* Same as DetatchParentLeft, but with right child
     */

    PrpsTree **p = NULL;

    if (IsRightChild(tree))
    {   

        p = calloc(1, sizeof(PrpsTree *));
        if (!p) MallocErr("DetatchParentRight");
        (*p) = (*tree) -> p;
        (*tree) -> p -> right = NULL;
        (*tree) -> p = NULL;
    } 

    else 
        p = NULL;

    return p;
}

PrpsTree **DetatchParent(PrpsTree **tree, int *dir)
{
    PrpsTree **p = NULL;

    p = DetatchParentLeft(tree);
    *dir = LEFT;
    if (p == NULL)
    {   *dir = RIGHT;
        p = DetatchParentRight(tree);
    } 

    if (p == NULL) 
        *dir = NONE;

    return p;
}


PrpsTree **DetatchChild(PrpsTree **tree, int dir)
{   /* Detatches and returns a child node from 
     * (*tree). 
     */

    PrpsTree **child = NULL;
    child = calloc(1, sizeof(PrpsTree *));
    if (!child) MallocErr("DetatchChild 1");

    if (dir == LEFT)
    {   if ((*tree) -> left == NULL)
            InconsistencyErr("DetatchChild 2");
        if ((*tree) -> left -> p == NULL)
            InconsistencyErr("DetatchChild 3");
        (*child) = (*tree) -> left;
        (*tree) -> left -> p = NULL;
        (*tree) -> left = NULL;
    }

    else if (dir == RIGHT)
    {   if ((*tree) -> right == NULL)
            InconsistencyErr("DetatchChild 4");
        if ((*tree) -> right -> p == NULL)
            InconsistencyErr("DetatchChild 5");
        (*child) = (*tree) -> right;
        (*tree) -> right = NULL;
        (*child) -> p = NULL;
    } 
    
    else InconsistencyErr("DetatchChild 6");

    return child;
}

void Negate(PrpsTree **node)
{   CheckConsistency(node);
    *((*node) -> neg) += 1;
    *((*node) -> neg) %= 2;
}

PrpsTree **TraverseLeft(PrpsTree **tree)
{
    PrpsTree **return_val = NULL;
    CheckConsistency(tree);
    if ((*tree) -> left != NULL)
        return_val = TraverseLeft(&((*tree) -> left));
    else 
        return_val = tree;
    return return_val; 
}

PrpsTree **CopyNode(PrpsTree **node)
{   PrpsTree **nvw_ptr_ptr = NULL;
    //CheckConsistency(node); 
    char **args = NULL;
    int i, n_args = (**node).n_args;


    nvw_ptr_ptr = calloc(1, sizeof(PrpsTree *));
    if (!nvw_ptr_ptr) MallocErr("CopyNode");
    (*nvw_ptr_ptr) = GenerateEmpty();

    if (OprtrNode(node) == TRUE/**((*node) -> type) == OPRTR*/)
    {   AllocateAsOprtr(nvw_ptr_ptr, *((*node) -> oprtr));
        *((*nvw_ptr_ptr) -> neg) = *((*node) -> neg);
    } 

    else if (PrpsNode(node) == TRUE/**((*node) -> type) == PRPS*/)
    {
        args = CopyArgs(node, &n_args);
        AllocateAsPrps(nvw_ptr_ptr, 
            (*node) -> stmnt -> stc, args, n_args);
        *((*nvw_ptr_ptr) -> neg) = *((*node) -> neg);
    } 

    else InconsistencyErr("CopyNode");

    return nvw_ptr_ptr;
}

PrpsTree **CopySubTree(PrpsTree **tree)
{   
    PrpsTree **return_ptr = NULL;   //
    PrpsTree **tmp_ptr = NULL;      // 

    CheckConsistency(tree);
    return_ptr = CopyNode(tree);

    if ((*tree) -> left != NULL)
    {   tmp_ptr = CopySubTree(&((*tree) -> left));
        (*return_ptr) -> left = *tmp_ptr;
        (*return_ptr) -> left -> p = (*return_ptr);

        free(tmp_ptr); tmp_ptr = NULL;
    } 
    
    if ((*tree) -> right != NULL)
    {   tmp_ptr = CopySubTree(&((*tree) -> right));
        (*return_ptr) -> right = *tmp_ptr;
        (*return_ptr) -> right -> p = (*return_ptr);

        free(tmp_ptr); tmp_ptr = NULL;
    }

    return return_ptr;
}

int GetType(PrpsTree **node)
{
    int return_val;

    if (OprtrNode(node))
        return_val = OPRTR;
    else if (PrpsNode(node))
        return_val = PRPS;
    else InconsistencyErr("GetType 1");

    return return_val;
}


int CheckEqualNodes(PrpsTree **node_a, PrpsTree **node_b)
{   // Seems to work
    int return_val = TRUE, type_a, type_b;
    int n_args_a, n_args_b;
    int i;

    CheckConsistency(node_a);
    CheckConsistency(node_b);

    type_a = GetType(node_a);
    type_b = GetType(node_b);

    if (type_a == type_b)
    {   if (type_a == OPRTR)
        {   if (*((*node_a) -> oprtr) != *((*node_b) -> oprtr))
                return_val = FALSE;
            else if (*((*node_a) -> neg) != *((*node_b) -> neg))
                return_val = FALSE;
        } 

        else if (type_a == PRPS)
        {   n_args_a = (*node_a) -> n_args;
            n_args_b = (*node_b) -> n_args;

            if (*((*node_a) -> neg) != *((*node_b) -> neg))
                return_val = FALSE;
            else if (*((*node_a) -> stmnt -> s) != *((*node_b) -> stmnt -> s))
                return_val = FALSE;
            else if (strcmp((*node_a) -> stmnt -> stc, (*node_b) -> stmnt -> stc) != FALSE)
                return_val = FALSE;
            else if (n_args_a != n_args_b)
                return_val = FALSE;
            else
            {
                for (i = 0; i < n_args_a; i++)
                {
                    if (strcmp((const char *) ((*node_a) -> argmnt)[i], 
                               (const char *) ((*node_b) -> argmnt)[i]) != 0)
                    {
                        return_val = FALSE;
                        break;
                    }

                    else if (*(((*node_a) -> argmnt)[i] -> s) != *(((*node_b) -> argmnt)[i] -> s))
                    {
                        return_val = FALSE;
                        break;
                    }
                }
            }
        }
    } 

    else return_val = FALSE;

    return return_val;
}

int CheckEqualSubTrees(PrpsTree **tree_a, PrpsTree **tree_b)
{   // Seems to work: tested a couple simple trees
    int return_val = 1;
    
    CheckConsistency(tree_a);
    CheckConsistency(tree_b);

    if (!CheckEqualNodes(tree_a, tree_b))
        return_val = 0;
    else
    {   if ((*tree_a) -> left != NULL && (*tree_b) -> left != NULL)
            return_val = CheckEqualSubTrees(&((*tree_a) -> left), &((*tree_b) -> left));
        else if (!((*tree_a) -> left == NULL && (*tree_b) -> left == NULL))
            return_val = 0;

        if (return_val)
        {   if ((*tree_a) -> right != NULL && (*tree_b) -> right != NULL)
                return_val = CheckEqualSubTrees(&((*tree_a) -> right), &((*tree_b) -> right));
            else if (!((*tree_a) -> right == NULL && (*tree_b) -> right == NULL))
                return_val = 0;
        }
    } 
    
    return return_val;
}

void TreeConsistency(PrpsTree **tree)
{
    CheckConsistency(tree);

    if (OprtrNode(tree))
    {
        TreeConsistency(&((*tree) -> left));
        TreeConsistency(&((*tree) -> right));
    }

    else if (PrpsNode(tree))
    {
        if ((*tree) -> left != NULL || (*tree) -> right != NULL)
            InconsistencyErr("TreeConsistency 1");
    }
}


/* L O G I C A L - I D E N T I T Y - O P E R A T I O N S */
void ConvertTAU(PrpsTree **tree)
{
    /* (a = b) = (!a || b) && (a || !b)
     */

    int dir = NONE;

    if (OprtrNodeType(tree, IS, TAU) || OprtrNodeType(tree, NOT, TAU))
    {
        PrpsTree **tree_right = NULL;
        //PrpsTree **tree_b = NULL;
        PrpsTree **p = NULL;
        PrpsTree **node_new = NULL;
        PrpsTree **tree_left = NULL;

        CheckConsistency(tree);
        CheckChildrenOprtr(tree, TAU);

        p = DetatchParent(tree, &dir);

        tree_right = CopySubTree(tree);

        tree_left = calloc(1, sizeof(PrpsTree *));
        if (!tree_left) 
            MallocErr("ConvertTAU");

        (*tree_left) = (*tree);
        (*tree) = GenerateEmpty();
        AllocateAsOprtr(tree, AND);

        *((*tree_left) -> oprtr) = OR;
        *((*tree_right) -> oprtr) = OR;

        Negate(&((*tree_left) -> left));
        Negate(&((*tree_right) -> right));
        SetChild(tree, tree_left, LEFT);
        SetChild(tree, tree_right, RIGHT);
        SetChild(p, tree, dir);

        free(tree_left);    tree_left = NULL;
        free(tree_right);   tree_right = NULL;
    }
}

// WORKS!
void ElliminateInnerOrs(PrpsTree **tree)
{
    /*  (a && b) || (c && d)
     *
     *  = (a || c) && (a || d) && (b || c) && (b || d) 
     *  
     *           O R 
     *
     *  a || (c && d)
     *  = a && (a || d) && (a || c) && (a || d)
     *
     *           O R
     *
     *  (a && b) || c
     *  = (a || c) && (b || c)
     *
     *
     *  'Should be run after negations been pushed downwards'
     */

    int dir = LEFT;
    PrpsTree **new_sub_root = NULL, **tree_p = NULL;
    PrpsTree **left = NULL, **right = NULL,
             **left_a = NULL, **left_b = NULL,
             **right_a = NULL, **right_b = NULL;
    PrpsTree **a_1 = NULL, **a_2 = NULL,
             **b_1 = NULL, **b_2 = NULL,
             **c_1 = NULL, **c_2 = NULL,
             **d_1 = NULL, **d_2 = NULL;

    if (OprtrNodeType(tree, IS, OR))
    {
        CheckConsistency(tree);

        if (OprtrNodeType(&((*tree) -> left), IS, AND) || OprtrNodeType(&((*tree) -> right), IS, AND))
        {
            //printf("full:\t"); tree_print(tree); printf("\n");

            left = DetatchChild(tree, LEFT);
            right = DetatchChild(tree, RIGHT);

            //printf("root:\t"); printf("%d", *((*tree) -> oprtr)); printf("\n");
            //printf("left:\t"); tree_print(left); printf("\n");
            //printf("right:\t"); tree_print(right); printf("\n");

            if (!OprtrNodeType(left, IS, AND))
            {
                /*  a || (c && d)
                 *  = (a || c) && (a || d)
                 */

                if (!OprtrNodeType(right, IS, AND))
                    DeathErr("ElliminateInnerOrs 4");
                //if (!PrpsNode(left))
                //    DeathErr("ElliminateInnerOrs 6");

                //tree_p = calloc(sizeof(PrpsTree *));
                //if (tree_p == NULL)
                //    MallocErr("ElliminateInnerOrs 6");

                a_1 = CopySubTree(left);
                a_2 = left;

                left = NULL;

                left = calloc(1, sizeof(PrpsTree *));
                if (left == NULL)
                    MallocErr("ElliminateInnerOrs 1");

                (*left) = GenerateEmpty();
                AllocateAsOprtr(left, OR);

                c_1 = DetatchChild(right, LEFT);
                d_1 = DetatchChild(right, RIGHT);

                SetLeftChild(left, a_1); // = Oprtr(IS, a_1, IS, c_1, IS, OR);
                SetRightChild(left, c_1);
                SetLeftChild(right, a_2);
                SetRightChild(right, d_1);
                *((*right) -> oprtr) = OR;

                free(a_1); a_1 = NULL;
                free(a_2); a_2 = NULL;
                free(c_1); c_1 = NULL;
                free(d_1); d_1 = NULL;
            }

            else if (!OprtrNodeType(right, IS, AND))
            {
                /*  (a && b) || c
                 *  = (a || c) && (b || c)
                 */

                if (!OprtrNodeType(left, IS, AND))
                    DeathErr("ElliminateInnerOrs 7");
                //if (!PrpsNode(right))
                //    DeathErr("ElliminateInnerOrs 8");

                c_1 = CopySubTree(right);
                c_2 = right;

                right = NULL; 
                right = calloc(1, sizeof(PrpsTree *));
                if (right == NULL)
                    MallocErr("ElliminateInnerOrs 1");

                (*right) = GenerateEmpty();
                AllocateAsOprtr(right, OR);

                a_1 = DetatchChild(left, LEFT);
                b_1 = DetatchChild(left, RIGHT);

                SetLeftChild(left, a_1);
                SetRightChild(left, c_1);
                SetLeftChild(right, b_1);
                SetRightChild(right, c_2);
                *((*left) -> oprtr) = OR;

                free(a_1); a_1 = NULL;
                free(b_1); b_1 = NULL;
                free(c_1); c_1 = NULL;
                free(c_2); c_2 = NULL;
            }

            else
            {
                /*  (a && b) || (c && d)
                 *  = (a || c) && (a || d) && (b || c) && (b || d)
                 */

                a_1 = DetatchChild(left, LEFT);
                b_1 = DetatchChild(left, RIGHT);

                c_1 = DetatchChild(right, LEFT);
                d_1 = DetatchChild(right, RIGHT);

                a_2 = CopySubTree(a_1);
                b_2 = CopySubTree(b_1);
                c_2 = CopySubTree(c_1);
                d_2 = CopySubTree(d_1);

                left_a = Oprtr(IS, a_1, IS, c_1, IS, OR);
                left_b = Oprtr(IS, a_2, IS, d_1, IS, OR);
                right_a = Oprtr(IS, b_1, IS, c_2, IS, OR);
                right_b = Oprtr(IS, b_2, IS, d_2, IS, OR);

                SetLeftChild(left, left_a);
                SetRightChild(left, left_b);
                SetLeftChild(right, right_a);
                SetRightChild(right, right_b);

                *((*left) -> oprtr) = AND;
                *((*right) -> oprtr) = AND;


                free(a_1); a_1 = NULL;
                free(a_2); a_2 = NULL;
                free(b_1); b_1 = NULL;
                free(b_2); b_2 = NULL;
                free(c_1); c_1 = NULL;
                free(c_2); c_2 = NULL;

                free(left_a);   left_a = NULL;
                free(left_b);   left_b = NULL;
                free(right_a);  right_a = NULL;
                free(right_b);  right_b = NULL;
            }

            SetLeftChild(tree, left);
            SetRightChild(tree, right);

            if (left != NULL)
            {
                free(left); left = NULL;
            }

            if (right != NULL)
            {
                free(right); right = NULL;
            }

            *((*tree) -> oprtr) = AND;

            CheckConsistency(tree);
            CheckConsistency(&((*tree) -> left));
            CheckConsistency(&((*tree) -> right));
            //printf("post:\t"); tree_print(tree); printf("\n");
        }
    }
}

void DistributeOrs(PrpsTree **tree, PrpsTree **tree_insert)
{
    
}

int AND_SubTree(PrpsTree **tree)
{
    /* Checks if the subtree is in the form of 
     *      (a1 ^ a2 ^ ... ^ an)
     */

}

void DistributeOrs_Caller(PrpsTree **tree)
{
    /*  (a1 ^ a2 ^ ... ^ an) v x
     *      =
     *  [ (a1 v x) ^ (a2 v x) ^ ... ^ (an v x)]
     *  
     *  
     *      OR
     *  
     *  x v (a1 ^ a2 ^ ... ^ an)
     *      =
     *  [ (a1 v x) ^ (a2 v x) ^ ... ^ (an v x)]
     *
     */

    int dir = LEFT;
    PrpsTree **new_sub_root = NULL, **tree_p = NULL;

    PrpsTree **left = NULL, **right = NULL,
             **left_a = NULL, **left_b = NULL,
             **right_a = NULL, **right_b = NULL;

    PrpsTree **a_1 = NULL, **a_2 = NULL,
             **b_1 = NULL, **b_2 = NULL,
             **c_1 = NULL, **c_2 = NULL,
             **d_1 = NULL, **d_2 = NULL;

    if (OprtrNodeType(tree, IS, OR))
    {
        CheckConsistency(tree);

        if (OprtrNodeType(&((*tree) -> left), IS, OR) || OprtrNodeType(&((*tree) -> right), IS, OR))
        {
            left = DetatchChild(tree, LEFT);
            right = DetatchChild(tree, RIGHT);

            if (LeafNode(left))
            {
                /*  a || (c && d)
                 *  = (a || d) && (a || c)
                 */

                if (!OprtrNodeType(right, IS, AND))
                    DeathErr("ElliminateInnerOrs 4");
                if (!PrpsNode(left))
                    DeathErr("ElliminateInnerOrs 6");

                //tree_p = calloc(sizeof(PrpsTree *));
                //if (tree_p == NULL)
                //    MallocErr("ElliminateInnerOrs 6");

                a_1 = CopySubTree(left);
                a_2 = CopySubTree(a_1);

                DeallocateFromPrps(left);

                (*left) -> oprtr = calloc(1, sizeof(int));

                if ((*left) -> oprtr == NULL)
                    MallocErr("ElliminateInnerOrs 9");

                *((*left) -> oprtr) = OR;
                //AllocateAsOprtr(left, OR);

                c_1 = DetatchChild(right, LEFT);
                d_1 = DetatchChild(right, RIGHT);

                SetLeftChild(left, a_1); // = Oprtr(IS, a_1, IS, c_1, IS, OR);
                SetRightChild(left, c_1);
                SetLeftChild(right, a_2);
                SetRightChild(right, d_1);
                *((*right) -> oprtr) = OR;

                free(a_1); a_1 = NULL;
                free(a_2); a_2 = NULL;
                free(c_1); c_1 = NULL;
                free(d_1); d_1 = NULL;
            }

            else if (LeafNode(right))
            {
                /*  (a && b) || c
                 *  = (a || c) && (b || c)
                 */

                if (!OprtrNodeType(left, IS, AND))
                    DeathErr("ElliminateInnerOrs 7");
                if (!PrpsNode(right))
                    DeathErr("ElliminateInnerOrs 8");

                c_1 = CopySubTree(right);
                c_2 = CopySubTree(c_1);

                DeallocateFromPrps(right);
                AllocateAsOprtr(right, OR);

                a_1 = DetatchChild(left, LEFT);
                b_1 = DetatchChild(left, RIGHT);

                SetLeftChild(left, a_1);
                SetRightChild(left, c_1);
                SetLeftChild(right, b_1);
                SetRightChild(right, c_2);
                *((*left) -> oprtr) = OR;

                free(a_1); a_1 = NULL;
                free(b_1); b_1 = NULL;
                free(c_1); c_1 = NULL;
                free(c_2); c_2 = NULL;
            }

            else
            {
                /*  (a && b) || (c && d)
                 *  = (a || c) && (a || d) && (b || c) && (b || d)
                 */

                a_1 = DetatchChild(left, LEFT);
                b_1 = DetatchChild(left, RIGHT);

                c_1 = DetatchChild(right, LEFT);
                d_1 = DetatchChild(right, RIGHT);

                a_2 = CopySubTree(a_1);
                b_2 = CopySubTree(b_1);
                c_2 = CopySubTree(c_1);
                d_2 = CopySubTree(d_1);

                left_a = Oprtr(IS, a_1, IS, c_1, IS, OR);
                left_b = Oprtr(IS, a_2, IS, d_1, IS, OR);
                right_a = Oprtr(IS, b_1, IS, c_2, IS, OR);
                right_b = Oprtr(IS, b_2, IS, d_2, IS, OR);

                SetLeftChild(left, left_a);
                SetRightChild(left, left_b);
                SetLeftChild(right, right_a);
                SetRightChild(right, right_b);

                *((*left) -> oprtr) = AND;
                *((*right) -> oprtr) = AND;


                free(a_1); a_1 = NULL;
                free(a_2); a_2 = NULL;
                free(b_1); b_1 = NULL;
                free(b_2); b_2 = NULL;
                free(c_1); c_1 = NULL;
                free(c_2); c_2 = NULL;

                free(left_a);   left_a = NULL;
                free(left_b);   left_b = NULL;
                free(right_a);  right_a = NULL;
                free(right_b);  right_b = NULL;
            }


            SetLeftChild(tree, left);
            SetRightChild(tree, right);

            if (left != NULL)
            {
                free(left); left = NULL;
            }

            if (right != NULL)
            {
                free(right); right = NULL;
            }

            *((*tree) -> oprtr) = AND;

            CheckConsistency(tree);
            CheckConsistency(&((*tree) -> left));
            CheckConsistency(&((*tree) -> right));
        }
    }
}


void ConvertIMP(PrpsTree **tree)
{   /* (a -> b ) = (!a || b)
     */

    CheckConsistency(tree);
    //CheckChildrenOprtr(tree, IMP);
    //CheckOprtrNode(tree);
    if (OprtrNodeType(tree, IS, IMP) || OprtrNodeType(tree, NOT, IMP))
    {   //Negate(tree);
        Negate(&((*tree) -> left));
        *((*tree) -> oprtr) = OR;
    }
}

void ConvertNotOr(PrpsTree **tree)
{   /*  !(a || b) = (!a && !b)
     */

    CheckConsistency(tree);
    if (OprtrNodeType(tree, NOT, OR))
    {   Negate(tree);
        Negate(&((*tree) -> left));
        Negate(&((*tree) -> right));
        *((*tree) -> oprtr) = AND;
    } 
}

void ConvertNotAnd(PrpsTree **tree)
{   /*  !(a && b) = (!a || !b)
     */
    
    int test;
    CheckConsistency(tree);
    test = OprtrNodeType(tree, NOT, AND);
    if (test == TRUE)
    {   Negate(tree);
        Negate(&((*tree) -> left));
        Negate(&((*tree) -> right));
        *((*tree) -> oprtr) = OR;
    } 
}

void ConvertIsOR(PrpsTree **tree)
{   /*  (a || b) = !(!a && !b)
     *  (a && b) || (c && d) =
     */

    CheckConsistency(tree);
    if (OprtrNodeType(tree, IS, OR))
    {   *((*tree) -> oprtr) = AND;
        Negate(tree);
        Negate(&((*tree) -> left));
        Negate(&((*tree) -> right));
    } 
}

void MergeOrs(PrpsTree **tree)
{
    /* (a || b) || (c || d)
     * = (a || c) && (a || d) && (b || c) && (b || d)
     */

    PrpsTree **left = NULL, **mid = NULL, **right = NULL;
    PrpsTree **left_a = NULL, **left_b = NULL,
             **right_a = NULL, **right_b = NULL;
    PrpsTree **a_1 = NULL, **a_2 = NULL,
             **b_1 = NULL, **b_2 = NULL,
             **c_1 = NULL, **c_2 = NULL,
             **d_1 = NULL, **d_2 = NULL;

    if (OprtrNodeType(tree, IS, OR))
        if (OprtrNodeType(&((*tree) -> left), IS, OR))
            if (OprtrNodeType(&((*tree) -> right), IS, OR))
            {
                left_a = DetatchChild(tree, LEFT);
                right_a = DetatchChild(tree, RIGHT);

                a_1 = DetatchChild(left_a, LEFT);
                b_1 = DetatchChild(left_a, RIGHT);
                c_1 = DetatchChild(right_a, LEFT);
                d_1 = DetatchChild(right_a, RIGHT);

                a_2 = CopySubTree(a_1); b_2 = CopySubTree(b_1);
                c_2 = CopySubTree(c_1); d_2 = CopySubTree(d_1);

                left_b = CopyNode(left_a); right_b = CopyNode(right_a);
                left = CopyNode(left_a);
                *((*left) -> oprtr) = AND;
                //mid = CopyNode(left);
                right = CopyNode(left);

                SetLeftChild(left_a, a_1); SetRightChild(left_a, c_1);
                SetLeftChild(left_b, a_2); SetRightChild(left_b, d_1);
                SetLeftChild(right_a, b_1); SetRightChild(right_a, c_2);
                SetLeftChild(right_b, b_2); SetRightChild(right_b, d_2);

                SetLeftChild(left, left_a); SetRightChild(left, left_b);
                SetLeftChild(right, right_a); SetRightChild(right, right_b);
                SetLeftChild(tree, left); SetRightChild(tree, right);
                *((*tree) -> oprtr) = AND;

                free(a_1);  a_1 = NULL; free(a_2);  a_2 = NULL;
                free(b_1);  b_1 = NULL; free(b_2);  b_2 = NULL;
                free(c_1);  c_1 = NULL; free(c_2);  c_2 = NULL;
                free(d_1);  d_1 = NULL; free(d_2);  d_2 = NULL;

                free(left_a);   left_a = NULL;
                free(left_b);   left_b = NULL;
                free(right_a);  right_a = NULL;
                free(right_b);  right_b = NULL;
                free(left);     left = NULL;
                free(right);    right = NULL;

                TreeConsistency(tree);
            }
}

void CNF_Step(PrpsTree **tree, void (**pfi)())
{   PrpsTree **tree_left = NULL, **tree_right = NULL;
    PrpsTree **l_a = NULL, **r_a = NULL;

    TreeConsistency(tree);
    if (OprtrNode(tree))
    {
        tree_right = calloc(1, sizeof(PrpsTree *));
        tree_left = calloc(1, sizeof(PrpsTree *));

        if (!tree_left || !tree_right)
            MallocErr("CNF_Step");


        // Operate!
        (**pfi)(tree);

        (*tree_left) = (*tree) -> left;
        (*tree_right) = (*tree) -> right;

        CNF_Step(tree_left, pfi);
        CNF_Step(tree_right, pfi);

        free(tree_right); 
        free(tree_left); 
        tree_left = NULL;
        tree_right = NULL;

        TreeConsistency(tree);
    }
}

void CNF(PrpsTree **tree, int d)
{   // Convert Imp
    // Convert Tau
    // !(a && b) = (!a || !b) = 

    void (*pfi)();
    int i;

    for (i = 0; i < d; i++)
    {
        // (a = b) -> [ (!a && !b) || (a && b) ]
        // (a -> b) && (b -> a) = 
        // = (!a || b) && (!b || a)
        pfi = &ConvertTAU;
        CNF_Step(tree, &pfi);
        //printf("1.%d:\t", i); tree_print(tree); printf("\n");

        // (a -> b ) -> (!a || b)
        pfi = &ConvertIMP;
        CNF_Step(tree, &pfi);
        //printf("2.%d:\t", i); tree_print(tree); printf("\n");

        // !(a || b) -> (!a && !b)
        pfi = &ConvertNotOr;
        CNF_Step(tree, &pfi);
        //printf("3.%d:\t", i); tree_print(tree); printf("\n");

        // !(a && b) = (!a || !b)
        pfi = &ConvertNotAnd;
        CNF_Step(tree, &pfi);
        //printf("4.%d:\t", i); tree_print(tree); printf("\n");


        //  (a && b) || (c && d)
        //  = (a || c) && (a || d) && (b || c) && (b || d) 
        //           O R 
        //  a || (c && d)
        //  = (a || d) && (a || c) && (a || d)
        //      here a could be a subtree also
        //           O R
        //  (a && b) || c
        //  = (a || c) && (b || c)
        //      here c could be a subtree also
        pfi = &ElliminateInnerOrs;
        CNF_Step(tree, &pfi);
        //printf("5.%d:\t", i); tree_print(tree); printf("\n");
    }
}

int ContainsAnd(PrpsTree **tree)
{
    int a, b, return_val = FALSE;

    if (OprtrNodeType(tree, IS, AND) || OprtrNodeType(tree, NOT, AND))
        return_val = TRUE;
    else
    {
        if (OprtrNode(tree))
        {
            a = ContainsAnd(&((*tree) -> left));
            b = ContainsAnd(&((*tree) -> right));

            return_val = a || b;
        }
    }

    return return_val;
}

int ContainsOnlyPositiveORs(PrpsTree **tree)
{
    int return_val = TRUE, a, b;

    if (OprtrNode(tree))
    {
        if (!OprtrNodeType(tree, IS, OR))
            return_val = FALSE;
        else
        {
            a = ContainsOnlyPositiveORs(&((*tree) -> left));
            b = ContainsOnlyPositiveORs(&((*tree) -> right));

            return_val = a && b;
        }
    }

    return return_val;
}

int IsCNF(PrpsTree **tree)
{   CheckConsistency(tree);
    int return_val = TRUE, a, b;

    if (OprtrNodeType(tree, IS, OR))
    {
        //a = ContainsAnd(&((*tree) -> left));
        //b = ContainsAnd(&((*tree) -> right));
        a = ContainsOnlyPositiveORs(&((*tree) -> left));
        b = ContainsOnlyPositiveORs(&((*tree) -> right));

        return_val = a && b;
    }

    else if (OprtrNodeType(tree, IS, AND))
    {
        a = IsCNF(&((*tree) -> left));
        b = IsCNF(&((*tree) -> right));

        return_val = a && b;
    }

    else if (OprtrNode(tree))
        return_val = FALSE;

    return return_val;
}
