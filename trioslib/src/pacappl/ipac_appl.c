#include <paclib.h>
#include "pacappl_local.h"

/* #define _DEBUG_ */


/*
     -------------------------------------------
     FUNCTION: papplic_bx
     -------------------------------------------
*/

int             /*+ Purpose: Applies an operator (given by a set of intervals)
                    on a binary image. Produces a BYTE image               +*/
  papplic_bx(
    unsigned short *uspixels,  /*+ In: Image to be operated                +*/
    unsigned char  *ucpixels1, /*+ In: Mask image                          +*/
    unsigned char  *ucpixels2, /*+ Out: resulting output image             +*/
    int      cv,       /*+ In: on template matching, looks only to pixels
		               under the window with the same label as the
                               center's one                                +*/
    itv_t   *itv,      /*+ In: basis file                                  +*/
    window_t  *win,    /*+ In: window                                      +*/
    int     width,     /*+ In: number of columns of the image              +*/
    int     npixels    /*+ In: number of pixels                            +*/
)
/*+ Return: 1 on success, 0 on failure                                     +*/
{

/* author: Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)                  */
/* date: Fri Nov 29 1996                                                    */

  int label ;
  int i, j, k , s, nshifts ;
  int wsize , wzip ;
  int *offset ;         /* pointer to an array of offsets */
  unsigned int *wpat ;  /* pointer to an array of configurations */



#ifdef _DEBUG_
pac_debug("Entering papplic_bx()") ;
#endif

  wsize = win_get_band_wsize(win, 1) ;
  wzip = size_of_zpat(wsize) ;


  /* set vector of offsets */
  offset = (int *)malloc(sizeof(int)*wsize) ;
  if(offset==NULL) {
    return pac_error(MSG, "papplic_bx: offset_create() failed.") ;
  }

#ifdef _DEBUG_
pac_debug("It will set vector of offsets") ;
#endif
  offset_set(offset, win, width, 1) ;

#ifdef _DEBUG_
pac_debug("It called offset_set, cv = %d", cv) ;
for(i=0; i<wsize; i++) {
  pac_debug("offset[%d]=%d\n", i, offset[i]) ;
}
#endif


  wpat = (unsigned int *)malloc(sizeof(int)*size_of_zpat(wsize)) ;
  if(wpat == NULL) {
    return pac_error(1, "papplic_bx: Memory allocation failed.") ;
  }


  /* Begin case A : cv=0 (All pixels whose value is different from
           0 are considered in the w-pattern) */

  if(!cv) {

    for(i = 0; i < npixels; i++) {         /* shifts the window */

      if(ucpixels1[i] != 0) {              /* mask condition satisfied */

	for(j = 0; j < wzip; j++) wpat[j] = 0 ;     /* blank w-pattern */

        /* gets the w-pattern centered at the point j */
	for(j = 0; j < wsize; j++) {  /* for each point of the window...*/

	  k = i + offset[j] ;
	  if(uspixels[k] != 0) {
	    s = j/NB ;
	    nshifts = j%NB ;
	    wpat[s] = (wpat[s] | bitmsk[nshifts]) ;
	  }

	}

        /* checks if the configuration is not in the intervals */

	label = itv_list_contain(itv, wpat, wzip) ;

        /* if found, put the gray-level to the output image */

	ucpixels2[i] = (unsigned char)label ;

      }

    }

  }

  /* Begin case B : cv=1 (Only the pixels whose value is the same as the
     pixel under the center of the window are considered in the w-pattern */
  else {

    for(i = 0; i < npixels; i++) {         /* shifts the window */

      if(ucpixels1[i] != 0) {              /* mask condition satisfied */

	for(j = 0; j < wzip; j++) wpat[j] = 0 ;     /* blank w-pattern */

        /* if current pixel j has value=0, then pattern is the zero vector */
        if(uspixels[i]) {
          /*  get the w-pattern centered at the point i */
          for(j=0; j<wsize; j++) {      /* for each point of the window ... */

            k = i+offset[j] ;
            /* Only pixels with the same value as of the pixel j    */
            /* will be turned on in the w-pattern                   */
            if(uspixels[k]==uspixels[i]) {
              s = j/NB ;
              nshifts = j%NB ;
              wpat[s] = (wpat[s] | bitmsk[nshifts]) ;
	    }

	  }

	}

        /* checks whether the configuration is in the intervals */

        label = itv_list_contain(itv, wpat, wzip) ;

        /* if found, put the gray-level to the output image */

        ucpixels2[i] = (unsigned char)label ;

      }

    }

  }

  free(offset) ;
  free(wpat) ;

  return(1) ;

}

/*
     -------------------------------------------
     FUNCTION: papplicpar_bx
     -------------------------------------------
*/

int             /*+ Purpose: Applies the parity (modulo-2 sum) operator
                    on a binary image. Produces a BYTE image               +*/
  papplicpar_bx(
    unsigned short *uspixels,  /*+ In: Image to be operated                +*/
    unsigned char  *ucpixels1, /*+ In: Mask image                          +*/
    unsigned char  *ucpixels2, /*+ Out: resulting output image             +*/
    int      cv,       /*+ In: on template matching, looks only to pixels
                       under the window with the same label as the
                               center's one                                +*/
    window_t  *win,    /*+ In: window                                      +*/
    int     width,     /*+ In: number of columns of the image              +*/
    int     npixels    /*+ In: number of pixels                            +*/
)
/*+ Return: 1 on success, 0 on failure                                     +*/
{

/* author: Carlos S. Santos (csantos@ime.usp.br)                  */
/* author: Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)                  */
/* date: Thu Mar 15 2007                                                    */

  int label ;
  int i, j, k , accum;
  int wsize ;
  int *offset ;         /* pointer to an array of offsets */


#ifdef _DEBUG_
pac_debug("Entering papplicpar_bx()") ;
#endif

  wsize = win_get_band_wsize(win, 1) ;
  /* set vector of offsets */
  offset = (int *)malloc(sizeof(int)*wsize) ;
  if(offset==NULL) {
    return pac_error(MSG, "papplicpar_bx: offset_create() failed.") ;
  }

#ifdef _DEBUG_
pac_debug("It will set vector of offsets") ;
#endif
  offset_set(offset, win, width, 1) ;

#ifdef _DEBUG_
pac_debug("It called offset_set, cv = %d", cv) ;
for(i=0; i<wsize; i++) {
  pac_debug("offset[%d]=%d\n", i, offset[i]) ;
}
#endif

  /* Begin case A : cv=0 (All pixels whose value is different from
           0 are considered in the w-pattern) */
  if(!cv) {
    for(i = 0; i < npixels; i++) {         /* shifts the window */
      if(ucpixels1[i] != 0) {              /* mask condition satisfied */
        accum = 0;
        /* gets the w-pattern centered at the point j */
        for(j = 0; j < wsize; j++) {  /* for each point of the window...*/
          k = i + offset[j] ;
          if(uspixels[k] != 0) {
            accum = accum + 1;
          }
        }
        if (accum%2 == 0) {
            label = 0;
        } else {
            label = 255;
        }
        /* put the gray-level to the output image */
        ucpixels2[i] = (unsigned char)label ;
      }
    }
  }

  /* Begin case B : cv=1 (Only the pixels whose value is the same as the
     pixel under the center of the window are considered in the w-pattern */
  else {
    for(i = 0; i < npixels; i++) {         /* shifts the window */
      if(ucpixels1[i] != 0) {              /* mask condition satisfied */
        /* if current pixel j has value=0, then pattern is the zero vector */
        if(uspixels[i]) {
          accum = 0;
          for(j=0; j<wsize; j++) {      /* for each point of the window ... */
            k = i+offset[j] ;
            /* Only pixels with the same value as of the pixel j    */
            /* will be turned on in the w-pattern                   */
            if(uspixels[k]==uspixels[i]) {
               accum = accum + 1;
            }
          }
        }
        if (accum%2 == 0) {
            label = 0;
        } else {
            label = 255;
        }
        /* put the gray-level to the output image */
        ucpixels2[i] = (unsigned char)label ;
      }
    }
  }

  free(offset) ;
  return(1) ;
}





/*
     -------------------------------------------
     FUNCTION: papplic_short
     -------------------------------------------
*/

int             /*+ Purpose: Applies an operator (given by a set of intervals)
                    on a binary image and produces a gray-scale SHORT image  +*/
  papplic_short(
    unsigned short *uspixels,  /*+ In: Image to be operated                +*/
    unsigned char  *ucpixels1, /*+ In: Mask image                          +*/
    unsigned short *uspixels2, /*+ Out: resulting output image             +*/
    int      cv,       /*+ In: on template matching, looks only to pixels
		               under the window, with the same label as the
                               center's one                                +*/
    itv_t   *itv,      /*+ In: basis file                                  +*/
    window_t  *win,    /*+ In: window                                      +*/
    int     width,     /*+ In: number of columns of the image              +*/
    int     npixels    /*+ In: number of pixels                            +*/
)
/*+ Return: 1 on success 0 on failure                                      +*/
{

/* author: Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)                  */
/* date: Tue Aug 17 1999                                                    */

  int label ;
  int i, j, k , s, nshifts;
  int wsize , wzip ;
  int *offset ;        /* pointer to an array of offsets */
  unsigned int *wpat ; /* pointer to an array of configurations */


#ifdef _DEBUG_
pac_debug("Entering papplic_short()") ;
#endif

  wsize = win_get_band_wsize(win, 1) ;
  wzip = size_of_zpat(wsize) ;

  /* set vector of offsets */
  offset = (int *)malloc(sizeof(int)*wsize) ;
  if(offset==NULL) {
    return pac_error(MSG, "papplic_short: offset_create() failed.") ;
  }

  offset_set(offset, win, width, 1) ;

#ifdef _DEBUG_
pac_debug("It called offset_set, cv = %d", cv) ;
for(i=0; i<wsize; i++) {
  pac_debug("offset[%d]=%d\n", i, offset[i]) ;
}
#endif


  wpat = (unsigned int *)malloc(sizeof(int)*size_of_zpat(wsize)) ;
  if(wpat == NULL) {
    return pac_error(1,"papplic_short: Memory allocation failed.") ;
  }


  /* Begin case A : cv=0 (All pixels whose value is different from
           0 are considered in the w-pattern) */

  if(!cv) {

    for(i = 0; i < npixels; i++) {         /* shifts the window */

      if(ucpixels1[i] != 0) {              /* mask condition satisfied */

	for(j = 0; j < wzip; j++) wpat[j] = 0 ;     /* blank w-pattern */

        /* gets the w-pattern centered at the point j */
	for(j = 0; j < wsize; j++) {  /* for each point of the window...*/

	  k = i + offset[j] ;
	  if(uspixels[k] != 0) {
	    s = j/NB ;
	    nshifts = j%NB ;
	    wpat[s] = (wpat[s] | bitmsk[nshifts]) ;
	  }

	}

        /* checks if the configuration is not in the intervals */

	label = itv_list_contain(itv, wpat, wzip) ;

        /* if found, put the gray-level to the output image */

	uspixels2[i] = (unsigned short)label ;

      }

    }

  }

  /* Begin case B : cv=1 (Only the pixels whose value is the same as the
     pixel under the center of the window are considered in the w-pattern */
  else {

    for(i = 0; i < npixels; i++) {         /* shifts the window */

      if(ucpixels1[i] != 0) {              /* mask condition satisfied */

	for(j = 0; j < wzip; j++) wpat[j] = 0 ;     /* blank w-pattern */

        /* gets the w-pattern centered at the point j */
	for(j = 0; j < wsize; j++) {  /* for each point of the window...*/

	  k = i + offset[j] ;
          if((uspixels[k] != 0) && ((uspixels[k]==uspixels[i]) || !uspixels[i]))
	    {
              s = j/NB ;
              nshifts = j%NB ;
              wpat[s] = (wpat[s] | bitmsk[nshifts]) ;
          }

        }

        /* checks if the configuration is not in the intervals */

        label = itv_list_contain(itv, wpat, wzip) ;

        /* if found, put the gray-level to the output image */

        uspixels2[i] = (unsigned short)label ;

      }

    }

  }

  free(offset) ;
  free(wpat) ;

  return(1) ;

}
