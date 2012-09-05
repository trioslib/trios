#include "trios.h"

xpl_t *            /*+ Purpose: builds an example's set from a pair of input-output
        images that represents a gray-scale-to-gray-scale transformation with gray-
        level restriction. The examples collecting process can be gathered by a
        mask image. These three images are considered gray-scale ones.           +*/

  collec_WKF(

    unsigned char  *p1,   /*+ In: input image data                              +*/
    unsigned char  *p2,   /*+ Out: output image data                            +*/
    unsigned char  *p3,   /*+ In: mask image data                               +*/
    int   *offset,      /*+ In: vector of offsets (contains window information) +*/
    int   wsize,            /*+ In: size of vector of offsets                   +*/
    int   npixels,          /*+ In: number of pixels in the image               +*/
    int   ki,               /*+ In: the input graylevel range                   +*/
    int   ko,               /*+ In: the output graylevel range                  +*/
    int   vplace            /*+ In: vertical placement of the window            +*/
);

xpl_t *            /*+ Purpose: builds an example's set from a pair of input-output
        images that represents a gray-scale-to-gray-scale transformation with gray-
        level restriction for classification. The examples collecting process can be
        gathered by a mask image. These three images are considered gray-scale ones.  +*/

  collec_WKC(

    unsigned char  *p1,     /*+ In: input image data                                  +*/
    unsigned char  *p2,     /*+ Out: output image data                                +*/
    unsigned char  *p3,     /*+ In: mask image data                                   +*/
    int   *offset,          /*+ In: vector of offsets (contains window information)   +*/
    int   wsize,            /*+ In: size of vector of offsets                         +*/
    int   npixels,          /*+ In: number of pixels in the image                     +*/
    int   ki,               /*+ In: the input graylevel range                         +*/
    int   vplace            /*+ In: vertical placement of the window                  +*/
);



int				/*+ Purpose: collect examples from a set of images         + */
 lcollecWK(char *fname_i1,	/*+ In: IMGSET file                                        + */
	   char *fname_i2,	/*+ In: WINSPEC file                                       + */
	   char *fname_i3,	/*+ In: optional input XPL file. It must be NULL if not
				   used                                                   + */
	   int ki,		/*+ In: input range                                        + */
	   int ko,		/*+ In: output range (claasifier if 0; filter if !=0)      + */
	   int vplace,		/*+ In: specifies the window vertical position             + */
	   char *o1_fname,	/*+ Out: output XPL file                                   + */
	   char *o2_fname	/*+ Out: optional output report file. It must be NULL if
				   not used                                               + */
    )
/*+ Return: 1 on success, 0 on failure                                          +*/
{
/* author: Nina S. Tomita and R. Hirata Jr. (nina@ime.usp.br)               */
/* date: Mon Oct 21 1996                                                    */

/*  Modification by:  Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)       */
/*  Date: Thu Nov  6 1997                                                   */
/*  Mod: Introduced the option to limit the range of gray-level. This       */
/*       option is used to learn the WK-operators                           */

/*  Modification by:  Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)       */
/*  Date: Tue Jan 13 1998                                                   */
/*  Mod: Modified to treat the case WKGG                                    */

/*  Modification by:  Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)       */
/*  Date: Wed Jun  3 1998                                                   */
/*  Mod:  Included the vplace variable                                      */

/*  Modification by:  Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)       */
/*  Date: Tue Jul 27 1999                                                   */
/*  Mod:  Included the option filter to choose between filter and           */
/*  Mod:  classifier                                                        */

/*  Modification by:  Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)       */
/*  Date: Fri Oct 29 1999                                                   */
/*  Mod: Include support to different input and output ranges               */

/*  Modification by:  Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)       */
/*  Date: Wed Feb  2 2000                                                   */
/*  Mod: Improved the treatment of errors                                   */

	unsigned char *c1, *c2, *c3;

	int k;
	int type;
	int *offset;
	int width, wsize, npixels;

	imgset_t *imgset = NULL;

	window_t *win = NULL;

	apert_t *apt = NULL;

	xpl_t *xpl = NULL, *xpl_new = NULL;

	img_t *img1 = NULL, *img2 = NULL, *img3 = NULL;


#ifdef _DEBUG_
    trios_debug("Starting lcollec %s e %s", fname_i1, o1_fname);
#endif

	/* read image set file */
	if ((imgset = imgset_read(fname_i1)) == NULL) {
        return trios_error(MSG, "lcollec: imgset_read() failed.");
	}

	/* check if image group size is greater than or equal to 2 */
	/* Only the first three images of each set will be used    */
	/*  Fisrt image is the image before transformation         */
	/*  Second image is the image after transformation         */
	/*  Third image is the mask image (indicates which points  */
	/*   of the first image should be scanned)                 */
	/*  If third image is absent, then the whole first image   */
	/*  will be scanned.                                       */
	if (imgset_get_grpsize(imgset) < 2) {
		imgset_free(imgset);
        return trios_error(1,
				 "Each image group must contain at leat two image files.");
	}

	/* read WINSPEC file */
	if ((win = win_read(fname_i2)) == NULL) {
		imgset_free(imgset);
        return trios_error(MSG, "lcollec: win_read() failed.");
	}

	/* If window has multiple bands, force it to ONE band only */
	/* Users will be warned about this fact                    */
	if (win_get_nbands(win) > 1) {
        trios_warning("Window has %d bands.", win_get_nbands(win));
        trios_warning("Only the first one will be used.");
		wsize = win_get_band_wsize(win, 1);
		win_set_nbands(win, 1);
		win_set_wsize(win, wsize);
	}
#ifdef _DEBUG_
    trios_debug("Window reading : done.");
#endif

	/* create aperture */
	if (!(apt = apert_create(1))) {
		imgset_free(imgset);
        return trios_error(MSG, "lcollec: apert_create() failed.");
	}

	apert_set_band_ki(apt, 1, ki);
	apert_set_band_ko(apt, 1, ko);
	apert_set_band_vplace(apt, 1, vplace);

	if (ko == 0)
		type = WKC;
	else
		type = WKF;


	/* allocate vector of offsets */
	wsize = win_get_band_wsize(win, 1);

	offset = (int *) malloc(sizeof(int) * wsize);
	if (offset == NULL) {
		imgset_free(imgset);
		win_free(win);
        return trios_error(MSG, "lcollec: offset_create() failed.");
	}


	/* if input example collection is specified then read it;
	   otherwise create an empty example collection structure */

	if (fname_i3 != NULL) {
		if (!(xpl = xpl_read(fname_i3, &win, &apt))) {
			imgset_free(imgset);
			win_free(win);
            return trios_error(MSG, "lcollec: xpl_read() failed.");
		}
	} else {
		xpl = xpl_create(wsize, type);
	}

	/* ---------------------------------------------------------------- */
	/* Begin : Process each group of images */
	for (k = 1; k <= imgset_get_ngroups(imgset); k++) {

		img1 = img2 = img3 = NULL;

		/* read images and convert them to the appropriate format */
		/* i.e, input is short, output is byte and mask is byte */
		if (!get_setofimages(imgset, GG, win, k, &img1, &img2, &img3)) {
            trios_error(MSG, "lcollec: get_images() failed.");
			goto END_lcollec;
		}

        width = img_get_width(img1);
        npixels = img_get_height(img1) * width;

		/* set vector of offsets */
		offset_set(offset, win, width, 1);


        c1 = (unsigned char *) img_get_data(img1);
        c2 = (unsigned char *) img_get_data(img2);
        c3 = (unsigned char *) img_get_data(img3);

		/* -------------------------------------------------------------- */
		/*                            WK - Filter                         */
		/* -------------------------------------------------------------- */
		if (type == WKF) {
			xpl_new = collec_WKF(c1, c2, c3, offset, wsize,
					     npixels, ki, ko, vplace);
			if (xpl_new == NULL) {
                trios_error(MSG, "lcollec: collec_WKF() failed.");
				goto END_lcollec;
			}
		}

		/* -------------------------------------------------------------- */
		/*                            WK - Classifier                     */
		/* -------------------------------------------------------------- */
		else if (type == WKC) {
            xpl_new =
			    collec_WKC(c1, c2, c3, offset, wsize, npixels, ki,
                       vplace);
			if (xpl_new == NULL) {
                trios_error(MSG, "lcollec: collec_WKC() failed.");
				goto END_lcollec;
			}
		}

		/* Release memory used by the images */
        img_free(img1);
		img1 = NULL;
        img_free(img2);
		img2 = NULL;
        img_free(img3);
		img3 = NULL;

		/* merge xpl_new into xpl */
		if (!xpl_merge(xpl, xpl_new)) {
            trios_error(MSG, "lcollec: xpl_merge() failed.");
			goto END_lcollec;
		}
		xpl_new = NULL;

	}



	/* --------------------------------------------------------------------- */

#ifdef _DEBUG_
    trios_debug("Before write");
#endif
	/* write contents of XPL to output file */
	if (!xpl_write(o1_fname, xpl, win, apt)) {
        trios_error(MSG, "lcollec : xpl_write() failed.");
		goto END_lcollec;
	}
#ifdef _DEBUG_
    trios_debug("Write done");
#endif



	/* free allocated memory */
	imgset_free(imgset);
	win_free(win);
	free(offset);
	xpl_free(xpl);
	apert_free(apt);

	return (1);

      END_lcollec:
	if (img1)
        img_free(img1);
	if (img2)
        img_free(img2);
	if (img3)
        img_free(img3);
	if (imgset)
		imgset_free(imgset);
	if (offset)
		free(offset);
	if (xpl)
		xpl_free(xpl);
	if (xpl_new)
		xpl_free(xpl_new);
	if (win)
		win_free(win);
	if (apt)
		apert_free(apt);
	return (0);

}

/*
     -------------------------------------------
     FUNCTION: collec_WKF
     -------------------------------------------
*/

xpl_t *            /*+ Purpose: builds an example's set from a pair of input-output
        images that represents a gray-scale-to-gray-scale transformation with gray-
        level restriction. The examples collecting process can be gathered by a
        mask image. These three images are considered gray-scale ones.           +*/

  collec_WKF(

    unsigned char  *p1,   /*+ In: input image data                              +*/
    unsigned char  *p2,   /*+ Out: output image data                            +*/
    unsigned char  *p3,   /*+ In: mask image data                               +*/
    int   *offset,      /*+ In: vector of offsets (contains window information) +*/
    int   wsize,            /*+ In: size of vector of offsets                   +*/
    int   npixels,          /*+ In: number of pixels in the image               +*/
    int   ki,               /*+ In: the input graylevel range                   +*/
    int   ko,               /*+ In: the output graylevel range                  +*/
    int   vplace            /*+ In: vertical placement of the window            +*/
)
                      /*+ Returns: an example's set on success, NULL on failure +*/
{
/*  author: Nina S. Tomita and R. Hirata Jr. (nina@ime.usp.br)  */
/*  date Mon Apr 14 1997                                        */

/*  Modification by:  Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)       */
/*  Date: Wed Jun  3 1998                                                   */
/*  Mod:  Modified to include the vplace variable                           */

/*  Modification by:  Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)       */
/*  Date: Fri Oct 29 1999                                                   */
/*  Mod:  Included code to treat input and output wk-ranges                 */

/*  Modification by:  Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)       */
/*  Date: Fri Feb 18 2000                                                   */
/*  Mod: Adjusted to graylevel ranges instead of amplitudes                 */

  char  *wpat ;               /* w-pattern */

  unsigned char *wpataux ;

  int   i, j, k ;
  int   wcenter, aux ;
  int   wpattmp ;
  int   wklabel ;
  int   st ;

  xpl_t *xpl ;                /* XPL structure */

  freq_node *freqnode ;       /* pointer to a frequency node */

#ifdef _DEBUG_
trios_debug("Entrei no collec_WKF") ;
#endif
#ifdef _DEBUG_1_
for(i=0;i<wsize;i++) {
trios_debug("offset[%d]=%d\n", i, offset[i]) ;
}
#endif

  wcenter = 0 ;

  wpat = (char *)malloc(sizeof(char)*wsize) ;
  if(wpat == NULL) {
    return (xpl_t *)trios_error(1,"Memory allocation failed.") ;
  }

  wpataux = (unsigned char *)malloc(sizeof(char)*wsize) ;
  if(wpataux == NULL) {
    return (xpl_t *)trios_error(1,"Memory allocation failed.") ;
  }

  if((xpl=xpl_create(wsize, WKF))==NULL) {
    free(wpat) ;
    return (xpl_t *)trios_error(MSG, "collec_GG: xpl_create() failed.") ;
  }

  /* shifts the window over all points of the image */

  for(j = 0; j<npixels; j++) {

    if(p3[j] != 0) {              /* mask condition satisfied */

      for(i=0; i<xpl->wsize; i++) {
    wpat[i] = 0 ;     /* blank w-pattern */
    wpataux[i] = 0 ;
      }

      /* gets the w-pattern centered at the point j */

      if (vplace == VP_Center) wcenter = p1[j] ;

      else if (vplace == VP_Median) {

    for(i=0; i<wsize; i++) {  /* for each point of the window...*/

      k = j+offset[i] ;
      aux = i ;

      while ((p1[k] < wpataux[aux-1])&&(aux>=0)) {

        wpataux[aux] = wpataux[aux-1] ;
        aux = aux - 1 ;

      }
      wpataux[aux] = p1[k] ;
    }

    if (wsize%2) {
      wcenter = wpataux[wsize/2] ;
    }
    else {
      wcenter = (wpataux[wsize/2] + wpataux[(wsize/2 - 1)])/2 ;
    }


#ifdef _DEBUG_2_
    trios_debug("Pattern coletado para achar a mediana");
    for(i=0;i<wsize;i++) {
      trios_debug("wpataux[%d]=%d\n", i, wpataux[i]) ;
    }
    trios_debug("p2[%d]=%d\n", wcenter, p2[j]) ;
#endif

      }

      for(i=0; i<wsize; i++) {  /* for each point of the window...*/

    k = j+offset[i] ;

    wpat[i] = wpattmp = p1[k] - wcenter ;

    if (wpattmp > ki) {
      wpat[i] =  ki ;
    }
    else if (wpattmp < -ki) {
      wpat[i] = - ki ;
    }
      }

      wklabel = p2[j] - wcenter ;

      if (wklabel > ko) {
    wklabel =  ko ;
      }
      else if (wklabel < -ko) {
    wklabel =  -ko ;
      }

#ifdef _DEBUG_2_
trios_debug("Pattern coletado");
for(i=0;i<wsize;i++) {
trios_debug("wpat[%d]=%d\n", i, wpat[i]) ;
}
trios_debug("p2[%d]=%d\n", j, wklabel) ;
#endif

      if((freqnode=freq_node_create(wklabel, 1))==NULL) {
    free(wpat) ;
    return (xpl_t *)trios_error(MSG, "collec_GG: freq_node_create() failed.") ;
      }

      /* insert new w-pattern into example's set */

#ifdef _DEBUG_2_
trios_debug("freqnode criado");
trios_debug("label1=%d , freq1=%d\n", freqnode->label, freqnode->freq) ;
#endif

      st = xpl_GG_insert(xpl, (xpl_GG **)(&xpl->root), wpat, freqnode) ;

      if(st == -1) {
    xpl_free(xpl) ;
    free(wpat) ;
    return (xpl_t *)trios_error(MSG, "collec_GG: xpl_GG_insert() failed.") ;
      }

    }

  }

  free(wpat) ;
  return(xpl) ;

}


xpl_t *            /*+ Purpose: builds an example's set from a pair of input-output
        images that represents a gray-scale-to-gray-scale transformation with gray-
        level restriction for classification. The examples collecting process can be
        gathered by a mask image. These three images are considered gray-scale ones.  +*/

  collec_WKC(

    unsigned char  *p1,     /*+ In: input image data                                  +*/
    unsigned char  *p2,     /*+ Out: output image data                                +*/
    unsigned char  *p3,     /*+ In: mask image data                                   +*/
    int   *offset,          /*+ In: vector of offsets (contains window information)   +*/
    int   wsize,            /*+ In: size of vector of offsets                         +*/
    int   npixels,          /*+ In: number of pixels in the image                     +*/
    int   ki,               /*+ In: the input graylevel range                         +*/
    int   vplace            /*+ In: vertical placement of the window                  +*/
)
                            /*+ Returns: an example's set on success, NULL on failure +*/
{
/*  author: Nina S. Tomita and R. Hirata Jr. (nina@ime.usp.br)  */
/*  date: Mon Jul 26 1999                                       */

  char  *wpat ;               /* w-pattern */

  unsigned char *wpataux ;

  int   i, j, k ;
  int   wcenter, aux ;
  int   wpattmp ;
  int   st ;

  xpl_t *xpl ;                /* XPL structure */

  freq_node *freqnode ;       /* pointer to a frequency node */


#ifdef _DEBUG_
trios_debug("Entrei no collec_WKC") ;
#endif
#ifdef _DEBUG_2_
for(i=0;i<wsize;i++) {
trios_debug("offset[%d]=%d\n", i, offset[i]) ;
}
#endif

  wcenter = 0 ;

  wpat = (char *)malloc(sizeof(char)*wsize) ;
  if(wpat == NULL) {
    return (xpl_t *)trios_error(1,"Memory allocation failed.") ;
  }

  wpataux = (unsigned char *)malloc(sizeof(char)*wsize) ;
  if(wpataux == NULL) {
    return (xpl_t *)trios_error(1,"Memory allocation failed.") ;
  }

  if((xpl=xpl_create(wsize, WKC))==NULL) {
    free(wpat) ;
    return (xpl_t *)trios_error(MSG, "collec_GG: xpl_create() failed.") ;
  }

  /* shifts the window over all points of the image */

  for(j = 0; j<npixels; j++) {

    if(p3[j] != 0) {              /* mask condition satisfied */

      for(i=0; i<xpl->wsize; i++) {
    wpat[i] = 0 ;     /* blank w-pattern */
    wpataux[i] = 0 ;
      }

      /* gets the w-pattern centered at the point j */

      if (vplace == VP_Center) wcenter = p1[j] ;

      else if (vplace == VP_Median) {

    for(i=0; i<wsize; i++) {  /* for each point of the window...*/

      k = j+offset[i] ;
      aux = i ;

      while ((p1[k] < wpataux[aux-1])&&(aux>=0)) {

        wpataux[aux] = wpataux[aux-1] ;

        aux = aux - 1 ;
      }
      wpataux[aux] = p1[k] ;
    }

    if (wsize%2) {
      wcenter = wpataux[wsize/2] ;
    }
    else {
      wcenter = (wpataux[wsize/2] + wpataux[(wsize/2 - 1)])/2 ;
    }


#ifdef _DEBUG_2_
    trios_debug("Pattern coletado para achar a mediana");
    for(i=0;i<wsize;i++) {
      trios_debug("wpataux[%d]=%d\n", i, wpataux[i]) ;
    }
    trios_debug("p2[%d]=%d\n", wcenter, p2[j]) ;
#endif

      }

      for(i=0; i<wsize; i++) {  /* for each point of the window...*/

    k = j+offset[i] ;

    wpat[i] = wpattmp = p1[k] - wcenter ;

    if (wpattmp > ki) {
      wpat[i] =  ki ;
    }
    else if (wpattmp < -ki) {
      wpat[i] = -ki ;
    }
      }

#ifdef _DEBUG_2_
trios_debug("Pattern coletado");
for(i=0;i<wsize;i++) {
trios_debug("wpat[%d]=%d\n", i, wpat[i]) ;
}
#endif

      if((freqnode=freq_node_create((int)p2[j], 1))==NULL) {
    free(wpat) ;
    return (xpl_t *)trios_error(MSG, "collec_GG: freq_node_create() failed.") ;
      }

      /* insert new w-pattern into example's set */

#ifdef _DEBUG_2_
trios_debug("freqnode criado");
trios_debug("label1=%d , freq1=%d\n", freqnode->label, freqnode->freq) ;
#endif

      st = xpl_GG_insert(xpl, (xpl_GG **)(&xpl->root), wpat, freqnode) ;

      if(st == -1) {
    xpl_free(xpl) ;
    free(wpat) ;
    return (xpl_t *)trios_error(MSG, "collec_GG: xpl_GG_insert() failed.") ;
      }

    }

  }

  free(wpat) ;
  return(xpl) ;

}
