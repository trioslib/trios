/*  Modification by:  Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)       */
/*  Date: Tue Aug  3 1999                                                   */
/*  Mod: label type modified from char to int                               */

#include <paclib.h>
#include "paclearn_local.h"


/* #define _DEBUG_ */


/* Macro for computing the distance of Hamming between two w-patterns */
#define Dist_hamming(dist, wzip, wpat1, wpat2)   \
        {                                        \
          int ii, jj ;                           \
          unsigned int wpat_xor ;                \
                                                 \
          dist = 0 ;                             \
          for(ii=0; ii<wzip; ii++) {             \
            wpat_xor = (wpat1[ii]^wpat2[ii]) ;   \
            for(jj=0; jj<32; jj++) {             \
              if((wpat_xor & bitmsk[jj])!=0)       \
                dist = dist + 1 ;                \
            }                                    \
          }                                      \
        }

/* Macro to group two adjacent intervals */
#define Set_new_cube(p_new, A1, A2, B1, B2, wzip) \
        {                                         \
          int jj ;                                \
                                                  \
          for(jj=0; jj<wzip; jj++) {              \
            p_new->A[jj] = (A1[jj] & A2[jj]) ;    \
            p_new->B[jj] = (B1[jj] | B2[jj]) ;    \
          }                                       \
          p_new->label = 0 ;                      \
	}

/* Macro to check if an interval exists or not in a list */
#define New_node_exist(exist, p_new, p, wzip) \
        {                                     \
          int flag;                           \
          int ii;                             \
                                              \
          exist = 0 ;                         \
          while(p) {                          \
            flag = 1;                         \
            for(ii=0; ii<wzip ; ii++){        \
              if(p_new->B[ii] != p->B[ii]) {  \
                flag = 0;                     \
                break;                        \
              }                               \
              if(p_new->A[ii] != p->A[ii]) {  \
                flag = 0;                     \
                break;                        \
              }                               \
            }                                 \
            if(flag == 1) {                   \
              exist = 1 ;                     \
              break ;                         \
            }                                 \
            p = p->next;                      \
          }                                   \
	}


/*
     -------------------------------------------
     FUNCTION: group_x
     -------------------------------------------
*/
 
itv_t *           /*+ Purpose: group the examples into intervals           +*/
  group_x(
    mtm_t *mtm    /*+ In: set of classified examples                       +*/
)
/*+ Return: ITV structure on success or NULL on failure                    +*/
{
/* author: Nina S. Tomita & R. Hirata Jr. (nina@ime.usp.br)                 */
/*  date: Thu Jan 22 1998                                                   */

/*  Modification by:  Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)       */
/*  Date: Tue Aug 17 1999                                                   */
/*  Mod: modifications due to modification of mtm_freq field of mtm_t       */
/*       structure. mtm_freq now is a linked list and not a vector.         */

/*  Modification by:  Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)       */
/*  Date: Mon Jan 31 2000                                                   */
/*  Mod: support for BG type mapping                                        */

  itv_t     *itv ;
  itv_BX    *p1, *p2 ;
  int       type, wsize ;
  freq_node *freqlist ;



  /* get some informations from minterms set */
  type  = mtm_get_type(mtm)  ;
  wsize = mtm_get_wsize(mtm) ;

  if((type!=BB) && (type != BG)) {
    return (itv_t *)pac_error(1, "Grouping is possible only for BB type data") ;
  }


  /* create a ITV structure */
  itv = itv_create(wsize, type, 0) ;
  if(itv==NULL) {
    return (itv_t *)pac_error(MSG, "group_x: itv_create() failed.") ;
  }

  /* 
                       At first, for each minterm with a given label
                       an interval is created. These intervals are combined
                       each other (when they are adjacent) and new
                       intervals are generated. At the end of the process,
                       only the largest intervals will remain
                       (the implicant primes). */

  freqlist = (freq_node *) mtm->mtm_freq ;
 
  while(freqlist) {

    /* create a list of intervals corresponding to the minterms
             with label "freqlist->label" */
      p1 = build_itv_from_mtm(mtm, freqlist->label) ;
      if(!p1) {
        return (itv_t *)pac_error(1, "group_x: build_itv_from_mtm() failed.") ;
      }

      /* Apply the Quine-McCluskey's procedure to group the minterms
         with label  freqlist->label                                  */
      p2 = pquine(p1, wsize) ;
      if(!p2) {
        return (itv_t *)pac_error(1, "group_x: quine_like() failed.") ;
      }

      /* Add the intervals generated by pquine() (the maximal intervals
         of elements with label freqlist->label) to the output
         itv structure */
      /* Note: in the next for loop, p1 is used as a auxiliar variable */
      for(; p2; p2=p1) {
        p1 = p2->next ;
        p2->label = freqlist->label ;
        p2->next = (itv_BX *)itv->head ;
        itv->head = (int *)p2 ;
        itv->nitv++ ;
      }
     
    freqlist = freqlist->next ;    
  }

  return(itv) ;        /* returns intervals set */

}




/*
     -------------------------------------------
     FUNCTION: build_itv_from_mtm
     -------------------------------------------
*/

itv_BX *     /*+ Purpose: create an interval with label ZERO for each
                 minterm with label "label" and put the intervals 
                 into a list of BX intervals                           +*/
  build_itv_from_mtm(
    mtm_t *mtm,         /*+ In: set of minterms                        +*/
    int   label         /*+ In: target label                           +*/
)
/*+ Return: a list of BX intervals                                     +*/
{
/* author: Nina S. Tomita & R. Hirata Jr. (nina@ime.usp.br)             */
/*  date: Thu Jan 22 1998                                               */


  itv_BX   *p, *head ;
  mtm_BX   *mtm_data ;
  int      nmtm, wsize, wzip ;
  int      i, j ;


  head = NULL ;

  mtm_data = (mtm_BX *)mtm->mtm_data ;
  nmtm = mtm_get_nmtm(mtm) ;

  wsize = mtm_get_wsize(mtm) ;
  wzip = size_of_zpat(wsize) ;


  for(i=0; i<nmtm; i++) {

    if(mtm_data[i].label == label) {
      p = itv_nodebx_create(wzip) ;
      if(!p) {
	return (itv_BX *)
	  pac_error(MSG, "build_itv_from_mtm: itv_nodebx_create() failed.") ;
      }

      for(j=0; j<wzip; j++) {
        p->A[j] = mtm_data[i].wpat[j] ;          /* left extremity  */
        p->B[j] = mtm_data[i].wpat[j] ;          /* right extremity */
      }

      p->label = 0 ;
      p->next = head ;

      head = p ;
    }

  }

  return(head) ;         
}



/*
     -------------------------------------------
     FUNCTION: pquine
     -------------------------------------------
*/
itv_BX *    /*+ Purpose: given a list of trivial intervals, groups adjacent
                  ones in a iterative process (create a equivalent set
                  of maximal intervals)                                 +*/
  pquine(
    itv_BX *p,          /*+ In: pointer to an intervals list            +*/
    int    wsize        /*+ In: w-pattern size                          +*/
)
/*+ Return: a list of BX intervals on success or NULL on failure        +*/
{
/* author: Nina S. Tomita & R. Hirata Jr. (nina@ime.usp.br)              */
/*  date: Thu Jan 22 1998                                                */


  itv_BX **L, **L_new, **L_aux ;
  itv_BX *x, *next_p, *p_out ;
  int    counter, wzip ;
  int    j, dim, stop ;


  /* create an auxiliar table of pointers to a list of BX intervals */
  L = (itv_BX **)malloc(sizeof(itv_BX *)*(wsize+1)) ;
  if(!L) {
    return (itv_BX *)pac_error(1, "pquine: memory allocation failed.") ;
  }

  /* create another auxiliar table of pointers to a list of BX intervals */
  L_new = (itv_BX **)malloc(sizeof(itv_BX *)*(wsize+1)) ;
  if(!L) {
    return (itv_BX *)pac_error(1, "pquine: memory allocation failed.") ;
  }

  wzip = size_of_zpat(wsize) ;
  p_out = NULL ;

  /* The intervals will be partitioned in such a way that L[i] will
     contain a list of intervals with "i" 1's                        */
  for(j=0; j<=wsize; j++) {
    L[j] = L_new[j] = NULL ;
  }

  while(p) {
    next_p = p->next ;
    counter = wpat_dimension(p->A, wzip) ;
    p->next = L[counter] ;
    L[counter] = p ;
    p = next_p ;
  }

  /* group each of the intervals to other with same dimension and
     create intervals with larger dimension */
  dim = wsize ;
  stop = 0 ;

  while((dim >= 1) && (!stop)) {

#ifdef _DEBUG_
for(j=0;j<=dim; j++) {
  next_p=L[j];
  pac_debug("Os com %d UNS...", j) ;
  while(next_p) {
    pac_debug("     %x", next_p->A[0]) ;
    next_p = next_p->next ;
  }
}
#endif

    /* combine all pairs of intervals in L, when possible, and
       put the new intervals in L_new */
    if((counter = grow_cubes(L, L_new, wsize, dim))==-1) {
      return (itv_BX *)pac_error(MSG, "pquine: grow_cubes() failed.") ;
    }

    /* free the intervals that have been used
       at least once in some combination. Those that have not been
       used are implicant primes, and therefore they are added to
       the output intervals list */

    for(j=0; j<=dim; j++){

      x = L[j] ;

      while(x != NULL){
        next_p = x->next;
        if(x->label == 0){   /* was not used in any combination */
          x->next = p_out ;
          p_out = x ;
        }
        else {               /* was used in some combination */
          itv_nodebx_free(x) ;
        }
        x = next_p ;
      }
      L[j] = NULL ;
    }

    /* no new interval was generated */
    if(!counter) stop = 1 ; 

    L_aux = L;
    L = L_new;      
    L_new = L_aux;
    dim--;

  }

  /* Process the (new) generated intervals from the last iteration */
  for(j=0; j<=dim; j++){

    p = L[j] ;

    while(p != NULL){
      if(p->label == 0){
        next_p = p->next;
        p->next = p_out;
        p_out = p;
        p = next_p ;
      }
      else {
        next_p = p ;
        itv_nodebx_free(p) ;
        p = next_p ;
      }
    }
    L[j] = NULL ;
  }
  

  free(L) ;
  free(L_new) ;

  return(p_out) ;
}


/*
     -------------------------------------------
     FUNCTION: grow_cubes
     -------------------------------------------
*/

int         /*+ Purpose: group intervals that are adjacent to create new ones +*/
  grow_cubes(
    itv_BX **L,       /*+ In: table containing pointers to lists of intervals +*/
    itv_BX **L_new,   /*+ In/Out: table containig pointers to lists of newly
                          created intervals                                   +*/
    int    wsize,     /*+ In: w-pattern's size                                +*/
    int    dim        /*+ In: current step in the iterative grouping process  +*/
)
/*+ Return: an integer                                                        +*/
{
/* author: Nina S. Tomita & R. Hirata Jr. (nina@ime.usp.br)                    */
/*  date: Thu Jan 22 1998                                                      */

  itv_BX *p1, *p2, *p_new, *p_aux ;
  int    exist, stop ;
  int    dist1, dist2, wzip, counter ;
  int    i, j ;


  wzip = size_of_zpat(wsize) ;

  counter = 0 ;

  /* */
  for(i=0; i<dim; i++){

    p1 = L[i] ;
    if(L[i+1] == NULL) p1 = NULL ;

    while(p1 != NULL){

      p2 = L[i+1] ;
      while(p2 != NULL){

        stop = 0 ;
        for(j=0; (j<wzip)&&(!stop); j++) {              
          if((p1->A[j]^p1->B[j])!=(p2->A[j]^p2->B[j])) {
            stop = 1 ;
          }
        }

        if(!stop) {
	  Dist_hamming(dist1, wzip, p1->A, p2->A) ;
	  Dist_hamming(dist2, wzip, p1->B, p2->B) ;

          if((dist1 == 1) && (dist2 == 1)) {
	    p_new = itv_nodebx_create(wzip) ;
	    if(!p_new) {
	      (void)pac_error(MSG, "grow_cubes: itv_nodebx_create() failed.") ;
	      return(-1) ;
	    }

            Set_new_cube(p_new, p1->A, p2->A, p1->B, p2->B, wzip) ;

            p1->label = 1;
            p2->label = 1;

            exist = 1 ;
            p_aux = L_new[i] ;
            New_node_exist(exist, p_new, p_aux, wzip) ;

	    if(!exist) {
              p_new->next = L_new[i];
              L_new[i] = p_new;
              counter++ ;
            }
            else{
              itv_nodebx_free(p_new) ;
            }
          }
        }
        p2 = p2->next;
      }
      p1 = p1->next;
    }
  }

  return(counter);
}                               
