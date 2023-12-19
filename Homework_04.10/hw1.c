#include <stdio.h>
#include <stdlib.h>
struct ls{
        int x;
        struct ls *next;
    };


void push_ls(struct ls *l, int a){
    struct ls *n, *p;
    n = l;
    while(n != NULL){
        p = n;
        n = n->next;
    }
    n = (struct ls*) malloc(sizeof(struct ls));
    n->next = NULL;
    n->x = a;
    p->next = n;
}

void deln_ls(struct ls *l, int a){
    struct ls *p, *n;
    n = l;
    while(n != NULL){
        if (n->x == a){
            n = n->next;
            free(p->next);
            p->next = n;
            break;
        }
        p = n;
        n = n->next;
    }
}

void deln_all_ls(struct ls *l, int a){
    struct ls *p, *n;
    n = l;
    while(n != NULL){
        if (n->x == a){
            n = n->next;
            free(p->next);
            p->next = n;
        }
        p = n;
        n = n->next;
    }
}
struct btree_f{
    int x;
    struct btree_f* left;
    struct btree_f* right;
};

void out_btf_inc(struct btree_f *t){
    if (t != NULL){
        printf("%d\n", t->x);
        out_btf_inc(t->left);
        out_btf_inc(t->right);
    }
}