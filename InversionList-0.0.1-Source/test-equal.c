//
// Created by solex on 25/04/23.
//
#include <errno.h>
#include <stdlib.h>
#include <string.h>

#ifdef NDEBUG
#undef NDEBUG
#endif
#include <assert.h>

#include "./inversion-list.h"
#include "./inversion-list.inc"

int main(void) {
    inversion_list_init();
    {
        unsigned int a[] = {1, 2, 3, 5, 7, 8, 9};
        unsigned int b[] = {1, 2, 4, 5, 7, 8, 12,592};

        InversionList *set = inversion_list_create(20, sizeof a / sizeof *a, a);
        InversionList *set2 = inversion_list_create(1024,sizeof b/sizeof  *b,b);
        InversionList *set_clone_cap = inversion_list_create(10000000, sizeof a / sizeof *a, a);
        assert(inversion_list_equal(set,set));
        assert(inversion_list_not_equal(set,set2));
        assert(inversion_list_equal(set,set_clone_cap));
        inversion_list_destroy(set);
        inversion_list_destroy(set2);
        inversion_list_destroy(set_clone_cap);
    }
    inversion_list_finish();
    return EXIT_SUCCESS;
}
