#include <trios.h>
#include <stdlib.h>
#include "collec_private.h"

xpl_t *collec_BB(unsigned short *s1, unsigned char *p2, unsigned char *p3, int *offset, int wsize, int npixels, int cv);
xpl_t *collec_GG(unsigned char *p1,	unsigned char *p2, unsigned char *p3, int *offset, int wsize, int npixels);

void put_border(window_t *win, img_t *img3)
{
    int top, left, bottom, right, i, j, w, h, img_w, img_h;
    w = win_get_width(win);
    h = win_get_height(win);
    top = h/2;
    left = w/2;
    bottom = (h % 2 == 1)? h/2:h - h/2;
    right = (w % 2 == 1)? w/2:w - w/2;

    img_w = img_get_width(img3);
    img_h = img_get_height(img3);

    for (i = 0; i < top; i++) {
        for (j = 0; j < img_w; j++) {
            img_set_pixel(img3, i, j, 0, 0);
        }
    }

    for (i = 0; i < bottom; i++) {
        for (j = 0; j < img_w; j++) {
            img_set_pixel(img3, img_h - i - 1, j, 0, 0);
        }
    }

    for (i = 0; i < img_h; i++) {
        for (j = 0; j < left; j++) {
            img_set_pixel(img3, i, j, 0, 0);
        }
    }

    for (i = 0; i < img_h; i++) {
        for (j = 0; j < right; j++) {
            img_set_pixel(img3, i, img_w - j - 1, 0, 0);
        }
    }
}

int get_setofimages(imgset_t *imgset, int map_type, window_t *win, int k, img_t **img1, img_t **img2, img_t **img3) {
    /* Read first image */
    int top, bottom, left, right;
    int i, j;
    char *filename = imgset_get_ffullname(imgset, 1, k);
    img_t *img = img_readPGM(filename);
    free(filename);
    if (img == NULL) {
        return 0;
    }
    if (map_type == BB) {
        *img1 = img_convert_type(img, sz16BIT);
        img_free(img);
    } else {
        *img1 = img;
    }

    /* Read second image */
    filename = imgset_get_ffullname(imgset, 2, k);
    img = img_readPGM(filename);
    free(filename);
    if (img == NULL) {
        return 0;
    }
    *img2 = img;
    /* If there is a mask, read it */
    if (imgset_get_grpsize(imgset) >= 3) {
        filename = imgset_get_ffullname(imgset, 3, k);
        img = img_readPGM(filename);
        free(filename);
        if (img == NULL) {
            return 0;
        }
        *img3 = img;
    } else {
        *img3 = img_create(img_get_width(*img1), img_get_height(*img1), 1, sz8BIT);
        for (i = 0; i < img_get_height(*img1); i++) {
            for (j = 0; j < img_get_width(*img1); j++) {
                img_set_pixel(*img3, i, j, 0, 1);
            }
        }
    }
    put_border(win, *img3);
    return 1;
}


/**
    Collects examples from a set of images.

    \param fname_i1 IMGSET file.
    \param fname_i2 WINSPEC file.
    \param fname_i3 Optional input XPL file. It must be NULL if not used.
    \param input_type Flag to indicate if the input images are binary. YES=1, NO=0.
    \param output_type Flag to indicate if the output images are binary. YES=1, NO=0.
    \param cv_flag Flag to indicate if all pixels that form a w-pattern must have the same value as the pixel under the central point of the window. This is useful to collect w-patterns ignoring neighboring objects.
    \param fname_o1 Output XPL file.
    \param fname_o2 Optional output report file. It must be NULL if not given.
    \return 1 on success. 0 on failure.

*/  
int lcollec(char *fname_i1, char *fname_i2, char *fname_i3, int  input_type, int  output_type, int  cv_flag, char *fname_o1, char *fname_o2) {
    imgset_t *imgset = NULL ;
    window_t *win  = NULL ;
    apert_t  *apt;
    xpl_t    *xpl  = NULL ;
    int      wsize=0, map_type ;
    #ifdef _DEBUG_
        trios_debug("Starting lcollec %s e %s e cv=%d", fname_i1, fname_o1, cv_flag) ;
    #endif
    /* read image set file */
    if ((imgset = imgset_read(fname_i1))==NULL) {
        return trios_error(MSG, "lcollec: imgset_read() failed.") ;
    }
    /* check if image group size is greater than or equal to 2 */
    /* Only the first three images of each set will be used    */
    /* Fisrt image is the image before transformation          */
    /* Second image is the image after transformation          */
    /* Third image is the mask image (indicates which points   */
    /*   of the first image should be scanned)                 */
    /* If third image is absent, then the whole first image    */
    /* will be scanned.                                        */
    if(imgset_get_grpsize(imgset)<2) {
        imgset_free(imgset) ;
        return trios_error(1, "Each image group must contain at least two image files.") ;
    }
    /* read WINSPEC file */
    if ((win = win_read(fname_i2))==NULL) {
        imgset_free(imgset) ;
        return trios_error(MSG, "lcollec: win_read() failed.") ;
    }
    #ifdef _DEBUG_
        trios_debug("Window reading : done.") ;
    #endif
    wsize = win_get_band_wsize(win, 1) ;
    /* If window has multiple bands, force it to ONE band only */
    /* Users will be warned about this fact                    */
    if(win_get_nbands(win) > 1) {
        trios_warning("Window has %d bands.", win_get_nbands(win)) ;
        trios_warning("Only the first one will be used.") ;
        win_set_nbands(win, 1) ;
        win_set_wsize(win, wsize) ;
    }
    /* set type BB, BG, GB or GG */
    if(input_type==1) {
        if(output_type==1) map_type = BB ;
        else map_type = BG ;
    } else {
        if(output_type==1) map_type = GB ;
        else map_type = GG ;
    }
    #ifdef _DEBUG_
    trios_debug("Map_type=%d", map_type) ;
    #endif
    /* if an input XPL file is given, then read it;
    otherwise create an empty XPL structure */ 
    if(fname_i3 != NULL) {
        if(!(xpl=xpl_read(fname_i3, &win, &apt))) {
            imgset_free(imgset) ;
            win_free(win) ;
            return trios_error(MSG, "lcollec: xpl_read() failed.") ;
        }
    } else {
        if(!(xpl = xpl_create(wsize, map_type))) {
            imgset_free(imgset) ;
            win_free(win) ;
            return trios_error(MSG, "lcollec: xpl_create() failed.") ;
        }
    }
    /* MAIN part of this routine: collection of examples */
    if(!lcollec_main(imgset, win, xpl, map_type, cv_flag, fname_o2)) {
        imgset_free(imgset) ;
        win_free(win) ;
        return trios_error(MSG, "lcollec: lcollec_main() failed.") ;    
    }
    #ifdef _DEBUG_
        trios_debug("lcollec_main: done.") ;
    #endif
    /* write contents of XPL to output file */
    if(!xpl_write(fname_o1, xpl, win, apt)) {
        imgset_free(imgset) ;
        win_free(win) ;
        xpl_free(xpl) ;
        return trios_error(MSG, "lcollec : xpl_write() failed.") ;
    }
    #ifdef _DEBUG_
        trios_debug("Writing XPL: done") ;
    #endif
    /* free allocated memory */
    imgset_free(imgset) ;
    win_free(win) ;
    xpl_free(xpl) ;
    return 1;
}


/**
    Collects examples from a set of images.
    
    \param imgset IMGSET containing the pairs to be collected.
    \param win Window of the operator.
    \param xpl A pre allocated xpl structure.
    \param map_type Mapping type. Only Binary-to-Binary(BB) supported.
    \param cv_flag Flag to indicate if all pixels that form a w-pattern must have the same value as the pixel under the central point of the window. This is useful to collect w-patterns ignoring neighboring objects.
    \param log_file Optional name for log file.
    \return 1 on succes. 0 on failure.
*/ 
int lcollec_main(imgset_t *imgset, window_t *win, xpl_t *xpl, int map_type, int cv_flag, char *log_file) {
    xpl_t    *xpl_new  = NULL ;
    int      k ;
    int      *offset ;
    int      width, wsize, npixels ;
    unsigned short    *s1, *s2 ;
    unsigned char     *c1, *c2, *c3 ;
    img_t *img1 = NULL, *img2 = NULL, *img3 = NULL ;
    FILE     *fd = NULL ;

    /* if log file is given, open it */
    if(log_file) {
        fd = fopen(log_file, "w") ;
        if(!fd) {
            return trios_error(1, "File (%s) open failed.", log_file) ;
        }
        fprintf(fd, "Initial (distinct) examples=%d\n", xpl->n_nodes) ;
        fprintf(fd, "Initial (total) examples=%d\n\n", xpl->sum) ;
    }
    /* allocate vector of offsets */
    wsize = win_get_band_wsize(win, 1) ;
    offset = (int *)malloc(sizeof(int)*wsize) ;
    if(offset==NULL) {
        return trios_error(MSG, "lcollec_main: offset_create() failed.") ;
    }
    /* ---------------------------------------------------------------- */
    /* Begin : Process each group of images */
    for(k=1; k<=imgset_get_ngroups(imgset); k++) {
        img1 = img2 = img3 = NULL ;
        /* -------------------------------------------------------------- */
        /*                            BB                                  */
        /* -------------------------------------------------------------- */
        if(map_type==BB) {
            /* read images and convert them to the appropriate format */
            /* i.e, input is short, output is byte and mask is byte */
            if(!get_setofimages(imgset, BB, win, k, &img1, &img2, &img3)) {
                trios_error(MSG, "lcollec_main: get_images() failed.") ;
                goto END_lcollec_main ;
            }
            width = img_get_width(img1);
            npixels = img_get_width(img1) * img_get_height(img1) ;
            /* set vector of offsets */
            offset_set(offset, win, width, 1) ;
            s1 = (unsigned short *)img_get_data(img1) ; /* treat a labeled image as binary */
            c2 = (unsigned char *)img_get_data(img2) ;
            c3 = (unsigned char *)img_get_data(img3) ;
            xpl_new = collec_BB(s1, c2, c3, offset, wsize, npixels, cv_flag) ;
            if(xpl_new==NULL) {
                trios_error(MSG, "lcollec_main: collec_BB() failed.") ;
                goto END_lcollec_main ;
            }
        } else if(map_type==BG) {
            return trios_error(1, "BG Mapping not supported yet.");
        } else if(map_type==GG) {
            /* read images and convert them to the appropriate format */
            /* i.e, input is short, output is byte and mask is byte */
            if(!get_setofimages(imgset, GG, win, k, &img1, &img2, &img3)) {
                trios_error(MSG, "lcollec_main: get_images() failed.") ;
                goto END_lcollec_main ;
            }
            width = img_get_width(img1);
            npixels = img_get_width(img1) * img_get_height(img1) ;
            /* set vector of offsets */
            offset_set(offset, win, width, 1) ;

            c1 = (unsigned char *)img_get_data(img1) ;
            c2 = (unsigned char *)img_get_data(img2) ;
            c3 = (unsigned char *)img_get_data(img3) ;
            xpl_new = collec_GG(c1, c2, c3, offset, wsize, npixels) ;
            if(xpl_new==NULL) {
                trios_error(MSG, "lcollec_main: collec_GG() failed.") ;
                goto END_lcollec_main ;
            }
        } else {
            return trios_error(1, "Case GB not yet implemented.") ;
        }

        /* Release memory used by the images */
        img_free(img1) ; img1 = NULL ;
        img_free(img2) ; img2 = NULL ;
        img_free(img3) ; img3 = NULL ;

        /* merge xpl_new into xpl */
        if(!xpl_merge(xpl, xpl_new)) {
            trios_error(MSG, "lcollec_main: xpl_merge() failed.") ;
            goto END_lcollec_main ;
        }
        xpl_free(xpl_new) ;
        xpl_new = NULL ;

        if(log_file) {
            fprintf(fd, "img=%d  distinct=%d  total=%d\n", k, xpl->n_nodes, xpl->sum) ;
        }
    }
    free(offset) ;
    if(log_file) {
        fclose(fd) ;
    }
    return 1;

END_lcollec_main:
    if (img1!=NULL) img_free(img1) ;
    if (img2!=NULL) img_free(img2) ;
    if (img3!=NULL) img_free(img3) ;
    if (offset!=NULL) free(offset) ;
    if (xpl_new!=NULL) xpl_free(xpl_new) ;
    return 0;
}


