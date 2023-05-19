import inversion_list

if __name__ == "__main__":
    inversion_list.init_library()

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
    
    inversion_list.finish_library()