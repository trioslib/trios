#include <paclib_kernel.h>
#include "paclearn_local.h"

/*
#define _DEBUG_
*/


/*
     -------------------------------------------
     FUNCTION: LDECISION 
     -------------------------------------------
*/

int        /*+ Purpose: Builds a classified examples table from a set of
	                observed examples                                  +*/
  ldecision(
    char *fname_i,   /*+ In: observed set of examples file name            +*/
    int   comp_prob, /*+ In: flag indicating if the probabilities should be
                            computed. (0=NO, 1=YES)                        +*/
    double freqmin,   /*+ In: Minimum frequency of an example (in %)  
                            (if this percentage is not satisfied, then 
                            example will be eliminated)                    +*/
    double estim,     /*+ In: Decision Estimator                            +*/
    int   comptype,  /*+ In: type of computation for the undecidable cases +*/
    int   undflabel, /*+ In: Value to be assigned to the undecidable
                             examples, if comptype==1                      +*/
    char *fname_o    /*+ Out: Output file name                             +*/
)
/*+ Return: 1 on success 0 on failure                                      +*/
{ 
/*  author: Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)                */
/*  date: Tue Oct 29 1996                                                  */

/*  Modification by:  Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)      */
/*  Date: Tue Jan  6 1998                                                  */
/*  Mod: Added a new parameter - comp_prob                                 */

/*  Modification by:  Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)       */
/*  Date: Sun Feb 20 2000                                                   */
/*  Mod: treatment of the undecidable cases (tie condition) fixed and       */
/*       improved. New parameter comptype introduced                        */

    xpl_t    *xpl ;
    mtm_t    *mtm ;
    window_t *win ;
    apert_t  *apt = NULL ;


#ifdef _DEBUG_
pac_debug("Entered ldecision with if=%s e of=%s", fname_i, fname_o) ;
#endif

    /* read observed examples file */
    if(NULL == (xpl = (xpl_t *)xpl_read(fname_i, &win, &apt))) 
      return pac_error(MSG, "xpl_read failed.") ;


    if(NULL == (mtm = (mtm_t *)mtm_decide(xpl, comp_prob,
                freqmin, estim, comptype, undflabel ))) {
      win_free(win) ;
      xpl_free(xpl) ;
      return pac_error(MSG, "mtm_decide failed.") ;
    }

    /* write the classified examples table to the output file */
    if(!mtm_write(fname_o, mtm, win, apt)) {
      win_free(win) ;
      xpl_free(xpl) ;
      mtm_free(mtm) ;
      return pac_error(MSG, "mtm_write failed.") ;
    }
 
#ifdef _DEBUG_
pac_debug("Antes dos frees finais") ;
#endif
    win_free(win) ;
#ifdef _DEBUG_
pac_debug("DEpois dos free win") ;
#endif
/* xpl_free(xpl) ; */
#ifdef _DEBUG_
pac_debug("DEpois dos free xpl") ;
#endif
    mtm_free(mtm) ;
#ifdef _DEBUG_
pac_debug("DEpois dos free mtm") ;
#endif
    //if(apt) apert_free(apt) ;
#ifdef _DEBUG_
pac_debug("DEpois dos frees finais") ;
#endif

    return(1);
}




