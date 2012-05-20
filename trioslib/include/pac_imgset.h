#ifndef _pac_imgset_h_
#define _pac_imgset_h_

typedef struct {
  int  ngroups ;       /* number of image groups         */
  int  grpsize ;       /* number of images in each group */
  char **dir ;         /* directory name (path)          */
  char ***file ;       /* file names                     */
} imgset_t ;


/*===========================================================================\
   pac_imgset.c
\========================================================================== */

/*
     -------------------------------------------
     FUNCTION: imgset_create
     -------------------------------------------
*/

imgset_t *     /*+ Purpose: create an image set structure                  +*/
  imgset_create(
    int ngroups,   /*+ In: number of image groups                          +*/
    int grpsize    /*+ In: size of image groups                            +*/
  ) ;
/*+ Return: a pointer to the image set structure                           +*/


/*
     -------------------------------------------
     FUNCTION: imgset_free
     -------------------------------------------
*/

void        /*+ Purpose: free image set memory area                        +*/
  imgset_free(
    imgset_t *imgset  /*+ In/Out: pointer to the image set                 +*/
  ) ;
/*+ Return: nothing                                                        +*/


/*
     -------------------------------------------
     FUNCTION: imgset_get_ngroups
     -------------------------------------------
*/

int            /*+ Purpose: get the number of groups in the image set      +*/
  imgset_get_ngroups(
    imgset_t *imgset    /*+ In: pointer to the image set                   +*/
  ) ;
/*+ Return: an integer (number of image groups)                            +*/


/*
     -------------------------------------------
     FUNCTION: imgset_get_grpsize
     -------------------------------------------
*/

int         /*+ Purpose: get the size (number of images) of the groups     +*/
  imgset_get_grpsize(
    imgset_t *imgset           /*+ In: pointer to the image set            +*/
  ) ;
/*+ Return: an integer (the size of the image groups)                      +*/


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
  ) ;
/*+ Return: the i-th image file name from the k-th group                   +*/


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
  ) ;
/*+ Return: 1 on success, 0 on failure +*/


/*
     -------------------------------------------
     FUNCTION: imgset_get_dname
     -------------------------------------------
*/

char *             /*+ Purpose: get the i-th pathname                      +*/
  imgset_get_dname(
    imgset_t *imgset,     /*+ In: pointer to the image set                 +*/
    int      i            /*+ In: indication for the desired path name     +*/
  ) ;
/*+ Return: the i-th pathname on succes, NULL on failure                   +*/ 


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
  ) ;
/*+ Return: 1 on success, 0 on failure                                     +*/


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
) ;
/*+ Return: the full file name of the i-th image in the k-th image group
    if such image exists, or NULL otherwise                                +*/


#endif /* _pac_imgset_h_ */
