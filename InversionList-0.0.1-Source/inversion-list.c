#include "./inversion-list.h"  // NOLINT(build/include)

#include <assert.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./inversion-list.inc"

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

static int _compare_uint8(const void *value1, const void *value2) {
  return *(uint8_t *)value1 - *(uint8_t *)value2;  // NOLINT(readability/casting)
}

static int _compare_uint16(const void *value1, const void *value2) {
  return *(uint16_t *)value1 - *(uint16_t *)value2;  // NOLINT(readability/casting)
}

static int _compare_uint32(const void *value1, const void *value2) {
  return *(uint32_t *)value1 - *(uint32_t *)value2;  // NOLINT(readability/casting)
}

/**
 * @brief Return an errno value in case of error or 0 otherwise.
 */
static int _compute_size_and_support_uint8(unsigned int capacity, size_t count,
                                            const void *buffer, size_t *size,
                                            unsigned int *support) {
  uint8_t *buffer_uint8 = (uint8_t *)buffer;
  qsort(buffer_uint8, count, sizeof(uint8_t), _compare_uint8);
  if (buffer_uint8[count - 1] >= capacity) {
    return EINVAL;
  }
  *size = 2;
  *support += 1;
  for (unsigned int i = 1; i < count; i++) {
    if (buffer_uint8[i] != buffer_uint8[i - 1]) {
      *support += 1;
      if (buffer_uint8[i] > buffer_uint8[i - 1] + 1) {
        *size += 2;
      }
    }
  }

  return 0;
}


/**
 * @brief Return an errno value in case of error or 0 otherwise.
 */
static int _compute_size_and_support_uint16(unsigned int capacity, size_t count,
                                            const void *buffer, size_t *size,
                                            unsigned int *support) {
  uint16_t *buffer_uint16 = (uint16_t *)buffer;
  qsort(buffer_uint16, count, sizeof(uint16_t), _compare_uint16);
  if (buffer_uint16[count - 1] >= capacity) {
    return EINVAL;
  }
  *size = 2;
  *support += 1;
  for (unsigned int i = 1; i < count; i++) {
    if (buffer_uint16[i] != buffer_uint16[i - 1]) {
      *support += 1;
      if (buffer_uint16[i] > buffer_uint16[i - 1] + 1) {
        *size += 2;
      }
    }
  }

  return 0;
}

/**
 * @brief Return an errno value in case of error or 0 otherwise.
 */
static int _compute_size_and_support_uint32(unsigned int capacity, size_t count,
                                            const void *buffer, size_t *size,
                                            unsigned int *support) {
  uint32_t *buffer_uint32 = (uint32_t *)buffer;
  qsort(buffer_uint32, count, sizeof(uint32_t), _compare_uint32);
  if (buffer_uint32[count - 1] >= capacity) {
    return EINVAL;
  }
  *size = 2;
  *support += 1;
  for (unsigned int i = 1; i < count; i++) {
    if (buffer_uint32[i] != buffer_uint32[i - 1]) {
      *support += 1;
      if (buffer_uint32[i] > buffer_uint32[i - 1] + 1) {
        *size += 2;
      }
    }
  }

  return 0;
}

static void _fill_couples_uint8(size_t count, uint8_t *couples, const uint8_t *buffer) {
  unsigned int i = 0;
  unsigned int current = 0;

  couples[current] = buffer[i];
  current++;
  couples[current] = buffer[i] + 1;

  for (i = 1; i < count; i++) {
    if (buffer[i] == couples[current]) {
      couples[current]++;
    } else if (buffer[i] > couples[current]) {
      current++;
      couples[current] = buffer[i];
      current++;
      couples[current] = buffer[i] + 1;
    }
  }
}

static void _fill_couples_uint16(size_t count, uint16_t *couples, const uint16_t *buffer) {
  unsigned int i = 0;
  unsigned int current = 0;

  couples[current] = buffer[i];
  current++;
  couples[current] = buffer[i] + 1;

  for (i = 1; i < count; i++) {
    if (buffer[i] == couples[current]) {
      couples[current]++;
    } else if (buffer[i] > couples[current]) {
      current++;
      couples[current] = buffer[i];
      current++;
      couples[current] = buffer[i] + 1;
    }
  }
}

static void _fill_couples_uint32(size_t count, uint32_t *couples, const uint32_t *buffer) {
  unsigned int i = 0;
  unsigned int current = 0;

  couples[current] = buffer[i];
  current++;
  couples[current] = buffer[i] + 1;

  for (i = 1; i < count; i++) {
    if (buffer[i] == couples[current]) {
      couples[current]++;
    } else if (buffer[i] > couples[current]) {
      current++;
      couples[current] = buffer[i];
      current++;
      couples[current] = buffer[i] + 1;
    }
  }
}

static void _copy_unsigned_int_array_to_uint8_array(size_t count, const unsigned int *src, uint8_t *dst) {
  for (size_t i = 0; i < count; i++) {
    dst[i] = (uint8_t)src[i];
  }
}

static void _copy_unsigned_int_array_to_uint16_array(size_t count, const unsigned int *src, uint16_t *dst) {
  for (size_t i = 0; i < count; i++) {
    dst[i] = (uint16_t)src[i];
  }
}

static void _copy_unsigned_int_array_to_uint32_array(size_t count, const unsigned int *src, uint32_t *dst) {
  for (size_t i = 0; i < count; i++) {
    dst[i] = (uint32_t)src[i];
  }
}

InversionList *inversion_list_create(unsigned int capacity, size_t count, const unsigned int *values) {
  assert(_counter > 0);

  bool can_use_8_bits = capacity <= 0xFF;
  bool can_use_16_bits = capacity <= 0xFFFF;

  size_t element_size;
  if (can_use_8_bits) {
    element_size = sizeof(uint8_t);
  } else if (can_use_16_bits) {
    element_size = sizeof(uint16_t);
  } else {
    element_size = sizeof(uint32_t);
  }

  void *buffer = _get_buffer(count * element_size);
  if (!buffer) {
    return NULL;
  }

  if (can_use_8_bits) {
    _copy_unsigned_int_array_to_uint8_array(count, values, (uint8_t *)buffer);
  } else if (can_use_16_bits) {
    _copy_unsigned_int_array_to_uint16_array(count, values, (uint16_t *)buffer);
  } else {
    _copy_unsigned_int_array_to_uint32_array(count, values, (uint32_t *)buffer);
  }

  unsigned int i;
  size_t size;
  unsigned int support = 0;
  if (count) {
    int status;
    if (can_use_8_bits) {
      status = _compute_size_and_support_uint8(capacity, count, buffer, &size, &support);
    } else if (can_use_16_bits) {
      status = _compute_size_and_support_uint16(capacity, count, buffer, &size, &support);
    } else {
      status = _compute_size_and_support_uint32(capacity, count, buffer, &size, &support);
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
      if (can_use_8_bits) {
        _fill_couples_uint8(count, (uint8_t *)set->couples.uint8, (uint8_t *)buffer);
      } else if (can_use_16_bits) {
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

  bool can_use_8_bits = set->capacity <= 0xFF;
  bool can_use_16_bits = set->capacity <= 0xFFFF;

  const void *couples;
  int (*compare)(const void *, const void *);
  size_t element_size;

  if (can_use_8_bits) {
    couples = set->couples.uint8;
    compare = _compare_uint8;
    element_size = sizeof(uint8_t);
  } else if (can_use_16_bits) {
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

  bool can_use_8_bits = set->capacity <= 0xFF;
  bool can_use_16_bits = set->capacity <= 0xFFFF;

  size_t element_size;
  if (can_use_8_bits) {
    element_size = sizeof(uint8_t);
  } else if (can_use_16_bits) {
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

static void _compute_complement_uint8(const InversionList *source,
                                      InversionList *complement,
                                      size_t final_size) {
  uint8_t *source_couples = source->couples.uint8;
  uint8_t *complement_couples = complement->couples.uint8;
                                        
  if (source_couples[0] == 0) {
    if (source_couples[source->size - 1] == source->capacity) {
      memcpy(complement_couples, source_couples + 1, final_size * sizeof(uint8_t));  // flawfinder: ignore
    } else {
      memcpy(complement_couples, source_couples + 1, (final_size - 1) * sizeof(uint8_t));  // flawfinder: ignore
      complement_couples[final_size - 1] = source->capacity;
    }
  } else {
    complement_couples[0] = 0;
    if (source_couples[source->size - 1] == source->capacity) {
      memcpy(complement_couples + 1, source_couples, (final_size - 1) * sizeof(uint8_t));  // flawfinder: ignore
    } else {
      memcpy(complement_couples + 1, source_couples, (final_size - 2) * sizeof(uint8_t));  // flawfinder: ignore
      complement_couples[final_size - 1] = source->capacity;
    }
  }
}

static void _compute_complement_uint16(const InversionList *source,
                                      InversionList *complement,
                                      size_t final_size) {
  uint16_t *source_couples = source->couples.uint16;
  uint16_t *complement_couples = complement->couples.uint16;
                                        
  if (source_couples[0] == 0) {
    if (source_couples[source->size - 1] == source->capacity) {
      memcpy(complement_couples, source_couples + 1, final_size * sizeof(uint16_t));  // flawfinder: ignore
    } else {
      memcpy(complement_couples, source_couples + 1, (final_size - 1) * sizeof(uint16_t));  // flawfinder: ignore
      complement_couples[final_size - 1] = source->capacity;
    }
  } else {
    complement_couples[0] = 0;
    if (source_couples[source->size - 1] == source->capacity) {
      memcpy(complement_couples + 1, source_couples, (final_size - 1) * sizeof(uint16_t));  // flawfinder: ignore
    } else {
      memcpy(complement_couples + 1, source_couples, (final_size - 2) * sizeof(uint16_t));  // flawfinder: ignore
      complement_couples[final_size - 1] = source->capacity;
    }
  }
}

static void _compute_complement_uint32(const InversionList *source,
                                      InversionList *complement,
                                      size_t final_size) {
  uint32_t *source_couples = source->couples.uint32;
  uint32_t *complement_couples = complement->couples.uint32;
                                        
  if (source_couples[0] == 0) {
    if (source_couples[source->size - 1] == source->capacity) {
      memcpy(complement_couples, source_couples + 1, final_size * sizeof(uint32_t));  // flawfinder: ignore
    } else {
      memcpy(complement_couples, source_couples + 1, (final_size - 1) * sizeof(uint32_t));  // flawfinder: ignore
      complement_couples[final_size - 1] = source->capacity;
    }
  } else {
    complement_couples[0] = 0;
    if (source_couples[source->size - 1] == source->capacity) {
      memcpy(complement_couples + 1, source_couples, (final_size - 1) * sizeof(uint32_t));  // flawfinder: ignore
    } else {
      memcpy(complement_couples + 1, source_couples, (final_size - 2) * sizeof(uint32_t));  // flawfinder: ignore
      complement_couples[final_size - 1] = source->capacity;
    }
  }
}

InversionList *inversion_list_complement(const InversionList *set) {
  assert(_counter > 0);

  bool can_use_8_bits = set->capacity <= 0xFF;
  bool can_use_16_bits = set->capacity <= 0xFFFF;

  size_t element_size;
  size_t size = set->size;
  if (can_use_8_bits) {
    element_size = sizeof(uint8_t);

    if (set->couples.uint8[0] == 0 && set->couples.uint8[set->size - 1] == set->capacity) {
      size -= 2;
    } else if (set->couples.uint8[0] != 0 && set->couples.uint8[set->size - 1] != set->capacity) {
      size += 2;
    }
  } else if (can_use_16_bits) {
    element_size = sizeof(uint16_t);

    if (set->couples.uint16[0] == 0 && set->couples.uint16[set->size - 1] == set->capacity) {
      size -= 2;
    } else if (set->couples.uint16[0] != 0 && set->couples.uint16[set->size - 1] != set->capacity) {
      size += 2;
    }
  } else {
    element_size = sizeof(uint32_t);

    if (set->couples.uint32[0] == 0 && set->couples.uint32[set->size - 1] == set->capacity) {
      size -= 2;
    } else if (set->couples.uint32[0] != 0 && set->couples.uint32[set->size - 1] != set->capacity) {
      size += 2;
    }
  }

  InversionList *complement = malloc(sizeof(InversionList) + size * element_size);
  if (complement) {
    complement->size = size;
    complement->capacity = set->capacity;
    complement->support = set->capacity - set->support;
    complement->couples.uint8 = (void *)complement + sizeof(InversionList);
    
    if (can_use_8_bits) {
      _compute_complement_uint8(set, complement, size);
    } else if (can_use_16_bits) {
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
static bool _add_uint8_string(const uint8_t *couples, unsigned int index, bool *first,
                       char **string, unsigned int *length) {
  unsigned int value;
  for (value = couples[index]; value < couples[index + 1]; value++) {
    if (*first) {
      *first = false;
    } else {
      if (!_add_string(string, length, ", ")) {
        return true;
      }
    }
    static char current[100];  // flawfinder: ignore
    snprintf(current, sizeof current, "%u", value);
    if (!_add_string(string, length, current)) {
      return true;
    }
  }

  return false;
}

/**
 * @brief Return true if an error occurred, false otherwise.
 */
static bool _add_uint16_string(const uint16_t *couples, unsigned int index, bool *first,
                       char **string, unsigned int *length) {
  unsigned int value;
  for (value = couples[index]; value < couples[index + 1]; value++) {
    if (*first) {
      *first = false;
    } else {
      if (!_add_string(string, length, ", ")) {
        return true;
      }
    }
    static char current[100];  // flawfinder: ignore
    snprintf(current, sizeof current, "%u", value);
    if (!_add_string(string, length, current)) {
      return true;
    }
  }

  return false;
}

/**
 * @brief Return true if an error occurred, false otherwise.
 */
static bool _add_uint32_string(const uint32_t *couples, unsigned int index, bool *first,
                       char **string, unsigned int *length) {
  unsigned int value;
  for (value = couples[index]; value < couples[index + 1]; value++) {
    if (*first) {
      *first = false;
    } else {
      if (!_add_string(string, length, ", ")) {
        return true;
      }
    }
    static char current[100];  // flawfinder: ignore
    snprintf(current, sizeof current, "%u", value);
    if (!_add_string(string, length, current)) {
      return true;
    }
  }

  return false;
}

const char *inversion_list_to_string(const InversionList *set) {
  static char *string = NULL;
  unsigned int length = 0;

  if (!set) {
    free(string);
    string = NULL;
    return NULL;
  }

  bool can_use_8_bits = set->capacity <= 0xFF;
  bool can_use_16_bits = set->capacity <= 0xFFFF;

  assert(_counter > 0);

  if (!_add_string(&string, &length, "[")) {
    return NULL;
  }

  unsigned int i;
  bool first = true;
  for (i = 0; i < set->size; i += 2) {
    bool error;
    if (can_use_8_bits) {
      error = _add_uint8_string(set->couples.uint8, i, &first, &string, &length);
    } else if (can_use_16_bits) {
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

