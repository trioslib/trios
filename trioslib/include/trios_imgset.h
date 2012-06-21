#ifndef _trios_imgset_h_
#define _trios_imgset_h_

#ifdef __cplusplus
extern "C" {
#endif


/*!
    Structure that holds a collection of images.

    The images are organized in groups, each group contains
    one or more images. Normally a group is used as a sample in the operator
    design and contains two or three images (input, ideal and, optionally, a mask).
 */
typedef struct {
  int  ngroups ;       /*!< Number of image groups         */
  int  grpsize ;       /*!< Number of images in each group */
  char **dir ;         /*!< Directory name (path)          */
  char ***file ;       /*!< File names                     */
} imgset_t ;

/*!
    Create an image set structure.

    \param ngroups Number of image groups.
    \param grpsize Size of image groups.
    \return A new image set.
*/

imgset_t *imgset_create(int ngroups, int grpsize);

/*!
    Free IMGSET memory.

    \param imgset Pointer to the image set.
*/
void imgset_free(imgset_t *imgset);

/*!
   Get the number of groups in the image set.

   \param imgset Pointer to the image set.
   \return Number of groups in imgset.
*/

int imgset_get_ngroups(imgset_t *imgset);

/*!
    Get the size (number of images) of the groups.

   \param imgset Pointer to the image set.
   \return Number of images of groups.
*/
int imgset_get_grpsize(imgset_t *imgset);

/*!
    Get the i-th image file name from the k-th group.

    \param imgset Pointer to image set.
    \param i Image identifier.
    \param k Group identifier.
*/
char *imgset_get_fname(imgset_t *imgset, int i, int k);

/*!
   Put image file name into i-th place of k-th group.

   \param imgset Pointer to image set.
   \param i Image identifier.
   \param k Group identifier.
   \param fname File name.
   \return 1 on success. 0 on failure.
*/

int imgset_set_fname(imgset_t *imgset, int i, int k, char *fname);

/*!
    Get the i-th pathname.

    \param imgset Pointer to image set.
    \param i Group identifier.
    \return The directory of the i-th group.
*/

char *imgset_get_dname(imgset_t *imgset, int i);

/*!
    Set the i-th group directory path.

    \param imgset Point to image set.
    \param i Group identifier.
    \param dir Path name.
    \return 1 on success. 0 on failure.
*/

int imgset_set_dname(imgset_t *imgset, int i, char *dir);
/*!
    Get the full file name of the i-th image in the k-th group.

    \param imgset Point to image set.
    \param i Image identifier.
    \param k Group identifier.
    \return The full file name if it exists. NULL on error.
*/

char *imgset_get_ffullname(imgset_t *imgset, int i, int k);

#ifdef __cplusplus
}
#endif


#endif /* _trios_imgset_h_ */
