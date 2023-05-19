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
    unsigned int a[] = {1, 2, 3};
    unsigned int b[] = {1};
    unsigned int c[] = {1, 2, 3, 7, 8, 9, 10, 12, 13, 14};
    unsigned int d[] = {2, 3, 4, 5, 8, 9, 10, 11};
    unsigned int e[] = {};

    InversionList *set_a_uint8 = inversion_list_create(100, sizeof a / sizeof *a, a);
    InversionList *set_a_uint16 = inversion_list_create(1000, sizeof a / sizeof *a, a);
    InversionList *set_a_uint32 = inversion_list_create(100000, sizeof a / sizeof *a, a);
    InversionList *set_b_uint8 = inversion_list_create(100, sizeof b / sizeof *b, b);
    InversionList *set_b_uint16 = inversion_list_create(1000, sizeof b / sizeof *b, b);
    InversionList *set_b_uint32 = inversion_list_create(100000, sizeof b / sizeof *b, b);
    InversionList *set_c_uint8 = inversion_list_create(100, sizeof c / sizeof *c, c);
    InversionList *set_c_uint16 = inversion_list_create(1000, sizeof c / sizeof *c, c);
    InversionList *set_c_uint32 = inversion_list_create(100000, sizeof c / sizeof *c, c);
    InversionList *set_d_uint8 = inversion_list_create(100, sizeof d / sizeof *d, d);
    InversionList *set_d_uint16 = inversion_list_create(1000, sizeof d / sizeof *d, d);
    InversionList *set_d_uint32 = inversion_list_create(100000, sizeof d / sizeof *d, d);
    InversionList *set_e_uint8 = inversion_list_create(100, sizeof e / sizeof *e, e);
    InversionList *set_e_uint16 = inversion_list_create(1000, sizeof e / sizeof *e, e);
    InversionList *set_e_uint32 = inversion_list_create(100000, sizeof e / sizeof *e, e);

    InversionList *intersection_set;

    intersection_set = inversion_list_intersection(set_a_uint8, set_a_uint8, NULL);
    assert(intersection_set->capacity == 100);
    assert(intersection_set->support == 3);
    assert(intersection_set->size == 2);
    assert(intersection_set->couples.uint8[0] == 1);
    assert(intersection_set->couples.uint8[1] == 4);
    inversion_list_destroy(intersection_set);

    intersection_set = inversion_list_intersection(set_a_uint16, set_a_uint8, NULL);
    assert(intersection_set->capacity == 1000);
    assert(intersection_set->support == 3);
    assert(intersection_set->size == 2);
    assert(intersection_set->couples.uint16[0] == 1);
    assert(intersection_set->couples.uint16[1] == 4);
    inversion_list_destroy(intersection_set);

    intersection_set = inversion_list_intersection(set_a_uint16, set_a_uint32, NULL);
    assert(intersection_set->capacity == 100000);
    assert(intersection_set->support == 3);
    assert(intersection_set->size == 2);
    assert(intersection_set->couples.uint32[0] == 1);
    assert(intersection_set->couples.uint32[1] == 4);
    inversion_list_destroy(intersection_set);

    intersection_set = inversion_list_intersection(set_a_uint8, set_b_uint8, NULL);
    assert(intersection_set->capacity == 100);
    assert(intersection_set->support == 1);
    assert(intersection_set->size == 2);
    assert(intersection_set->couples.uint8[0] == 1);
    assert(intersection_set->couples.uint8[1] == 2);
    inversion_list_destroy(intersection_set);

    intersection_set = inversion_list_intersection(set_a_uint16, set_b_uint32, NULL);
    assert(intersection_set->capacity == 100000);
    assert(intersection_set->support == 1);
    assert(intersection_set->size == 2);
    assert(intersection_set->couples.uint32[0] == 1);
    assert(intersection_set->couples.uint32[1] == 2);
    inversion_list_destroy(intersection_set);

    intersection_set = inversion_list_intersection(set_c_uint8, set_d_uint8, NULL);
    assert(intersection_set->capacity == 100);
    assert(intersection_set->support == 5);
    assert(intersection_set->size == 4);
    assert(intersection_set->couples.uint8[0] == 2);
    assert(intersection_set->couples.uint8[1] == 4);
    assert(intersection_set->couples.uint8[2] == 8);
    assert(intersection_set->couples.uint8[3] == 11);
    inversion_list_destroy(intersection_set);

    intersection_set = inversion_list_intersection(set_c_uint32, set_d_uint8, NULL);
    assert(intersection_set->capacity == 100000);
    assert(intersection_set->support == 5);
    assert(intersection_set->size == 4);
    assert(intersection_set->couples.uint32[0] == 2);
    assert(intersection_set->couples.uint32[1] == 4);
    assert(intersection_set->couples.uint32[2] == 8);
    assert(intersection_set->couples.uint32[3] == 11);
    inversion_list_destroy(intersection_set);

    intersection_set = inversion_list_intersection(set_e_uint8, set_e_uint8, NULL);
    assert(intersection_set->capacity == 100);
    assert(intersection_set->support == 0);
    assert(intersection_set->size == 0);
    inversion_list_destroy(intersection_set);

    intersection_set = inversion_list_intersection(set_e_uint8, set_e_uint16, NULL);
    assert(intersection_set->capacity == 1000);
    assert(intersection_set->support == 0);
    assert(intersection_set->size == 0);
    inversion_list_destroy(intersection_set);

    intersection_set = inversion_list_intersection(set_e_uint8, set_d_uint8, NULL);
    assert(intersection_set->capacity == 100);
    assert(intersection_set->support == 0);
    assert(intersection_set->size == 0);
    inversion_list_destroy(intersection_set);

    intersection_set = inversion_list_intersection(set_e_uint32, set_d_uint16, NULL);
    assert(intersection_set->capacity == 100000);
    assert(intersection_set->support == 0);
    assert(intersection_set->size == 0);
    inversion_list_destroy(intersection_set);

    inversion_list_destroy(set_a_uint8);
    inversion_list_destroy(set_a_uint16);
    inversion_list_destroy(set_a_uint32);
    inversion_list_destroy(set_b_uint8);
    inversion_list_destroy(set_b_uint16);
    inversion_list_destroy(set_b_uint32);
    inversion_list_destroy(set_c_uint8);
    inversion_list_destroy(set_c_uint16);
    inversion_list_destroy(set_c_uint32);
    inversion_list_destroy(set_d_uint8);
    inversion_list_destroy(set_d_uint16);
    inversion_list_destroy(set_d_uint32);
    inversion_list_destroy(set_e_uint8);
    inversion_list_destroy(set_e_uint16);
    inversion_list_destroy(set_e_uint32);
  }
  {
    unsigned int a[] = {6, 7, 8, 9, 20, 21, 22, 23};
    unsigned int b[] = {1, 2, 3, 16, 17, 18, 19, 20, 21};
    unsigned int c[] = {8, 9, 10, 11, 12, 13, 19, 20, 23, 24, 25};

    InversionList *set_a_uint8 = inversion_list_create(100, sizeof a / sizeof *a, a);
    InversionList *set_a_uint16 = inversion_list_create(1000, sizeof a / sizeof *a, a);
    InversionList *set_a_uint32 = inversion_list_create(100000, sizeof a / sizeof *a, a);
    InversionList *set_b_uint8 = inversion_list_create(100, sizeof b / sizeof *b, b);
    InversionList *set_b_uint16 = inversion_list_create(1000, sizeof b / sizeof *b, b);
    InversionList *set_b_uint32 = inversion_list_create(100000, sizeof b / sizeof *b, b);
    InversionList *set_c_uint8 = inversion_list_create(100, sizeof c / sizeof *c, c);
    InversionList *set_c_uint16 = inversion_list_create(1000, sizeof c / sizeof *c, c);
    InversionList *set_c_uint32 = inversion_list_create(100000, sizeof c / sizeof *c, c);

    InversionList *intersection_set;
    
    intersection_set = inversion_list_intersection(NULL);
    assert(intersection_set == NULL);
    inversion_list_destroy(intersection_set);

    // TODO what implementation to use when only 1 argument?

    intersection_set = inversion_list_intersection(set_a_uint8, NULL);
    assert(intersection_set->capacity == 100);
    assert(intersection_set->support == 8);
    assert(intersection_set->size == 4);
    assert(intersection_set->couples.uint8[0] == 6);
    assert(intersection_set->couples.uint8[1] == 10);
    assert(intersection_set->couples.uint8[2] == 20);
    assert(intersection_set->couples.uint8[3] == 24);
    inversion_list_destroy(intersection_set);

    intersection_set = inversion_list_intersection(set_b_uint8, set_a_uint16, NULL);
    assert(intersection_set->capacity == 1000);
    assert(intersection_set->support == 2);
    assert(intersection_set->size == 2);
    assert(intersection_set->couples.uint16[0] == 20);
    assert(intersection_set->couples.uint16[1] == 22);
    inversion_list_destroy(intersection_set);

    intersection_set = inversion_list_intersection(set_a_uint32, set_c_uint8, NULL);
    assert(intersection_set->capacity == 100000);
    assert(intersection_set->support == 4);
    assert(intersection_set->size == 6);
    assert(intersection_set->couples.uint32[0] == 8);
    assert(intersection_set->couples.uint32[1] == 10);
    assert(intersection_set->couples.uint32[2] == 20);
    assert(intersection_set->couples.uint32[3] == 21);
    assert(intersection_set->couples.uint32[4] == 23);
    assert(intersection_set->couples.uint32[5] == 24);
    inversion_list_destroy(intersection_set);

    // TODO change implementation to works with more than 2 arguments

    intersection_set = inversion_list_intersection(set_a_uint16, set_c_uint8, set_b_uint8, NULL);
    assert(intersection_set->capacity == 1000);
    assert(intersection_set->support == 1);
    assert(intersection_set->size == 2);
    assert(intersection_set->couples.uint16[0] == 20);
    assert(intersection_set->couples.uint16[1] == 21);
    inversion_list_destroy(intersection_set);

    inversion_list_destroy(set_a_uint8);
    inversion_list_destroy(set_a_uint16);
    inversion_list_destroy(set_a_uint32);
    inversion_list_destroy(set_b_uint8);
    inversion_list_destroy(set_b_uint16);
    inversion_list_destroy(set_b_uint32);
    inversion_list_destroy(set_c_uint8);
    inversion_list_destroy(set_c_uint16);
    inversion_list_destroy(set_c_uint32);
  }
  {
    unsigned int a[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    unsigned int b[] = {4, 5, 6, 7};

    InversionList *set_a = inversion_list_create(100, sizeof a / sizeof *a, a);
    InversionList *set_b = inversion_list_create(100, sizeof b / sizeof *b, b);

    InversionList *intersection_set;

    intersection_set = inversion_list_intersection(set_a, set_b, NULL);
    assert(intersection_set->capacity == 100);
    assert(intersection_set->support == 4);
    assert(intersection_set->size == 2);
    assert(intersection_set->couples.uint8[0] == 4);
    assert(intersection_set->couples.uint8[1] == 8);
    inversion_list_destroy(intersection_set);

    inversion_list_destroy(set_a);
    inversion_list_destroy(set_b);
  }
  inversion_list_finish();
  return EXIT_SUCCESS;
}
