
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
#include "trios_img.h"

/*!
 * \brief Typedef for the decision tree implementation. It is choosen as a void pointer to allow any type of structure. Right now we use the implementation from OpenCV.
 */
typedef void dTree;


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
    apert_t *apt; /*! < Aperture of the operator. NULL if the type is not WC*. */
} image_operator_t;

/*!
 * \brief Wraps an interval structure and window in a BB image operator.
 * \param itv BB Interval.
 * \param win Window.
 * \return An image operator that applies the input interval using the input window.
 */
image_operator_t *image_operator_itv(itv_t *itv, window_t *win);

/*!
 * \brief Builds a BB image operator.
 * \param set Training set.
 * \param win Window.
 * \return Trained BB image operator.
 */
image_operator_t *image_operator_build_bb(imgset_t *set, window_t *win);

/*!
 * \brief Builds a GG image operator.
 * \param set Training set.
 * \param win Window.
 * \return Trained GG image operator.
 */
image_operator_t *image_operator_build_gg(imgset_t *set, window_t *win);

/*!
 * \brief Builds a GB image operator. The decision process uses the MEDIAN criteria.
 * \param set Training set.
 * \param win Window.
 * \return Trained GB image operator.
 */
image_operator_t *image_operator_build_gb(imgset_t *set, window_t *win);

/*!
 * \brief Builds a WKF image operator.
 * \param set Training set.
 * \param win Window.
 * \param ki Input range.
 * \param ko Output range.
 * \param vplace Vertical placement of the window.
 * \return Trained WKF image operator.
 */
image_operator_t *image_operator_build_wkf(imgset_t *set, window_t *win, int ki, int ko, int vplace);

/*!
 * \brief Builds a WKC image operator.
 * \param set Training set.
 * \param win Window.
 * \param ki Input range.
 * \param vplace Vertical placement of the window.
 * \return Trained WKC image operator.
 */
image_operator_t *image_operator_build_wkc(imgset_t *set, window_t *win, int ki, int vplace);

/*!
 * \brief Builds a WKC or WKF image operator.
 * \param set Training set.
 * \param win Window.
 * \param apt Aperture configuration.
 * \return Trained WKC image operator.
 */
image_operator_t *image_operator_build_wk(imgset_t *set, window_t *win, apert_t *apt);

/*!
 * \brief Transforms an image using an image operator.
 * \param iop Image operator.
 * \param input Input image.
 * \param mask Application mask.
 * \return
 */
img_t *image_operator_apply(image_operator_t *iop, img_t *input, img_t *mask);

/*!
 * \brief Calculates the MAE of an image operator in a test set.
 * \param iop Image operator.
 * \param test Test set.
 * \param acc Accuracy.
 * \return The MAE of the image operator.
 */
unsigned long image_operator_mae(image_operator_t *iop, imgset_t *test, double *acc);

/*!
 * \brief Calculates the MSE of an image operator in a test set.
 * \param iop Image operator.
 * \param test Test set.
 * \return The MSE of the image operator.
 */
unsigned long image_operator_mse(image_operator_t *iop, imgset_t *test);

/*!
 * \brief Frees the memory used by an image operator.
 * \param iop Image operator.
 */
void image_operator_free(image_operator_t *iop);

#ifdef __cplusplus
}
#endif

#endif
