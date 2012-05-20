#ifndef _pac_show_h_
#define _pac_show_h_

/*  Modification by:  Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)       */
/*  Date: Tue Aug 24 1999                                                   */
/*  Mod: Modifications to implement BG and GG (or WKGG) types               */


typedef struct {
  int  wsize ;       /* w-pattern size*/
  int  wzip ;        /* compacted w-pattern size */
  int  up_half ;     /* bottom limit of upper half */
  int  mid_row ;     /* middle row */
  int  up ;          /* bottom limit of lower half */
  int  map_type ;    /* mapping type */
  int  row_length ;  /* length corresponding to one row in the
                        wpat_vec matrix */
  char *wpat_vec ;   /* string for representing a w-pattern in a matrix
                        like format */
  int  *idx_vec ;    /* vector of index. idx_vec[p] points to the offset at
                        wpat_vec where the element corresponding to point
                        p of the window is */
} show_t ;


/*===========================================================================\
   pac_show.c
\========================================================================== */


show_t *     /*+ Purpose: create a SHOW structure                          +*/
  show_create(
    window_t *win,            /*+ In: window                               +*/
    int      map_type,        /*+ In: mapping type                         +*/
    int      band             /*+ In: window band                          +*/
    ) ;

void
  wpat2vecB(
    unsigned int *wpat,   /*+ In: w-pattern                                +*/
    show_t *show    /*+ In/Out: SHOW structure                             +*/
  ) ;


void
  wpat2vecG(
    char   *wpat,   /*+ In: w-pattern                                      +*/
    show_t *show    /*+ In/Out: SHOW structure                             +*/
  ) ;


void
  wpat2vecUG(
    char   *wpat,   /*+ In: w-pattern                                      +*/
    show_t *show    /*+ In/Out: SHOW structure                             +*/
    ) ;


void
  show_free(                                                                  
    show_t *show       /*+ In: pointer to the show structure               +*/
) ;


#endif
