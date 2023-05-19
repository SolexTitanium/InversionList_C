
.. _program_listing_file_include_inversion-list.h:

Program Listing for File inversion-list.h
=========================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_inversion-list.h>` (``include/inversion-list.h``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #ifndef INVERSION_LIST_H_
   #define INVERSION_LIST_H_
   
   #include <stdbool.h>
   #include <stdlib.h>
   
   typedef struct _InversionList InversionList;
   
   typedef struct _InversionListIterator InversionListIterator;
   
   typedef struct _InversionListCoupleIterator InversionListCoupleIterator;
   
   extern bool inversion_list_init(void);
   extern bool inversion_list_finish(void);
   
   extern InversionList *inversion_list_create(unsigned int capacity, size_t count, const unsigned int *values);
   extern void inversion_list_destroy(InversionList *set);
   
   extern unsigned int inversion_list_capacity(const InversionList *set);
   extern unsigned int inversion_list_support(const InversionList *set);
   
   extern bool inversion_list_member(const InversionList *set, unsigned int value);
   extern InversionList *inversion_list_clone(const InversionList *set);
   extern InversionList *inversion_list_complement(const InversionList *set);
   
   extern const char *inversion_list_to_string(const InversionList *set);
   
   extern bool inversion_list_equal(const InversionList *set1, const InversionList *set2);
   
   extern bool inversion_list_not_equal(const InversionList *set1, const InversionList *set2);
   
   extern bool inversion_list_less(const InversionList *set1, const InversionList *set2);
   
   extern bool inversion_list_less_equal(const InversionList *set1, const InversionList *set2);
   
   extern bool inversion_list_greater(const InversionList *set1, const InversionList *set2);
   
   extern bool inversion_list_greater_equal(const InversionList *set1, const InversionList *set2);
   
   extern bool inversion_list_disjoint(const InversionList *set1, const InversionList *set2);
   
   extern InversionList *inversion_list_union(const InversionList *set1, const InversionList *set2);
   
   extern InversionList *inversion_list_intersection(const InversionList *set1, const InversionList *set2);
   
   extern InversionListIterator *inversion_list_iterator_create(const InversionList *set);
   
   extern void inversion_list_iterator_destroy(InversionListIterator *iterator);
   
   extern InversionListIterator *inversion_list_iterator_next(InversionListIterator *iterator);
   
   extern InversionListIterator *inversion_list_iterator_rewind(InversionListIterator *iterator);
   
   extern bool inversion_list_iterator_valid(const InversionListIterator *iterator);
   
   extern unsigned int inversion_list_iterator_get(const InversionListIterator *iterator);
   
   extern InversionListCoupleIterator *inversion_list_couple_iterator_create(const InversionList *set);
   
   extern void inversion_list_couple_iterator_destroy(InversionListCoupleIterator *iterator);
   
   extern InversionListCoupleIterator *inversion_list_couple_iterator_next(InversionListCoupleIterator *iterator);
   
   extern InversionListCoupleIterator *inversion_list_couple_iterator_rewind(InversionListCoupleIterator *iterator);
   
   extern bool inversion_list_iterator_couple_valid(const InversionListCoupleIterator *iterator);
   
   extern unsigned int inversion_list_couple_iterator_get_inf(const InversionListCoupleIterator *iterator);
   
   extern unsigned int inversion_list_couple_iterator_get_sup(const InversionListCoupleIterator *iterator);
   
   #endif  // INVERSION_LIST_H_
