/*
 * This file is part of c-list. See COPYING for details.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by the
 * Free Software Foundation; either version 2.1 of the License, or (at
 * your option) any later version.
 */

/*
 * Tests for Public API
 * This test, unlikely the others, is linked against the real, distributed,
 * shared library. Its sole purpose is to test for symbol availability.
 */

#undef NDEBUG
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "c-list.h"

typedef struct {
        int id;
        CList link;
} Node;

static void test_api(void) {
        CList *list_iter, *list_safe, list = C_LIST_INIT(list);
        Node *node_iter, *node_safe, node = { .id = 0, .link = C_LIST_INIT(node.link) };

        assert(c_list_entry(&node.link, Node, link) == &node);
        assert(!c_list_is_linked(&node.link));
        assert(c_list_is_empty(&list));

        /* basic link / unlink calls */

        c_list_link_before(&list, &node.link);
        assert(c_list_is_linked(&node.link));
        assert(!c_list_is_empty(&list));

        c_list_unlink(&node.link);
        assert(c_list_is_linked(&node.link));
        assert(c_list_is_empty(&list));

        c_list_link_after(&list, &node.link);
        assert(c_list_is_linked(&node.link));
        assert(!c_list_is_empty(&list));

        c_list_unlink_init(&node.link);
        assert(!c_list_is_linked(&node.link));
        assert(c_list_is_empty(&list));

        /* link / unlink aliases */

        c_list_link_front(&list, &node.link);
        assert(c_list_is_linked(&node.link));

        c_list_unlink_init(&node.link);
        assert(!c_list_is_linked(&node.link));

        c_list_link_tail(&list, &node.link);
        assert(c_list_is_linked(&node.link));

        c_list_unlink_init(&node.link);
        assert(!c_list_is_linked(&node.link));

        /* loop helpers */

        assert(c_list_loop_first(&list) == list.next);
        assert(c_list_loop_last(&list) == list.prev);
        assert(c_list_loop_next(&list) == list.next);
        assert(c_list_loop_prev(&list) == list.prev);

        /* loop macros */

        c_list_for_each(list_iter, &list)
                assert(list_iter != &list);
        c_list_for_each_safe(list_iter, list_safe, &list)
                assert(list_iter != &list);
        c_list_for_each_entry(node_iter, &list, link)
                assert(&node_iter->link != &list);
        c_list_for_each_entry_safe(node_iter, node_safe, &list, link)
                assert(&node_iter->link != &list);

        /* list accessors */

        assert(!c_list_first(&list));
        assert(!c_list_last(&list));
        assert(!c_list_first_entry(&list, Node, link));
        assert(!c_list_last_entry(&list, Node, link));
}

int main(int argc, char **argv) {
        test_api();
        return 0;
}
