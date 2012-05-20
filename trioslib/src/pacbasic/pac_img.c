#include <stdlib.h>
#include <pac_img.h>

img_t *img_create(int w, int h, int n, int sz) {
    int i, j, k;
    img_t *t = malloc(sizeof(img_t));
    t->width = w;
    t->height = h;
    t->nbands = n;
    t->pixel_size = sz;
    t->data = malloc(sz * w * h * n);
    if (t->data == NULL) {
        return (img_t *) pac_error(1, "Failed malloc for img->data");
    }
    for (i = 0; i < h; i++) {
        for (j = 0; j < w; j++) {
            for (k = 0; k < n; k++) {
                img_set_pixel(t, i, j, k, 0);
            }
        }
    }   
    return t;
}

void img_set_pixel(img_t *t, int i, int j, int k, unsigned int v) {
    if (t->pixel_size == sz8BIT) {
        unsigned char val = (unsigned char) v;
        t->data[k * t->width * t->height + i * t->width + j] = val;
    } else if (t->pixel_size == sz16BIT) {
        unsigned short val = (unsigned short) v;
        unsigned short *data = (unsigned short *) t->data;
        data[k * t->width * t->height + i * t->width + j] = val;
    } else if (t->pixel_size == sz32BIT) {
        unsigned int *data = (unsigned int *) t->data;
        data[k * t->width * t->height + i * t->width + j] = v;
    }
}

unsigned int img_get_pixel(img_t *t, int i, int j, int k) {
    if (t->pixel_size == sz8BIT) {
        return t->data[k * t->width * t->height + i * t->width + j];
    } else if (t->pixel_size == sz16BIT) {
        unsigned short *data = (unsigned short *) t->data;
        return data[k * t->width * t->height + i * t->width + j];
    } else if (t->pixel_size == sz32BIT) {
        unsigned int *data = (unsigned int *) t->data;
        return data[k * t->width * t->height + i * t->width + j];
    }
    return t->data[t->pixel_size * (k * t->width * t->height + i * t->width + j)];
}

int img_get_width(img_t *t) {
    return t->width;
}

int img_get_height(img_t *t) {
    return t->height;
}

int img_get_nbands(img_t *t) {
    return t->nbands;
}

int img_get_pixel_size(img_t *t) {
    return t->pixel_size;
}

unsigned char *img_get_data(img_t *t) {
    return t->data;
}

void img_free(img_t *t) {
    if (t != NULL) {
        if (t->data != NULL) 
            free(t->data);
        free(t);
    }
}

