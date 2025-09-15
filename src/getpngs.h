#include "consts.h"

#ifndef GETPNGS
#define GETPNGS
void get_parent_name(const char *path, char *out, size_t outsz);

typedef double Image[NUM_ACTIVATION_NEURONS];
typedef struct {
    Image image;
    int digit;
    int error_code;
    int valid;
} PngError;
#endif
