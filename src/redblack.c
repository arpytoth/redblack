#include <stdio.h>
#include <stdlib.h>

/* 
 * THEORY
 * -------
 *
 * A red-black tree is a binary tree where each node has a color and the
 * following attributes are true:
 *
 * 1) The root is black.
 * 2) All leaves are black.
 * 3) Both children of each red node are black
 * 4) The path from each leaf up to the root contain the same number of black
 *    nodes.
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
    int value;
} RedBlackNode;

typedef struct RedBlackTree
{
    struct RedBlackNode *root;
    struct RedBlackNode *nil;
} RedBlackTree;


////////////////////////////////////////////////////////////////////////////////
//                       CONSTRUCTORS AND DESTRUCTORS                         //
////////////////////////////////////////////////////////////////////////////////

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
    tree->nil = redblack_new_node();
    tree->nil->color = ncBLACK;
    tree->nil->left = tree->nil;
    tree->nil->right = tree->nil;
    tree->nil->parent = tree->nil;
    return tree;
}

////////////////////////////////////////////////////////////////////////////////
//                                   UTILS                                    //
////////////////////////////////////////////////////////////////////////////////
RedBlackNode *successor(RedBlackNode *n)
{
    if (n == NULL)
    {
        return NULL;
    }
    else if (n->right != NULL)
    {
        RedBlackNode *s = n->right;
        while (s->left != NULL)
            s = s->left;
        return s;
    }
    else
    {
        RedBlackNode *s = n->parent;
        RedBlackNode *ch = n;
        while (s != NULL && ch == s->right)
        {
            ch = s;
            s = s->parent;
        }
        return s;
    }
}



void set_color(RedBlackNode *p, NodeColor color)
{
    if (p != NULL)
        p->color = color;
}

NodeColor color_of(RedBlackNode *p)
{
    if (p == NULL)
        return ncBLACK;
    else
        return p->color;
}

RedBlackNode *parent_of(RedBlackNode *p)
{
    return (p == NULL) ? NULL : p->parent;
}

RedBlackNode *left_of(RedBlackNode *p)
{
    return (p == NULL) ? NULL : p->left;
}

RedBlackNode *right_of(RedBlackNode *p)
{
    return (p == NULL) ? NULL : p->right;
}

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
        char color;
        int count;
        
        color = node->color == ncRED?'R':'B';
        count = sprintf(buffer, "%d%c", node->key, color);
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
//                                 ROTATIONS                                  //
////////////////////////////////////////////////////////////////////////////////

void right_rotate(RedBlackTree *tree, RedBlackNode *p)
{
    if (p != NULL)
    {
        RedBlackNode *l = p->left;
        p->left= l->right;
        if (l->right != NULL)
            l->right->parent = p;
        l->parent = p->parent;
        if (p->parent == NULL)
            tree->root = l;
        else if (p->parent->right == p)
            p->parent->right = l;
        else
            p->parent->left = l;

        l->right = p;
        p->parent = l;
    }
}


void left_rotate(RedBlackTree *tree, RedBlackNode *p)
{
    if (p != NULL)
    {
        RedBlackNode *r = p->right;
        p->right = r->left;
        if (r->left != NULL)
            r->left->parent = p;
        r->parent = p->parent;
        if (p->parent == NULL)
            tree->root = r;
        else if (p->parent->left == p)
            p->parent->left = r;
        else
            p->parent->right = r;

        r->left = p;
        p->parent = r;
    }
}


////////////////////////////////////////////////////////////////////////////////
//                                   INSERT                                   //
////////////////////////////////////////////////////////////////////////////////


void fix_after_insert(RedBlackTree *tree, RedBlackNode *x)
{
    /*
     * Algorithm from CLR.
     */

    x->color = ncRED;
    while (x != NULL && x != tree->root && x->parent->color == ncRED)
    {
        if (parent_of(x) == left_of(parent_of(parent_of(x))))
        {
            RedBlackNode *y = right_of(parent_of(parent_of(x)));
            if (color_of(y) == ncRED) 
            {
                set_color(parent_of(x), ncBLACK);
                set_color(y, ncBLACK);
                set_color(parent_of(parent_of(x)), ncRED);
                x = parent_of(parent_of(x));
            }
            else
            {
                if (x == right_of(parent_of(x)))
                {
                    x = parent_of(x);
                    left_rotate(tree, x);
                }
                set_color(parent_of(x), ncBLACK);
                set_color(parent_of(parent_of(x)), ncRED);
                right_rotate(tree, parent_of(parent_of(x)));
            }
        }
        else
        {
            RedBlackNode *y = left_of(parent_of(parent_of(x)));
            if (color_of(y) == ncRED) 
            {
                set_color(parent_of(x), ncBLACK);
                set_color(y, ncBLACK);
                set_color(parent_of(parent_of(x)), ncRED);
                x = parent_of(parent_of(x));
            }
            else
            {
                if (x == left_of(parent_of(x)))
                {
                    x = parent_of(x);
                    right_rotate(tree, x);
                }
                set_color(parent_of(x), ncBLACK);
                set_color(parent_of(parent_of(x)), ncRED);
                left_rotate(tree, parent_of(parent_of(x)));
            }
        }
    }   
    tree->root->color = ncBLACK;
}

void redblack_insert(RedBlackTree *tree, int key)
{
    RedBlackNode *t = tree->root;
   
    if (tree->root == NULL)
    { 
        RedBlackNode *node;
        node = redblack_new_node();
        node->key = key;

        tree->root = node;
        node->color = ncBLACK;
    }
    else
    {
        int cmp;
        RedBlackNode *parent;
        RedBlackNode *e;

        do
        {
            parent = t;

            if (key < parent->key)
                t = t->left;
            else if (key > parent->key)
                t = t->right;
            else
                return;
        } while (t != NULL);
        
        e = redblack_new_node();
        e->key = key;
        if (key < parent->key)
            parent->left = e;
        else
            parent->right = e;
        e->parent = parent;
        fix_after_insert(tree, e);
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

void fix_after_delete(RedBlackTree *tree, RedBlackNode *x)
{   
    /*
     * Algorithm found in CLR.
     */

    while (x != tree->root && x->color == ncBLACK)
    {
        if (x == left_of(parent_of(x)))
        {
            RedBlackNode *s = right_of(parent_of(x));
            if (color_of(s) == ncRED)
            {
                set_color(s, ncBLACK);
                set_color(parent_of(x), ncRED);
                left_rotate(tree, parent_of(x));
                s = right_of(parent_of(x));
            }
            
            if (color_of(left_of(s)) == ncBLACK &&
                color_of(right_of(s)) == ncBLACK) 
            {
                set_color(s, ncRED);
                x = parent_of(x);
            }
            else
            {
                if (color_of(right_of(s)) == ncBLACK)
                {
                    set_color(left_of(s), ncBLACK);
                    set_color(s, ncRED);
                    right_rotate(tree, s);
                    s = right_of(parent_of(x));
                }

                set_color(s, color_of(parent_of(x)));
                set_color(parent_of(x), ncBLACK);
                set_color(right_of(s), ncBLACK);
                left_rotate(tree, parent_of(x));
                x = tree->root;
            }

        }
        else // symetric
        {
            RedBlackNode *sib = left_of(parent_of(x));
            if (color_of(sib) == ncRED)
            {
                set_color(sib, ncBLACK);
                set_color(parent_of(x), ncRED);
                right_rotate(tree, parent_of(x));
                sib = left_of(parent_of(x));
            }

            if (color_of(right_of(sib)) == ncBLACK &&
                color_of(left_of(sib)) == ncBLACK)
            {
                set_color(sib, ncRED);
                x = parent_of(x);
            }
            else
            {
                if (color_of(left_of(sib)) == ncBLACK) 
                {
                    set_color(right_of(sib), ncBLACK);
                    set_color(sib, ncRED);
                    left_rotate(tree, sib);
                    sib = left_of(parent_of(x));
                }

                set_color(sib, color_of(parent_of(x)));
                set_color(parent_of(x), ncBLACK);
                set_color(left_of(sib), ncBLACK);
                right_rotate(tree, parent_of(x));
                x = tree->root;
            }
        }
    }

    set_color(x, ncBLACK);
}

void redblack_delete_node(RedBlackTree *tree, RedBlackNode *p)
{
    RedBlackNode *r;

    printf("delete p: %d\n", p->key);

    // Case1. The node to delete has exactly two children. In this case we do
    //        a simple relabeling with the successor and delete de successor 
    //        that will have at most one child.
    if (p->left != NULL && p->right != NULL)
    {
        RedBlackNode *s = successor(p);
        p->key = s->key;
        // TODO exchange values;

        printf("successor: %d\n", s->key);
        p = s;
    }

    // Start fixup at replacement node, if it exists.
    r = (p->left != NULL ? p->left : p->right);
    if (r != NULL)
    {
        // link the replacement to parent
        r->parent = p->parent;
        if (p->parent == NULL)
            tree->root = r;
        else if (p == p->parent->left)
            p->parent->left = r;
        else
            p->parent->right = r;

        // NULL out links so they are OK to use by fix_after_delete.
        p->left = p->right = p->parent = NULL;
        
        if (p->color == ncBLACK)
            fix_after_delete(tree, r);
        free(p);
    }
    else if (p->parent == NULL)
    {
        // return if we are the only node.
        tree->root = NULL;
    }
    else
    {
        // No children? Use self as phantom replacement and unlink.
        if (p->color == ncBLACK)
            fix_after_delete(tree, p);

        if (p->parent != NULL)
        {
            if (p == p->parent->left)
                p->parent->left = NULL;
            else
                p->parent->right = NULL;
            p->parent = NULL;
        }
        free(p);
    }
}

void redblack_delete(RedBlackTree *tree, int key)
{
    RedBlackNode *node_to_del;

    node_to_del = redblack_search(tree, key);
    if (node_to_del != NULL)
    {
        redblack_delete_node(tree, node_to_del);
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
    redblack_insert(tree, 4);
    redblack_insert(tree, 6);
    redblack_insert(tree, 3);
    redblack_insert(tree, 2);
    redblack_insert(tree, 1);
    redblack_delete(tree, 6);
    redblack_print(tree); 
}
