#include <pacbasic.h>

/* #define _DEBUG_ */

/*
     -------------------------------------------
     FUNCTION: show_create
     -------------------------------------------
*/

show_t *     /*+ Purpose: create a SHOW structure                          +*/
  show_create(
    window_t *win,            /*+ In: window                               +*/
    int      map_type,        /*+ In: mapping type                         +*/
    int      band             /*+ In: window band                          +*/
  )
/*+ Return: a pointer to a SHOW structure on success, NULL on failure      +*/
{
/* author: Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)                  */
/* date: Fri Nov 29 1996                                                    */

/*  Modification by:  Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)       */
/*  Date: Tue Aug 24 1999                                                   */
/*  Mod: Modifictaions to implement BG and GG (or WKGG) types               */

  show_t *show ;
  int    height, width, wsize ;
  int    wpat_vec_size, row_length ;
  int    i, j, k, lg ;



  /* allocate show structure */ 
  show = (show_t *)malloc(sizeof(show_t)) ;
  if(!show) {
    return (show_t *)pac_error(1, "Memory allocation error.") ;
  }


  /* get window dimensions */
  height = win_get_height(win) ;
  width = win_get_width(win) ;
  wsize = win_get_band_wsize(win, band) ;


  /* initialize some fields in the show structure */
  show->up_half = (height+1)/2 - 2 ;
  show->mid_row = (height+1)/2 - 1 ;
  show->up = height-1 ;
  show->wsize = wsize ;
  show->wzip = size_of_zpat(wsize) ;
  show->map_type = map_type ;


  if((map_type == BB) || (map_type == BG)) {
    lg = 1 ;
  }
  else if((map_type == WKF) || (map_type == WKC) || (map_type == GG)) {
    lg = 4 ;
  }
  else {
    return (show_t *)pac_error(1, 
      "show_create: mapping %d not implemented.", map_type) ;
  }

  row_length = width*lg + width ;
  show->row_length = row_length ;
  wpat_vec_size = row_length*height ;


  /* Allocate buffer for the show structure */ 
  show->wpat_vec = (char *)malloc(sizeof(char)*wpat_vec_size) ;
  show->idx_vec = (int *)malloc(sizeof(int)*wsize) ;


  /* Fill idx_vec. idx_vec[p] will point to the position at
     wpat_vec[] where the element corresponding to point p of
     the window will be placed */ 
  k = 0 ;
  for(i=0; i<height; i++) {
    for(j=0; j<width; j++) {
      if(win->windata[(band-1)*width*height + width*i+j]==1) {
        show->idx_vec[k] = row_length*i + j*(lg+1) ;
        k++ ;
      }
    }
  }

  /* Fill wpat_vec[] with blank characters. End of each row
     will be filled with end of string character            */

  for(i=0; i<wpat_vec_size; i++) {
    show->wpat_vec[i] = ' ' ;
  }
  for(i=0; i<height; i++) {
    show->wpat_vec[row_length*(i+1)-1] = '\0' ;  
  }

#ifdef _DEBUG_
for(i=0; i<wsize; i++) {
  pac_debug("idx_vec[%d] = %d", i, show->idx_vec[i]) ;
}
for(i=0; i<height; i++) {
  pac_debug("wpat_vec[%d] = %s", i, &show->wpat_vec[row_length*i]) ;
}
#endif


  return(show) ;    
}



/*
     -------------------------------------------
     FUNCTION: wpat2vecB
     -------------------------------------------
*/

void      /*+ Purpose: set the string in the SHOW structure in a way that
                      represents the geometrical format of the w-pattern   +*/
  wpat2vecB(
    unsigned int *wpat, /*+ In: w-pattern                                  +*/
    show_t *show        /*+ In/Out: SHOW structure                         +*/
  )
/*+ Return: nothing                                                        +*/
{
/* author: Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)                  */
/* date: Fri Nov 29 1996                                                    */

  int   i, k, idx ;
  char  *wpat_vec ;


  wpat_vec = show->wpat_vec ;

  for(i=0; i<show->wsize; i++) {
    k = i/NB ;
    idx = i%NB ;
    if((wpat[k] & bitmsk[idx])!=0) {
      wpat_vec[show->idx_vec[i]] = '1' ;
    }
    else {
      wpat_vec[show->idx_vec[i]] = '0' ;
    }
  }

}


/*
     -------------------------------------------
     FUNCTION: wpat2vecG
     -------------------------------------------
*/

void      /*+ Purpose: set the string in the SHOW structure in a way that
                      represents the geometrical format of the w-pattern  
                      Values of w-pattern are assumed signed               +*/
  wpat2vecG(
    char   *wpat,   /*+ In: w-pattern                                      +*/
    show_t *show    /*+ In/Out: SHOW structure                             +*/
  )
/*+ Return: nothing                                                        +*/
{
/* author: Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)                  */
/* date: Tue Aug 24 1999                                                    */

  char  str[5] ;
  char  *wpat_vec ;
  int   i, j ;


  wpat_vec = show->wpat_vec ;

  for(i=0; i<show->wsize; i++) {
    sprintf(str, "%4d", (int)wpat[i]) ;
    for(j=0;j<4; j++) {
      wpat_vec[show->idx_vec[i]+j] = str[j] ;
    }
  }

#ifdef _DEBUG_
  for(i=0; i<=show->up; i++) {
    pac_debug("%s\n", &wpat_vec[show->row_length*i]) ;
  }
#endif

}


/*
     -------------------------------------------
     FUNCTION: wpat2vecUG
     -------------------------------------------
*/

void      /*+ Purpose: set the string in the SHOW structure in a way that
                      represents the geometrical format of the w-pattern.
                      Values of w-pattern are assumed unsigned             +*/
  wpat2vecUG(
    char   *wpat,   /*+ In: w-pattern                                      +*/
    show_t *show    /*+ In/Out: SHOW structure                             +*/
  )
/*+ Return: nothing                                                        +*/
{
/* author: Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)                  */
/* date: Tue Aug 24 1999                                                    */

  char  str[5] ;
  char  *wpat_vec ;
  int   i, j ;
  unsigned char  value ;


  wpat_vec = show->wpat_vec ;

  for(i=0; i<show->wsize; i++) {
    value = (unsigned char)wpat[i] ;

    sprintf(str, "%4d", (int)value) ;
    for(j=0;j<4; j++) {
      wpat_vec[show->idx_vec[i]+j] = str[j] ;
    }
  }

#ifdef _DEBUG_
  for(i=0; i<=show->up; i++) {
    pac_debug("%s\n", &wpat_vec[show->row_length*i]) ;
  }
#endif

}


/*
     -------------------------------------------
     FUNCTION: show_free
     -------------------------------------------
*/
 
void            /*+ Purpose: free a show structure                         +*/
  show_free(                                                                  
    show_t *show       /*+ In: pointer to the show structure               +*/
)
/*+ Return: nothing                                                        +*/
{
 
/* author: Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)                  */
/* date: Tue Dec  3 1996                                                    */

  if(show) {
    if(show->wpat_vec) free(show->wpat_vec) ;
    if(show->idx_vec) free(show->idx_vec) ;
  }
  free(show) ;
}





