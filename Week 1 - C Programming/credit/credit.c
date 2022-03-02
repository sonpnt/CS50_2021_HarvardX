#include <stdio.h>
#include <cs50.h>
#include <math.h>

//declare function
int sd(int num);

int main(void)
{
    //declare related variable
    long cardNo;
    long amex = 1;
    long maca = 1;
    long visa13 = 1;
    long visa16 = 1;
    bool validity = 0;
    int checksum = 0;

    //promt for card number
    cardNo = get_long("Card number: ");

    //do initial quick check
    long resid14 = (int)floor(cardNo / pow(10, 14));
    //1. check if the number of digits and the first 2 digits match amex (15 digits)
    if ((int)floor(cardNo / pow(10, 13)) == 34 || (int)floor(cardNo / pow(10, 13)) == 37)
    {
        maca = 0;
        visa13 = 0;
        visa16 = 0;
    }
    //2. check if the number of digits and the first 2 digits match maca (16 digits)
    else if (resid14 == 51 || resid14 == 52 || resid14 == 53 || resid14 == 54 || resid14 == 55)
    {
        amex = 0;
        visa13 = 0;
        visa16 = 0;
    }
    //3. check if the number of digits and the first 2 digits match visa (16 digits)
    else if ((int)floor(cardNo / pow(10, 15)) == 4)
    {
        amex = 0;
        maca = 0;
        visa13 = 0;
    }
    //4. check if the number of digits and the first 2 digits match visa (13 digits)
    else if ((int)floor(cardNo / pow(10, 12)) == 4)
    {
        amex = 0;
        maca = 0;
        visa16 = 0;
    }
    else
    {
        amex = 0;
        maca = 0;
        visa13 = 0;
        visa16 = 0;
    }
    //print temporary results for bug check
    printf("amex = %li\n", amex);
    printf("maca = %li\n", maca);
    printf("visa13 = %li\n", visa13);
    printf("visa16 = %li\n", visa16);
    //check validity
    if (amex == 1 || maca == 1 || visa13 == 1 || visa16 == 1)
    {
        long cardNo1 = cardNo;
        while (cardNo1 > 0)
        {
            int digit01 = cardNo1 % 10;
            int digit02 = fmod((cardNo1 - digit01) / 10, 10);
            cardNo1 = (cardNo1 - digit01 - 10 * digit02) / 100;
            checksum = checksum + digit01 + sd(2 * digit02);
        }
        validity = (checksum % 10 == 0);
    }
    //print outcome
    if (amex * validity == 1)
    {
        printf("AMEX\n");
    }
    else if (maca * validity == 1)
    {
        printf("MASTERCARD\n");
    }
    else if (visa13 * validity == 1 || visa16 * validity == 1)
    {
        printf("VISA\n");
    }
    else
    {
        printf("INVALID\n");
    }

}

//create sum of digit function
int sd(int num)
{
    int digit01 = num % 10;
    int digit02 = (num - digit01) / 10 % 10;
    int sd = digit01 + digit02;
    return sd;
}