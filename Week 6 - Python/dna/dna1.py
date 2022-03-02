import csv
import sys

def main():

    # Ensure correct usage
    if len(sys.argv) != 3:
        sys.exit("Usage: python dna.py data.csv sequence.txt")

    # Read data into memory from csv file
    with open(sys.argv[1], "r") as csvfile:
        reader = csv.DictReader(csvfile)
        person = list(reader)
    
    # Read lookup info into memory from text file        
    with open(sys.argv[2], "r") as txtfile:
        sequence = txtfile.read()

    # Count number of consecutive repeats of each SRT in the lookup sequence
    counts = []
    for i in range(1, len(reader.fieldnames)): # i runs from 1st SRT to the last SRT
        strs = reader.fieldnames[i] 
        counts.append(0)
        
        # Loop through the sequence to look for consecutive repeats
        for j in range(1, len(sequence)):
            str_count = 0
            # When SRT found, start counting repeats
            if sequence[j:(j + len(strs))] == strs:
                k = 0
                # Everytime found a match, the checker will move k blocks (= length of the STR) from the original point
                while sequence[(j+k):(j+k+len(strs))] == strs:
                    str_count += 1
                    k += len(strs)
                # if str_count is larger than the current counts, update the counts
                if str_count > counts[i-1]:
                    counts[i-1]=str_count
    # Compare with information in the database
    for l in range(len(person)):
        matches = 0
        fuzzy_matches = 0
        for m in range(1, len(reader.fieldnames)):
            # Check if each STR of the lookup sequence matches each STR of each object in the database 
            if int(person[l][reader.fieldnames[m]]) == int(counts[m-1]):
                matches += 1
            if int(person[l][reader.fieldnames[m]]) - int(counts[m-1]) == 1:
                fuzzy_matches += 1
            # When number of STRs to be checked are 8, we need at least 7
            if len(reader.fieldnames) > 5:
                if matches + fuzzy_matches == (len(reader.fieldnames)-1):
                    print(person[l]['name'])
                    print(counts)
                    print(matches + fuzzy_matches)
                    sys.exit(0)
                    return
            elif len(reader.fieldnames) == 4:
                if matches == 3:
                    print(person[l]['name'])
                    print(counts)
                    print(matches)
                    sys.exit(0)
                    return
    print("No match")
    print(counts)

main()
