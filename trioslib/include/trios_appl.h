#ifndef _trios_appl_h_
#define _trios_appl_h_

#include <trios_img.h>
#include <trios_itv.h>
#include <trios_learn.h>
#include "trios_mtm.h"
#include "trios_apert.h"

#ifdef __cplusplus
extern "C" {
#endif


/*!
    Applies an operator in an image. This functions reads the structures from the disk.

    \param f_appl Input image filename.
    \param f_basis Basis file.
    \param cv Center value.
    \param hash_flag Switch between normal and hash application.
    \param on_value Value of the output pixels.
    \param f_out Output image filename.
    \return 1 on success. 0 on failure.
 */
int  lpapplic_disk(char *f_appl, char *f_basis, char *f_mask,  int  cv, int  hash_flag, int  on_value, char *f_out );

/*!
 * Applies an operator in an image using in memory structures.
 * \param input Input image.
 * \param op Trained operator.
 * \param win Window of the trained operator.
 * \param mask Mask image.
 * \param cv Center value.
 * \param hash Switch between normal and hash application.
 * \param on_value Value of the output pixels.
 * \return
 */
img_t *lpapplic_memory(img_t *input, itv_t *op, window_t *win, img_t *mask, int cv, int hash, int on_value);


img_t *lapplyGG_memory(img_t *input, dTree *tree, window_t *win);
img_t *lapplyWK_memory(img_t *input, dTree *tree, window_t *win, apert_t *apt);

#ifdef __cplusplus
}
#endif


#endif

