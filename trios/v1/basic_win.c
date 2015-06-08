#include "trios_error.h"
#include "trios_win.h"
#include "stdlib.h"

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

/* Mod: Window structure (1) changed to support multiple bands, (2) no     */
/*      longer holds information about aperture. Modifications were made   */
/*      to adequate to the structure changing.                             */

/*!
    Get the window size.

    \param win Pointer to window.
    \return The window size.
*/
int win_get_wsize(window_t * win)
{
	return (win->wsize[win_get_nbands(win)]);
}

/*!
    Set the window size. The number of bands must be set.

    \param win Pointer to window.
    \param wsize Window size.
*/
void win_set_wsize(window_t * win, int wsize)
{
	win->wsize[win_get_nbands(win)] = wsize;
}

/*!
    Get the window size of a given band.

    \param win Pointer to window.
    \param band Band number.
    \return The window size of the band.
*/
int win_get_band_wsize(window_t * win, int band)
{
	return (win->wsize[band - 1]);
}

/*!
    Get the window width.

    \param win Pointer to window.
    \return The window width.
*/
int win_get_width(window_t * win)
{
	return (win->width);
}

/*!
    Get the window height.

    \param win Pointer to window.
    \return The window height.
*/
int win_get_height(window_t * win)
{
	return (win->height);
}

/*!
    Get the number of bands of a window.

    \param win Pointer to window.
    \return The number of bands of a window.
*/
int win_get_nbands(window_t * win)
{
	return (win->nbands);
}

/*!
    Set the number of bands of a window.

    \param win Pointer to window.
    \param nbands Number of bands.
*/
void win_set_nbands(window_t * win, int nbands)
{
	win->nbands = nbands;
}

/*!
    Get the window data.

    \param win Pointer to window.
    \return The window data pointer.
*/
char *win_get_data(window_t * win)
{
	return (win->windata);
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
int win_set_point(int i, int j, int k, int value, window_t * win)
{

	int nbands;

	if (i >= win->height) {
		return trios_error(1, "Window point is out of its dimension.");
	}
	if (j >= win->width) {
		return trios_error(1, "Window point is out of its dimension.");
	}

	nbands = win->nbands;
	if (k > nbands) {
		return trios_error(1, "Window point is out of its dimension.");
	}
#ifdef _DEBUG_2_
	trios_debug("nbands=%d k=%d i=%d j=%d pos=%d", nbands, k, i, j,
		    (k - 1) * win->width * win->height + i * win->width + j);
#endif

	if (value == 1
	    && win->windata[(k - 1) * win->width * win->height +
			    i * win->width + j] == 0) {
		(win->wsize[k - 1])++;
		(win->wsize[nbands])++;
	} else if (value == 0
		   && win->windata[(k - 1) * win->width * win->height +
				   i * win->width + j] == 1) {
		(win->wsize[k - 1])--;
		(win->wsize[nbands])--;
	}
	win->windata[(k - 1) * win->width * win->height + i * win->width + j] =
	    (char)value;

	return (1);

}

/*!
    Get the value of an element in the window.

    \param win Pointer to window.
    \param i Row number.
    \param j Column number.
    \param k Band number.
    \return The value of the window matrix at win->windata(i,j).
*/
int win_get_point(int i, int j, int k, window_t * win)
/*+ Return: the value of the window matrix at win->windata(i,j)            +*/
{

	if (i >= win->height) {
		return trios_error(1,
				   "Window point %d %d %d is out of its dimension %d %d %d.",
				   i, j, k, win->height, win->width,
				   win->nbands);
	}
	if (j >= win->width) {
		return trios_error(1,
				   "Window point %d %d %d is out of its dimension %d %d %d.",
				   i, j, k, win->height, win->width,
				   win->nbands);
	}
	if (k > win->nbands) {
		return trios_error(1,
				   "Window point %d %d %d is out of its dimension nb %d %d %d.",
				   i, j, k, win->height, win->width,
				   win->nbands);
	}

	return ((int)win->
		windata[(k - 1) * win->width * win->height + i * win->width +
			j]);

}

/*!
    Create a window.

    \param height Window heigth.
    \param width Window width.
    \param nbands Number of bands.
    \return A window with the desired size. NULL on error.
  */
window_t *win_create(int height, int width, int nbands)
{
	window_t *win;
	int i;

	if ((width > MAXWINWIDTH) || (height > MAXWINHEIGHT) ||
	    (nbands > MAXWINBANDS)) {
		return (window_t *) trios_error(1,
						"Window dimension must be less than %dx%dx%d.",
						MAXWINHEIGHT, MAXWINWIDTH,
						MAXWINBANDS);
	}

	win = (window_t *) malloc(sizeof(window_t));
	if (win == NULL) {
		return (window_t *) trios_error(1, "Memory allocation failed.");
	}

	win->wsize = (int *)malloc(sizeof(int) * (nbands + 1));
	if (win->wsize == NULL) {
		return (window_t *) trios_error(1, "Memory allocation failed.");
	}

	win->windata = (char *)malloc(sizeof(char) * width * height * nbands);
	if (win->windata == NULL) {
		return (window_t *) trios_error(1, "Memory allocation failed.");
	}

	win->height = height;
	win->width = width;
	win->nbands = nbands;

	for (i = 0; i < nbands + 1; i++) {
		win->wsize[i] = 0;
	}

	for (i = 0; i < width * height * nbands; i++) {
		win->windata[i] = 0;
	}

	return (win);

}

/*!
    Free the memory of a window.

    \param win Pointer to window.
  */
void win_free(window_t * win)
{
	if (win) {
		if (win->wsize)
			free(win->wsize);
		if (win->windata)
			free(win->windata);
		free(win);
	}
}
