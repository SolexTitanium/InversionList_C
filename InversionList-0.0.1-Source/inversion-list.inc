#ifndef INVERSION_LIST_INC_
#define INVERSION_LIST_INC_

struct _InversionList {
  unsigned int capacity;
  unsigned int support;
  size_t size;
  unsigned int couples[];
};

#endif  // INVERSION_LIST_INC_
