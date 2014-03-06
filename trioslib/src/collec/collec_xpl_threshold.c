#include "trios_img.h"
#include "trios_xpl.h"
#include "trios_win.h"
#include "trios_imgset.h"
#include "trios_io.h"
#include "trios_collec.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*
 * ------------------------------------------- FUNCTION: colthresh_BB
 * ------------------------------------------- 
 */

xpl_t *				/* + Purpose: Threshold gray-level image
				 * and collects binary w-patterns + */
colthresh_BB(unsigned short *s1,	/* + In: input image data + */
	     unsigned char *p2,	/* + Out: output image data + */
	     unsigned char *p3,	/* + In: mask image data + */
	     int *offset,	/* + In: vector of offsets (contains
				 * window information) + */
	     int wsize,		/* + In: size of vector of offsets + */
	     int npixels,	/* + In: number of pixels in the image + */
	     int maxgrey	/* + In: maximun gray level value + */
    )
/*
 * + Returns: an example's set on success, NULL on failure +
 */
{
	int i, j, k, s;
	unsigned int freq0, freq1;
	int nshifts;
	unsigned int *bpat;
	int *wpat, *ord;
	int aux, st;
	xpl_t *xpl;		/* XPL structure */

	bpat = (unsigned int *)malloc(sizeof(int) * size_of_zpat(wsize));
	if (bpat == NULL) {
		return (xpl_t *) trios_error(1, "Memory allocation failed.");
	}

	wpat = (int *)malloc(sizeof(int) * wsize);
	if (wpat == NULL) {
		return (xpl_t *) trios_error(1, "Memory allocation failed.");
	}

	ord = (int *)malloc(sizeof(int) * (wsize + 2));
	if (ord == NULL) {
		return (xpl_t *) trios_error(1, "Memory allocation failed.");
	}

	/*
	 * create the output XPL structure 
	 */
	if ((xpl = xpl_create(wsize, BB)) == NULL) {
		free(wpat);
		free(bpat);
		free(ord);
		return (xpl_t *) trios_error(MSG,
					     "collec_BB: xpl_create() failed.");
	}
#ifdef _DEBUG_
	trios_debug("xpl structure have been allocated");
#endif

	for (j = 0; j < npixels; j++) {	/* shifts the window through each
					 * pixel */

		if (p3[j] != 0) {	/* mask condition satisfied */

			for (k = 0; k < wsize; k++)
				wpat[k] = 0;	/* blank w-pattern */

			/*
			 * gets the w-pattern centered at the point j 
			 */
			for (k = 0; k < wsize; k++) {	/* for each point of the
							 * window... */
				i = j + offset[k];
				if (i >= 0 && i < npixels)
					wpat[k] = (int)s1[i];
			}

			for (k = 0; k < wsize; k++) {
				ord[k + 1] = wpat[k];
			}
			ord[0] = 0;
			ord[wsize + 1] = maxgrey;

			/*
			 * sort the pixel values of the pattern observed 
			 */
			/*
			 * ord[0]=0 and ord[wsize]=maxgrey, always 
			 */
			for (i = 1; i < wsize; i++) {
				for (k = i + 1; k < wsize + 1; k++) {
					if (ord[i] > ord[k]) {
						aux = ord[k];
						ord[k] = ord[i];
						ord[i] = aux;
					}
				}
			}
#ifdef _DEBUG_
			if ((j > 2000) && (j < 2100)) {
				for (i = 0; i < wsize + 2; i++) {
					trios_debug("ord[%d]=%d", i, ord[i]);
				}
				trios_debug("y=%d\n", p2[j]);
			}
#endif

			/*
			 * generate binary examples from the gray scale example 
			 */
			for (i = 1; i < wsize + 2; i++) {
				aux = ord[i] - ord[i - 1];
				if (aux > 0) {
					for (k = 0; k < xpl->wzip; k++)
						bpat[k] = 0;
					for (k = 0; k < wsize; k++) {
						if (wpat[k] >= ord[i]) {
							s = k / NB;
							nshifts = k % NB;
							bpat[s] =
							    (bpat[s] |
							     bitmsk[nshifts]);
						}
					}
					if ((int)p2[j] >= ord[i]) {
						freq0 = 0;
						freq1 = aux;
					} else if (p2[j] <= ord[i - 1]) {
						freq0 = aux;
						freq1 = 0;
					} else {
						freq1 = p2[j] - ord[i - 1];
						freq0 = aux - freq1;
					}
#ifdef _DEBUG_
					if ((j > 2000) && (j < 2100)) {
						trios_debug
						    ("It will insert pattern %x freq0=%d freq1=%d",
						     bpat[0], freq0, freq1);
					}
#endif

					st = xpl_BB_insert(xpl,
							   (xpl_BB
							    **) (&xpl->root),
							   bpat, freq0, freq1);
					if (st == -1) {
						xpl_free(xpl);
						free(wpat);
						return (xpl_t *)
						    trios_error(MSG,
								"collec_BB: xpl_BB_insert() failed.");
					}
				}
			}
		}
	}

	free(bpat);
	free(ord);
	free(wpat);
	return (xpl);

}

int lcollec_threshold(char *fname_i1, char *fname_i2, int maxgrey,
		      int output_format, char *o1_fname)
{
	imgset_t *imgset;
	window_t *win;
	xpl_t *xpl, *xpl_new;
	int k;
	int *offset;
	int nc, wsize, npixels;
	unsigned short *s1;
	unsigned char *c2, *c3;
	img_t *img1, *img2, *img3;

	/*
	 * read image set file 
	 */
	if ((imgset = imgset_read(fname_i1)) == NULL) {
		return trios_error(MSG, "lcolthresh: imgset_read() failed.");
	}

	/*
	 * check if image group size is greater than or equal to 2 
	 */
	/*
	 * Only the first three images of each set will be used 
	 */
	/*
	 * Fisrt image is the image before transformation 
	 */
	/*
	 * Second image is the image after transformation 
	 */
	/*
	 * Third image is the mask image (indicates which points 
	 */
	/*
	 * of the first image should be scanned) 
	 */
	/*
	 * If third image is absent, then the whole first image 
	 */
	/*
	 * will be scanned (except their margins) 
	 */
	if (imgset_get_grpsize(imgset) < 2) {
		imgset_free(imgset);
		return trios_error(1,
				   "Each image group must contain at leat two image files.");
	}

	/*
	 * read WINSPEC file 
	 */
	if ((win = win_read(fname_i2)) == NULL) {
		imgset_free(imgset);
		return trios_error(MSG, "lcolthresh: win_read() failed.");
	}

	/*
	 * allocate vector of offsets 
	 */
	wsize = win_get_wsize(win);

	offset = (int *)malloc(sizeof(int) * wsize);
	if (offset == NULL) {
		imgset_free(imgset);
		win_free(win);
		return trios_error(MSG, "lcolthresh: offset_create() failed.");
	}

	xpl = xpl_create(wsize, BB);
	if (!xpl) {
		return trios_error(MSG, "lcolthresh: xpl_create() failed.");
	}
#ifdef _DEBUG_
	trios_debug("Starting image scanning");
#endif

	/*
	 * ---------------------------------------------------------------- 
	 */
	/*
	 * Begin : Process each group of images 
	 */
	for (k = 1; k <= imgset_get_ngroups(imgset); k++) {

		/*
		 * read images and convert them to the appropriate format 
		 */
		/*
		 * i.e, input is short, output is byte and mask is byte 
		 */

		if (!get_setofimages(imgset, BB, win, k, &img1, &img2, &img3)) {
			xpl_free(xpl);
			win_free(win);
			free(offset);
			imgset_free(imgset);
			return trios_error(MSG,
					   "lcolthresh: get_images() failed.");
		}
		nc = img_get_width(img1);
		npixels = img_get_width(img1) * img_get_height(img1);

		/*
		 * set vector of offsets (note that for each image group it may
		 * vary) 
		 */
		offset_set(offset, win, nc, 1);

		s1 = (unsigned short *)img_get_data(img1);
		c2 = (unsigned char *)img_get_data(img2);
		c3 = (unsigned char *)img_get_data(img3);

		xpl_new =
		    colthresh_BB(s1, c2, c3, offset, wsize, npixels, maxgrey);
		if (xpl_new == NULL) {
			return trios_error(MSG,
					   "lcolthresh: colthresh_BB() failed.");
		}

		/*
		 * release memory used by the images 
		 */
		img_free(img1);
		img1 = NULL;
		img_free(img2);
		img2 = NULL;
		img_free(img3);
		img3 = NULL;

		/*
		 * merge xpl_new into xpl 
		 */
		if (!xpl_merge(xpl, xpl_new)) {
			return trios_error(MSG, "lcollec: xpl_merge() failed.");
		}
		xpl_free(xpl_new);
		xpl_new = NULL;
	}

#ifdef _DEBUG_
	trios_debug("End of image scanning");
#endif

	if (output_format & XPL_OUTPUT == XPL_OUTPUT) {
		char *ext = malloc(sizeof(char) * (strlen(o1_fname) + 4 + 1));
		strcpy(ext, o1_fname);
		strcat(ext, ".xpl");
		if (!xpl_write(ext, xpl, win, NULL)) {
			imgset_free(imgset);
			win_free(win);
			free(offset);
			xpl_free(xpl);
			free(ext);
			return trios_error(MSG,
					   "lcollec : xpl_write() failed.");
		}
		free(ext);
	}
	if (output_format & SWT_OUTPUT == SWT_OUTPUT) {
		char *ext = malloc(sizeof(char) * (strlen(o1_fname) + 4 + 1));
		strcpy(ext, o1_fname);
		strcat(ext, ".swt");
		swt_write(ext, xpl, win);
		free(ext);
	}

	/*
	 * free allocated memory 
	 */
	imgset_free(imgset);
	win_free(win);
	free(offset);
	xpl_free(xpl);

	return (1);

}
