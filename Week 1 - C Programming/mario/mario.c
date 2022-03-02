 8#include <stdio.h>
#include <cs50.h>

int main(void)
{
    //declare elements of the game
    int height;
    int h = 0;

    //prompt for height (between 1 and 8)
    do
    {
        height = get_int("Height: ");
    }
    while (height < 1 || height > 8);

    //create the pyramids
    while (h < height)
    {
        //create space between the first pyramid and the left screen margin
        int space1 = 0;
        while (space1 < height - h - 1)
        {
            space1++;
            printf(" ");
        }

        //create the first pyramid
        int hash1 = 0;
        while (hash1 < h + 1)
        {
            hash1++;
            printf("#");
        }

        //create space between two pyramids
        printf("  ");

        //create the second pyramid
        int hash2 = 0;
        while (hash2 < h + 1)
        {
            hash2++;
            printf("#");
        }

        printf("\n");
        h++;
    }


}
