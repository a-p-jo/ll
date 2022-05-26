### Base Structs
This library provides two base `struct`s, which handle the linking of nodes :
1. `SingleLink` :

  - for singularly linked lists, à la C++ STL's `forward_list`.
  - size is that of a single pointer.
2. `DoubleLink` : 
  - for doubly linked lists, à la C++ STL's `list`.
  -  size is of two pointers.
  -  inherits from `SingleLink`, so all `SingleLink` methods may be used on it.

To compose your linked list, simply *inherit* by making it the first member of your node `struct` :
```c
struct node { DoubleLink link; T value; };
```
What this *inheritance* means is that a `struct node *` and a `DoubleLink *` can be [correctly casted](https://en.cppreference.com/w/c/language/struct)
back and forth, which is crucial for the next feature provided by the library.  

### Range-based Loops
Loops of the `for_n()`, `until()` and `for_all()` variety (including `_back()` versions
for iterating in *reverse*) are provided as macros expanding to `for` loop statements.

Many examples can be found in `ll.c`, which uses these to implement methods. 
One particularly useful demonstration is :
```c
/* Count the number of nodes between begin and end, returning -1 if end not found */
intmax_t DoubleLink_len(const DoubleLink *begin, const DoubleLink *end, bool back)
{
        uintmax_t cnt = 0;
        if (back) {
                if (end)
                        DoubleLink_until_back(begin, end) {
                                if (!begin->prv) return -1; /* We're at the last node */        
                                else cnt++;
                        }
                else /* Count all nodes till list terminates if end is NULL */
                        DoubleLink_for_all_back(begin)
                                cnt++;
        } else
                if (end)
                        DoubleLink_until(begin, end) {
                                if (!begin->nxt) return -1;
                                else cnt++;
                        }
                else
                        DoubleLink_for_all(begin)
                                cnt++;
        return cnt;
}
```
These loops are not only convenient and error-free, but also very expressive to a future reader.
They may be used for any `strut node *` where the `struct node` *inherits* from an appropriate base struct.

### Methods
Methods are provided symetrically for both types of links.
All methods accept ranges as input, so they can operate on single nodes,
whole lists, and anything in between. 
***You*** must ensure that passed ranges are valid, and do not overlap where that is prohibited.
- `get()` : to return the nth node from a given node.
- `len()` : to count the number of nodes between two nodes, the inverse of `get()`.
- `insert()` : to link a range of nodes after a given node.
- `remove()` : to de-link a range of nodes, the inverse of `insert()`.
- `swap()` : to exchange the linking of two ranges of nodes.
- `reverse()` : to invert the order of linking for a range of nodes.

To see examples, take a look at `sample.c`.
