
#ifndef TRIOS_OPERATOR__H
#define TRIOS_OPERATOR__H

#ifdef __cplusplus
extern "C" {
#endif

#include "trios_win.h"
#include "trios_itv.h"
#include "trios_xpl.h"
#include "trios_mtm.h"
#include "trios_apert.h"
#include "trios_learn.h"
#include "trios_img.h"

/*!
 * \brief Structure that holds an image operator.
 *
 * Its main purpose is to make the treatment of image operators uniform across high-level functions of the library.
 */
typedef struct {
    int type; /*! < Type of the operator. Currently supported: BB, GG, WCK, WCF. */
    window_t *win; /*! < Window of the operator. */
    xpl_t *collec; /*! < Set of pairs (W-pattern, label) used to train the operator. */
    mtm_t *decision; /*! < Set of pairs (W-pattern, label) in which the label minimizes the MSE of the training set. */

    itv_t *bb; /*! < BB operator. NULL if the type is not BB */
    dTree *gg; /*! < GG or WC* operator. NULL if the type is BB. */
    apert_t *apt; /*! < Aperture of the operator. NULL if the type is not WC* */
} image_operator_t;

image_operator_t *image_operator_build_bb(imgset_t *set, window_t *win);
image_operator_t *image_operator_build_gg(imgset_t *set, window_t *win);
image_operator_t *image_operator_build_wkc(imgset_t *set, window_t *win, apert_t *apt);
image_operator_t *image_operator_build_wkf(imgset_t *set, window_t *win, apert_t *apt);

img_t *image_operator_apply(image_operator_t *iop, img_t *input, img_t *mask);

void image_operator_free(image_operator_t *iop);

#ifdef __cplusplus
}
#endif

#endif
