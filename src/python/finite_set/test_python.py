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

def test_disjoint():
    a = [1, 2, 3, 5, 7, 8, 9]
    b = [1, 2, 3, 5, 7, 8, 9]
    c = [10, 20, 4, 6, 11, 12, 13]
    d = [6, 20, 4, 1, 11, 12, 13]

    set_a = inversion_list.IntegerSet.from_iterable(a)
    set_b = inversion_list.IntegerSet.from_iterable(b)
    set_c = inversion_list.IntegerSet.from_iterable(c)
    set_d = inversion_list.IntegerSet.from_iterable(d)

    # assert(inversion_list_disjoint(seta,setb) == false);
    # assert(inversion_list_disjoint(seta,setc) == true);
    # assert(inversion_list_disjoint(seta,setd) == false);

    assert set_a.isdisjoint(set_b) == False
    assert set_a.isdisjoint(set_c) == True
    assert set_a.isdisjoint(set_d) == False

    assert(set_a.isdisjoint(a) == False)
    assert(set_a.isdisjoint(c) == True)
    assert(set_a.isdisjoint(d) == False)

def test_intersection():
    a = [1, 2, 3]
    b = [1]
    c = [1, 2, 3, 7, 8, 9, 10, 12, 13, 14]
    d = [2, 3, 4, 5, 8, 9, 10, 11]
    e = []

    set_a = inversion_list.IntegerSet.from_iterable(a)
    set_b = inversion_list.IntegerSet.from_iterable(b)
    set_c = inversion_list.IntegerSet.from_iterable(c)
    set_d = inversion_list.IntegerSet.from_iterable(d)
    set_e = inversion_list.IntegerSet.from_iterable(e)

    assert(set_a.intersection(set_a) == set_a)
    assert(set_a.intersection(set_b) == set_b)
    assert(set_c.intersection(set_d) == inversion_list.IntegerSet([(2, 4), (8, 11)]))
    assert(set_e.intersection(set_e) == inversion_list.IntegerSet())

    # Second test

    a = [6, 7, 8, 9, 20, 21, 22, 23]
    b = [1, 2, 3, 16, 17, 18, 19, 20, 21]
    c = [8, 9, 10, 11, 12, 13, 19, 20, 23, 24, 25]

    set_a = inversion_list.IntegerSet.from_iterable(a)
    set_b = inversion_list.IntegerSet.from_iterable(b)
    set_c = inversion_list.IntegerSet.from_iterable(c)

    assert(set_a.intersection() == set_a)
    assert(set_b.intersection(set_a) == inversion_list.IntegerSet([(20, 22)]))
    assert(set_a.intersection(set_c, set_b) == inversion_list.IntegerSet([(20, 21)]))

if __name__ == "__main__":
    inversion_list.init_library()

    #test1()
    #test2()
    test_less()
    test_disjoint()
    test_intersection()
    
    inversion_list.finish_library()