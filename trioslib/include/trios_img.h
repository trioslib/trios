#ifndef TRIOS_IMG__
#define TRIOS_IMG__

#ifdef __cplusplus
extern "C" {
#endif


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


/*!
  Creates a img_t structure.

  \param w Width of the image.
  \param h Height of the image.
  \param n Number of bands of the image.
  \param sz Size of each pixel in the image. This parameter defines the maximum value of each pixel in the image. One of sz8BIT, sz16BIT or sz32BIT.
  \return The created image of NULL if the creation failed.
  */
img_t *img_create(int w, int h, int c, int pixel_size);


/*!
  Converts an image to other image with the same content but different type.

  \param img Image to convert.
  \param pixel_size Size of the converted image.
  \return Converted image or NULL if failed.
  */
img_t *img_convert_type(img_t *img, int pixel_size);


/*!
  Sets the (i, j, k) pixel in the image as v.

  \param t Image to set the pixel.
  \param i Row of the pixel.
  \param j Column of the pixel.
  \param k Band of the pixel.
  \param v New value of the pixel.
  */
void img_set_pixel(img_t *, int, int, int, unsigned int);


/*!
  Gets the value of the (i, j, k) pixel in the image.

  \param t Image.
  \param i Row of the pixel.
  \param j Column of the pixel.
  \param k Band of the pixel.
  */
unsigned int img_get_pixel(img_t *, int, int, int);


/*!
  Returns the width of the image.

  \param t Image.
  \return Width of t.
  */
int img_get_width(img_t *);


/*!
  Returns the height of the image.

  \param t Image.
  \return Height of t.
  */
int img_get_height(img_t *);


/*!
  Returns the number of bands of the image.

  \param t Image.
  \return Number of bands of t.
  */
int img_get_nbands(img_t *);


/*!
  Returns the pixel size of the image.

  \param t Image.
  \return Pixel size of t.
  */
int img_get_pixel_size(img_t *t);


/*!
  Returns the data pointer of the image.

  \param t Image.
  \return Data pointer of t.
  */
unsigned char *img_get_data(img_t *);


/*!
  Frees the memory of the image.

  \param t Image.
  */
void img_free(img_t *i);

#ifdef __cplusplus
}
#endif


#endif
