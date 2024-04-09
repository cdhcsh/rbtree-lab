#include "rbtree.h"

#include <stdlib.h>
#include <stdio.h>

static void _delete_node(node_t *n, node_t *nil);

static node_t *_min(const node_t *n, const node_t *nil);

static node_t *_max(const node_t *n, const node_t *nil);

static void _fix_insert_balance(rbtree *t, node_t *n);

static void _transplant(rbtree *t, node_t *d, node_t *p);

static void _fix_erase_balance(rbtree *t, node_t *x);

static void _right_rotate(rbtree *t, node_t *n);

static void _left_rotate(rbtree *t, node_t *x);

static int _inorder(const rbtree *t, node_t *node, key_t *arr, int i, int n);

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
    free(n);
}

node_t *rbtree_insert(rbtree *t, const key_t key) {
    node_t *y = t->nil;
    node_t *x = t->root;
    while (x != t->nil) {
        y = x;
        if (x->key <= key) {
            x = x->right;
        } else {
            x = x->left;
        }
    }
    x = (node_t *) calloc(1, sizeof(node_t));
    x->parent = y;
    x->key = key;
    x->left = t->nil;
    x->right = t->nil;
    x->color = RBTREE_RED;
    if (y == t->nil) {
        t->root = x;
    } else if (y->key <= key) {
        y->right = x;
    } else {
        y->left = x;
    }
    _fix_insert_balance(t, x);
    return x;
}

static void _fix_insert_balance(rbtree *t, node_t *n) {
    node_t *m;
    while (n->parent->color == RBTREE_RED) {
        m = n->parent->parent; // 할아부지
        if (n->parent->parent->left->color == n->parent->parent->right->color) { //case 1
            m->color = RBTREE_RED;
            m->left->color = RBTREE_BLACK;
            m->right->color = RBTREE_BLACK;
            n = m;
        } else if (n->parent == m->left) {
            if (n == n->parent->right) { // case 2
                n = n->parent;
                _left_rotate(t, n);
            }
            n->parent->color = RBTREE_BLACK; // case 3
            m->color = RBTREE_RED;
            _right_rotate(t, m);
        } else {
            if (n == n->parent->left) { // case 2
                n = n->parent;
                _right_rotate(t, n);
            }
            n->parent->color = RBTREE_BLACK;
            m->color = RBTREE_RED;
            _left_rotate(t, m);
        }
    }
    t->root->color = RBTREE_BLACK;
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
    node_t *p = t->root;
    while (p != t->nil && p->key != key) {
        if (p->key < key) {
            p = p->right;
        } else {
            p = p->left;
        }
    }
    if (p == t->nil) {
        return NULL;
    } else {
        return p;
    }
}

node_t *rbtree_min(const rbtree *t) {
    return _min(t->root, t->nil);
}

static node_t *_min(const node_t *n, const node_t *nil) {
    node_t *p = (node_t *) n;
    while (p != nil && p->left != nil) {
        p = p->left;
    }
    return p == nil ? NULL : p;
}

node_t *rbtree_max(const rbtree *t) {
    return _max(t->root, t->nil);
}

static node_t *_max(const node_t *n, const node_t *nil) {
    node_t *p = (node_t *) n;
    while (p != nil && p->right != nil) {
        p = p->right;
    }
    return p == nil ? NULL : p;
}

int rbtree_erase(rbtree *t, node_t *z) {
    color_t color = z->color;
    node_t *y = z;
    node_t *x;
    if (z->left == t->nil) {
        x = z->right;
        _transplant(t, z, z->right);
    } else if (z->right == t->nil) {
        x = z->left;
        _transplant(t, z, z->left);
    } else {
        y = _min(z->right, t->nil);
        color = y->color;
        x = y->right;
        if (y == z->right) {
            x->parent = y;
        } else {
            _transplant(t, y, y->right);
            y->right = z->right;
            y->right->parent = y;
        }
        _transplant(t, z, y);
        y->left = z->left;
        y->left->parent = y;
        y->color = z->color;
    }
    if (color == RBTREE_BLACK) {
        _fix_erase_balance(t, x);
    }
    free(z);
    return 0;
}

static void _fix_erase_balance(rbtree *t, node_t *x) {
    while (x != t->root && x->color == RBTREE_BLACK) {
        node_t *w;
        if (x == x->parent->left) {
            w = x->parent->right; // 형제
            if (w->color == RBTREE_RED) { // case 1
                w->color = RBTREE_BLACK;
                x->parent->color = RBTREE_RED;
                _left_rotate(t, x->parent);
                w = x->parent->right;
            }
            if (w->left->color == RBTREE_BLACK && w->right->color == RBTREE_BLACK) { // case 2
                w->color = RBTREE_RED;
                x = x->parent;
            } else {
                if (w->right->color == RBTREE_BLACK) { //case 3
                    w->color = RBTREE_RED;
                    w->left->color = RBTREE_BLACK;
                    _right_rotate(t, w);
                    w = x->parent->right;
                }
                w->color = x->parent->color; // case 4
                x->parent->color = RBTREE_BLACK;
                w->right->color = RBTREE_BLACK;
                _left_rotate(t, x->parent);
                x = t->root;
            }
        } else {
            w = x->parent->left; // 형제
            if (w->color == RBTREE_RED) { // case 1
                w->color = RBTREE_BLACK;
                x->parent->color = RBTREE_RED;
                _right_rotate(t, x->parent);
                w = x->parent->left;
            }
            if (w->left->color == RBTREE_BLACK && w->right->color == RBTREE_BLACK) { // case 2
                w->color = RBTREE_RED;
                x = x->parent;
            } else {
                if (w->left->color == RBTREE_BLACK) { //case 3
                    w->color = RBTREE_RED;
                    w->right->color = RBTREE_BLACK;
                    _left_rotate(t, w);
                    w = x->parent->left;
                }
                w->color = x->parent->color;
                x->parent->color = RBTREE_BLACK;
                w->left->color = RBTREE_BLACK;
                _right_rotate(t, x->parent);
                x = t->root;
            }
        }
    }
    x->color = RBTREE_BLACK;
}


static void _transplant(rbtree *t, node_t *d, node_t *p) {
    if (d->parent == t->nil) {
        t->root = p;
    } else if (d == d->parent->left) {
        d->parent->left = p;
    } else {
        d->parent->right = p;
    }
    p->parent = d->parent;
}

static int _inorder(const rbtree *t, node_t *node, key_t *arr, int i, int n) {
    if (i >= n)
        return 0;
    if (node != t->nil) {
        if (node->left != t->nil)
            i = _inorder(t, node->left, arr, i, n);
        arr[i++] = node->key;
        if (node->right != t->nil)
            i = _inorder(t, node->right, arr, i, n);
    }
    return i;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
    _inorder(t, t->root, arr, 0, n);
    return 0;
}

static void _left_rotate(rbtree *t, node_t *x) {
    node_t *y = x->right;
    x->right = y->left; //m의 오른쪽 서브트리를 n의 왼쪽 서브트리로 연결
    if (y->left != t->nil) {
        y->left->parent = x;
    }

    y->parent = x->parent;
    if (x->parent == t->nil) {
        t->root = y;
    } else if (x == x->parent->left) {
        x->parent->left = y;
    } else {
        x->parent->right = y;
    }
    y->left = x; // 멋있다 김남훈
    x->parent = y;
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
