#ifndef LL_H_
#define LL_H_

#include <stddef.h>  /* NULL */
#include <stdbool.h>
#include <stdint.h>  /* intmax_t, uintmax_t */

/* Base structs for singularly linked and doubly linked lists.
 * Declare an instance as the first member in your node struct,
 * to derive from these and use the below functions. 
 */
typedef struct SingleLink { struct SingleLink *nxt; } SingleLink;
typedef struct DoubleLink { struct DoubleLink *nxt, *prv; } DoubleLink;

#define SingleLink_INIT ( (SingleLink) {NULL}       )
#define DoubleLink_INIT ( (DoubleLink) {NULL, NULL} )

#define _m_(s, T, m)                 ( (void *)((T *)s)->m )
#define _for_n_(begin, cnt, n, T, m) for ((cnt) = 0; (begin) && (cnt) < (n); (begin) = _m_(begin, T, m), (cnt)++)
#define _until_(begin, end, T, m)    for (; (begin) && (void *)(begin) != (void *)(end); (begin) = _m_(begin, T, m))

/* Iterates over [begin, begin+n) or [begin,EOL) if n is out of range,
 * where EOL is one-past-the-end of the list.
 */
#define SingleLink_for_n(begin, cnt, n)      _for_n_(begin, cnt, n, SingleLink, nxt)
#define DoubleLink_for_n(begin, cnt, n)      _for_n_(begin, cnt, n, DoubleLink, nxt)
#define DoubleLink_for_n_back(begin, cnt, n) _for_n_(begin, cnt, n, DoubleLink, prv)

/* Iterates over [begin, end) or [begin,EOL) if end is not found. */
#define SingleLink_until(begin, end)         _until_(begin, end, SingleLink, nxt)
#define DoubleLink_until(begin, end)         _until_(begin, end, DoubleLink, nxt)
#define DoubleLink_until_back(begin, end)    _until_(begin, end, DoubleLink, prv)

/* Iterates from [begin, EOL). */
#define SingleLink_for_all(begin)            SingleLink_until(begin, NULL)
#define DoubleLink_for_all(begin)            DoubleLink_until(begin, NULL)
#define DoubleLink_for_all_back(begin)       DoubleLink_until_back(begin, NULL)

/* Returns nth node after begin, or NULL if n is out of range. 
 * If n is 0, returns last node.
 */
SingleLink *SingleLink_get(const SingleLink *begin, uintmax_t n_nodes);
DoubleLink *DoubleLink_get(const DoubleLink *begin, uintmax_t n_nodes, bool reverse);

/* Returns number of nodes in [begin, end] or -1 if end is not found.
 * If end is NULL, returns number of nodes in [begin, EOL).
 */
intmax_t SingleLink_len(const SingleLink *begin, const SingleLink *end);
/* If reverse is true, iterates in reverse from begin to find end */
intmax_t DoubleLink_len(const DoubleLink *begin, const DoubleLink *end, bool reverse);

/* Inserts the range [src_begin, src_end] between dst & dst->nxt.
 * src_end may not preceed src_begin and the source range must not overlap
 * with the destination range [dst, dst+1].
 */
void SingleLink_insert(SingleLink *dst, SingleLink *src_begin, SingleLink *src_end);
void DoubleLink_insert(DoubleLink *dst, DoubleLink *src_begin, DoubleLink *src_end);

/* Removes (begin, end] from begin.
 * end must not preceed begin.
 */
void SingleLink_remove(SingleLink *begin, SingleLink *end);
void DoubleLink_remove(DoubleLink *begin, DoubleLink *end);

/* Swaps [a_begin, a_end] and [b_begin, b_end]
 * a_end must not preceed a_begin, b_end must not preceed b_begin.
 * Neither ranges may overlap with each other.
 */
void SingleLink_swap(SingleLink *a_begin_prv, SingleLink *a_begin, SingleLink *a_end, SingleLink *b_begin_prv, SingleLink *b_begin, SingleLink *b_end);
void DoubleLink_swap(DoubleLink *a_begin, DoubleLink *a_end, DoubleLink *b_begin, DoubleLink *b_end);

/* Transforms [begin, end] to [end, begin] in-place.
 * end must not preceed begin.
 */
void SingleLink_reverse(SingleLink *begin_prv, SingleLink *begin, SingleLink *end);
void DoubleLink_reverse(DoubleLink *begin, DoubleLink *end);

#endif
