#ifndef _trios_io_h_
#define _trios_io_h_

#include <io_header.h>
#include <trios_img.h>
#include "trios_mtm.h"
#include "trios_itv.h"

/* ---- io_win.c ---------------------------------------------- */

/*
     -------------------------------------------
     FUNCTION: win_read
     -------------------------------------------
*/

window_t *   /*+ Purpose: Reads a file with a description of the window  +*/
  win_read(
    char *fname  /*+ In: String with the name of the file                +*/ 
  )
/*+ Return: structure window_t on success, NULL on failure               +*/ ;


/*
     -------------------------------------------
     FUNCTION: win_read_data
     -------------------------------------------
*/

window_t *        /*+ Purpose: Reads a description of the window  +*/
  win_read_data(
     FILE *fd     /*+ In: file descriptor                         +*/ 
  )
/*+ Return: structure window_t on success, NULL on failure        +*/ ;


/*
     -------------------------------------------
     FUNCTION: win_write
     -------------------------------------------
*/

int         /*+ Purpose: Writes a file with a description of the window +*/  
  win_write(
    char *fname,  /*+ In: String with the name of the file              +*/ 
    window_t *win /*+ In: Window data structure                         +*/
  )
/*+ Return: 1 on success, NULL on failure                               +*/ ;


/*
     -------------------------------------------
     FUNCTION: win_write_data
     -------------------------------------------
*/

void          /*+ Purpose: Writes a data description of the window  +*/
  win_write_data(
    FILE *fd,     /*+ In: String with the name of the file          +*/
    window_t *win /*+ In: Window data structure                     +*/
  )
/*+ Return: nothing                                                 +*/ ;



/* ------ io_imgset.c --------------------------- */

/*
     -------------------------------------------
     FUNCTION: imgset_read
     -------------------------------------------
*/

imgset_t *     /*+ Purpose: read an image collection file                  +*/ 
 imgset_read(
   char *fname   /*+ In: Image collection's file name                      +*/
 ) ;
/*+ Return: a pointer to a image set structure on success, NULL on failure +*/


/*
     -------------------------------------------
     FUNCTION: imgset_write
     -------------------------------------------
*/

int            /*+ Purpose: write an image set file               +*/ 
  imgset_write(
    char *fname,       /*+ In: file name                          +*/
    imgset_t *imgset   /*+ In: imgset to be written               +*/
  ) ;
/*+ Return: 1 on success, 0 on failure                            +*/

int            /*+ Purpose: Cretaes and writes an image set file  +*/ 
  imgset_create_write(
    char *fname,       /*+ In: IMGSET file name                   +*/
    char *fname_obs,   /*+ In: observed image file name           +*/
    char *fname_ide,   /*+ In: ideal image file name              +*/
    char *fname_mask   /*+ In: (optional) mask image file name    +*/
  ) ;

/* ------ io_img.c ------------------------------- */

img_t *img_readPGM(char *filename);
void img_writePGM(char *filename, img_t *i);


/* ------ io_xpl.c ------------------------------- */

xpl_t *             /*+ Purpose: Read an examples file   +*/
  xpl_read(
    char *fname,    /*+ In: name of the file of examples      +*/
    window_t **win /*+ Out: pointer to window structure      +*/
    //apert_t  **apt  /*+ Out: pointer to an aperture structure +*/
    ) ;

int               /*+ Purpose: Write the examples file     +*/  
  xpl_write(
    char *fname,  /*+ In: Name of the file to be written   +*/
    xpl_t *xpl,   /*+ In: Pointer to example structure     +*/
    window_t *win/*+ In: Pointer to window structure      +*/
    //apert_t  *apt /*+ In: Pointer to an aperture structure +*/
    ) ;


/* --------- io_showxpl.c ---------------------------------- */

/*
     -------------------------------------------
     FUNCTION: lshowxpl
     -------------------------------------------
*/
int                 /*+ Purpose: shows the elements of an example set +*/  
  lshowxpl(
    char *fname_i,    /*+ In: XPL file name                           +*/
    int  select_type, /*+ In: type of examples to be showed
                              1 : All
                              2 : only conflicting ones
                              3 : only non-conflicting ones           +*/
    int  from,        /*+ In: range start                             +*/
    int  to,          /*+ In: range end                               +*/
    int  sort_type,   /*+ In: key to sorting :
                              1 : regular (by w-pattern)
                              2 : by frequency                        +*/
    int  order_type,  /*+ In: ordering type :
                              1 : ascendent
                              2 : descendent                          +*/
    char *fname_o     /*+ In: output XPL show file                    +*/
  )
/*+ Return: 1 on success, 0 on failure                                +*/ ;

/* --------- io_showmtm.c ---------------------------------- */

/*
     -------------------------------------------
     FUNCTION: lshowmtm
     -------------------------------------------
*/
int lshowmtm         /*+ Purpose: shows the elements of an mtm set +*/
  (                                                                    
    char *fname_i,         /*+ In: The input mtm file +*/
    char *fname_o,         /*+ Out: Name of the outout file +*/
    int from,        /*+ In: Range start +*/
    int to           /*+ In: Range end +*/
)
/*+ Returns: 1 on success, 0 on failure +*/ ;


/* ------ io_mtm.c ------------------------------- */

mtm_t *           /*+ Purpose: Reads a miniterm collection file             +*/
  mtm_read(
    char *fname,    /*+ In:  name of the file of miniterms                  +*/
    window_t **win /*+ Out: pointer to window structure                    +*/
    //apert_t  **apt  /*+ Out: pointer to an aperture structure +*/
    ) ;

int                /*+ Purpose: Writes the miniterms collection to a file  +*/
  mtm_write(
    char *fname,     /*+ In: name of the file                              +*/
    mtm_t *mtm,      /*+ In: pointer to  miniterm structure                +*/ 
    window_t *win   /*+ In: pointer to window structure                   +*/
    //apert_t  *apt    /*+ In: Pointer to an aperture structure +*/
    ) ;


/* --------- io_itv.c -------------------*/

/*
     -------------------------------------------
     FUNCTION: itv_read
     -------------------------------------------
*/
itv_t *               /*+ Purpose: read an interval set from a file   +*/   
  itv_read(
    char     *fname,   /*+ In: file name                              +*/
    window_t **win    /*+ Out: WINDOW structure                      +*/
    //apert_t  **apt     /*+ Out: Pointer to an aperture structure      +*/
  )
/*+ Return: a pointer to a ITV structure on success, NULL on failure  +*/ ;


/*
     -------------------------------------------
     FUNCTION: itv_write
     -------------------------------------------
*/
int               /*+ Purpose: write an intervals set to a file +*/
  itv_write(
    char  *fname,  /*+ In: file name                            +*/
    itv_t *itv,    /*+ In: intervals set                        +*/
    window_t *win /*+ In: window                               +*/
    //apert_t  *apt  /*+ In: Pointer to an aperture structure +*/
  )
/*+ Return: 1 on success, 0 on failure                          +*/ ;


#endif
















