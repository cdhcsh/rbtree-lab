#include "rbtree.h"

#include <stdlib.h>
#include <stdio.h>

static void _delete_node(node_t *n, node_t *nil);

static node_t *_insert_node(node_t *n, const key_t key, node_t *nil);

static int _black_height(node_t *t);

static node_t *_min(const node_t *n, const node_t *nil);

static node_t *_max(const node_t *n, const node_t *nil);

static void _left_rotate(rbtree *t, node_t *n);

static void _right_rotate(rbtree *t, node_t *n);

static void _fix_insert_balance(rbtree *t, node_t *n);

static void _transplant(rbtree *t, node_t *d, node_t *p);

static void _fix_erase_balance(rbtree *t, node_t *n);

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
    _fix_insert_balance(t, n);
    return n;
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

int rbtree_erase(rbtree *t, node_t *n) {
    color_t color = n->color;
    node_t *p = n;
    node_t *d;
    if (n->left == t->nil) {
        d = n->right;
        _transplant(t, p, p->right);
    } else if (n->right == t->nil) {
        d = n->left;
        _transplant(t, p, p->left);
    } else {
        p = _min(n->right, t->nil);
        color = p->color;
        d = p->right;
        if (p->parent == n) {
            d->parent = n;
        } else {
            _transplant(t, p, p->right);
            p->right = n->right;
            p->right->parent = p;
        }
        _transplant(t, n, p);
        p->left = n->left;
        p->left->parent = p;
        p->color = n->color;
    }
    if (color == RBTREE_BLACK) {
        _fix_erase_balance(t, d);
    }
    return 1;
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

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
//    printf("%d", sizeof());
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

static void _fix_erase_balance(rbtree *t, node_t *n) {
    while (n != t->root && n->color == RBTREE_BLACK) {
        node_t *u = n->parent->left == n ? n->parent->left : n->parent->right;
        if (u->left->color == RBTREE_BLACK && u->right->color == RBTREE_BLACK) { // case 2
            u->color = RBTREE_RED;
            n = n->parent;
        } else if (n == n->parent->left) {
            if (u->color == RBTREE_RED) { // case 1
                n->parent->color = RBTREE_RED;
                u->color = RBTREE_BLACK;
                _left_rotate(t, n->parent);
                n = n->parent;
            } else {
                if (u->right->color == RBTREE_BLACK) { //case 3
                    u->color = RBTREE_RED;
                    u->right->color = RBTREE_BLACK;
                    _right_rotate(t, u);
                    u = n->parent->right;
                }
                u->color = n->parent->color; // case 4
                n->parent->color = RBTREE_BLACK;
                u->right->color = RBTREE_BLACK;
                _left_rotate(t, n->parent);
                n = t->root;
            }
        } else {
            if (u->color == RBTREE_RED) { // case 1
                n->parent->color = RBTREE_RED;
                u->color = RBTREE_BLACK;
                _right_rotate(t, n->parent);
                n = n->parent;
            } else {
                if (u->left->color == RBTREE_BLACK) { //case 3
                    u->color = RBTREE_RED;
                    u->left->color = RBTREE_BLACK;
                    _left_rotate(t, u);
                    u = n->parent->left;
                }
                u->color = n->parent->color; // case 4
                n->parent->color = RBTREE_BLACK;
                u->left->color = RBTREE_BLACK;
                _right_rotate(t, n->parent);
                n = t->root;
            }
        }
    }
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