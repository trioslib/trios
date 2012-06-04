#ifndef PAC_IMG__
#define PAC_IMG__

/*! Size of an unsigned char.*/
#define sz8BIT sizeof(unsigned char) 
/*! Size of an unsigned short. */
#define sz16BIT sizeof(unsigned short)
/*! Size of an unsigned int. */
#define sz32BIT sizeof(unsigned int)

/*!
    Structure to hold an image. Supports images with many pixel size (unsigned char, unsigned short and unsigned int).
*/
typedef struct {
    int width, /*!< Width of the image. */
    height, /*!< Height of the image. */
    nbands; /*!< Number of bands of the image. */ 
    int pixel_size; /*!< Number of bytes of a pixel. */
    unsigned char *data; /*!< Pointer to the data of the image. */
} img_t;

img_t *img_create(int w, int h, int c, int pixel_size);
img_t *img_convert_type(img_t *img, int pixel_size);
void img_set_pixel(img_t *, int, int, int, unsigned int);
unsigned int img_get_pixel(img_t *, int, int, int);
int img_get_width(img_t *);
int img_get_height(img_t *);
int img_get_nbands(img_t *);
int img_get_pixel_size(img_t *t);
unsigned char *img_get_data(img_t *);
void img_free(img_t *i);

#endif
