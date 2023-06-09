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
    unsigned int a[] = {1, 2, 3, 5, 7, 8, 9, 0, 2};
    InversionList *set = inversion_list_create(20, sizeof a / sizeof *a, a);
    InversionList *complement = inversion_list_complement(set);
    inversion_list_destroy(set);
    assert(complement->capacity == 20);
    assert(complement->support == 12);
    assert(complement->size == 6);
    assert(complement->couples.uint8[0] == 4);
    assert(complement->couples.uint8[1] == 5);
    assert(complement->couples.uint8[2] == 6);
    assert(complement->couples.uint8[3] == 7);
    assert(complement->couples.uint8[4] == 10);
    assert(complement->couples.uint8[5] == 20);
    inversion_list_destroy(complement);
  }
  {
    unsigned int a[] = {1, 2, 3, 5, 7, 8, 9, 2};
    InversionList *set = inversion_list_create(20, sizeof a / sizeof *a, a);
    InversionList *complement = inversion_list_complement(set);
    inversion_list_destroy(set);
    assert(complement->capacity == 20);
    assert(complement->support == 13);
    assert(complement->size == 8);
    assert(complement->couples.uint8[0] == 0);
    assert(complement->couples.uint8[1] == 1);
    assert(complement->couples.uint8[2] == 4);
    assert(complement->couples.uint8[3] == 5);
    assert(complement->couples.uint8[4] == 6);
    assert(complement->couples.uint8[5] == 7);
    assert(complement->couples.uint8[6] == 10);
    assert(complement->couples.uint8[7] == 20);
    inversion_list_destroy(complement);
  }
  {
    unsigned int a[] = {1, 2, 3, 5, 7, 8, 9, 2, 19};
    InversionList *set = inversion_list_create(20, sizeof a / sizeof *a, a);
    InversionList *complement = inversion_list_complement(set);
    inversion_list_destroy(set);
    assert(complement->capacity == 20);
    assert(complement->support == 12);
    assert(complement->size == 8);
    assert(complement->couples.uint8[0] == 0);
    assert(complement->couples.uint8[1] == 1);
    assert(complement->couples.uint8[2] == 4);
    assert(complement->couples.uint8[3] == 5);
    assert(complement->couples.uint8[4] == 6);
    assert(complement->couples.uint8[5] == 7);
    assert(complement->couples.uint8[6] == 10);
    assert(complement->couples.uint8[7] == 19);
    inversion_list_destroy(complement);
  }
  {
    unsigned int a[] = {1, 2, 3, 5, 7, 8, 9, 2, 19, 0};
    InversionList *set = inversion_list_create(20, sizeof a / sizeof *a, a);
    InversionList *complement = inversion_list_complement(set);
    inversion_list_destroy(set);
    assert(complement->capacity == 20);
    assert(complement->support == 11);
    assert(complement->size == 6);
    assert(complement->couples.uint8[0] == 4);
    assert(complement->couples.uint8[1] == 5);
    assert(complement->couples.uint8[2] == 6);
    assert(complement->couples.uint8[3] == 7);
    assert(complement->couples.uint8[4] == 10);
    assert(complement->couples.uint8[5] == 19);
    inversion_list_destroy(complement);
  }
  inversion_list_finish();
  return EXIT_SUCCESS;
}
