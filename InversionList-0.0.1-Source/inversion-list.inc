#ifndef INVERSION_LIST_INC_
#define INVERSION_LIST_INC_

#include <stddef.h>
#include <stdint.h>

struct _InversionList {
  unsigned int capacity;
  unsigned int support;
  size_t size;
  union {
    uint8_t *uint8;
    uint16_t *uint16;
    uint32_t *uint32;
  } couples;
};

#endif  // INVERSION_LIST_INC_
