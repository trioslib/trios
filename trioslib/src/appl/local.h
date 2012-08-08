#ifndef LOCAL_H
#define LOCAL_H

/*!
  Reads a mask from a file or creates a new image with all pixels set to 1 if there is no mask.

  \param f_mask Path to the mask.
  \param width Width of the mask.
  \param height Height of the mask.
  \param win Window to use the mask with.
  \return Mask read from file or a new image with all pixels set to 1 if no file was given.
  */
img_t *set_mask(char *f_mask, int width, int height, window_t * win);

#endif // LOCAL_H
