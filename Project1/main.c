#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define STB_IMAGE_IMPLEMENTATION
#include "./headers/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "./headers/stb_image_write.h"

/**
 * Create a new 1-dimensional array with the given size
 * @param[in] _size the size of the array
 * @param[out] _ empty 1-dimensional array filled with 0
 */

unsigned char *uc_arrayNew_1d(int _size)
{
    return (unsigned char *)calloc(_size, sizeof(unsigned char));
}

/**
 * Background subtraction of two images
 * @param[in] image1 the first input image
 * @param[in] image2 the second input image
 * @param[in] width the width of the image
 * @param[in] height the height of the image
 * @param[in] channel the channel of the image
 * @param[out] _ the background subtraction of two images
 */

unsigned char *threshold_background_subtraction(unsigned char *image1, unsigned char *image2, int width, int height, int channel)
{
    unsigned char *result = uc_arrayNew_1d(width * height * channel);
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int check = 0;
            for (int k = 0; k < channel; k++)
            {
                if(abs(image1[i * width * channel + j * channel + k] - image2[i * width * channel + j * channel + k]) < 80)
                {
                    check++;
                }
            }
            for (int k = 0; k < channel; k++)
            {
                if(check == 3)
                {
                    result[i * width * channel + j * channel + k] = 0;
                }
                else
                {
                    result[i * width * channel + j * channel + k] = image2[i * width * channel + j * channel + k];
                }
            }
        }
    }
    return result;
}

/**
 * Combine two images
 * @param[in] image1 the first input image
 * @param[in] image2 the second input image
 * @param[in] image4 the second input image
 * @param[in] width the width of the image
 * @param[in] height the height of the image
 * @param[in] channel the channel of the image
 * @param[out] _ the combined image
 */

unsigned char *combine_image(unsigned char *image1, unsigned char *image2, unsigned char *image4, int width, int height, int channel)
{
    unsigned char *people = threshold_background_subtraction(image1, image2, width, height, channel);
    unsigned char *result = uc_arrayNew_1d(width * height * channel);
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            for (int k = 0; k < channel; k++)
            {
                result[i * width * channel + j * channel + k] = (image1[i * width * channel + j * channel + k] + image2[i * width * channel + j * channel + k] + image4[i * width * channel + j * channel + k]) / 3;
                if (people[i * width * channel + j * channel + k] == 0)
                {
                    people[i * width * channel + j * channel + k] = image4[i * width * channel + j * channel + k];
                }
            }
        }
    }
    return people;
}

int main()
{
    // declare variables
    int width, height, channel;
    char path_img1[] = "./images1/background.jpg";
    char path_img2[] = "./images1/foreground.jpg";
    char path_img3[] = "./images1/background_subtraction.jpg";
    char path_img4[] = "./images1/new_background.jpg";
    char path_img5[] = "./images1/combined_image.jpg";

    // read image
    unsigned char *image1 = stbi_load(path_img1, &width, &height, &channel, 0);
    unsigned char *image2 = stbi_load(path_img2, &width, &height, &channel, 0);
    unsigned char *image4 = stbi_load(path_img4, &width, &height, &channel, 0);
    if (image1 == NULL || image2 == NULL || image4 == NULL)
    {
        printf("\nError in loading the image\n");
        exit(1);
    }
    printf("Width: %d\nHeight: %d\nChannel: %d\n", width, height, channel);
    
    // background subtraction
    unsigned char *image3 = threshold_background_subtraction(image1, image2, width, height, channel);
    stbi_write_jpg(path_img3, width, height, channel, image3, width * channel);
    printf("New image saved to: %s\n", path_img3);

    // combine two images
    unsigned char *image5 = combine_image(image1, image2, image4, width, height, channel);
    stbi_write_jpg(path_img5, width, height, channel, image5, width * channel);
    printf("New image saved to: %s\n", path_img5);

}