#include <stdlib.h>

#ifdef NDEBUG
#undef NDEBUG
#endif
#include <assert.h>

#include "./inversion-list.h"
#include "./inversion-list.inc"

int main(void) {
  InversionList set;
  assert(sizeof set == 2 * sizeof(unsigned int) + sizeof(size_t) + sizeof(void *));
  assert((void *)&set.capacity == (void *)&set);
  assert((void *)&set.support == (void *)&set + sizeof(unsigned int));
  assert((void *)&set.size == (void *)&set + 2 * sizeof(unsigned int));
  assert((void *)&set.couples == (void *)&set + 2 * sizeof(unsigned int) + sizeof(size_t));
  assert((void *)&set.couples.uint8 == (void *)&set.couples.uint16);
  assert((void *)&set.couples.uint16 == (void *)&set.couples.uint32);
  assert(sizeof(set.couples.uint8[0]) == sizeof(uint8_t));
  assert(sizeof(set.couples.uint16[0]) == sizeof(uint16_t));
  assert(sizeof(set.couples.uint32[0]) == sizeof(uint32_t));

  // TODO Est ce que c'est bon si on a des pointeurs à la place de tableaux sur couples ?

  // Old code:

  // InversionList set;
  // assert(sizeof set == 2 * sizeof(unsigned int) + sizeof(size_t));
  // assert((void *)&set.capacity == (void *)&set);
  // assert((void *)&set.support == (void *)&set + sizeof(unsigned int));
  // assert((void *)&set.size == (void *)&set + 2 * sizeof(unsigned int));
  // assert((void *)set.couples == (void *)&set + 2 * sizeof(unsigned int) + sizeof(size_t));
  // assert((void *)&set.couples[0] == (void *)&set + 2 * sizeof(unsigned int) + sizeof(size_t));
  // assert((void *)&set.couples[1] == (void *)&set + 2 * sizeof(unsigned int) + sizeof(size_t) + sizeof(unsigned int));
  return EXIT_SUCCESS;
}
