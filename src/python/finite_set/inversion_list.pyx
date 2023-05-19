# distutils: include_dirs = ../../../InversionList-0.0.1-Source
# distutils: sources = ../../../InversionList-0.0.1-Source/inversion-list.c

cimport cinversion_list

from typing import Optional, Iterable, Tuple, Iterator, AbstractSet
from cpython cimport array
import array

def init_library() -> bool:
    return cinversion_list.inversion_list_init()

def finish_library() -> bool:
    return cinversion_list.inversion_list_finish()

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

        self.size = len(values)
        self.structure = cinversion_list.inversion_list_create(max(values) + 1, self.size, <unsigned int *>values.data.as_voidptr)
        if not self.structure:
            raise MemoryError("Error when creating the inversion list")

    @classmethod
    def from_iterable(
        cls,
        iterable: Optional[Iterable[int]] = None, 
    ) -> "IntegerSet": pass

    def __repr__(self) -> str: return cinversion_list.inversion_list_to_string(self.structure).decode()
    def __hash__(self) -> int: pass
    def __contains__(self, item: object) -> bool: pass
    def __len__(self) -> int: return self.size
    def __iter__(self) -> Iterator[int]: pass
    def intervals(self) -> Iterator[Tuple[int, int]]: pass
    def ranges(self) -> Iterator[range]: pass
    def __eq__(self, other: object) -> bool: pass
    def __ne__(self, other: object) -> bool: pass
    def __lt__(self, other: "IntegerSet") -> bool: pass
    def __le__(self, other: "IntegerSet") -> bool: pass
    def __gt__(self, other: "IntegerSet") -> bool: pass
    def __ge__(self, other: "IntegerSet") -> bool: pass
    def isdisjoint(self, other: Iterable[int]) -> bool: pass
    def __and__(self, other: "IntegerSet") -> "IntegerSet": pass
    def intersection(self, *others: Iterator[int]) -> "IntegerSet": pass
    def __or__(self, other: "IntegerSet") -> "IntegerSet": pass
    def union(self, *others: Iterator[int]) -> "IntegerSet": pass
    def __sub__(self, other: "IntegerSet") -> "IntegerSet": pass
    def difference(self, *others: Iterator[int]) -> "IntegerSet": pass
    def __xor__(self, other: "IntegerSet") -> "IntegerSet": pass
    def symmetric_difference(self, other: Iterator[int]) -> "IntegerSet": pass
    def intervals(self) -> Iterator[Tuple[int, int]]: pass
    def ranges(self) -> Iterator[range]: pass
