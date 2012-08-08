#include <trios.h>

#include "local.h"

/*!
  Reads a mask from a file or creates a new image with all pixels set to 1 if there is no mask.

  \param f_mask Path to the mask.
  \param width Width of the mask.
  \param height Height of the mask.
  \param win Window to use the mask with.
  \return Mask read from file or a new image with all pixels set to 1 if no file was given.
  */
img_t *set_mask(char *f_mask, int width, int height, window_t * win)
{
    if (f_mask == NULL) {
        img_t *mask = img_create(width, height, 1, sz8BIT);
        int i, j;
        for (i = 0; i < height; i++) {
            for (j = 0; j < width; j++) {
                img_set_pixel(mask, i, j, 0, 1);
            }
        }
        return mask;
    } else {
        return img_readPGM(f_mask);
    }
}

