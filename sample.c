#include <stdbool.h>
#include <stdio.h>
#include <assert.h>
#include <alloca.h>
#include "ll.h"

#define LEN(A) (sizeof(A) / sizeof(A[0]))

enum {ELNUM = 11};

#define listprint(nodeT, l) do {        \
	nodeT *L = (nodeT *)(l);        \
	SingleLink_for_all (L)          \
		printf("%zu ", L->val); \
	putchar('\n');                  \
} while(0)

#define nxt(node) (void *)((node).nxt)

static void DoubleLink_test(void)
{
        typedef struct { DoubleLink lnk; size_t val; } dlnode;

	puts("DoubleLink_test()...");

	dlnode *head = &(dlnode) {.lnk = DoubleLink_INIT};
        DoubleLink *p = &head->lnk;
        for (size_t i = 0; i < ELNUM; i++, p = p->nxt) {
		dlnode *node = alloca(sizeof(*node));
                *node = (dlnode) {DoubleLink_INIT, i};
                DoubleLink_insert(p, &node->lnk, &node->lnk);
        }

        head = nxt(head->lnk);
        puts("Original List :");
        listprint(dlnode, head);

        DoubleLink_reverse(&head->lnk, p);
	puts("Reversed :");
        listprint(dlnode, p);

        DoubleLink_remove(DoubleLink_get(p, 1, false), DoubleLink_get(p, 2, false));
	puts("3rd element deleted : ");
	listprint(dlnode, p);

        DoubleLink *newp = p->nxt;
        DoubleLink_swap(p, p, p->nxt, p->nxt);
	puts("First and second element swapped :");
	listprint(dlnode, newp);
}

static void SingleLink_test(void)
{
        typedef struct { SingleLink lnk; size_t val; } slnode;

	puts("SingleLink_test()...");

        slnode *head = &(slnode) {.lnk = SingleLink_INIT};
        SingleLink *p = &head->lnk;
        for (size_t i = 0; i < ELNUM; i++, p = p->nxt) {
		slnode *node = alloca(sizeof(*node));
                *node = (slnode) {SingleLink_INIT, i};
		SingleLink_insert(p, &node->lnk, &node->lnk);      
        }

        head = nxt(head->lnk);
        puts("Original List :");
        listprint(slnode, head);

        SingleLink_reverse(NULL, &head->lnk, p);
        puts("Reversed :");
        listprint(slnode, p);

        SingleLink_remove(SingleLink_get(p, 1), SingleLink_get(p, 2));
        puts("3rd element deleted : ");
        listprint(slnode, p);

        
        SingleLink *newp = p->nxt;
        SingleLink_swap(NULL, p, p, p, p->nxt, p->nxt);
        puts("First and second element swapped :");
        listprint(slnode, newp);
}

int main(void)
{
        SingleLink_test();
        DoubleLink_test();
}
