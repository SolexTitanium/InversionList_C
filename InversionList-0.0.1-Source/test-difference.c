#include <stdlib.h>

#ifdef NDEBUG
#undef NDEBUG
#endif
#include <assert.h>

#include "./inversion-list.h"
#include "./inversion-list.inc"

int main(void) {
  inversion_list_init();
  {
    unsigned int a[] = {1,2,7,8,10};
    unsigned int b[] = {3,4,6,9,10};

    InversionList *set_a_uint32 = inversion_list_create(100000, sizeof a / sizeof *a, a);
    InversionList *set_a_uint16 = inversion_list_create(1000, sizeof a / sizeof *a, a);
    InversionList *set_a_uint8 = inversion_list_create(100, sizeof a / sizeof *a, a);
    InversionList *set_b_uint32 = inversion_list_create(100000, sizeof b / sizeof *b, b);
    InversionList *set_b_uint16 = inversion_list_create(1000, sizeof b / sizeof *b, b);
    InversionList *set_b_uint8 = inversion_list_create(100, sizeof b / sizeof *b, b);

    InversionList *difference_set;
    
    difference_set = inversion_list_difference(set_a_uint32, set_b_uint32);
    assert(difference_set->capacity == 100000);
    assert(difference_set->size = 2);
    assert(difference_set->support == 4);
    assert(difference_set->couples.uint32[0] == 1);
    assert(difference_set->couples.uint32[1] == 3);
    assert(difference_set->couples.uint32[2] == 7);
    assert(difference_set->couples.uint32[3] == 9);
    inversion_list_destroy(difference_set);

    inversion_list_destroy(set_a_uint32);
    inversion_list_destroy(set_a_uint16);
    inversion_list_destroy(set_a_uint8);
    inversion_list_destroy(set_b_uint32);
    inversion_list_destroy(set_b_uint16);
    inversion_list_destroy(set_b_uint8);
  }
  inversion_list_finish();
  return EXIT_SUCCESS;
}