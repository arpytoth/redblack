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
 * Case 2. 
 */

void redblack_attach_node(RedBlackNode *parent, RedBlackNode *node, int left)
{
    if (left == 1)
        parent->left = node;
    else
        parent->right = node;

    node->parent = parent;
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
    redblack_insert(tree, -1);
    redblack_insert(tree, 3);
    redblack_insert(tree, 12);
    redblack_insert(tree, 9);
    redblack_insert(tree, 21);
    redblack_insert(tree, 19);
    redblack_insert(tree, 25);
    
    redblack_delete(tree, 12);
    node = redblack_search(tree, 12);
    if (node != NULL)
        printf("Found\n");
    else
        printf("Not found\n");
    
    
}
