#ifndef PAC_IMG__
#define PAC_IMG__

#define sz8BIT sizeof(unsigned char)
#define sz16BIT sizeof(unsigned short)
#define sz32BIT sizeof(unsigned int)


typedef struct {
    int width, height, nbands;
    int pixel_size; /* in bytes */
    unsigned char *data;
} img_t;

img_t *img_create(int w, int h, int c, int pixel_size);
img_t *img_convert_type(img_t *img, int pixel_size);
void img_set_pixel(img_t *, int, int, int, unsigned int);
unsigned int img_get_pixel(img_t *, int, int, int);
int img_get_width(img_t *);
int img_get_height(img_t *);
int img_get_nbands(img_t *);
unsigned char *img_get_data(img_t *);
void img_free(img_t *i);

#endif
