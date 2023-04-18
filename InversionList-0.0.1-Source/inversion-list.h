#ifndef INVERSION_LIST_H_
#define INVERSION_LIST_H_

/**
 * @file inversion-list.h
 */

#include <stdbool.h>
#include <stdlib.h>

/**
 * `InversionList` is an opaque structure able to handle sets of unsigned integers.
 */
typedef struct _InversionList InversionList;

/**
 * Initialise the library.
 */
extern bool inversion_list_init(void);
/**
 * Terminate the library.
 */
extern bool inversion_list_finish(void);

/**
 * Create a set of integers.
 *
 * @param capacity the capacity not to be reached
 * @param count the number of elements of @p values
 * @param values the values to be inserted in the set
 *
 * @return a new set of integers or `NULL` if there is an error
 *
 * @note a `NULL` return indicates an error which can be retrieved in `errno`
 */
extern InversionList *inversion_list_create(unsigned int capacity, size_t count, const unsigned int *values);
/**
 * Destroy a set of integers.
 *
 * @param set the set to be destroyed
 */
extern void inversion_list_destroy(InversionList *set);

/**
 * Get the set capacity.
 *
 * @param set the set whose capacity is requested
 *
 * @return the @p set capacity
 */
extern unsigned int inversion_list_capacity(const InversionList *set);
/**
 * Get the set support.
 *
 * @param set the set whose support is requested
 *
 * @return the @p set support
 */
extern unsigned int inversion_list_support(const InversionList *set);

/**
 * Get the membership of an element.
 *
 * @param set the set for which the membership of the @p value is requested
 * @param value the value
 *
 * @return the membership of the @p value in the @p set
 */
extern bool inversion_list_member(const InversionList *set, unsigned int value);
/**
 * Create a clone of set of integers.
 *
 * @param set a set whose clone is requested
 *
 * @return a @p set clone or `NULL` if there is an error
 *
 * @note a `NULL` return indicates an error which can be retrieved in `errno`
 */
extern InversionList *inversion_list_clone(const InversionList *set);
/**
 * Create a complement of a set of integers.
 *
 * @param set a set whose complement is requested
 *
 * @return a @p set complement or `NULL` if there is an error
 *
 * @note a `NULL` return indicates an error which can be retrieved in `errno`
 */
extern InversionList *inversion_list_complement(const InversionList *set);

/**
 * Convert a set of integers to a string.
 *
 * @param set a set whose string conversion is requested
 *
 * @return the string representation of @p set or `NULL` if there is an error
 *         or if @p set is equal to `NULL`.
 *
 * @note a `NULL` return may indicate an error which can be retrieved in `errno`
 */
extern const char *inversion_list_to_string(const InversionList *set);

#endif  // INVERSION_LIST_H_