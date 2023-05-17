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
 * `InversionListIterator` is an structure who iterate each integers in InversionList
 */
typedef struct _InversionListIterator InversionListIterator;
/**
 * `InversionListCoupleIterator` is an structure who iterate each couple in InversionList
 */
typedef struct _InversionListCoupleIterator InversionListCoupleIterator;

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
/**
 * Check the equality between two InversionList
 * @param set1 first InversionList
 * @param set2 second InversionList
 * @return true if set1 and set2 are equals, false in other cases
 */
extern bool inversion_list_equal(const InversionList *set1, const InversionList *set2);
/**
 * Check the inequality between two InversionList
 * @param set1 : first InversionList
 * @param set2 : second InversionList
 * @return false if set1 and set2 are equals, true in other cases
 */
extern bool inversion_list_not_equal(const InversionList *set1, const InversionList *set2);
/**
 *
 * @param set1
 * @param set2
 * @return
 */
extern bool inversion_list_less(const InversionList *set1, const InversionList *set2);
/**
 *
 * @param set1
 * @param set2
 * @return
 */
extern bool inversion_list_less_equal(const InversionList *set1, const InversionList *set2);
/**
 *
 * @param set1
 * @param set2
 * @return
 */
extern bool inversion_list_greater(const InversionList *set1, const InversionList *set2);
/**
 *
 * @param set1
 * @param set2
 * @return
 */
extern bool inversion_list_greater_equal(const InversionList *set1, const InversionList *set2);
/**
 * Check that 2 sets have no elements in common
 * @param set1
 * @param set2
 * @return return true if no element of set 1 and in set 2
 */
extern bool inversion_list_disjoint(const InversionList *set1, const InversionList *set2);

/**
 * @brief 
 * 
 * @param set 
 * @param ... 
 * @return
 */
extern InversionList *inversion_list_union(const InversionList *set, ...);

/**
 * @brief 
 * 
 * @param set 
 * @param ... 
 * @return
 */
extern InversionList *inversion_list_intersection(const InversionList *set, ...);

/**
 *
 * @param set1
 * @param set2
 * @return
 */
extern InversionList *inversion_list_difference(const InversionList *set1, const InversionList *set2);

/**
 * Check the set consisting of elements belonging to set1 and set 2 but not belonging to the intersection of set1 and set2
 * @param set1
 * @param set2
 * @return 
 */
extern InversionList *inversion_list_symmetric_difference(const InversionList *set1,const InversionList *set2);

/**
 *
 * @param set
 * @return
 */

extern InversionListIterator *inversion_list_iterator_create(const InversionList *set);
/**
 *
 * @param iterator
 */
extern void inversion_list_iterator_destroy(InversionListIterator *iterator);
/**
 *
 * @param iterator
 * @return
 */
extern InversionListIterator *inversion_list_iterator_next(InversionListIterator *iterator);
/**
 *
 * @param iterator
 * @return
 */
extern InversionListIterator *inversion_list_iterator_rewind(InversionListIterator *iterator);
/**
 *
 * @param iterator
 * @return
 */
extern bool inversion_list_iterator_valid(const InversionListIterator *iterator);
/**
 *
 * @param iterator
 * @return
 */
extern unsigned int inversion_list_iterator_get(const InversionListIterator *iterator);
/**
 * Allows to create an InversionListCoupleIterator
 * @param set
 * @return InversionListCoupleIterator
 */
extern InversionListCoupleIterator *inversion_list_couple_iterator_create(const InversionList *set);
/**
 *Allows you to destroy InversionListCoupleIterator
 * @param iterator
 */
extern void inversion_list_couple_iterator_destroy(InversionListCoupleIterator *iterator);
/**
 * Allows to have the terminal of the following couple
 * @param iterator
 * @return the next couple
 */
extern InversionListCoupleIterator *inversion_list_couple_iterator_next(InversionListCoupleIterator *iterator);
/**
 * Lets go back to the beginning
 * @param iterator
 * @return InversionListCoupleIterator
 */
extern InversionListCoupleIterator *inversion_list_couple_iterator_rewind(InversionListCoupleIterator *iterator);
/**
 * Check if the couple is valid
 * @param iterator
 * @return true if the pair is valid
 */
extern bool inversion_list_couple_iterator_valid(const InversionListCoupleIterator *iterator);
/**
 *  Get the value of lower couple
 * @param iterator
 * @return the value of lower couple
 */
extern unsigned int inversion_list_couple_iterator_get_inf(const InversionListCoupleIterator *iterator);
/**
 * Get the value of superior couple
 * @param iterator
 * @return the value of superoir couple
 */
extern unsigned int inversion_list_couple_iterator_get_sup(const InversionListCoupleIterator *iterator);
#endif  // INVERSION_LIST_H_
