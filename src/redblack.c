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

void redblack_insert(RedBlackNode *root, int elem)
{
    
}
