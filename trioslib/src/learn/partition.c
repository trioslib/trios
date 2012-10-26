#include "trios.h"


int            /*+ Purpose: given a set of examples (mtm) determines which
                             is the variable (direction) that most equatively
                             partitions the examples.                      +*/
  which_var(
    mtm_t *mtm,         /*+ In: set of classified examples                  +*/
    int   LIMIT         /*+ In: limit                                       +*/
    )
/*+ Return: the value corresponding to a direction (between 0 and window size),
 NULL on failure        +*/
{

/*  author: Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)                 */
/*  Date: Tue Apr 20 1999                                                   */

  mtm_BX *q ;
  int    aux, i, j, mdiv2, var, min_ct, ct, mask_i ;


  mdiv2 = mtm->nmtm/2 ;             /* one half of examples */




  q = (mtm_BX *)mtm->mtm_data ;     /* pointer to mtm examples */

  var = 0 ; /*min_ct = 10000000 ;*/
  min_ct = LIMIT ;    /* by Martha */

  /* for each variable (0 to wsize-1), counts how many examples
     of mtm are in the interval where the variable is constant.
     Keeps the variable that contains the closest number to one
     half of the examples and return it                         */

  for(i=0; i<mtm->wsize; i++) {
    ct = 0 ;
    aux = i/32 ;
    mask_i = i%32 ;

    for(j=0; j<mtm->nmtm; j++) {
      if(q[j].wpat[aux] & bitmsk[mask_i]) {
        ct++ ;
      }
    }
    if(min_ct > abs(ct-mdiv2)) {
      min_ct = abs(ct-mdiv2) ;
      var = i ;
    }
  }


  return(var) ;
}


int            /*+ Purpose: to partition the set of examples recursively
                            until each of the partitions has no more
                then "threshold" examples			   +*/
  mtm_part(
    mtm_t *mtm,        /*+ In: classified examples                         +*/
    itv_t *itv,        /*+ In: interval                                    +*/
    int   threshold,   /*+ In: Maximun number of examples in each partition+*/
    mtm_t **MTM,       /*+ In/Out: vector of examples subsets              +*/
    itv_t **ITV,       /*+ In/Out: vector of intervals                     +*/
    int   *noper       /*+ In/Out: next partition to be built              +*/
)
/*+ Return: 1 on success, 0 on failure                                     +*/
{

/*  author: Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)                 */
/*  date: Tue Apr 20 1999                                                   */

  itv_t  *itv0, *itv1 ;
  mtm_t  *mtm0, *mtm1 ;
  int    var ;


  if(mtm->nmtm == 0) {
    return(1) ;
  }
  if(*noper > 255) {
    return trios_error(1,
      "Insufficient buffer size. Please contact maintenance people.") ;
  }

  if(mtm->nmtm <= threshold) {
    ITV[*noper] = itv ;
    MTM[*noper] = mtm ;
    *noper = *noper + 1 ;
  }
  else {
    var = which_var(mtm, 10000000) ;

    /* partitions the intervals */
    itv0 = itv1 = NULL ;
    if(!itv_setvar(itv, var, &itv0, &itv1)) {
      return trios_error(MSG,"mtm_part: call to itv_setvar() failed.") ;
    }
    itv_free(itv) ;

    /* partition the set of examples */
    if(!sep_mtm(mtm, itv, var, &mtm0, &mtm1)) {
      return trios_error(MSG,"mtm_part: call to sep_mtm() failed.") ;
    }
    mtm_free(mtm) ;

    if( !mtm_part(mtm0, itv0, threshold, MTM,ITV, noper) ) {
      return trios_error(MSG,"mtm_part: Recursive call to mtm_part() failed.") ;
    }
    if( !mtm_part(mtm1, itv1, threshold, MTM,ITV, noper) ) {
      return trios_error(MSG,"mtm_part: Recursive call to mtm_part() failed.") ;
    }

  }

  return(1) ;

}

/*
     -------------------------------------------
     FUNCTION: sep_mtm
     -------------------------------------------
*/

int             /*+ Purpose: separate the set of classified examples in
                disjoint subsets, according to the decomposition variable  +*/
  sep_mtm(
    mtm_t *mtm,        /*+ In: classified examples                         +*/
    itv_t *itv,        /*+ In: interval                                    +*/
    int   var,         /*+ In: Which direction the partition will be made ?+*/
    mtm_t **mtm0,      /*+ Out: examples with value(var)=0                 +*/
    mtm_t **mtm1       /*+ Out: examples with value(var)=1                 +*/
)
/*+ Return: 1 on succes, 0 on failure                                      +*/
{
/*  author: Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)                 */
/*  date: Wed Jul  8 1998                                                   */


  mtm_BX       *q, *q0, *q1 ;
  unsigned int nmtm, count ;
  int          j, j0, j1, aux, mask_i, wsize ;
  freq_node    *fq, *freqlist1=NULL, *freqlist0=NULL ;


  q0 = q1 = NULL ;
  q = (mtm_BX *)mtm->mtm_data ;
  nmtm = mtm_get_nmtm(mtm) ;
  wsize = mtm_get_wsize(mtm) ;

  /* How many examples with value(var)=ON ? */
  count=0 ;
  aux = var/32 ;
  mask_i = var%32 ;
  for(j=0; j<mtm->nmtm; j++) {
    if(q[j].wpat[aux]&bitmsk[mask_i]) {
      count++ ;
    }
  }

  /* create subsets for the examples */
  *mtm0 = *mtm1 = NULL ;
  if(nmtm-count!=0) {
    *mtm0 = mtm_create(wsize, BB, nmtm-count) ;
    if(*mtm0==NULL) {
      return(0) ;
    }
    q0 = (mtm_BX *)(*mtm0)->mtm_data ;
  }
  if(count!=0) {
    *mtm1 = mtm_create(wsize, BB, count) ;
    if(*mtm1==NULL) {
      return trios_error(MSG, "mtm_sep: mtm_create() failed") ;
    }
    q1 = (mtm_BX *)(*mtm1)->mtm_data ;
  }

#ifdef _DEBUG_
  trios_debug("Exemplos ZER0=%d", nmtm-count) ;
  trios_debug("Exemplos UM=%d", count) ;
#endif

  /* separate the examples into the subsets */
  aux = var/32 ;
  mask_i = var%32 ;
  j0 = j1 = 0 ;

  for(j=0; j<mtm->nmtm; j++) {

    if(!(fq = freq_node_create(q[j].label,1))) {
      return trios_error(MSG, "mtm_sep: freq_node_create() failed.") ;
    }

    if(q[j].wpat[aux]&bitmsk[mask_i]) {
      if(!set_freq(fq, &freqlist1)) {
        return trios_error(MSG, "mtm_sep: set_freq() failed.") ;
      }
      q1[j1].wpat = q[j].wpat ;
      q1[j1].label = q[j].label ;
      q[j].wpat = NULL ;
      j1++ ;
    }
    else {
      if(!set_freq(fq, &freqlist0)) {
        return trios_error(MSG, "mtm_sep: set_freq() failed.") ;
      }
      q0[j0].wpat = q[j].wpat ;
      q0[j0].label = q[j].label ;
      q[j].wpat = NULL ;
      j0++ ;
    }
  }

  (*mtm1)->mtm_freq = freqlist1 ;
  (*mtm0)->mtm_freq = freqlist0 ;

  return(1) ;
}


/*
     -------------------------------------------
     FUNCTION: itv_setvar
     -------------------------------------------
*/

int             /*+ Purpose: Given an interval, set the indicated variable
                    as a constant (thus creating two subintervals)         +*/
  itv_setvar(
    itv_t  *itv,       /*+ In: input interval                              +*/
    int    var,        /*+ In: variable index                              +*/
    itv_t  **itv0,     /*+ Out: first output interval                      +*/
    itv_t  **itv1      /*+ Out: second output interval                     +*/
)
/*+ Return: 1 on success, 0 on failure                                     +*/
{

/*  author: Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)                 */
/*  date: Wed Jul  8 1998                                      */

  itv_BX       *p, *q ;
  unsigned int *A, *B ;
  int          wzip, i ;
  itv_t        *I0, *I1 ;

  /* Let [A,B] be the given interval */
  /* The subintervals that will be created are : */
  /*   [A+X,B] and [A, B-X]                      */
  /* For example, if [A,B]=[000,111] and the variable is 2, */
  /* then the resulting intervals are : */
  /*   [100,111] and [000, 011]                             */

  wzip = size_of_zpat(itv->wsize) ;

  /* allocates space for the patterns of the interval [A,B] */
  if((A = (unsigned int *)malloc(sizeof(int)*wzip)) == NULL) {
    return trios_error(1, "itv_setvar: memory allocation error.") ;
  }
  if((B = (unsigned int *)malloc(sizeof(int)*wzip)) == NULL) {
    return trios_error(1, "itv_setvar: memory allocation error.") ;
  }

  p = (itv_BX *)itv->head ;
  for(i=0; i<wzip; i++) {
    A[i] = p->A[i] ;
    B[i] = p->B[i] ;
  }

  B[var/32] = (B[var/32] & (~bitmsk[var%32])) ;

  /* create structure to hold a set of intervals */
  if(NULL==(I0 = itv_create(itv->wsize, BB, 0))) {
    return trios_error(MSG, "itv_setvar: itv_create() failed.") ;
  }

  /* the itv_t structure will contain, actually, just one interval */
  q = itv_nodebx_create(wzip) ;
  itvbx_set(q, p->A, B, wzip, 1, (itv_BX *)(I0->head)) ;
  I0->head = (int *)q ;
  I0->nitv = 1 ;
  *itv0 = I0 ;


  A[var/32] = A[var/32] | bitmsk[var%32] ;

  /* create structure to hold a set of intervals */
  if(NULL==(I1 = itv_create(itv->wsize, BB, 0))) {
    return trios_error(MSG, "itv_setvar: itv_create() failed.") ;
  }

  /* the itv_t structure will contain, actually, just one interval */
  q = itv_nodebx_create(wzip) ;
  itvbx_set(q, A, p->B, wzip, 1, (itv_BX *)(I1->head)) ;
  I1->head = (int *)q ;
  I1->nitv = 1 ;
  *itv1 = I1 ;

  free(A); free(B) ;

  return(1) ;
}



/*
     -------------------------------------------
     FUNCTION: lpartition
     -------------------------------------------
*/

int lpartition_disk(char *fname_i,int itv_type,int threshold, char *mtm_pref, char *itv_pref, int *n_itv) {
/*  author: Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)                 */
/*  date: SUn Apr 25 1999                                                   */

/* Date: Wed Feb 17 2000                                                   */
/* Mod: Window structure (1) changed to support multiple bands, (2) no     */
/*      longer holds information about aperture.                           */
/*      New structure "apert_t" was created.                               */
/*      Modifications were made to adequate to the structure changing.     */

  window_t *win ;
  apert_t  *apt ;
  mtm_t    *mtm ;
  mtm_t    *mtm_list[256] ;
  itv_t    *start_itv ;
  itv_t    *itv_list[256] ;
  char     fname[512] ;
  FILE     *fd ;
  int      wsize, noper ;
  int      i, j ;

  /* read classified examples */
  if(!(mtm=mtm_read(fname_i, &win, &apt))) {
    return trios_error(MSG, "lpartition: mtm_read() failed to read %s", fname_i)
;
  }
#ifdef _DEBUG_
trios_debug("Reading classified examples file: done.") ;
#endif

  wsize = win_get_wsize(win) ;

  /* create the initial interval */
  start_itv = itv_gen_itv(win, itv_type, BB, 0, 1, 0) ;
  if(start_itv==NULL) {
    return trios_error(MSG, "lpartition: itv_gen_itv() failed.") ;
  }

  /* Check if all examples are in the starting interval */
  /*

      MISSING !!!!!!!!

  */


  noper = 0 ;
  for(i=0; i<256 ; i++) {
    mtm_list[i] = NULL ;
    itv_list[i] = NULL ;
  }

  /* Divide examples and compute the respective partitions (intervals) */
  /* The memory space used both by mtm and by start_itv are released   */
  if(!mtm_part(mtm, start_itv, threshold, mtm_list, itv_list, &noper)) {
    win_free(win) ;
    mtm_free(mtm) ;
    return trios_error(MSG, "lpartition: sep_mtm() failed.") ;
  }
  mtm = NULL ;
  start_itv = NULL ;


#ifdef _DEBUG_
trios_debug("Separating examples: done.") ;

for(i=0; i<noper; i++) {
  p = (itv_BX *)itv_list[i]->head ;
  trios_debug("itv_list[%d]=[%x,%x]\n", i, p->A[0], p->B[0]) ;
}
#endif


  *n_itv = noper;

  for(i=0; i<noper; i++) {

    /* write the (i+1)-th intervals set */
    sprintf(fname, "%s%d%s", itv_pref, i+1, ".itv") ;

#ifdef _DEBUG_
    trios_debug("File name is %s", fname) ;
#endif

    if(!itv_write(fname, itv_list[i], win/*, apt*/)) {
      win_free(win) ;
      for(j=i; j<noper; j++) itv_free(itv_list[j]) ;
      return trios_error(MSG,
           "lpartition: itv_write() failed to write %s", fname) ;
    }
    itv_free(itv_list[i]) ;


    /* write the i+1-th mtm subset */
    sprintf(fname, "%s%d%s", mtm_pref, i+1, ".mtm") ;
#ifdef _DEBUG_
    trios_debug("File name is %s", fname) ;
#endif

    if(!mtm_write(fname, mtm_list[i], win, apt)) {
      win_free(win) ;
      for(j=i; j<noper; j++) mtm_free(mtm_list[j]) ;
      return trios_error(MSG,
           "lpartition: mtm_write() failed to write %s", fname) ;
    }
    mtm_free(mtm_list[i]) ;
  }

  /* free memory */
  win_free(win) ;

  return(1) ;
}

int lpartition_memory(window_t *win, mtm_t *mtm, int itv_type, int threshold, mtm_t ***_mtm_out, itv_t ***_itv_out, int *n_itv) {
    int r;
    int i;
    char temp[100];
    window_t *tt;
    mtm_t **mtm_out;
    itv_t **itv_out;
    mtm_write("partition_temp___.mtm", mtm, win, NULL);
    r = lpartition_disk("partition_temp___.mtm", itv_type, threshold, "partition_mtm_temp__", "partition_itv_temp__", n_itv);
    if (r == 0) return 0;

    trios_malloc(mtm_out, sizeof(mtm_t *) * (*n_itv), int, "Failed to alloc mtm_t list");
    trios_malloc(itv_out, sizeof(itv_t *) * (*n_itv), int, "Failed to alloc itv_t list");

    for(i = 0; i < *n_itv; i++) {
        sprintf(temp, "partition_mtm_temp__%d.mtm", i+1);
        mtm_out[i] = mtm_read(temp, &tt, NULL);
        win_free(tt);
        sprintf(temp, "partition_itv_temp__%d.itv", i+1);
        itv_out[i] = itv_read(temp, &tt);
        win_free(tt);
        sprintf(temp, "rm partition_mtm_temp__%d.mtm partition_itv_temp__%d.itv", i+1, i+1);
        r = system(temp);
    }

    *_mtm_out = mtm_out;
    *_itv_out = itv_out;
    return 1;
}

itv_t *lisi_partitioned(window_t *win, mtm_t *mtm, int threshold) {
    itv_t *acc = NULL;

    return NULL;
}
