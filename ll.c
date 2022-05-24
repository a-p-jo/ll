#include "ll.h"
#include <assert.h>

SingleLink *SingleLink_get(const SingleLink *p, uintmax_t n)
{
        assert(p);
        if (n) {
                uintmax_t cnt;
                SingleLink_for_n(p, cnt, n)
                        if (!p->nxt) /* out-of-bounds error */
                                return NULL;
        } else
                SingleLink_for_all(p)
                        ;
        return (SingleLink *)p; /* Cast away const */
}

DoubleLink *DoubleLink_get(const DoubleLink *p, uintmax_t n, bool back)
{
        assert(p);
        if (n) {
                uintmax_t cnt;
                if (back) {
                        DoubleLink_for_n_back(p, cnt, n)
                                if (!p->prv)
                                        return NULL;
                } else {
                        DoubleLink_for_n(p, cnt, n)
                                if (!p->nxt)
                                        return NULL;
                }
        } else
                if (back)
                        DoubleLink_for_all_back(p)
                                ;
                else
                        DoubleLink_for_all(p)
                                ;
        return (DoubleLink *)p;
}

intmax_t SingleLink_len(const SingleLink *begin, const SingleLink *end)
{
        assert(begin);
        uintmax_t cnt = 0;
        if (end)
                SingleLink_until(begin, end) {
                        if (!begin->nxt)
                                return -1; /* end not found */
                        cnt++;
                }
        else
                SingleLink_for_all(begin)
                        cnt++;
        return cnt;
}

intmax_t DoubleLink_len(const DoubleLink *begin, const DoubleLink *end, bool back)
{
        assert(begin && end);
        uintmax_t cnt = 0;
        if (back) {
                if (end)
                        DoubleLink_until_back(begin, end) {
                                if (!begin->prv)
                                        return -1;
                                cnt++;
                        }
                else
                        DoubleLink_for_all_back(begin)
                                cnt++;
        } else
                if (end)
                        DoubleLink_until(begin, end) {
                                if (!begin->nxt)
                                        return -1;
                                cnt++;
                        }
                else
                        DoubleLink_for_all(begin)
                                cnt++;
        return cnt;
}

void SingleLink_insert(SingleLink *dst, SingleLink *begin, SingleLink *end)
{
        assert(dst && begin && end && SingleLink_len(begin, end) >= 0);

        end->nxt = dst->nxt;
        dst->nxt = begin;
}

void DoubleLink_insert(DoubleLink *dst, DoubleLink *begin, DoubleLink *end)
{
        assert(dst && begin && end && DoubleLink_len(begin, end, false) >= 0);

        end->nxt = dst->nxt;
        /* If dst->nxt is NULL, dst is the tail node;
	 * there are none after it to update. 
         * Otherwise, update the prv of the node after dst.
         */
        if (dst->nxt)
                dst->nxt->prv = end;
        dst->nxt = begin, begin->prv = dst;
}

void SingleLink_remove(SingleLink *begin, SingleLink *end)
{
        assert(begin && end && SingleLink_len(begin, end) >= 0);

        begin->nxt = end->nxt;
}

void DoubleLink_remove(DoubleLink *begin, DoubleLink *end)
{
        assert(begin && end && DoubleLink_len(begin, end, false) >= 0);
        begin->nxt = end->nxt;
        if (end->nxt)
                end->nxt->prv = begin;
}

void DoubleLink_swap(
		DoubleLink *a_begin, DoubleLink *a_end, 
		DoubleLink *b_begin, DoubleLink *b_end
) {
        assert(
                a_begin && a_end && b_begin && b_end
                && DoubleLink_len(a_begin, a_end, false) >= 0
                && DoubleLink_len(b_begin, b_end, false) >= 0
        );

        /* Unless a_begin is the head node in its list, update its prv to b_begin */
        if (a_begin->prv)
                a_begin->prv->nxt = b_begin;
        /* If b_begin immediately follows a_end (ranges are consecutive),
         * do NOT update a_end's next node's prv to it, as this would result in
         * a cicular (infinite) link.
         */
        if (a_end->nxt && a_end->nxt != b_begin)
                a_end->nxt->prv = b_end; 
        if (b_begin->prv && b_begin->prv != a_end)
                b_begin->prv->nxt = a_begin;
        if (b_end->nxt)
                b_end->nxt->prv = a_end;

        DoubleLink *tmp = a_begin->prv;
        /* If a_begin and b_begin are consecutive 
	 * (range a is a singleton consecutive to b),
         * exchanging their prv will cause a circular link.
         * Instead, make b_begin the node preceeding a_begin.
         */
        if (a_begin != b_begin->prv)
                a_begin->prv = b_begin->prv;
        else
                a_begin->prv = b_begin;
        b_begin->prv = tmp;

        tmp = a_end->nxt;
        a_end->nxt = b_end->nxt;
        /* Similarly, only exchange b_end and a_end's nxt if they are not consecutive. 
         * (This implies range b is a singleton consecutive to a).
         */
        if (b_end != tmp)
                b_end->nxt = tmp;
        else
                b_end->nxt = a_end;
}

void SingleLink_swap(
		SingleLink *a_begin_prv, SingleLink *a_begin, SingleLink *a_end,
		SingleLink *b_begin_prv, SingleLink *b_begin, SingleLink *b_end
) {
        assert(
                a_begin && a_end && b_begin && b_end
                && a_begin_prv? a_begin_prv->nxt == a_begin : true
                && b_begin_prv? b_begin_prv->nxt == b_begin : true
                && SingleLink_len(a_begin, a_end) >= 0
                && SingleLink_len(b_begin, b_end) >= 0
        );
        if (a_begin_prv)
                a_begin_prv->nxt = b_begin;
        if (b_begin_prv && b_begin_prv != a_end)
                b_begin_prv->nxt = a_begin;
        
        SingleLink *tmp = a_end->nxt;
        a_end->nxt = b_end->nxt;
        if (b_end != tmp)
                b_end->nxt = tmp;
        else
                b_end->nxt = a_end;
}

void DoubleLink_reverse(DoubleLink *left, DoubleLink *right)
{
        assert(left && right && DoubleLink_len(left, right, false) >= 0);

        /* Swap nodes from the left with corresponding nodes from the right,
	 * until the midpoint is reached.
         * Do not test for premature end of list. 
	 * If this causes a NULL derefrence, the segfault/UB is well-deserved.
         */
        while (left != right) {
                DoubleLink *nxt = left->nxt, *prv = right->prv;
                DoubleLink_swap(left, left, right, right);
                /* As the loop swaps 2 nodes per iteration, a range of even length
                 * cannot have a real middle node.
                 * However, just before the theoretical midpoint, the next node from the left
                 * will be the current node from the right.
                 * If so, the reversal is complete after the current swap.
                 */
                if (nxt == right)
                        break;
                left = nxt, right = prv;
        }
}

void SingleLink_reverse(SingleLink *begin_prv, SingleLink *begin, SingleLink *end)
{
        assert(
			begin && end && begin_prv? begin_prv->nxt == begin : true
			&& SingleLink_len(begin, end) >= 0
	);
        /* The loop assumes a range > 1 node */
        if (begin == end)
                return;
        /* If a node preceeds the range, update its nxt to the end of the range */
        if (begin_prv)
                begin_prv->nxt = end;
        /* For each node in the range, set it's nxt to the node preceeding it */
        for (
			SingleLink *prv = begin_prv, *cur = begin, *nxt = cur->nxt ;;
			prv = cur, cur = nxt, nxt = nxt->nxt
	) {
                cur->nxt = prv;
                if (cur == end) /* Range includes end; loop must set it too */
                        break;
        }
}
