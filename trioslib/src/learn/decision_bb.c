#include "trios_win.h"
#include "trios_itv.h"
#include "trios_mtm.h"
#include <trios_xpl.h>
#include "paclearn_local.h"
#include <stdio.h>

/* #define _DEBUG_ */
/* #define _DEBUG_1_ */

/*!
    Makes the decision for BB and BG. Recursive version.

    \param p Set of examples.
    \param mtm Classified examples set.
    \param sum Total number of examples.
    \param wzip Number of bytes of the w-pattern.
    \param freqmin Minimal frequency of a w-pattern. If the pattern has a smaller frequency it is ignored.
    \param estimator Decision estimator.
    \param undflabel Value assigned to the undecidable examples.
    \return 1 on success. 0 on failure.
*/
int decide_tree_bb(xpl_BB * p, mtm_t * mtm, unsigned int sum, int wzip,
		   double freqmin, double estimator, int undflabel)
{
	float verify;
	int wpat_max;
	unsigned int wpat_freq;

	if (p) {

		if (!decide_tree_bb
		    (p->left, mtm, sum, wzip, freqmin, estimator, undflabel))
			return trios_error(MSG, "decide_tree_bb failure");

		wpat_freq = p->fq0 + p->fq1;

		verify = ((float)wpat_freq / sum) * 100;

#ifdef _DEBUG_
		trios_debug("mtmbbtree wpat_freq=%d, sum=%d, verify=%f wzip=%d",
			    wpat_freq, sum, verify, wzip);
#endif

		if (verify >= freqmin) {

			if (estimator == AVERAGE) {

				int wpat_label;

				if (p->fq0 < p->fq1)
					wpat_label = 1;
				else
					wpat_label = 0;

#ifdef _DEBUG_
				trios_debug
				    ("mtmbbtree Vai inserir wpat[0]=%d label=%d",
				     p->wpat[0], wpat_label);
#endif

				if (!mtm_BX_insert
				    (mtm, (int)mtm->nmtm, wzip, p->wpat,
                     wpat_label, wpat_freq, p->fq1)) {
					return trios_error(MSG,
							   "decide_tree_bb failure");
				}

				mtm->nmtm++;

			}

			else if (estimator == MABS) {

				int wpat_label;

				if ((p->fq0 == 0) || (p->fq1 == 0))
					wpat_label = (p->fq0 == 0) ? 1 : 0;
				else
					wpat_label = undflabel;

#ifdef _DEBUG_
				trios_debug
				    ("mtmbbtree Vai inserir wpat[0]=%d label=%d",
				     p->wpat[0], wpat_label);
#endif

				if (!mtm_BX_insert
				    (mtm, (int)mtm->nmtm, wzip, p->wpat,
                     wpat_label, wpat_freq, p->fq1)) {
					return trios_error(MSG,
							   "decide_tree_bb failure");
				}

				/*      
				   mtm->mtm_freq[wpat_label] += wpat_freq ;
				 */

				mtm->nmtm++;

			}

			else {

				int wpat_label;

				if (p->fq0 < p->fq1) {
					wpat_max = p->fq1;
					wpat_label = 1;
				} else {
					wpat_max = p->fq0;
					wpat_label = 0;
				}

				if (((float)wpat_max / wpat_freq) >= estimator) {
					if (!mtm_BX_insert
					    (mtm, (int)mtm->nmtm, wzip, p->wpat,
                         wpat_label, wpat_freq, p->fq1)) {
						return trios_error(MSG,
								   "decide_tree_bb failure");
					}
				} else {
					if (!mtm_BX_insert
					    (mtm, (int)mtm->nmtm, wzip, p->wpat,
                         undflabel, wpat_freq, p->fq1)) {
						return trios_error(MSG,
								   "decide_tree_bb failure");
					}
				}
				mtm->nmtm++;
			}

		}

		if (!decide_tree_bb
		    (p->right, mtm, sum, wzip, freqmin, estimator, undflabel))
			return trios_error(MSG, "decide_tree_bb failure");

	}

	return (1);

}

/*!
    Makes the decision for BB and BG. Iterative version.

    \param xpl Set of examples.
    \param mtm Classified examples set.
    \param freqmin Minimal frequency of a w-pattern. If the pattern has a smaller frequency it is ignored.
    \param estimator Decision estimator.
    \param undflabel Value assigned to the undecidable examples.
    \return 1 on success. 0 on failure.
*/
int decide_ll_bb(xpl_t * xpl, mtm_t * mtm, double freqmin, double estimator,
		 int undflabel, int comp_prob)
{
/*  Modification */
/*  Date: Thu Aug 19 1999                                                   */
/*  Mod: instead keeping frequency information as probabilities, now they   */
/*       will be kept as integer numbers. The frequency of a given pattern  */
/*       is now information included in the mtm file. However, the number   */
/*       of times a given pattern has been observed with label 1 will       */
/*       be an optional information (option comp_prob must be set to 1      */
/*       in order include this info to the file)                            */

	float verify;
	int wpat_max, wpat_label, wzip;
	unsigned int sum, wpat_freq;
	xpl_BB *head;

	/* Transform the tree structure of xpl->root to a linked list structure */
	/* in order to make it useful in a non-recursive algorithm              */
	xpl_tree2ll(xpl, 1);
	head = (xpl_BB *) xpl->root;
	sum = xpl_get_sum(xpl);
	wzip = size_of_zpat(xpl_get_wsize(xpl));

	/* If estimator is AVERAGE the most frequent label is assigned to */
	/* w-pattern                                                      */
	if (estimator == AVERAGE) {

		while (head) {
			wpat_freq = head->fq0 + head->fq1;

			verify = ((float)wpat_freq / sum) * 100;

			if (verify >= freqmin) {

				if (head->fq0 < head->fq1) {
					wpat_label = 1;
				} else if (head->fq0 > head->fq1) {	/* Mod. Mon Feb 23 1998 */
					wpat_label = 0;
				} else {	/* Mod. Mon Feb 23 1998 */
					wpat_label = undflabel;	/* Mod. Mon Feb 23 1998 */
				}

				if (!comp_prob) {
					if (!mtm_BX_insert
					    (mtm, (int)mtm->nmtm, wzip,
					     head->wpat, wpat_label, wpat_freq,
					     0)) {
						return trios_error(MSG,
								   "mtm_BX_insert() failed.");
					}
				} else {
					if (!mtm_BX_insert
					    (mtm, (int)mtm->nmtm, wzip,
					     head->wpat, wpat_label, wpat_freq,
					     head->fq1)) {
						return trios_error(MSG,
								   "mtm_BX_insert() failed.");
					}
				}
				mtm->nmtm++;

			}
			head = head->right;
		}
	}
	/* if estimator==MABS, then we have a decidable case only if     */
	/* there is just one label assigned to the example.              */
	/* Otherwise the example is undecidable, and the label specified */
	/* by the user will be used (undflabel)                             */
	else if (estimator == MABS) {
		while (head) {
			wpat_freq = head->fq0 + head->fq1;

			verify = ((float)wpat_freq / sum) * 100;

			if (verify >= freqmin) {
				if ((head->fq0 == 0) || (head->fq1 == 0))
					wpat_label = (head->fq0 == 0) ? 1 : 0;
				else
					wpat_label = undflabel;

				if (!comp_prob) {
					if (!mtm_BX_insert
					    (mtm, (int)mtm->nmtm, wzip,
					     head->wpat, wpat_label, wpat_freq,
					     0)) {
						return trios_error(MSG,
								   "mtm_BX_insert() failed.");
					}
				} else {
					if (!mtm_BX_insert
					    (mtm, (int)mtm->nmtm, wzip,
					     head->wpat, wpat_label, wpat_freq,
					     head->fq1)) {
						return trios_error(MSG,
								   "mtm_BX_insert() failed.");
					}
				}

				mtm->nmtm++;
			}
			head = head->right;
		}
	}

	else {
		while (head) {
			wpat_freq = head->fq0 + head->fq1;

			verify = ((float)wpat_freq / sum) * 100;

			if (verify >= freqmin) {
				if (head->fq0 < head->fq1) {
					wpat_max = head->fq1;
					wpat_label = 1;
				} else {
					wpat_max = head->fq0;
					wpat_label = 0;
				}

				if (((float)wpat_max / wpat_freq) < estimator) {
					wpat_label = undflabel;
				}
				if (!comp_prob) {
					if (!mtm_BX_insert
					    (mtm, (int)mtm->nmtm, wzip,
					     head->wpat, wpat_label, wpat_freq,
					     0)) {
						return trios_error(MSG,
								   "mtm_BX_insert() failed.");
					}
				} else {
					if (!mtm_BX_insert
					    (mtm, (int)mtm->nmtm, wzip,
					     head->wpat, wpat_label, wpat_freq,
					     head->fq1)) {
						return trios_error(MSG,
								   "mtm_BX_insert() failed");
					}
				}

				mtm->nmtm++;
			}
			head = head->right;
		}

	}

	return (1);
}
