#include <ctype.h>
#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

int main(void)
{
    //declare related variables
    int letters = 0;
    int words = 1;
    int sentences = 0;

    //prompt the user for a string of text
    string s = get_string("Text: ");

    //compute variables
    for (int i = 0; i < strlen(s); i++)
    {
        //count number of letters
        if (isupper(s[i]) || islower(s[i]))
        {
            letters = letters + 1;
        }
        //count number of words
        if (s[i] == ' ')
        {
            words = words + 1;
        }
        //count number of sentences
        if (s[i] == '.' || s[i] == '!' || s[i] == '?')
        {
            sentences = sentences + 1;
        }
    }
    //compute L, S, and index
    float L = 100.0 * letters / words;
    float S = 100.0 * sentences / words;
    int index = round(0.0588 * L - 0.296 * S - 15.8);

    //print output
    printf("%f\n", 0.0588 * L - 0.296 * S - 15.8);
    printf("%i\n", letters);
    printf("%i\n", words);
    printf("%i\n", sentences);
    printf("%f\n", L);
    printf("%f\n", S);
    if (index <= 0)
    {
        printf("Before Grade 1\n");
    }
    else if (index > 15)
    {
        printf("Grade 16+\n");
    }
    else
    {
        printf("Grade %i\n", index);
    }
}