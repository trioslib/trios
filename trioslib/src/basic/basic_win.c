#include <trios.h>

/* #define _DEBUG_
 #define _DEBUG_2_*/


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


/*!
    Get the window size.

    \param win Pointer to window.
    \return The window size.
*/
int win_get_wsize(window_t *win) {
/*  author: Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)                 */
/*  date: Fri Nov 29 1996                                                   */
 
  return( win->wsize[win_get_nbands(win)] ) ;
}


/*!
    Set the window size. The number of bands must be set.

    \param win Pointer to window.
    \param wsize Window size.
*/
void win_set_wsize(window_t *win, int wsize) {
/*  author: Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)                 */
/*  date: Fri Nov 29 1996                                                   */
 
  win->wsize[ win_get_nbands(win) ] = wsize ;
}


/*!
    Get the window size of a given band.

    \param win Pointer to window.
    \param band Band number.
    \return The window size of the band.
*/
int win_get_band_wsize(window_t *win, int band) {
/*  author: Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)                 */
/*  date: Thu Feb 17 2000                                                   */
  
  return( win->wsize[band-1] ) ;
}


/*!
    Get the window width.

    \param win Pointer to window.
    \return The window width.
*/
int win_get_width(window_t *win) {
/*  author: Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)                 */
/*  date: Fri Nov 29 1996                                                   */

  return(win->width) ;
}


/*!
    Get the window height.

    \param win Pointer to window.
    \return The window height.
*/
int win_get_height(window_t *win) {
/*  author: Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)                 */
/*  date: Fri Nov 29 1996                                                   */
 
  return(win->height) ;
}


/*!
    Get the number of bands of a window.

    \param win Pointer to window.
    \return The number of bands of a window.
*/
int win_get_nbands(window_t *win) {
/*  author: Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)                 */
/*  date: Fri Nov 29 1996                                                   */
 
  return(win->nbands) ;
}

/*!
    Set the number of bands of a window.

    \param win Pointer to window.
    \param nbands Number of bands.
*/
void win_set_nbands(window_t *win, int nbands) {
/*  author: Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)                 */
/*  date: Fri Nov 29 1996                                                   */
 
  win->nbands = nbands ;
}


/*!
    Get the window data.

    \param win Pointer to window.
    \return The window data pointer.
*/
char *win_get_data(window_t *win) {
/*  author: Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)                 */
/*  date: Fri Nov 29 1996                                                   */
 
  return(win->windata) ;
}


/*!
    Set a value (0 or 1) on the window. All elements with value 1 are included in the window.

    \param win Pointer to window.
    \param i Row number.
    \param j Column number.
    \param k Band number.
    \param value Value of the (i,j,k) element.
    \return 1 on success. 0 on failure.
*/
int win_set_point(int i, int j, int k, int value, window_t *win) {
/*  author: Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)                */
/*  date: Fri Nov 29 1996                                                  */

/* Date: Wed Feb 16 2000                                                   */
/* Mod: Window structure changed to support multiple bands. Changes to     */
/*      adequate to the structure changing                                 */


  int nbands ;

  if(i >= win->height) {
    return trios_error(1, "Window point is out of its dimension.") ;
  }
  if(j >= win->width) {
    return trios_error(1, "Window point is out of its dimension.") ;
  }

  nbands = win->nbands ;
  if(k > nbands) {
    return trios_error(1, "Window point is out of its dimension.") ;
  }

#ifdef _DEBUG_2_
trios_debug("nbands=%d k=%d i=%d j=%d pos=%d", nbands, k, i, j, (k-1)*win->width*win->height + i*win->width + j) ;
#endif

  win->windata[ (k-1)*win->width*win->height + i*win->width + j ] = (char)value ;
  if(value==1) {
    (win->wsize[k-1])++ ;
    (win->wsize[nbands])++ ;
  }

  return(1) ;

}


/*!
    Get the value of an element in the window.

    \param win Pointer to window.
    \param i Row number.
    \param j Column number.
    \param k Band number.
    \return The value of the window matrix at win->windata(i,j).
*/
int win_get_point(int i, int j, int k, window_t *win)
/*+ Return: the value of the window matrix at win->windata(i,j)            +*/
{
/*  author: Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)                 */
/*  date: Fri Nov 29 1996                                                   */

/* Date: Wed Feb 16 2000                                                   */
/* Mod: Window structure changed to support multiple bands. Changes to     */
/*      adequate to the structure changing                                 */


  if(i >= win->height) {
    return trios_error(1,"Window point is out of its dimension.") ;
  }
  if(j >= win->width) {
    return trios_error(1,"Window point is out of its dimension.") ;
  }
  if(k > win->nbands) {
    return trios_error(1, "Window point is out of its dimension.") ;
  }

  return((int)win->windata[ (k-1)*win->width*win->height + i*win->width + j ]) ;

}



/*!
    Create a window.

    \param height Window heigth.
    \param width Window width.
    \param nbands Number of bands.
    \return A window with the desired size. NULL on error.
  */
window_t *win_create(int height, int width, int nbands) {
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
    return (window_t *)trios_error(1, "Window dimension must be less than %dx%dx%d.",
                    MAXWINHEIGHT, MAXWINWIDTH, MAXWINBANDS) ;
  }

  win = (window_t *)malloc(sizeof(window_t)) ;
  if(win==NULL) {
    return (window_t *)trios_error(1, "Memory allocation failed.") ;
  }

  win->wsize = (int *)malloc(sizeof(int)*(nbands+1)) ;
  if(win->wsize==NULL) {
    return (window_t *)trios_error(1, "Memory allocation failed.") ;
  }

  win->windata = (char *)malloc(sizeof(char)*width*height*nbands) ;
  if(win->windata==NULL) {
    return (window_t *)trios_error(1, "Memory allocation failed.") ;
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


/*!
    Free the memory of a window.

    \param win Pointer to window.
  */
void win_free(window_t *win) {
/*  author: Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)                 */
/*  date: Fri Nov 29 1996                                                   */
 
  if(win) {
    if(win->wsize) free(win->wsize) ;
    if(win->windata) free(win->windata) ;
    free(win) ;
  }
}








