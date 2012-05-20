#include <pacbasic.h>

/* #define _DEBUG_ */
/* #define _DEBUG_2_ */


/* --------------------------------------------------------------- *

   Below, a simple description of the window structure :

                                _
                                /
                               /---> bands (depth)
          window matrix width / 
          <----------------> /
                            /
          +--+--+-...---+--+    _
          |  |  |       |  |    |
          +--+--+-...---+--+    |
          |  |  |       |  |    |
          +--+--+-...---+--+    |------> window matrix height
          :  :  :       :  :    |
          +--+--+-...---+--+    |
          |  |  |       |  |    |
          +--+--+-...---+--+    -

          The points (i,j,k) of the window matrix whose value is 1
          define the window format.


   Internal representation :

                                          +----> (height*width*bands)-1
           0                              |
          +--+--+--+--+--+--+--+...---+--+--+
          |  |  |  |  |  |  |  |      |  |  | 
          +--+--+--+--+--+--+--+...---+--+--+

          This array of bytes is created scanning the window matrix
          in the raster order, band 0 to nbands.


   The window structure contains the following informations :

          - window size
          - window matrix height, width and bands
          - window data (the array of bytes representing the matrix)

 * --------------------------------------------------------------- */


/* Modification by:  Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)       */
/* Date: Wed Feb 16 2000                                                   */
/* Mod: Window structure (1) changed to support multiple bands, (2) no     */
/*      longer holds information about aperture. Modifications were made   */
/*      to adequate to the structure changing.                             */


/*
     -------------------------------------------
     FUNCTION: win_get_wsize
     -------------------------------------------
*/

int           /*+ Purpose: get the window size                             +*/
  win_get_wsize(
    window_t *win     /*+ In: a pointer to a window                        +*/
  )
/*+ Return: the window size                                                +*/
{
/*  author: Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)                 */
/*  date: Fri Nov 29 1996                                                   */
 
  return( win->wsize[win_get_nbands(win)] ) ;
}


/*
     -------------------------------------------
     FUNCTION: win_set_wsize
     -------------------------------------------
*/

void      /*+ Purpose: set the window size (number of bands must be set)   +*/
  win_set_wsize(
    window_t *win,    /*+ In: a pointer to a window                        +*/
    int       wsize   /*+ In: wsize                                        +*/
  )
/*+ Return: nothing                                                        +*/
{
/*  author: Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)                 */
/*  date: Fri Nov 29 1996                                                   */
 
  win->wsize[ win_get_nbands(win) ] = wsize ;
}


/*
     -------------------------------------------
     FUNCTION: win_get_band_wsize
     -------------------------------------------
*/

int           /*+ Purpose: get the window size of a given band             +*/
  win_get_band_wsize(
    window_t *win,     /*+ In: a pointer to a window                       +*/
    int      band      /*+ In: a band number                               +*/
  )
/*+ Return: the window size of a given band                                +*/
{
/*  author: Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)                 */
/*  date: Thu Feb 17 2000                                                   */
  
  return( win->wsize[band-1] ) ;
}


/*
     -------------------------------------------
     FUNCTION: win_get_width
     -------------------------------------------
*/

int              /*+ Purpose: get the window matrix width                  +*/   
  win_get_width(
    window_t *win     /*+ In: a pointer to a window                        +*/
  )
/*+ Return: the window matrix width                                        +*/
{
/*  author: Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)                 */
/*  date: Fri Nov 29 1996                                                   */

  return(win->width) ;
}


/*
     -------------------------------------------
     FUNCTION: win_get_height
     -------------------------------------------
*/

int                  /*+ Purpose: get the window matrix height             +*/
  win_get_height(
    window_t *win    /*+ In: a pointer to a window                         +*/
  ) 
/*+ Return: the window matrix height                                       +*/
{
/*  author: Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)                 */
/*  date: Fri Nov 29 1996                                                   */
 
  return(win->height) ;
}


/*
     -------------------------------------------
     FUNCTION: win_get_nbands
     -------------------------------------------
*/

int                  /*+ Purpose: get the number of bands of a window      +*/
  win_get_nbands(
    window_t *win    /*+ In: a pointer to a window                         +*/
  ) 
/*+ Return: the number of bands of the window                              +*/
{
/*  author: Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)                 */
/*  date: Fri Nov 29 1996                                                   */
 
  return(win->nbands) ;
}

/*
     -------------------------------------------
     FUNCTION: win_set_nbands
     -------------------------------------------
*/

void                 /*+ Purpose: set the number of bands of a window      +*/
  win_set_nbands(
    window_t *win,   /*+ In: a pointer to a window                         +*/
    int      nbands
  ) 
/*+ Return: nothing                                                        +*/
{
/*  author: Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)                 */
/*  date: Fri Nov 29 1996                                                   */
 
  win->nbands = nbands ;
}


/*
     -------------------------------------------
     FUNCTION: win_get_data
     -------------------------------------------
*/

char *       /*+ Purpose: get the window data                              +*/
  win_get_data(
    window_t *win      /*+ In: a pointer to a window                       +*/
  )
/*+ Return: the window data                                                +*/
{
/*  author: Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)                 */
/*  date: Fri Nov 29 1996                                                   */
 
  return(win->windata) ;
}


/*
     -------------------------------------------
     FUNCTION: win_set_point
     -------------------------------------------
*/

int             /*+ Purpose: set a value (0 or 1) on window matrix         +*/
  win_set_point(
    int i,          /*+ In: row number of the point to be set              +*/
    int j,          /*+ In: column number of the point to be set           +*/
    int k,          /*+ In: band of the point to be set                    +*/
    int value,      /*+ In: value to be set                                +*/
    window_t *win   /*+ In/Out: a pointer to a window                      +*/
  )
/*+ Return: 1 on success, 0 on failure                                    +*/
{
/*  author: Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)                */
/*  date: Fri Nov 29 1996                                                  */

/* Date: Wed Feb 16 2000                                                   */
/* Mod: Window structure changed to support multiple bands. Changes to     */
/*      adequate to the structure changing                                 */


  int nbands ;

  if(i >= win->height) {
    return pac_error(1, "Window point is out of its dimension.") ;
  }
  if(j >= win->width) {
    return pac_error(1, "Window point is out of its dimension.") ;
  }

  nbands = win->nbands ;
  if(k > nbands) {
    return pac_error(1, "Window point is out of its dimension.") ;
  }

#ifdef _DEBUG_2_
pac_debug("nbands=%d k=%d i=%d j=%d pos=%d", nbands, k, i, j, (k-1)*win->width*win->height + i*win->width + j) ;
#endif

  win->windata[ (k-1)*win->width*win->height + i*win->width + j ] = (char)value ;
  if(value==1) {
    (win->wsize[k-1])++ ;
    (win->wsize[nbands])++ ;
  }

  return(1) ;

}


/*
     -------------------------------------------
     FUNCTION: win_get_point
     -------------------------------------------
*/

int            /*+ Purpose: get the value of a point from window data     +*/
  win_get_point(
    int i,           /*+ In: Row number of the point                       +*/
    int j,           /*+ In: column number of the point                    +*/
    int k,           /*+ In: band of the point                             +*/
    window_t *win    /*+ In: a pointer to a window                         +*/
  )
/*+ Return: the value of the window matrix at win->windata(i,j)            +*/
{
/*  author: Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)                 */
/*  date: Fri Nov 29 1996                                                   */

/* Date: Wed Feb 16 2000                                                   */
/* Mod: Window structure changed to support multiple bands. Changes to     */
/*      adequate to the structure changing                                 */


  if(i >= win->height) {
    return pac_error(1,"Window point is out of its dimension.") ;
  }
  if(j >= win->width) {
    return pac_error(1,"Window point is out of its dimension.") ;
  }
  if(k > win->nbands) {
    return pac_error(1, "Window point is out of its dimension.") ;
  }

  return((int)win->windata[ (k-1)*win->width*win->height + i*win->width + j ]) ;

}



/*
     -------------------------------------------
     FUNCTION: win_create
     -------------------------------------------
*/

window_t *     /*+ Purpose: create  a window                               +*/
  win_create(
    int height,       /*+ In: height of the window                         +*/
    int width,        /*+ In: width of the window                          +*/
    int nbands        /*+ In: number of bands of the window                +*/   
  )
/*+ Return: a pointer to the window on success, NULL on failure            +*/ 
{
/* author: Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)                  */
/* date: Fri Nov 29 1996                                                    */

/*  Modification by:  Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)       */
/*  Date: Thu Oct 30 1997                                                   */
/*  Mod: Modified to include wkrange                                        */

/*  Modification by:  Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)       */
/*  Date: Wed Jun  3 1998                                                   */
/*  Mod: Modified to include the vertical placement                         */

/*  Modification by:  Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)       */
/*  Date: Fri Oct 29 1999                                                   */
/*  Mod: Modified to include support to input/output ranges                 */

/* Date: Wed Feb 16 2000                                                   */
/* Mod: Window structure changed to support multiple bands. Changes to     */
/*      adequate to the structure changing                                 */

  window_t *win ;
  int      i ;

  if((width > MAXWINWIDTH) || (height > MAXWINHEIGHT) ||
     (nbands > MAXWINBANDS)) {
    return (window_t *)pac_error(1, "Window dimension must be less than %dx%dx%d.",
                    MAXWINHEIGHT, MAXWINWIDTH, MAXWINBANDS) ;
  }

  win = (window_t *)malloc(sizeof(window_t)) ;
  if(win==NULL) {
    return (window_t *)pac_error(1, "Memory allocation failed.") ;
  }

  win->wsize = (int *)malloc(sizeof(int)*(nbands+1)) ;
  if(win->wsize==NULL) {
    return (window_t *)pac_error(1, "Memory allocation failed.") ;
  }

  win->windata = (char *)malloc(sizeof(char)*width*height*nbands) ;
  if(win->windata==NULL) {
    return (window_t *)pac_error(1, "Memory allocation failed.") ;
  }

  win->height = height ;
  win->width = width ;
  win->nbands = nbands ;

  for(i=0; i<nbands+1; i++) {
    win->wsize[i] = 0 ;
  }

  for(i=0; i<width*height*nbands; i++) {
    win->windata[i] = 0 ;
  }

  return(win) ;

}


/*
     -------------------------------------------
     FUNCTION: win_free
     -------------------------------------------
*/

void           /*+ Purpose: free the memory area of a window               +*/
  win_free(
    window_t *win                /*+ In: a pointer to a window             +*/
  )
/*+ Return: nothing                                                        +*/
{
/*  author: Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)                 */
/*  date: Fri Nov 29 1996                                                   */
 
  if(win) {
    if(win->wsize) free(win->wsize) ;
    if(win->windata) free(win->windata) ;
    free(win) ;
  }
}







