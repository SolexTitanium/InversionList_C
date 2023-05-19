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
    cdef cinversion_list.InversionListCoupleIterator *iterator

    def __init__(self, set: IntegerSet):
        self.iterator = cinversion_list.inversion_list_couple_iterator_create(set.structure)
    
    def __dealloc__(self):
        cinversion_list.inversion_list_couple_iterator_destroy(self.iterator)
    
    def __iter__(self):
        return self
    
    def __next__(self):
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
    cdef cinversion_list.InversionListCoupleIterator *iterator

    def __init__(self, set: IntegerSet):
        self.iterator = cinversion_list.inversion_list_couple_iterator_create(set.structure)
    
    def __dealloc__(self):
        cinversion_list.inversion_list_couple_iterator_destroy(self.iterator)
    
    def __iter__(self):
        return self
    
    def __next__(self):
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
        self.iterator = cinversion_list.inversion_list_iterator_create(set.structure)
    
    def __dealloc__(self):
        cinversion_list.inversion_list_iterator_destroy(self.iterator)
    
    def __iter__(self):
        return self
    
    def __next__(self):
        if cinversion_list.inversion_list_iterator_valid(self.iterator):
            value = cinversion_list.inversion_list_iterator_get(self.iterator)

            cinversion_list.inversion_list_iterator_next(self.iterator)

            return value
        else:
            raise StopIteration

# The use of class IntegerSet(AbstractSet[int]) cause an error
cdef class IntegerSet:
    cdef cinversion_list.InversionList *structure
    cdef unsigned int size

    def __init__(
        self,
        intervals: Optional[Iterable[Tuple[int, int]]] = None,
    ) -> None:
        cdef array.array values = array.array('I', [])

        if intervals is not None:
            for interval in intervals:
                for value in range(interval[0], interval[1]):
                    values.append(value)
            
            maximum = max(values)
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
        integer_set = IntegerSet()
        cdef array.array values = array.array('I', [])

        if iterable is not None:
            for value in iterable:
                values.append(value)
            
            maximum = max(values)
        else:
            maximum = 0
        
        integer_set.size = len(values)
        cinversion_list.inversion_list_destroy(integer_set.structure)
        integer_set.structure = cinversion_list.inversion_list_create(maximum + 1, integer_set.size, <unsigned int *>values.data.as_voidptr)
        if not integer_set.structure:
            raise MemoryError("Error when creating the inversion list")

        return integer_set

    def __repr__(self) -> str:
        return cinversion_list.inversion_list_to_string(self.structure).decode()

    def __hash__(self) -> int: pass

    def __contains__(self, item: object) -> bool: pass

    def __len__(self) -> int: return self.size

    def __iter__(self) -> Iterator[int]:
        return IntegerSetIterator(self)

    def intervals(self) -> Iterator[Tuple[int, int]]:
        return IntegerSetInvervalIterator(self)

    def ranges(self) -> Iterator[range]:
        return IntegerSetRangeIterator(self)

    def __eq__(self, other: object) -> bool:
        if type(self) == type(other):
            return cinversion_list.inversion_list_equal(self.structure, (<IntegerSet>other).structure)
        
        return False
        

    def __ne__(self, other: object) -> bool:
        if type(self) == type(other):
            return cinversion_list.inversion_list_not_equal(self.structure, (<IntegerSet>other).structure)
        
        return True

    def __lt__(self, other: "IntegerSet") -> bool:
        if type(self) == type(other):
            return cinversion_list.inversion_list_less(self.structure, (<IntegerSet>other).structure)
        
        return False

    def __le__(self, other: "IntegerSet") -> bool:
        if type(self) == type(other):
            return cinversion_list.inversion_list_less_equal(self.structure, (<IntegerSet>other).structure)
        
        return False

    def __gt__(self, other: "IntegerSet") -> bool:
        if type(self) == type(other):
            return cinversion_list.inversion_list_greater(self.structure, (<IntegerSet>other).structure)
        
        return False

    def __ge__(self, other: "IntegerSet") -> bool:
        if type(self) == type(other):
            return cinversion_list.inversion_list_greater_equal(self.structure, (<IntegerSet>other).structure)
        
        return False

    def isdisjoint(self, other: Iterable[int]) -> bool: pass

    def __and__(self, other: "IntegerSet") -> "IntegerSet": pass

    def intersection(self, *others: Iterator[int]) -> "IntegerSet": pass

    def __or__(self, other: "IntegerSet") -> "IntegerSet": pass

    def union(self, *others: Iterator[int]) -> "IntegerSet": pass

    def __sub__(self, other: "IntegerSet") -> "IntegerSet": pass

    def difference(self, *others: Iterator[int]) -> "IntegerSet": pass

    def __xor__(self, other: "IntegerSet") -> "IntegerSet": pass

    def symmetric_difference(self, other: Iterator[int]) -> "IntegerSet": pass

