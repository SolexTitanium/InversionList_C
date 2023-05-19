# distutils: include_dirs = ../../../include
# distutils: sources = ../../c/inversion-list.c

cimport cinversion_list

from typing import Optional, Iterable, Tuple, Iterator, AbstractSet
from cpython cimport array
import array

def init_library() -> bool:
    """
    Initialize the library.

    Must be called before using the IntegerSet class.
    """
    return cinversion_list.inversion_list_init()

def finish_library() -> bool:
    """
    Finish the library.

    Must be called after init_library was called.
    """
    return cinversion_list.inversion_list_finish()

cdef class IntegerSetRangeIterator:
    """Iterate each integers in InversionList"""
    cdef cinversion_list.InversionListCoupleIterator *iterator

    def __init__(self, set: IntegerSet):
        """Create a IntegerSetRangeIterator"""
        self.iterator = cinversion_list.inversion_list_couple_iterator_create(set.structure)
    
    def __dealloc__(self):
        """Destroy iterator in memory"""
        cinversion_list.inversion_list_couple_iterator_destroy(self.iterator)
    
    def __iter__(self):
        """Return self"""
        return self
    
    def __next__(self):
        """Get range of next couple"""
        if cinversion_list.inversion_list_couple_iterator_valid(self.iterator):
            boundary = range(
                cinversion_list.inversion_list_couple_iterator_get_inf(self.iterator),
                cinversion_list.inversion_list_couple_iterator_get_sup(self.iterator)
            )

            cinversion_list.inversion_list_couple_iterator_next(self.iterator)

            return boundary
        else:
            raise StopIteration

cdef class IntegerSetInvervalIterator:
    """Iterate each couple in InversionList"""
    cdef cinversion_list.InversionListCoupleIterator *iterator

    def __init__(self, set: IntegerSet):
        """Create a IntegerSetIntervalIterator"""
        self.iterator = cinversion_list.inversion_list_couple_iterator_create(set.structure)
    
    def __dealloc__(self):
        """Destroy iterator in memory"""
        cinversion_list.inversion_list_couple_iterator_destroy(self.iterator)
    
    def __iter__(self):
        """Return self"""
        return self
    
    def __next__(self):
        """Get next couple in the set"""
        if cinversion_list.inversion_list_couple_iterator_valid(self.iterator):
            boundary = (
                cinversion_list.inversion_list_couple_iterator_get_inf(self.iterator),
                cinversion_list.inversion_list_couple_iterator_get_sup(self.iterator)
            )

            cinversion_list.inversion_list_couple_iterator_next(self.iterator)

            return boundary
        else:
            raise StopIteration

cdef class IntegerSetIterator:
    cdef cinversion_list.InversionListIterator *iterator

    def __init__(self, set: IntegerSet):
        """Create a IntegerSetIterator"""
        self.iterator = cinversion_list.inversion_list_iterator_create(set.structure)
    
    def __dealloc__(self):
        """Destroy iterator in memory"""
        cinversion_list.inversion_list_iterator_destroy(self.iterator)
    
    def __iter__(self):
        """Return self"""
        return self
    
    def __next__(self):
        """Get next integer in the set"""
        if cinversion_list.inversion_list_iterator_valid(self.iterator):
            value = cinversion_list.inversion_list_iterator_get(self.iterator)

            cinversion_list.inversion_list_iterator_next(self.iterator)

            return value
        else:
            raise StopIteration

# The use of class IntegerSet(AbstractSet[int]) cause an error
cdef class IntegerSet:
    """IntegerSet is a structure able to handle sets of unsigned integers"""
    cdef cinversion_list.InversionList *structure
    cdef unsigned int size

    def __init__(
        self,
        intervals: Optional[Iterable[Tuple[int, int]]] = None,
    ) -> None:
        cdef array.array values = array.array('I', [])

        if intervals is not None and len(intervals) > 0:
            for interval in intervals:
                for value in range(interval[0], interval[1]):
                    values.append(value)

            if len(values) > 0:
                maximum = max(values)
            else:
                maximum = 0
        else:
            maximum = 0

        self.size = len(values)
        self.structure = cinversion_list.inversion_list_create(maximum + 1, self.size, <unsigned int *>values.data.as_voidptr)
        if not self.structure: 
            raise MemoryError("Error when creating the inversion list")
    
    def __dealloc__(self):
        cinversion_list.inversion_list_destroy(self.structure)

    @classmethod
    def from_iterable(
        cls,
        iterable: Optional[Iterable[int]] = None, 
    ) -> "IntegerSet":
        """Create a IntegerSet from a int Iterable"""
        integer_set = IntegerSet()
        cdef array.array values = array.array('I', [])

        if iterable is not None:
            for value in iterable:
                values.append(value)
            
            if len(values) > 0:
                maximum = max(values)
            else:
                maximum = 0
        else:
            maximum = 0
        
        integer_set.size = len(values)
        cinversion_list.inversion_list_destroy(integer_set.structure)
        integer_set.structure = cinversion_list.inversion_list_create(maximum + 1, integer_set.size, <unsigned int *>values.data.as_voidptr)
        if not integer_set.structure:
            raise MemoryError("Error when creating the inversion list")

        return integer_set

    def __repr__(self) -> str:
        """Return a string representation of self"""
        return cinversion_list.inversion_list_to_string(self.structure).decode()

    def __hash__(self) -> int: pass

    def __contains__(self, item: object) -> bool:
        """Check if item is contains in self """
        pass

    def __len__(self) -> int:
        """Return the number of element in self"""
        return self.size

    def __iter__(self) -> Iterator[int]:
        """Return IntegerSetIterator of self"""
        return IntegerSetIterator(self)

    def intervals(self) -> Iterator[Tuple[int, int]]:
        """Return IntegerSetIntervalIterator of self"""
        return IntegerSetInvervalIterator(self)

    def ranges(self) -> Iterator[range]:
        """Return IntegerSetRangeIterator of self"""
        return IntegerSetRangeIterator(self)

    def __eq__(self, other: object) -> bool:
        """Check the equality between self and other"""
        if type(self) == type(other):
            return cinversion_list.inversion_list_equal(self.structure, (<IntegerSet>other).structure)
        
        return False

    def __ne__(self, other: object) -> bool:
        """Check if self is strictly included in other"""
        if type(self) == type(other):
            return cinversion_list.inversion_list_not_equal(self.structure, (<IntegerSet>other).structure)
        
        return True

    def __lt__(self, other: "IntegerSet") -> bool:
        """Check if self is strictly included in other"""
        return cinversion_list.inversion_list_less(self.structure, (<IntegerSet>other).structure)

    def __le__(self, other: "IntegerSet") -> bool:
        """Check if self is included or is equal to other"""
        return cinversion_list.inversion_list_less_equal(self.structure, (<IntegerSet>other).structure)

    def __gt__(self, other: "IntegerSet") -> bool:
        """Check if self strictly includes other """
        return cinversion_list.inversion_list_greater(self.structure, (<IntegerSet>other).structure)

    def __ge__(self, other: "IntegerSet") -> bool:
        """Check if self includes or is equal to other"""
        return cinversion_list.inversion_list_greater_equal(self.structure, (<IntegerSet>other).structure)

    def isdisjoint(self, other: Iterable[int]) -> bool:
        """Check that 2 IntegerSet have no elements in common"""
        new_set = IntegerSet.from_iterable(other)
        return cinversion_list.inversion_list_disjoint(self.structure, (<IntegerSet>new_set).structure)

    def __and__(self, other: "IntegerSet") -> "IntegerSet":
        """Return an IntegerSet who is the intersection of self and other"""
        new_set: IntegerSet = IntegerSet.from_iterable(self)

        cdef cinversion_list.InversionList *new_structure = cinversion_list.inversion_list_intersection(new_set.structure, (<IntegerSet>other).structure, NULL)
        cinversion_list.inversion_list_destroy(new_set.structure)
        new_set.structure = new_structure

        return new_set

    def intersection(self, *others: Iterator[int]) -> "IntegerSet":
        """Return an IntegerSet who is the intersection of IntegerSet and all IntegerSet in arguments""" 
        new_set = IntegerSet.from_iterable(self)

        for other in others:
            new_set = new_set & IntegerSet.from_iterable(other)

        return new_set

    def __or__(self, other: "IntegerSet") -> "IntegerSet":
        """Return a new InversionList who is the union of self and other"""
        new_set: IntegerSet = IntegerSet.from_iterable(self)

        cdef cinversion_list.InversionList *new_structure = cinversion_list.inversion_list_union(new_set.structure, (<IntegerSet>other).structure, NULL)
        cinversion_list.inversion_list_destroy(new_set.structure)
        new_set.structure = new_structure

        return new_set

    def union(self, *others: Iterator[int]) -> "IntegerSet":
        """Return a new InversionList who is the union of set and all IntegerSet in arguments"""
        new_set = IntegerSet.from_iterable(self)

        for other in others:
            new_set = new_set | IntegerSet.from_iterable(other)

        return new_set

    def __sub__(self, other: "IntegerSet") -> "IntegerSet":
        """Return a new InversionList who is the difference between set and other"""
        new_set: IntegerSet = IntegerSet.from_iterable(self)

        cdef cinversion_list.InversionList *new_structure = cinversion_list.inversion_list_difference(new_set.structure, (<IntegerSet>other).structure, NULL)
        cinversion_list.inversion_list_destroy(new_set.structure)
        new_set.structure = new_structure

        return new_set

    def difference(self, *others: Iterator[int]) -> "IntegerSet":
        """Return a new InversionList who is the difference between set and all IntegerSet in arguments"""
        new_set = IntegerSet.from_iterable(self)

        for other in others:
            new_set = new_set - IntegerSet.from_iterable(other)

        return new_set

    def __xor__(self, other: "IntegerSet") -> "IntegerSet":
        """Check the set consisting of elements belonging to self and other but not belonging to the intersection of self and other"""
        new_set: IntegerSet = IntegerSet.from_iterable(self)

        cdef cinversion_list.InversionList *new_structure = cinversion_list.inversion_list_symmetric_difference(new_set.structure, (<IntegerSet>other).structure)
        cinversion_list.inversion_list_destroy(new_set.structure)
        new_set.structure = new_structure

        return new_set

    def symmetric_difference(self, other: Iterator[int]) -> "IntegerSet":
        """Check the set consisting of elements belonging to self and other but not belonging to the intersection of self and other"""
        return IntegerSet.from_iterable(self) ^ IntegerSet.from_iterable(other)
