#ifndef LL_H_
#define LL_H_

#include <stddef.h>  /* NULL */
#include <stdbool.h>
#include <stdint.h>  /* intmax_t, uintmax_t */

/* Base structs for singularly linked and doubly linked lists.
 * Declare an instance as the first member in your node struct,
 * to derive from these and use the below functions. 
 */
typedef struct SingleLink { struct SingleLink *nxt; }       SingleLink;
typedef struct DoubleLink { struct DoubleLink *nxt, *prv; } DoubleLink;

/* Constant expression default initialzers. */
#define SingleLink_INIT (SingleLink){0}
#define DoubleLink_INIT (DoubleLink){0}

/* Iterates "n" times from "begin" until (but excluding) the "n"th node.
 * UB if < n-1 nodes follow.
 *
 * Where,
 * "begin" is a modifiable pointer lvalue to a SingleLink/DoubleLink object,
 * or to an aggregate that contains such an object as it's first member.
 * "cnt" is a modifiable integer lvalue.
 * "n" is a unsigned integer rvalue.
 */
#define SingleLink_for_n(begin, cnt, n)     \
	for (cnt = 0; cnt < (n); begin = (void *)((SingleLink *)begin)->nxt, cnt++)
#define DoubleLink_for_n(begin, cnt, n)     \
	for (cnt = 0; cnt < (n); begin = (void *)((DoubleLink *)begin)->nxt, cnt++)
#define DoubleLink_for_n_back(begin, cnt, n) \
	for (cnt = 0; cnt < (n); begin = (void *)((DoubleLink *)begin)->prv, cnt++)

/* Iterates from "begin" until reaching (but excluding) the "end" node.
 * UB if "end" does not follow.
 *
 * Where,
 * "begin" is as specified for *_for_n() macros.
 * "end" is a pointer rvalue to a SingleLink/DoubleLink object,
 * or to an aggregate that contains such an object as it's first member.
 */
#define SingleLink_until(begin, end)     \
	for (; (void *)begin != (void *)end; begin = (void *)((SingleLink *)begin)->nxt)
#define DoubleLink_until(begin, end)     \
	for (; (void *)begin != (void *)end; begin = (void *)((DoubleLink *)begin)->nxt)
#define DoubleLink_until_back(begin, end) \
	for (; (void *)begin != (void *)end; begin = (void *)((DoubleLink *)begin)->prv)

/* Iterates from "begin" for all nodes in the list.
 *
 * Where,
 * "begin" is as specified for *_for_n() macros.
 */
#define SingleLink_for_all(begin)      SingleLink_until(begin, NULL)
#define DoubleLink_for_all(begin)      DoubleLink_until(begin, NULL)
#define DoubleLink_for_all_back(begin) DoubleLink_until_back(begin, NULL)

/* Returns nth node after begin, or NULL if n is out-of-bounds.
 * If n is 0, returns last node in list.
 *
 * O(n) time and O(1) space complexity.
 */
SingleLink *SingleLink_get(const SingleLink *begin, uintmax_t n);
DoubleLink *DoubleLink_get(const DoubleLink *begin, uintmax_t n, bool reverse);

/* Returns number of nodes between begin and end (excluding begin)
 * or -1 if end is not found.
 * If end is NULL, returns number of nodes from begin onwards till end of list.
 *
 * O(n) time and O(1) space complexity.
 */
intmax_t SingleLink_len(const SingleLink *begin, const SingleLink *end);
/* If reverse is true, iterates in reverse from begin to find end */
intmax_t DoubleLink_len(const DoubleLink *begin, const DoubleLink *end, bool reverse);

/* Inserts the range [src_begin, src_end] after dst.
 * src_end must not preceed src_begin and the source range must not overlap
 * with the destination range [dst, dst->nxt].
 *
 * O(1) space & time complexity.
 */
void SingleLink_insert(SingleLink *dst, SingleLink *src_begin, SingleLink *src_end);
void DoubleLink_insert(DoubleLink *dst, DoubleLink *src_begin, DoubleLink *src_end);

/* Removes all nodes till end (including end) after begin.
 * end must not preceed begin.
 *
 * O(1) space & time complexity.
 */
void SingleLink_remove(SingleLink *begin, SingleLink *end);
void DoubleLink_remove(DoubleLink *begin, DoubleLink *end);

/* Swaps [a_begin, a_end] and [b_begin, b_end]
 * a_end must not preceed a_begin, b_end must not preceed b_begin.
 * Neither ranges may overlap with each other.
 *
 * O(1) space & time complexity.
 */
void SingleLink_swap(
		SingleLink *a_begin_prv, SingleLink *a_begin, SingleLink *a_end,
		SingleLink *b_begin_prv, SingleLink *b_begin, SingleLink *b_end
);
void DoubleLink_swap(
		DoubleLink *a_begin, DoubleLink *a_end,
		DoubleLink *b_begin, DoubleLink *b_end
);

/* Transforms [begin, end] to [end, begin] in-place.
 * end must not preceed begin.
 *
 * O(n) time and O(1) space complexity.
 */
void SingleLink_reverse(SingleLink *begin_prv, SingleLink *begin, SingleLink *end);
void DoubleLink_reverse(DoubleLink *begin, DoubleLink *end);

#endif
