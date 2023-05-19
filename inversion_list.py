import ctypes
from typing import *

so_file = "./InversionList-0.0.1-Source/inversion-list.so"
inversion_functions = ctypes.CDLL(so_file)



class IntegerSet(AbstractSet[int]):
    #def structure
    _fields_ = [
        ('capacity', ctypes.c_int),
        ('support', ctypes.c_int),
        ('size', ctypes.c_size_t),
        ('couples',ctypes.c_int)
    ]
    def __init__(
        self,
        intervals: Optional[Iterable[Tuple[int, int]]] = None,
    ) -> None:inversion_functions.inversion_list_init()

    @classmethod
    #dans from iterable conversion du paramètres iterable en truc compréhensible par du C 
    #inversion liste create fonctionne mais renvoie int 
    #donc à partir de lign 32 erreur car pas d'attribut 
    def from_iterable(
        cls,
        iterable: Optional[Iterable[int]] = None
    ) -> "IntegerSet":
            ctypes.c_int_array = ctypes.c_int * len(iterable)
            A=inversion_functions.inversion_list_create(100000,len(iterable),ctypes.c_int_array(*iterable))
            self.support=A.inversion_list_support
            self.capacity=100000
            self.couples=inversion_functions.inversion_list_couples(A)
            self.size=inversion_functions.inversion_list_size(A)

            return self

    def __repr__(self) -> str:return inversion_functions.inversion_list_to_string(self)

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
    A.__init__()

    B=[1, 2, 3, 5, 7, 8, 9]
    A.__init__()
    A.from_iterable(B)
    print("A=")
    print(A.__repr__())