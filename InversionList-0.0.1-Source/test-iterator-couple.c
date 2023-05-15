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
        InversionList *set = inversion_list_create(20, sizeof a / sizeof *a, a);
        InversionListCoupleIterator *it = inversion_list_couple_iterator_create(set);
        assert(it!=NULL);

        assert(inversion_list_couple_iterator_next(it)->index == 0);
        assert(inversion_list_couple_iterator_get_inf(it) ==1);
        assert(inversion_list_couple_iterator_get_sup(it) == 4);
        assert(inversion_list_iterator_couple_valid(it));
        assert(inversion_list_couple_iterator_next(it)->index == 0);
        assert(inversion_list_couple_iterator_get_inf(it) == 5);
        assert(inversion_list_couple_iterator_get_sup(it) == 10);
        assert(inversion_list_couple_iterator_next(it)->index == 6);
        inversion_list_couple_iterator_next(it);
        assert(inversion_list_iterator_couple_valid(it)==false);
        inversion_list_couple_iterator_next(it);
        inversion_list_couple_iterator_rewind(it);
        assert(inversion_list_couple_iterator_get_inf(it) ==1);

       
        inversion_list_couple_iterator_destroy(it);
        inversion_list_destroy(set);

    }
    inversion_list_finish();
    return EXIT_SUCCESS;

}