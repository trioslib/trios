#ifndef _pac_mtm_h_
#define _pac_mtm_h_

#define MODA               5 
#define AVERAGE           10 
#define MEDIAN            15
#define MABS              20 
#define MINIMUM           25
#define MAXIMUM           30
#define AMPLITUDE         35
#define MODA_DISCRETE     40
#define AVERAGE_DISCRETE  45

/*  Modification by:  Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)       */
/*  Date: Tue Jan  6 1998                                                   */
/*  Mod: Added two new fields to the structure mtm_bx (Px and px)           */
/*  Mod: Added a new field to the structure mtm_t (comp_prob)               */

/*  Modification by:  Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)       */
/*  Date: Wed Apr 29 1998                                                   */
/*  Mod: Added definitions for MAE and MSE                                  */ 

/*  Modification by:  Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)       */
/*  Date: Tue Jul 27 1999                                                   */
/*  Mod: Changed the definitions for MAE and MSE to MODA_DISCRETE           */
/*  Mod: and AVERAGE_DISCRETE                                               */

/*  Modification by:  Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)       */
/*  Date: Thu Jul 29 1999                                                   */
/*  Mod: The labels of all patterns have been changed to int                */

/*  Modification by:  Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)       */
/*  Date: Thu Aug 19 1999                                                   */
/*  Mod: mtm_BX and mtm_GX structure changed. For mtm_BX, field PX and      */
/*       px were changed to fq and fq1, respectively. For mtm_GX, a new     */
/*       field fq was introduced. fq keeps the number of times a w-pattern  */
/*       has been observed during sampling. fq1 keeps how many times        */
/*       the label 1 was observed assigned to the w-pattern                 */


typedef struct mtm_bx {
  unsigned int   *wpat ;    /* pointer to w-pattern                       */
  int            label ;    /* label attributed to the pattern            */
  unsigned int   fq ;       /* frequency of w-pattern                     */
  unsigned int   fq1 ;      /* frequency of w-pattern assigned to label 1 */
} mtm_BX ;

typedef struct mtm_gx {
  char  *wpat ;             /* pointer to w-pattern             */
  int   label ;             /* label attributed to the pattern  */
  unsigned int fq ;         /* frequency of the w-pattern       */     
} mtm_GX ;

typedef struct {
  int    wsize ;       /* size of w-pattern                      */
  char   type ;        /* type of examples: BB, BG, GB or GG     */
  unsigned int nmtm ;  /* number of miniterms                    */
  unsigned int nsum ;  /* total number of examples (sum of fq's) */
  int    comp_prob ;   /* if 1, indicates that probabilities were computed */
  freq_node *mtm_freq; /* pointer to list of frequencies */
  int   *mtm_data ;    /* pointer to the table                   */
} mtm_t ;



/*===========================================================================\
   pac_mtm.c
\========================================================================== */

/*
     -------------------------------------------
     FUNCTION: MTM CREATE
     -------------------------------------------
*/

mtm_t *          /*+ Purpose: allocate a MTM structure                     +*/
  mtm_create(
    int wsize,   /*+ In: w-pattern size                                    +*/
    int type,    /*+ In: transformation type (BB, BG, GB or GG)            +*/
    unsigned int nmtm  /*+ In: number of minterms                          +*/
)
/*+ Returns: a pointer to the MTM structure                                +*/ ;



/*
     -------------------------------------------
     FUNCTION: MTM FREE
     -------------------------------------------
*/
void                      /*+ Purpose: free memory area used MTM structure  +*/
  mtm_free(
    mtm_t *mtm            /*+ In: pointer to the MTM structure              +*/
)
/*+ Return: nothing                                                         +*/ ;



/*
     -------------------------------------------
     FUNCTION: MTM GET SIZE
     -------------------------------------------
*/
int                 /*+ Purpose: get the size of w-pattern                  +*/
  mtm_get_wsize(
    mtm_t *mtm      /*+ In: pointer to the MTM                              +*/
)
/*+ Return: the size of w-pattern (integer between 1 and 256)               +*/ ;



/*
     -------------------------------------------
     FUNCTION: MTM GET TYPE
     -------------------------------------------
*/
int                 /*+ Purpose: get the type of the labeled examples      +*/
  mtm_get_type(
    mtm_t *mtm      /*+ In: pointer to the MTM                             +*/
)
/*+ Return: the type of w-pattern                                          +*/ ;



/*
     -------------------------------------------
     FUNCTION: MTM GET NMTM
     -------------------------------------------
*/
unsigned int                 /*+ Purpose: get the number of minterms                +*/
  mtm_get_nmtm(
    mtm_t *mtm      /*+ In: pointer to the MTM                             +*/
)
/*+ Return: the number of minterms                                         +*/ ;



/*
     -------------------------------------------
     FUNCTION: MTM GET NSUM
     -------------------------------------------
*/
unsigned int                 /*+ Purpose: get the number of seen examples           +*/
  mtm_get_nsum(
    mtm_t *mtm      /*+ In: pointer to the MTM                             +*/
)
/*+ Return: the number of seen examples                                    +*/ ;



/*
     -------------------------------------------
     FUNCTION: MTM GET COMP_PROB
     -------------------------------------------
*/
int                 /*+ Purpose: get the comp_prob flag value              +*/
  mtm_get_comp_prob(
    mtm_t *mtm      /*+ In: pointer to the MTM                             +*/
)
/*+ Return: comp_prob value (0 or 1)                                       +*/ ;


freq_node *     /*+ Purpose: Get list of frequencies                        +*/
  mtm_get_freqlist(                                                                   
    mtm_t *mtm_i       /*+ In: classified examples set                      +*/
) ;


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
/*+ Return: nothing                                                        +*/ ;




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
/*+ Return: nothing                                                        +*/ ;




/*
     -------------------------------------------
     FUNCTION: MTM SET NMTM
     -------------------------------------------
*/
void                /*+ Purpose: set the number of minterms                +*/
  mtm_set_nmtm(
    mtm_t *mtm,     /*+ pointer to the MTM                                 +*/
    unsigned int nmtm  /*+ In: number of minterms                             +*/  
)
/*+ Return: nothing                                                        +*/ ;



/*
     -------------------------------------------
     FUNCTION: MTM SET Nsum
     -------------------------------------------
*/
void                /*+ Purpose: set the number of seen examples           +*/
  mtm_set_nsum(
    mtm_t *mtm,     /*+ pointer to the MTM                                 +*/
    unsigned int nsum  /*+ In: number of minterms                             +*/  
)
/*+ Return: nothing                                                        +*/ ;



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
/*+ Return: comp_prob value (0 or 1)                                       +*/ ;



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
    unsigned int *wpat,
    int label,            /*+ In: label of the inserted minterm            +*/
    unsigned int fq,      /*+ In: frequency of wpat                        +*/
    unsigned int fq1      /*+ In: 1-frequency of wpat                      +*/
) ;
/*+ Return: 1 on success 0 on failure                                      +*/


/*
     -------------------------------------------
     FUNCTION: MTM_GX_insert 
     -------------------------------------------
*/
int                       /*+ Purpose: inserts a minterm in the table      +*/
  mtm_GX_insert(
    mtm_t *mtm,           /*+ In: pointer to mtm structure                 +*/ 
    int index1,            /*+ In: insert position                          +*/
    int wsize,            /*+ In: wpat size in bytes                       +*/
    char *wpat,           /*+ In: w-pattern                                +*/
    int label,            /*+ In: label of the inserted minterm            +*/
    unsigned int fq       /*+ In: frequency of wpat                        +*/  
) ;
/*+ Return: 1 on success 0 on failure                                      +*/


/*
     -------------------------------------------
     FUNCTION: mtm_GX_get_pattern
     -------------------------------------------
*/
 
char *          /*+ Purpose: Get a pattern from the table                  +*/
  mtm_GX_get_pattern(                   
    mtm_t *mtm,           /*+ In: pointer to mtm structure                 +*/ 
    int index1             /*+ In: pattern position                         +*/
)
/*+ Return: A char pointer to a pattern                                    +*/ ;




/*
     -------------------------------------------
     FUNCTION: mtm_GX_get_label
     -------------------------------------------
*/
 
int            /*+ Purpose: Get the label of a  pattern from the table    +*/
  mtm_GX_get_label(                   
    mtm_t *mtm,           /*+ In: pointer to mtm structure                 +*/ 
    int index1             /*+ In: pattern position                         +*/
)
/*+ Return: the label of the pattern of the given position                 +*/ ;


/*
     -------------------------------------------
     FUNCTION: mtm_sep
     -------------------------------------------
*/
 
int             /*+ Purpose: Separate examples of a given class k          +*/
  mtm_sep(
    mtm_t *mtm_i,      /*+ In/Out: classified examples set                 +*/
    int   k,           /*+ In: class to be separeted                       +*/
    mtm_t **mtm_o1,    /*+ In/Out: examples of class k                     +*/
    mtm_t **mtm_o2     /*+ In/Out: examples of classes other than k        +*/
  )
/*+ Return: 1 on success, 0 on failure                                     +*/ ;


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
/*+ Return: the number of examples in class k                              +*/ ;

 
int             /*+ Purpose: given a mtm table, computes the mae           +*/
  mtm_compute_mae(                                                                
    mtm_t *mtm,        /*+ In: mtm table                                   +*/
    double *mae_error  /*+ Out: mae                                        +*/
) ;



int             /*+ Purpose: Search examples table for a given wpat        +*/
  mtm_search_wpat_BX(
    unsigned int  nmtm,     /*+ In: size of the table                      +*/ 
    mtm_t         *mtm,     /*+ In: classified examples table              +*/
    unsigned int *wpat,     /*+ In: wpattern                               +*/
    int           wzip      /*+ In: size of wpattern                       +*/
    ) ;



int             /*+ Purpose: Search examples table for a given wpat         +*/
  mtm_search_wpat_GX(
    unsigned int nmtm,  /*+ In: size of the table                           +*/ 
    mtm_t       *mtm,   /*+ In: classified examples table                   +*/
    char        *wpat,  /*+ In: wpattern                                    +*/
    int          wsize  /*+ In: size of wpattern                            +*/
    ) ;


unsigned int    /*+ Purpose: Index mtm by frequencies                      +*/
  mtm_index_by_freq_count(                                                                  
    mtm_t *mtm_i,      /*+ In: classified examples set                     +*/
    int   *indexes     /*+ In: vector of indexes                           +*/
    ) ;
/*+ Return: 1 on success or 0 on failure                                   +*/


unsigned int    /*+ Purpose: Index mtm by labels                               +*/
  mtm_index_by_wklabel(                                                                  
    int    ko,             /*+ In: output aperture                             +*/
    mtm_t *mtm_i,          /*+ In: classified examples set                     +*/
    unsigned int *labindx, /*+ Out: indexes to labels                          +*/
    unsigned int *indexes, /*+ Out: vector of indexes                          +*/
    unsigned int *lastlbl  /*+ Out: pointer to the last label                  +*/
    ) ;
/*+ Return: Number of labels on success or 0 on failure                        +*/

#endif


