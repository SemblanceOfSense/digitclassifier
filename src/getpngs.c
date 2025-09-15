#include <png.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>

#include "getpngs.h"

PngError get_png(char* filename) {
    PngError png;

    // (Partially) Shamelessly stolen from stackoverflow
    // https://stackoverflow.com/a/65590152

    FILE* fp = fopen(filename, "r");
    char digit_name[32];
    get_parent_name(filename, digit_name, sizeof(digit_name));
    png.digit = atoi(digit_name);
    png.valid = 2;

    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    png_infop info_ptr = png_create_info_struct(png_ptr);
    png_init_io(png_ptr, fp);
    png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_IDENTITY, NULL);
    png_bytepp rows = png_get_rows(png_ptr, info_ptr);

    // Thank you to the amazing https://jeromebelleman.gitlab.io/posts/devops/libpng/

    for (int i = 0; i < ACTIVATIONS_HEIGHT; i++) {
        for (int j = 0; j < ACTIVATIONS_WIDTH * 3; j += 3) {
            png.image[i * ACTIVATIONS_HEIGHT + (j / 3)] = (double) rows[i][j] / (double) 255;
        }
    }

    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    fclose(fp);

    return png;
}

PngError* get_pngs(char* dirname) {
    static PngError pngs[70000];
    int i = 0;

    DIR* dp = opendir(dirname);
    struct dirent* entry;

    while ((entry = readdir(dp)) != NULL) {
        char path[1024];
        struct stat statbuf;

        // Skip "." and ".."
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        snprintf(path, sizeof(path), "%s/%s", dirname, entry->d_name);

        if (stat(path, &statbuf) == -1) {
            perror("stat");
            continue;
        }

        if (S_ISDIR(statbuf.st_mode)) {
            get_pngs(path);
        } else {
            pngs[i] = get_png(path);
            i++;
        }
    }

    closedir(dp);

    return pngs;
}

void imgcpy(Image dest, Image src) {
    for (int i = 0; i < NUM_ACTIVATION_NEURONS; i++) {
        dest[i] = src[i];
    }
}

// Chatgpted lolololol
void get_parent_name(const char *path, char *out, size_t outsz) {
    if (!out || outsz == 0) return;
    out[0] = '\0';

    // Find the parent directory component safely.
    const char *last_slash = strrchr(path, '/');
    if (!last_slash || last_slash == path) return;           // no parent
    // Skip the filename part to the previous slash
    const char *p = last_slash - 1;
    while (p > path && *p != '/') --p;
    const char *parent = (*p == '/') ? p + 1 : path;

    size_t len = (size_t)(last_slash - parent);               // length of parent name
    // Copy with bounds
    if (outsz > 1) {
        size_t n = len < (outsz - 1) ? len : (outsz - 1);
        memcpy(out, parent, n);
        out[n] = '\0';
    }
}
