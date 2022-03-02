def main():
    cardno = input("Card number: ")
    number = int(cardno)

    # Comment
    luhn = first_con(cardno)
    dig13 = len(cardno) == 13
    dig15 = len(cardno) == 15
    dig16 = len(cardno) == 16
    start2 = cardno[0] + cardno[1]
    startAmex = start2 in ['34', '37']
    startMaca = start2 in ['51', '52', '53', '54', '55']
    startVisa = cardno[0] == '4'

    # Comment
    if luhn:
        if dig15 and startAmex:
            print("AMEX")
        elif dig16 and startMaca:
            print("MASTERCARD")
        elif (dig13 or dig16) and startVisa:
            print("VISA")
        else:
            print("INVALID")
    else:
        print("INVALID")

# Comment


def first_con(cardno):
    length = len(cardno)
    # print(length)
    luhn = 0
    for i in range(0, length, 2):
        # print("i = " + str(i))
        luhn += int(cardno[length - i - 1])
    for i in range(1, length, 2):
        luhn += productdig(int(cardno[length - i - 1]))
    if luhn % 10 == 0:
        return True
    else:
        return False

# comment


def productdig(i):
    total = 0
    s = str(i * 2)
    if i > 4:
        total += (int(s[0]) + int(s[1]))
    else:
        total += i * 2
    return total


# Comment
main()