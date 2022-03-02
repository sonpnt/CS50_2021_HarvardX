import csv
import sys
import math


def main():
    if len(sys.argv) != 3:
        print("Usage: python dna.py data.csv sequence.txt")
        sys.exit(1)

    people = []
    # TODO: Read people into memory from file
    with open(sys.argv[1], "r") as file:
        reader = csv.DictReader(file)

        # converting the file to dictionary
        people = list(reader)
        # by first converting to list and get the first element
        list1 = people[0]  # this creates a mini dictionary
        # making a list from the keys of the list
        keys = list(list1.keys())  # this creates array
    # print(keys)

    # comment
    for i in range(len(people)):
        for j in range(1, len(keys), 1):
            people[i][keys[j]] = int(people[i][keys[j]])

    # read text file
    f = open(sys.argv[2], 'r')
    sequence = f.readline()
    # create dict counts to store results
    counts = {}
    # read the sequence

    keys.remove('name')
    for key in keys:
        old = ''
        counter = 0
        i = 0
        while i < len(sequence):
            new = sequence[i:i + len(key)]
            if new == key:
                i += len(key)
                if new == old:
                    counter += 1
                else:
                    counter = 1

                if new in counts:
                    counts[new] = max(counts[new], counter)
                else:
                    counts[new] = counter
                old = new
            else:
                counter = 0
                old = new
                i += 1
    nomatch = True
    for person in people:
        check = True
        for key in keys:
            if key in counts:
                if counts[key] != person[key]:
                    check = False
                    break
            else:
                check = False
                break
        if check:
            print(person['name'])
            nomatch = False
    if nomatch:
        print("No match")
    # print(people)
    # print(counts)


main()

