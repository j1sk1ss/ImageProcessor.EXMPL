#include "../include/filters.h"


static int __filter(bitmap_t* bmp, int (*filter)(palette_t*, unsigned char), unsigned char t) {
    if (!bmp || !filter) return -10;
    for (int i = 0; i < bmp->iheader.biHeight; i++) {
        for (int j = 0; j < bmp->iheader.biWidth; j++) {
            palette_t plt;
            int get_res = BMP_get_pixel_24(j, i, bmp, &plt);
            if (get_res != 1) {
                return get_res;
            }

            filter(&plt, t);

            int set_res = BMP_set_pixel_24(j, i, bmp, &plt);
            if (set_res != 1) {
                return set_res * 10;
            }
        }
    }

    return 1;
}

static int __gray_scale(palette_t* plt, unsigned char t) {
    int gray = 0.299 * plt->r + 0.587 * plt->g + 0.114 * plt->b;
    plt->r = plt->g = plt->b = gray;
    return 1;
}

static int __color(palette_t* plt, unsigned char t) {
    if (t == 0) { // r
        plt->g = 0x00;
        plt->b = 0x00;
    }
    else if (t == 1) { // g
        plt->r = 0x00;
        plt->b = 0x00;
    }
    else if (t == 2) { // b
        plt->r = 0x00;
        plt->g = 0x00;
    }

    return 1;
}

static int __negative(palette_t* plt, unsigned char t) {
    plt->r = 0xFF - plt->r;
    plt->g = 0xFF - plt->g;
    plt->b = 0xFF - plt->b;
    return 1;
}

static int __threshold(palette_t* plt, unsigned char t) {
    int gray = plt->r;
    if (gray > t) plt->r = plt->g = plt->b = 0xFF;
    else plt->r = plt->g = plt->b = 0x00;
    return 1;
}

int FILTER_grayscale(bitmap_t* bmp) {
    return __filter(bmp, __gray_scale, 0x00);
}

int FILTER_color(bitmap_t* bmp, int clr) {
    return __filter(bmp, __color, (char)clr);
}

int FILTER_negative(bitmap_t* bmp) {
    return __filter(bmp, __negative, 0x00);
}

int FILTER_threshold(bitmap_t* bmp, unsigned char t) {
    return __filter(bmp, __threshold, t);
}
