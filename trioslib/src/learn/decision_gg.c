#include <trios.h>

/* #define _DEBUG_ */

/*  Modification by:  Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)       */
/*  Date: Thu Aug 19 1999                                                   */
/*  Mod: each node of the mtm table will incorporate a field fq, which      */
/*       tells how many times the example has been observed during          */
/*       sampling.                                                          */

/*
     -------------------------------------------
     FUNCTION: DECIDE_TREE_GG - MODA
     -------------------------------------------
*/

int /*+ Purpose: Makes the decision for GG                 + */
decide_tree_gg_moda(
				  xpl_GG * p,	/*+ In: Begining  of the examples                      + */
				  mtm_t * mtm,	/*+ In/Out:  Pointer to minterms                       + */
				  unsigned int sum,	/*+ In: Total number of examples                     + */
				  int wsize,	/*+ In: wpatern size (in bytes)                        + */
				  double freqmin,	/*+ In: Minimal frequency to consider an example       + */
				  int undflabel,	/*+ In: Value assigned to undecidable cases            + */
				  int comptype	/*+ In: Type of computation to be done for assigning the
						   label of the undecidable pattern                     + */
    )
/*+ Return: 1 on Success 0 on Failure                                       +*/
{
/*  author: Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)                  */
/*  date:   Wed Nov 12 1997                                                  */

/*  Modification by:  Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)       */
/*  Date: Tue Jan 13 1998                                                   */
/*  Mod: Fixed a bug of indexing an array by a char                         */

/*  Modification by:  Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)       */
/*  Date: Mon Aug  2 1999                                                   */
/*  Mod: Modified to support int labels and do not compute the freq         */

/*  Modification by:  Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)       */
/*  Date: Thu Aug 19 1999                                                   */
/*  Mod: changes regarding frequency information                            */

/*  Modification by:  Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)       */
/*  Date: Wed Feb 16 2000                                                   */
/*  Mod: Moved the code of decision to a new routine freq_moda              */

	unsigned int wpat_freq;	/* the frequency of a pattern               */
	int wpat_label;		/* the most frequent label                  */

	int status;		/* used to verify if the pattern was sufficiently
				   observed to be taken in consideration            */

#ifdef DEBUG_
	trios_debug("Starting decide moda");
#endif

	if (p) {

		if (!decide_tree_gg_moda
		    (p->left, mtm, sum, wsize, freqmin, undflabel, comptype))
			return trios_error(MSG,
					   "decide_tree_gg_moda: decide_tree_gg_moda() failed");

		status =
		    freq_moda(p->freqlist, sum, freqmin, undflabel, comptype,
			      &wpat_label, &wpat_freq);

#ifdef _DEBUG_
		trios_debug("status =%d", status);
#endif

		if (status == 1) {
			if (!mtm_GX_insert
			    (mtm, (int)mtm->nmtm, wsize, p->wpat, wpat_label,
			     wpat_freq)) {
				return trios_error(MSG,
						   "decide_tree_gg: mtm_GX_insert() failed");
			}
			mtm->nmtm++;
		} else if (status == 0)
			return trios_error(MSG,
					   "decide_tree_gg: freq_moda() failed");

		if (!decide_tree_gg_moda
		    (p->right, mtm, sum, wsize, freqmin, undflabel, comptype))
			return trios_error(MSG,
					   "decide_tree_gg: decide_tree_gg_moda() failed");
	}
	return (1);

}

/*
     -------------------------------------------
     FUNCTION: DECIDE_TREE_GG - AVERAGE
     -------------------------------------------
*/

int /*+ Purpose: Makes the decision  for GB and GG         + */
decide_tree_gg_average(
				     xpl_GG * p,	/*+ In: Begining  of the examples                      + */
				     mtm_t * mtm,	/*+ In/Out:  Pointer to minterms                       + */
				     unsigned int sum,	/*+ In: Total number of examples                       + */
				     int wsize,	/*+ In: size of wpatern (in bytes)                     + */
				     double freqmin	/*+ In: Minimal frequency to consider an example       + */
    )
/*+ Return: 1 on Success 0 on Failure                                       +*/
{
/*  author: Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)                  */
/*  date:   Wed Nov 12 1997                                                  */

/*  Modification by:  Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)       */
/*  Date: Tue Jan 13 1998                                                   */
/*  Mod: Fixed a bug of indexing an array by a char                         */

/*  Modification by:  Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)       */
/*  Date: Mon Aug  2 1999                                                   */
/*  Mod: Modified to support int labels and do not compute the freq         */

/*  Modification by:  Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)       */
/*  Date: Thu Aug 19 1999                                                   */
/*  Mod: changes regarding frequency information                            */

/*  Modification by:  Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)       */
/*  Date: Wed Feb 16 2000                                                   */
/*  Mod: Moved the code of decision to a new routine freq_average           */

	unsigned int wpat_freq;	/* the total frequency of the pattern */
	int wpat_label;		/* the labels of a pattern            */

	int status;

#ifdef _DEBUG_
	trios_debug("Starting decide average");
#endif

	if (p) {

		if (!decide_tree_gg_average(p->left, mtm, sum, wsize, freqmin))
			return trios_error(MSG,
					   "decide_tree_gg: decide_tree_gg_average() failed");

		status =
		    freq_average(p->freqlist, sum, freqmin, &wpat_label,
				 &wpat_freq);

#ifdef _DEBUG_
		trios_debug("status =%d", status);
#endif

		if (status == 1) {
			if (!mtm_GX_insert
			    (mtm, (int)mtm->nmtm, wsize, p->wpat, wpat_label,
			     wpat_freq)) {
				return trios_error(MSG,
						   "decide_tree_gg_average: mtm_GX_insert() failure");
			}
			mtm->nmtm++;

		}

		if (!decide_tree_gg_average(p->right, mtm, sum, wsize, freqmin))
			return trios_error(MSG,
					   "decide_tree_gg: decide_tree_gg_average() failed");
	}

	return (1);
}

/*
     -------------------------------------------
     FUNCTION: DECIDE_TREE_GG - MEDIAN
     -------------------------------------------
*/

int /*+ Purpose: Makes the decision for GG                 + */
decide_tree_gg_median(
				    xpl_GG * p,	/*+ In: Begining  of the examples                      + */
				    mtm_t * mtm,	/*+ In/Out:  Pointer to minterms                       + */
				    unsigned int sum,	/*+ In: Total number of examples                       + */
				    int wsize,	/*+ In: size of wpatern (in bytes)                     + */
				    double freqmin,	/*+ In: Minimal frequency to consider an example, in % + */
				    int undflabel	/*+ In: Value assigned to undecidable cases            + */
    )
/*+ Return: 1 on Success 0 on Failure                                       +*/
{
/*  author: Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)                  */
/*  date: Mon Aug  2 1999                                                    */

/*  Modification by:  Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)       */
/*  Date: Thu Aug 19 1999                                                   */
/*  Mod: changes regarding frequency information                            */

/*  Modification by:  Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)       */
/*  Date: Wed Feb 16 2000                                                   */
/*  Mod: Moved the code of decision to a new routine freq_median            */

	unsigned int wpat_freq;	/* the frequency of a pattern */
	int wpat_label;		/* the label of a pattern */

	int status;

	if (p) {

		if (!decide_tree_gg_median
		    (p->left, mtm, sum, wsize, freqmin, undflabel))
			return trios_error(MSG,
					   "decide_tree_gg_median() failure");

		status =
		    freq_median(p->freqlist, sum, freqmin, &wpat_label,
				&wpat_freq);

		if (status == 1) {

			if (!mtm_GX_insert
			    (mtm, (int)mtm->nmtm, wsize, p->wpat, wpat_label,
			     wpat_freq)) {
				return trios_error(MSG,
						   "decide_tree_gg_median: mtm_GX_insert() failure");
			}
			mtm->nmtm++;
		}

		if (!decide_tree_gg_median
		    (p->right, mtm, sum, wsize, freqmin, undflabel))
			return trios_error(MSG,
					   "decide_tree_gg_median() failure");

	}

	return (1);

}

/*
     -------------------------------------------
     FUNCTION: DECIDE_TREE_GG - ABSOLUTE
     -------------------------------------------
*/

int /*+ Purpose: Makes the decision  for GB and GG         + */
decide_tree_gg_absolute(
				      xpl_GG * p,	/*+ In: Begining  of the examples                      + */
				      mtm_t * mtm,	/*+ In/Out:  Pointer to minterms                       + */
				      unsigned int sum,	/*+ In: Total number of examples                       + */
				      int wsize,	/*+ In: size of wpatern (in bytes)                     + */
				      double freqmin,	/*+ In: Minimal frequency to consider an example       + */
				      int undflabel	/*+ In: Value assigned to undecidable cases            + */
    )
/*+ Return: 1 on Success 0 on Failure                                       +*/
{
/*  author: Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)                  */
/*  date:   Wed Nov 12 1997                                                  */

/*  Modification by:  Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)       */
/*  Date: Tue Jan 13 1998                                                   */
/*  Mod: Fixed a bug of indexing an array by a char                         */

/*  Modification by:  Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)       */
/*  Date: Thu Aug 19 1999                                                   */
/*  Mod: changes regarding frequency information                            */

/*  Modification by:  Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)       */
/*  Date: Wed Feb 16 2000                                                   */
/*  Mod: Moved the code of decision to a new routine freq_absolute          */

	unsigned int wpat_freq;	/* the frequency of a pattern */
	int wpat_label;		/* the label of a pattern */

	int status;

	if (p) {

		if (!decide_tree_gg_absolute
		    (p->left, mtm, sum, wsize, freqmin, undflabel))
			return trios_error(MSG,
					   "decide_tree_gg_absolute() failure");

		status =
		    freq_absolute(p->freqlist, sum, freqmin, undflabel,
				  &wpat_label, &wpat_freq);

		if (status == 1) {

			if (!mtm_GX_insert
			    (mtm, (int)mtm->nmtm, wsize, p->wpat, wpat_label,
			     wpat_freq)) {
				return trios_error(MSG,
						   "decide_tree_gg_absolute: mtm_GX_insert() failure");
			}
			mtm->nmtm++;
		}

		if (!decide_tree_gg_absolute
		    (p->right, mtm, sum, wsize, freqmin, undflabel))
			return trios_error(MSG,
					   "decide_tree_gg_absolute() failure");

	}

	return (1);

}

/*
     -------------------------------------------
     FUNCTION: DECIDE_TREE_GG - GENERAL
     -------------------------------------------
*/

int /*+ Purpose: Makes the decision  for GB and GG         + */
decide_tree_gg_general(
				     xpl_GG * p,	/*+ In: Begining  of the examples                      + */
				     mtm_t * mtm,	/*+ In/Out:  Pointer to minterms                       + */
				     unsigned int sum,	/*+ In: Total number of examples                       + */
				     int wsize,	/*+ In: size of wpatern (in bytes)                     + */
				     double freqmin,	/*+ In: Minimal frequency to consider an example, in % + */
				     double estimator,	/*+ In: Decision Estimator                             + */
				     int undflabel,	/*+ In: Value assigned to undecidable cases            + */
				     int comptype	/*+ In: Type of computation to be done for assigning the
							   label of the undecidable pattern                     + */
    )
/*+ Return: 1 on Success 0 on Failure                                       +*/
{
/*  author: Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)                  */
/*  date:   Wed Nov 12 1997                                                  */

/*  Modification by:  Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)       */
/*  Date: Tue Jan 13 1998                                                   */
/*  Mod: Fixed a bug of indexing an array by a char                         */

/*  Modification by:  Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)       */
/*  Date: Thu Aug 19 1999                                                   */
/*  Mod: changes regarding frequency information                            */

/*  Modification by:  Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)       */
/*  Date: Wed Feb 16 2000                                                   */
/*  Mod: Moved the code of decision to a new routine freq_general           */

	unsigned int wpat_freq;	/* the frequency of a pattern */
	int wpat_label;		/* the label of a pattern */

	int status;

	if (p) {

		if (!decide_tree_gg_general
		    (p->left, mtm, sum, wsize, freqmin, estimator, undflabel,
		     comptype))
			return trios_error(MSG,
					   "decide_tree_gg_general() failure");

		status =
		    freq_absolute(p->freqlist, sum, freqmin, undflabel,
				  &wpat_label, &wpat_freq);

		if (status == 1) {

			if (!mtm_GX_insert
			    (mtm, (int)mtm->nmtm, wsize, p->wpat, wpat_label,
			     wpat_freq)) {
				return trios_error(MSG,
						   "decide_tree_bg_general: mtm_BX_insert() failure");
			}

			mtm->nmtm++;

		}

		if (!decide_tree_gg_general
		    (p->right, mtm, sum, wsize, freqmin, estimator, undflabel,
		     comptype))
			return trios_error(MSG,
					   "decide_tree_gg_general() failure");
	}

	return (1);

}
