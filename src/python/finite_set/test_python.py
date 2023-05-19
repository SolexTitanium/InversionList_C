import inversion_list

if __name__ == "__main__":
    inversion_list.init_library()

    A = inversion_list.IntegerSet([(1, 10), (20, 25)])
    print(A, f"size = {len(A)}")

    inversion_list.finish_library()