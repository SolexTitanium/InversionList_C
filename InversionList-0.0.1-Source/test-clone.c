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
    InversionList *clone = inversion_list_clone(set);
    inversion_list_destroy(set);
    assert(clone->capacity == 20);
    assert(clone->support == 8);
    assert(clone->size == 6);
    assert(clone->couples.uint8[0] == 0);
    assert(clone->couples.uint8[1] == 4);
    assert(clone->couples.uint8[2] == 5);
    assert(clone->couples.uint8[3] == 6);
    assert(clone->couples.uint8[4] == 7);
    assert(clone->couples.uint8[5] == 10);
    inversion_list_destroy(clone);
  }
  inversion_list_finish();
  return EXIT_SUCCESS;
}
