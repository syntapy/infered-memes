prps.h:

//void DistributeOr(PrpsTree **tree)
//{   /*  !(a || (b && c))
//     *  = !a && !(b && c)
//     *  = !a && (!b || !c)
//     */
//
//    CheckConsistency(tree);
//    if (OprtrNodeType(tree, NOT, AND))
//    {   
//
//    }
//
//}
//
//void DistributeAnd(PrpsTree **tree)
//{   /*  !(a && (b || c)) 
//     *  = !a || !(b || c)
//     *  = !a || (!b && !c)
//     *  = 
//     *  = !((a || b) && (a || c))
//     *  = !(a || b) || !(a || c)
//     *  = (!a && !b) || (!a && !c)
//     */
//
//    CheckConsistency(tree);
//    if (OprtrNodeType(tree, NOT, OR))
//    {   if (OprtrNodeType(&((*tree) -> right), IS, AND))
//        {   
//
//        } else if (OprtrNodeType(&((*tree) -> left), IS, AND))
//        {
//
//        }
//    }
//}


//int GetMaxDepth(PrpsTree **tree)
//{   unsigned int depth = 1, depth_left = 0, depth_right = 0;
//    CheckConsistency(tree);
//
//    if (OprtrNode(tree))
//    {   depth_left = GetMaxDepth(&((*tree) -> left));
//        depth_right = GetMaxDepth(&((*tree) -> right));
//        if (depth_left > depth_right)
//            depth += depth_left;
//        else depth += depth_right;
//    }
//    return depth;
//}
//
//void PrintOprtr(int oprtr)
//{   switch (oprtr)
//    {   case AND:
//            printf(" AND ");
//        case OR:
//            printf(" OR" );
//        case IMP:
//            printf(" IMP" );
//        case TAU:
//            printf(" TAU ");
//    }
//}
//
//void PrintTree(PrpsTree **tree, unsigned int tree_depth, int newline)
//{   //unsigned int tree_depth = GetDepth(tree);
//    unsigned int i;
//    PrpsTree **node = NULL;
//    CheckConsistency(tree);
//
//    node = malloc(sizeof(PrpsTree *));
//    if (!node) MallocErr("PrintTree");
//    (*node) = GenerateEmpty();
//
//    if (tree_depth != 0)
//    {   fprintf(stdout, "    ");
//        for (i = 0; i < tree_depth; i++)
//        {   fprintf(stdout, "   ");
//        }
//        if (OprtrNode(tree))
//        {   if (*((*tree) -> neg) == NOT)
//                printf("\b\b\bNOT ");
//            PrintOprtr(*((*tree) -> oprtr));
//        } 
//        else if (PrpsNode(tree))
//            printf("%s", (*tree) -> stmnt -> stc);
//
//        if (newline == YES)
//            printf("\n");
//
//        PrintTree(&((*tree) -> left), (unsigned int) (tree_depth - 1), NO);
//        if (newline == YES)
//            PrintTree(&((*tree) -> right), (unsigned int) (tree_depth - 1), YES);
//        else PrintTree(&((*tree) -> right), (unsigned int) (tree_depth - 1), NO);
//    }
//}
