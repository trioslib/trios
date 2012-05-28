#include <pacbasic.h>

/* #define _DEBUG_ */


/*!
  Allocate a XPL structure.

  \param wsize W-pattern size.
  \param type Transformation type (BB, BG, GB or GG).
  \return A pointer to the XPL structure.
  */
xpl_t * xpl_create(int wsize, int type) {
  xpl_t *xpl ;

  if((xpl=(xpl_t *)malloc(sizeof(xpl_t))) == NULL) {
    return (xpl_t *)pac_error(1, "xpl_create: Memory allocation failed.") ;
  }

  xpl->wsize = wsize ;
  xpl->wzip = size_of_zpat(wsize) ;
  xpl->type = type ;
  xpl->n_nodes = 0 ;
  xpl->sum  = 0 ;
  xpl->root = NULL ;

  return(xpl) ;

}

/*!
  Free the memory used by a XPL structure.

  \param xpl Pointer to the structure.
  */
void xpl_free(xpl_t *xpl) {
  if(xpl->root) {

    switch(xpl->type) {
    case BB:
      xpl_BB_free_tree((xpl_BB *)xpl->root) ;
      break ;

    case BG:
      pac_error(MSG, "This operator is not supported yet");
      /*xpl_BG_free_tree((xpl_BG *)xpl->root) ;*/
      break ;

    case GG:
    case WKC:
    case WKF:
    case WK3F:
    case WK3C:
    case WKGG2F:
    case WKGG2C:
    case GG3:
      pac_error(MSG, "This operator is not supported yet");
      /*xpl_GG_free((xpl_GG *)xpl->root) ;*/
      break;

    default:
      pac_error(1, "xpl_free: Mapping %d not implemented yet.", xpl->type) ;
    }
  }

  free(xpl) ;

}

/*!
  Get the w-pattern size of examples.

  \param xpl Pointer to the XPL.
  \return The W-pattern size.
  */
int xpl_get_wsize(xpl_t *xpl) {
  return(xpl->wsize) ;
}

/*!
  Get the compacted size of W-patterns.

  \param xpl Pointer to the XPL.
  \return The W-pattern compacted size.
  */
int xpl_get_wzip(xpl_t *xpl) {
  return(xpl->wzip) ;
}

/*!
  Get the w-pattern size of examples.

  \param xpl Pointer to the XPL.
  \return Number of nodes in the XPL tree.
  */
unsigned int xpl_get_n_nodes(xpl_t *xpl) {
  return(xpl->n_nodes) ;
}

/*!
  Get the number of examples in the XPL.

  \param xpl Pointer to the XPL.
  \return Number of examples in the XPL tree.
  */
unsigned int xpl_get_sum(xpl_t *xpl) {
  return(xpl->sum) ;
}

/*!
  Get the pointer to the tree that corresponds to the examples set.

  \param xpl Pointer to the XPL.
  \return The pointer to the XPL tree.
  */
int *xpl_get_root(xpl_t *xpl) {
  return(xpl->root) ;
}

/*!
  Get the transformation type.

  \param xpl Pointer to the XPL.
  \return The transformation type.
  */
int xpl_get_type(xpl_t *xpl) {
  return(xpl->type) ;
}


/*!
  Set the pointer to the XPL tree.

  \param xpl Pointer to the XPL.
  \param root Pointer to the XPL tree.
  */
void xpl_set_root(xpl_t *xpl, int *root) {
  xpl->root = (int *)root ;
}


/*
     -------------------------------------------
     FUNCTION: xpl_set_wsize
     -------------------------------------------
*/

void             /*+ Purpose: set the w-pattern size associated to the XPL +*/
  xpl_set_wsize(
    xpl_t *xpl,           /*+ In/Out: pointer to the XPL                   +*/
    int   wsize           /*+ In: w-pattern size to be set                 +*/
  )
/*+ Return: nothing                                                        +*/
{
/*  author: Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)                 */
/*  date: Fri Nov 29 17:02:32 EDT 1996                                      */

  xpl->wsize = wsize ;
}

/*
     -------------------------------------------
     FUNCTION: xpl_set_wzip
     -------------------------------------------
*/

void                  /*+ Purpose: set the compacted size of w-patterns    +*/
  xpl_set_wzip(
    xpl_t *xpl,               /*+ In/Out: pointer to the XPL               +*/
    int   wzip                /*+ In: compacted size of w-patterns         +*/
  )
/*+ Return: nothing                                                        +*/
{
/*  author: Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)                 */
/*  date: Fri Nov 29 1996                                                   */

  xpl->wzip = wzip ;
}

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
  )
/*+ Return: nothing                                                        +*/
{
/*  author: Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)                 */
/*  date: Fri Nov 29 1996                                                   */

  xpl->n_nodes = n_nodes ;
}

/*
     -------------------------------------------
     FUNCTION: xpl_set_sum
     -------------------------------------------
*/

void           /*+ Purpose: set the number of examples in the XPL          +*/
  xpl_set_sum(
    xpl_t *xpl,           /*+ In/Out: pointer to the XPL                   +*/
    unsigned int sum      /*+ In: amount of examples                       +*/
  )
/*+ Return: nothing                                                        +*/
{
/*  author: Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)                 */
/*  date: Fri Nov 29 1996                                                   */

  xpl->sum = sum ;
}



/*
     -------------------------------------------
     FUNCTION: xpl_set_type
     -------------------------------------------
*/

void          /*+ Purpose: set the transformation type                     +*/
  xpl_set_type(
    xpl_t *xpl,                /*+ In/Out: pointer to the XPL              +*/
    int  type /*+ In: the transformation type to be set (BB, BG, GB or GG) +*/
  )
/*+ Return: nothing                                                        +*/
{
/*  author: Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)                 */
/*  date: Fri Nov 29 1996                                                   */

  xpl->type = type ;
}


/*
     -------------------------------------------
     FUNCTION: xpl_merge
     -------------------------------------------
*/

int       /*+ Purpose: merge two XPLs and assign the result to the first one +*/
  xpl_merge(
    xpl_t *xpl1,       /*+ In/Out: pointer to the first XPL                +*/
    xpl_t *xpl2        /*+ In/Out: pointer to the second XPL               +*/
  )
/*+ Return: 1 on success, 0 on failure                                     +*/
{
/*  author: Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)                 */
/*  date: Fri Nov 29 1996                                                   */

/*  Modification by:  Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)       */
/*  Date: Tue Jan 13 1998                                                   */
/*  Mod: Modified to treat the case WK                                      */

  int type ;


  if((type=xpl_get_type(xpl1)) != xpl_get_type(xpl2)) {
    return pac_error(1, "xpl_merge: different mapping types!") ;
  }


  if(xpl_get_n_nodes(xpl1)==0) {

    xpl_set_n_nodes(xpl1, xpl_get_n_nodes(xpl2)) ;
    xpl_set_sum(xpl1, xpl_get_sum(xpl2)) ;
    xpl_set_root(xpl1, (int *)xpl2->root) ;

  }

  else {

    if(type==BB) {
      if(!xpl_BB_merge(xpl1, (xpl_BB *)xpl2->root)) {
        return pac_error(MSG, "xpl_merge : xpl_BB_merge() failed.") ;
      }   
    }
    else { 
      if(type==BG) {
        pac_error(MSG, "This operator is not supported yet");
        /*     
    	if(!xpl_BG_merge(xpl1, (xpl_BG *)xpl2->root)) {
	      return pac_error(MSG, "xpl_merge : xpl_BG_merge() failed.") ;
    	}   */ 
      }
      else {
        pac_error(MSG, "This operator is not supported yet");
    	/*if(!xpl_GG_merge(xpl1, (xpl_GG *)xpl2->root)) {
	      return pac_error(MSG, "xpl_merge : xpl_GG_merge() failed.") ;
	    } */  
      }
    }
  }

  xpl_set_n_nodes(xpl2, 0) ;
  xpl_set_sum(xpl2, 0) ;
  xpl_set_root(xpl2, NULL) ;

  return(1) ;

}


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
  )
/*+ Return: 1 on success, 0 on failure                                     +*/
{
/*  author: Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)                 */
/*  date: Fri Nov 29 1996                                                   */

/*  Modification by:  Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)       */
/*  Date: Tue Jan 13 1998                                                   */
/*  Mod: Modified to treat the case WK                                      */

/*  Modification by:  Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)       */
/*  Date: Wed Jul 21 1999                                                   */
/*  Mod: Modified to treat the case BG                                      */

  int type ;


  type = xpl_get_type(xpl) ;

  switch(type) {
  case BB :
    {
      xpl_BB *head = NULL, *p ;

      if(order_type == 1) {
        if(!tree2ll_BB_asc((xpl_BB *)xpl->root, &head)) {
          return pac_error(MSG, "tree2ll : tree2ll_BB() failed.") ;
        }
      }
      else {
        if(!tree2ll_BB_desc((xpl_BB *)xpl->root, &head)) {
          return pac_error(MSG, "tree2ll : tree2ll_BB() failed.") ;
        }
        for(p=head; p!=NULL; ) {
          p->right = p->left ;
          p->left = NULL ;
          p = p->right ;
        }
      }
      xpl->root = (int *)head ;
    }
    break ;
  case BG :
    {
      pac_error(MSG, "This operator is not supported yet");
      /*
      xpl_BG *head = NULL, *p ;

      if(order_type == 1) {
        if(!tree2ll_BG_asc((xpl_BG *)xpl->root, &head)) {
          return pac_error(MSG, "tree2ll : tree2ll_BG() failed.") ;
        }
      }
      else {
        if(!tree2ll_BG_desc((xpl_BG *)xpl->root, &head)) {
          return pac_error(MSG, "tree2ll : tree2ll_BG() failed.") ;
        }
        for(p=head; p!=NULL; ) {
          p->right = p->left ;
          p->left = NULL ;
          p = p->right ;
        }
      }
      xpl->root = (int *)head ;*/
    }
    break ;
  case GG :
  case WKF :
  case WKC :
  case WK3F :
  case GG3 :
    {
      pac_error(MSG, "This operator is not supported yet");
      /*xpl_GG *head = NULL, *p ;

      if(order_type == 1) {
        if(!tree2ll_GG_asc((xpl_GG *)xpl->root, &head)) {
          return pac_error(MSG, "tree2ll : tree2ll_GG() failed.") ;
        }
      }
      else {
        if(!tree2ll_GG_desc((xpl_GG *)xpl->root, &head)) {
          return pac_error(MSG, "tree2ll : tree2ll_GG() failed.") ;
        }
        for(p=head; p!=NULL; ) {
          p->right = p->left ;
          p->left = NULL ;
          p = p->right ;
        }
      }
      xpl->root = (int *)head ;*/
    }
    break ;

  case GB :
  default : 
    return pac_error(MSG, "tree2ll: Mappings %d is not implemented yet.", type) ;
  }

  return(1) ;
}



/*
     -------------------------------------------
     FUNCTION: xpl_conflicts
     -------------------------------------------
*/

int            /*+ Purpose: count the number of conflicting examples
                            in a XPL set                                   +*/
  xpl_conflicts(
    xpl_t *xpl                       /*+ In: XPL set                       +*/
  )
/*+ Return: -1 on failure, >=0 on success                                  +*/
{
/*  author: Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)                 */
/*  date: Fri Nov 29 1996                                                   */

/*  Modification by:  Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)       */
/*  Date: Tue Jan 13 1998                                                   */
/*  Mod: Modified to treat the case WK                                      */

/*  Modification by:  Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)       */
/*  Date: Wed Jul 21 1999                                                   */
/*  Mod: Modified to treat the case BG                                      */

  int type ;
  int ncflt = 0 ;
  
  type = xpl_get_type(xpl) ;
  
  switch(type) {
  case BB : {
    xpl_BB *p ;
      
    p = (xpl_BB *)xpl->root ; ncflt = 0 ;
    while(p) {
      if((p->fq1 != 0) && (p->fq0 != 0)) {
	ncflt++ ;
      }  
      p = p->right ;
    }
    break ;
  }  
  case BG : {
    xpl_BG *p ;
    freq_node *q ;
    
    p = (xpl_BG *)xpl->root ; ncflt = 0 ;
    while(p) {
      q = p->freqlist ;
      if((q->freq != 0) && (q->next)) {
	ncflt++ ;
      }  
      p = p->right ;
    }
  }
  break ;

  case GG :  
  case WKF :
  case WKC :
  case WK3F:
  case GG3 : {
    pac_error(MSG, "This operator is not supported yet");
    /*xpl_GG *p ;
    freq_node *q ;
    
    p = (xpl_GG *)xpl->root ; ncflt = 0 ;
    while(p) {
      q = p->freqlist ;
      if((q->freq != 0) && (q->next)) {
	ncflt++ ;
      }
      p = p->right ;
    }*/
  }
  break ;
  
  case GB :
  default :
    (void)pac_error(1, "xpl_conflicts: Mapping %d is not implemented yet.", type) ;
    return(-1) ;
    break ;
  
  }

  return(ncflt) ;

}

/*
     -------------------------------------------
     FUNCTION: xpl_sort
     -------------------------------------------
*/

int         /*+ Purpose: sort examples in a XPL set by frequency           +*/
  xpl_sort(
    xpl_t *xpl                         /*+ In/Out: XPL set                 +*/
  )
/*+ Return: 1 on success, 0 on failure                                     +*/
{
/*  author: Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)                 */
/*  date: Fri Nov 29 1996                                                   */

/*  Modification by:  Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)       */
/*  Date: Tue Jan 13 1998                                                   */
/*  Mod: Modified to treat the case WK                                      */

/*  Modification by:  Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)       */
/*  Date: Wed Jul 21 1999                                                   */
/*  Mod: Modified to treat the case BG                                      */



  int type ;

  type = xpl_get_type(xpl) ;

  switch(type) {
    case BB : {
      xpl_BB *p ;

      p = NULL ;
      if(!xpl_sort_BB((xpl_BB *)xpl->root, &p)) {
        return pac_error(MSG, "xpl_sort : xpl_sort_BB() failed.") ;
      }
      xpl->root = (int *)p ;
    }
    break ;

    case BG : {
      pac_error(MSG, "This operator is not supported yet");
      /*xpl_BG *p ;
      p = NULL ;
      if(!xpl_sort_BG((xpl_BG *)xpl->root, &p)) {
        return pac_error(MSG, "xpl_sort : xpl_sort_BG() failed.") ;
      }
      xpl->root = (int *)p ;*/
    }
    break ;

    case GG :
    case WKF:
    case WKC:
    case WKGG2C:
    case WKGG2F:
    case WK3C:
    case WK3F:
    case GG3: {
      pac_error(MSG, "This operator is not supported yet");
      /*xpl_GG *p ;

      p = NULL ;
      if(!xpl_GG_sort((xpl_GG *)xpl->root, &p)) {
        return pac_error(MSG, "xpl_sort : xpl_GG_sort() failed.") ;
      }
      xpl->root = (int *)p ;*/
    }
    break ;

    case GB :
    default : {
      (void)pac_error(1, "xpl_sort: Mapping %d is not implemented yet.", type) ;
      return(-1) ;
    }
    break ;

  }

  return(1) ;
}
