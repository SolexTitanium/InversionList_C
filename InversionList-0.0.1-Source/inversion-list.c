#include "./inversion-list.h"  // NOLINT(build/include)

#include <assert.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./inversion-list.inc"

#define MAX(a, b) ((a > b) ? a : b)

static void *_buffer = NULL;
static size_t _size = 0;

static void *_get_buffer(size_t size) {
  if (size > _size) {
    void *buffer = realloc(_buffer, size);
    if (buffer) {
      _buffer = buffer;
      _size = size;
    } else {
      errno = ENOMEM;
      return NULL;
    }
  }
  return _buffer;
}

static int _counter = 0;

static void _verify(void) {
  assert(_counter == 0);
}

static bool _is_uint32(unsigned int list_capacity) {
  return list_capacity > 0xFFFF;
}

static bool _is_uint16(unsigned int list_capacity) {
  if (_is_uint32(list_capacity)) {
    return false;
  }

  return list_capacity > 0xFF;
}

static bool _is_uint8(unsigned int list_capacity) {
  if (_is_uint16(list_capacity)) {
    return false;
  }

  return list_capacity <= 0xFF;
}

bool inversion_list_init(void) {
  static bool first = true;
  if (first) {
    atexit(_verify);
    first = false;
  }
  if (!_counter++) {
    assert(printf("Inversion list initialised\n"));
  }
  return true;
}

bool inversion_list_finish(void) {
  if (_counter) {
    if (!--_counter) {
      free(_buffer);
      _buffer = NULL;
      _size = 0;

      inversion_list_to_string(NULL);
      assert(printf("Inversion list finished\n"));
    }
    return true;
  } else {
    return false;
  }
}

#define DEFINE_COMPARE(type)\
static int _compare_##type(const void *value1, const void *value2) {\
  return *(type##_t *)value1 - *(type##_t *)value2;\
}\

DEFINE_COMPARE(uint8);
DEFINE_COMPARE(uint16);
DEFINE_COMPARE(uint32);

/**
 * @brief Return an errno value in case of error or 0 otherwise.
 */
#define DEFINE_COMPUTE_SIZE_AND_SUPPORT(type)\
static int _compute_size_and_support_##type(unsigned int capacity, size_t count,\
                                            type##_t *buffer, size_t *size,\
                                            unsigned int *support) {\
  qsort(buffer, count, sizeof(type##_t), _compare_##type);\
  if (buffer[count - 1] >= capacity) {\
    return EINVAL;\
  }\
  *size = 2;\
  *support += 1;\
  for (unsigned int i = 1; i < count; i++) {\
    if (buffer[i] != buffer[i - 1]) {\
      *support += 1;\
      if (buffer[i] > buffer[i - 1] + 1) {\
        *size += 2;\
      }\
    }\
  }\
\
  return 0;\
}

DEFINE_COMPUTE_SIZE_AND_SUPPORT(uint8);
DEFINE_COMPUTE_SIZE_AND_SUPPORT(uint16);
DEFINE_COMPUTE_SIZE_AND_SUPPORT(uint32);

#define DEFINE_FILL_COUPLES(type)\
static void _fill_couples_##type(size_t count, type##_t *couples, const type##_t *buffer) {\
  unsigned int i = 0;\
  unsigned int current = 0;\
\
  couples[current] = buffer[i];\
  current++;\
  couples[current] = buffer[i] + 1;\
\
  for (i = 1; i < count; i++) {\
    if (buffer[i] == couples[current]) {\
      couples[current]++;\
    } else if (buffer[i] > couples[current]) {\
      current++;\
      couples[current] = buffer[i];\
      current++;\
      couples[current] = buffer[i] + 1;\
    }\
  }\
}

DEFINE_FILL_COUPLES(uint8);
DEFINE_FILL_COUPLES(uint16);
DEFINE_FILL_COUPLES(uint32);

#define DEFINE_COPY_UNSIGNED_INT_ARRAY_TO_TYPE_ARRAY(type)\
static void _copy_unsigned_int_array_to_##type##_array(size_t count, const unsigned int *src, type##_t *dst) {\
  for (size_t i = 0; i < count; i++) {\
    dst[i] = (type##_t)src[i];\
  }\
}\

DEFINE_COPY_UNSIGNED_INT_ARRAY_TO_TYPE_ARRAY(uint8);
DEFINE_COPY_UNSIGNED_INT_ARRAY_TO_TYPE_ARRAY(uint16);
DEFINE_COPY_UNSIGNED_INT_ARRAY_TO_TYPE_ARRAY(uint32);

InversionList *inversion_list_create(unsigned int capacity, size_t count, const unsigned int *values) {
  assert(_counter > 0);

  bool can_use_uint8 = _is_uint8(capacity);
  bool can_use_uint16 = _is_uint16(capacity);

  size_t element_size;
  if (can_use_uint8) {
    element_size = sizeof(uint8_t);
  } else if (can_use_uint16) {
    element_size = sizeof(uint16_t);
  } else {
    element_size = sizeof(uint32_t);
  }

  void *buffer = _get_buffer(count * element_size);
  if (!buffer) {
    return NULL;
  }

  if (can_use_uint8) {
    _copy_unsigned_int_array_to_uint8_array(count, values, (uint8_t *)buffer);
  } else if (can_use_uint16) {
    _copy_unsigned_int_array_to_uint16_array(count, values, (uint16_t *)buffer);
  } else {
    _copy_unsigned_int_array_to_uint32_array(count, values, (uint32_t *)buffer);
  }

  unsigned int i;
  size_t size;
  unsigned int support = 0;
  if (count) {
    int status;
    if (can_use_uint8) {
      status = _compute_size_and_support_uint8(capacity, count, (uint8_t *)buffer, &size, &support);
    } else if (can_use_uint16) {
      status = _compute_size_and_support_uint16(capacity, count, (uint16_t *)buffer, &size, &support);
    } else {
      status = _compute_size_and_support_uint32(capacity, count, (uint32_t *)buffer, &size, &support);
    }

    if (status != 0) {
      errno = status;
      return NULL;
    }
  } else {
    size = 0;
  }

  InversionList *set = malloc(sizeof(InversionList) + size * element_size);
  if (set) {
    set->size = size;
    set->capacity = capacity;
    set->support = support;
    // couples are stored after the InversionList structure
    set->couples.uint8 = (uint8_t *)set + sizeof(InversionList);
    if (size) {
      if (can_use_uint8) {
        _fill_couples_uint8(count, (uint8_t *)set->couples.uint8, (uint8_t *)buffer);
      } else if (can_use_uint16) {
        _fill_couples_uint16(count, (uint16_t *)set->couples.uint16, (uint16_t *)buffer);
      } else {
        _fill_couples_uint32(count, (uint32_t *)set->couples.uint32, (uint32_t *)buffer);
      }
    }
    return set;
  } else {
    errno = ENOMEM;
    return NULL;
  }
}

void inversion_list_destroy(InversionList *set) {
  assert(_counter > 0);

  free(set);
}

unsigned int inversion_list_capacity(const InversionList *set) {
  assert(_counter > 0);

  return set->capacity;
}

unsigned int inversion_list_support(const InversionList *set) {
  assert(_counter > 0);

  return set->support;
}

static int _index_of(const void *element, const void *base, size_t size) {
  return (element - base) / size;
}

static int _sign;
static bool _strict;
static const void *_found;
static int (*_compare_func)(const void *, const void *);
static int _search(const void *key, const void *value) {
  int compare = _compare_func(key, value);
  if (_strict) {
    if (_sign * compare > 0) {
      _found = value;
      compare = _sign;
    } else {
      compare = -_sign;
    }
  } else {
    if (_sign * compare >= 0) {
      _found = value;
      compare = _sign;
    }
  }
  return compare;
}

static const void *_bound(const void *key, const void *base, size_t nmemb, size_t size, int (*compar)(const void *, const void *), int sign,
                          bool strict) {
  _compare_func = compar;
  _found = NULL;
  _sign = sign;
  _strict = strict;
  bsearch(key, base, nmemb, size, _search);
  return _found;
}

static const void *_lower_bound(const void *key, const void *base, size_t nmemb, size_t size, int (*compar)(const void *, const void *),
                                bool strict) {
  return _bound(key, base, nmemb, size, compar, 1, strict);
}

static const void *_upper_bound(const void *key, const void *base, size_t nmemb, size_t size, int (*compar)(const void *, const void *),
                                bool strict) {
  return _bound(key, base, nmemb, size, compar, -1, strict);
}

bool inversion_list_member(const InversionList *set, unsigned int value) {
  assert(_counter > 0);

  bool is_using_uint8 = _is_uint8(set->capacity);
  bool is_using_uint16 = _is_uint16(set->capacity);

  const void *couples;
  int (*compare)(const void *, const void *);
  size_t element_size;

  if (is_using_uint8) {
    couples = set->couples.uint8;
    compare = _compare_uint8;
    element_size = sizeof(uint8_t);
  } else if (is_using_uint16) {
    couples = set->couples.uint16;
    compare = _compare_uint16;
    element_size = sizeof(uint16_t);
  } else {
    couples = set->couples.uint32;
    compare = _compare_uint32;
    element_size = sizeof(uint32_t);
  }

  const void *bound = _lower_bound(&value, couples, set->size, element_size, compare, false);
  if (bound) {
    return _index_of(bound, couples, element_size) % 2 == 0;
  } else {
    return false;
  }
}

InversionList *inversion_list_clone(const InversionList *set) {
  assert(_counter > 0);

  bool is_using_uint8 = _is_uint8(set->capacity);
  bool is_using_uint16 = _is_uint16(set->capacity);

  size_t element_size;
  if (is_using_uint8) {
    element_size = sizeof(uint8_t);
  } else if (is_using_uint16) {
    element_size = sizeof(uint16_t);
  } else {
    element_size = sizeof(uint32_t);
  }

  InversionList *clone = malloc(sizeof(InversionList) + set->size * element_size);
  if (clone) {
    memcpy(clone, set, sizeof(InversionList) + set->size * element_size);  // flawfinder: ignore
    clone->couples.uint8 = (uint8_t *)clone + sizeof(InversionList);
    return clone;
  } else {
    errno = ENOMEM;
    return NULL;
  }
}

#define DEFINE_COMPUTE_COMPLEMENT(type)\
static void _compute_complement_##type(const InversionList *source,\
                                      InversionList *complement,\
                                      size_t final_size) {\
  type##_t *source_couples = source->couples.type;\
  type##_t *complement_couples = complement->couples.type;\
\
  if (source_couples[0] == 0) {\
    if (source_couples[source->size - 1] == source->capacity) {\
      memcpy(complement_couples, source_couples + 1, final_size * sizeof(type##_t));\
    } else {\
      memcpy(complement_couples, source_couples + 1, (final_size - 1) * sizeof(type##_t));\
      complement_couples[final_size - 1] = source->capacity;\
    }\
  } else {\
    complement_couples[0] = 0;\
    if (source_couples[source->size - 1] == source->capacity) {\
      memcpy(complement_couples + 1, source_couples, (final_size - 1) * sizeof(type##_t));\
    } else {\
      memcpy(complement_couples + 1, source_couples, (final_size - 2) * sizeof(type##_t));\
      complement_couples[final_size - 1] = source->capacity;\
    }\
  }\
}

DEFINE_COMPUTE_COMPLEMENT(uint8);
DEFINE_COMPUTE_COMPLEMENT(uint16);
DEFINE_COMPUTE_COMPLEMENT(uint32);

#define DEFINE_LIST_COMPLEMENT_COMPUTE_SIZES(type)\
static void _list_complement_compute_sizes_##type(const InversionList *set,\
                                                 size_t *set_size,\
                                                 size_t *element_size) {\
  *element_size = sizeof(type##_t);\
\
  if (set->couples.type[0] == 0 && set->couples.type[set->size - 1] == set->capacity) {\
    *set_size -= 2;\
  } else if (set->couples.type[0] != 0 && set->couples.type[set->size - 1] != set->capacity) {\
    *set_size += 2;\
  }\
}

DEFINE_LIST_COMPLEMENT_COMPUTE_SIZES(uint8);
DEFINE_LIST_COMPLEMENT_COMPUTE_SIZES(uint16);
DEFINE_LIST_COMPLEMENT_COMPUTE_SIZES(uint32);

InversionList *inversion_list_complement(const InversionList *set) {
  assert(_counter > 0);

  bool is_using_uint8 = _is_uint8(set->capacity);
  bool is_using_uint16 = _is_uint16(set->capacity);

  size_t element_size;
  size_t size = set->size;
  if (is_using_uint8) {
    _list_complement_compute_sizes_uint8(set, &size, &element_size);
  } else if (is_using_uint16) {
    _list_complement_compute_sizes_uint16(set, &size, &element_size);
  } else {
    _list_complement_compute_sizes_uint32(set, &size, &element_size);
  }

  InversionList *complement = malloc(sizeof(InversionList) + size * element_size);
  if (complement) {
    complement->size = size;
    complement->capacity = set->capacity;
    complement->support = set->capacity - set->support;
    complement->couples.uint8 = (void *)complement + sizeof(InversionList);
    
    if (is_using_uint8) {
      _compute_complement_uint8(set, complement, size);
    } else if (is_using_uint16) {
      _compute_complement_uint16(set, complement, size);
    } else {
      _compute_complement_uint32(set, complement, size);
    }

    return complement;
  } else {
    errno = ENOMEM;
    return NULL;
  }
}

static bool _add_string(char **pstring, unsigned int *plength, const char *add) {
  if (add) {
    unsigned int inc_length = strlen(add);  // flawfinder: ignore
    char *string = realloc(*pstring, *plength + inc_length + 1);
    if (string) {
      *pstring = string;
      strncpy(*pstring + *plength, add, inc_length + 1);  // flawfinder: ignore
      *plength += inc_length;
      return true;
    } else {
      errno = ENOMEM;
      return false;
    }
  } else {
    return false;
  }
}

/**
 * @brief Return true if an error occurred, false otherwise.
 */
#define DEFINE_ADD_TYPE_STRING(type)\
static bool _add_##type##_string(const type##_t *couples, unsigned int index, bool *first,\
                       char **string, unsigned int *length) {\
  unsigned int value;\
  for (value = couples[index]; value < couples[index + 1]; value++) {\
    if (*first) {\
      *first = false;\
    } else {\
      if (!_add_string(string, length, ", ")) {\
        return true;\
      }\
    }\
    static char current[100];\
    snprintf(current, sizeof current, "%u", value);\
    if (!_add_string(string, length, current)) {\
      return true;\
    }\
  }\
\
  return false;\
}

DEFINE_ADD_TYPE_STRING(uint8);
DEFINE_ADD_TYPE_STRING(uint16);
DEFINE_ADD_TYPE_STRING(uint32);

const char *inversion_list_to_string(const InversionList *set) {
  static char *string = NULL;
  unsigned int length = 0;

  if (!set) {
    free(string);
    string = NULL;
    return NULL;
  }

  bool can_use_uint8 = _is_uint8(set->capacity);
  bool can_use_uint16 = _is_uint16(set->capacity);

  assert(_counter > 0);

  if (!_add_string(&string, &length, "[")) {
    return NULL;
  }

  unsigned int i;
  bool first = true;
  for (i = 0; i < set->size; i += 2) {
    bool error;
    if (can_use_uint8) {
      error = _add_uint8_string(set->couples.uint8, i, &first, &string, &length);
    } else if (can_use_uint16) {
      error = _add_uint16_string(set->couples.uint16, i, &first, &string, &length);
    } else {
      error = _add_uint32_string(set->couples.uint32, i, &first, &string, &length);
    }

    if (error) {
      return NULL;
    }
  }

  if (!_add_string(&string, &length, "]")) {
    return NULL;
  }

  return string;
}

#define DEFINE_IS_EQUAL(type1, type2)\
static bool _is_equal_##type1##_##type2(const InversionList *set1, const InversionList *set2) {\
  type1##_t *set1_couples_pointer = set1->couples.type1;\
  type2##_t *set2_couples_pointer = set2->couples.type2;\
  for (int i = 0; i < set1->size; ++i) {\
      if (*set1_couples_pointer == *set2_couples_pointer){\
          set1_couples_pointer++;\
          set2_couples_pointer++;\
      } else return false;\
  }\
\
  return true;\
}

DEFINE_IS_EQUAL(uint8, uint8);
DEFINE_IS_EQUAL(uint8, uint16);
DEFINE_IS_EQUAL(uint8, uint32);
DEFINE_IS_EQUAL(uint16, uint8);
DEFINE_IS_EQUAL(uint16, uint16);
DEFINE_IS_EQUAL(uint16, uint32);
DEFINE_IS_EQUAL(uint32, uint8);
DEFINE_IS_EQUAL(uint32, uint16);
DEFINE_IS_EQUAL(uint32, uint32);

bool inversion_list_equal(const InversionList *set1, const InversionList *set2){
    if (set1 == NULL || set2 == NULL) {
        return false;
    }
    if (set1 == set2) {
        return true;
    }
    if (set1->support != set2->support) {
        return false;
    }

    bool set1_is_uint8 = _is_uint8(set1->capacity);
    bool set1_is_uint16 = _is_uint16(set1->capacity);

    bool set2_is_uint8 = _is_uint8(set2->capacity);
    bool set2_is_uint16 = _is_uint16(set2->capacity);

    if (set1_is_uint8) {
      if (set2_is_uint8) {
        return _is_equal_uint8_uint8(set1, set2);
      } else if (set2_is_uint16) {
        return _is_equal_uint8_uint16(set1, set2);
      } else {
        return _is_equal_uint8_uint32(set1, set2);
      }
    } else if (set1_is_uint16) {
      if (set2_is_uint8) {
        return _is_equal_uint16_uint8(set1, set2);
      } else if (set2_is_uint16) {
        return _is_equal_uint16_uint16(set1, set2);
      } else {
        return _is_equal_uint16_uint32(set1, set2);
      }
    } else {
      if (set2_is_uint8) {
        return _is_equal_uint32_uint8(set1, set2);
      } else if (set2_is_uint16) {
        return _is_equal_uint32_uint16(set1, set2);
      } else {
        return _is_equal_uint32_uint32(set1, set2);
      }
    }

    return false;
}

bool inversion_list_not_equal(const InversionList *set1, const InversionList *set2){
    return !inversion_list_equal(set1,set2);
}

#define DEFINE_INVERSION_LIST_LESS_EQUAL(type1, type2)\
static bool _inversion_list_less_equal_##type1##_##type2(const InversionList *set1, const InversionList *set2) {\
  size_t set1_index = 0;\
  size_t set2_index = 0;\
\
  while (set1_index < set1->size && set2_index < set2->size) {\
    if (set1->couples.type1[set1_index] < set2->couples.type2[set2_index]){\
      return false;\
    }\
\
    if (set1->couples.type1[set1_index] >= set2->couples.type2[set2_index + 1]) {\
      set2_index += 2;\
      continue;\
    }\
\
    if (set1->couples.type1[set1_index + 1] > set2->couples.type2[set2_index + 1]) {\
      return false;\
    }\
\
    set1_index += 2;\
  }\
\
  return set2_index < set2->size;\
}

DEFINE_INVERSION_LIST_LESS_EQUAL(uint8, uint8);
DEFINE_INVERSION_LIST_LESS_EQUAL(uint8, uint16);
DEFINE_INVERSION_LIST_LESS_EQUAL(uint8, uint32);
DEFINE_INVERSION_LIST_LESS_EQUAL(uint16, uint8);
DEFINE_INVERSION_LIST_LESS_EQUAL(uint16, uint16);
DEFINE_INVERSION_LIST_LESS_EQUAL(uint16, uint32);
DEFINE_INVERSION_LIST_LESS_EQUAL(uint32, uint8);
DEFINE_INVERSION_LIST_LESS_EQUAL(uint32, uint16);
DEFINE_INVERSION_LIST_LESS_EQUAL(uint32, uint32);

bool inversion_list_less_equal(const InversionList *set1, const InversionList *set2) {
  bool set1_is_uint8 = _is_uint8(set1->capacity);
  bool set1_is_uint16 = _is_uint16(set1->capacity);
  bool set2_is_uint8 = _is_uint8(set2->capacity);
  bool set2_is_uint16 = _is_uint16(set2->capacity);

  if (set1->size == 0) {
    // If set1 is an empty set, it is included in any set
    return true;
  }

  if (set1_is_uint8) {
    if (set2_is_uint8) {
      _inversion_list_less_equal_uint8_uint8(set1, set2);
    } else if (set2_is_uint16) {
      _inversion_list_less_equal_uint8_uint16(set1, set2);
    } else {
      _inversion_list_less_equal_uint8_uint32(set1, set2);
    }
  } else if (set1_is_uint16) {
    if (set2_is_uint8) {
      _inversion_list_less_equal_uint16_uint8(set1, set2);
    } else if (set2_is_uint16) {
      _inversion_list_less_equal_uint16_uint16(set1, set2);
    } else {
      _inversion_list_less_equal_uint16_uint32(set1, set2);
    }
  } else {
    if (set2_is_uint8) {
      _inversion_list_less_equal_uint32_uint8(set1, set2);
    } else if (set2_is_uint16) {
      _inversion_list_less_equal_uint32_uint16(set1, set2);
    } else {
      _inversion_list_less_equal_uint32_uint32(set1, set2);
    }
  }
}

bool inversion_list_less(const InversionList *set1, const InversionList *set2) {
  return inversion_list_less_equal(set1, set2) && inversion_list_not_equal(set1, set2);
}

bool inversion_list_greater(const InversionList *set1, const InversionList *set2) {
  return inversion_list_less(set2, set1);
}

bool inversion_list_greater_equal(const InversionList *set1, const InversionList *set2) {
  return inversion_list_less_equal(set2, set1);
}

#define DEFINE_INVERSION_LIST_DISJOINT(type1, type2)\
static bool _inversion_list_disjoint_##type1##_##type2(const InversionList *set1, const InversionList *set2) {\
  size_t set1_index = 0;\
  size_t set2_index = 0;\
\
  while (set1_index < set1->size && set2_index < set2->size) {\
    if (set1->couples.type1[set1_index] == set2->couples.type2[set2_index]) {\
      return false;\
    }\
\
    if (set1->couples.type1[set1_index] >= set2->couples.type2[set2_index + 1]) {\
      set2_index += 2;\
      continue;\
    }\
\
    if (set1->couples.type1[set1_index + 1] == set2->couples.type2[set2_index + 1]) {\
      return false;\
    }\
\
    set1_index += 2;\
  }\
\
  return true;\
}

DEFINE_INVERSION_LIST_DISJOINT(uint8, uint8);
DEFINE_INVERSION_LIST_DISJOINT(uint8, uint16);
DEFINE_INVERSION_LIST_DISJOINT(uint8, uint32);
DEFINE_INVERSION_LIST_DISJOINT(uint16, uint8);
DEFINE_INVERSION_LIST_DISJOINT(uint16, uint16);
DEFINE_INVERSION_LIST_DISJOINT(uint16, uint32);
DEFINE_INVERSION_LIST_DISJOINT(uint32, uint8);
DEFINE_INVERSION_LIST_DISJOINT(uint32, uint16);
DEFINE_INVERSION_LIST_DISJOINT(uint32, uint32);

bool inversion_list_disjoint(const InversionList *set1, const InversionList *set2) {
  bool set1_is_uint8 = _is_uint8(set1->capacity);
  bool set1_is_uint16 = _is_uint16(set1->capacity);
  bool set2_is_uint8 = _is_uint8(set2->capacity);
  bool set2_is_uint16 = _is_uint16(set2->capacity);

  if (set1->size == 0) {
    // If set1 is an empty set, it is included in any set
    return true;
  }

  if (set1_is_uint8) {
    if (set2_is_uint8) {
      _inversion_list_disjoint_uint8_uint8(set1, set2);
    } else if (set2_is_uint16) {
      _inversion_list_disjoint_uint8_uint16(set1, set2);
    } else {
      _inversion_list_disjoint_uint8_uint32(set1, set2);
    }
  } else if (set1_is_uint16) {
    if (set2_is_uint8) {
      _inversion_list_disjoint_uint16_uint8(set1, set2);
    } else if (set2_is_uint16) {
      _inversion_list_disjoint_uint16_uint16(set1, set2);
    } else {
      _inversion_list_disjoint_uint16_uint32(set1, set2);
    }
  } else {
    if (set2_is_uint8) {
      _inversion_list_disjoint_uint32_uint8(set1, set2);
    } else if (set2_is_uint16) {
      _inversion_list_disjoint_uint32_uint16(set1, set2);
    } else {
      _inversion_list_disjoint_uint32_uint32(set1, set2);
    }
  }
}

#define COPY_SET2_COUPLES_TO_NEW_COUPLES(source_type, destination_type)\
for (size_t index = 0; index != set2->size; index++) {\
  ((destination_type##_t *)*new_couples)[index] = set2->couples.source_type[index];\
}

// The first argument must have a greater or equal type than the second one
static void _concat_couples(const InversionList *set1, const InversionList *set2, void **new_couples, size_t *new_couples_element_size) {
  size_t new_couples_element_count = set1->size + set2->size;

  // Put set2's couples first then set1's couples in new_couples
  if (_is_uint32(set1->capacity)) {
    *new_couples = _get_buffer(new_couples_element_count * sizeof(uint32_t));
    *new_couples_element_size = sizeof(uint32_t);

    if (_is_uint8(set2->capacity)) {
      COPY_SET2_COUPLES_TO_NEW_COUPLES(uint8, uint32);
    } else if (_is_uint16(set2->capacity)) {
      COPY_SET2_COUPLES_TO_NEW_COUPLES(uint16, uint32);
    } else {
      memcpy(*new_couples, set2->couples.uint32, set2->size * sizeof(uint32_t));
    }

    memcpy(*new_couples + set2->size * sizeof(uint32_t), set1->couples.uint32, set1->size * sizeof(uint32_t));
  } else if (_is_uint16(set1->capacity)) {
    *new_couples = _get_buffer(new_couples_element_count * sizeof(uint16_t));
    *new_couples_element_size = sizeof(uint16_t);

    if (_is_uint8(set2->capacity)) {
      COPY_SET2_COUPLES_TO_NEW_COUPLES(uint8, uint16);
    } else {
      memcpy(*new_couples, set2->couples.uint16, set2->size * sizeof(uint16_t));
    }

    memcpy(*new_couples + set2->size * sizeof(uint16_t), set1->couples.uint16, set1->size * sizeof(uint16_t));
  } else {
    *new_couples = _get_buffer(new_couples_element_count * sizeof(uint8_t));
    *new_couples_element_size = sizeof(uint8_t);

    memcpy(*new_couples, set2->couples.uint8, set2->size * sizeof(uint8_t));
    memcpy(*new_couples + set2->size * sizeof(uint8_t), set1->couples.uint8, set1->size * sizeof(uint8_t));
  }
}

// Sort by the first element of the couple, then by the second one if the first one is equal
#define DEFINE_COMPARE_COUPLES(type)\
static int _compare_couples_##type(const void *couple1, const void *couple2) {\
  if (((type##_t *)couple1)[0] == ((type##_t *)couple2)[0]) {\
    return (int)((type##_t *)couple1)[1] - ((type##_t *)couple2)[1];\
  }\
  return (int)((type##_t *)couple1)[0] - ((type##_t *)couple2)[0];\
}

DEFINE_COMPARE_COUPLES(uint8);
DEFINE_COMPARE_COUPLES(uint16);
DEFINE_COMPARE_COUPLES(uint32);

#define DEFINE_UNION(type)\
static void _union_##type(type##_t *couples, size_t couple_count, type##_t **final_couples, size_t *final_couples_element_count) {\
  if (couple_count == 0) {\
    *final_couples = NULL;\
    *final_couples_element_count = 0;\
    return;\
  }\
  *final_couples = malloc(sizeof(type##_t) * couple_count * 2);\
  *final_couples_element_count = 0;\
\
  if (couple_count > 0) {\
    (*final_couples)[0] = couples[0];\
    (*final_couples)[1] = couples[1];\
    *final_couples_element_count += 2;\
  }\
\
  size_t final_couples_index = 1;\
  size_t couples_index = 2;\
  while (couples_index < (couple_count*2)) {\
    if (couples[couples_index - 1] >= couples[couples_index]) {\
      (*final_couples)[final_couples_index] = couples[couples_index + 1];\
    } else {\
      (*final_couples)[final_couples_index + 1] = couples[couples_index];\
      (*final_couples)[final_couples_index + 2] = couples[couples_index + 1];\
      *final_couples_element_count += 2;\
      final_couples_index += 2;\
    }\
\
    couples_index += 2;\
  }\
}

DEFINE_UNION(uint8);
DEFINE_UNION(uint16);
DEFINE_UNION(uint32);

#define DEFINE_COMPUTE_SUPPORT(type)\
static void _compute_support_##type(InversionList *set) {\
  set->support = 0;\
  for (size_t index = 0; index < set->size; index += 2) {\
    set->support += set->couples.type[index + 1] - set->couples.type[index];\
  }\
}

DEFINE_COMPUTE_SUPPORT(uint8);
DEFINE_COMPUTE_SUPPORT(uint16);
DEFINE_COMPUTE_SUPPORT(uint32);

#define UNION(type) {\
  qsort(new_couples, couple_count, couple_size, _compare_couples_##type);\
  _union_##type(new_couples, couple_count, (type##_t**)&final_couples, &final_couples_element_count);\
  \
  union_result = malloc(sizeof(InversionList) + final_couples_element_count * sizeof(type##_t));\
  union_result->size = final_couples_element_count;\
  union_result->capacity = MAX(set1->capacity, set2->capacity);\
  union_result->couples.uint8 = (uint8_t *)union_result + sizeof(InversionList);\
  \
  if (final_couples_element_count != 0) {\
    memcpy(union_result->couples.uint8, final_couples, final_couples_element_count * sizeof(type##_t));\
  }\
  \
  _compute_support_##type(union_result);\
}

#define DEFINE_INTERSECTION(type)\
static void _intersection_##type(type##_t *couples, size_t couple_count, type##_t **final_couples, size_t *final_couples_element_count) {\
  if (couple_count == 0) {\
    *final_couples = NULL;\
    *final_couples_element_count = 0;\
    return;\
  }\
  *final_couples = malloc(sizeof(type##_t) * couple_count * 2);\
  *final_couples_element_count = 0;\
\
  size_t final_couples_index = 0;\
  size_t couples_index = 2;\
  while (couples_index < (couple_count*2)) {\
    if (couples[couples_index - 1] > couples[couples_index]) {\
      (*final_couples)[final_couples_index] = couples[couples_index];\
      (*final_couples)[final_couples_index + 1] = couples[couples_index - 1];\
      final_couples_index += 2;\
      *final_couples_element_count += 2;\
    }\
\
    couples_index += 2;\
  }\
}

DEFINE_INTERSECTION(uint8);
DEFINE_INTERSECTION(uint16);
DEFINE_INTERSECTION(uint32);

#define INTERSECTION(type) {\
  qsort(new_couples, couple_count, couple_size, _compare_couples_##type);\
  _intersection_##type(new_couples, couple_count, (type##_t**)&final_couples, &final_couples_element_count);\
  \
  intersection = malloc(sizeof(InversionList) + final_couples_element_count * sizeof(type##_t));\
  intersection->size = final_couples_element_count;\
  intersection->capacity = MAX(set1->capacity, set2->capacity);\
  intersection->couples.uint8 = (uint8_t *)intersection + sizeof(InversionList);\
  \
  if (final_couples_element_count != 0) {\
    memcpy(intersection->couples.uint8, final_couples, final_couples_element_count * sizeof(type##_t));\
  }\
  \
  _compute_support_##type(intersection);\
}

InversionList *inversion_list_union(const InversionList *set1, const InversionList *set2) {
  InversionList *union_result;
  void *new_couples;
  size_t new_couples_element_size;

  // The first argument always has a greater or equal type than the second one
  // This is to avoid having to handle all the cases in _concat_couples
  if ((_is_uint32(set2->capacity) && !_is_uint32(set1->capacity)) ||
      (_is_uint16(set2->capacity) && _is_uint8(set1->capacity))) {
    _concat_couples(set2, set1, &new_couples, &new_couples_element_size);
  } else {
    _concat_couples(set1, set2, &new_couples, &new_couples_element_size);
  }

  void *final_couples;
  size_t final_couples_element_count;

  const size_t couple_count = (set1->size + set2->size) / 2;
  const size_t couple_size = new_couples_element_size * 2;
  if (new_couples_element_size == sizeof(uint32_t)) {
    UNION(uint32);
  } else if (new_couples_element_size == sizeof(uint16_t)) {
    UNION(uint16);
  } else {
    UNION(uint8);
  }

  free(final_couples);

  return union_result;
}

InversionList *inversion_list_intersection(const InversionList *set1, const InversionList *set2) {
  InversionList *intersection;
  void *new_couples;
  size_t new_couples_element_size;

  // The first argument always has a greater or equal type than the second one
  // This is to avoid having to handle all the cases in _concat_couples
  if ((_is_uint32(set2->capacity) && !_is_uint32(set1->capacity)) ||
      (_is_uint16(set2->capacity) && _is_uint8(set1->capacity))) {
    _concat_couples(set2, set1, &new_couples, &new_couples_element_size);
  } else {
    _concat_couples(set1, set2, &new_couples, &new_couples_element_size);
  }

  void *final_couples;
  size_t final_couples_element_count;

  const size_t couple_count = (set1->size + set2->size) / 2;
  const size_t couple_size = new_couples_element_size * 2;
  if (new_couples_element_size == sizeof(uint32_t)) {
    INTERSECTION(uint32);
  } else if (new_couples_element_size == sizeof(uint16_t)) {
    INTERSECTION(uint16);
  } else {
    INTERSECTION(uint8);
  }

  free(final_couples);

  return intersection;}

InversionListIterator *inversion_list_iterator_create(const InversionList *set) {
  InversionListIterator *it = malloc(sizeof (InversionList*) + sizeof (size_t) + sizeof (uint32_t));
  if (it == NULL){
    return NULL;
  }

  it->index = 0;
  it->iterated = set;

  if (_is_uint8(set->capacity)) {
    it->value = set->couples.uint8[it->index];
  } else if (_is_uint16(set->capacity)) {
    it->value = set->couples.uint16[it->index];
  } else {
    it->value = set->couples.uint32[it->index];
  }

  return it;
}

void inversion_list_iterator_destroy(InversionListIterator *iterator) {
  free(iterator);
}

InversionListIterator *inversion_list_iterator_next(InversionListIterator *iterator) {
  if (!inversion_list_iterator_valid(iterator)) {
    return iterator;
  }

  if(_is_uint8(iterator->iterated->capacity)) {
    if (iterator->value < iterator->iterated->couples.uint8[iterator->index + 1] - 1) {
      iterator->value++;
    } else {
      iterator->index += 2;
      if (inversion_list_iterator_valid(iterator)) {
        iterator->value = iterator->iterated->couples.uint8[iterator->index];
      } else {
        iterator->value = 0;
      }
    }
  }
  else if (_is_uint16(iterator->iterated->capacity)) {
    if (iterator->value < iterator->iterated->couples.uint16[iterator->index + 1] - 1) {
      iterator->value++;
    } else {
      iterator->index += 2;
      if (inversion_list_iterator_valid(iterator)) {
        iterator->value = iterator->iterated->couples.uint16[iterator->index];
      } else {
        iterator->value = 0;
      }
    }
  } else {
    if (iterator->value < iterator->iterated->couples.uint32[iterator->index + 1] - 1) {
      iterator->value++;
    } else {
      iterator->index += 2;
      if (inversion_list_iterator_valid(iterator)) {
        iterator->value = iterator->iterated->couples.uint32[iterator->index];
      } else {
        iterator->value = 0;
      }
    }
  }
  
  return iterator;
}

InversionListIterator *inversion_list_iterator_rewind(InversionListIterator *iterator){
  iterator->index = 0;

  if (_is_uint8(iterator->iterated->capacity)) {
    iterator->value = iterator->iterated->couples.uint8[0];
  } else if (_is_uint16(iterator->iterated->capacity)) {
    iterator->value = iterator->iterated->couples.uint16[0];
  } else {
    iterator->value = iterator->iterated->couples.uint32[0];
  }

  return iterator;

}

bool inversion_list_iterator_valid(const InversionListIterator *iterator) {
  if (iterator->index < iterator->iterated->size){
    return true;
  }
  return false;
}

unsigned int inversion_list_iterator_get(const InversionListIterator *iterator) {
  if (inversion_list_iterator_valid(iterator)){
    return iterator->value;
  }
    return 0;
}
InversionListCoupleIterator *inversion_list_couple_iterator_create(const InversionList *set){
InversionListCoupleIterator *it = malloc(sizeof (InversionList*) + sizeof (size_t));
    if (it == NULL){
      return NULL;
    }
    it->index = 0;
    it->iterated = set;
    return it;

}

void inversion_list_couple_iterator_destroy(InversionListCoupleIterator *iterator){
  free(iterator);
}

InversionListCoupleIterator *inversion_list_couple_iterator_next(InversionListCoupleIterator *iterator){
  if (!inversion_list_iterator_couple_valid(iterator)) {
    return iterator;
  }

  if(iterator->index+2 <= iterator->iterated->size) {
    iterator->index += 2;
  }
  return iterator;
}

InversionListCoupleIterator *inversion_list_couple_iterator_rewind(InversionListCoupleIterator *iterator){
  iterator->index=0;
  return iterator;
}

bool inversion_list_iterator_couple_valid(const InversionListCoupleIterator *iterator){
  if (iterator->index < iterator->iterated->size){  
    return true;
  }

  return false;
}

unsigned int inversion_list_couple_iterator_get_inf(const InversionListCoupleIterator *iterator){
 if(inversion_list_iterator_couple_valid(iterator)){
    if(_is_uint8(iterator->iterated->capacity)){
      return iterator->iterated->couples.uint8[iterator->index];
    }else if(_is_uint16(iterator->iterated->capacity)){
      return iterator->iterated->couples.uint16[iterator->index];
    }else{
      return iterator->iterated->couples.uint32[iterator->index];
    }
  }  

  return 0;
}

unsigned int inversion_list_couple_iterator_get_sup(const InversionListCoupleIterator *iterator){
  if(inversion_list_iterator_couple_valid(iterator)){
    if(_is_uint8(iterator->iterated->capacity)){
      return iterator->iterated->couples.uint8[iterator->index+1];
    }else if(_is_uint16(iterator->iterated->capacity)){
      return iterator->iterated->couples.uint16[iterator->index+1];
    }else{
      return iterator->iterated->couples.uint32[iterator->index+1];
    }
  }
  
  return 0;
  
}