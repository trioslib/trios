#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

#include <pac_img.h>
#include <pac_error.h>

void removeComments(FILE *f) {
    int c;
    do {
        c = fgetc(f);
    } while (isspace(c) != 0);
    if (c == '#') {
        do {
            c = fgetc(f);
        } while (c != '\n');
    } else {
        ungetc(c, f);
    }
}

img_t *img_readPGM(char *filename) {
    int w, h, val, maxval;
    int i, j, k;
    char magic1, magic2;
    img_t *img;
    FILE *f = fopen(filename, "r");
    magic1 = fgetc(f); magic2 = fgetc(f);
    if (tolower(magic1) == 'p' && magic2 == '5') {
        removeComments(f);
        fscanf(f, "%d %d %d", &w, &h, &maxval);
        fgetc(f);
        if (maxval > 255) {
            return (img_t *) pac_error(1, "16-bit images no supported");
        }
        img = img_create(w, h, 1, sz8BIT);
        for (i = 0; i < h; i++) {
            for (j = 0; j < w; j++) {
                val = fgetc(f);
                img_set_pixel(img, i, j, 0, val);
            }
        }        
        fclose(f);
        return img;
    } else {
        return (img_t *) pac_error(1, "img_readPNM: File not supported");
    }
}


void img_writePGM(char *filename, img_t *img) {
        FILE *f = fopen(filename, "w");
        int i, j, w, h;
        if (f == NULL) {
            pac_error(1, "img_writePGM: Cannot open file");
            return;
        }
        if (img == NULL) {
            pac_error(1, "img_writePGM: NULL image");
            return;
        }
        w = img_get_width(img);
        h = img_get_height(img);
        fprintf(f, "P5\n%d %d\n%d\n", w, h, 255);
        for (i = 0; i < h; i++) {
            for (j = 0; j < w; j++) {
                fputc(img_get_pixel(img, i, j, 0), f);
            }
        }
        fputc('\n', f);
        fclose(f);
}

