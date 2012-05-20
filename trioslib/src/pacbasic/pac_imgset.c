#include <pacbasic.h>

/* #define _DEBUG_ */


/*
     -------------------------------------------
     FUNCTION: imgset_create
     -------------------------------------------
*/

imgset_t *     /*+ Purpose: create an image set structure                  +*/
  imgset_create(
    int ngroups,   /*+ In: number of image groups                          +*/
    int grpsize    /*+ In: size of image groups                            +*/
  )
/*+ Return: a pointer to the image set structure                           +*/
{
/* author: Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)                  */
/* date: Fri Nov 29 1996                                                    */

  imgset_t *imgset ;
  int      i, k ;
  char     **f ;


  imgset = (imgset_t *)malloc(sizeof(imgset_t)) ;
  if(imgset==NULL) {
    return (imgset_t *)pac_error(1, "Memory allocation failed.") ;
  }

  imgset->dir = (char **)malloc(sizeof(char *)*grpsize) ;
  if(imgset->dir==NULL) {
    free(imgset) ;
    return (imgset_t *)pac_error(1, "Memory allocation failed.") ;
  }

  imgset->file = (char ***)malloc(sizeof(char **)*ngroups) ;
  if(imgset->file==NULL) {
    free(imgset->dir) ;
    free(imgset) ;
    return (imgset_t *)pac_error(1, "Memory allocation failed.") ;
  }

  for(k=0; k<ngroups; k++) {
    imgset->file[k] = (char **)malloc(sizeof(char *)*grpsize) ;
    if(imgset->file[k]==NULL) {
      free(imgset->dir) ;
      free(imgset->file) ;
      free(imgset) ;
      return (imgset_t *)pac_error(1, "Memory allocation failed.") ;
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


/*
     -------------------------------------------
     FUNCTION: imgset_free
     -------------------------------------------
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
    pac_warning("Trying to free NULL imgset_t pointer.") ;
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

/*
     -------------------------------------------
     FUNCTION: imgset_get_ngroups
     -------------------------------------------
*/

int            /*+ Purpose: get the number of groups in the image set      +*/
  imgset_get_ngroups(
    imgset_t *imgset    /*+ In: pointer to the image set                   +*/
  )
/*+ Return: an integer (number of image groups)                            +*/
{
/* author: Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)                  */
/* date: Fri Nov 29 1996                                                    */
 
  return(imgset->ngroups) ;
}

/*
     -------------------------------------------
     FUNCTION: imgset_get_grpsize
     -------------------------------------------
*/

int         /*+ Purpose: get the size (number of images) of the groups     +*/
  imgset_get_grpsize(
    imgset_t *imgset           /*+ In: pointer to the image set            +*/
  )
/*+ Return: an integer (the size of the image groups)                      +*/
{
/* author: Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)                  */
/* date: Fri Nov 29 1996                                                    */

  return(imgset->grpsize) ;
}

/*
     -------------------------------------------
     FUNCTION: imgset_get_fname
     -------------------------------------------
*/

char *      /*+ Purpose: get the i-th image file name from the k-th group  +*/
  imgset_get_fname(
    imgset_t *imgset, /*+ In: pointer to a image set                       +*/
    int       i,      /*+ In: the image identifier                         +*/
    int       k       /*+ In: the image group identifier                   +*/
  )
/*+ Return: the i-th image file name from the k-th group                   +*/
{
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
pac_debug("File name : %s.", fname) ;
#endif

  if(strlen(fname)==0) return(NULL) ;

  return(fname) ;
}

/*
     -------------------------------------------
     FUNCTION: imgset_set_fname
     -------------------------------------------
*/

int              /*+ Purpose: put image file name into i-th place of k-th
                              image group                                  +*/
  imgset_set_fname(
    imgset_t *imgset,   /*+ In/Out: pointer to the image set               +*/
    int       i,        /*+ In: the image identifier                       +*/
    int       k,        /*+ In: the identifier for the image group         +*/
    char      *fname    /*+ In: image file name                            +*/
  )
/*+ Return: 1 on success, 0 on failure +*/
{
/* author: Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)                  */
/* date: Fri Nov 29 1996                                                    */
 
  char **f ;


#ifdef _DEBUG_
pac_debug("%d-th group, %d-th file %s", k, i, fname) ;
#endif

  if(k > imgset_get_ngroups(imgset)) {
    return pac_error(1, "Images set do not contain %d-th group.", k) ;
  }
  if(i > imgset_get_grpsize(imgset)) {
    return pac_error(1, "Image group doesn't contain %d-th element", i) ;
  }

  if(fname != NULL) {
    f = imgset->file[k-1] ;
    f[i-1] = (char *)malloc(sizeof(char)*(strlen(fname)+1)) ;
    if(!f[i-1]) {
      return pac_error(1, "Memory allocation failed.") ;
    }
    strcpy(f[i-1], fname) ;
  }

  return(1) ;

}

/*
     -------------------------------------------
     FUNCTION: imgset_get_dname
     -------------------------------------------
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
pac_debug("Directory name : %s.", dir) ;
#endif

  if(strlen(dir)==0) return(NULL) ;
  return(dir) ;
}

/*
     -------------------------------------------
     FUNCTION: imgset_set_dname
     -------------------------------------------
*/

int               /*+ Purpose: put a path name into a image set            +*/
  imgset_set_dname(
    imgset_t *imgset,    /*+ In/Out: pointer to the image set              +*/
    int      i,          /*+ In: indication for the path name to be set    +*/
    char     *dir        /*+ In: the path name                             +*/
  )
/*+ Return: 1 on success, 0 on failure                                     +*/
{
/* author: Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)                  */
/* date: Fri Nov 29 1996                                                    */
 
#ifdef _DEBUG_
pac_debug("%d-th diretorio %s", i, dir) ;
#endif


  if(i>imgset_get_grpsize(imgset)) {
    (void)pac_error(1, "Image group doesn't contain %d-th element\n", i) ;
    return(0) ;
  }

  if(dir!=NULL) {

    imgset->dir[i-1] = (char *)malloc(sizeof(char)*(strlen(dir)+1)) ;
    if(!imgset->dir[i-1]) {
      return pac_error(1, "Memory allocation failed.") ;
    }
    strcpy(imgset->dir[i-1], dir) ;
  }

  return(1) ;

}

/*
     -------------------------------------------
     FUNCTION: imgset_get_ffullname
     -------------------------------------------
*/
 
char *          /*+ Purpose: gets the full file name of the i-th image
                             in the k-th image group                       +*/
  imgset_get_ffullname(                                                       
    imgset_t *imgset,  /*+ In: IMGSET structure                            +*/
    int      i,        /*+ In: i-th image                                  +*/
    int      k         /*+ In: k-th image group                            +*/
)
/*+ Return: the full file name of the i-th image in the k-th image group
    if such image exists, or NULL otherwise                                +*/
{
 
/*  author: Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)                 */
/*  date: Fri Mar 20 1998                                                   */

  char *dirname ;
  char *fname ;
  char *fullname ;


  if(!(dirname = imgset_get_dname(imgset, i))) {
    return(NULL) ;
  }
#ifdef _DEBUG_
pac_debug("dirname=%s", dirname) ;
#endif
  
  if(!(fname = imgset_get_fname(imgset, i, k))) {
    return(NULL) ;
  }
#ifdef _DEBUG_
pac_debug("fname=%s", fname) ;
#endif

  fullname = (char *)malloc(strlen(dirname)+strlen(fname)+1) ;

  strcpy(fullname, dirname) ;
  strcat(fullname, fname) ;

  return(fullname) ;
}



