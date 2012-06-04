#include <paclib_kernel.h>
#include "paclearn_local.h"

/* #define _DEBUG_ */

/*
     -------------------------------------------
     FUNCTION: lgroup
     -------------------------------------------
*/

int                /*+ Purpose: group the minterms (w-patterns) with same
                       labels in maximal intervals                      +*/
  lgroup(
    char *fname_i, /*+ In: set of classified examples file              +*/
    char *fname_o  /*+ In: set of intervals                             +*/
)
                   /*+ Return: 1 on success, 0 on failure               +*/
{
/* author: Nina S. Tomita & R. Hirata Jr. (nina@ime.usp.br)              */
/* date: Thu Oct 31 1996                                                 */

/* Date: Wed Feb 17 2000                                                   */
/* Mod: Window structure (1) changed to support multiple bands, (2) no     */
/*      longer holds information about aperture.                           */
/*      New structure "apert_t" was created.                               */
/*      Modifications were made to adequate to the structure changing.     */

  mtm_t    *mtm ;
  window_t *win ;
  apert_t  *apt ;
  itv_t    *itv ;


  /* read the set of classified examples file */
  if(!(mtm=mtm_read(fname_i, &win, &apt))) {
    return pac_error(MSG, "lgroup: mtm_read() failed.") ;
  }

#ifdef _DEBUG_
pac_debug("Classified examples reading: done") ;
#endif

  /* Try to group the examples in intervals of as high as possible dimension */
  if(!(itv=group_x(mtm))) {
    mtm_free(mtm) ;
    return pac_error(MSG, "lgroup: group() failed.") ;
  }

  /* write the intervals in a file */
  if(!itv_write(fname_o, itv, win, apt)) {
    mtm_free(mtm) ;
    itv_free(itv) ;
    win_free(win) ;
    return pac_error(MSG, "lgroup: itv_write() failed.") ;
  }

#ifdef _DEBUG_
pac_debug("Intervals writing: done") ;
#endif


  mtm_free(mtm) ;
  itv_free(itv) ;
  win_free(win) ;
  /*if(apt) apert_free(apt) ;*/

  return(1) ;
}

