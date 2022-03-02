#include <cs50.h>
#include <stdio.h>

int main(void)
{
    //declare elements of the population
    int start;
    int end;
    int born;
    int pass;
    int n = 0;
    // TODO: Prompt for start size
    do
    {
        start = get_int("Start size: ");
    }
    while (start < 9);

    // TODO: Prompt for end size
    do
    {
        end = get_int("End size: ");
    }
    while (end < start);

    // TODO: Calculate number of years until we reach threshold
    if (start == end)
    {
        printf("Years: 0");
    }
    else
    {
        do
        {
            born = start / 3;
            pass = start / 4;
            start = start + born - pass;
            n++;
        }
        while (start < end);
    }

    // TODO: Print number of years
    printf("Years: %i\n", n);
}