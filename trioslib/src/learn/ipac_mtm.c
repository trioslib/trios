#include <paclib_kernel.h>
#include "paclearn_local.h"

/* #define _DEBUG_ */
/* #define _DEBUG_1_ */

/*  Modification by:  Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)       */
/*  Date: Tue Jan  6 1998                                                   */
/*  Mod: mtm_decide changed to compute probabilities P(x) and p(y|x) if     */
/*       comp_prob==1                                                       */
/*  Mod: recursive routine to do the decision changed by a non-recursive    */
/*       in the BB case (decide_ll_bb)                                      */

/*  Modification by:  Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)       */
/*  Date: Tue Jan 13 1998                                                   */
/*  Mod: Modified to treat the case WK                                      */

/*  Modification by:  Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)       */
/*  Date: Thu Aug 19 1999                                                   */
/*  Mod: each node of the mtm table will incorporate a field fq, which      */
/*       tells how many times the example has been observed during          */
/*       sampling (or when that is not possible, gives an idea of the       */
/*       frequency of that example)                                         */



/*
     -------------------------------------------
     FUNCTION: MTM_DECIDE
     -------------------------------------------
*/

mtm_t               /*+ Purpose: Builds the mtm table from the examples    +*/
 *mtm_decide(
   xpl_t *xpl,      /*+ In: pointer to examples                            +*/
   int   comp_prob, /*+ In: A flag indicating if the probabiities should be
                            computed (0=NO, 1=YES)                         +*/
   double freqmin,   /*+ In: Minimun frequency of an example to be accepted +*/
   double estimator, /*+ In: Decision Estimator                             +*/
   int   comptype,  /*+ In: type of computation for the undecidable cases  +*/
   int   undflabel  /*+ In: Value to be assigned to the undecidable
                            examples, if comptype==1                       +*/
)
/*+ Return: Pointer to mtm                                                 +*/
{
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
    return (mtm_t *)pac_error(1,
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
    return(mtm_t *)pac_error(MSG, "mtm_decide: mtm_create failure") ;
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
        pac_warning("Label for the undecidable examples will be 1.") ;
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
	return(mtm_t *)pac_error(MSG, "mtm_decide: decide examples failure") ;
      } 

      mtm_set_comp_prob( mtm, comp_prob ) ;

      break ;
    }   
    
    case BG: {
        return (mtm_t *) pac_error(1, "BG operators not supported yet");
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
        return (mtm_t *) pac_error(1, "Gray-level operators not supported yet");
        break ;
    }   
    
    default: {  
        mtm_free( mtm ) ;
        return (mtm_t *)pac_error(1,
			       "mtm_decide: type GB not implemented yet.") ;
        break ; 
    } 
  }
  return(mtm);
}



/*
     ------------------------------------------------------------
     FUNCTION: DECIDE_TREE_BB - Recursive version of the decision
     ------------------------------------------------------------
*/

int                  /*+ Purpose: Makes the decision  for BB and BG         +*/
  decide_tree_bb(
    xpl_BB *p,       /*+ In: Begining  of the examples                      +*/
    mtm_t *mtm,      /*+ In/Out:  Pointer to minterms                       +*/
    unsigned int sum,/*+ In: Total number of examples                       +*/
    int   wzip,      /*+ In: Number of bytes to put wpatern                 +*/
    double freqmin,   /*+ In: Minimal frequency to consider an example, in % +*/
    double estimator, /*+ In: Decision Estimator                             +*/
    int   undflabel  /*+ In: Value assigned to the undecidable examples     +*/
)
/*+ Return: 1 on Success 0 on Failure                                       +*/ 
{
/*  author: Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)                  */
/*  date: Wed Oct 30 1996                                                    */
  
  float     verify;
  int       wpat_max ;
  unsigned int wpat_freq ;

  if(p) {
    
    if(!decide_tree_bb(p->left, mtm, sum, wzip, freqmin, estimator, undflabel)) 
       return pac_error(MSG, "decide_tree_bb failure") ;
        
    wpat_freq = p->fq0 + p->fq1 ;
       
    verify = ((float)wpat_freq / sum) * 100 ;
    
#ifdef _DEBUG_
pac_debug("mtmbbtree wpat_freq=%d, sum=%d, verify=%f wzip=%d", wpat_freq, sum, verify,wzip) ;
#endif

    if(verify >= freqmin) {
      
      if (estimator == AVERAGE) {
	  
	int wpat_label ;
	
	if (p->fq0 < p->fq1) 
	  wpat_label = 1 ;
	else 
	  wpat_label = 0 ; 

#ifdef _DEBUG_
pac_debug("mtmbbtree Vai inserir wpat[0]=%d label=%d", p->wpat[0], wpat_label) ;
#endif
	
	if(!mtm_BX_insert( mtm, (int)mtm->nmtm , wzip, p->wpat, wpat_label, 0, 0)) {
          return pac_error(MSG, "decide_tree_bb failure") ;
        }

        mtm->nmtm++ ; 

      }
	
      else if (estimator == MABS) {

	int wpat_label ;
	
	if ((p->fq0 == 0) || (p->fq1 == 0)) 
	  wpat_label = (p->fq0 == 0) ? 1 : 0 ; 
	else
	  wpat_label = undflabel ;
	
#ifdef _DEBUG_
pac_debug("mtmbbtree Vai inserir wpat[0]=%d label=%d", p->wpat[0], wpat_label) ;
#endif

	if(!mtm_BX_insert( mtm, (int)mtm->nmtm , wzip, p->wpat, wpat_label, 0, 0 )) {
          return pac_error(MSG, "decide_tree_bb failure") ;
        }

	/*	
	mtm->mtm_freq[wpat_label] += wpat_freq ;
	*/

        mtm->nmtm++ ; 

      }

      else {
	
	int wpat_label ;
	
	if (p->fq0 < p->fq1) {
	  wpat_max = p->fq1 ;
	  wpat_label = 1 ;
	}
	else { 
	  wpat_max = p->fq0 ;
	  wpat_label = 0 ; 
	}
	
	if( ((float)wpat_max / wpat_freq) >= estimator ) {
	  if(!mtm_BX_insert( mtm, (int)mtm->nmtm , wzip, p->wpat, wpat_label, 0, 0)) {
	    return pac_error(MSG, "decide_tree_bb failure") ;
	  }
	}
	else {
	  if(!mtm_BX_insert( mtm, (int)mtm->nmtm , wzip, p->wpat, undflabel, 0, 0)) {
	    return pac_error(MSG, "decide_tree_bb failure") ;
	  }
	}
	mtm->nmtm++ ;
      }

    }
    
    if(!decide_tree_bb(p->right, mtm, sum, wzip, freqmin, estimator, undflabel)) 
      return pac_error(MSG, "decide_tree_bb failure") ;

  } 
  
  return(1);

}


/*
     -------------------------------------------
     FUNCTION: decide_ll_bb
     -------------------------------------------
*/
 
int                  /*+ Purpose: Makes the decision for BB case            +*/
  decide_ll_bb(
    xpl_t *xpl,      /*+ In: set of examples                                +*/
    mtm_t *mtm,      /*+ In/Out:  Pointer to minterms                       +*/
    double freqmin,   /*+ In: Minimal frequency to consider an example, in % +*/
    double estimator, /*+ In: Decision Estimator                             +*/
    int   undflabel, /*+ In: Value assigned to the undecidable examples     +*/
    int   comp_prob  /*+ In: flag indicating if prob. should be computed    +*/
)
/*+ Return: 1 on Success 0 on Failure                                       +*/ 
{ 
/*  author: Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)                 */
/*  date: Tue Jan  6 1998                                                   */

/*  Modification by:  Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)       */
/*  Date: Mon Feb 23 1998                                                   */
/*  Mod: estimator==AVERAGE case modified to assign the conflicting case    */
/*       label to the example when freq0==freq1 (instead of 0)              */

/*  Modification by:  Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)       */
/*  Date: Tue Mar 10 1998                                                   */
/*  Mod: Probabilities changed from "float" to "double"                     */

/*  Modification by:  Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)       */
/*  Date: Thu Aug 19 1999                                                   */
/*  Mod: instead keeping frequency information as probabilities, now they   */
/*       will be kept as integer numbers. The frequency of a given pattern  */
/*       is now information included in the mtm file. However, the number   */
/*       of times a given pattern has been observed with label 1 will       */
/*       be an optional information (option comp_prob must be set to 1      */
/*       in order include this info to the file)                            */      

  float     verify ;
  int       wpat_max, wpat_label, wzip ;
  unsigned  int sum, wpat_freq ; 
  xpl_BB    *head ;


  /* Transform the tree structure of xpl->root to a linked list structure */
  /* in order to make it useful in a non-recursive algorithm              */
  xpl_tree2ll(xpl, 1) ;
  head = (xpl_BB *)xpl->root ;
  sum = xpl_get_sum(xpl) ;
  wzip = size_of_zpat(xpl_get_wsize(xpl)) ;


  /* If estimator is AVERAGE the most frequent label is assigned to */
  /* w-pattern                                                      */
  if (estimator == AVERAGE) {
	
    while(head) {
      wpat_freq = head->fq0 + head->fq1 ;
       
      verify = ((float)wpat_freq / sum) * 100 ;
    
      if(verify >= freqmin) {

	if (head->fq0 < head->fq1) {
	  wpat_label = 1 ;
        }
	else if(head->fq0 > head->fq1) {  /* Mod. Mon Feb 23 1998 */
	  wpat_label = 0 ;
        }
        else {                          /* Mod. Mon Feb 23 1998 */
          wpat_label = undflabel ;         /* Mod. Mon Feb 23 1998 */
        }

        if(!comp_prob) {
	  if(!mtm_BX_insert( mtm, (int)mtm->nmtm, wzip, head->wpat,
                             wpat_label, wpat_freq, 0 )) {
            return pac_error(MSG, "mtm_BX_insert() failed.") ;
          }
        }
        else {
	  if(!mtm_BX_insert(mtm, (int)mtm->nmtm, wzip, head->wpat,
                            wpat_label, wpat_freq, head->fq1)) {
            return pac_error(MSG, "mtm_BX_insert() failed.") ;
          }
        }
        mtm->nmtm++ ; 

      }
      head = head->right ;
    }
  }
  /* if estimator==MABS, then we have a decidable case only if     */
  /* there is just one label assigned to the example.              */
  /* Otherwise the example is undecidable, and the label specified */
  /* by the user will be used (undflabel)                             */
  else if (estimator == MABS) {
    while(head) {
      wpat_freq = head->fq0 + head->fq1 ;
       
      verify = ((float)wpat_freq / sum) * 100 ;
    
      if(verify >= freqmin) {
	if ((head->fq0 == 0) || (head->fq1 == 0)) 
	  wpat_label = (head->fq0 == 0) ? 1 : 0 ; 
	else
	  wpat_label = undflabel ;

        if(!comp_prob) {
	  if(!mtm_BX_insert( mtm, (int)mtm->nmtm, wzip, head->wpat,
                             wpat_label, wpat_freq, 0 )) {
            return pac_error(MSG, "mtm_BX_insert() failed.") ;
          }
        }
        else {
	  if(!mtm_BX_insert(mtm, (int)mtm->nmtm, wzip, head->wpat,
                            wpat_label, wpat_freq, head->fq1 )) {
            return pac_error(MSG, "mtm_BX_insert() failed.") ;
          }
        }

        mtm->nmtm++ ; 
      }
      head = head->right ;
    }
  }

  else {
    while(head) {
      wpat_freq = head->fq0 + head->fq1 ;
       
      verify = ((float)wpat_freq / sum) * 100 ;
    
      if(verify >= freqmin) {
	if (head->fq0 < head->fq1) {
	  wpat_max = head->fq1 ;
	  wpat_label = 1 ;
	}
	else { 
	  wpat_max = head->fq0 ;
	  wpat_label = 0 ; 
	}

	if( ((float)wpat_max / wpat_freq) < estimator ) {
          wpat_label = undflabel ;
        }
        if(!comp_prob) {
	  if(!mtm_BX_insert( mtm, (int)mtm->nmtm, wzip, head->wpat,
                             wpat_label, wpat_freq, 0 )) {
            return pac_error(MSG, "mtm_BX_insert() failed.") ;
          }
        }
        else {
	  if(!mtm_BX_insert(mtm, (int)mtm->nmtm, wzip, head->wpat,
                            wpat_label, wpat_freq, head->fq1)) {
            return pac_error(MSG, "mtm_BX_insert() failed") ;
          }
        }

	mtm->nmtm++ ;
      }
      head = head->right;
    }

  }

  return(1) ;  
}




