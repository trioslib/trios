#include <trios.h>

/*!
 * Builds an example's set from a pair of input-output
 * images that represents a gray-scale-to-gray-scale transformation. The
 * examples collecting process can be gathered by a mask image.
 * These three images are considered gray-scale ones.
 * \param p1 Input image data.
 * \param p2 Output image data.
 * \param p3 Mask image data.
 * \param offset Vector of offsets (window).
 * \param wsize Size of the offset vector.
 * \param npixels Number of pixels in the images.
 * \return An example's set on success, NULL on failure.
 */
xpl_t *collec_GG(unsigned char *p1,	unsigned char *p2, unsigned char *p3, int *offset, int wsize, int npixels) {
	int i, j, k;
    int *wpat;		/* w-pattern */
	int st;
	xpl_t *xpl;		/* XPL structure */
	freq_node *freqnode;	/* pointer to a frequency node */
#ifdef _DEBUG_
    trios_debug("Entrei no collec_GG");
#endif
#ifdef _DEBUG_1_
	for (i = 0; i < wsize; i++) {
        trios_debug("offset[%d]=%d\n", i, offset[i]);
	}
#endif

    wpat = (int *) malloc(sizeof(int) * wsize);
	if (wpat == NULL) {
		return (xpl_t *) trios_error(1, "Memory allocation failed.");
	}

	if ((xpl = xpl_create(wsize, GG)) == NULL) {
		free(wpat);
		return (xpl_t *) trios_error(MSG,
					     "collec_GG: xpl_create() failed.");
	}

	for (j = 0; j < npixels; j++) {	/* shifts the window */

		if (p3[j] != 0) {	/* mask condition satisfied */

			for (i = 0; i < xpl->wsize; i++)
				wpat[i] = 0;	/* blank w-pattern */

			/* gets the w-pattern centered at the point j */

			for (i = 0; i < wsize; i++) {	/* for each point of the window... */
				k = j + offset[i];
                if (k >= 0 && k < npixels && p1[k] != 0)
					wpat[i] = p1[k];
			}

#ifdef _DEBUG_2_
            trios_debug("Pattern coletado");
			for (i = 0; i < wsize; i++) {
                trios_debug("wpat[%d]=%d\n", i, wpat[i]);
			}
            trios_debug("p2[%d]=%d\n", j, p2[j]);
#endif

			if ((freqnode =
			     freq_node_create((int) p2[j], 1)) == NULL) {
				free(wpat);
				return (xpl_t *) trios_error(MSG,
							     "collec_GG: freq_node_create() failed.");
			}

			/* insert new w-pattern into example's set */

#ifdef _DEBUG_2_
            trios_debug("freqnode criado");
            trios_debug("label1=%d , freq1=%d\n", freqnode->label,
				  freqnode->freq);
#endif

			st = xpl_GG_insert(xpl, (xpl_GG **) (&xpl->root), wpat,
					   freqnode);

			if (st == -1) {
				xpl_free(xpl);
				free(wpat);
				return (xpl_t *) trios_error(MSG,
							     "collec_GG: xpl_GG_insert() failed.");
			}

		}

	}

	free(wpat);
	return (xpl);

}
