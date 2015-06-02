#include <trios_mtm.h>
#include "trios_error.h"
#include "trios_misc.h"

 /*#define _DEBUG_
    #define _DEBUG_2_ */

mtm_t *mtm_create(int wsize, int type, unsigned int nmtm)
{
	mtm_t *mtm;

	if ((mtm = (mtm_t *) malloc(sizeof(mtm_t))) == NULL) {
		return (mtm_t *) trios_error(1, "Memory allocation failed.");
	}

	mtm->wsize = wsize;
	mtm->type = type;
	mtm->nmtm = nmtm;
	mtm->mtm_freq = NULL;
	mtm->mtm_data = NULL;
	mtm->comp_prob = 0;
	mtm->nsum = 0;

	switch (type) {

	case BB:
	case BG:{

            if (!(mtm->mtm_data = (int *)malloc(sizeof(mtm_BX) * nmtm))) {
				free(mtm);
                return (mtm_t *) trios_error(1, "mtm_create: memory allocation error for mtm");
			}
			break;
		}

	case GB:
	case GG:
	case WKC:
	case WKF:
	case WK3F:
	case WK3C:
	case WKGG2F:
	case WKGG2C:
	case GG3:{
			if (!
			    (mtm->mtm_data =
			     (int *)malloc(sizeof(mtm_GX) * nmtm))) {
				free(mtm);
				return (mtm_t *) trios_error(1,
							     "mtm_create: memory allocation error for mtm");
			}
			break;
		}

	default:{
			return
			    (mtm_t *) trios_error(1,
						  "mtm_create: Invalid type, not BB or BG or GB or GG or WKGG");
			break;
		}
	}

	return (mtm);

}

void mtm_free(mtm_t * mtm)
{

	int i;
	mtm_BX *table_BX;
	mtm_GX *table_GX;

	if (mtm == NULL)
		return;

	if (mtm->mtm_freq) {
		freqlist_free(mtm->mtm_freq);
	}

	if (mtm->mtm_data) {
		switch (mtm->type) {
		case BB:
		case BG:{

				table_BX = (mtm_BX *) mtm->mtm_data;
				for (i = 0; i < mtm->nmtm; i++)
					if (table_BX[i].wpat)
						free(table_BX[i].wpat);
				break;
			}

		case GB:
		case GG:
		case WKC:
		case WKF:
		case WK3F:
		case WK3C:
		case WKGG2F:
		case WKGG2C:
		case GG3:{
				table_GX = (mtm_GX *) mtm->mtm_data;
				for (i = 0; i < mtm->nmtm; i++)
					free(table_GX[i].wpat);
				break;
			}

		default:{
				trios_error(MSG,
					    "free_mtm failure. Invalid type (not BX or GX).");
				break;

			}
		}

		free(mtm->mtm_data);
	}

	free(mtm);

}

int mtm_get_wsize(mtm_t * mtm)
{
	return (mtm->wsize);
}

int mtm_get_type(mtm_t * mtm)
{
	return (mtm->type);
}

unsigned int mtm_get_nmtm(mtm_t * mtm)
{
	return (mtm->nmtm);
}

unsigned int mtm_get_nsum(mtm_t * mtm)
{
	return (mtm->nsum);

}

int mtm_get_comp_prob(mtm_t * mtm)
{
	return (mtm->comp_prob);

}

freq_node *mtm_get_freqlist(mtm_t * mtm_i)
{
	freq_node *p;

	p = mtm_i->mtm_freq;

	return (p);
}

void mtm_set_wsize(mtm_t * mtm, int wsize)
{
	mtm->wsize = wsize;

}

void mtm_set_type(mtm_t * mtm, int type)
{
	mtm->type = type;

}

void mtm_set_nmtm(mtm_t * mtm, unsigned int nmtm)
{
	mtm->nmtm = nmtm;

}

void mtm_set_nsum(mtm_t * mtm, unsigned int nsum)
{

	mtm->nsum = nsum;

}

void mtm_set_comp_prob(mtm_t * mtm, int comp_prob)
{

	mtm->comp_prob = comp_prob;

}

int mtm_BX_insert(mtm_t * mtm, int index1, int wzip, unsigned int *wpat,
		  int label, unsigned int fq, unsigned int fq1)
{
	mtm_BX *p;
	freq_node *freqlist, *freqnode;
	int j;

	p = (mtm_BX *) mtm->mtm_data;

	freqlist = (freq_node *) mtm->mtm_freq;

	p[index1].wpat = (unsigned int *)malloc(sizeof(int) * wzip);
	if (!p[index1].wpat) {
		return trios_error(1, "Memory allocation failed.");
	}

	for (j = 0; j < wzip; j++)
		p[index1].wpat[j] = wpat[j];

	p[index1].label = label;	/* Here we removed a cast to char */

	if ((freqnode = freq_node_create(label, 1)) == NULL) {
		return trios_error(MSG,
				   "mtm_insert: freq_node_create() failed.");
	}

	set_freq(freqnode, &freqlist);	/* Insert the node to the list */

	mtm->mtm_freq = freqlist;
	mtm->nsum = mtm->nsum + fq;

	p[index1].fq = fq;
	p[index1].fq1 = fq1;

	return (1);
}

/*
     -------------------------------------------
     FUNCTION: MTM_GX_insert
     -------------------------------------------
*/
int /*+ Purpose: inserts a minterm in the table      + */ mtm_GX_insert(
									       mtm_t * mtm,	/*+ In: pointer to mtm structure                 + */
									       int index1,	/*+ In: insert position                          + */
									       int wsize,	/*+ In: wpat size in bytes                       + */
									       int *wpat,	/*+ In: w-pattern                                + */
									       int label,	/*+ In: label of the inserted minterm            + */
									       unsigned int fq	/*+ In: frequency of wpat                        + */
    )
/*+ Return: 1 on success 0 on failure                                      +*/
{
/*  author: Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)                 */
/*  date: Fri Nov 13 1997                                                   */

/*  Modification by:  Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)       */
/*  Date: Tue Aug 10 1999                                                   */
/*  Mod: Inserted code to treat the list of frequencies                     */

/*  Modification by:  Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)       */
/*  Date: Thu Aug 19 1999                                                   */
/*  Mod: changes regarding frequency information                            */

	mtm_GX *p;
	freq_node *freqlist, *freqnode;
	int j;

	p = (mtm_GX *) mtm->mtm_data;

	freqlist = (freq_node *) mtm->mtm_freq;

	p[index1].wpat = (int *)malloc(sizeof(int) * wsize);
	if (!p[index1].wpat) {
		return trios_error(1, "Memory allocation failed.");
	}
#ifdef _DEBUG_
	trios_debug("index=%d", index1);
	trios_debug("mtm_gx_insert ");
	for (j = 0; j < wsize; j++) {
		trios_debug("wpat[%d]=%d", j, p[index1].wpat[j]);
	}
	trios_debug("with label = %d", label);
#endif

	for (j = 0; j < wsize; j++) {
		p[index1].wpat[j] = wpat[j];
	}

	p[index1].label = label;	/* Here we removed a cast to char */
	p[index1].fq = fq;	/* Aug 19, 1999 */

	if ((freqnode = freq_node_create(label, 1)) == NULL) {
		return trios_error(MSG,
				   "mtm_insert: freq_node_create() failed.");
	}

	set_freq(freqnode, &freqlist);	/* Insert the node to the list */
	mtm->mtm_freq = freqlist;
	mtm->nsum = mtm->nsum + fq;

	return (1);
}

/*
     -------------------------------------------
     FUNCTION: mtm_GX_get_pattern
     -------------------------------------------
*/

int *				/*+ Purpose: Get a pattern from the table                  + */
mtm_GX_get_pattern(mtm_t * mtm,	/*+ In: pointer to mtm structure                 + */
		   int index1	/*+ In: pattern position                         + */
    )
/*+ Return: A char pointer to a pattern                                    +*/
{

/*  author: Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)                 */
/*  date: Thu Jan  8 1998                                                   */

	mtm_GX *p;

	p = (mtm_GX *) mtm->mtm_data;

	return (p[index1].wpat);

}

int /*+ Purpose: Get the label of a  pattern from the table    + */
mtm_GX_get_label(
			       mtm_t * mtm,	/*+ In: pointer to mtm structure                 + */
			       int index1	/*+ In: pattern position                         + */
    )
/*+ Return: the label of the pattern at the given position                 +*/
{

/*  author: Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)                 */
/*  date: Thu Jan  8 1998                                                   */

	mtm_GX *p;

	p = (mtm_GX *) mtm->mtm_data;

	return (p[index1].label);

}

int mtm_sep(mtm_t * mtm_i, int k, mtm_t ** mtm_o1, mtm_t ** mtm_o2)
{
	mtm_BX *p;
	int ct1, ct2;
	int wsize, wzip, type, comp_prob;
	unsigned int nmtm_i, nmtm_1, nmtm_2;
	int i;

	type = mtm_get_type(mtm_i);
	wsize = mtm_get_wsize(mtm_i);
	comp_prob = mtm_get_comp_prob(mtm_i);
	wzip = size_of_zpat(wsize);

	nmtm_i = mtm_get_nmtm(mtm_i);
	nmtm_1 = mtm_count(mtm_i, k);
	nmtm_2 = nmtm_i - nmtm_1;

	*mtm_o1 = mtm_create(wsize, type, nmtm_1);
	*mtm_o2 = mtm_create(wsize, type, nmtm_2);

	if ((*mtm_o1 == NULL) || (*mtm_o2 == NULL)) {
		return trios_error(MSG, "mtm_sep: mtm_create() failed.");
	}

	p = (mtm_BX *) mtm_i->mtm_data;
	ct1 = ct2 = 0;
	i = 0;

	while (i < nmtm_i) {

		if (p[i].label == k) {
			if (!mtm_BX_insert
			    (*mtm_o1, ct1, wzip, p[i].wpat, p[i].label, p[i].fq,
			     p[i].fq1)) {
				mtm_free(*mtm_o1);
				return trios_error(MSG,
						   "select: mtm_BX_insert() failed.");
			}
			ct1++;
			/* *mtm_o1->mtm_freq[p[i].label]++ ; */
		} else {
			if (!mtm_BX_insert
			    (*mtm_o2, ct2, wzip, p[i].wpat, p[i].label, p[i].fq,
			     p[i].fq1)) {
				mtm_free(*mtm_o2);
				return trios_error(MSG,
						   "select: mtm_BX_insert() failed.");
			}
			ct2++;
			/*      *mtm_o2->mtm_freq[p[i].label]++ ; */
		}

		i++;
	}

	if (comp_prob) {
		mtm_set_comp_prob(*mtm_o1, 1);
		mtm_set_comp_prob(*mtm_o2, 1);
	}

	/*
	   mtm_i->mtm_data = NULL ;
	   mtm_i->nmtm = 0 ;
	 */

	return (1);
}

unsigned int mtm_count(mtm_t * mtm_i, int k)
{

	freq_node *p;
	unsigned int counter;

	counter = 0;

	p = mtm_i->mtm_freq;
	while (p) {
		if (p->label == k)
			counter = p->freq;
		p = p->next;
	}

	return (counter);
}

unsigned int mtm_index_by_freq_count(mtm_t * mtm_i, int *indexes)
{
	unsigned int i, freqmax, nlabels, fnow, flast, fold, nmtm;
	freq_node *p, *q, *p_last, *qlist, *qmax, *q_last, *p_zero;

	mtm_BX *bxdata;
	/*mtm_GX *gxdata ; */

	nlabels = 0;
	p_zero = NULL;

	/* Get the freqlist and copy it */
	p = mtm_get_freqlist(mtm_i);
	nmtm = mtm_get_nmtm(mtm_i);

	indexes = (unsigned int *)malloc(sizeof(int) * nmtm);
	if (!indexes) {
		return trios_error(1,
				   "mtm_index_by_freq: Memory allocation failed for indexes.");
	}

	while (p) {
		if ((q = freq_node_create(p->label, p->freq)) == NULL) {
			return trios_error(MSG,
					   "mtm_index_by_freq: freq_node_create() failed.");
		}
		set_freq(q, &qlist);	/* Insert the node to the list */
		nlabels++;
		p = p->next;
	}

	p = p_last = qlist;	/* release pointer */
	q = NULL;

	/* search list for label 0 */

	while (p) {
		if (p->label == 0) {
			nlabels--;
			p_last->next = p->next;
			p_zero = p;
			p_zero->next = NULL;
			break;
		}
		p_last = p;
		p = p->next;
	}

	p = p_last = qlist;

	/* sort all labels but 0 */
	for (i = 0; i < nlabels; i++) {
		q_last = NULL;
		qmax = NULL;
		freqmax = 0;
		/* find the label which has the largest frequency */
		while (p) {
			if (p->freq > freqmax) {
				freqmax = p->freq;
				qmax = p;
				q_last = p_last;
			}
			p_last = p;
			p = p->next;
		}
		/* put the node into q list and update qlist */
		q_last->next = qmax->next;
		qmax->next = q;
		q = qmax;
	}

	p_zero->next = q;
	q = p_zero;

	p = q;
	flast = fold = 0;

	/* update cummulative histogram */

	while (p) {
		fnow = p->freq;
		p->freq = flast = flast + fold;
		fold = fnow;
	}

	/* build the indexes table of mtm                      */
	/* indexes[indexes for label starts at] = i            */

	if (mtm_get_type(mtm_i) == BG) {

		bxdata = (mtm_BX *) mtm_i->mtm_data;

		for (i = 0; i < nmtm; i++) {
			p = q;
			while (p) {
				if (p->label == bxdata[i].label) {
					indexes[p->freq] = i;
					p->freq = p->freq + 1;
					break;
				}
			}
		}

	} else {
		return trios_error(MSG, "Operator not supported");
		/*gxdata = (mtm_GX *)mtm_i->mtm_data ;

		   for (i = 0; i < nmtm; i++) {
		   p = q ;
		   while (p) {
		   if (p->label == gxdata[i].label) {
		   indexes[p->freq] = i ;
		   p->freq = p->freq + 1 ;
		   break ;
		   }
		   }
		   } */
	}

	return (1);
}

int mtm_compute_mae(mtm_t * mtm, double *mae_error)
{
	mtm_BX *q;
	int i, nmtm;
	int FMAE;

	/* ---------------------------------------------------------------------

	   How to compute MAE of psi
	   ^^^^^^^^^^^^^^^^^^^^^^^^^

	   if px = P(y=1|X=x) and Px = P(X=x), then

	   MAE<psi> = sum { Px.px : psi(x)=0 } + sum { Px.(1-px) : psi(x)=1 } 

	   We are using fq(x), fq1(x) and nsum = sum {fq(x) }

	   Let FMAE = sum { fq1(x) : psi(x)=0 } + sum { fq(x)-fq1(x) : psi(x)=1 }

	   Since Px=fq(x)/nsum  and  px=fq1(x)/fq,

	   fq(x) fq1(x)           fq(x)  fq(x) - fq1(x)
	   MAE<psi> = sum {----- ------ } +  sum {----- (--------------) }
	   nsum  fq(x)             nsum      fq(x)
	   fq1(x)                     fq(x) - fq1(x)
	   = sum { ----- : psi(x)=0 } + sum { -------------- : psi(x)=1 }
	   nsum                           nsum

	   Hence,

	   MAE = FMAE/nsum ;

	   --------------------------------------------------------------------- */

	/* If frequency of examples are not given, MAE cannot be */
	/* computed.                                             */
	if (!mtm_get_comp_prob(mtm)) {
		return trios_error(1,
				   "Frequency info needed for MAE computation.");
	}

	q = (mtm_BX *) mtm->mtm_data;
	nmtm = mtm_get_nmtm(mtm);

	FMAE = 0;

	for (i = 0; i < nmtm; i++) {
		if (q[i].label == 1) {
			FMAE = FMAE + (q[i].fq - q[i].fq1);
		} else {
			FMAE = FMAE + q[i].fq1;
		}
	}

	*mae_error = (double)(FMAE) / (double)(mtm->nsum);

	return (1);
}

int mtm_search_wpat_BX(unsigned int nmtm, mtm_t * mtm, unsigned int *wpat,
		       int wzip)
{
	mtm_BX *p;
	int success, result;
	int middle, low, high;

#ifdef _DEBUG_
	trios_debug("Entering mtm_search_wpat_BX()");
#endif

	success = -1;
	low = 0;
	high = nmtm - 1;

	p = (mtm_BX *) mtm->mtm_data;

	while (low <= high) {
		middle = (int)((low + high) / 2);
#ifdef _DEBUG_
		trios_debug("middle = %d", middle);
#endif
		result = wpat_compare(wpat, p[middle].wpat, wzip);
		if (result == 0) {
			success = middle;
			break;
		} else {
			if (result == 1) {	/* wpat > p[middle].wpat */
				low = middle + 1;
			} else
				high = middle - 1;	/* wpat < p[middle].wpat */
		}
	}

	return (success);

}
