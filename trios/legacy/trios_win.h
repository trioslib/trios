/* Window definition */

#include "stdlib.h"
#include "stdio.h"

#ifndef _trios_win_h_
#define _trios_win_h_

#ifdef __cplusplus
extern "C" {
#endif


#define MAXWINWIDTH  256
#define MAXWINHEIGHT 256
#define MAXWINBANDS  256

/*!
    Structure that holds a W-operator window. Below, a simple description of the window structure :

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
 */
typedef struct {
  unsigned char width ;  /*!< window matrix width (1<=width<=256)  */
  unsigned char height ; /*!< window matrix height (1<=width<=256) */
  unsigned char nbands ; /*!< window matrix bands (1<=bands<=256)  */
  int  *wsize ;          /*!< window size                          */
  char *windata ;        /*!< window description array             */
} window_t ;


int           /*+ Purpose: get the window size of a given band             +*/
  win_get_band_wsize(
    window_t *win,     /*+ In: a pointer to a window                       +*/
    int      band      /*+ In: a band number                               +*/
    ) ;


int           /*+ Purpose: get the window size                             +*/
  win_get_wsize(
    window_t *win     /*+ In: a pointer to a window                        +*/
    ) ;

void      /*+ Purpose: set the window size (number of bands must be set)   +*/
  win_set_wsize(
    window_t *win,    /*+ In: a pointer to a window                        +*/
    int       wsize   /*+ In: wsize                                        +*/
  ) ;

int              /*+ Purpose: get the window matrix width                  +*/   
  win_get_width(
    window_t *win     /*+ In: a pointer to a window                        +*/
    ) ;

int                  /*+ Purpose: get the window matrix height             +*/
  win_get_height(
    window_t *win    /*+ In: a pointer to a window                         +*/
    ) ;

int                  /*+ Purpose: get the number of bands of a window      +*/
  win_get_nbands(
    window_t *win    /*+ In: a pointer to a window                         +*/
    ) ;

void                 /*+ Purpose: set the number of bands of a window      +*/
  win_set_nbands(
    window_t *win,   /*+ In: a pointer to a window                         +*/
    int      nbands
  ) ;

char *       /*+ Purpose: get the window data                              +*/
  win_get_data(
    window_t *win      /*+ In: a pointer to a window                       +*/
    ) ;

int             /*+ Purpose: set a value (0 or 1) on window matrix         +*/
  win_set_point(
    int i,          /*+ In: row number of the point to be set              +*/
    int j,          /*+ In: column number of the point to be set           +*/
    int k,          /*+ In: band of the point to be set                    +*/
    int value,      /*+ In: value to be set                                +*/
    window_t *win   /*+ In/Out: a pointer to a window                      +*/
    ) ;

int            /*+ Purpose: get the value of a point from window data     +*/
  win_get_point(
    int i,           /*+ In: Row number of the point                       +*/
    int j,           /*+ In: column number of the point                    +*/
    int k,           /*+ In: band of the point                             +*/
    window_t *win    /*+ In: a pointer to a window                         +*/
    ) ;

window_t *     /*+ Purpose: create  a window                               +*/
  win_create(
    int height,       /*+ In: height of the window                         +*/
    int width,        /*+ In: width of the window                          +*/
    int nbands        /*+ In: number of bands of the window                +*/   
    ) ;

void           /*+ Purpose: free the memory area of a window               +*/
  win_free(
    window_t *win                /*+ In: a pointer to a window             +*/
    ) ;

window_t *win_read(char *fname);


window_t *win_read_data(FILE * fd);
void win_write_data(FILE * fd, window_t * win);


#ifdef __cplusplus
}
#endif


#endif /* _trios_win_h_ */




