#include <errno.h>
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
    InversionList *set = inversion_list_create(300, sizeof a / sizeof *a, a);
    assert(set->couples.uint16[0] == 0);
    assert(set->couples.uint16[1] == 4);
    assert(set->couples.uint16[2] == 5);
    assert(set->couples.uint16[3] == 6);
    assert(set->couples.uint16[4] == 7);
    assert(set->couples.uint16[5] == 10);
    inversion_list_destroy(set);

    set = inversion_list_create(70000, sizeof a / sizeof *a, a);
    assert(set->couples.uint32[0] == 0);
    assert(set->couples.uint32[1] == 4);
    assert(set->couples.uint32[2] == 5);
    assert(set->couples.uint32[3] == 6);
    assert(set->couples.uint32[4] == 7);
    assert(set->couples.uint32[5] == 10);
    inversion_list_destroy(set);
  }
  inversion_list_finish();
  return EXIT_SUCCESS;
}
