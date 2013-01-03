#ifndef _pac_xpl_h_
#define _pac_xpl_h_


#ifdef __cplusplus
extern "C" {
#endif

#define  EQUAL   0
#define  LESS    -1
#define  GREAT   1

#include "trios_misc.h"

/*!
    Structure that holds a pair (binary W-pattern, binary result) in the extracted example set (represented by a tree).
 */
typedef struct xpl_bb {
  unsigned int *wpat ;        /*!< pointer to w-pattern          */
  unsigned int  fq0 ;         /*!< frequency of zeros            */
  unsigned int  fq1 ;         /*!< frequency of non-zeros        */
  int    bal ;                /*!< code to control balanced tree */
  struct xpl_bb *left ;       /*!< pointer to left child         */
  struct xpl_bb *right ;      /*!< pointer to right child        */
} xpl_BB ;

/*!
    Structure that holds a pair (binary W-pattern, gray levels) in the extracted example set (represented by a tree).
 */
typedef struct xpl_bg {
  unsigned int *wpat ;             /*!< pointer to w-pattern           */
  struct freq_node *freqlist;      /*!< pointer to list of frequencies */
  int    bal ;                     /*!< code to control balanced tree  */
  struct xpl_bg *left ;            /*!< pointer to left child          */
  struct xpl_bg *right ;           /*!< pointer to right child         */
} xpl_BG ;

/*!
    Structure that holds a pair (gray W-pattern, gray levels) in the extracted example set (represented by a tree).
 */
typedef struct xpl_gg {
  int   *wpat ;                   /*!< pointer to w-pattern           */
  struct freq_node *freqlist;      /*!< pointer to list of frequencies */
  int    bal ;                     /*!< code to control balanced tree  */
  struct xpl_gg *left ;            /*!< pointer to left child          */
  struct xpl_gg *right ;           /*!< pointer to right child         */
} xpl_GG ;


/*!
    Set with the patterns extracted from an imgset_t.
 */
typedef struct {
  int   wsize ;          /*!< size of w-pattern                                */
  int   wzip ;           /*!< size of compacted W-pattern                      */
  int   type ;           /*!< type of examples: BB, BG, GB or GG               */
  unsigned int n_nodes ; /*!< number of nodes (distinct examples)              */
  unsigned int sum ;     /*!< sum of frequency                                 */
  int   *root ;          /*!< pointer to the tree (that contains the examples) */
} xpl_t ;


/*
     -------------------------------------------
     FUNCTION: xpl_create
     -------------------------------------------
*/

xpl_t *            /*+ Purpose: allocate a XPL structure                   +*/
  xpl_create(
    int wsize,              /*+ In: w-pattern size                         +*/
    int type                /*+ In: transformation type (BB, BG, GB or GG) +*/
  ) ;
/*+ Return: a pointer to the XPL structure                                 +*/


/*
     -------------------------------------------
     FUNCTION: xpl_free
     -------------------------------------------
*/

void        /*+ Purpose: free memory area used by a XPL structure          +*/
  xpl_free(
    xpl_t *xpl         /*+ In/Out: pointer to the XPL structure            +*/
  ) ;
/*+ Return: nothing                                                        +*/


/*
     -------------------------------------------
     FUNCTION: xpl_get_wsize
     -------------------------------------------
*/

int               /*+ Purpose: get the w-pattern size of examples          +*/      
  xpl_get_wsize(
    xpl_t *xpl              /*+ In: pointer to the XPL                     +*/
  ) ;
/*+ Return: the w-pattern size (integer between 1 and 256)                 +*/


/*
     -------------------------------------------
     FUNCTION: xpl_get_wzip
     -------------------------------------------
*/

int             /*+ Purpose: get the compacted size of w-patterns          +*/
  xpl_get_wzip(
    xpl_t *xpl                /*+ In: pointer to the XPL                   +*/
  ) ;
/*+ Return: the w-pattern compacted size                                   +*/


/*
     -------------------------------------------
     FUNCTION: xpl_get_n_nodes
     -------------------------------------------
*/

unsigned int   /*+ Purpose: get the number of distinct examples in the XPL tree +*/
  xpl_get_n_nodes(
    xpl_t *xpl             /*+ In: pointer to the XPL                      +*/
  ) ;
/*+ Return: number of nodes in the XPL tree                                +*/


/*
     -------------------------------------------
     FUNCTION: xpl_get_sum
     -------------------------------------------
*/

unsigned int  /*+ Purpose: get the number of examples in the XPL         +*/
  xpl_get_sum(
    xpl_t *xpl                       /*+ In: pointer to the XPL            +*/
  ) ;
/*+ Return: the number of examples in the XPL                              +*/


/*
     -------------------------------------------
     FUNCTION: xpl_get_root
     -------------------------------------------
*/

int *       /*+ Purpose: get the pointer to the tree that corresponds to the     
                    examples set                                           +*/
  xpl_get_root(
    xpl_t *xpl                     /*+ In: pointer to the XPL              +*/
  ) ;
/*+ Return: the pointer to the XPL tree                                    +*/


/*
     -------------------------------------------
     FUNCTION: xpl_get_type
     -------------------------------------------
*/

int          /*+ Purpose: get the transformation type (BB, BG, GB or GG)   +*/
  xpl_get_type(
    xpl_t *xpl                     /*+ In: pointer to the XPL              +*/
  ) ;
/*+ Return: the transformation type (BB, BG, GB or GG)                     +*/


/*
     -------------------------------------------
     FUNCTION: xpl_set_root
     -------------------------------------------
*/

void            /*+ Purpose: set a pointer to a XPL tree                   +*/
  xpl_set_root(
    xpl_t *xpl,             /*+ In/Out: pointer to the XPL                 +*/
    int *root               /*+ In: pointer to the XPL tree                +*/
  ) ;
/*+ Return: nothing                                                        +*/


/*
     -------------------------------------------
     FUNCTION: xpl_set_wsize
     -------------------------------------------
*/

void             /*+ Purpose: set the w-pattern size associated to the XPL +*/
  xpl_set_wsize(
    xpl_t *xpl,           /*+ In/Out: pointer to the XPL                   +*/
    int   wsize           /*+ In: w-pattern size to be set                 +*/
  ) ;
/*+ Return: nothing                                                        +*/


/*
     -------------------------------------------
     FUNCTION: xpl_set_wzip
     -------------------------------------------
*/

void                  /*+ Purpose: set the compacted size of w-patterns    +*/
  xpl_set_wzip(
    xpl_t *xpl,               /*+ In/Out: pointer to the XPL               +*/
    int   wzip                /*+ In: compacted size of w-patterns         +*/
  ) ;
/*+ Return: nothing                                                        +*/


/*
     -------------------------------------------
     FUNCTION: xpl_set_n_nodes
     -------------------------------------------
*/

void            /*+ Purpose: set the number of nodes in the tree. Usually
                             used to initialize a XPL structure            +*/
  xpl_set_n_nodes(
    xpl_t *xpl,             /*+ In/Out: pointer to the XPL                 +*/
    unsigned int n_nodes    /*+ In: number of nodes in the XPL tree        +*/
  ) ;
/*+ Return: nothing                                                        +*/


/*
     -------------------------------------------
     FUNCTION: xpl_set_sum
     -------------------------------------------
*/

void           /*+ Purpose: set the number of examples in the XPL          +*/
  xpl_set_sum(
    xpl_t *xpl,           /*+ In/Out: pointer to the XPL                   +*/
    unsigned int sum      /*+ In: amount of examples                       +*/
  ) ;
/*+ Return: nothing                                                        +*/


/*
     -------------------------------------------
     FUNCTION: xpl_set_type
     -------------------------------------------
*/

void          /*+ Purpose: set the transformation type                     +*/
  xpl_set_type(
    xpl_t *xpl,                /*+ In/Out: pointer to the XPL              +*/
    int  type /*+ In: the transformation type to be set (BB, BG, GB or GG) +*/
  ) ;
/*+ Return: nothing                                                        +*/


/*
     -------------------------------------------
     FUNCTION: xpl_merge
     -------------------------------------------
*/

int       /*+ Purpose: merge two XPLs and assign the result to the first one +*/
  xpl_merge(
    xpl_t *xpl1,       /*+ In/Out: pointer to the first XPL                +*/
    xpl_t *xpl2        /*+ In/Out: pointer to the second XPL               +*/
  ) ;
/*+ Return: 1 on success, 0 on failure                                     +*/


/*
     -------------------------------------------
     FUNCTION: xpl_tree2ll
     -------------------------------------------
*/

int              /*+ Purpose: change the tree structure of a XPL set to 
                   the linked list structure                               +*/
  xpl_tree2ll(
    xpl_t *xpl,                   /*+ In/Out: XPL structure                +*/
    int   order_type              /*+ In: Ascendent(1) or descendent(2)    +*/
  ) ;
/*+ Return: 1 on success, 0 on failure                                     +*/


/*
     -------------------------------------------
     FUNCTION: xpl_conflicts
     -------------------------------------------
*/

int            /*+ Purpose: count the number of conflicting examples
                            in a XPL set                                   +*/
  xpl_conflicts(
    xpl_t *xpl                       /*+ In: XPL set                       +*/
  ) ;
/*+ Return: -1 on failure, >=0 on success                                  +*/


/*
     -------------------------------------------
     FUNCTION: xpl_sort
     -------------------------------------------
*/

int         /*+ Purpose: sort examples in a XPL set by frequency           +*/
  xpl_sort(
    xpl_t *xpl                         /*+ In/Out: XPL set                 +*/
  ) ;
/*+ Return: 1 on success, 0 on failure                                     +*/


/*===========================================================================\
   pac_xplBB.c
\========================================================================== */

/*
     -------------------------------------------
     FUNCTION: xpl_set_rootBB
     -------------------------------------------
*/

int            /*+ Purpose: set a pointer to a XPL tree                    +*/
  xpl_set_rootBB(
    xpl_t *xpl,             /*+ In/Out: pointer to the XPL                 +*/
    xpl_BB *root            /*+ In: pointer to the XPL tree                +*/
  ) ;
/*+ Return: 1 on success, 0 on failure                                     +*/


/*
     -------------------------------------------
     FUNCTION: BB_leftrotate
     -------------------------------------------
*/

void              /*+ Purpose: rotate a subtree to the left                +*/
  BB_leftrotate(
    xpl_BB **pp              /*+ In/Out: pointer to the subtree's root     +*/
  ) ;
/*+ Return: nothing                                                        +*/

/*
     -------------------------------------------
     FUNCTION: BB_rightrotate
     -------------------------------------------
*/

void             /*+ Purpose: rotate a subtree to the right                +*/
  BB_rightrotate(
    xpl_BB **pp                  /*+ In/Out: pointer to the subtree's root +*/
  ) ;
/*+ Return: nothing                                                        +*/


/*
     -------------------------------------------
     FUNCTION: xpl_BB_create
     -------------------------------------------
*/

xpl_BB *                /*+ Purpose: allocate a BB node                    +*/      
  xpl_BB_create(
    int wzip                     /*+ In: compacted window size             +*/
  ) ;
/*+ Return: a pointer to a node of BB type                                 +*/

/*
     -------------------------------------------
     FUNCTION: xpl_BB_set_node
     -------------------------------------------
*/

void                     /*+ Purpose: set the fields of a BB node          +*/
  xpl_BB_set_node(
    xpl_BB *p,               /*+ In/Out: pointer to the BB node            +*/
    unsigned int *wpat,
    int    wzip,             /*+ In: w-pattern size                        +*/
    unsigned int fq0,        /*+ In: negative frequency of the w-pattern   +*/
    unsigned int fq1         /*+ In: positive frequency of the w-pattern   +*/
  ) ;
/*+ Return: nothing                                                        +*/


/*
     -------------------------------------------
     FUNCTION: xpl_BB_insert
     -------------------------------------------
*/

int              /*+ Purpose: insert a node in a BB tree (recursively)     +*/
  xpl_BB_insert(
    xpl_t   *xpl,            /*+ In/Out: pointer to the XPL structure      +*/
    xpl_BB  **pp,            /*+ In/Out: pointer to a subtree's root       +*/
    unsigned int *wpat,
    unsigned int fq0,             /*+ In: w-pattern's negative frequency        +*/
    unsigned int fq1              /*+ In: w-pattern's positive frequency        +*/
  ) ;
/*+ Return: -1 on failure, >= 0  on success                                +*/


/*
     -------------------------------------------
     FUNCTION: xpl_BB_free
     -------------------------------------------
*/

void         /*+ Purpose: free memory area used by a BB node               +*/
  xpl_BB_free(
    xpl_BB *p          /*+ In/Out: pointer to the BB node                  +*/
  ) ;
/*+ Return: nothing                                                        +*/


/*
     -------------------------------------------
     FUNCTION: xpl_BB_free_tree
     -------------------------------------------
*/

void                  /*+ Purpose: free memory area used by a BB tree      +*/
  xpl_BB_free_tree(
    xpl_BB *p                    /*+ In/Out: pointer to the BB tree        +*/
  ) ;
/*+ Return: nothing                                                        +*/


/*
     -------------------------------------------
     FUNCTION: xpl_BB_merge
     -------------------------------------------
*/

int             /*+ Purpose: merge two BB tree, recursively                +*/
  xpl_BB_merge(
    xpl_t *xpl,/*+ In/Out: pointer to the XPL, containing the fisrt BB tree +*/
    xpl_BB  *p                       /*+ In: pointer to the second BB tree +*/
  ) ;
/*+ Return: 1 on success, 0 on failure                                     +*/


/*
     -------------------------------------------
     FUNCTION: xpl_BB_add
     -------------------------------------------
*/

int               /*+ Purpose: add a new node into a BB tree, recursively  +*/
  xpl_BB_add(
    xpl_t  *xpl,    /*+ In/Out: pointer to the XPL, containing the BB tree +*/
    xpl_BB **p_cur,          /*+ In/Out: current node in the BB tree       +*/
    xpl_BB *p_new            /*+ In: pointer to the node to be added       +*/
  ) ;
/*+ Return: -1 on failure, >= 0  on success                                +*/


/*
     -------------------------------------------
     FUNCTION: tree2ll_BB_asc
     -------------------------------------------
*/

int           /*+ Purpose: change BB tree structure to the linked list
                           structure (ascendent ordering)                  +*/
  tree2ll_BB_asc(
    xpl_BB *p,           /*+ In: BB tree structure                         +*/
    xpl_BB **pp          /*+ Out: linked list structure                    +*/
  ) ;
/*+ Return: 1 on success, 0 on failure                                     +*/


/*
     -------------------------------------------
     FUNCTION: tree2ll_BB_desc
     -------------------------------------------
*/

int           /*+ Purpose: change BB tree structure to the linked list
                           structure (descendent ordering)                 +*/
  tree2ll_BB_desc(
    xpl_BB *p,              /*+ In: BB tree structure                      +*/
    xpl_BB **pp             /*+ Out: linked list structure                 +*/
  ) ;
/*+ Return: 1 on success, 0 on failure                                     +*/



/*
     -------------------------------------------
     FUNCTION: xpl_sort_BB
     -------------------------------------------
*/

int                /*+ Purpose: sort BB XPL set                            +*/
  xpl_sort_BB(
    xpl_BB *p,            /*+ In: pointer to XPL tree to be sorted         +*/
    xpl_BB **pp           /*+ Out: sorted XPL tree                         +*/
  ) ;
/*+ Return: 1 on success, 0 on failure                                     +*/


/*
     -------------------------------------------
     FUNCTION: xpl_sort_insert
     -------------------------------------------
*/

int                /*+ Purpose: insert a node in a BB XPL tree, balancing     
                         it according to the frequency of examples         +*/
  xpl_sort_insert(
    xpl_BB  *p,                 /*+ In: node to be inserted                +*/
    xpl_BB **pp                 /*+ In/Out: sorted XPL tree                +*/
  ) ;
/*+ Return: >= 0 on success, -1 on failure                                 +*/

/*
     -------------------------------------------
     FUNCTION: xpl_set_rootGG
     -------------------------------------------
*/

int            /*+ Purpose: set a pointer to a XPL tree                    +*/
  xpl_set_rootGG(
    xpl_t *xpl,             /*+ In/Out: pointer to the XPL                 +*/
    xpl_GG *root            /*+ In: pointer to the XPL tree                +*/
  ) ;
/*+ Return: 1 on success, 0 on failure                                     +*/


/*
     -------------------------------------------
     FUNCTION: GG_leftrotate
     -------------------------------------------
*/

void              /*+ Purpose: rotate a subtree to the left                +*/
  GG_leftrotate(
    xpl_GG **pp              /*+ In/Out: pointer to the subtree's root     +*/
  ) ;
/*+ Return: nothing                                                        +*/


/*
     -------------------------------------------
     FUNCTION: GG_rightrotate
     -------------------------------------------
*/

void             /*+ Purpose: rotate a subtree to the right                +*/
  GG_rightrotate(
    xpl_GG **pp                  /*+ In/Out: pointer to the subtree's root +*/
  ) ;
/*+ Return: nothing                                                        +*/


/*
     -------------------------------------------
     FUNCTION: xpl_GG_create
     -------------------------------------------
*/

xpl_GG *                /*+ Purpose: allocate a GG node                    +*/
  xpl_GG_create(
    int wsize                     /*+ In:  window size                     +*/
  ) ;
/*+ Return: a pointer to a node of GG type                                 +*/


/*
     -------------------------------------------
     FUNCTION: xpl_GG_set_node
     -------------------------------------------
*/

void                     /*+ Purpose: set the fields of a GG node          +*/
  xpl_GG_set_node(
    xpl_GG *p,               /*+ In/Out: pointer to the GG node            +*/
    int   *wpat,            /*+ In: w-pattern                             +*/
    int    wsize,            /*+ In: w-pattern size                        +*/
    freq_node *freqlist      /*+ In: pointer to list of frequencies        +*/
  ) ;
/*+ Return: nothing                                                        +*/


/*
     -------------------------------------------
     FUNCTION: xpl_GG_insert
     -------------------------------------------
*/

int              /*+ Purpose: insert a node in a GG tree (recursively)        +*/
  xpl_GG_insert(
    xpl_t   *xpl,            /*+ In/Out: pointer to the XPL structure         +*/
    xpl_GG  **pp,            /*+ In/Out: pointer to a subtree's root          +*/
    int   *wpat,            /*+ In: w-pattern to be inserted                 +*/
    freq_node *freqlist      /*+ In: w-pattern list of frequencies and labels +*/
  ) ;
/*+ Return: -1 on failure, >= 0  on success                                   +*/


/*
     -------------------------------------------
     FUNCTION: xpl_GG_free
     -------------------------------------------
*/

void         /*+ Purpose: free memory area used by a GG node               +*/
  xpl_GG_free(
    xpl_GG *p          /*+ In/Out: pointer to the GG node                  +*/
  ) ;
/*+ Return: nothing                                                        +*/


/*
     -------------------------------------------
     FUNCTION: xpl_GG_free_tree
     -------------------------------------------
*/

void                  /*+ Purpose: free memory area used by a GG tree      +*/
  xpl_GG_free_tree(
    xpl_GG *p                    /*+ In/Out: pointer to the GG tree        +*/
  ) ;
/*+ Return: nothing                                                        +*/


/*
     -------------------------------------------
     FUNCTION: xpl_GG_merge
     -------------------------------------------
*/

int             /*+ Purpose: merge two GG tree, recursively                +*/
  xpl_GG_merge(
    xpl_t *xpl,  /*+ In/Out: pointer to the XPL, containing the fisrt GG tree +*/
    xpl_GG  *p   /*+ In: pointer to the second GG tree                        +*/
  ) ;
/*+ Return: 1 on success, 0 on failure                                     +*/

/*
     -------------------------------------------
     FUNCTION: xpl_GG_add
     -------------------------------------------
*/

int               /*+ Purpose: add a new node into a GG tree, recursively  +*/
  xpl_GG_add(
    xpl_t  *xpl,    /*+ In/Out: pointer to the XPL, containing the GG tree +*/
    xpl_GG **p_cur,          /*+ In/Out: current node in the GG tree       +*/
    xpl_GG *p_new            /*+ In: pointer to the node to be added       +*/
  ) ;
/*+ Return: -1 on failure, >= 0  on success                                +*/

/*
     -------------------------------------------
     FUNCTION: tree2ll_GG_asc
     -------------------------------------------
*/

int           /*+ Purpose: change GG tree structure to the linked list
                           structure (ascendent ordering)                  +*/
  tree2ll_GG_asc(
    xpl_GG *p,           /*+ In: GG tree structure                         +*/
    xpl_GG **pp          /*+ Out: linked list structure                    +*/
  ) ;
/*+ Return: 1 on success, 0 on failure                                     +*/

/*
     -------------------------------------------
     FUNCTION: tree2ll_GG_desc
     -------------------------------------------
*/

int           /*+ Purpose: change GG tree structure to the linked list
                           structure (descendent ordering)                 +*/
  tree2ll_GG_desc(
    xpl_GG *p,              /*+ In: GG tree structure                      +*/
    xpl_GG **pp             /*+ Out: linked list structure                 +*/
  ) ;
/*+ Return: 1 on success, 0 on failure                                     +*/


/*
     -------------------------------------------
     FUNCTION: xpl_GG_sort
     -------------------------------------------
*/

int                /*+ Purpose: sort GG XPL set                            +*/
  xpl_GG_sort(
    xpl_GG *p,            /*+ In: pointer to XPL tree to be sorted         +*/
    xpl_GG **pp           /*+ Out: sorted XPL tree                         +*/
  ) ;
/*+ Return: 1 on success, 0 on failure                                     +*/

/*
     -------------------------------------------
     FUNCTION: xpl_GG_sort_insert
     -------------------------------------------
*/

int                /*+ Purpose: insert a node in a GG XPL tree, balancing
                         it according to the frequency of examples         +*/
  xpl_GG_sort_insert(
    xpl_GG  *p,                 /*+ In: node to be inserted                +*/
    xpl_GG **pp                 /*+ In/Out: sorted XPL tree                +*/
  ) ;
/*+ Return: >= 0 on success, -1 on failure                                 +*/



#ifdef __cplusplus
}
#endif


#endif



