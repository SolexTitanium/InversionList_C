from cython import bint

cdef extern from "inversion-list.h":
    ctypedef struct InversionList:
        pass
    
    ctypedef struct InversionListIterator:
        pass

    ctypedef struct InversionListCoupleIterator:
        pass

    bint inversion_list_init()
    bint inversion_list_finish()

    InversionList *inversion_list_create(unsigned int capacity, size_t count, const unsigned int *values)
    void inversion_list_destroy(InversionList *set)

    unsigned int inversion_list_capacity(const InversionList *set)
    unsigned int inversion_list_support(const InversionList *set)
    bint inversion_list_member(const InversionList *set, unsigned int value)

    InversionList *inversion_list_clone(const InversionList *set)
    InversionList *inversion_list_complement(const InversionList *set)
    const char *inversion_list_to_string(const InversionList *set)

    bint inversion_list_equal(const InversionList *set1, const InversionList *set2)
    bint inversion_list_not_equal(const InversionList *set1, const InversionList *set2)
    bint inversion_list_less(const InversionList *set1, const InversionList *set2)
    bint inversion_list_less_equal(const InversionList *set1, const InversionList *set2)
    bint inversion_list_greater(const InversionList *set1, const InversionList *set2)
    bint inversion_list_greater_equal(const InversionList *set1, const InversionList *set2)
    bint inversion_list_disjoint(const InversionList *set1, const InversionList *set2)

    InversionList *inversion_list_union(const InversionList *set, ...)
    InversionList *inversion_list_intersection(const InversionList *set, ...)
    InversionList *inversion_list_difference(const InversionList *set, ...)
    InversionList *inversion_list_symmetric_difference(const InversionList *set1, const InversionList *set2)

    InversionListIterator *inversion_list_iterator_create(const InversionList *set)
    void inversion_list_iterator_destroy(InversionListIterator *iterator)
    InversionListIterator *inversion_list_iterator_next(InversionListIterator *iterator)
    InversionListIterator *inversion_list_iterator_rewind(InversionListIterator *iterator)
    bint inversion_list_iterator_valid(const InversionListIterator *iterator)
    unsigned int inversion_list_iterator_get(const InversionListIterator *iterator)

    InversionListCoupleIterator *inversion_list_couple_iterator_create(const InversionList *set)
    void inversion_list_couple_iterator_destroy(InversionListCoupleIterator *iterator)
    InversionListCoupleIterator *inversion_list_couple_iterator_next(InversionListCoupleIterator *iterator)
    InversionListCoupleIterator *inversion_list_couple_iterator_rewind(InversionListCoupleIterator *iterator)
    bint inversion_list_couple_iterator_valid(const InversionListCoupleIterator *iterator)
    unsigned int inversion_list_couple_iterator_get_inf(const InversionListCoupleIterator *iterator)
    unsigned int inversion_list_couple_iterator_get_sup(const InversionListCoupleIterator *iterator)
