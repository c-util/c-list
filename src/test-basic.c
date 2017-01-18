/*
 * Tests for basic functionality
 * This contains basic, deterministic tests for list behavior, API
 * functionality, and usage.
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "c-list.h"

static void test_iterators(void) {
        CList *iter, *safe, a, b, list = C_LIST_INIT(list);
        unsigned int i;

        assert(!c_list_first(&list));
        assert(!c_list_last(&list));

        /* link @a and verify iterators see just it */

        c_list_link_tail(&list, &a);
        assert(c_list_is_linked(&a));
        assert(c_list_first(&list) == &a);
        assert(c_list_last(&list) == &a);

        i = 0;
        c_list_for_each(iter, &list) {
                assert(iter == &a);
                ++i;
        }
        assert(i == 1);

        /* link @b as well and verify iterators again */

        c_list_link_tail(&list, &b);
        assert(c_list_is_linked(&a));
        assert(c_list_is_linked(&b));

        i = 0;
        c_list_for_each(iter, &list) {
                assert((i == 0 && iter == &a) ||
                       (i == 1 && iter == &b));
                ++i;
        }
        assert(i == 2);

        /* verify safe-iterator while removing elements */

        i = 0;
        c_list_for_each_safe(iter, safe, &list) {
                assert(iter == &a || iter == &b);
                c_list_unlink(iter);
                ++i;
        }
        assert(i == 2);

        assert(c_list_is_empty(&list));
}

int main(int argc, char **argv) {
        test_iterators();
        return 0;
}
