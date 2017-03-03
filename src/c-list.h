#pragma once

/*
 * Circular Double Linked List Implementation in Standard ISO-C11
 *
 * This implements a generic circular double linked list. List entries must
 * embed the CList object, which provides pointers to the next and previous
 * element. Insertion and removal can be done in O(1) due to the double links.
 * Furthermore, the list is circular, thus allows access to front/tail in O(1)
 * as well, even if you only have a single head pointer (which is not how the
 * list is usually operated, though).
 *
 * Note that you are free to use the list implementation without a head
 * pointer. However, usual operation uses a single CList object as head, which
 * is itself linked in the list and as such must be identified as list head.
 * This allows very simply list operations and avoids a lot of special cases.
 * Most importantly, you can unlink entries without requiring a head pointer.
 */

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>

typedef struct CList CList;

/**
 * struct CList - Entry of a circular double linked list
 * @next:               next entry
 * @prev:               previous entry
 *
 * Each entry in a list must embed a CList object. This object contains
 * pointers to its next and previous elements, which can be freely accessed by
 * the API user at any time. Note that the list is circular, and the list head
 * is linked in the list as well.
 *
 * The list head must be initialized via C_LIST_INIT before use. There is no
 * reason to initialize entry objects before linking them. However, if you need
 * a boolean state that tells you whether the entry is linked or not, you should
 * initialize the entry via C_LIST_INIT as well.
 */
struct CList {
        CList *next;
        CList *prev;
};

#define C_LIST_INIT(_var) { .next = &(_var), .prev = &(_var) }

/**
 * c_list_entry() - get parent container of list entry
 * @_what:              list entry, or NULL
 * @_t:                 type of parent container
 * @_m:                 member name of list entry in @_t
 *
 * If the list entry @_what is embedded into a surrounding structure, this will
 * turn the list entry pointer @_what into a pointer to the parent container
 * (using offsetof(3), or sometimes called container_of(3)).
 *
 * If @_what is NULL, this will also return NULL.
 *
 * Return: Pointer to parent container, or NULL.
 */
#define c_list_entry(_what, _t, _m) \
        ((_t *)(((void *)(_what) ?: \
                 (void *)NULL + offsetof(_t, _m)) - offsetof(_t, _m)))

/**
 * c_list_is_linked() - check whether a entry is linked
 * @what:               entry to check, or NULL
 *
 * Return: True if @what is linked in a list, false if not.
 */
static inline _Bool c_list_is_linked(CList *what) {
        return what && what->next != what;
}

/**
 * c_list_is_empty() - check whether a list is empty
 * @list:               list to check, or NULL
 *
 * Return: True if @list is empty, false if not.
 */
static inline _Bool c_list_is_empty(CList *list) {
        return !list || !c_list_is_linked(list);
}

/**
 * c_list_link_before() - link entry into list
 * @where:              linked list entry used as anchor
 * @what:               entry to link
 *
 * This links @what directly in front of @where. @where can either be a list
 * head or any entry in the list.
 *
 * If @where points to the list head, this effectively links @what as new tail
 * element. Hence, the macro c_list_link_tail() is an alias to this.
 *
 * @what is not inspected prior to being linked. Hence, it better not be linked
 * into another list, or the other list will be corrupted.
 */
static inline void c_list_link_before(CList *where, CList *what) {
        CList *prev = where->prev, *next = where;

        next->prev = what;
        what->next = next;
        what->prev = prev;
        prev->next = what;
}
#define c_list_link_tail(_list, _what) c_list_link_before((_list), (_what))

/**
 * c_list_link_after() - link entry into list
 * @where:              linked list entry used as anchor
 * @what:               entry to link
 *
 * This links @what directly after @where. @where can either be a list head or
 * any entry in the list.
 *
 * If @where points to the list head, this effectively links @what as new front
 * element. Hence, the macro c_list_link_front() is an alias to this.
 *
 * @what is not inspected prior to being linked. Hence, it better not be linked
 * into another list, or the other list will be corrupted.
 */
static inline void c_list_link_after(CList *where, CList *what) {
        CList *prev = where->prev, *next = where;

        next->prev = what;
        what->next = next;
        what->prev = prev;
        prev->next = what;
}
#define c_list_link_front(_list, _what) c_list_link_after((_list), (_what))

/**
 * c_list_unlink() - unlink element from list
 * @what:               element to unlink
 *
 * This unlinks @what. If @what was initialized via C_LIST_INIT(), it has no
 * effect. If @what was never linked, nor initialized, behavior is undefined.
 *
 * Note that this does not modify @what. It just modifies the previous and next
 * elements in the list to no longer reference @what. If you want to make sure
 * @what is re-initialized after removal, use c_list_unlink_init().
 */
static inline void c_list_unlink(CList *what) {
        CList *prev = what->prev, *next = what->next;

        next->prev = prev;
        prev->next = next;
}

/**
 * c_list_unlink_init() - unlink element from list and re-initialize
 * @what:               element to unlink
 *
 * This is like c_list_unlink() but re-initializes @what after removal.
 */
static inline void c_list_unlink_init(CList *what) {
        /* condition is not needed, but avoids STOREs in fast-path */
        if (c_list_is_linked(what)) {
                c_list_unlink(what);
                *what = (CList)C_LIST_INIT(*what);
        }
}

/**
 * c_list_loop_first() - return first list element, or head if empty
 * @list:               list to operate on
 *
 * This is an O(1) accessor to the first list element. If the list is empty,
 * this returns a pointer to the list head. Hence, this never returns NULL.
 *
 * Return: Pointer to first list element, or pointer to head if empty.
 */
static inline CList *c_list_loop_first(CList *list) {
        return list->next;
}

/**
 * c_list_loop_last() - return last list element, or head if empty
 * @list:               list to operate on
 *
 * This is an O(1) accessor to the last list element. If the list is empty,
 * this returns a pointer to the list head. Hence, this never returns NULL.
 *
 * Return: Pointer to last list element, or pointer to head if empty.
 */
static inline CList *c_list_loop_last(CList *list) {
        return list->prev;
}

/**
 * c_list_loop_next() - return next list element, or head if none
 * @what:               list entry to operate on
 *
 * This is an O(1) accessor to the next list element. If @what is the list tail
 * this will return a pointer to the list head. Hence, this never returns NULL.
 *
 * Return: Pointer to next list element, or pointer to head if none.
 */
static inline CList *c_list_loop_next(CList *what) {
        return what->next;
}

/**
 * c_list_loop_prev() - return previous list element, or head if none
 * @what:               list entry to operate on
 *
 * This is an O(1) accessor to the previous list element. If @what is the list
 * front this will return a pointer to the list head. Hence, this never returns
 * NULL.
 *
 * Return: Pointer to previous list element, or pointer to head if none.
 */
static inline CList *c_list_loop_prev(CList *what) {
        return what->prev;
}

/**
 * c_list_for_each() - loop over all list entries
 * @_iter:              iterator to use
 * @_list:              list to loop over
 *
 * This is a macro to use as for-loop to iterate an entire list. It is meant as
 * convenience macro. Feel free to code your own loop iterator.
 */
#define c_list_for_each(_iter, _list)                   \
        for (_iter = c_list_loop_first(_list);          \
             _iter != (_list);                          \
             _iter = c_list_loop_next(_iter))


/**
 * c_list_for_each_safe() - loop over all list entries, safe for removal
 * @_iter:              iterator to use
 * @_safe:              used to store pointer to next element
 * @_list:              list to loop over
 *
 * This is a macro to use as for-loop to iterate an entire list, safe against
 * removal of the current element. It is meant as convenience macro. Feel free
 * to code your own loop iterator.
 *
 * Note that this fetches the next element prior to executing the loop body.
 * This makes it safe against removal of the current entry, but it will go
 * havoc if you remove other list entries. You better not modify anything but
 * the current list entry.
 */
#define c_list_for_each_safe(_iter, _safe, _list)                                       \
        for (_iter = c_list_loop_first(_list), _safe = c_list_loop_next(_iter);         \
             _iter != (_list);                                                          \
             _iter = _safe, _safe = c_list_loop_next(_safe))

/**
 * c_list_for_each_entry() - loop over all list entries
 * @_iter:              iterator to use
 * @_list:              list to loop over
 * @_m:                 member name of CList object in list type
 *
 * This combines c_list_for_each() with c_list_entry(), making it easy to
 * iterate over a list of a specific type.
 */
#define c_list_for_each_entry(_iter, _list, _m)                                         \
        for (_iter = c_list_entry(c_list_loop_first(_list), __typeof__(*_iter), _m);    \
             &_iter->_m != (_list);                                                     \
             _iter = c_list_entry(c_list_loop_next(&_iter->_m), __typeof__(*_iter), _m))

/**
 * c_list_for_each_entry_safe() - loop over all list entries, safe for removal
 * @_iter:              iterator to use
 * @_safe:              used to store pointer to next element
 * @_list:              list to loop over
 * @_m:                 member name of CList object in list type
 *
 * This combines c_list_for_each_safe() with c_list_entry(), making it easy to
 * iterate over a list of a specific type.
 */
#define c_list_for_each_entry_safe(_iter, _safe, _list, _m)                             \
        for (_iter = c_list_entry(c_list_loop_first(_list), __typeof__(*_iter), _m),    \
             _safe = c_list_entry(c_list_loop_next(&_iter->_m), __typeof__(*_iter), _m);\
             &_iter->_m != (_list);                                                     \
             _iter = _safe,                                                             \
             _safe = c_list_entry(c_list_loop_next(&_safe->_m), __typeof__(*_iter), _m))

/**
 * c_list_first() - return pointer to first element, or NULL if empty
 * @list:               list to operate on, or NULL
 *
 * This returns a pointer to the first element, or NULL if empty. This never
 * returns a pointer to the list head.
 *
 * Return: Pointer to first list element, or NULL if empty.
 */
static inline CList *c_list_first(CList *list) {
        return c_list_is_empty(list) ? NULL : list->next;
}

/**
 * c_list_last() - return pointer to last element, or NULL if empty
 * @list:               list to operate on, or NULL
 *
 * This returns a pointer to the last element, or NULL if empty. This never
 * returns a pointer to the list head.
 *
 * Return: Pointer to last list element, or NULL if empty.
 */
static inline CList *c_list_last(CList *list) {
        return c_list_is_empty(list) ? NULL : list->prev;
}

/**
 * c_list_first_entry() - return pointer to first entry, or NULL if empty
 * @_list:              list to operate on, or NULL
 * @_t:                 type of list entries
 * @_m:                 name of CList member in @_t
 *
 * This is like c_list_first(), but also applies c_list_entry() on the result.
 *
 * Return: Pointer to first list entry, or NULL if empty.
 */
#define c_list_first_entry(_list, _t, _m) \
        c_list_entry(c_list_first(_list), _t, _m)

/**
 * c_list_last_entry() - return pointer to last entry, or NULL if empty
 * @_list:              list to operate on, or NULL
 * @_t:                 type of list entries
 * @_m:                 name of CList member in @_t
 *
 * This is like c_list_last(), but also applies c_list_entry() on the result.
 *
 * Return: Pointer to last list entry, or NULL if empty.
 */
#define c_list_last_entry(_list, _t, _m) \
        c_list_entry(c_list_last(_list), _t, _m)

#ifdef __cplusplus
}
#endif
