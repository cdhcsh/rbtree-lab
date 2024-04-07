#include "rbtree.h"

#include <stdlib.h>

static void _delete_node(node_t *n, node_t *nil);

static node_t *_insert_node(node_t *n, const key_t key, node_t *nil);

static int _black_height(node_t *t);

rbtree *new_rbtree(void) {
    rbtree *p = (rbtree *) calloc(1, sizeof(rbtree));
    p->nil = (node_t *) calloc(1, sizeof(node_t));
    p->nil->color = RBTREE_BLACK;
    p->root = p->nil;
    return p;
}

void delete_rbtree(rbtree *t) {
    // TODO: reclaim the tree nodes's memory
    _delete_node(t->root, t->nil);
    free(t->nil);
    free(t);
}

static void _delete_node(node_t *n, node_t *nil) {
    if (n == nil || n == NULL) {
        return;
    }
    if (n->left == nil && n->right == nil) {
        free(n);
        return;
    }
    if (n->left != nil) {
        _delete_node(n->left, nil);
    }
    if (n->right != nil) {
        _delete_node(n->right, nil);
    }
}

node_t *rbtree_insert(rbtree *t, const key_t key) {
    // TODO: implement insert
    if (t->root == t->nil) {
        return t->root = _insert_node(t->root, key, t->nil);
    } else
        return _insert_node(t->root, key, t->nil
        );
}

node_t *_insert_node(node_t *t, const key_t key, node_t *nil) {
    if (t == NULL) {
        return NULL;
    }
    if (t == nil) {
        node_t *n = (node_t *) calloc(1, sizeof(node_t));
        n->left = nil;
        n->right = nil;
        n->parent = nil;
        n->key = key;
        return n;
    }
    node_t **next;
    if (t->key > key) {
        next = &(t->left);
    } else {
        next = &(t->right);
    }
    node_t *tmp = _insert_node(*next, key, nil);
    tmp->parent = t;
    *next = tmp;
    return t;
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
    // TODO: implement find
    return t->root;
}

node_t *rbtree_min(const rbtree *t) {
    // TODO: implement find
    return t->root;
}

node_t *rbtree_max(const rbtree *t) {
    // TODO: implement find
    return t->root;
}

int rbtree_erase(rbtree *t, node_t *p) {
    // TODO: implement erase
    return 0;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
    // TODO: implement to_array
    return 0;
}
//static int _black_height(node_t *t){
//}
