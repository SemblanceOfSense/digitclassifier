// getmnist.h
#ifndef GETMNIST_H
#define GETMNIST_H

#include <stdint.h>
#include <stdio.h>

#include "consts.h"

#define IMAGE_SIZE NUM_ACTIVATION_NEURONS

typedef double Image[NUM_ACTIVATION_NEURONS];

typedef struct {
    Image image;
    uint8_t digit;
} MnistImage;

#ifdef __cplusplus
extern "C" {
#endif

int read_int32(FILE *fp);  // if you need it outside getmnist.c
MnistImage *load_mnist(const char *image_file, const char *label_file, int *count);

// if you need to copy the pixel array:
void imgcpy(double dest[IMAGE_SIZE], const double src[IMAGE_SIZE]);

#ifdef __cplusplus
}
#endif

#endif // GETMNIST_H
