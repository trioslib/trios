#include "trios_win.h"
#include "pac_itv.h"
#include "trios_mtm.h"
#include "pac_xpl.h"
#include "paclearn_local.h"
#include <stdio.h>

/*
#define _DEBUG_
*/


/*!
    Builds a classified examples table from a set of observed examples.

    \param fname_i File name of the observed examples.
    \param comp_prob A flag indicating if the probabiities should be computed (0=NO, 1=YES).
    \param freqmin Minimum frequency. If the example has frequency below this value it is ignored.
    \param estim Decision estimator.
    \param comptype Type of computation for undecidable cases.
    \param undflabel Label to use in the undecidable cases, if comptype == 1.
    \return 1 on success. 0 on failure.
*/

int ldecision(char *fname_i, int comp_prob, double freqmin, double estim, int comptype, int undflabel, char *fname_o) {
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
    /*apert_t  *apt = NULL ;*/


#ifdef _DEBUG_
pac_debug("Entered ldecision with if=%s e of=%s", fname_i, fname_o) ;
#endif

    /* read observed examples file */
    if(NULL == (xpl = (xpl_t *)xpl_read(fname_i, &win/*, &apt*/)))
      return trios_error(MSG, "xpl_read failed.") ;


    if(NULL == (mtm = (mtm_t *)mtm_decide(xpl, comp_prob,
                freqmin, estim, comptype, undflabel ))) {
      win_free(win) ;
      xpl_free(xpl) ;
      return trios_error(MSG, "mtm_decide failed.") ;
    }

    /* write the classified examples table to the output file */
    if(!mtm_write(fname_o, mtm, win/*, apt*/)) {
      win_free(win) ;
      xpl_free(xpl) ;
      mtm_free(mtm) ;
      return trios_error(MSG, "mtm_write failed.") ;
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


/*!
    Builds the classified examples set.

    \param xpl Examples set.
    \param comp_prob A flag indicating if the probabiities should be computed (0=NO, 1=YES).
    \param freqmin Minimum frequency. If the example has frequency below this value it is ignored.
    \param estimator Decision estimator.
    \param comptype Type of computation for undecidable cases.
    \param undflabel Label to use in the undecidable cases, if comptype == 1.
    \return The created classified examples set.
  */
mtm_t *mtm_decide(xpl_t *xpl, int   comp_prob, double freqmin, double estimator, int comptype,int undflabel) {
/*  author: Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)                 */
/*  date: Tue Oct 29 1996                                                   */

/*  Modification by:  Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)       */
/*  Date: Tue Jan  6 1998                                                   */
/*  Mod: Added a new parameter - comp_prob                                  */
/*  Mod: Routine modified to compute the probabilities P(x) and p(y|x) if   */
/*       comp_prob== 1                                                      */

/*  Modification by:  Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)       */
/*  Date: Sun Feb 20 2000                                                   */
/*  Mod: treatment of the undecidable cases (tie condition) fixed and       */
/*       improved. New parameter comptype introduced                        */

  mtm_t *mtm ;
  int   type, wsize ;
  unsigned int nmtm, sum ;


  wsize = xpl_get_wsize( xpl ) ;

  type  = xpl_get_type( xpl ) ;

  if((type!=BB) && (comp_prob)) {
    return (mtm_t *)trios_error(1,
      "Probabilities can be computed only for binary to binay operators.\
       Other cases not implemented yet.") ;
  }

  sum = xpl_get_sum ( xpl ) ;

  /* The number of examples in the classified examples set may be
     smaller than in the input examples set because some examples
     may be ignored due to their small frequency. However, we decided
     that the classified examples set will have the same number of nodes
     (i.e., rows) of the examples set because their size will be very
     close after the decision. The previous version of this routine
     used linked lists to save memory, but we noticed that it was
     time consuming and it's preferable to have a memory consuming
     routine (but not too excessively, of course) than time consuming. */

  nmtm  = xpl_get_n_nodes( xpl ) ;

#ifdef _DEBUG_1_
  pac_debug("nmtm=%d, type=%d", nmtm, xpl->type) ;
#endif

  if (NULL == (mtm = (mtm_t *)mtm_create( wsize, type, nmtm ))) {
    return(mtm_t *)trios_error(MSG, "mtm_decide: mtm_create failure") ;
  }

#ifdef _DEBUG_
  pac_debug("mtm_create() done. wsize=%d, type=%d, nmtm=%d", wsize, type, nmtm) ;
#endif


  /* Now we've to reset the value of nmtm because it will be different
     depending on the decisions                                           */

  mtm_set_nmtm( mtm, 0 );

  switch ( type ) {

    case BB: {

      if (undflabel > 1) {
        undflabel = 1 ; /* Nonsense to put a label > 1 for BB */
        trios_warning("Label for the undecidable examples will be 1.") ;
      }

      /* For binary cases, the estimators media, moda and median will be set
     to be the same                                                     */

      if ( (estimator != MABS) && ((estimator > 1) || (estimator < 0.5)) ) {
    estimator = (float) AVERAGE ;
      }

#ifdef _DEBUG_
      pac_debug("BIN mtmdecide estimador = %f \n", estimator) ;
#endif

      /* This is a call for the RECURSIVE VERSION
      if(!decide_tree_bb((xpl_BB *)xpl->root, mtm, sum, xpl->wzip,
                          freqmin, estimator, undflabel, comp_prob)) {
    xpl_free( xpl ) ;
        mtm_free( mtm ) ;
        return(mtm_t *)pac_error(MSG, "mtm_decide: decide examples failure") ;
      }
      */

      if(!decide_ll_bb( xpl, mtm, freqmin, estimator, undflabel, comp_prob )) {
    mtm_free( mtm ) ;
    return(mtm_t *)trios_error(MSG, "mtm_decide: decide examples failure") ;
      }

      mtm_set_comp_prob( mtm, comp_prob ) ;

      break ;
    }

    case BG: {
        return (mtm_t *) trios_error(1, "BG operators not supported yet");
        break ;
    }

    case GG:
    case WKC:
    case WKF:
    case WK3F:
    case WK3C:
    case WKGG2F:
    case WKGG2C:
    case GG3: {
        mtm_free( mtm ) ;
        return (mtm_t *) trios_error(1, "Gray-level operators not supported yet");
        break ;
    }

    default: {
        mtm_free( mtm ) ;
        return (mtm_t *)trios_error(1,
                   "mtm_decide: type GB not implemented yet.") ;
        break ;
    }
  }
  return(mtm);
}


