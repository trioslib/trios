#include "trios_itv.h"
#include "trios_mtm.h"
#include "trios_win.h"
#include "trios_misc.h"
#include "trios_learn.h"

#include "trios_error.h"

#include "paclearn_local.h"

#if defined(_OPENMP) 
#include "omp.h"
#endif

#ifdef __linux__

#include <unistd.h>
#endif

#define MAX_PARTITIONS 768

/*!
 * Given a set of examples (mtm) determines which is the variable (direction) that most equatively partitions the examples.
 * \param mtm Set of classified examples.
 * \param LIMIT Threshold.
 * \return The value corresponding to a direction (0 .. window size).
 */
int which_var(mtm_t * mtm, int LIMIT)
{
/*  author: Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)                 */
/*  Date: Tue Apr 20 1999                                                   */
	mtm_BX *q;
	int aux, i, j, mdiv2, var, min_ct, ct, mask_i;

	mdiv2 = mtm->nmtm / 2;	/* one half of examples */

	q = (mtm_BX *) mtm->mtm_data;	/* pointer to mtm examples */

	var = 0;		/*min_ct = 10000000 ; */
	min_ct = LIMIT;		/* by Martha */

	/* for each variable (0 to wsize-1), counts how many examples
	   of mtm are in the interval where the variable is constant.
	   Keeps the variable that contains the closest number to one
	   half of the examples and return it                         */

	for (i = 0; i < mtm->wsize; i++) {
		ct = 0;
		aux = i / 32;
		mask_i = i % 32;

		for (j = 0; j < mtm->nmtm; j++) {
			if (q[j].wpat[aux] & bitmsk[mask_i]) {
				ct++;
			}
		}
		if (min_ct > abs(ct - mdiv2)) {
			min_ct = abs(ct - mdiv2);
			var = i;
		}
	}

	return (var);
}

/*!
 * Partitions the set of examples recursively until each of the partitions has no more than "threshold" examples.
 * \param mtm Classified examples' set.
 * \param itv Interval to partition.
 * \param threshold Maximum number of examples.
 * \param MTM List of mtm in each partition.
 * \param ITV List of itv in each partition
 * \param noper Next partition to be built (at the end this contains the total number of partitions).
 * \return 1 on success, 0 on failure.
 */
int mtm_part(mtm_t * mtm, itv_t * itv, int threshold, mtm_t ** MTM,
	     itv_t ** ITV, int *noper)
{
/*  author: Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)                 */
/*  date: Tue Apr 20 1999                                                   */
	itv_t *itv0, *itv1;
	mtm_t *mtm0, *mtm1;
	int var;

	if (mtm->nmtm == 0) {
		return (1);
	}
	if (*noper > MAX_PARTITIONS - 1) {
		return trios_error(1,
				   "Insufficient buffer size. Please contact maintenance people.");
	}

	if (mtm->nmtm <= threshold) {
		ITV[*noper] = itv;
		MTM[*noper] = mtm;
		*noper = *noper + 1;
	} else {
		var = which_var(mtm, 10000000);
		/* partitions the intervals */
		itv0 = itv1 = NULL;
		if (!itv_setvar(itv, var, &itv0, &itv1)) {
			return trios_error(MSG,
					   "mtm_part: call to itv_setvar() failed.");
		}
		itv_free(itv);

		/* partition the set of examples */
		if (!sep_mtm(mtm, itv, var, &mtm0, &mtm1)) {
			return trios_error(MSG,
					   "mtm_part: call to sep_mtm() failed.");
		}
		mtm_free(mtm);

		if (!mtm_part(mtm0, itv0, threshold, MTM, ITV, noper)) {
			return trios_error(MSG,
					   "mtm_part: Recursive call to mtm_part() failed.");
		}
		if (!mtm_part(mtm1, itv1, threshold, MTM, ITV, noper)) {
			return trios_error(MSG,
					   "mtm_part: Recursive call to mtm_part() failed.");
		}

	}
	return (1);
}

/*!
 * Separates the set of classified examples in disjoint subsets, according to the decomposition variable.
 * \param mtm Set of classified examples.
 * \param itv Interval list.
 * \param var Direction to partition.
 * \param mtm0 Examples with var=0.
 * \param mtm1 Examples with var=1.
 * \return 1 on success, 0 on failure.
 */
int sep_mtm(mtm_t * mtm, itv_t * itv, int var, mtm_t ** mtm0, mtm_t ** mtm1)
{
/*  author: Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)                 */
/*  date: Wed Jul  8 1998                                                   */

	mtm_BX *q, *q0, *q1;
	unsigned int nmtm, count;
	int j, j0, j1, aux, mask_i, wsize;
	freq_node *fq, *freqlist1 = NULL, *freqlist0 = NULL;

	q0 = q1 = NULL;
	q = (mtm_BX *) mtm->mtm_data;
	nmtm = mtm_get_nmtm(mtm);
	wsize = mtm_get_wsize(mtm);

	/* How many examples with value(var)=ON ? */
	count = 0;
	aux = var / 32;
	mask_i = var % 32;
	for (j = 0; j < mtm->nmtm; j++) {
		if (q[j].wpat[aux] & bitmsk[mask_i]) {
			count++;
		}
	}

	/* create subsets for the examples */
	*mtm0 = *mtm1 = NULL;
	if (nmtm - count > 0) {
		*mtm0 = mtm_create(wsize, BB, nmtm - count);
		if (*mtm0 == NULL) {
			return (0);
		}
		q0 = (mtm_BX *) (*mtm0)->mtm_data;
	}
	if (count > 0) {
		*mtm1 = mtm_create(wsize, BB, count);
		if (*mtm1 == NULL) {
			return trios_error(MSG, "mtm_sep: mtm_create() failed");
		}
		q1 = (mtm_BX *) (*mtm1)->mtm_data;
	}
#ifdef _DEBUG_
	trios_debug("Exemplos ZER0=%d", nmtm - count);
	trios_debug("Exemplos UM=%d", count);
#endif

	/* separate the examples into the subsets */
	aux = var / 32;
	mask_i = var % 32;
	j0 = j1 = 0;

	for (j = 0; j < mtm->nmtm; j++) {

		if (!(fq = freq_node_create(q[j].label, 1))) {
			return trios_error(MSG,
					   "mtm_sep: freq_node_create() failed.");
		}

		if (q[j].wpat[aux] & bitmsk[mask_i]) {
			if (!set_freq(fq, &freqlist1)) {
				return trios_error(MSG,
						   "mtm_sep: set_freq() failed.");
			}
			q1[j1].wpat = q[j].wpat;
			q1[j1].label = q[j].label;
			q1[j1].fq = q[j].fq;
			q1[j1].fq1 = q[j].fq1;
			q[j].wpat = NULL;
			j1++;
		} else {
			if (!set_freq(fq, &freqlist0)) {
				return trios_error(MSG,
						   "mtm_sep: set_freq() failed.");
			}
			q0[j0].wpat = q[j].wpat;
			q0[j0].label = q[j].label;
			q0[j0].fq = q[j].fq;
			q0[j0].fq1 = q[j].fq1;
			q[j].wpat = NULL;
			j0++;
		}
	}

	(*mtm1)->mtm_freq = freqlist1;
	(*mtm0)->mtm_freq = freqlist0;

	return (1);
}

/*!
 * Given an interval, set the indicated variable as a constant (thus creating two subintervals).
 * \param itv Input interval.
 * \param var Variable index.
 * \param itv0 First output interval.
 * \param itv1 Second output interval.
 * \return 1 on success, 0 on failure.
 */
int itv_setvar(itv_t * itv, int var, itv_t ** itv0, itv_t ** itv1)
{

/*  author: Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)                 */
/*  date: Wed Jul  8 1998                                      */

	itv_BX *p, *q;
	unsigned int *A, *B;
	int wzip, i;
	itv_t *I0, *I1;

	/* Let [A,B] be the given interval */
	/* The subintervals that will be created are : */
	/*   [A+X,B] and [A, B-X]                      */
	/* For example, if [A,B]=[000,111] and the variable is 2, */
	/* then the resulting intervals are : */
	/*   [100,111] and [000, 011]                             */

	wzip = size_of_zpat(itv->wsize);

	/* allocates space for the patterns of the interval [A,B] */
	if ((A = (unsigned int *)malloc(sizeof(int) * wzip)) == NULL) {
		return trios_error(1, "itv_setvar: memory allocation error.");
	}
	if ((B = (unsigned int *)malloc(sizeof(int) * wzip)) == NULL) {
		return trios_error(1, "itv_setvar: memory allocation error.");
	}

	p = (itv_BX *) itv->head;
	for (i = 0; i < wzip; i++) {
		A[i] = p->A[i];
		B[i] = p->B[i];
	}

	B[var / 32] = (B[var / 32] & (~bitmsk[var % 32]));

	/* create structure to hold a set of intervals */
	if (NULL == (I0 = itv_create(itv->wsize, BB, 0))) {
		return trios_error(MSG, "itv_setvar: itv_create() failed.");
	}

	/* the itv_t structure will contain, actually, just one interval */
	q = itv_nodebx_create(wzip);
	itvbx_set(q, p->A, B, wzip, 1, (itv_BX *) (I0->head));
	I0->head = (int *)q;
	I0->nitv = 1;
	*itv0 = I0;

	A[var / 32] = A[var / 32] | bitmsk[var % 32];

	/* create structure to hold a set of intervals */
	if (NULL == (I1 = itv_create(itv->wsize, BB, 0))) {
		return trios_error(MSG, "itv_setvar: itv_create() failed.");
	}

	/* the itv_t structure will contain, actually, just one interval */
	q = itv_nodebx_create(wzip);
	itvbx_set(q, A, p->B, wzip, 1, (itv_BX *) (I1->head));
	I1->head = (int *)q;
	I1->nitv = 1;
	*itv1 = I1;

	free(A);
	free(B);

	return (1);
}

int lpartition_memory(window_t * win, mtm_t * __mtm, int itv_type,
		      int threshold, mtm_t *** _mtm_out, itv_t *** _itv_out,
		      int *n_itv)
{
	int r;
	int i, j, noper, wzip;
	char temp[100];
	window_t *tt;
	mtm_t **mtm_out, *mtm;
	itv_t **itv_out, *start_itv;

	mtm = mtm_create(__mtm->wsize, __mtm->type, __mtm->nmtm);
	mtm->nsum = __mtm->nsum;
	for (i = 0; i < mtm->nmtm; i++) {
		((mtm_BX *) mtm->mtm_data)[i].fq =
		    ((mtm_BX *) __mtm->mtm_data)[i].fq;
		((mtm_BX *) mtm->mtm_data)[i].fq1 =
		    ((mtm_BX *) __mtm->mtm_data)[i].fq1;
		((mtm_BX *) mtm->mtm_data)[i].label =
		    ((mtm_BX *) __mtm->mtm_data)[i].label;
		mtm->comp_prob = __mtm->comp_prob;
		wzip = size_of_zpat(mtm_get_wsize(__mtm));
		trios_malloc(((mtm_BX *) mtm->mtm_data)[i].wpat,
			     wzip * sizeof(unsigned int), int,
			     "Error copying mtm");
		for (j = 0; j < wzip; j++) {
			((mtm_BX *) mtm->mtm_data)[i].wpat[j] =
			    ((mtm_BX *) __mtm->mtm_data)[i].wpat[j];
		}
	}

	trios_malloc(mtm_out, sizeof(mtm_t *) * (MAX_PARTITIONS), int,
		     "Failed to alloc mtm_t list");
	trios_malloc(itv_out, sizeof(itv_t *) * (MAX_PARTITIONS), int,
		     "Failed to alloc itv_t list");

	start_itv = itv_gen_itv(win, itv_type, BB, 0, 1, 0);
	if (start_itv == NULL) {
		return trios_error(MSG, "lpartition: itv_gen_itv() failed.");
	}

	noper = 0;
	for (i = 0; i < MAX_PARTITIONS; i++) {
		mtm_out[i] = NULL;
		itv_out[i] = NULL;
	}

	/* Divide examples and compute the respective partitions (intervals) */
	/* The memory space used both by mtm and by start_itv aren't released   */
	if (!mtm_part(mtm, start_itv, threshold, mtm_out, itv_out, &noper)) {
		win_free(win);
		mtm_free(mtm);
		return trios_error(MSG, "lpartition: sep_mtm() failed.");
	}

	*_mtm_out = mtm_out;
	*_itv_out = itv_out;
	*n_itv = noper;
	return 1;
}

/*!
 * Joins a set of intervals, freeing their memory in the process.
 * \param in Interval set.
 * \param n Number of intervals.
 * \return The new interval.
 */
itv_t *litvconcat_memory(itv_t ** in, int n)
{
/*  author: Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)                 */
/*  date: Sun Apr 25 1999                                                   */

	itv_t *out_itv, *in_itv;
	itv_BX *p, *q;
	int i;

	out_itv = in[0];

	for (i = 1; i < n; i++) {
		in_itv = in[i];
		p = (itv_BX *) out_itv->head;
		q = (itv_BX *) in_itv->head;
		if (in_itv->nitv > 0) {
			out_itv->nitv = out_itv->nitv + in_itv->nitv;
			Concatenate_lists(p, q);
			out_itv->head = (int *)p;
			in_itv->head = NULL;
		}
		itv_free(in_itv);
	}

	return out_itv;
}

/*!
 * Runs ISI on a partition of the original interval.
 * \param part_m Partition of the set of classified examples.
 * \param part_i Partition that contains the part_m examples.
 * \param i Number of the partition.
 * \param win Operator window.
 */
itv_t *solve_partition(mtm_t * part_m, itv_t * part_i, int i, window_t * win)
{
	char cmd[100];
	int pid = 0;
#ifdef DEBUG
	printf("Start %d\n", i);
#endif
#ifdef __linux__
	pid = getpid();
#endif
	part_i = lisi_memory(part_m, part_i, 3, 20, 0, 0);
	/*sprintf(cmd, "part.temp%d-%d.itv", pid, i + 1);
	   itv_write(cmd, part_i, win); */
	mtm_free(part_m);
	/*itv_free(part_i); */
#ifdef DEBUG
	printf("Finished %d\n", i);
#endif
	return part_i;
}

itv_t *lisi_partitioned(window_t * win, mtm_t * mtm, int threshold)
{
	itv_t *acc = NULL;
	mtm_t **part_m;
	itv_t **part_i;
	int i, n, pid = 0;
	char cmd[100], cmd2[100];
#ifdef __linux__
	pid = getpid();
#endif

	if (mtm->nmtm <= threshold) {
		itv_t *starting_interval = itv_gen_itv(win, 1, BB, 0, 1, 0);
		return lisi_memory(mtm, starting_interval, 3, 5, 0, 0);
	}

	if (!lpartition_memory(win, mtm, 1, threshold, &part_m, &part_i, &n)) {
		return (itv_t *) trios_error(MSG, "Error in partition!");
	}
#ifdef DEBUG
	printf("Number of partitions %d\n", n);
#if defined(_OPENMP)	
        printf("OMP NUM PROC %d threads %d\n", omp_get_num_procs(),
	       omp_get_max_threads());
#endif 
#endif



#pragma omp parallel for
	for (i = 0; i < n; i++) {
		part_i[i] = solve_partition(part_m[i], part_i[i], i, win);
	}


#ifdef DEBUG
	printf("Finished ISI\n");
#endif
	acc = litvconcat_memory(part_i, n);

	free(part_m);
	free(part_i);

	return acc;
}
