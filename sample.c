#include <stdbool.h>
#include <stdio.h>
#include <assert.h>
#include "ll.h"

#define LEN(A) (sizeof(A) / sizeof(A[0]))

enum {ELNUM = 11};

typedef struct {
        DoubleLink lnk;
        size_t val;
} inode;

static void ilst_print(const inode *l)
{
        DoubleLink_for_all(l)
                printf("%zu ", l->val);
        putchar('\n');
}

static inline void *nxt(const void *l) { return ((DoubleLink *)l)->nxt; }

static void DoubleLink_test(void)
{
        puts("DoubleLink_test()...");
        inode *head = &(inode){.lnk = DoubleLink_INIT};
        DoubleLink *p = &head->lnk;
        inode buf[ELNUM];
        for (size_t i = 0; i < LEN(buf); i++, p = p->nxt) {
                buf[i] = (inode) {DoubleLink_INIT, i};
                DoubleLink_insert(p, &buf[i].lnk, &buf[i].lnk);      
        }
        head = nxt(&head->lnk);
        puts("Original List :");
        ilst_print(head);

        puts("Reversed :");
        DoubleLink_reverse(&head->lnk, p);
        ilst_print((inode *)p);

        puts("3rd element deleted : ");
        DoubleLink_remove(DoubleLink_get(p, 1, false), DoubleLink_get(p, 2, false));
        ilst_print((inode *)p);

        puts("First and second element swapped :");
        DoubleLink *newp = p->nxt;
        DoubleLink_swap(p, p, p->nxt, p->nxt);
        ilst_print((inode *)newp);
}

typedef struct { 
        SingleLink lnk;
        size_t val;
} sinode;

static void silst_print(const sinode *l)
{
        SingleLink_for_all(l)
                printf("%zu ", l->val);
        putchar('\n');
}

static void SingleLink_test(void)
{
        puts("SingleLink_test()...");
        sinode *head = &(sinode){.lnk = SingleLink_INIT};
        SingleLink *p = &head->lnk;
        sinode buf[ELNUM];
        for (size_t i = 0; i < LEN(buf); i++, p = p->nxt) {
                buf[i] = (sinode) {SingleLink_INIT, i};
                SingleLink_insert(p, &buf[i].lnk, &buf[i].lnk);      
        }
        head = nxt(&head->lnk);
        puts("Original List :");
        silst_print(head);

        SingleLink_reverse(NULL, &head->lnk, p);
        puts("Reversed :");
        silst_print((sinode *)p);

        SingleLink_remove(SingleLink_get(p, 1), SingleLink_get(p, 2));
        puts("3rd element deleted : ");
        silst_print((sinode *)p);

        
        SingleLink *newp = p->nxt;
        SingleLink_swap(NULL, p, p, p, p->nxt, p->nxt);
        puts("First and second element swapped :");
        silst_print((sinode *)newp);
}

int main(void)
{
        SingleLink_test();
        DoubleLink_test();
}
