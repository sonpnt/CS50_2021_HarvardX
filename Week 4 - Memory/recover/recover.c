#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int main(int argc, char *argv[])
{
    //Check if the command-line is correct
    if (argc != 2)
    {
        printf("Usage: ./recover image\n");
        return 1;
    }

    // Number of bytes in a block
    const int BLOCK_SIZE = 512;

    // Open input file
    FILE *infile = fopen(argv[1], "r");
    if (infile == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", argv[1]);
        return 1;
    }

    //Initiate output file
    FILE *outptr = fopen("000.jpg", "w");
    if (outptr == NULL)
    {
        fclose(infile);
        fprintf(stderr, "Could not create 000.jpg.\n");
        return 1;
    }


    //File name
    char outfile[8];

    // Create a buffer to copy data
    uint8_t buffer[512];

    // Open a random file
    int counter = -1;

    // While not end of file
    while (fread(buffer, sizeof(uint8_t), 512, infile) == 512)
    {
        // Check if this block marks the beginning of a new picture
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff)
        {
            // Increase file number
            counter += 1;
            // Output file name
            sprintf(outfile, "%03i.jpg", counter);
            printf("Open file %s\n", outfile);

            // Close current output file
            fclose(outptr);

            // Open output file
            outptr = fopen(outfile, "w");
            if (outptr == NULL)
            {
                fclose(infile);
                fprintf(stderr, "Could not create %s.\n", outfile);
                return 1;
            }


            // Write buffer to new file
            fwrite(buffer, sizeof(uint8_t) * 512, 1, outptr);
        }
        else if (counter >= 0)
        {
            // Write buffer to current file
            fwrite(buffer, sizeof(uint8_t) * 512, 1, outptr);
        }
    }
    // Close files
    fclose(outptr);
    fclose(infile);
}