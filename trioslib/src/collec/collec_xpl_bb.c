#include <trios.h>
#include <stdlib.h>

/**
    Builds an example's set from a pair of input-output
    images that represents a binary-to-binary transformation. The examples
    collecting process can be gathered by a mask image.
    These three images are considered binary ones, i.e, all pixels which
    value is not ZERO are treated as a pixel with value equal to ONE.

    \param s1 Input image data (short-valued image)
    \param p2 Output image data (byte-valued image)
    \param p3 Mask image data (byte-valued image)
    \param offset Vector of offsets (with window information)
    \param wsize Size of vector of offsets.
    \param npixels Number of pixels in the image
    \param cv Center-value flag.
    \return An example's set on success, NULL on failure.
*/
xpl_t *collec_BB(unsigned short *s1, unsigned char *p2, unsigned char *p3, int *offset, int wsize, int npixels, int cv) {
    int   i, j, k, s ;
    int   nshifts ;
    unsigned int *wpat ;      /* w-pattern */
    int   st ;
    xpl_t *xpl ;                /* XPL structure */

    #ifdef _DEBUG_
        pac_debug("Entrei no collec_BB") ;
    #endif
    #ifdef _DEBUG_1_
        for(i=0;i<wsize;i++) {
            pac_debug("offset[%d]=%d\n", i, offset[i]) ;
        }
    #endif
    wpat = (unsigned int *)malloc(sizeof(int)*size_of_zpat(wsize)) ;
    if(wpat == NULL) {
        return (xpl_t *)trios_error(1,"Memory allocation failed.") ;
    }
    if((xpl=xpl_create(wsize, BB))==NULL) {
        free(wpat) ;
        return (xpl_t *)trios_error(MSG, "collec_BB: xpl_create() failed.") ;
    }
    /* Begin case A : cv=0 (All pixels with value different from
    0 are considered in the w-pattern) */
    /* TODO: extract this case into a separate function */
    if(!cv) {
        #ifdef _DEBUG_
            pac_debug("Vai entrar no caso cv_flag=0") ;
        #endif
        for(j = 0; j<npixels; j++) {         /* shifts the window */
            #ifdef _DEBUG_2_
                pac_debug("Iterating...") ;
            #endif
            if(p3[j] != 0)  {
                /* mask condition satisfied */
                #ifdef _DEBUG_2_
                    pac_debug("mask condition satisfied") ;
                #endif
                for(s=0; s<xpl->wzip; s++) wpat[s] = 0 ;     /* blank w-pattern */
                #ifdef _DEBUG_2_
                pac_debug("w-pattern cleaned!") ;
                #endif
                /* gets the w-pattern centered at the point j */
                for(i=0; i<wsize; i++) {  /* for each point of the window...*/
                    k = j+offset[i] ;
                    if(k >= 0 && k < npixels && s1[k] != 0) { // TODO: see if changes here impact something. It shouldn't, but who knows.
                        s = i/NB ;
                        nshifts = i%NB ;
                        wpat[s] = (wpat[s] | bitmsk[nshifts]) ;
                    }
                }
                #ifdef _DEBUG_2_
                    pac_debug("trying to insert w-pattern into example's set...") ;
                #endif
                /* insert new w-pattern into example's set */
                if(p2[j] == 0) {
                    st = xpl_BB_insert(xpl, (xpl_BB **)(&xpl->root), wpat, 1, 0) ;
                } else {
                    st = xpl_BB_insert(xpl, (xpl_BB **)(&xpl->root), wpat, 0, 1) ;
                }
                if(st == -1) {
                    xpl_free(xpl) ;
                    free(wpat) ;
                    return (xpl_t *)trios_error(MSG, "collec_BB: xpl_BB_insert() failed.") ;
                }
            }
        }
     /* end case A */
    /* Begin case B : cv=1 (Only pixels with same value as the value of the
    pixel under the center of the window are considered in the w-pattern */
    /* TODO: Split this case into a separate function */
    } else {
        #ifdef _DEBUG_
            pac_debug("Vai entrar no caso cv_flag=1") ;
        #endif
        for(j = 0; j<npixels; j++) {               /* shifts the window */
            if(p3[j] != 0) {                    /* mask condition satisfied */
                for(s=0; s<xpl->wzip; s++) wpat[s] = 0 ;      /* blank wpat */
                /* if current pixel j has value=0, then pattern is the zero vector */
                if(s1[j]) {
                    /*  get the w-pattern centered at the point j */
                    for(i=0; i<wsize; i++) {      /* for each point of the window ... */
                        k = j+offset[i] ;
                        /* Only pixels with the same value as of the pixel j    */
                        /* will be turned on in the w-pattern                   */
                        if(k >= 0 && k < npixels && s1[k]==s1[j]) {
                            s = i/NB ;
                            nshifts = i%NB ;
                            wpat[s] = (wpat[s] | bitmsk[nshifts]) ;
                        }
                    }
                }
                /* insert new w-pattern into example's set */
                if(p2[j] == 0) {
                    st = xpl_BB_insert(xpl, (xpl_BB **)(&xpl->root), wpat, 1, 0) ;
                }
                else {
                    st = xpl_BB_insert(xpl, (xpl_BB **)(&xpl->root), wpat, 0, 1) ;
                }
                if(st == -1) {
                    free(wpat) ;
                    xpl_free(xpl) ;
                    return (xpl_t *)trios_error(MSG, "collec_BB: xpl_BB_insert() failed.") ;
                }
            }
        }
    } /* end case B */
    free(wpat);
    return xpl;
}

