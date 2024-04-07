#include "rbtree.h"

#include <stdlib.h>

static void _delete_node(node_t *n, node_t *nil);

static node_t *_insert_node(node_t *n, const key_t key, node_t *nil);

static int _black_height(node_t *t);

static void _left_rotate(rbtree *t, node_t *n);

static void _right_rotate(rbtree *t, node_t *n);

static void _fix_balance(rbtree *t, node_t *n);

rbtree *new_rbtree(void) {
    rbtree *p = (rbtree *) calloc(1, sizeof(rbtree));
    p->nil = (node_t *) calloc(1, sizeof(node_t));
    p->nil->color = RBTREE_BLACK;
    p->root = p->nil;
    return p;
}


void delete_rbtree(rbtree *t) {
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
    node_t *p = t->nil;
    node_t *c = t->root;
    while (c != t->nil) {
        p = c;
        if (c->key <= key) {
            c = c->right;
        } else {
            c = p->left;
        }
    }
    node_t *n = (node_t *) calloc(1, sizeof(node_t));
    n->parent = p;
    n->key = key;
    n->left = t->nil;
    n->right = t->nil;
    n->color = RBTREE_RED;
    if (p == t->nil) {
        t->root = n;
    } else if (p->key <= key) {
        p->right = n;
    } else {
        p->left = n;
    }
    _fix_balance(t, n);
    return n;
}


node_t *rbtree_find(const rbtree *t, const key_t key) {
    return t->root;
}

node_t *rbtree_min(const rbtree *t) {
    return t->root;
}

node_t *rbtree_max(const rbtree *t) {
    return t->root;
}

int rbtree_erase(rbtree *t, node_t *p) {
    return 0;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
    return 0;
}

static int _black_height(node_t *t) {
    return 1;
}

static void _left_rotate(rbtree *t, node_t *n) {
    node_t *m = n->right;
    n->right = m->left; //m의 오른쪽 서브트리를 n의 왼쪽 서브트리로 연결
    if (m->left != t->nil) {
        m->left->parent = n;
    }

    m->parent = n->parent;
    if (n->parent == t->nil) {
        t->root = m;
    } else if (n == n->parent->left) {
        n->parent->left = m;
    } else {
        n->parent->right = m;
    }
    m->left = n; // 멋있다 김남훈
    n->parent = m;
}

static void _right_rotate(rbtree *t, node_t *n) {
    node_t *m = n->left;
    n->left = m->right; //m의 오른쪽 서브트리를 n의 왼쪽 서브트리로 연결
    if (m->right != t->nil) {
        m->right->parent = n;
    }

    m->parent = n->parent;
    if (n->parent == t->nil) {
        t->root = m;
    } else if (n == n->parent->left) {
        n->parent->left = m;
    } else {
        n->parent->right = m;
    }
    m->right = n;
    n->parent = m;
}

static void _fix_balance(rbtree *t, node_t *n) {
    node_t *m;
    while (n->parent->color == RBTREE_RED) {
        if (n->parent == n->parent->parent->left) {
            m = n->parent->parent->right; // m -> 삼촌
            if (m->color == RBTREE_RED) {//case 1
                n->parent->color = RBTREE_BLACK;
                m->color = RBTREE_BLACK;
                m->parent->color = RBTREE_BLACK;
                n = n->parent->parent;
            } else {
                if (n == n->parent->right) { // case 2
                    n = n->parent;
                    _left_rotate(t, n);
                }
                n->parent->color = RBTREE_BLACK;
                n->parent->parent->color = RBTREE_RED;
                _right_rotate(t, n->parent->parent);
            }
        } else {
            m = n->parent->parent->left; // m -> 삼촌
            if (m->color == RBTREE_RED) {//case 1
                n->parent->color = RBTREE_BLACK;
                m->color = RBTREE_BLACK;
                m->parent->color = RBTREE_BLACK;
                n = n->parent->parent;
            } else {
                if (n == n->parent->left) { // case 2
                    n = n->parent;
                    _right_rotate(t, n);
                }
                n->parent->color = RBTREE_BLACK;
                n->parent->parent->color = RBTREE_RED;
                _left_rotate(t, n->parent->parent);
            }
        }
        t->root->color = RBTREE_BLACK;
    }
}