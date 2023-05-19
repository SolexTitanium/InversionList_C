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
  
    difference_set = inversion_list_difference(set_a_uint32, set_b_uint32, NULL);
    assert(difference_set->capacity == 100000);
    assert(difference_set->size = 2);
    assert(difference_set->support == 4);
    assert(difference_set->couples.uint32[0] == 1);
    assert(difference_set->couples.uint32[1] == 3);
    assert(difference_set->couples.uint32[2] == 7);
    assert(difference_set->couples.uint32[3] == 9);
    inversion_list_destroy(difference_set);

    difference_set = inversion_list_difference(set_a_uint32, set_b_uint16, NULL);
    assert(difference_set->capacity == 100000);
    assert(difference_set->size = 2);
    assert(difference_set->support == 4);
    assert(difference_set->couples.uint32[0] == 1);
    assert(difference_set->couples.uint32[1] == 3);
    assert(difference_set->couples.uint32[2] == 7);
    assert(difference_set->couples.uint32[3] == 9);
    inversion_list_destroy(difference_set);

    difference_set = inversion_list_difference(set_a_uint32, set_b_uint8, NULL);
    assert(difference_set->capacity == 100000);
    assert(difference_set->size = 2);
    assert(difference_set->support == 4);
    assert(difference_set->couples.uint32[0] == 1);
    assert(difference_set->couples.uint32[1] == 3);
    assert(difference_set->couples.uint32[2] == 7);
    assert(difference_set->couples.uint32[3] == 9);
    inversion_list_destroy(difference_set);

    difference_set = inversion_list_difference(set_a_uint16, set_b_uint32, NULL);
    assert(difference_set->capacity == 100000);
    assert(difference_set->size = 2);
    assert(difference_set->support == 4);
    assert(difference_set->couples.uint32[0] == 1);
    assert(difference_set->couples.uint32[1] == 3);
    assert(difference_set->couples.uint32[2] == 7);
    assert(difference_set->couples.uint32[3] == 9);
    inversion_list_destroy(difference_set);

    difference_set = inversion_list_difference(set_a_uint16, set_b_uint16, NULL);
    assert(difference_set->capacity == 1000);
    assert(difference_set->size = 2);
    assert(difference_set->support == 4);
    assert(difference_set->couples.uint16[0] == 1);
    assert(difference_set->couples.uint16[1] == 3);
    assert(difference_set->couples.uint16[2] == 7);
    assert(difference_set->couples.uint16[3] == 9);
    inversion_list_destroy(difference_set);

    difference_set = inversion_list_difference(set_a_uint16, set_b_uint8, NULL);
    assert(difference_set->capacity == 1000);
    assert(difference_set->size = 2);
    assert(difference_set->support == 4);
    assert(difference_set->couples.uint16[0] == 1);
    assert(difference_set->couples.uint16[1] == 3);
    assert(difference_set->couples.uint16[2] == 7);
    assert(difference_set->couples.uint16[3] == 9);
    inversion_list_destroy(difference_set);

    difference_set = inversion_list_difference(set_a_uint8, set_b_uint32, NULL);
    assert(difference_set->capacity == 100000);
    assert(difference_set->size = 2);
    assert(difference_set->support == 4);
    assert(difference_set->couples.uint32[0] == 1);
    assert(difference_set->couples.uint32[1] == 3);
    assert(difference_set->couples.uint32[2] == 7);
    assert(difference_set->couples.uint32[3] == 9);
    inversion_list_destroy(difference_set);

    difference_set = inversion_list_difference(set_a_uint8, set_b_uint16, NULL);
    assert(difference_set->capacity == 1000);
    assert(difference_set->size = 2);
    assert(difference_set->support == 4);
    assert(difference_set->couples.uint16[0] == 1);
    assert(difference_set->couples.uint16[1] == 3);
    assert(difference_set->couples.uint16[2] == 7);
    assert(difference_set->couples.uint16[3] == 9);
    inversion_list_destroy(difference_set);

    difference_set = inversion_list_difference(set_a_uint8, set_b_uint8, NULL);
    assert(difference_set->capacity == 100);
    assert(difference_set->size = 2);
    assert(difference_set->support == 4);
    assert(difference_set->couples.uint8[0] == 1);
    assert(difference_set->couples.uint8[1] == 3);
    assert(difference_set->couples.uint8[2] == 7);
    assert(difference_set->couples.uint8[3] == 9);
    inversion_list_destroy(difference_set);

    inversion_list_destroy(set_a_uint32);
    inversion_list_destroy(set_a_uint16);
    inversion_list_destroy(set_a_uint8);
    inversion_list_destroy(set_b_uint32);
    inversion_list_destroy(set_b_uint16);
    inversion_list_destroy(set_b_uint8);
  }
  {
    unsigned int a[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13};
    unsigned int b[] = {4, 5, 6, 7, 8, 20, 21, 22, 23, 24};
    unsigned int c[] = {8, 9, 10, 11, 13, 14, 15, 23, 24};
    unsigned int d[] = {5};
    unsigned int e[] = {21, 22};

    InversionList *set_a_uint8 = inversion_list_create(100, sizeof a / sizeof *a, a);
    InversionList *set_a_uint16 = inversion_list_create(1000, sizeof a / sizeof *a, a);
    InversionList *set_a_uint32 = inversion_list_create(100000, sizeof a / sizeof *a, a);
    InversionList *set_b_uint8 = inversion_list_create(100, sizeof b / sizeof *b, b);
    InversionList *set_b_uint16 = inversion_list_create(1000, sizeof b / sizeof *b, b);
    InversionList *set_b_uint32 = inversion_list_create(100000, sizeof b / sizeof *b, b);
    InversionList *set_c_uint8 = inversion_list_create(100, sizeof c / sizeof *c, c);
    InversionList *set_c_uint16 = inversion_list_create(1000, sizeof c / sizeof *c, c);
    InversionList *set_c_uint32 = inversion_list_create(100000, sizeof c / sizeof *c, c);
    InversionList *set_d = inversion_list_create(100, sizeof d / sizeof *d, d);
    InversionList *set_e = inversion_list_create(100, sizeof e / sizeof *e, e);

    InversionList *difference_set;

    difference_set = inversion_list_difference(set_a_uint32, NULL);
    assert(difference_set->capacity == 100000);
    assert(difference_set->size = 2);
    assert(difference_set->support == 13);
    assert(difference_set->couples.uint32[0] == 1);
    assert(difference_set->couples.uint32[1] == 14);
    inversion_list_destroy(difference_set);

    difference_set = inversion_list_difference(set_a_uint16, set_a_uint8, set_b_uint16, NULL);
    assert(difference_set->capacity == 1000);
    assert(difference_set->size == 0);
    assert(difference_set->support == 0);
    inversion_list_destroy(difference_set);

    difference_set = inversion_list_difference(set_a_uint8, set_b_uint32, set_c_uint16, NULL);
    assert(difference_set->capacity == 100000);
    assert(difference_set->size == 4);
    assert(difference_set->support == 4);
    assert(difference_set->couples.uint32[0] == 1);
    assert(difference_set->couples.uint32[1] == 4);
    assert(difference_set->couples.uint32[2] == 12);
    assert(difference_set->couples.uint32[3] == 13);
    inversion_list_destroy(difference_set);

    difference_set = inversion_list_difference(set_a_uint8, set_c_uint32, set_b_uint16, NULL);
    assert(difference_set->capacity == 100000);
    assert(difference_set->size == 4);
    assert(difference_set->support == 4);
    assert(difference_set->couples.uint32[0] == 1);
    assert(difference_set->couples.uint32[1] == 4);
    assert(difference_set->couples.uint32[2] == 12);
    assert(difference_set->couples.uint32[3] == 13);
    inversion_list_destroy(difference_set);

    difference_set = inversion_list_difference(set_b_uint16, set_c_uint16, NULL);
    assert(difference_set->capacity == 1000);
    assert(difference_set->size == 4);
    assert(difference_set->support == 7);
    assert(difference_set->couples.uint16[0] == 4);
    assert(difference_set->couples.uint16[1] == 8);
    assert(difference_set->couples.uint16[2] == 20);
    assert(difference_set->couples.uint16[3] == 23);
    inversion_list_destroy(difference_set);

    difference_set = inversion_list_difference(set_b_uint8, set_c_uint8, set_d, set_e, NULL);
    assert(difference_set->capacity == 100);
    assert(difference_set->size == 6);
    assert(difference_set->support == 4);
    assert(difference_set->couples.uint8[0] == 4);
    assert(difference_set->couples.uint8[1] == 5);
    assert(difference_set->couples.uint8[2] == 6);
    assert(difference_set->couples.uint8[3] == 8);
    assert(difference_set->couples.uint8[4] == 20);
    assert(difference_set->couples.uint8[5] == 21);
    inversion_list_destroy(difference_set);

    inversion_list_destroy(set_a_uint8);
    inversion_list_destroy(set_a_uint16);
    inversion_list_destroy(set_a_uint32);
    inversion_list_destroy(set_b_uint8);
    inversion_list_destroy(set_b_uint16);
    inversion_list_destroy(set_b_uint32);
    inversion_list_destroy(set_c_uint8);
    inversion_list_destroy(set_c_uint16);
    inversion_list_destroy(set_c_uint32);
    inversion_list_destroy(set_d);
    inversion_list_destroy(set_e);
  }
  inversion_list_finish();
  return EXIT_SUCCESS;
}