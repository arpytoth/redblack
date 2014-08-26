#include <stdlib.h>
#include "redblack.h"

typedef enum NodeColor
{
    ncRED, ncBLACK
} NodeColor;

typedef struct RedBlackNode
{
    struct RedBlackNode *parent;
    struct RedBlackNode *left;
    struct RedBlackNode *right;
    int value;
} RedBlackNode;

typedef struct RedBlackTree
{
    struct RedBlackNode *root;
} RedBlackTree;


RedBlackNode *redblack_new_node()
{
    RedBlackNode *node;
    node = (RedBlackNode *)malloc(sizeof(RedBlackNode));
    node->parent = NULL;
    node->left = NULL;
    node->right = NULL;
    node->value = 0;
}

RedBlackNode *redblack_search_parent(RedBlackNode *root, RedBlackNode *node)
{
    if (root->elem < node->elem)
    {
        if (root
    }
}

void redblack_insert_node(RedBlackTree *tree, RedBlackNode *node)
{
    if (tree->root == NULL)
    {
        tree->root = node;
    }
    else
    {
    }
}
