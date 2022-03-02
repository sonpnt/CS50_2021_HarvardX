import cs50


def main():
    # Promt user
    while True:
        i = cs50.get_int("Height: ")
        if i > 0 and i < 9:
            break

    # Call function
    pyramid(i, i)


def pyramid(i, j):
    if i > 1:
        pyramid(i - 1, j)
    print(" " * (j - i), end="")
    print('#' * i, end="")
    print("  ", end="")
    print('#' * i)


# call main
main()

