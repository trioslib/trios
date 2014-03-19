#ifndef TRIOS_WIN_SELECTION_H
#define TRIOS_WIN_SELECTION_H

#ifdef __cplusplus
extern "C" {
#endif

#include "trios_win.h"
#include "trios_xpl.h"
#include "trios_mtm.h"

/*! Holds the rank of a window point. */
typedef struct {
    float weight; /*! < Rank of the point. */
    int i; /*! < Point row. */
    int j; /*! < Point column. */
} point_weight;


/* other methods for window selection will be implemented! */

/*!
 * Runs the Relief algorithm to select a subwindow of domain with n points.
 * \param xpl Set of patterns extracted from an imageset.
 * \param domain A window containing all the points to be considered.
 * \param m Number of prototype instances.
 * \param n The number of points in the desired window.
 * \param pw (Optional) Sorted array of the weight of each point in the domain window.
 * \param seed (Optional) Random seed. Pass 0 to use the default value.
 * \return A window with n points.
 * \sa lcollec_memory
 */
window_t *window_relief(xpl_t *xpl, window_t *domain, int m, int n, point_weight **pw, int seed);


window_t *window_martins_barrera(xpl_t *xpl, window_t *domain, point_weight **pw);

/*!
 * Resample the xpl_domain patterns using new_win. Useful when the lcollec_* functions take a long time or when
 * determining two-level architectures.
 * \param xpl_domain Patterns extracted with win_domain.
 * \param win_domain A (tipically big) window.
 * \param new_win A subwindow of win_domain.
 * \return The xpl_domain patterns as if they were observed using new_win.
 */
xpl_t *window_resample_xpl(xpl_t *xpl_domain, window_t *win_domain, window_t *new_win);


xpl_t *xpl_remove_variables(xpl_t * old_xpl, int *vars, int n_remove);

#ifdef __cplusplus
}
#endif
#endif // TRIOS_WIN_SELECTION_H
