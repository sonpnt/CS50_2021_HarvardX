#include <ctype.h>
#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

int main(int argc, string argv[])
{
    //check if user puts a key
    if (argc == 1)
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }
    //check if the key has exactly 26 charaters
    else if (strlen(argv[1]) != 26)
    {
        printf("Key must contain 26 characters.\n");
        return 1;
    }
    else
    {
        //check if the key has any repeated characters or non alphabet characters
        //use nested loop
        for (int i = 0; i < 26; i++)
        {
            for (int j = i + 1; j < 26; j++)
            {
                //check if the key contains any repeated characters
                if (toupper(argv[1][i]) == toupper(argv[1][j]))
                {
                    printf("Key must not contain repeated characters.\n");
                    return 1;
                }
            }
            ////check if the key contains any non alphabet characters
            if (toupper(argv[1][i]) < 65 || toupper(argv[1][i]) > 90)
            {
                printf("Key must contain only alphabetic characters.\n");
                return 1;
            }
        }
        //prompt user for plaintext
        char *s = get_string("plaintext: ");
        for (int i = 0; i < strlen(s); i++)
        {
            //for each alphabet charater, determine what letter it maps to
            //preserve case
            if (isupper(s[i]) != 0)
            {
                s[i] = toupper(argv[1][s[i] - 65]);
            }
            else if (islower(s[i]) != 0)
            {
                s[i] = tolower(argv[1][s[i] - 97]);
            }
        }
        //output ciphertext
        printf("ciphertext: %s\n", s);

    }
}