from ctypes import *
from typing import *

so_file = "./InversionList-0.0.1-Source/inversion-list.so"
inversion_functions = CDLL(so_file)


class IntegerSet(AbstractSet[int]):
    def __init__(
        self,
        intervals: Optional[Iterable[Tuple[int, int]]] = None,
    ) -> None:...

    @classmethod
    def from_iterable(
        cls,
        iterable: Optional[Iterable[int]] = None,
    ) -> "IntegerSet":
            return inversion_functions.inversion_list_create(iterable)

    def __repr__(self) -> str:
        ...

    def __hash__(self) -> int:
        ...

    def __contains__(self, item: object) -> bool:
        ...

    def __len__(self) -> int:
        ...


    def __iter__(self) -> Iterator[int]: return inversion_functions.inversion_list_iterator_next(self)


    def intervals(self) -> Iterator[Tuple[int, int]]:
        ...


    def ranges(self) -> Iterator[range]:
        ...


    def __eq__(self, other: object) -> bool: return inversion_functions.inversion_list_equal(self, object)


    def __ne__(self, other: object) -> bool: return inversion_functions.inversion_list_not_equal(self, object)
    
    def __lt__(self, other: "IntegerSet") -> bool: return inversion_functions.inversion_list_less(self,other)

    def __le__(self, other: "IntegerSet") -> bool: return inversion_functions.inversion_list_less_equal(self, other)

    def __gt__(self, other: "IntegerSet") -> bool: return inversion_functions.inversion_list_less_equal(self,other)

    def __ge__(self, other: "IntegerSet") -> bool: return inversion_functions.inversion_list_greater_equal(self,other)

    def isdisjoint(self, other: Iterable[int]) -> bool: return inversion_functions.inversion_list_disjoint(self,other)

    def __and__(self, other: "IntegerSet") -> "IntegerSet": ...

    def intersection(self, *others: Iterator[int]) -> "IntegerSet": ...

    def __or__(self, other: "IntegerSet") -> "IntegerSet": ...

    def union(self, *others: Iterator[int]) -> "IntegerSet": return inversion_functions.inversion_list_union(self,other)

    def __sub__(self, other: "IntegerSet") -> "IntegerSet": ...

    def difference(self, *others: Iterator[int]) -> "IntegerSet": return inversion_functions.inversion_list_difference(self,other)
         
    def __xor__(self, other: "IntegerSet") -> "IntegerSet": ...

    def symmetric_difference(self, other: Iterator[int]) -> "IntegerSet": return inversion_functions.inversion_list_symmetric_difference(self,other)

    def intervals(self) -> Iterator[Tuple[int, int]]: ...

    def ranges(self) -> Iterator[range]: ...


if __name__=="__main__":
    A=IntegerSet()
    print(A.__repr__())