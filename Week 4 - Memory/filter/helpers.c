#include "helpers.h"
#include "math.h"

void swap(BYTE *a, BYTE *b);
// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int tmp = round((image[i][j].rgbtBlue + image[i][j].rgbtGreen + image[i][j].rgbtRed) / 3.0);
            image[i][j].rgbtBlue = tmp;
            image[i][j].rgbtGreen = tmp;
            image[i][j].rgbtRed = tmp;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    //initiate mid-point
    int mid = width / 2;
    int tmp;

    for (int i = 0; i < height; i ++)
    {
        for (int j = 0; j < mid; j++)
        {
            swap(&image[i][j].rgbtRed, &image[i][width - j - 1].rgbtRed);
            swap(&image[i][j].rgbtGreen, &image[i][width - j - 1].rgbtGreen);
            swap(&image[i][j].rgbtBlue, &image[i][width - j - 1].rgbtBlue);
            /*reflect wrt Blue
            tmp = image[i][j].rgbtBlue;
            image[i][j].rgbtBlue = image[i][width - j - 1].rgbtBlue;
            image[i][width - j - 1].rgbtRed = tmp;
            */
        }
    }
    return;
}
//Create swap function
void swap(BYTE *a, BYTE *b)
{
    BYTE tmp = *a;
    *a = *b;
    *b = tmp;
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE tmp[height][width];
    //Create another version of the image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            tmp[i][j] = image[i][j];
        }
    }

    //Blur the original image using input from the copy version
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            //Initiate the sum of each color and counter
            int red = 0;
            int green = 0;
            int blue = 0;
            int counter = 0;

            //Initiate location of the surrounding pixels
            int a = 1;  //up by 1
            int b = 1;  //down by 1
            int c = 1;  //left by 1
            int d = 1;  //right by 1
            //Check if a pixel is on the edges of the photo
            //1. Vertical
            if (i == 0)
            {
                a = 0;
            }
            else if (i == height - 1)
            {
                b = 0;
            }
            //2. Horizontal
            if (j == 0)
            {
                c = 0;
            }
            else if (j == width - 1)
            {
                d = 0;
            }

            //Loop through all pixels in the convolution to calculate the sum
            for (int m = -a; m < b + 1; m++)
            {
                for (int n = -c; n < d + 1; n++)
                {
                    red += tmp[i + m][j + n].rgbtRed;
                    blue += tmp[i + m][j + n].rgbtBlue;
                    green += tmp[i + m][j + n].rgbtGreen;
                    counter++;
                }
            }
            //The center pixel equals the average
            image[i][j].rgbtBlue = round(blue / (float)counter);
            image[i][j].rgbtGreen = round(green / (float)counter);
            image[i][j].rgbtRed = round(red / (float)counter);
        }
    }
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{

    //Create a temporary image with a one-pixel-wide black frame
    RGBTRIPLE tmp[height + 2][width + 2];
    for (int i = 0; i < height + 2; i++)
    {
        tmp[i][0].rgbtRed = 0;
        tmp[i][0].rgbtBlue = 0;
        tmp[i][0].rgbtGreen = 0;
        tmp[i][width + 1].rgbtRed = 0;
        tmp[i][width + 1].rgbtBlue = 0;
        tmp[i][width + 1].rgbtGreen = 0;
    }
    for (int i = 0; i < width + 2; i++)
    {
        tmp[0][i].rgbtRed = 0;
        tmp[0][i].rgbtBlue = 0;
        tmp[0][i].rgbtGreen = 0;
        tmp[height + 1][i].rgbtRed = 0;
        tmp[height + 1][i].rgbtBlue = 0;
        tmp[height + 1][i].rgbtGreen = 0;
    }
    //copy original image to the temporary image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            tmp[i + 1][j + 1].rgbtRed = image[i][j].rgbtRed;
            tmp[i + 1][j + 1].rgbtBlue = image[i][j].rgbtBlue;
            tmp[i + 1][j + 1].rgbtGreen = image[i][j].rgbtGreen;
        }
    }
    //Vertical kernel
    int Gy[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};
    //Horizontal kernel
    int Gx[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};

    //Transform the image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            //Initiate the sum of each color and X-Y
            int redX = 0;
            int greenX = 0;
            int blueX = 0;
            int redY = 0;
            int greenY = 0;
            int blueY = 0;
            int sumX[3] = {0, 0, 0};
            int sumY[3] = {0, 0, 0};

            //Compute Gx
            for (int m = 0; m < 3; m++)
            {
                for (int n = 0; n < 3; n++)
                {
                    int num[3] = {tmp[i + m][j + n].rgbtRed, tmp[i + m][j + n].rgbtGreen, tmp[i + m][j + n].rgbtBlue};
                    for (int k = 0; k < 3; k++)
                    {
                        sumX[k] += num[k] * Gx[m][n];
                        sumY[k] += num[k] * Gy[m][n];
                    }
                }
            }

            int color[3];
            for (int t = 0; t < 3; t++)
            {
                color[t] = round(sqrt(sumX[t] * (float)sumX[t] + sumY[t] * sumY[t]));
                if (color[t] > 255)
                {
                    color[t] = 255;
                }
            }

            //Update temporary image to the main image
            image[i][j].rgbtRed = color[0];
            image[i][j].rgbtBlue = color[2];
            image[i][j].rgbtGreen = color[1];
        }
    }

    return;
}
