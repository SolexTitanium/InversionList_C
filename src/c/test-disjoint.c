#include <errno.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

#ifdef NDEBUG
#undef NDEBUG
#endif
#include <assert.h>

#include "./inversion-list.h"
#include "./inversion-list.inc"

int main(void) {
//      printf("test-1\n");

  inversion_list_init();
  {
    unsigned int a[] = {1, 2, 3, 5, 7, 8, 9};
    unsigned int b[] = {1, 2, 3, 5, 7, 8, 9};
    unsigned int c[] = {10, 20, 4, 6, 11, 12, 13};
    unsigned int d[] = {6, 20, 4, 1, 11, 12, 13};
  
    InversionList *seta = inversion_list_create(100, sizeof a / sizeof *a, a);
    InversionList *setb = inversion_list_create(100, sizeof b / sizeof *b, b);
    InversionList *setc = inversion_list_create(100, sizeof c / sizeof *c, c);
    InversionList *setd = inversion_list_create(100, sizeof d / sizeof *d, d);

    assert(inversion_list_disjoint(seta,setb) == false);
    assert(inversion_list_disjoint(seta,setc) == true);
    assert(inversion_list_disjoint(seta,setd) == false);
    
    inversion_list_destroy(seta);
    inversion_list_destroy(setb);
    inversion_list_destroy(setc);
    inversion_list_destroy(setd);
  }
  inversion_list_finish();
  return EXIT_SUCCESS;
}