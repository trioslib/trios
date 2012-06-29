#include <trios.h>
#include "trios_error.h"

 /*#define _DEBUG_
 #define _DEBUG_2_*/

/*
     -------------------------------------------
     FUNCTION: MTM CREATE
     -------------------------------------------
*/

mtm_t *          /*+ Purpose: allocate a MTM structure                     +*/
  mtm_create(
    int wsize,   /*+ In: w-pattern size                                    +*/
    int type,    /*+ In: transformation type (BB, BG, GB, GG or WKGG)      +*/
    unsigned int nmtm  /*+ In: number of minterms                          +*/
)
/*+ Return: a pointer to the MTM structure                                 +*/
{
  mtm_t *mtm ;

  if((mtm=(mtm_t *)malloc(sizeof(mtm_t))) == NULL) {
    return (mtm_t *)trios_error(1, "Memory allocation failed.") ;
  }

  mtm->wsize = wsize ;
  mtm->type = type ;
  mtm->nmtm = nmtm ;
  mtm->mtm_freq = NULL ;
  mtm->mtm_data = NULL ;
  mtm->comp_prob = 0 ;
  mtm->nsum = 0 ;

  switch(type) {
    
  case BB :
  case BG : {
      
    if(!(mtm->mtm_data = (int *) malloc( sizeof(mtm_BX) * nmtm))) {
      free ( mtm ) ;
      return 
	(mtm_t *)trios_error(1, "mtm_create: memory allocation error for mtm") ;
    }
    break ;
  }

  case GB :
  case GG:
  case WKC:
  case WKF:
  case WK3F:
  case WK3C:
  case WKGG2F:
  case WKGG2C:
  case GG3: {
    trios_error(MSG, "Operator not supported");
    /*
    if(!(mtm->mtm_data = (int *) malloc( sizeof(mtm_GX) * nmtm))) {
      free ( mtm ) ;
      return 
	(mtm_t *)trios_error(1, "mtm_create: memory allocation error for mtm") ;
    }*/
    break ;
  }
  
  default : {
      return 
	(mtm_t *)trios_error(1, "mtm_create: Invalid type, not BB or BG or GB or GG or WKGG") ;
      break; }
  }

  return(mtm) ;
  
}




/*
     -------------------------------------------
     FUNCTION: MTM FREE
     -------------------------------------------
*/
void                      /*+ Purpose: free memory area used MTM structure  +*/
  mtm_free(
    mtm_t *mtm            /*+ In: pointer to the MTM structure              +*/
)
/*+ Return: nothing                                                         +*/ 
{

  int i ;
  mtm_BX *table_BX ; 
  /*mtm_GX *table_GX ;*/

  if (mtm==NULL) return ;

  if(mtm->mtm_freq) {
    freqlist_free(mtm->mtm_freq) ; 
  }

  if(mtm->mtm_data) {
    switch ( mtm->type ) {
      case BB:
      case BG: {
      
	table_BX = (mtm_BX *)mtm->mtm_data ; 
        for (i = 0; i < mtm->nmtm; i++) 
	  if(table_BX[i].wpat) free( table_BX[i].wpat ) ;
	break;
      }	

      case GB :
      case GG:
      case WKC:
      case WKF:
      case WK3F:
      case WK3C:
      case WKGG2F:
      case WKGG2C:
      case GG3: {
        trios_error(MSG, "Operator not supported");
        /*
 	table_GX = (mtm_GX *)mtm->mtm_data ; 
        for (i = 0; i < mtm->nmtm; i++) 
      free( table_GX[i].wpat ) ;*/
	break;
      }	

      default : {
        trios_error(MSG, "free_mtm failure. Invalid type (not BX or GX).");
        break;
              
      }
    }
	  
    free(mtm->mtm_data) ;
  } 

  free(mtm) ;

}




/*
     -------------------------------------------
     FUNCTION: MTM GET SIZE
     -------------------------------------------
*/
int                 /*+ Purpose: get the size of w-pattern                  +*/
  mtm_get_wsize(
    mtm_t *mtm      /*+ In: pointer to the MTM                              +*/
)
/*+ Return: the size of w-pattern (integer between 1 and 256)               +*/ 
{
  return(mtm->wsize) ;
}




/*
     -------------------------------------------
     FUNCTION: MTM GET TYPE
     -------------------------------------------
*/
int                 /*+ Purpose: get the type of the labeled examples      +*/
  mtm_get_type(
    mtm_t *mtm      /*+ In: pointer to the MTM                             +*/
)
/*+ Return: the type of w-pattern                                          +*/
{
  return(mtm->type) ;
}




/*
     -------------------------------------------
     FUNCTION: MTM GET NMTM
     -------------------------------------------
*/
unsigned int        /*+ Purpose: get the number of minterms                +*/
  mtm_get_nmtm(
    mtm_t *mtm      /*+ In: pointer to the MTM                             +*/
)
/*+ Return: the number of minterms                                         +*/
{
  return(mtm->nmtm) ;
}


/*
     -------------------------------------------
     FUNCTION: MTM GET NSUM
     -------------------------------------------
*/
unsigned int        /*+ Purpose: get the number of seen examples           +*/
  mtm_get_nsum(
    mtm_t *mtm      /*+ In: pointer to the MTM                             +*/
)
/*+ Return: the number of seen examples                                    +*/
{
  return(mtm->nsum) ;

}


/*
     -------------------------------------------
     FUNCTION: MTM GET COMP_PROB
     -------------------------------------------
*/
int                 /*+ Purpose: get the comp_prob flag value              +*/
  mtm_get_comp_prob(
    mtm_t *mtm      /*+ In: pointer to the MTM                             +*/
)
/*+ Return: comp_prob value (0 or 1)                                       +*/
{
  return(mtm->comp_prob) ;

}




/*
     -------------------------------------------
     FUNCTION: mtm_get_freqlist
     -------------------------------------------
*/
 
freq_node *     /*+ Purpose: Get list of frequencies                        +*/
  mtm_get_freqlist(                                                                   
    mtm_t *mtm_i       /*+ In: classified examples set                      +*/
)
/*+ Return: freqlist                                                        +*/
{
  freq_node    *p ;

  p = mtm_i->mtm_freq ;

  return(p) ;
}



/*
     -------------------------------------------
     FUNCTION: MTM SET SIZE
     -------------------------------------------
*/
void                /*+ Purpose: set the size of w-pattern                 +*/
  mtm_set_wsize(
    mtm_t *mtm,     /*+ In: pointer to the MTM                             +*/
    int wsize       /*+ In: number of variables                            +*/
)
/*+ Return: nothing                                                        +*/
{
  mtm->wsize = wsize ;

}





/*
     -------------------------------------------
     FUNCTION: MTM SET TYPE
     -------------------------------------------
*/
void                /*+ Purpose: set the type of a labeled example         +*/
  mtm_set_type(
    mtm_t *mtm,     /*+ In: pointer to the MTM                             +*/
    int  type       /*+ In: type of the w-patterns                         +*/
)
/*+ Return: nothing                                                        +*/
{
  mtm->type = type ;

}





/*
     -------------------------------------------
     FUNCTION: MTM SET NMTM
     -------------------------------------------
*/
void                /*+ Purpose: set the number of minterms                +*/
  mtm_set_nmtm(
    mtm_t *mtm,         /*+ pointer to the MTM                             +*/
    unsigned int nmtm   /*+ In: number of minterms                         +*/  
)
/*+ Return: nothing                                                        +*/
{
  mtm->nmtm = nmtm ;

}



/*
     -------------------------------------------
     FUNCTION: MTM SET Nsum
     -------------------------------------------
*/
void                /*+ Purpose: set the number of seen examples           +*/
  mtm_set_nsum(
    mtm_t *mtm,        /*+ pointer to the MTM                              +*/
    unsigned int nsum  /*+ In: number of minterms                          +*/  
)
/*+ Return: nothing                                                        +*/
{

  mtm->nsum = nsum ;

}


/*
     -------------------------------------------
     FUNCTION: MTM SET COMP_PROB
     -------------------------------------------
*/
void                /*+ Purpose: get the comp_prob flag value              +*/
  mtm_set_comp_prob(
    mtm_t *mtm,     /*+ In: pointer to the MTM                             +*/
    int   comp_prob /*+ In: comp_prob flag (0 or 1)                        +*/
)
/*+ Return: comp_prob value (0 or 1)                                       +*/
{

  mtm->comp_prob = comp_prob ;

}



/*
     -------------------------------------------
     FUNCTION: MTM_BX_insert 
     -------------------------------------------
*/
int                       /*+ Purpose: inserts a minterm in the table      +*/
  mtm_BX_insert(
    mtm_t *mtm,           /*+ In: pointer to mtm structure                 +*/ 
    int index1,            /*+ In: insert position                          +*/
    int wzip,             /*+ In: wpat size in bytes                       +*/
    unsigned int *wpat,   /*+ In: compacted pattern                        +*/
    int label,            /*+ In: label of the inserted minterm            +*/
    unsigned int fq,      /*+ In: frequency of wpat                        +*/
    unsigned int fq1      /*+ In: 1-frequency of wpat                      +*/
)
/*+ Return: 1 on success 0 on failure                                      +*/
{
  mtm_BX *p ;
  freq_node *freqlist, *freqnode ;
  int j ;


  p = (mtm_BX *)mtm->mtm_data ;

  freqlist = (freq_node *) mtm->mtm_freq ;

  p[index1].wpat = (unsigned int *)malloc(sizeof(int)*wzip) ;
  if(!p[index1].wpat) {
    return trios_error(1, "Memory allocation failed.") ;
  }

  for ( j = 0; j < wzip; j++) 
    p[index1].wpat[j] = wpat[j] ;

  p[index1].label = label ; /* Here we removed a cast to char */

  if((freqnode = freq_node_create(label, 1))==NULL) {
    return trios_error(MSG, "mtm_insert: freq_node_create() failed.") ;
  }

  set_freq(freqnode, &freqlist) ; /* Insert the node to the list */

  mtm->mtm_freq = freqlist ;
  mtm->nsum = mtm->nsum + fq ;

  p[index1].fq = fq ;
  p[index1].fq1 = fq1 ;

  return(1) ;
}






/*
     -------------------------------------------
     FUNCTION: mtm_sep
     -------------------------------------------
*/
 
int             /*+ Purpose: Separate examples of a given class k          +*/
  mtm_sep(
    mtm_t *mtm_i,      /*+ In/Out: classified examples set                 +*/
    int   k,           /*+ In: class to be separated                       +*/
    mtm_t **mtm_o1,    /*+ In/Out: examples of class k                     +*/
    mtm_t **mtm_o2     /*+ In/Out: examples of classes other than k        +*/
  )
/*+ Return: 1 on success, 0 on failure                                     +*/
{
  mtm_BX   *p ;
  int      ct1, ct2 ;
  int      wsize, wzip, type, comp_prob ;
  unsigned int nmtm_i, nmtm_1, nmtm_2 ;
  int      i ;


  type  = mtm_get_type( mtm_i ) ;
  wsize = mtm_get_wsize( mtm_i ) ;
  comp_prob = mtm_get_comp_prob( mtm_i ) ;
  wzip  = size_of_zpat( wsize ) ;

  nmtm_i = mtm_get_nmtm( mtm_i ) ;
  nmtm_1 = mtm_count( mtm_i, k ) ;
  nmtm_2 = nmtm_i - nmtm_1 ;

  *mtm_o1 = mtm_create( wsize, type, nmtm_1 );
  *mtm_o2 = mtm_create( wsize, type, nmtm_2 ) ;

  if((*mtm_o1==NULL) || (*mtm_o2 == NULL)) {
    return trios_error(MSG, "mtm_sep: mtm_create() failed.") ;
  }

  p = (mtm_BX *)mtm_i->mtm_data ;
  ct1 = ct2 = 0 ;
  i = 0 ;

  while(i < nmtm_i) {

    if(p[i].label == k) {
      if(!mtm_BX_insert( *mtm_o1, ct1, wzip, p[i].wpat, p[i].label,
                        p[i].fq, p[i].fq1 )) {
        mtm_free(*mtm_o1) ;
        return trios_error(MSG, "select: mtm_BX_insert() failed.") ;
      }
      ct1++ ;
      /* *mtm_o1->mtm_freq[p[i].label]++ ; */
    }
    else {
      if(!mtm_BX_insert( *mtm_o2, ct2, wzip, p[i].wpat, p[i].label,
                        p[i].fq, p[i].fq1 )) {
        mtm_free(*mtm_o2) ;
        return trios_error(MSG, "select: mtm_BX_insert() failed.") ;
      }
      ct2++ ;
      /*      *mtm_o2->mtm_freq[p[i].label]++ ; */
    }

    i++ ;
  }

  if(comp_prob) {
    mtm_set_comp_prob(*mtm_o1, 1) ;
    mtm_set_comp_prob(*mtm_o2, 1) ;
  }

  /*
  mtm_i->mtm_data = NULL ;
  mtm_i->nmtm = 0 ;
  */

  return(1) ;         
}


/*
     -------------------------------------------
     FUNCTION: mtm_count
     -------------------------------------------
*/
 
unsigned int    /*+ Purpose: Count examples in class k                     +*/
  mtm_count(                                                                  
    mtm_t *mtm_i,      /*+ In: classified examples set                     +*/
    int   k            /*+ In: class number                                +*/
)
/*+ Return: the number of examples in class k                              +*/
{

  freq_node    *p ;
  unsigned int counter ;

  counter = 0 ;

  p = mtm_i->mtm_freq ;
  while(p) {
    if(p->label == k) counter = p->freq ;
    p = p->next ;
  }

  return(counter) ;
}



/*
     -------------------------------------------
     FUNCTION: mtm_index_by_freq_count
     -------------------------------------------
*/
 
unsigned int    /*+ Purpose: Index mtm by frequencies                      +*/
  mtm_index_by_freq_count(                                                                  
    mtm_t *mtm_i,      /*+ In: classified examples set                     +*/
    int   *indexes     /*+ In: vector of indexes                           +*/
)
/*+ Return: 1 on success or 0 on failure                                   +*/
{
  unsigned int i, freqmax, nlabels, fnow, flast, fold, nmtm ;
  freq_node    *p, *q, *p_last, *qlist, *qmax, *q_last, *p_zero ;

  mtm_BX *bxdata ;
  /*mtm_GX *gxdata ;*/

  nlabels = 0 ;
  p_zero = NULL ;

  /* Get the freqlist and copy it */
  p = mtm_get_freqlist(mtm_i) ;
  nmtm = mtm_get_nmtm(mtm_i) ;

  indexes = (unsigned int *)malloc(sizeof(int)*nmtm) ;
  if(!indexes) {
    return trios_error(1, "mtm_index_by_freq: Memory allocation failed for indexes.") ;
  }
  
  while(p) {
    if((q = freq_node_create(p->label, p->freq))==NULL) {
      return trios_error(MSG, "mtm_index_by_freq: freq_node_create() failed.") ;
    }    
    set_freq(q, &qlist) ; /* Insert the node to the list */
    nlabels++ ;
    p = p->next ;
  }

  p = p_last = qlist ;  /* release pointer */
  q = NULL ;  
  

  /* search list for label 0 */

  while(p) {
    if (p->label == 0) {
      nlabels-- ;
      p_last->next = p->next ;
      p_zero = p ;
      p_zero->next = NULL ;
      break ;
    }
    p_last = p ;
    p = p->next ;
  }
  
  p = p_last = qlist ;

  /* sort all labels but 0 */
  for (i = 0; i < nlabels; i++) {
    q_last = NULL ;
    qmax = NULL ;
    freqmax = 0 ;
    /* find the label which has the largest frequency */
    while(p) {
      if (p->freq > freqmax) {
	freqmax = p->freq ;
	qmax = p ;
	q_last = p_last ;
      }
      p_last = p ;
      p = p->next ;
    }
    /* put the node into q list and update qlist */
    q_last->next = qmax->next ;
    qmax->next = q ;
    q = qmax ;
  }
  
  p_zero->next = q ;
  q = p_zero ;

  p = q ;
  flast = fold = 0 ;
  
  /* update cummulative histogram */

  while (p) {
    fnow = p->freq ;
    p->freq = flast = flast + fold ;
    fold = fnow ;
  }

  /* build the indexes table of mtm                      */
  /* indexes[indexes for label starts at] = i            */

  if (mtm_get_type(mtm_i) == BG) {

    bxdata = (mtm_BX *)mtm_i->mtm_data ;

    for (i = 0; i < nmtm; i++) {
      p = q ;
      while (p) {
	if (p->label == bxdata[i].label) {
	  indexes[p->freq] = i ;
	  p->freq = p->freq + 1 ;
	  break ;
	}
      }
    }

  }
  else {
    return trios_error(MSG, "Operator not supported");
    /*gxdata = (mtm_GX *)mtm_i->mtm_data ;
      
    for (i = 0; i < nmtm; i++) {
      p = q ;
      while (p) {
	if (p->label == gxdata[i].label) {
	  indexes[p->freq] = i ;
	  p->freq = p->freq + 1 ;
	  break ;
	}
      }
    }*/
  }

  return(1) ;
}







/*
     -------------------------------------------
     FUNCTION: mtm_compute_mae
     -------------------------------------------
*/
 
int             /*+ Purpose: given a mtm table, computes the mae           +*/
  mtm_compute_mae(
    mtm_t *mtm,        /*+ In: mtm table                                   +*/
    double *mae_error  /*+ Out: mae                                        +*/
)
/*+ Return: 1 on success, 0 on failure                                     +*/
{
  mtm_BX *q ;
  int    i, nmtm ;
  int    FMAE ;

  /* ---------------------------------------------------------------------

     How to compute MAE of psi
     ^^^^^^^^^^^^^^^^^^^^^^^^^
  
     if px = P(y=1|X=x) and Px = P(X=x), then

     MAE<psi> = sum { Px.px : psi(x)=0 } + sum { Px.(1-px) : psi(x)=1 } 

     We are using fq(x), fq1(x) and nsum = sum {fq(x) }


     Let FMAE = sum { fq1(x) : psi(x)=0 } + sum { fq(x)-fq1(x) : psi(x)=1 }

     Since Px=fq(x)/nsum  and  px=fq1(x)/fq,

                     fq(x) fq1(x)           fq(x)  fq(x) - fq1(x)
     MAE<psi> = sum {----- ------ } +  sum {----- (--------------) }
                     nsum  fq(x)             nsum      fq(x)
                      fq1(x)                     fq(x) - fq1(x)
              = sum { ----- : psi(x)=0 } + sum { -------------- : psi(x)=1 }
                      nsum                           nsum

     Hence,

            MAE = FMAE/nsum ;

     --------------------------------------------------------------------- */

  /* If frequency of examples are not given, MAE cannot be */
  /* computed.                                             */
  if(!mtm_get_comp_prob(mtm)) {
    return trios_error(1, "Frequency info needed for MAE computation.") ;
  }
  
  q = (mtm_BX *)mtm->mtm_data ;
  nmtm = mtm_get_nmtm(mtm) ;

  FMAE = 0 ;

  for(i=0; i<nmtm; i++) {
    if(q[i].label == 1) {
      FMAE = FMAE + (q[i].fq - q[i].fq1) ;
    }
    else {
      FMAE = FMAE + q[i].fq1 ;
    }
  }

  *mae_error = (double)(FMAE)/(double)(mtm->nsum) ;

  return(1) ;
}





/*
     -------------------------------------------
     FUNCTION: mtm_search_wpat_BX
     -------------------------------------------
*/
 
int             /*+ Purpose: Search examples table for a given wpat        +*/
  mtm_search_wpat_BX(
    unsigned int  nmtm,     /*+ In: size of the table                      +*/ 
    mtm_t         *mtm,     /*+ In: classified examples table              +*/
    unsigned int *wpat,     /*+ In: wpattern                               +*/
    int           wzip      /*+ In: size of wpattern                       +*/
  )
/*+ Return: Index on success, -1 on failure                                +*/
{
  mtm_BX   *p ;
  int      success, result ;
  int      middle, low, high ;
  
#ifdef _DEBUG_
trios_debug("Entering mtm_search_wpat_BX()") ;
#endif

  success = -1 ; low = 0 ; high = nmtm - 1 ;
  
  p = (mtm_BX *)mtm->mtm_data ;

  while(low <= high) {
    middle = (int)((low + high)/2) ;
#ifdef _DEBUG_
trios_debug("middle = %d",middle) ;
#endif
    result = wpat_compare(wpat, p[middle].wpat, wzip) ;
    if(result==0) {
      success = middle ;
      break ;
    }
    else { 
      if(result==1) { /* wpat > p[middle].wpat */
	low = middle + 1 ;
      }
      else high = middle - 1 ; /* wpat < p[middle].wpat */
    }
  }

  return(success) ;         

}




