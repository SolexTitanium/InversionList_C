from cython import bint

cdef extern from "inversion-list.h":
    cdef struct _InversionList:
        unsigned int size
    ctypedef _InversionList InversionList

    bint inversion_list_init()
    bint inversion_list_finish()

    InversionList *inversion_list_create(unsigned int capacity, size_t count, const unsigned int *values)
    void inversion_list_destroy(InversionList *set)
    unsigned int inversion_list_capacity(const InversionList *set)
    unsigned int inversion_list_support(const InversionList *set)
    const char *inversion_list_to_string(const InversionList *set)
