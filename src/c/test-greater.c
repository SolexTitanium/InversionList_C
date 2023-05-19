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
    unsigned int a[] = {1, 2, 3, 4, 6, 7};
    unsigned int b[] = {2, 3};
    unsigned int c[] = {2, 3, 6};
    unsigned int d[] = {0, 1};
    unsigned int e[] = {1, 2, 3, 4, 6, 7, 8, 9, 34, 35, 36};
    unsigned int f[] = {4, 5, 6};

    InversionList *set_a = inversion_list_create(100, sizeof a / sizeof *a, a);
    InversionList *set_b = inversion_list_create(100, sizeof b / sizeof *b, b);
    InversionList *set_c = inversion_list_create(100, sizeof c / sizeof *c, c);
    InversionList *set_d = inversion_list_create(100, sizeof d / sizeof *d, d);
    InversionList *set_e = inversion_list_create(100, sizeof e / sizeof *e, e);
    InversionList *set_f = inversion_list_create(100, sizeof f / sizeof *f, f);

    assert(inversion_list_greater(set_a, set_b));
    assert(inversion_list_greater(set_a, set_c));
    assert(!inversion_list_greater(set_a, set_d));
    assert(!inversion_list_greater(set_b, set_d));
    assert(!inversion_list_greater(set_d, set_b));
    assert(!inversion_list_greater(set_c, set_d));
    assert(!inversion_list_greater(set_d, set_c));
    assert(inversion_list_greater(set_e, set_a));
    assert(!inversion_list_greater(set_a, set_e));
    assert(!inversion_list_greater(set_e, set_f));
    assert(!inversion_list_greater(set_a, set_a));

    inversion_list_destroy(set_a);
    inversion_list_destroy(set_b);
    inversion_list_destroy(set_c);
    inversion_list_destroy(set_d);
    inversion_list_destroy(set_e);
    inversion_list_destroy(set_f);
  }
  inversion_list_finish();
  return EXIT_SUCCESS;
}