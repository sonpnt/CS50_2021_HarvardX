import cs50

# comment
s = cs50.get_string("Text: ")
letters = 0
words = 1
sentences = 0

# comment
for i in range(len(s)):
    if s[i].isalpha():
        letters += 1
    if s[i] == ' ':
        words += 1
    if s[i] in ['.', '!', '?']:
        sentences += 1

# comment
L = 100.0 * letters / words
S = 100.0 * sentences / words
index = round(0.0588 * L - 0.296 * S - 15.8)

# comment
if index < 0:
    print("Before Grade 1")
elif index > 15:
    print("Grade 16+")
else:
    print("Grade " + str(index))
