#ifndef _trios_itv_h_
#define _trios_itv_h_

#ifdef __cplusplus
extern "C" {
#endif

#include "trios_win.h"

#define ISI_0 0
#define ISI_1 1
#define ISI_2 2
#define ISI_3 3

/*!
    Structure that holds an interval of a binary W-operator.
*/
typedef struct itv_bx { /*!< Structure of the interval                      +*/
  unsigned int *A ;     /*!< left extremity of the interval                 +*/
  unsigned int *B ;     /*!< right extremity of the interval                +*/
  int label ;           /*!< value associated to the points of the interval +*/
  struct itv_bx *next ; /*!< pointer to the next interval                   +*/
} itv_BX ;

/*!
 * Structure that holds an interval of a gray level W-operator.
 */
typedef struct itv_gx {
  char    *A ;          /*!< lower extremity of the interval                +*/
  char    *B ;          /*!< upper extremity of the interval                +*/
  int     label ;       /*!< value associated to the points of the interval +*/
  double  size ;        /*!< size of the interval                           +*/
  int     nindexes ;    /*!< number of indexes                              +*/
  int    *plabels ;     /*!< list of labeled examples in the interval       +*/
  struct itv_gx *next ; /*!< pointer to the next interval                   +*/
} itv_GX ;


/*  Modification by:  Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)       */
/*  Date: Tue Aug 17 1999                                                   */
/*  Mod: a new field _maxlabel_ was inserted into itv_t structure.          */
/*       To handle OCR training, using tree structure training method,      */
/*       labels for the classes in the intermediary nodes of the training   */
/*       tree are greater than 255 (because labels 0-255 are reserved for   */
/*       real characters). That makes neccessary that the images produced   */
/*       when the basis (operator) is applied be a SHORT image.             */
/*       In order to avoid that images produced be always a SHORT image, to */
/*       save disk space, we established a convention that, if maxlabel>255 */
/*       then image produced will be SHORT, otherwise it will be BYTE       */

/*!
    Structure that holds an image operator.
*/
typedef struct {
  int  wsize ;          /*!< w-pattern size +*/
  int  wzip ;           /*!< compacted w-pattern size +*/
  int  type ;           /*!< type (BB, BG, GB, GG) +*/
  int  nitv ;           /*!< number of intervals +*/
  int  maxlabel ;       /*!< maximum label assigned to the intervals in the set +*/
  int  deflabel ;       /*!< default value to be associated to the
                            points out of the intervals union +*/
  int  *head ;          /*!< pointer to the intervals list head +*/
} itv_t ;


/*===========================================================================\
   pac_itv.c
\========================================================================== */

int       /*+ Purpose: give the number of intervals in a set of intervals  +*/
  itv_get_nitv(
    itv_t *itv      /*+ In: pointer to the ITV structure                   +*/
  ) ;
/*+ Return: a  positive integer                                            +*/


int            /*+ Purpose: give the size of w-pattern                     +*/
  itv_get_wsize(
    itv_t *itv              /*+ In: pointer to the ITV structure           +*/
  ) ;
/*+ Return: a positive integer                                             +*/


int            /*+ Purpose: give the pointer to the header of the list
                            of intervals                                   +*/
  *itv_get_head(
    itv_t *itv                /*+ In: pointer to the ITV structure         +*/
  ) ;
/*+ Return: a pointer to integer (it must be converted to the adequate
    type to be used)                                                       +*/


int   /*+ Purpose: gives the default label associated to the intervals set +*/
  itv_get_deflabel(
    itv_t *itv          /*+ In: pointer to the ITV structure               +*/
  ) ;
/*+ Return: an integer (default label)                                     +*/


int          /*+ Purpose: give the type : BB, BG, GB or GG                 +*/
  itv_get_type(
    itv_t *itv          /*+ In: pointer to the ITV structure               +*/
  ) ;
/*+ Return: an integer                                                     +*/


int            /*+ Purpose: give  the compacted w-pattern size             +*/
  itv_get_wzip(
    itv_t *itv        /*+ In: pointer to the ITV structure                 +*/
  ) ;
/*+ Return: a positive integer                                             +*/


int            /*+ Purpose: gives the maximum label found in a set of intervals +*/
  itv_get_maxlabel(
    itv_t *itv        /*+ In: pointer to the ITV structure                 +*/
  ) ;
/*+ Return: a positive integer (maximum label)                             +*/


void      /*+ Purpose: set the number of intervals in a ITV structure      +*/
  itv_set_nitv(
    itv_t *itv,     /*+ In/Out: pointer to the ITV structure               +*/
    int   nitv      /*+ In: label to be set as the number of intervals     +*/
  ) ;
/*+ Return: nothing                                                        +*/


void            /*+ Purpose: set the w-pattern size of an intervals set    +*/
  itv_set_wsize(
    itv_t *itv,         /*+ In/Out: pointer to intervals set               +*/
    int   wsize         /*+ In: w-pattern's size to be set                 +*/
  ) ;
/*+ Return: nothing                                                        +*/


void   /*+ Purpose: set the head of the intervals list of an intervals set +*/
  itv_set_head(
    itv_t *itv,               /*+ In/Out: intervals set                    +*/
    int   *head               /*+ In: pointer to intervals list head       +*/
  ) ;
/*+ Return: nothing                                                        +*/


void             /*+ Purpose: set the default label in an intervals set    +*/
  itv_set_deflabel(
    itv_t *itv,                  /*+ In/Out: intervals set                 +*/
    int    deflabel              /*+ In: default label                     +*/
  ) ;
/*+ Return: nothing                                                        +*/


void        /*+ Purpose: set the type in an intervals set                  +*/
  itv_set_type(
    itv_t *itv,      /*+ In/Out: intervals set                             +*/
    int   type       /*+ In: type to be set (BB, BG, GB or GG)             +*/
  ) ;
/*+ Return: nothing                                                        +*/


void   /*+ Purpose: set the compacted w-pattern's size of an intervals set +*/
  itv_set_wzip(
    itv_t *itv,          /*+ In/Out: intervals set                         +*/
    int   wzip           /*+ In: compacted w-pattern's size                +*/
  ) ;
/*+ Return: nothing                                                        +*/


void           /*+ Purpose: set the maximum label of a set of intervals    +*/
  itv_set_maxlabel(
    itv_t *itv,       /*+ In/Out: pointer to the ITV structure             +*/
    int   maxlabel    /*+ In: maximum label                                +*/
  ) ;
/*+ Return: a positive integer (maximum label)                             +*/


itv_t *       /*+ Purpose: create a ITV structure                          +*/
  itv_create(
    int  wsize,     /*+ In: w-pattern size                                 +*/
    int  type,      /*+ In: type (BB, BG, GB or GG)                        +*/
    int  deflabel   /*+ In: default label to be assigned to the ITV set    +*/
  ) ;
/*+ Return: a pointer to a ITV structure on success, 0 on failure          +*/


void          /*+ Purpose: free memory area used by a ITV structure        +*/
  itv_free(
    itv_t *itv              /*+ In: intervals set                          +*/
  ) ;
/*+ Return: nothing                                                        +*/


void            /*+ Purpose: free a intervals list                         +*/   
  itv_list_free(
    int *head,        /*+ In: head of intervals list                       +*/
    int type          /*+ In: type (BB, BG, GB or GG)                      +*/
  ) ;
/*+ Return: nothing                                                        +*/


void      /*+ Purpose: free memory area used by an interval of BX type     +*/
  itv_nodebx_free(
    itv_BX  *p               /*+ In: pointer to the interval               +*/
  ) ;
/*+ Return: nothing                                                        +*/



itv_BX *           /*+ Purpose: create an interval of BX type              +*/
  itv_nodebx_create(
    int  wzip                /*+ In: compacted size of w-pattern           +*/
  ) ;
/*+ Return: pointer to an interval of BX type                              +*/


void     /*+ Purpose: set some data  of an interval                        +*/
  itvbx_set(
    itv_BX *p,         /*+ In/Out: pointer to the interval                 +*/
    unsigned int *A,
    unsigned int *B,
    int    wzip,       /*+ In: compacted size of w-pattern                 +*/
    int    label,      /*+ In: label assigned to the interval              +*/
    itv_BX *next       /*+ In: if the interval is linked in a list of
                               intervals, this must be the pointer to the
                               next interval                               +*/
  ) ;
/*+ Return: nothing                                                        +*/


int             /*+ Purpose: compute  the dimension of an interval         +*/
  itv_dimension(                                                              
    itv_BX *p,         /*+ In: the interval                                +*/
    int    wzip        /*+ In: compacted w-pattern's size                  +*/
  ) ;
/*+ Return: an integer between 0 and n (where n stands for wsize)          +*/


int             /*+ Purpose: check if two intervals are identical          +*/
  itv_equal(                                                                  
    itv_BX *itv1,      /*+ In: first interval                              +*/
    itv_BX *itv2,      /*+ In: second interval                             +*/
    int    wzip        /*+ In: compacted w-pattern's size                  +*/
) ;
/*+ Return: 1 if they are equal, 0 otherwise                               +*/



itv_BX *        /*+ Purpose: compute  the intersection of two intervals    +*/
  itv_and(                                                                    
    itv_BX *itv1,      /*+ In: fisrt interval                              +*/
    itv_BX *itv2,      /*+ In: second interval                             +*/
    int    wzip        /*+ In: compacted w-pattern's size                  +*/
) ;
/*+ Return: pointer to the resulting interval (NULL if the interval is empty), 
    -1 on failure                                                          +*/


itv_BX *        /*+ Purpose: group two adjacent intervals                  +*/
  itv_group(                                                                  
    itv_BX *itv1,      /*+ In: first interval                              +*/
    itv_BX *itv2,      /*+ In: second interval                             +*/
    int    wzip        /*+ In: compacted w-pattern's size                  +*/
) ;
/*+ Return: a pointer to an interval on success, NULL on failure           +*/



itv_t *         /*+ Purpose: separate all intervals, with a given label, from
                             a set of intervals                            +*/
  itv_select(                                                                 
    itv_t *itv,        /*+ In/Out: interval set                            +*/
    int   label        /*+ In: the label                                   +*/
) ;
/*+ Return: a pointer to an interval set (a subset of the input set,
            containing only intervals with the specified label) on success, 
            NULL on failure                                                +*/


int             /*+ Purpose: sort intervals according to its dimension +*/
  itv_sort(
    itv_t *itv  /*+ In/Out: intervals structure                        +*/
  ) ;
/*+ Return: 1 on success, 0 on failure                                 +*/


int             /*+ Purpose: check if an interval contains a w-pattern     +*/
  itv_contain(                                                                
    itv_BX *p_itv,     /*+ In: the pointer to the interval                 +*/
    unsigned int *wpat,
    int    wzip        /*+ In: compacted w-pattern's size                  +*/
) ;
/*+ Return: 1 if the w-pattern is contained in the interval, 0 otherwise   +*/


int             /*+ Purpose: given an interval set and a label "old_label",
                             changes the label of the intervals set
                             with label "old_label" by "new_label"         +*/
  itv_label(                                                                  
    itv_t *itv ,       /*+ In/Out: intervals set                           +*/
    int   old_label,   /*+ In: old label                                   +*/
    int   new_label    /*+ In: new label                                   +*/
) ;
/*+ Return: 1 on success, 0 on failure                                     +*/



int             /*+ Purpose: check if some interval of a list contains a
                             w-pattern, and returns the respective label   +*/
  itv_list_contain(                                                                
    itv_t  *itv,        /*+ In: the pointer to the list of intervals       +*/
    unsigned int *wpat, /*+ In: the w-pattern                              +*/
    int    wzip         /*+ In: compacted w-pattern's size                 +*/
) ;
/*+ Return: the label of the interval that contains the w-pattern, or the
            default label value otherwise                                  +*/


itv_t *         /*+ Purpose: create one interval (1: [empty,W], 2:[o,W],
                             3:[empty,W-o])                                +*/
  itv_gen_itv(
    window_t *win,     /*+ In: window                                      +*/
    int      itv_type, /*+ In: interval type (1: [empty,W], 2:[o,W],
                               3:[empty,W-o])                              +*/
    int  map_type,     /*+ In: mapping type (BB=0 or BG=1 or GG=2 
			                     or WKC=3 or WKF=4)            +*/
    int  k0,           /*+ In: Input range for aperture                    +*/
    int  label,        /*+ In: label to be assigned to the interval        +*/
    int  def_label     /*+ In: label outside the interval (default label)  +*/
) ;
     /*+ Return: Pointer to ITV structure on success, NULL on failure           +*/ 

void itv_nodegx_free(itv_GX* p);


itv_t *itv_read(char *fname, window_t ** win /*, apert_t **apt */ );
int itv_write(char *fname, itv_t * itv, window_t * win /*apert_t *apt */ );

#ifdef __cplusplus
}
#endif


#endif /* _trios_itv_h_ */




