#include <trios.h>

#include <trios_img.h>

/*!
  Reads a mask from a file or creates a new image with all pixels set to 1 if there is no mask.

  \param f_mask Path to the mask.
  \param width Width of the mask.
  \param height Height of the mask.
  \param win Window to use the mask with.
  \return Mask read from file or a new image with all pixels set to 1 if no file was given.
  */
img_t *set_mask(char *f_mask, int width, int height, window_t * win)
{
	if (f_mask == NULL) {
		img_t *mask = img_create(width, height, 1, sz8BIT);
		int i, j;
		for (i = 0; i < height; i++) {
			for (j = 0; j < width; j++) {
				img_set_pixel(mask, i, j, 0, 1);
			}
		}
		return mask;
	} else {
		return img_readPGM(f_mask);
	}
}


img_t *alloc_out_image(itv_t *itv, int width, int height, int type, int on_value, unsigned char **ucpixels2, unsigned short **uspixels2) {
    /* creates output image : SHORT if type==BG and maximum label > 255 */
    img_t *img_out;
    img_out = NULL;
    if (type == BG) {
        if (itv_get_maxlabel(itv) > 255) {
            img_out = img_create(width, height, 1, sz16BIT);
            if (!(img_out)) {
                return trios_error(MSG,
                           "lpapplic: image creation failed.");
            }
            *uspixels2 = (unsigned short *) img_get_data(img_out);
        } else {
            img_out = img_create(width, height, 1, sz8BIT);
            if (!(img_out)) {
                return trios_error(MSG,
                           "lpapplic: mm_createimage() failed.");
            }
            *ucpixels2 = (unsigned char *) img_get_data(img_out);
        }
    } else {
        img_out = img_create(width, height, 1, sz8BIT);
        if (!(img_out)) {
            return trios_error(MSG,
                       "lpapplic: mm_createimage() failed.");
        }
        *ucpixels2 = (unsigned char *) img_get_data(img_out);
        /* Change label of the intervals in the basis by "on_value" */
        if (!itv_label(itv, 1, on_value)) {
            return trios_error(MSG,
                       "lpapplic: itv_label() failed.");
        }

    }
    return img_out;
}

img_t *validate_image_size(img_t *img_appl) {
    img_t *img_tmp;
    if (img_get_pixel_size(img_appl) != sz16BIT) {
        img_tmp = img_convert_type(img_appl, sz16BIT);
        if (!img_tmp) {
            return trios_error(MSG,
                       "lpapplic: type conversion failed failed.");
        }
        img_free(img_appl);
        return img_tmp;
    }
    return img_appl;
}

img_t *lpapplic_memory(img_t * img_appl, itv_t * itv, window_t *win, img_t * img_mask, int cv,
               int hash_flag, int on_value)
{
    int npixels, type, width, height;

    img_t *img_out;		/* Out: resulting output image                        */
    img_t *img_tmp;		/* temporary image */
    unsigned short *uspixels;	/*  Image to be operated                */
    unsigned char *ucpixels1;	/*  Mask image                          */
    unsigned char *ucpixels2;	/*  resulting output image (byte)       */
    unsigned short *uspixels2;	/*  resulting output image (short)      */

    img_appl = validate_image_size(img_appl);
    width = img_get_width(img_appl);
    height = img_get_height(img_appl);

    if (img_mask == NULL) {
        img_mask = set_mask(NULL, width, height, win);
    }

    uspixels = (unsigned short *) img_get_data(img_appl);
    ucpixels1 = (unsigned char *) img_get_data(img_mask);
    npixels = img_get_width(img_appl) * img_get_height(img_appl);
    type = itv_get_type(itv);

    img_out = alloc_out_image(itv, width, height, type, on_value, &ucpixels2, &uspixels2);
    if (!img_out) {
        img_free(img_appl);
        img_free(img_mask);
        return trios_error(MSG, "lpapplic: failed to create output image");
    }

    switch (type) {
    case BB:
    case BG:
        if (itv_get_maxlabel(itv) <= 255) {
            /* if the window size is too large, probably there will be
               few repetitions of the patterns                          */
            if ((hash_flag == 0) || (win_get_wsize(win) > 32)) {
                if (!papplic_bx
                    (uspixels, ucpixels1, ucpixels2, cv, itv,
                     win, width, npixels)) {
                    img_free(img_appl);
                    img_free(img_mask);
                    img_free(img_out);
                    trios_error(MSG,
                            "lpapplic : pacapplic_bx() failed.");
                }
            } else {
                /*if(!hashapplic_bx(uspixels, ucpixels1, ucpixels2, cv,
                   itv, win, width, npixels)) {
                   img_free(img_appl) ;
                   img_free(img_mask) ;
                   img_free(img_out) ;
                   trios_error(MSG, "lpapplic : hashapplic_bx() failed.") ;
                   } */
            }
        } else {
            if ((hash_flag == 0) || (win_get_wsize(win) > 32)) {
                if (!papplic_short
                    (uspixels, ucpixels1, uspixels2, cv, itv,
                     win, width, npixels)) {
                    img_free(img_appl);
                    img_free(img_mask);
                    img_free(img_out);
                    trios_error(MSG,
                            "lpapplic : pacapplic_short() failed.");
                }
            } else {
                /*if(!hashapplic_short(uspixels, ucpixels1, uspixels2, cv,
                   itv, win, width, npixels)) {
                   img_free(img_appl) ;
                   img_free(img_mask) ;
                   img_free(img_out) ;
                   trios_error(MSG, "lpapplic : hashapplic_short() failed.") ;
                   } */
            }
        }
        break;
    default:
        trios_error(1, "Types GG or WK. See routine lapplicDT().");
    }

    return img_out;
}

int lpapplic_disk(char *f_appl, char *f_basis, char *f_mask, int cv,
		  int hash_flag, int on_value, char *f_out)
{
	int width, height, npixels, type;

	img_t *img_appl,	/* In: Image to be operated                           */
	*img_mask,		/* In: Mask image                                     */
	*img_out;		/* Out: resulting output image                        */
	img_t *img_tmp;		/* temporary image */
	unsigned short *uspixels;	/*  Image to be operated                */
	unsigned char *ucpixels1;	/*  Mask image                          */
	unsigned char *ucpixels2;	/*  resulting output image (byte)       */
	unsigned short *uspixels2;	/*  resulting output image (short)      */
	window_t *win;
	/*apert_t  *apt ; */
	itv_t *itv;
	/* read input image file */
	img_appl = img_readPGM(f_appl);
	if (!img_appl) {
		return trios_error(MSG, "lpapplic: img_readPGM() failed.");
	}
	width = img_get_width(img_appl);
	height = img_get_height(img_appl);
    img_appl = validate_image_size(img_appl);
	if ((itv = itv_read(f_basis, &win /*, &apt */ )) == NULL) {
		img_free(img_appl);
		return trios_error(MSG, "lpapplic: itv_read() failed.");
	}
	/* read or create and set mask image border to Zero */
	if (!(img_mask = set_mask(f_mask, width, height, win))) {
		img_free(img_appl);
		return trios_error(MSG, "lpapplic: set_mask() failed.");
	}
	uspixels = (unsigned short *) img_get_data(img_appl);
	ucpixels1 = (unsigned char *) img_get_data(img_mask);
	/* creates output image : SHORT if type==BG and maximum label > 255 */
	npixels = img_get_width(img_appl) * img_get_height(img_appl);
	type = itv_get_type(itv);

    img_out = alloc_out_image(itv, width, height, type, on_value, &ucpixels2, &uspixels2);
    if (!img_out) {
        img_free(img_appl);
        img_free(img_mask);
        return trios_error(MSG, "lpapplic: failed to create output image");
    }

	switch (type) {
	case BB:
	case BG:
		if (itv_get_maxlabel(itv) <= 255) {
			/* if the window size is too large, probably there will be
			   few repetitions of the patterns                          */
			if ((hash_flag == 0) || (win_get_wsize(win) > 32)) {
				if (!papplic_bx
				    (uspixels, ucpixels1, ucpixels2, cv, itv,
				     win, width, npixels)) {
					img_free(img_appl);
					img_free(img_mask);
					img_free(img_out);
					trios_error(MSG,
						    "lpapplic : pacapplic_bx() failed.");
				}
			} else {
				/*if(!hashapplic_bx(uspixels, ucpixels1, ucpixels2, cv,
				   itv, win, width, npixels)) {
				   img_free(img_appl) ;
				   img_free(img_mask) ;
				   img_free(img_out) ;
				   trios_error(MSG, "lpapplic : hashapplic_bx() failed.") ;
				   } */
			}
		} else {
			if ((hash_flag == 0) || (win_get_wsize(win) > 32)) {
				if (!papplic_short
				    (uspixels, ucpixels1, uspixels2, cv, itv,
				     win, width, npixels)) {
					img_free(img_appl);
					img_free(img_mask);
					img_free(img_out);
					trios_error(MSG,
						    "lpapplic : pacapplic_short() failed.");
				}
			} else {
				/*if(!hashapplic_short(uspixels, ucpixels1, uspixels2, cv,
				   itv, win, width, npixels)) {
				   img_free(img_appl) ;
				   img_free(img_mask) ;
				   img_free(img_out) ;
				   trios_error(MSG, "lpapplic : hashapplic_short() failed.") ;
				   } */
			}
		}
		break;
	default:
		trios_error(1, "Types GG or WK. See routine lapplicDT().");
	}
	img_writePGM(f_out, img_out);
	/*if(0) {
	   img_free(img_appl) ;
	   img_free(img_mask) ;
	   img_free(img_out) ;
	   trios_error(MSG, "lpapplic : mm_writeimage() failed.") ;
	   } */

	itv_free(itv);
	win_free(win);

	img_free(img_appl);
	img_free(img_mask);
	img_free(img_out);

	return (1);

}


/*!
    Applies an operator (given by a set of intervals) on a binary image. Produces a sz8BIT image.

    \param uspixels Pixels of the input image.
    \param ucpixels1 Pixels of the mask image.
    \param ucpixels2 Pixels of the output image.
    \param cv On template matching, looks only to pixels under the window with the same label as the center's one.
    \param itv Basis file.
    \param win Window of the operator.
    \param width Width of the input image.
    \param npixels Number of pixels in the image.
    \return 1 on success. 0 on failure.
*/

int /*+ Purpose:                + */ papplic_bx(
						       unsigned short *uspixels,	/*+ In: Image to be operated                + */
						       unsigned char *ucpixels1,	/*+ In: Mask image                          + */
						       unsigned char *ucpixels2,	/*+ Out: resulting output image             + */
						       int cv,	/*+ In: on template matching, looks only to pixels
								   under the window with the same label as the
								   center's one                                + */
						       itv_t * itv,	/*+ In: basis file                                  + */
						       window_t * win,	/*+ In: window                                      + */
						       int width,	/*+ In: number of columns of the image              + */
						       int npixels	/*+ In: number of pixels                            + */
    )
/*+ Return: 1 on success, 0 on failure                                     +*/
{
	int label;
	int i, j, k, s, nshifts;
	int wsize, wzip;
	int *offset;		/* pointer to an array of offsets */
	unsigned int *wpat;	/* pointer to an array of configurations */



#ifdef _DEBUG_
	trios_debug("Entering papplic_bx()");
#endif

	wsize = win_get_band_wsize(win, 1);
	wzip = size_of_zpat(wsize);


	/* set vector of offsets */
	offset = (int *) malloc(sizeof(int) * wsize);
	if (offset == NULL) {
		return trios_error(MSG, "papplic_bx: offset_create() failed.");
	}
#ifdef _DEBUG_
	trios_debug("It will set vector of offsets");
#endif
	trios_debug("%d", width);
	offset_set(offset, win, width, 1);

#ifdef _DEBUG_
	trios_debug("It called offset_set, cv = %d", cv);
	for (i = 0; i < wsize; i++) {
		trios_debug("offset[%d]=%d\n", i, offset[i]);
	}
#endif


	wpat = (unsigned int *) malloc(sizeof(int) * size_of_zpat(wsize));
	if (wpat == NULL) {
		return trios_error(1, "papplic_bx: Memory allocation failed.");
	}


	/* Begin case A : cv=0 (All pixels whose value is different from
	   0 are considered in the w-pattern) */

	if (!cv) {

		for (i = 0; i < npixels; i++) {	/* shifts the window */

			if (ucpixels1[i] != 0) {	/* mask condition satisfied */

				for (j = 0; j < wzip; j++)
					wpat[j] = 0;	/* blank w-pattern */

				/* gets the w-pattern centered at the point j */
				for (j = 0; j < wsize; j++) {	/* for each point of the window... */

					k = i + offset[j];
					if (k >= 0 && k < npixels
					    && uspixels[k] != 0) {
						s = j / NB;
						nshifts = j % NB;
						wpat[s] =
						    (wpat[s] | bitmsk[nshifts]);
					}

				}
				/* checks if the configuration is not in the intervals */
				label = itv_list_contain(itv, wpat, wzip);
				/* if found, put the gray-level to the output image */
				ucpixels2[i] = (unsigned char) label;

			}

		}

	}

	/* Begin case B : cv=1 (Only the pixels whose value is the same as the
	   pixel under the center of the window are considered in the w-pattern */
	else {

		for (i = 0; i < npixels; i++) {	/* shifts the window */

			if (ucpixels1[i] != 0) {	/* mask condition satisfied */

				for (j = 0; j < wzip; j++)
					wpat[j] = 0;	/* blank w-pattern */

				/* if current pixel j has value=0, then pattern is the zero vector */
				if (uspixels[i]) {
					/*  get the w-pattern centered at the point i */
					for (j = 0; j < wsize; j++) {	/* for each point of the window ... */

						k = i + offset[j];
						/* Only pixels with the same value as of the pixel j    */
						/* will be turned on in the w-pattern                   */
						if (k >= 0 && k < npixels
						    && uspixels[k] ==
						    uspixels[i]) {
							s = j / NB;
							nshifts = j % NB;
							wpat[s] =
							    (wpat[s] |
							     bitmsk[nshifts]);
						}

					}

				}

				/* checks whether the configuration is in the intervals */

				label = itv_list_contain(itv, wpat, wzip);

				/* if found, put the gray-level to the output image */

				ucpixels2[i] = (unsigned char) label;

			}

		}

	}

	free(offset);
	free(wpat);

	return (1);

}




/*!
    Applies an operator (given by a set of intervals) on a binary image and produces a gray-scale sz16BIT image.

    \param uspixels Pixels of the input image.
    \param ucpixels1 Pixels of the mask image.
    \param ucpixels2 Pixels of the output image.
    \param cv On template matching, looks only to pixels under the window with the same label as the center's one.
    \param itv Basis file.
    \param win Window of the operator.
    \param width Width of the input image.
    \param npixels Number of pixels in the image.
    \return 1 on success. 0 on failure.
*/

int				/*+ Purpose: Applies an operator (given by a set of intervals)
				   on a binary image and produces a gray-scale SHORT image  + */ papplic_short(
														      unsigned short *uspixels,	/*+ In: Image to be operated                + */
														      unsigned char *ucpixels1,	/*+ In: Mask image                          + */
														      unsigned short *uspixels2,	/*+ Out: resulting output image             + */
														      int cv,	/*+ In: on template matching, looks only to pixels
																   under the window, with the same label as the
																   center's one                                + */
														      itv_t * itv,	/*+ In: basis file                                  + */
														      window_t * win,	/*+ In: window                                      + */
														      int width,	/*+ In: number of columns of the image              + */
														      int npixels	/*+ In: number of pixels                            + */
    )
/*+ Return: 1 on success 0 on failure                                      +*/
{
	int label;
	int i, j, k, s, nshifts;
	int wsize, wzip;
	int *offset;		/* pointer to an array of offsets */
	unsigned int *wpat;	/* pointer to an array of configurations */


#ifdef _DEBUG_
	trios_debug("Entering papplic_short()");
#endif

	wsize = win_get_band_wsize(win, 1);
	wzip = size_of_zpat(wsize);

	/* set vector of offsets */
	offset = (int *) malloc(sizeof(int) * wsize);
	if (offset == NULL) {
		return trios_error(MSG,
				   "papplic_short: offset_create() failed.");
	}

	offset_set(offset, win, width, 1);

#ifdef _DEBUG_
	trios_debug("It called offset_set, cv = %d", cv);
	for (i = 0; i < wsize; i++) {
		trios_debug("offset[%d]=%d\n", i, offset[i]);
	}
#endif


	wpat = (unsigned int *) malloc(sizeof(int) * size_of_zpat(wsize));
	if (wpat == NULL) {
		return trios_error(1,
				   "papplic_short: Memory allocation failed.");
	}


	/* Begin case A : cv=0 (All pixels whose value is different from
	   0 are considered in the w-pattern) */

	if (!cv) {

		for (i = 0; i < npixels; i++) {	/* shifts the window */

			if (ucpixels1[i] != 0) {	/* mask condition satisfied */

				for (j = 0; j < wzip; j++)
					wpat[j] = 0;	/* blank w-pattern */

				/* gets the w-pattern centered at the point j */
				for (j = 0; j < wsize; j++) {	/* for each point of the window... */

					k = i + offset[j];
					if (uspixels[k] != 0) {
						s = j / NB;
						nshifts = j % NB;
						wpat[s] =
						    (wpat[s] | bitmsk[nshifts]);
					}

				}

				/* checks if the configuration is not in the intervals */

				label = itv_list_contain(itv, wpat, wzip);

				/* if found, put the gray-level to the output image */

				uspixels2[i] = (unsigned short) label;

			}

		}

	}

	/* Begin case B : cv=1 (Only the pixels whose value is the same as the
	   pixel under the center of the window are considered in the w-pattern */
	else {

		for (i = 0; i < npixels; i++) {	/* shifts the window */

			if (ucpixels1[i] != 0) {	/* mask condition satisfied */

				for (j = 0; j < wzip; j++)
					wpat[j] = 0;	/* blank w-pattern */

				/* gets the w-pattern centered at the point j */
				for (j = 0; j < wsize; j++) {	/* for each point of the window... */

					k = i + offset[j];
					if ((uspixels[k] != 0)
					    && ((uspixels[k] == uspixels[i])
						|| !uspixels[i])) {
						s = j / NB;
						nshifts = j % NB;
						wpat[s] =
						    (wpat[s] | bitmsk[nshifts]);
					}

				}

				/* checks if the configuration is not in the intervals */

				label = itv_list_contain(itv, wpat, wzip);

				/* if found, put the gray-level to the output image */

				uspixels2[i] = (unsigned short) label;

			}

		}

	}

	free(offset);
	free(wpat);

	return (1);

}
