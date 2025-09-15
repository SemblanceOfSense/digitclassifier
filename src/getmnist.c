// getmnist.c
#include "getmnist.h"
#include <stdlib.h>

#define MAGIC_IMAGE 2051
#define MAGIC_LABEL 2049

int read_int32(FILE *fp) {
    unsigned char b[4];
    if (fread(b, 1, 4, fp) != 4) { perror("read"); exit(1); }
    return (b[0] << 24) | (b[1] << 16) | (b[2] << 8) | b[3];
}

void imgcpy(double dest[IMAGE_SIZE], const double src[IMAGE_SIZE]) {
    for (int i = 0; i < IMAGE_SIZE; i++) dest[i] = src[i];
}

MnistImage *load_mnist(const char *image_file, const char *label_file, int *count) {
    FILE *img_fp = fopen(image_file, "rb");
    FILE *lbl_fp = fopen(label_file, "rb");
    if (!img_fp || !lbl_fp) { perror("fopen"); exit(1); }

    int magic_img = read_int32(img_fp);
    int num_images = read_int32(img_fp);
    int rows = read_int32(img_fp);
    int cols = read_int32(img_fp);

    int magic_lbl = read_int32(lbl_fp);
    int num_labels = read_int32(lbl_fp);

    if (magic_img != MAGIC_IMAGE || magic_lbl != MAGIC_LABEL || num_images != num_labels) {
        fprintf(stderr, "Invalid MNIST dataset\n"); exit(1);
    }
    if (rows * cols != IMAGE_SIZE) {
        fprintf(stderr, "Expected 28x28 images\n"); exit(1);
    }

    *count = num_images;
    MnistImage *dataset = malloc((size_t)num_images * sizeof *dataset);
    if (!dataset) { perror("malloc"); exit(1); }

    for (int i = 0; i < num_images; i++) {
        unsigned char label;
        if (fread(&label, 1, 1, lbl_fp) != 1) { perror("fread label"); exit(1); }
        dataset[i].digit = label;

        for (int j = 0; j < IMAGE_SIZE; j++) {
            unsigned char px;
            if (fread(&px, 1, 1, img_fp) != 1) { perror("fread pixel"); exit(1); }
            dataset[i].image[j] = (double)px / 255.0;
        }
    }

    fclose(img_fp);
    fclose(lbl_fp);
    return dataset;
}
