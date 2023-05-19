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
    {
        unsigned int a[] = {5, 6, 7, 78, 79, 80, 81, 90};
        unsigned int b[] = {5, 6, 7, 78, 79, 80, 81};
        unsigned int c[] = {6, 7, 78, 79, 80, 81, 90};
        unsigned int d[] = {5, 6, 7, 78, 79, 80, 81, 90, 50};

        InversionList *set_a_uint8 = inversion_list_create(100, sizeof a / sizeof *a, a);
        InversionList *set_a_uint16 = inversion_list_create(1000, sizeof a / sizeof *a, a);
        InversionList *set_a_uint32 = inversion_list_create(100000, sizeof a / sizeof *a, a);
        InversionList *set_a_bis_uint8 = inversion_list_create(100, sizeof a / sizeof *a, a);
        InversionList *set_a_bis_uint16 = inversion_list_create(1000, sizeof a / sizeof *a, a);
        InversionList *set_a_bis_uint32 = inversion_list_create(100000, sizeof a / sizeof *a, a);

        InversionList *set_b_uint8 = inversion_list_create(100, sizeof b / sizeof *b, b);
        InversionList *set_b_uint16 = inversion_list_create(1000, sizeof b / sizeof *b, b);
        InversionList *set_b_uint32 = inversion_list_create(100000, sizeof b / sizeof *b, b);

        InversionList *set_c = inversion_list_create(100, sizeof c / sizeof *c, c);
        InversionList *set_d = inversion_list_create(100, sizeof d / sizeof *d, d);

        assert(inversion_list_equal(set_a_uint8, set_a_uint8));
        assert(inversion_list_equal(set_a_uint8, set_a_uint16));
        assert(inversion_list_equal(set_a_uint8, set_a_uint32));
        assert(inversion_list_equal(set_a_uint16, set_a_uint8));
        assert(inversion_list_equal(set_a_uint16, set_a_uint16));
        assert(inversion_list_equal(set_a_uint16, set_a_uint32));
        assert(inversion_list_equal(set_a_uint32, set_a_uint8));
        assert(inversion_list_equal(set_a_uint32, set_a_uint16));
        assert(inversion_list_equal(set_a_uint32, set_a_uint32));
        assert(!inversion_list_not_equal(set_a_uint8, set_a_uint8));
        assert(!inversion_list_not_equal(set_a_uint8, set_a_uint16));
        assert(!inversion_list_not_equal(set_a_uint8, set_a_uint32));
        assert(!inversion_list_not_equal(set_a_uint16, set_a_uint8));
        assert(!inversion_list_not_equal(set_a_uint16, set_a_uint16));
        assert(!inversion_list_not_equal(set_a_uint16, set_a_uint32));
        assert(!inversion_list_not_equal(set_a_uint32, set_a_uint8));
        assert(!inversion_list_not_equal(set_a_uint32, set_a_uint16));
        assert(!inversion_list_not_equal(set_a_uint32, set_a_uint32));

        assert(inversion_list_equal(set_a_bis_uint8, set_a_uint8));
        assert(inversion_list_equal(set_a_bis_uint8, set_a_uint16));
        assert(inversion_list_equal(set_a_bis_uint8, set_a_uint32));
        assert(inversion_list_equal(set_a_bis_uint16, set_a_uint8));
        assert(inversion_list_equal(set_a_bis_uint16, set_a_uint16));
        assert(inversion_list_equal(set_a_bis_uint16, set_a_uint32));
        assert(inversion_list_equal(set_a_bis_uint32, set_a_uint8));
        assert(inversion_list_equal(set_a_bis_uint32, set_a_uint16));
        assert(inversion_list_equal(set_a_bis_uint32, set_a_uint32));
        assert(!inversion_list_not_equal(set_a_bis_uint8, set_a_uint8));
        assert(!inversion_list_not_equal(set_a_bis_uint8, set_a_uint16));
        assert(!inversion_list_not_equal(set_a_bis_uint8, set_a_uint32));
        assert(!inversion_list_not_equal(set_a_bis_uint16, set_a_uint8));
        assert(!inversion_list_not_equal(set_a_bis_uint16, set_a_uint16));
        assert(!inversion_list_not_equal(set_a_bis_uint16, set_a_uint32));
        assert(!inversion_list_not_equal(set_a_bis_uint32, set_a_uint8));
        assert(!inversion_list_not_equal(set_a_bis_uint32, set_a_uint16));
        assert(!inversion_list_not_equal(set_a_bis_uint32, set_a_uint32));

        assert(!inversion_list_equal(set_a_uint8, set_b_uint8));
        assert(!inversion_list_equal(set_a_uint8, set_b_uint16));
        assert(!inversion_list_equal(set_a_uint8, set_b_uint32));
        assert(!inversion_list_equal(set_a_uint16, set_b_uint8));
        assert(!inversion_list_equal(set_a_uint16, set_b_uint16));
        assert(!inversion_list_equal(set_a_uint16, set_b_uint32));
        assert(!inversion_list_equal(set_a_uint32, set_b_uint8));
        assert(!inversion_list_equal(set_a_uint32, set_b_uint16));
        assert(!inversion_list_equal(set_a_uint32, set_b_uint32));
        assert(inversion_list_not_equal(set_a_uint8, set_b_uint8));
        assert(inversion_list_not_equal(set_a_uint8, set_b_uint16));
        assert(inversion_list_not_equal(set_a_uint8, set_b_uint32));
        assert(inversion_list_not_equal(set_a_uint16, set_b_uint8));
        assert(inversion_list_not_equal(set_a_uint16, set_b_uint16));
        assert(inversion_list_not_equal(set_a_uint16, set_b_uint32));
        assert(inversion_list_not_equal(set_a_uint32, set_b_uint8));
        assert(inversion_list_not_equal(set_a_uint32, set_b_uint16));
        assert(inversion_list_not_equal(set_a_uint32, set_b_uint32));

        assert(!inversion_list_equal(set_a_uint8, set_c));
        assert(!inversion_list_equal(set_c, set_a_uint8));
        assert(inversion_list_not_equal(set_a_uint8, set_c));
        assert(inversion_list_not_equal(set_c, set_a_uint8));

        assert(inversion_list_not_equal(set_a_uint8, set_d));
        assert(inversion_list_not_equal(set_b_uint16, set_d));
        assert(inversion_list_not_equal(set_c, set_d));

        inversion_list_destroy(set_a_uint8);
        inversion_list_destroy(set_a_uint16);
        inversion_list_destroy(set_a_uint32);
        inversion_list_destroy(set_a_bis_uint8);
        inversion_list_destroy(set_a_bis_uint16);
        inversion_list_destroy(set_a_bis_uint32);

        inversion_list_destroy(set_b_uint8);
        inversion_list_destroy(set_b_uint16);
        inversion_list_destroy(set_b_uint32);

        inversion_list_destroy(set_c);
        inversion_list_destroy(set_d);
    }
    inversion_list_finish();
    return EXIT_SUCCESS;
}
