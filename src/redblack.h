#ifndef _REDBLACK_H_
#define _REDBLACK_H_

////////////////////////////////////////////////////////////////////////////////
//                           GENERIC RED-BLACK TREE                           //
////////////////////////////////////////////////////////////////////////////////


typedef struct RedBlackTree
{
    struct RedBlackNode *root;
    int (*compare)(void *, void *);
    void (*free)(void *);
    int (*print)(char *, void *);
} RedBlackTree;


RedBlackTree *rb_new(int (*compare)(void *, void *), void (*free)(void *),
    int (*print)(char *, void *));
void rb_insert(RedBlackTree *tree, void *key, void *value);
void *rb_search(RedBlackTree *tree, void *key);
void *rb_delete(RedBlackTree *tree, void *key);


////////////////////////////////////////////////////////////////////////////////
//                            INT RED-BLACK TREE                              //
////////////////////////////////////////////////////////////////////////////////               

RedBlackTree *irb_new();
void irb_insert(RedBlackTree *tree, int key, void *value);
void *irb_search(RedBlackTree *tree, int key);
void *irb_delete(RedBlackTree *tree, int key);

////////////////////////////////////////////////////////////////////////////////
//                          STRING KEY RED-BLACK TREE                         //
////////////////////////////////////////////////////////////////////////////////

RedBlackTree *srb_new();
void  srb_insert(RedBlackTree *tree, const char *key, void *value);
void *srb_search(RedBlackTree *tree, char *key);
void *srb_delete(RedBlackTree *tree, char *key);

#endif // _REDBLACK_H_

