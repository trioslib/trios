#ifndef _trios_io_h_
#define _trios_io_h_

#ifdef __cplusplus
extern "C" {
#endif


#include <io_header.h>
#include <trios_img.h>
#include "trios_mtm.h"
#include "trios_itv.h"
#include <trios_apert.h>

/*!
  Read a PGM image.

  \param filename Path to the image file.
  \return An img_t with the image.
  */
img_t *img_readPGM(char *filename);

/*!
  Writes an image to a PGM file.

  \param filename Path to save the image to.
  \param img Image to be saved.
  */
void img_writePGM(char *filename, img_t *img);

/*!
    Read an image collection from a file.

    \param fname Path to the file.
    \return A pointer to the created image set, NULL on failure.
*/

imgset_t *imgset_read(char *fname);

/*!
    Wrtie an image set to a file.

    \param fname Filename.
    \param imgset Image set.
    \return 1 on success. 0 on failure.
*/

int imgset_write(char *fname, imgset_t *imgset);

/*!
    Read an interval set from a file.

    \param fname File name.
    \param win Window structure.
    \return A pointer to a itv_t structure, NULL on failure.
    \sa itv_t
 */

itv_t *itv_read(char *fname, window_t **win/*, apert_t **apt*/);

/*!
    Write an interval set to a file.

    \param fname File name.
    \param itv Interval set.
    \param win Window.
    \return 1 on success. 0 on failure.
*/
int itv_write(char *fname, itv_t *itv, window_t *win /*apert_t *apt*/);

/*!
    Reads a classified examples set from a file.

    \param fname File name.
    \param win Window used.
    \return A mtm_t structure or NULL on failure.
*/

mtm_t *mtm_read(char *fname, window_t **win/*, apert_t  **apt*/);

/*!
    Writes a classified examples set to a file.

    \param fname File name.
    \param mtm Classified examples structure.
    \param win Window used.
    \return 1 on success. 0 on failure.
*/
int mtm_write(char *fname, mtm_t *mtm, window_t *win/*, apert_t *apt*/);

/*!
    Read a window structure from a file.

    \param fname File name.
    \return Window read from file. NULL on failure.
*/
window_t *win_read(char *fname);

/*!
    Write a file with the window data.

    \param fname File name.
    \param win Window to write.
    \return 1 on success. 0 on failure.
*/
int win_write(char *fname, window_t *win);


/*!
    Read an examples file.

    \param fname File name.
    \param win Window structure.
    \return Pointer to the examples and window structure.
*/
xpl_t *xpl_read(char *fname, window_t **win, apert_t **apt);

/*!
    Write the examples file.

    \param fname File name.
    \param xpl Examples structure.
    \param win Window structure.
    \return 1 on success. 0 on failure.
*/
int xpl_write(char *fname, xpl_t *xpl, window_t *win, apert_t  *apt);

/*!
    Write a special examples file to use with stackfd.

    \param fname Output file name.
    \param xpl Example tree.
    \param win Window used in the lcollec_threshold.
*/
int swt_write(char *fname, xpl_t *xpl, window_t *win);

/*!
 * Reads a decision tree from a file.
 * \param fname File name.
 * \param type Operator's type.
 * \param win Window.
 * \param apt Aperture.
 * \return  The read decision tree.
 */
int dt_read(char *fname, int *type, window_t **win, apert_t **apt);


/*!
 * Writes a decision tree to a file
 * \param fname File name.
 * \param type Type of the operator.
 * \param win Window.
 * \param apt Aperture.
 * \return
 */
int dt_write(char *fname, int type, window_t *win, apert_t *apt);



dTree *read_tree2(char *filename);

void write_tree2(char *filename, dTree *tree);

#ifdef __cplusplus
}
#endif


#endif
















