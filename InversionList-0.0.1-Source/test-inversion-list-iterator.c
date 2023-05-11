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
        InversionListIterator *it = inversion_list_iterator_create(set);
        assert(it!=NULL);

        assert(it->value == set->couples.uint8[0]);
        assert(inversion_list_iterator_next(it)->index == 0);
        assert(inversion_list_iterator_get(it) == 2);
        assert(inversion_list_iterator_valid(it));
        assert(inversion_list_iterator_next(it)->index == 0);
        assert(inversion_list_iterator_get(it) == 3);
        assert(inversion_list_iterator_next(it)->index == 2);
        assert(inversion_list_iterator_get(it) == 5);
        inversion_list_iterator_next(it);
        inversion_list_iterator_next(it);
        assert(inversion_list_iterator_next(it)->value == 9);
        assert(inversion_list_iterator_valid(it));
        assert(inversion_list_iterator_next(it)->value == 0);
        assert(!inversion_list_iterator_valid(it));
        inversion_list_iterator_rewind(it);
        assert(it->value == 1);


        inversion_list_iterator_destroy(it);
        inversion_list_destroy(set);

    }
    inversion_list_finish();
    return EXIT_SUCCESS;
}

