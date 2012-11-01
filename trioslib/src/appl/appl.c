#include <trios.h>

#include "local.h"
#include "../collec/collec_private.h"

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
    img_t *mask;
    if (f_mask == NULL) {
        mask = img_create(width, height, 1, sz8BIT);
        int i, j;
        for (i = 0; i < height; i++) {
            for (j = 0; j < width; j++) {
                img_set_pixel(mask, i, j, 0, 1);
            }
        }
    } else {
        mask = img_readPGM(f_mask);
    }
    put_border(win, mask);
    return mask;
}

