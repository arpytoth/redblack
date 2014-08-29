#include <stdio.h>
#include <stdlib.h>

/* 
 * THEORY
 * -------
 *
 * A red-black tree is a binary tree in which:
 * -> each node has a color (red or black) associated with it
 * -> the following 3 properties hold:
 *    1. The root of the tree is black
 *    2. The children of a red node are black
 *    3. For each node with at least one null child, the number of black nodes
 *       on the path from the root to the null child is the same.
 *
 */

typedef enum NodeColor
{
    ncRED, ncBLACK
} NodeColor;

typedef struct RedBlackNode
{
    struct RedBlackNode *parent;
    struct RedBlackNode *left;
    struct RedBlackNode *right;
    NodeColor color;
    int key;
} RedBlackNode;

typedef struct RedBlackTree
{
    struct RedBlackNode *root;
    struct RedBlackNode *nil;
} RedBlackTree;


RedBlackNode *redblack_new_node()
{
    RedBlackNode *node;
    node = (RedBlackNode *)malloc(sizeof(RedBlackNode));
    node->parent = NULL;
    node->left = NULL;
    node->right = NULL;
    node->key = 0;
}

RedBlackTree *redblack_new()
{
    RedBlackTree *tree;

    tree = (RedBlackTree*)malloc(sizeof(RedBlackTree));
    tree->root = NULL;
    return tree;
}

////////////////////////////////////////////////////////////////////////////////
//                             UTILS                                          //
////////////////////////////////////////////////////////////////////////////////
int pow2(int pow)
{
    int result = 1;
    int i;
    
    for (i = 1; i <= pow; i++)
        result *= 2;
    
    return result;
}

int redblack_depth(RedBlackNode *node)
{
    if (node == NULL)
    {
        return 0;
    }
    else
    {
        int depth = 1;
        int left = 0;
        int right = 0;
        
        if (node->left != NULL)
            left = redblack_depth(node->left);

        if (node->right != NULL)
            right = redblack_depth(node->right);
       
       if (left > right)
            return depth + left;
        else 
            return depth + right;
    }   
}

////////////////////////////////////////////////////////////////////////////////
//                             PRINT                                          //
////////////////////////////////////////////////////////////////////////////////

void print_node(RedBlackNode *node)
{
    int size = 2;
    int i;

    if (node == NULL)
    {
        printf(" ");
        for (i = 0; i < size; i++)
            printf("x");
    }
    else
    {
        char buffer[10];
        int count = sprintf(buffer, "%d", node->key);
        printf(" ");
        for (i = 0; i < size-count; i++)
            printf(" ");
        printf("%s", buffer);
    }
}

void redblack_print(RedBlackTree *tree)
{
    RedBlackNode **queue;
    int queue_pos = 0;
    int depth = redblack_depth(tree->root);
    int queue_size = depth * pow2(depth - 1);
    int count = 0;
    int level = 0;
    
    queue = (RedBlackNode**)malloc(sizeof(RedBlackNode*) * queue_size);

    queue[queue_pos++] = tree->root;

    while (queue_pos != 0 && level < depth)
    {
        if (count == 0)
        {
            int k;
            int width =  (pow2(depth - level) / 2  - 1) ;

            for (k = 0; k < width; k++)
                printf("   ");  
        }
        else
        {
            int k;
            int width =  (pow2(depth + 1 - level) / 2  - 1) ;

            for (k = 0; k < width; k++)
                printf("   ");  

        }


        RedBlackNode *node = queue[0];
        int i;
        for (i = 1; i < queue_pos; i++)
            queue[i-1] = queue[i];
        queue_pos--;

        print_node(node);
        
        if (node != NULL)
        {
            queue[queue_pos++] = node->left;
            queue[queue_pos++] = node->right;
        }
        else
        {
            queue[queue_pos++] = NULL;
            queue[queue_pos++] = NULL;

        }
        count++;
        if (count == pow2(level))
        {
            level++;
            count = 0;
            printf("\n");
        }
    }
    printf("\n");
    free(queue);
}

////////////////////////////////////////////////////////////////////////////////
// ROTATIONS
////////////////////////////////////////////////////////////////////////////////

void right_rotate(RedBlackNode **node)
{
    /*
     *         05                 02
     *     02     06    =>   01        05
     *   01  03                     03    06
     *
     *   In this case we right rotate by node 05.
     */

    RedBlackNode *parent = (*node)->parent;
    RedBlackNode *left = (*node)->left;
    RedBlackNode *right = NULL;
    
    if (left != NULL)
        right = left->right;
 
    if (parent != NULL)
    {
        if (parent->left == *node)
            parent->left = left;
        else
            parent->right = left;
    }
    if (left != NULL)
        left->parent = parent;    
    
    if (left != NULL)
        left->right = (*node);
    (*node)->parent = left;

    (*node)->left = right;
    if (right != NULL)
        right->parent = (*node);

    (*node) = left;     
}


void left_rotate(RedBlackNode **node)
{ 
    /*
     *         02                         05
     *   01          05       =>      02     06
     *           03      06        01   03   
     */
    RedBlackNode *parent = (*node)->parent;
    RedBlackNode *right = (*node)->right;
    RedBlackNode *left = right->left;
 
    if (parent != NULL)
    {
        if (parent->left == (*node))
            parent->left = right;
        else
            parent->right = right;
    }
    if (right != NULL)
    {
        right->parent = parent;    
        right->left = *node;
        (*node)->parent = right;
    }

    (*node)->right = left;
    if (left != NULL)
        left->parent = (*node);

    (*node) = right;
}


////////////////////////////////////////////////////////////////////////////////
// INSERT
////////////////////////////////////////////////////////////////////////////////

/*
 * 1. use the Binary Search Tree insert algorithm to add the new node
 * 2. color the node to red
 * 3. restore red-black tree properties if necessary.
 *
 * For step 3 for inserting into a non-empty tree, what we need to do depends
 * on the color of the parent:
 *
 * Case 1. The parent is Black, there's nothing more to do
 * Case 2. The parent is Red, so the parent now has a Red child, to ok, we need
 *         to fix this.
 *  Case 2a. P's simbling G is black or null.
 */

void redblack_attach_node(RedBlackNode *parent, RedBlackNode *node, int left)
{
    if (left == 1)
        parent->left = node;
    else
        parent->right = node;

    node->parent = parent;
}

RedBlackNode *grandparent(RedBlackNode *node)
{
    if (node != NULL && node->parent != NULL)
        return node->parent->parent;
    else
        return NULL;
}

RedBlackNode *uncle(RedBlackNode *node)
{
    RedBlackNode *g = grandparent(node);
    if (g == NULL)
        return NULL;
    if (node->parent == g->left)
        return g->right;
    else
        return g->left;
}

void insert_case1(RedBlackNode *n);

void insert_case4(RedBlackNode *n)
{
    RedBlackNode *g;

    g = grandparent(n);
    if (n == n->parent->right && n->parent == g->left)
    {
        //rotate_left(n->parent);
    }
}

void insert_case3(RedBlackNode *n)
{
    RedBlackNode *u;
    RedBlackNode *g;

    u = uncle(n);
    if (u != NULL && u->color == ncRED)
    {
        n->parent->color = ncBLACK;
        u->color = ncBLACK;
        g = grandparent(n);
        g->color = ncRED;
        insert_case1(g);
    }
    else
    {
        insert_case4(g);
    }
}

void insert_case2(RedBlackNode *n)
{
    if (n->parent->color == ncBLACK)
        return;
    else
        insert_case2(n);
}

void insert_case1(RedBlackNode *n)
{
    if (n->parent == NULL)
        n->color = ncBLACK;
    else
        insert_case2(n);
}

void redblack_fix_insert5(RedBlackNode *node)
{

}

void redblack_insert_fix(RedBlackNode *parent)
{
       
   }

void redblack_insert_node(RedBlackNode *parent, RedBlackNode *node)
{

    if (node->key < parent->key)
    {
        if (parent->left == NULL)
            redblack_attach_node(parent, node, 1);
        else
            redblack_insert_node(parent->left, node);
    }
    else if (!(parent->key == node->key))
    {
        if (parent->right == NULL)
            redblack_attach_node(parent, node, 0);
        else
            redblack_insert_node(parent->right, node);
    }
}

void redblack_insert(RedBlackTree *tree, int key)
{
    RedBlackNode *node;

    node = redblack_new_node();
    node->key = key;
    if (tree->root == NULL)
    {
        // Case1. The current node is at the root of the tree. In this case we
        //        repaint the node to black. 
        tree->root = node;
        node->color = ncRED;
    }
    else
    {
        redblack_insert_node(tree->root, node);
    }
}

////////////////////////////////////////////////////////////////////////////////
// SEARCH
////////////////////////////////////////////////////////////////////////////////

RedBlackNode *redblack_search_node(RedBlackNode *parent, int key)
{
    if (parent == NULL)
        return NULL;
    else if (key < parent->key)
        return redblack_search_node(parent->left, key);
    else if (parent->key == key)
        return parent;
    else
        return redblack_search_node(parent->right, key);
}

RedBlackNode *redblack_search(RedBlackTree *tree, int key)
{
    return redblack_search_node(tree->root, key);
}

////////////////////////////////////////////////////////////////////////////////
///                                DELETE                                     //
////////////////////////////////////////////////////////////////////////////////

RedBlackNode *redblack_find_min(RedBlackNode *root)
{
    if (root->left == NULL)
        return root;
    else
        return redblack_find_min(root->left);
}

void redblack_delete_node(RedBlackNode *node)
{
    // We have 3 cases to consider:

    if (node->left == NULL && node->right == NULL)
    {
        // 1. Node to be removed has no children, simply remove the node.
        RedBlackNode *parent = node->parent;
        if (parent->left == node)
            parent->left = NULL;
        else
            parent->right = NULL;
        free(node);
    }
    else if (node->left == NULL || node->right == NULL)
    {
        // 2. Node has one child, link the child to the parent of the node
        // to be deleted, and free the node.
        RedBlackNode *parent = node->parent;
        RedBlackNode *child = node->left;
        
        if (child == NULL)
            child = node->right;

        if (parent->left == node)
            parent->left = child;
        else
            parent->right = child;

        free(node);
    }
    else
    {
        // 3. Node to be removed has two children. Find the minimum element
        // in the right subtree of the node to be removed, replace the node
        // value with the minimum value, and remove the minimum value, that is
        // a leaf, so we do this recursively.
       
        RedBlackNode *min = redblack_find_min(node->right);
        printf("Right is: %d\n", node->right->key);
        printf("Minimum is: %d\n", min->key);
        node->key = min->key;
        redblack_delete_node(min);
    }
}

void redblack_delete(RedBlackTree *tree, int key)
{
    RedBlackNode *node_to_del;

    node_to_del = redblack_search(tree, key);
    if (node_to_del != NULL)
    {
        if (node_to_del->parent == NULL)
        {
            free(node_to_del);
            tree->root = NULL;
        }
        else
        {
            redblack_delete_node(node_to_del);
        }
    }
}



////////////////////////////////////////////////////////////////////////////////
// TEST
////////////////////////////////////////////////////////////////////////////////

int main()
{
    RedBlackTree *tree = redblack_new();
    RedBlackNode *node = NULL;
    int d;

    /*
     *           5
     *
     *       2        12
     *    
     *  -4      3    9     21
     *
     *                  19    25
     */
    redblack_insert(tree, 5);
    redblack_insert(tree, 2);
    redblack_insert(tree, 6);
    redblack_insert(tree, 1);
    redblack_insert(tree, 3);
    redblack_print(tree); 
    left_rotate(&tree->root);
    redblack_print(tree);
}
