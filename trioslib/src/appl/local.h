#ifndef LOCAL_H
#define LOCAL_H

/*!
  Reads a mask from a file or creates a new image with all pixels set to 1 if there is no mask.

  \param f_mask Path to the mask.
  \param width Width of the mask.
  \param height Height of the mask.
  \param win Window to use the mask with.
  \param alloc_mask Out value to indicate that a new image was allocated.
  \return Mask read from file or a new image with all pixels set to 1 if no file was given.
  */
img_t *set_mask(char *f_mask, int width, int height, window_t * win, int *alloc_mask);

img_t *multi_level_apply_level_bb(multi_level_operator_t *mop, int level, int op, img_t **inputs, img_t *mask);

img_t *multi_level_apply_level_gx(multi_level_operator_t *mop, int level, int op, img_t **inputs, img_t *mask);

#endif
