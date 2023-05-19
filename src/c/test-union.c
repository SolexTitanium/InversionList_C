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
    unsigned int a[] = {1, 2, 3, 7, 8, 9, 21, 22, 23, 24, 25, 26, 27, 28, 29};
    unsigned int b[] = {5, 6, 7, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19};

    InversionList *set_a_uint32 = inversion_list_create(100000, sizeof a / sizeof *a, a);
    InversionList *set_a_uint16 = inversion_list_create(1000, sizeof a / sizeof *a, a);
    InversionList *set_a_uint8 = inversion_list_create(100, sizeof a / sizeof *a, a);
    InversionList *set_b_uint32 = inversion_list_create(100000, sizeof b / sizeof *b, b);
    InversionList *set_b_uint16 = inversion_list_create(1000, sizeof b / sizeof *b, b);
    InversionList *set_b_uint8 = inversion_list_create(100, sizeof b / sizeof *b, b);

    InversionList *union_set;
    
   union_set = inversion_list_union(set_a_uint32, set_b_uint32, NULL);
    assert(union_set->capacity == 100000);
    assert(union_set->size = 6);
    assert(union_set->support == 27);
    assert(union_set->couples.uint32[0] == 1);
    assert(union_set->couples.uint32[1] == 4);
    assert(union_set->couples.uint32[2] == 5);
    assert(union_set->couples.uint32[3] == 20);
    assert(union_set->couples.uint32[4] == 21);
    assert(union_set->couples.uint32[5] == 30);
    inversion_list_destroy(union_set);
    
    union_set = inversion_list_union(set_a_uint32, set_b_uint16, NULL);
    assert(union_set->capacity == 100000);
    assert(union_set->size = 6);
    assert(union_set->support == 27);
    assert(union_set->couples.uint32[0] == 1);
    assert(union_set->couples.uint32[1] == 4);
    assert(union_set->couples.uint32[2] == 5);
    assert(union_set->couples.uint32[3] == 20);
    assert(union_set->couples.uint32[4] == 21);
    assert(union_set->couples.uint32[5] == 30);
    inversion_list_destroy(union_set);
    
    union_set = inversion_list_union(set_a_uint32, set_b_uint8, NULL);
    assert(union_set->capacity == 100000);
    assert(union_set->size = 6);
    assert(union_set->support == 27);
    assert(union_set->couples.uint32[0] == 1);
    assert(union_set->couples.uint32[1] == 4);
    assert(union_set->couples.uint32[2] == 5);
    assert(union_set->couples.uint32[3] == 20);
    assert(union_set->couples.uint32[4] == 21);
    assert(union_set->couples.uint32[5] == 30);
    inversion_list_destroy(union_set);
    
    union_set = inversion_list_union(set_a_uint16, set_b_uint32, NULL);
    assert(union_set->capacity == 100000);
    assert(union_set->size = 6);
    assert(union_set->support == 27);
    assert(union_set->couples.uint32[0] == 1);
    assert(union_set->couples.uint32[1] == 4);
    assert(union_set->couples.uint32[2] == 5);
    assert(union_set->couples.uint32[3] == 20);
    assert(union_set->couples.uint32[4] == 21);
    assert(union_set->couples.uint32[5] == 30);
    inversion_list_destroy(union_set);
    
    union_set = inversion_list_union(set_a_uint16, set_b_uint16, NULL);
    assert(union_set->capacity == 1000);
    assert(union_set->size = 6);
    assert(union_set->support == 27);
    assert(union_set->couples.uint16[0] == 1);
    assert(union_set->couples.uint16[1] == 4);
    assert(union_set->couples.uint16[2] == 5);
    assert(union_set->couples.uint16[3] == 20);
    assert(union_set->couples.uint16[4] == 21);
    assert(union_set->couples.uint16[5] == 30);
    inversion_list_destroy(union_set);
    
    union_set = inversion_list_union(set_a_uint16, set_b_uint8, NULL);
    assert(union_set->capacity == 1000);
    assert(union_set->size = 6);
    assert(union_set->support == 27);
    assert(union_set->couples.uint16[0] == 1);
    assert(union_set->couples.uint16[1] == 4);
    assert(union_set->couples.uint16[2] == 5);
    assert(union_set->couples.uint16[3] == 20);
    assert(union_set->couples.uint16[4] == 21);
    assert(union_set->couples.uint16[5] == 30);
    inversion_list_destroy(union_set);
    
    union_set = inversion_list_union(set_a_uint8, set_b_uint32, NULL);
    assert(union_set->capacity == 100000);
    assert(union_set->size = 6);
    assert(union_set->support == 27);
    assert(union_set->couples.uint32[0] == 1);
    assert(union_set->couples.uint32[1] == 4);
    assert(union_set->couples.uint32[2] == 5);
    assert(union_set->couples.uint32[3] == 20);
    assert(union_set->couples.uint32[4] == 21);
    assert(union_set->couples.uint32[5] == 30);
    inversion_list_destroy(union_set);
    
    union_set = inversion_list_union(set_a_uint8, set_b_uint16, NULL);
    assert(union_set->capacity == 1000);
    assert(union_set->size = 6);
    assert(union_set->support == 27);
    assert(union_set->couples.uint16[0] == 1);
    assert(union_set->couples.uint16[1] == 4);
    assert(union_set->couples.uint16[2] == 5);
    assert(union_set->couples.uint16[3] == 20);
    assert(union_set->couples.uint16[4] == 21);
    assert(union_set->couples.uint16[5] == 30);
    inversion_list_destroy(union_set);
    
    union_set = inversion_list_union(set_a_uint8, set_b_uint8, NULL);
    assert(union_set->capacity == 100);
    assert(union_set->size = 6);
    assert(union_set->support == 27);
    assert(union_set->couples.uint8[0] == 1);
    assert(union_set->couples.uint8[1] == 4);
    assert(union_set->couples.uint8[2] == 5);
    assert(union_set->couples.uint8[3] == 20);
    assert(union_set->couples.uint8[4] == 21);
    assert(union_set->couples.uint8[5] == 30);
    inversion_list_destroy(union_set);

    inversion_list_destroy(set_a_uint32);
    inversion_list_destroy(set_a_uint16);
    inversion_list_destroy(set_a_uint8);
    inversion_list_destroy(set_b_uint32);
    inversion_list_destroy(set_b_uint16);
    inversion_list_destroy(set_b_uint8);
  }
  {
    unsigned a[] = {};
    unsigned b[] = {1};
    unsigned c[] = {2, 3, 4};
    unsigned d[] = {1, 2, 3, 7, 8, 20, 21, 22, 23};
    unsigned e[] = {4, 5, 6, 12, 13};

    InversionList *set_a = inversion_list_create(100, sizeof a / sizeof *a, a);
    InversionList *set_b = inversion_list_create(100, sizeof b / sizeof *b, b);
    InversionList *set_c = inversion_list_create(100, sizeof c / sizeof *c, c);
    InversionList *set_d = inversion_list_create(100, sizeof d / sizeof *d, d);
    InversionList *set_e = inversion_list_create(100, sizeof e / sizeof *e, e);

    InversionList *union_set;

    union_set = inversion_list_union(set_a, set_a, NULL);
    assert(union_set->capacity == 100);
    assert(union_set->size == 0);
    assert(union_set->support == 0);
    inversion_list_destroy(union_set);

    union_set = inversion_list_union(set_a, set_b, NULL);
    assert(union_set->capacity == 100);
    assert(union_set->size == 2);
    assert(union_set->support == 1);
    assert(union_set->couples.uint8[0] == 1);
    assert(union_set->couples.uint8[1] == 2);
    inversion_list_destroy(union_set);

    union_set = inversion_list_union(set_a, set_c, NULL);
    assert(union_set->capacity == 100);
    assert(union_set->size == 2);
    assert(union_set->support == 3);
    assert(union_set->couples.uint8[0] == 2);
    assert(union_set->couples.uint8[1] == 5);
    inversion_list_destroy(union_set);

    union_set = inversion_list_union(set_b, set_d, NULL);
    assert(union_set->capacity == 100);
    assert(union_set->size == 6);
    assert(union_set->support == 9);
    assert(union_set->couples.uint8[0] == 1);
    assert(union_set->couples.uint8[1] == 4);
    assert(union_set->couples.uint8[2] == 7);
    assert(union_set->couples.uint8[3] == 9);
    assert(union_set->couples.uint8[4] == 20);
    assert(union_set->couples.uint8[5] == 24);
    inversion_list_destroy(union_set);

    union_set = inversion_list_union(set_d, set_e, NULL);
    assert(union_set->capacity == 100);
    assert(union_set->size == 6);
    assert(union_set->support == 14);
    assert(union_set->couples.uint8[0] == 1);
    assert(union_set->couples.uint8[1] == 9);
    assert(union_set->couples.uint8[2] == 12);
    assert(union_set->couples.uint8[3] == 14);
    assert(union_set->couples.uint8[4] == 20);
    assert(union_set->couples.uint8[5] == 24);
    inversion_list_destroy(union_set);

    inversion_list_destroy(set_a);
    inversion_list_destroy(set_b);
    inversion_list_destroy(set_c);
    inversion_list_destroy(set_d);
    inversion_list_destroy(set_e);
  }
  {
    unsigned int a[] = {2, 3, 4, 10, 12};

    InversionList *set_a = inversion_list_create(20, sizeof a / sizeof *a, a);
    InversionList *complement_a = inversion_list_complement(set_a);

    InversionList *union_set;

    union_set = inversion_list_union(set_a, complement_a, NULL);
    assert(union_set->capacity == 20);
    assert(union_set->size == 2);
    assert(union_set->support == 20);
    assert(union_set->couples.uint8[0] == 0);
    assert(union_set->couples.uint8[1] == 20);
    inversion_list_destroy(union_set);

    inversion_list_destroy(set_a);
    inversion_list_destroy(complement_a);
  }
  {
    unsigned int a[] = {5, 6, 7, 8, 9, 10, 11, 12, 20, 21, 22};
    unsigned int b[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    unsigned int c[] = {15, 16, 7, 8, 23, 24};
    unsigned int d[] = {100, 101, 102, 103, 104, 150, 151, 152, 153, 154, 155};

    InversionList *set_a_uint8 = inversion_list_create(200, sizeof a / sizeof *a, a);
    InversionList *set_a_uint16 = inversion_list_create(1000, sizeof a / sizeof *a, a);
    InversionList *set_a_uint32 = inversion_list_create(100000, sizeof a / sizeof *a, a);
    InversionList *set_b_uint8 = inversion_list_create(200, sizeof b / sizeof *b, b);
    InversionList *set_b_uint16 = inversion_list_create(1000, sizeof b / sizeof *b, b);
    InversionList *set_b_uint32 = inversion_list_create(100000, sizeof b / sizeof *b, b);
    InversionList *set_c_uint8 = inversion_list_create(200, sizeof c / sizeof *c, c);
    InversionList *set_c_uint16 = inversion_list_create(1000, sizeof c / sizeof *c, c);
    InversionList *set_c_uint32 = inversion_list_create(100000, sizeof c / sizeof *c, c);
    InversionList *set_d_uint8 = inversion_list_create(200, sizeof d / sizeof *d, d);
    InversionList *set_d_uint16 = inversion_list_create(1000, sizeof d / sizeof *d, d);
    InversionList *set_d_uint32 = inversion_list_create(100000, sizeof d / sizeof *d, d);

    InversionList *union_set;

    union_set = inversion_list_union(set_a_uint8, set_b_uint8, set_c_uint8, NULL);
    assert(union_set->capacity == 200);
    assert(union_set->size == 6);
    assert(union_set->support == 19);
    assert(union_set->couples.uint8[0] == 1);
    assert(union_set->couples.uint8[1] == 13);
    assert(union_set->couples.uint8[2] == 15);
    assert(union_set->couples.uint8[3] == 17);
    assert(union_set->couples.uint8[4] == 20);
    assert(union_set->couples.uint8[5] == 25);
    inversion_list_destroy(union_set);

    union_set = inversion_list_union(set_a_uint16, set_b_uint8, set_c_uint16, NULL);
    assert(union_set->capacity == 1000);
    assert(union_set->size == 6);
    assert(union_set->support == 19);
    assert(union_set->couples.uint16[0] == 1);
    assert(union_set->couples.uint16[1] == 13);
    assert(union_set->couples.uint16[2] == 15);
    assert(union_set->couples.uint16[3] == 17);
    assert(union_set->couples.uint16[4] == 20);
    assert(union_set->couples.uint16[5] == 25);
    inversion_list_destroy(union_set);

    union_set = inversion_list_union(set_b_uint32, set_a_uint16, set_c_uint8, NULL);
    assert(union_set->capacity == 100000);
    assert(union_set->size == 6);
    assert(union_set->support == 19);
    assert(union_set->couples.uint32[0] == 1);
    assert(union_set->couples.uint32[1] == 13);
    assert(union_set->couples.uint32[2] == 15);
    assert(union_set->couples.uint32[3] == 17);
    assert(union_set->couples.uint32[4] == 20);
    assert(union_set->couples.uint32[5] == 25);
    inversion_list_destroy(union_set);

    union_set = inversion_list_union(set_a_uint8, set_b_uint8, set_c_uint8, set_d_uint8, NULL);
    assert(union_set->capacity == 200);
    assert(union_set->size == 10);
    assert(union_set->support == 30);
    assert(union_set->couples.uint8[0] == 1);
    assert(union_set->couples.uint8[1] == 13);
    assert(union_set->couples.uint8[2] == 15);
    assert(union_set->couples.uint8[3] == 17);
    assert(union_set->couples.uint8[4] == 20);
    assert(union_set->couples.uint8[5] == 25);
    assert(union_set->couples.uint8[6] == 100);
    assert(union_set->couples.uint8[7] == 105);
    assert(union_set->couples.uint8[8] == 150);
    assert(union_set->couples.uint8[9] == 156);
    inversion_list_destroy(union_set);

    union_set = inversion_list_union(set_a_uint32, set_b_uint32, set_c_uint32, set_d_uint32, NULL);
    assert(union_set->capacity == 100000);
    assert(union_set->size == 10);
    assert(union_set->support == 30);
    assert(union_set->couples.uint32[0] == 1);
    assert(union_set->couples.uint32[1] == 13);
    assert(union_set->couples.uint32[2] == 15);
    assert(union_set->couples.uint32[3] == 17);
    assert(union_set->couples.uint32[4] == 20);
    assert(union_set->couples.uint32[5] == 25);
    assert(union_set->couples.uint32[6] == 100);
    assert(union_set->couples.uint32[7] == 105);
    assert(union_set->couples.uint32[8] == 150);
    assert(union_set->couples.uint32[9] == 156);
    inversion_list_destroy(union_set);

    union_set = inversion_list_union(set_c_uint8, set_a_uint16, set_b_uint16, set_c_uint16, set_d_uint16, set_b_uint8, NULL);
    assert(union_set->capacity == 1000);
    assert(union_set->size == 10);
    assert(union_set->support == 30);
    assert(union_set->couples.uint16[0] == 1);
    assert(union_set->couples.uint16[1] == 13);
    assert(union_set->couples.uint16[2] == 15);
    assert(union_set->couples.uint16[3] == 17);
    assert(union_set->couples.uint16[4] == 20);
    assert(union_set->couples.uint16[5] == 25);
    assert(union_set->couples.uint16[6] == 100);
    assert(union_set->couples.uint16[7] == 105);
    assert(union_set->couples.uint16[8] == 150);
    assert(union_set->couples.uint16[9] == 156);
    inversion_list_destroy(union_set);

    union_set = inversion_list_union(set_c_uint8, set_a_uint16, set_b_uint32, set_c_uint16, set_d_uint16, set_b_uint8, NULL);
    assert(union_set->capacity == 100000);
    assert(union_set->size == 10);
    assert(union_set->support == 30);
    assert(union_set->couples.uint32[0] == 1);
    assert(union_set->couples.uint32[1] == 13);
    assert(union_set->couples.uint32[2] == 15);
    assert(union_set->couples.uint32[3] == 17);
    assert(union_set->couples.uint32[4] == 20);
    assert(union_set->couples.uint32[5] == 25);
    assert(union_set->couples.uint32[6] == 100);
    assert(union_set->couples.uint32[7] == 105);
    assert(union_set->couples.uint32[8] == 150);
    assert(union_set->couples.uint32[9] == 156);
    inversion_list_destroy(union_set);

    union_set = inversion_list_union(NULL);
    assert(union_set->capacity == 0);
    assert(union_set->size == 0);
    assert(union_set->support == 0);
    inversion_list_destroy(union_set);

    union_set = inversion_list_union(set_a_uint8, NULL);
    assert(union_set->capacity == 200);
    assert(union_set->size == 4);
    assert(union_set->support == 11);
    assert(union_set->couples.uint8[0] == 5);
    assert(union_set->couples.uint8[1] == 13);
    assert(union_set->couples.uint8[2] == 20);
    assert(union_set->couples.uint8[3] == 23);
    inversion_list_destroy(union_set);

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
  }
  inversion_list_finish();
  return EXIT_SUCCESS;
}
