#ifndef TRIOS_MULTI_H
#define TRIOS_MULTI_H

#ifdef __cplusplus
extern "C" {
#endif

#include <trios_win.h>
#include <trios_xpl.h>
#include <trios_collec.h>
#include <trios_error.h>
#include <trios_itv.h>
#include "trios_operator.h"

/*!
 * \brief Type that holds either a dTree or a itv_t. Used in multi-level operators.
 */
typedef char classifier_t;


/*!
 * This structure stores the data that represents a level in a multi-level
 * image operator. Each level can contain an arbitrary number of operators
 * and each operator takes as input "ninputs" windows that collect pixels
 * of each operator in the previous level.
 *
 * \sa multi_architecture_t
 */
typedef struct {
    int ninputs; /*!< Number of operators in the previous level. It is also the number of input windows that each operator in this level uses. */
    int noperators; /*!< Number of operators in this level. The last level must have only one operator.  */
    window_t ***windows; /*!< Matrix of windows. The (i, j) element contains the j-th window of the i-th operator. */
} multi_architecture_level_t;

/*!
 * Structure that represents an architecture in a multi-level
 * image operator. It supports operators with an arbitrary number
 * of levels and, for each level, an arbitrary number of operators.
 * The only limitation is that an operator in one level can use only
 * pixels in it's previous level.
 *
 * \sa multi_architecture_level_t
 */
typedef struct  {
    int nlevels; /*!< Number of levels of the operator.*/
    multi_architecture_level_t *levels; /*!< Array of levels */
} multi_architecture_t;

/*!
 * A level in a multi-level operator. It contains the trained operators and its windows.
 */
typedef struct {
    int ninputs; /*!< Number of operators in the previous level. It is also the number of inputs that each operator in this level takes. */
    int noperators; /*!< Number of operators in this level. */
    window_t ***windows; /*!< Matrix of windows. The (i, j) element contains the j-th window of the i-th operator. */
    classifier_t **trained_operator; /*!< Array of the trained operators. */
} multi_level_operator_level_t;

/*!
 * A multi-level operator that contains "nlevels" levels.
 */
typedef struct {
    int type; /*!< Type of the operator: BB or GG. */
    int nlevels; /*!< Number of levels of the operator. */
    multi_level_operator_level_t *levels; /*!< Array of operator levels. */
} multi_level_operator_t;

/*!
 *
 * \param nlevels
 * \param operators_in_level
 * \return The created multi level architecture.
 */
multi_architecture_t *multi_level_arch_create(int nlevels, int *operators_in_level);

/*!
 * Set the window of the "input"-th of the "operator"-th in the "level"-th level. The window is not copied,
 * so if you free the window elsewhere this structure will break.
 * \param m Multi-level architecture.
 * \param level Number of the level.
 * \param op Number of the operator.
 * \param input Number of the input.
 * \param window Window to add to the architecture.
 * \return 1 on success. 0 on failure.
 */
int multi_level_arch_set_window(multi_architecture_t *m, int level, int op, int input, window_t *window);

/*!
 * Frees the memory area used by a multi-level architecture.
 * \param m A multi-level architecture.
 */
void multi_level_arch_free(multi_architecture_t *m);

/*!
 * Creates a multi-level operator based on a given architecture.
 * \param m Multi-level architecture.
 * \param type BB or GG.
 * \return The created operator.
 */
multi_level_operator_t *multi_level_operator_create(multi_architecture_t *m, int type);

/*!
 * Frees the memory used by a multi-level operator.
 * \param op Multi-level operator.
 */
void multi_level_operator_free(multi_level_operator_t *op);

/*!
 * Build a multi-level operator using a given multi-level architecture using samples from a given image set.
 * \param m Multi-level architecture of the operator.
 * \param set Image set to extract examples from.
 * \return The trained multi-level operator.
 * \sa multi_architecture_t, imgset_t
 */
multi_level_operator_t *multi_level_build_single(multi_architecture_t *m, imgset_t *set);

/*!
 * Build a BB multi-level operator using a given multi-level architecture using samples from an array of image sets.
 * \param m Multi-level architecture of the operator.
 * \param set Array of Image sets to extract examples from containing one element for each level.
 * \return The trained multi-level operator.
 * \sa multi_architecture_t, imgset_t
 */
multi_level_operator_t *multi_level_build_bb(multi_architecture_t *m, imgset_t **set);

/*!
 * Build a GG multi-level operator using a given multi-level architecture using samples from an array of image sets.
 * \param m Multi-level architecture of the operator.
 * \param set Array of Image sets to extract examples from containing one element for each level.
 * \return The trained multi-level operator.
 * \sa multi_architecture_t, imgset_t
 */
multi_level_operator_t *multi_level_build_gg(multi_architecture_t *m, imgset_t **set);


/*!
 * Build a multi-level (BB or GG) operator using a given multi-level architecture using samples from an array of image sets.
 * \param m Multi-level architecture of the operator.
 * \param set Array of Image sets to extract examples from containing one element for each level.
 * \param type BB or GG.
 * \return The trained multi-level operator.
 * \sa multi_architecture_t, imgset_t
 */
multi_level_operator_t *multi_level_build(multi_architecture_t *m, imgset_t **set, int type);

/*!
 * Returns the window used in the input-th input of the op-th operator in the "level"-th level.
 * \param mop Multi-level operator.
 * \param level Level number.
 * \param op Operator number.
 * \param input Input number.
 * \return The window used in the input-th input of the op-th operator in the "level"-th level.
 */
window_t *multi_level_operator_get_window(multi_level_operator_t *mop, int level, int op, int input);

/*!
 * Applies a multi-level operator to an image.
 * \param op Trained multi-level operator.
 * \param img Image to apply the operator.
 * \param mask Application mask.
 * \return Result image.
 */
img_t *multi_level_apply(multi_level_operator_t *op, img_t *img, img_t *mask);

/*!
 * Apply an operator of a level of a multi-level operator.
 * \param mop Multi-level operator.
 * \param level Level number.
 * \param op Operator number.
 * \param inputs Array of input images.
 * \param mask Application mask.
 * \return The output of this operator.
 *
 */
img_t *multi_level_apply_level_bb(multi_level_operator_t *mop, int level, int op, img_t **inputs, img_t *mask);

window_t *multi_level_operator_joint_window(multi_level_operator_t *mop, int level, int op);

/*!
 * \brief Combine the input operators using a multi-level approach.The returned operator shares data with the operators in ops.
 * \param ops List of image operators to combine.
 * \param nops Number of operators to combine.
 * \param level2 Training set for level 2.
 * \return A two-level operator that combines the input operators.
 */
multi_level_operator_t *multi_level_combine_operators(image_operator_t **ops, int nops, imgset_t *level2);

/*!
 * \brief Combine the input operators. This function skips the second level collec and uses the supplied xpl_t instead.
 * \param ops List of image operators to combine.
 * \param nops Number of operators to combine.
 * \param collec2nd Substitute XPL.
 * \return A trained two-level operator.
 */
multi_level_operator_t *multi_level_combine_xpl(image_operator_t **ops, int nops, xpl_t *collec2nd);

#ifdef __cplusplus
}
#endif

#endif
