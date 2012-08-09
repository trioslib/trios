#include <stdlib.h>
#include <trios_img.h>
#include <trios_error.h>

/*!
  Creates a img_t structure.

  \param w Width of the image.
  \param h Height of the image.
  \param n Number of bands of the image.
  \param sz Size of each pixel in the image. This parameter defines the maximum value of each pixel in the image. One of sz8BIT, sz16BIT or sz32BIT.
  \return The created image of NULL if the creation failed.
  */

img_t *img_create(int w, int h, int n, int sz) {
    int i, j, k;
    img_t *t = malloc(sizeof(img_t));
    t->width = w;
    t->height = h;
    t->nbands = n;
    t->pixel_size = sz;
    t->data = malloc(sz * w * h * n);
    if (t->data == NULL) {
        return (img_t *) trios_error(1, "Failed malloc for img->data");
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

/*!
  Converts an image to other image with the same content but different type.

  \param img Image to convert.
  \param pixel_size Size of the converted image.
  \return Converted image or NULL if failed.
  */
img_t *img_convert_type(img_t *img, int pixel_size) {
    int i, j, k, w, h, n;
    img_t *cvt;
    w = img_get_width(img);
    h = img_get_height(img);
    n = img_get_nbands(img);
    cvt = img_create(w, h, n, pixel_size);
    for (i = 0; i < h; i++) {
        for (j = 0; j < w; j++) {
            for (k = 0; k < n; k++) {
                unsigned int val = img_get_pixel(img, i, j, k);
                img_set_pixel(cvt, i, j, k, val);
            }
        }
    }
    return cvt;
}

/*!
  Sets the (i, j, k) pixel in the image as v.

  \param t Image to set the pixel.
  \param i Row of the pixel.
  \param j Column of the pixel.
  \param k Band of the pixel.
  \param v New value of the pixel.
  */
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

/*!
  Gets the value of the (i, j, k) pixel in the image.

  \param t Image.
  \param i Row of the pixel.
  \param j Column of the pixel.
  \param k Band of the pixel.
  */
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

/*!
  Returns the width of the image.

  \param t Image.
  \return Width of t.
  */
int img_get_width(img_t *t) {
    return t->width;
}

/*!
  Returns the height of the image.

  \param t Image.
  \return Height of t.
  */
int img_get_height(img_t *t) {
    return t->height;
}

/*!
  Returns the number of bands of the image.

  \param t Image.
  \return Number of bands of t.
  */
int img_get_nbands(img_t *t) {
    return t->nbands;
}

/*!
  Returns the pixel size of the image.

  \param t Image.
  \return Pixel size of t.
  */
int img_get_pixel_size(img_t *t) {
    return t->pixel_size;
}

/*!
  Returns the data pointer of the image.

  \param t Image.
  \return Data pointer of t.
  */
unsigned char *img_get_data(img_t *t) {
    return t->data;
}

/*!
  Frees the memory of the image.

  \param t Image.
  */
void img_free(img_t *t) {
    if (t != NULL) {
        if (t->data != NULL) 
            free(t->data);
        free(t);
    }
}

