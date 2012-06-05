#include <pacbasic.h>

 /*#define _DEBUG_*/


/*!
    Create an image set structure.

    \param ngroups Number of image groups.
    \param grpsize Size of image groups.
    \return A new image set.
*/

imgset_t *imgset_create(int ngroups, int grpsize) {
/* author: Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)                  */
/* date: Fri Nov 29 1996                                                    */

  imgset_t *imgset ;
  int      i, k ;
  char     **f ;


  imgset = (imgset_t *)malloc(sizeof(imgset_t)) ;
  if(imgset==NULL) {
    return (imgset_t *)trios_error(1, "Memory allocation failed.") ;
  }

  imgset->dir = (char **)malloc(sizeof(char *)*grpsize) ;
  if(imgset->dir==NULL) {
    free(imgset) ;
    return (imgset_t *)trios_error(1, "Memory allocation failed.") ;
  }

  imgset->file = (char ***)malloc(sizeof(char **)*ngroups) ;
  if(imgset->file==NULL) {
    free(imgset->dir) ;
    free(imgset) ;
    return (imgset_t *)trios_error(1, "Memory allocation failed.") ;
  }

  for(k=0; k<ngroups; k++) {
    imgset->file[k] = (char **)malloc(sizeof(char *)*grpsize) ;
    if(imgset->file[k]==NULL) {
      free(imgset->dir) ;
      free(imgset->file) ;
      free(imgset) ;
      return (imgset_t *)trios_error(1, "Memory allocation failed.") ;
    }
  }

  for(i=0; i<grpsize; i++) {
    imgset->dir[i] = NULL ;
  }

  for(k=0; k<ngroups; k++) {
    f = imgset->file[k];
    for(i=0; i<grpsize; i++) {
      f[i] = NULL ;
    }
  }

  imgset->ngroups = ngroups ;
  imgset->grpsize = grpsize ;

  return(imgset) ;
}


/*!
    Free IMGSET memory.

    \param imgset Pointer to the image set.
*/

void        /*+ Purpose: free image set memory area                        +*/
  imgset_free(
    imgset_t *imgset  /*+ In/Out: pointer to the image set                 +*/
  )
/*+ Return: nothing                                                        +*/
{
/* author: Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)                  */
/* date: Fri Nov 29 1996                                                    */
 
  int  i, k ;
  char **f ;
  if (imgset == NULL) {
    trios_warning("Trying to free NULL imgset_t pointer.") ;
    return;
  }
  if(imgset->dir) {
    for(i=0; i<imgset->grpsize; i++) {
      if(imgset->dir[i]) free(imgset->dir[i]) ;
      imgset->dir[i] = NULL ;
    }
    free(imgset->dir) ;
    imgset->dir = NULL ;
  }

  if(imgset->file) {
    for(k=0; k<imgset->ngroups; k++) {
      if(imgset->file[k]) {
        f = imgset->file[k];
        if(f) {
          for(i=0; i<imgset->grpsize; i++) {
            if(f[i]) free(f[i]) ;
            f[i] = NULL ;
          }
          free(imgset->file[k]) ;
          imgset->file[k] = NULL ;
        }
      }
    }
    free(imgset->file) ;
    imgset->file = NULL ;    
  }
  free(imgset) ;
}

/*!
   Get the number of groups in the image set.

   \param imgset Pointer to the image set.
   \return Number of groups in imgset.
*/

int imgset_get_ngroups(imgset_t *imgset) {
/* author: Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)                  */
/* date: Fri Nov 29 1996                                                    */
  return(imgset->ngroups) ;
}

/*!
    Get the size (number of images) of the groups.

   \param imgset Pointer to the image set.
   \return Number of images of groups.
*/

int imgset_get_grpsize(imgset_t *imgset) {
/* author: Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)                  */
/* date: Fri Nov 29 1996                                                    */

  return(imgset->grpsize) ;
}

/*!
    Get the i-th image file name from the k-th group.

    \param imgset Pointer to image set.
    \param i Image identifier.
    \param k Group identifier.
*/
char *imgset_get_fname(imgset_t *imgset, int i, int k) {
/* author: Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)                  */
/* date: Fri Nov 29 1996                                                    */
 
  char **f, *fname ;


  if(k > imgset_get_ngroups(imgset)) {
    return (NULL) ;
  }
  if(i > imgset_get_grpsize(imgset)) {
    return (NULL) ;
  }

  f = imgset->file[k-1] ;
  fname = f[i-1] ;
  if(fname==NULL) return(NULL) ;

#ifdef _DEBUG_
trios_debug("File name : %s.", fname) ;
#endif

  if(strlen(fname)==0) return(NULL) ;

  return(fname) ;
}

/*!
   Put image file name into i-th place of k-th group.

   \param imgset Pointer to image set.
   \param i Image identifier.
   \param k Group identifier.
   \param fname File name.
   \return 1 on success. 0 on failure.
*/

int imgset_set_fname(imgset_t *imgset, int i, int k, char *fname) {
/* author: Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)                  */
/* date: Fri Nov 29 1996                                                    */
 
  char **f ;


#ifdef _DEBUG_
trios_debug("%d-th group, %d-th file %s", k, i, fname) ;
#endif

  if(k > imgset_get_ngroups(imgset)) {
    return trios_error(1, "Images set do not contain %d-th group.", k) ;
  }
  if(i > imgset_get_grpsize(imgset)) {
    return trios_error(1, "Image group doesn't contain %d-th element", i) ;
  }

  if(fname != NULL) {
    f = imgset->file[k-1] ;
    f[i-1] = (char *)malloc(sizeof(char)*(strlen(fname)+1)) ;
    if(!f[i-1]) {
      return trios_error(1, "Memory allocation failed.") ;
    }
    strcpy(f[i-1], fname) ;
  }

  return(1) ;

}

/*!
    Get the i-th pathname.

    \param imgset Pointer to image set.
    \param i Group identifier.
    \return The directory of the i-th group.
*/

char *             /*+ Purpose: get the i-th pathname                      +*/
  imgset_get_dname(
    imgset_t *imgset,     /*+ In: pointer to the image set                 +*/
    int      i            /*+ In: indication for the desired path name     +*/
  )
/*+ Return: the i-th pathname on succes, NULL on failure                   +*/ 
{
/* author: Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)                  */
/* date: Fri Nov 29 1996                                                    */

  char *dir ;


  if(i > imgset_get_grpsize(imgset)) {
    return(NULL) ;
  }

  dir = imgset->dir[i-1] ;

  if(dir==NULL) return(NULL) ;

#ifdef _DEBUG_
trios_debug("Directory name : %s.", dir) ;
#endif

  if(strlen(dir)==0) return(NULL) ;
  return(dir) ;
}

/*!
    Set the i-th group directory path.

    \param imgset Point to image set.
    \param i Group identifier.
    \param dir Path name.
    \return 1 on success. 0 on failure.
*/

int imgset_set_dname(imgset_t *imgset, int i, char *dir) {
/* author: Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)                  */
/* date: Fri Nov 29 1996                                                    */
 
#ifdef _DEBUG_
trios_debug("%d-th diretorio %s", i, dir) ;
#endif


  if(i>imgset_get_grpsize(imgset)) {
    (void)trios_error(1, "Image group doesn't contain %d-th element\n", i) ;
    return(0) ;
  }

  if(dir!=NULL) {

    imgset->dir[i-1] = (char *)malloc(sizeof(char)*(strlen(dir)+1)) ;
    if(!imgset->dir[i-1]) {
      return trios_error(1, "Memory allocation failed.") ;
    }
    strcpy(imgset->dir[i-1], dir) ;
  }

  return(1) ;

}

/*!
    Get the full file name of the i-th image in the k-th group.

    \param imgset Point to image set.
    \param i Image identifier.
    \param k Group identifier.
    \return The full file name if it exists. NULL on error.
*/
 
char *imgset_get_ffullname(imgset_t *imgset, int i, int k) {
 
/*  author: Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)                 */
/*  date: Fri Mar 20 1998                                                   */

  char *dirname ;
  char *fname ;
  char *fullname ;


  if(!(dirname = imgset_get_dname(imgset, i))) {
    return(NULL) ;
  }
#ifdef _DEBUG_
trios_debug("dirname=%s", dirname) ;
#endif
  
  if(!(fname = imgset_get_fname(imgset, i, k))) {
    return(NULL) ;
  }
#ifdef _DEBUG_
trios_debug("fname=%s", fname) ;
#endif

  fullname = (char *)malloc(strlen(dirname)+strlen(fname)+1) ;

  strcpy(fullname, dirname) ;
  strcat(fullname, fname) ;

  return(fullname) ;
}



