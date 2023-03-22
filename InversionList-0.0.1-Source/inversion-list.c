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

static int _compare_unsigned_int(const void *value1, const void *value2) {
  return *(unsigned int *)value1 - *(unsigned int *)value2;  // NOLINT(readability/casting)
}

InversionList *inversion_list_create(unsigned int capacity, size_t count, const unsigned int *values) {
  assert(_counter > 0);

  unsigned int *buffer = _get_buffer(count * sizeof(unsigned int));
  if (!buffer) {
    return NULL;
  }

  unsigned int i;
  size_t size;
  memcpy(buffer, values, count * sizeof(unsigned int));  // flawfinder: ignore
  unsigned int support = 0;
  if (count) {
    qsort(buffer, count, sizeof *buffer, _compare_unsigned_int);
    if (buffer[count - 1] >= capacity) {
      errno = EINVAL;
      return NULL;
    }
    size = 2;
    support++;
    for (i = 1; i < count; i++) {
      if (buffer[i] != buffer[i - 1]) {
        support++;
        if (buffer[i] > buffer[i - 1] + 1) {
          size += 2;
        }
      }
    }
  } else {
    size = 0;
  }

  InversionList *set = malloc(sizeof(InversionList) + size * sizeof(unsigned int));
  if (set) {
    set->size = size;
    set->capacity = capacity;
    set->support = support;
    if (size) {
      i = 0;
      unsigned int current = 0;
      set->couples[current] = buffer[i];
      current++;
      set->couples[current] = buffer[i] + 1;
      for (i = 1; i < count; i++) {
        if (buffer[i] == set->couples[current]) {
          set->couples[current]++;
        } else if (buffer[i] > set->couples[current]) {
          current++;
          set->couples[current] = buffer[i];
          current++;
          set->couples[current] = buffer[i] + 1;
        }
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

  const void *bound = _lower_bound(&value, set->couples, set->size, sizeof(unsigned int), _compare_unsigned_int, false);
  if (bound) {
    return _index_of(bound, set->couples, sizeof(unsigned int)) % 2 == 0;
  } else {
    return false;
  }
}

InversionList *inversion_list_clone(const InversionList *set) {
  assert(_counter > 0);

  InversionList *clone = malloc(sizeof(InversionList) + set->size * sizeof(unsigned int));
  if (clone) {
    memcpy(clone, set, sizeof(InversionList) + set->size * sizeof(unsigned int));  // flawfinder: ignore
    return clone;
  } else {
    errno = ENOMEM;
    return NULL;
  }
}

InversionList *inversion_list_complement(const InversionList *set) {
  assert(_counter > 0);

  size_t size = set->size;
  if (set->couples[0] == 0 && set->couples[set->size - 1] == set->capacity) {
    size -= 2;
  } else if (set->couples[0] != 0 && set->couples[set->size - 1] != set->capacity) {
    size += 2;
  }

  InversionList *complement = malloc(sizeof(InversionList) + size * sizeof(unsigned int));
  if (complement) {
    complement->size = size;
    complement->capacity = set->capacity;
    complement->support = set->capacity - set->support;
    if (set->couples[0] == 0) {
      if (set->couples[set->size - 1] == set->capacity) {
        memcpy(complement->couples, set->couples + 1, size * sizeof(unsigned int));  // flawfinder: ignore
      } else {
        memcpy(complement->couples, set->couples + 1, (size - 1) * sizeof(unsigned int));  // flawfinder: ignore
        complement->couples[size - 1] = set->capacity;
      }
    } else {
      complement->couples[0] = 0;
      if (set->couples[set->size - 1] == set->capacity) {
        memcpy(complement->couples + 1, set->couples, (size - 1) * sizeof(unsigned int));  // flawfinder: ignore
      } else {
        memcpy(complement->couples + 1, set->couples, (size - 2) * sizeof(unsigned int));  // flawfinder: ignore
        complement->couples[size - 1] = set->capacity;
      }
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

const char *inversion_list_to_string(const InversionList *set) {
  static char *string = NULL;
  unsigned int length = 0;

  if (!set) {
    free(string);
    string = NULL;
    return NULL;
  }

  assert(_counter > 0);

  if (!_add_string(&string, &length, "[")) {
    return NULL;
  }

  unsigned int i;
  bool first = true;
  for (i = 0; i < set->size; i += 2) {
    unsigned int value;
    for (value = set->couples[i]; value < set->couples[i + 1]; value++) {
      if (first) {
        first = false;
      } else {
        if (!_add_string(&string, &length, ", ")) {
          return NULL;
        }
      }
      static char current[100];  // flawfinder: ignore
      snprintf(current, sizeof current, "%u", value);
      if (!_add_string(&string, &length, current)) {
        return NULL;
      }
    }
  }

  if (!_add_string(&string, &length, "]")) {
    return NULL;
  }

  return string;
}

