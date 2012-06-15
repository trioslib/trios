/* Window definition */

#ifndef _pac_win_h_
#define _pac_win_h_

#define MAXWINWIDTH  256
#define MAXWINHEIGHT 256
#define MAXWINBANDS  256


typedef struct {
  unsigned char width ;  /* window matrix width (1<=width<=256)  */
  unsigned char height ; /* window matrix height (1<=width<=256) */
  unsigned char nbands ; /* window matrix bands (1<=bands<=256)  */
  int  *wsize ;          /* window size                          */
  char *windata ;        /* window description array             */
} window_t ;


/*===========================================================================\
   pac_win.c
\========================================================================== */


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


#endif /* _pac_win_h_ */




