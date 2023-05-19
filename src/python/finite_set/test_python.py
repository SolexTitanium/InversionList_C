import inversion_list

def test1():
    A = inversion_list.IntegerSet([(1, 10), (20, 25)])
    B = inversion_list.IntegerSet([(1, 10), (20, 25)])
    C = inversion_list.IntegerSet([(1, 10), (20, 25), (40, 100)])
    print(f"set A = {A}, size = {len(A)}")

    print("Ranges:")
    for r in A.ranges():
        print(f"\t{r}")

    print("Intervals:")
    for interval in A.intervals():
        print(f"\t{interval}")

    print("Iterator:")
    for value in A:
        print(f"\t{value}")
    
    print(f"A == B: {A == B}")
    print(f"A == 1: {A == 1}")
    print(f"A != C: {A != C}")
    print(f"A != False: {A != False}")

def test2():
    a = [1, 2, 3, 5, 7, 8, 9]
    b = [1, 2, 4, 5, 7, 8, 12, 592]

    set_a = inversion_list.IntegerSet.from_iterable(a)
    set_b = inversion_list.IntegerSet.from_iterable(b)

    print(set_a == set_b, set_a != set_b, set_a == set_a)

def test_less():
    a = inversion_list.IntegerSet.from_iterable([3, 4, 5, 8, 12, 13])
    b = inversion_list.IntegerSet.from_iterable([3, 4])
    c = inversion_list.IntegerSet.from_iterable([4, 5])
    d = inversion_list.IntegerSet.from_iterable([4])
    e = inversion_list.IntegerSet.from_iterable([3, 4, 12])

    f = inversion_list.IntegerSet.from_iterable([1])
    g = inversion_list.IntegerSet.from_iterable([1, 2, 3])
    h = inversion_list.IntegerSet.from_iterable([5, 6])
    i = inversion_list.IntegerSet.from_iterable([5, 6, 7, 8])
    j = inversion_list.IntegerSet.from_iterable([8, 9, 12, 13])
    k = inversion_list.IntegerSet.from_iterable([3, 4, 5, 17, 18])
    assert (b < a) == True
    assert (c < a) == True
    assert (d < a) == True
    assert (e < a) == True

    assert (f < a) == False
    assert (g < a) == False
    assert (h < a) == False
    assert (i < a) == False
    assert (j < a) == False
    assert (k < a) == False

if __name__ == "__main__":
    inversion_list.init_library()

    #test1()
    #test2()
    test_less()
    
    inversion_list.finish_library()