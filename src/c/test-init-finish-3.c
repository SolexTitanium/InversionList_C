#include <stdlib.h>

#ifdef NDEBUG
#undef NDEBUG
#endif
#include <assert.h>

#include "./inversion-list.h"
#include "./inversion-list.inc"

int main(void) {
  assert(inversion_list_init());
  assert(inversion_list_init());
  assert(inversion_list_init());
  assert(inversion_list_finish());
  assert(inversion_list_finish());
  assert(inversion_list_finish());
  assert(!inversion_list_finish());
  return EXIT_SUCCESS;
}
