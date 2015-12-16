#include "trios_win.h"
#include "trios_itv.h"
#include "trios_mtm.h"

#include "paclearn_local.h"
#include <stdio.h>
#include <stdlib.h>

/* #define _DEBUG_ */
/* #define _DEBUG_2_ */

int mtx_list_contain(mtx_t * mtx, unsigned int *wpat, int wzip);
int all_covered(itv_t * itv, mtm_t * mtm);
int all_covered2(itv_t * itv, mtx_t * mx_head, mtm_t * mtm);

void mtx_free(mtx_t * mp);

void mtx_free(mtx_t * mp)
{

	if (mp->I != NULL) {
		itv_nodebx_free(mp->I);
		mp->I = NULL;
	}
	if (mp->M != NULL) {
		free(mp->M);
		mp->M = NULL;
	}
	free(mp);

}

/*
     -------------------------------------------
     FUNCTION: pmincover
     -------------------------------------------
*/

int /*+ Purpose: compute the minimun cover of a set of minterms.+ */ pmincover(
										      mtm_t * mtm,	/*+ In: minterms                                    + */
										      itv_t * itv	/*+ In/Out: intervals/minimun cover                 + */
    )
/*+ Return: 1 on success, 0 on failure                                     +*/
{
	/* remove from itv all intervals that do not contain any example 
	   with label 1 */
	if (!itv_filter(mtm, itv)) {
		return trios_error(MSG, "pmincover: itv_filter() failed.");
	}

	/* compute a minimum cover */
	if (!p_cover(mtm, itv)) {
		return trios_error(MSG, "pmincover: p_cover() failed.");
	}

	return (1);
}

/*
     -------------------------------------------
     FUNCTION: p_cover
     -------------------------------------------
*/

int				/*+ Purpose: compute the minimun cover of a set of minterms.
				   At the end of the process, the non-essential intervals 
				   will be free                                            + */ p_cover(
													       mtm_t * mtm,	/*+ In: minterms                                    + */
													       itv_t * itv	/*+ In/Out: intervals/minimun cover                 + */
    )
/*+ Return: 1 on success, 0 on failure                                     +*/
{
	idx_ll *ll_head;
	mtx_t *mx_head, *mx;
	itv_BX *p_essen;
	int st, stop;

	/* create a list of indexes that point to positive minterms */
	if (!(ll_head = mtm_tbl2ll(mtm))) {
		return trios_error(MSG, "p_cover: mtm_tbl2ll() failed.");
	}

	/* create an auxiliar structure to manage intervals */
	if (!itv2mp(itv, &mx_head)) {
		return trios_error(MSG, "p_cover: itv2mp() failed.");
	}

	stop = 0;

	while (!stop) {

		/* select essential intervals */
		p_essen = essential_sel(&mx_head, mtm, &ll_head);

		if (p_essen) {
			if (!itv_add(itv, &p_essen)) {
				return trios_error(MSG,
						   "p_cover: itv_add() failed.");
			}
		}
#ifdef _DEBUG_
		if (!all_covered2(itv, mx_head, mtm)) {
			(void)trios_error(MSG,
					  "p_cover after essential_sel : OOOPPSSSS !! ");
		}
#endif

		if (ll_head) {

			/* eliminate dominated rows */
			if (!(st = del_dominated(&mx_head, mtm, ll_head))) {
				return trios_error(MSG,
						   "p_cover: del_dominated() failed.");
			}
#ifdef _DEBUG_
			if (!all_covered2(itv, mx_head, mtm)) {
				(void)trios_error(MSG,
						  "p_cover after del_dominated : OOOPPSSSS !! ");
			}
#endif

			if (st == 2)
				stop = 1;
		} else {
			stop = 1;
		}
	}

	if (ll_head) {
		/* find a cover of the resulting examples by a greedy criteria */
		p_essen = greedy_sel(&mx_head, mtm, ll_head);
		if (!p_essen) {
			return trios_error(MSG,
					   "p_cover: greedy_sel() failed.");
		}
		(void)itv_add(itv, &p_essen);

	}
	/* No need to release ll_head here, because they have been
	   released before (at this point, it is NULL) ******* */

	/* free other intervals */
	while (mx_head) {
		mx = mx_head->next;
		mtx_free(mx_head);
		/*
		   itv_nodebx_free(mx_head->I) ;
		   mx_head->I = NULL ;
		   if(mx_head->M != NULL) {
		   free(mx_head->M) ; mx_head->M = NULL ;
		   }
		   free(mx_head) ;
		 */
		mx_head = mx;
	}

#ifdef _DEBUG_
	if (!all_covered(itv, mtm)) {
		(void)trios_error(MSG, "p_cover After all : OOOPPSSSS !!");
	}
#endif

	return (1);
}

/*
     -------------------------------------------
     FUNCTION: mtm_tbl2ll
     -------------------------------------------
*/

idx_ll *			/*+ Purpose: create a list of indices pointing to the
				   examples of a set of classified examples      + */
mtm_tbl2ll(mtm_t * mtm		/*+ In: classified examples set                     + */
    )
/*+ Return: a pointer to a list of indices. If it is NULL, then either
     the examples set is empty or an error has occurred                    +*/
{

	idx_ll *p, *head;
	int i;

	head = NULL;

	for (i = mtm->nmtm - 1; i >= 0; i--) {
		p = (idx_ll *) malloc(sizeof(idx_ll));
		if (!p) {
			return (idx_ll *) trios_error(1,
						      "mtm_tbl2ll: memory allocation failed.");
		}
		p->idx = i;
		p->next = head;
		head = p;
	}

	return (head);
}

/*
     -------------------------------------------
     FUNCTION: itv2mp
     -------------------------------------------
*/

int /*+ Purpose: create auxiliar structure to hold intervals   + */ itv2mp(
										  itv_t * itv,	/*+ In/Out: set of intervals                         + */
										  mtx_t ** mx_head	/*+ In/Out: aux. structure pointing to the intervals + */
    )
/*+ Return: 1 on success, 0 on failure. The interval set will be empty     +*/
{
	mtx_t *mp, *mp_head;
	itv_BX **p_vector, *p, *p_next;
	int wsize, i, dim;

	/* Firstly, put the intervals in a temporary structure p_vector[]. */
	/* p_vector[i] points to the list containing all intervals of      */
	/* dimension i                                                     */

	wsize = itv_get_wsize(itv);

	if (!(p_vector = (itv_BX **) malloc((wsize + 1) * sizeof(itv_BX *)))) {
		return trios_error(1, "itv2mp: memory allocation failed.");
	}
	for (i = 0; i <= wsize; i++) {
		p_vector[i] = NULL;
	}

	p = (itv_BX *) itv->head;

	while (p != NULL) {
		dim = itv_dimension(p, size_of_zpat(wsize));
		p_next = p->next;
		p->next = p_vector[dim];
		p_vector[dim] = p;
		p = p_next;
	}

	itv->head = (int *)NULL;
	itv->nitv = 0;

	/* Second: These intervals will be put in a special list, where */
	/* the intervals with smaller dimension will be at the rear and */
	/* those with larger dimension will be at the front             */

	mp_head = NULL;

	for (i = 0; i <= wsize; i++) {

		p = p_vector[i];

		while (p != NULL) {
			p_next = p->next;

			if (!(mp = (mtx_t *) malloc(sizeof(mtx_t)))) {
				return trios_error(1,
						   "itv2mp: memory allocation failed.");
			}
			p->next = NULL;
			mp->I = p;
			mp->dim = i;
			mp->M = NULL;
			mp->next = mp_head;
			mp_head = mp;
			p = p_next;
		}
	}

	free(p_vector);

	*mx_head = mp_head;

	return (1);
}

/*
     -------------------------------------------
     FUNCTION: essential_sel
     -------------------------------------------
*/

itv_BX *			/*+ Purpose: select essential intervals                    + */
essential_sel(mtx_t ** mx_head,	/*+ In/Out: list of intervals                       + */
	      mtm_t * mtm,	/*+ In: classified examples set                     + */
	      idx_ll ** ll_head	/*+ In/Out: index for classified examples set       + */
    )
/*+ Return: a pointer to a list with essential intervals                   +*/
{
	mtx_t *mp_head, *mp, *mp_bef, *xp, *xp_bef;
	mtm_BX *q;
	idx_ll *p, *p_next;
	itv_BX *p_essen;
	int counter;
	int j, stop_flag;

	p = *ll_head;

	p_essen = NULL;
	mp_head = *mx_head;
	q = (mtm_BX *) mtm->mtm_data;

	/* For each column (examples), check if there is an essential interval */
	/* covering it                                                         */

	while (p != NULL) {

		p_next = p->next;
		mp = mp_head;
		mp_bef = NULL;
		counter = 0;

		/* Check wether the current example being analysed is covered only */
		/* by one interval. If so, this interval is essential              */

		xp = xp_bef = NULL;

		while ((mp != NULL) && (counter <= 1)) {
			stop_flag = 0;
			j = 0;
			while ((!stop_flag) && (j < size_of_zpat(mtm->wsize))) {
				if (((mp->I->A[j] & q[p->idx].wpat[j]) !=
				     mp->I->A[j])
				    || ((mp->I->B[j] & q[p->idx].wpat[j]) !=
					q[p->idx].wpat[j])) {
					stop_flag = 1;
				}
				j++;
			}
			if (!stop_flag) {	/* xp is the last node so far that  */
				xp_bef = mp_bef;	/* contains the current example     */
				xp = mp;
				counter++;
			}
			mp_bef = mp;
			mp = mp->next;
		}

		/* If there exists only one mark in the column corresponding to    */
		/* the example q[p->idx], then the interval pointed by xp is       */
		/* essential and it will be added to the list of essentials.       */
		/* After that, all classified examples (in the list pointed by p)  */
		/* that are covered by the interval are removed (that means that   */
		/* in the list pointed by p will remain those examples that        */
		/* are not covered by any of the selected intervals so far)        */

		if (counter == 1) {

#ifdef _DEBUG_2_
			trios_debug("essen: interval selected A=%x B=%x",
				    xp->I->A[0], xp->I->B[0]);
#endif

			if (xp == mp_head) {
				mp_head = mp_head->next;
				xp->I->next = p_essen;
				p_essen = xp->I;
			} else {
				xp_bef->next = xp->next;
				xp->I->next = p_essen;
				p_essen = xp->I;
			}
			xp->I = NULL;

			delete_mtm(mtm, p, ll_head, p_essen);
			p_next = *ll_head;
			mtx_free(xp);
		} else if (counter == 0) {
			trios_fatal
			    ("\nessential_sel: \n*******\nUNEXPECTED ERROR!! error code 120\n ******");
		}

		p = p_next;
	}

	*mx_head = mp_head;
	return (p_essen);
}

/*
     -------------------------------------------
     FUNCTION: delete_mtm
     -------------------------------------------
*/

void				/*+ Purpose: delete from the list of indices, those that
				   are pointing to examples that are contained in a given
				   interval I                                             + */ delete_mtm(
														 mtm_t * mtm,	/*+ In: set of classified examples                  + */
														 idx_ll * p,	/*+ In: a element in the list of indices            + */
														 idx_ll ** ll_head,	/*+ In/Out: the list of indices                     + */
														 itv_BX * I	/*+ In: an interval                                 + */
    )
/*+ Return: nothing                                                        +*/
{
	idx_ll *cur, *curbef;
	mtm_BX *q;
	int j, stop_flag;

	/* Elimina da lista ll_head todos os elementos que sao cobertos pelo  */
	/* intervalo I. Devolve a proxima coluna a ser eliminada              */

	cur = *ll_head;
	curbef = NULL;
	q = (mtm_BX *) mtm->mtm_data;

	while (cur) {

		stop_flag = 0;

		j = 0;
		while ((!stop_flag) && (j < size_of_zpat(mtm->wsize))) {
			if (((I->A[j] & q[cur->idx].wpat[j]) != I->A[j]) ||
			    ((I->B[j] & q[cur->idx].wpat[j]) !=
			     q[cur->idx].wpat[j])) {
				stop_flag = 1;
			}
			j++;
		}

		if (stop_flag == 0) {	/* exemplo q[cur->idx] esta' contido em I */
			if (curbef == NULL) {
				*ll_head = cur->next;
				free(cur);
				cur = *ll_head;
			} else {
				curbef->next = cur->next;
				free(cur);
				cur = curbef->next;
			}
		} else {
			curbef = cur;
			cur = cur->next;
		}
	}
}

/*
     -------------------------------------------
     FUNCTION: itv_add
     -------------------------------------------
*/

int				/*+ Purpose: add a list of intervals to a set of intervals
				   (does not check anything, does not copy).           + */ itv_add(
													   itv_t * itv,	/*+ In/Out: set of intervals                        + */
													   itv_BX ** p_essen	/*+ In/Out: list of intervals to be added           + */
    )
/*+ Return: 1 on success, 0 on failure                                     +*/
{
	itv_BX *p;
	int count;

	p = *p_essen;

	if (p) {
		count = 1;
		while (p->next) {
			count++;
			p = p->next;
		}
		p->next = (itv_BX *) itv->head;
		itv->head = (int *)*p_essen;
		itv->nitv = itv->nitv + count;
		*p_essen = NULL;

	}

	return (1);
}

/*
     -------------------------------------------
     FUNCTION: del_dominated
     -------------------------------------------
*/

int				/*+ Purpose: delete dominated rows of the prime implicant
				   table                                         + */ del_dominated(
													   mtx_t ** mx_head,	/*+ In/Out: list pointing to the intervals          + */
													   mtm_t * mtm,	/*+ In: set of classified examples                  + */
													   idx_ll * ll_head	/*+ In: list of indices                             + */
    )
/*+ Return: 0 on failure, 2 if there is no modification and 1 if some rows
    were deleted                                                           +*/
{
	mtx_t *m, *m1, *ant, *ant1;
	int cob_type, rowsize, i, flag, retcode;

	m = *mx_head;
	if (!set_matrix(m, mtm, ll_head)) {
		return trios_error(MSG, "del_dominated: set_matrix() failed.");
	}

	rowsize = (mtm->nmtm + 31) / NB;

	ant = NULL;
	retcode = 2;

	while (m != NULL) {

		m1 = m->next;
		ant1 = m;
		cob_type = 3;

		while (m1 != NULL) {

			cob_type = 3;

			if (m->dim > m1->dim) {
				flag = 0;
				i = 0;
				while ((flag == 0) && (i < rowsize)) {
					if ((m->M[i] & m1->M[i]) != m1->M[i]) {
						flag = 1;
					}
					i++;
				}
				if (flag == 1) {
					cob_type = 3;
				} else {
					cob_type = 1;
				}
			} else if (m->dim == m1->dim) {
				flag = 0;
				i = 0;
				while ((flag != 3) && (i < rowsize)) {
					if (m->M[i] != m1->M[i]) {
						if ((m->M[i] & m1->M[i]) ==
						    m1->M[i]) {
							flag = flag | 1;
						} else if ((m->M[i] & m1->M[i])
							   == m->M[i]) {
							flag = flag | 2;
						} else {
							flag = 3;
						}
					}
					i++;
				}
				if (flag == 0) {
					cob_type = 1;
				} else {
					cob_type = flag;
				}
			} else {
				flag = 0;
				i = 0;
				while ((flag == 0) && (i < rowsize)) {
					if ((m1->M[i] & m->M[i]) != m->M[i]) {
						flag = 1;
					}
					i++;
				}
				if (flag == 1) {
					cob_type = 3;
				} else {
					cob_type = 2;
				}
			}

			if (cob_type == 1) {
				ant1->next = m1->next;
				mtx_free(m1);
				/*
				   itv_nodebx_free(m1->I) ;
				   free(m1->M) ; m1->M = NULL ;
				   m1->I = NULL ;
				   free(m1) ;
				 */
				m1 = ant1->next;
				retcode = 1;
			} else if (cob_type == 2) {
				if (ant == NULL) {
					*mx_head = m->next;
					mtx_free(m);
					/*
					   itv_nodebx_free(m->I) ;
					   free(m->M) ;   m->M = NULL ;
					   m->I = NULL ;
					   free(m) ;
					 */
					m = *mx_head;
				} else {
					ant->next = m->next;
					mtx_free(m);
					/*
					   itv_nodebx_free(m->I) ;
					   free(m->M) ;   m->M = NULL ;
					   m->I = NULL ;
					   free(m) ;
					 */
					m = ant->next;
				}
				m1 = NULL;
				retcode = 1;
			} else {
				ant1 = ant1->next;
				m1 = m1->next;
			}
		}
		if (cob_type != 2) {
			ant = m;
			m = m->next;
		}
	}

	return (retcode);
}

/*
     -------------------------------------------
     FUNCTION: set_matrix
     -------------------------------------------
*/

int				/*+ Purpose: given pointers for intervals and examples,
				   create the prime implicant table that will be
				   used for removing the dominated rows (intervals)       + */ set_matrix(
														 mtx_t * mx_head,	/*+ In/Out: list pointing to some intervals         + */
														 mtm_t * mtm,	/*+ In: set of classified examples                  + */
														 idx_ll * ll_head	/*+ In: list of indices                             + */
    )
/*+ Return: 1 on success, 0 on failure                                     +*/
{
	mtx_t *m;
	mtm_BX *q;
	idx_ll *p;
	int i, rowsize, iwzip, shift;
	int j, stop_flag;

	rowsize = (mtm->nmtm + 31) / NB;	/* in the matrix to be built, each example
						   (column) will be represented by a pixel  */

	m = mx_head;
	while (m != NULL) {
		if (m->M != NULL) {
			free(m->M);
			m->M = NULL;
		}
		m->M = (int *)malloc(sizeof(int) * rowsize);
		if (m->M == NULL) {
			return trios_error(1,
					   "set_matrix: memory allocation error.");
		}
		for (i = 0; i < rowsize; i++) {
			m->M[i] = 0;
		}
		m = m->next;
	}

	i = 0;
	q = (mtm_BX *) mtm->mtm_data;
	p = ll_head;

	while (p != NULL) {
		m = mx_head;
		iwzip = i / NB;
		shift = i % NB;
		while (m != NULL) {
			stop_flag = 0;
			j = 0;

			while ((!stop_flag) && (j < size_of_zpat(mtm->wsize))) {
				if (((m->I->A[j] & q[p->idx].wpat[j]) !=
				     m->I->A[j])
				    || ((m->I->B[j] & q[p->idx].wpat[j]) !=
					q[p->idx].wpat[j])) {
					stop_flag = 1;
				}
				j++;
			}

			if (!stop_flag) {
				m->M[iwzip] = (m->M[iwzip] | bitmsk[shift]);
			}
			m = m->next;
		}
		i++;
		p = p->next;
	}

	return (1);
}

/*
     -------------------------------------------
     FUNCTION: greedy_sel
     -------------------------------------------
*/

itv_BX *			/*+ Purpose: select a cover by the greedy method           + */
greedy_sel(mtx_t ** mx_head,	/*+ In/Out: list pointing to the intervals          + */
	   mtm_t * mtm,		/*+ In: set of classified examples                  + */
	   idx_ll * ll_head	/*+ In: list of indices                             + */
    )
/*+ Return: the selected subset of intervals                               +*/
{
	itv_BX *essen;
	mtx_t *mp, *last;
	mtm_BX *q;
	idx_ll *x, *cur;
	int j, stop_flag;
	int flag;

	q = (mtm_BX *) mtm->mtm_data;
	cur = ll_head;
	essen = NULL;

	while (cur != NULL) {

		x = cur;
		cur = cur->next;

		last = NULL;
		mp = *mx_head;
		flag = 0;

		/* find first interval that covers current example cur */
		while ((!flag) && (mp)) {

			stop_flag = 0;
			j = 0;

			while ((!stop_flag) && (j < size_of_zpat(mtm->wsize))) {
				if (((mp->I->A[j] & q[x->idx].wpat[j]) !=
				     mp->I->A[j])
				    || ((mp->I->B[j] & q[x->idx].wpat[j]) !=
					q[x->idx].wpat[j])) {
					stop_flag = 1;
				}
				j++;
			}
			if (!stop_flag) {
				flag = 1;
			} else {
				last = mp;
				mp = mp->next;
			}
		}

		free(x);

		if (mp == NULL) {
			trios_fatal
			    ("greedy_sel\n *******\nUNEXPECTED ERROR\n********");
		}
#ifdef _DEBUG_
		trios_debug("greedy_sel: interval selected A=%x B=%x",
			    mp->I->A[0], mp->I->B[0]);
#endif

		mp->I->next = essen;
		essen = mp->I;
		mp->I = NULL;
		if (last == NULL) {
			*mx_head = mp->next;
		} else {
			last->next = mp->next;
		}
		/* free(mp) ; */
		mtx_free(mp);

		if (!del_covered(mtm, &cur, essen)) {
			return (itv_BX *) trios_error(MSG,
						      "greedy_sel: del_covered() failed.");
		}

	}

	return (essen);
}

/*
     -------------------------------------------
     FUNCTION: del_covered
     -------------------------------------------
*/

int				/*+ Purpose: delete, from a list of indices, those that
				   are pointing to examples that are contained in a given
				   interval I (this routine is similar to del_mtm, the
				   difference is the returning value)                      + */ del_covered(
														   mtm_t * mtm,	/*+ In: set of classified examples                  + */
														   idx_ll ** head,	/*+ In/Out: list of indices                         + */
														   itv_BX * I	/*+ In: interval                                    + */
    )
/*+ Return: 1 on success, 0 on failure                                     +*/
{
	idx_ll *cur, *curbef, *px;
	mtm_BX *q;
	int j, stop_flag;

	/* Elimina da lista head todos os elementos que sao cobertos pelo */
	/* intervalo I. */

	px = NULL;
	cur = *head;
	curbef = NULL;
	q = (mtm_BX *) mtm->mtm_data;

	while (cur) {

		stop_flag = 0;

		j = 0;
		while ((!stop_flag) && (j < size_of_zpat(mtm->wsize))) {
			if (((I->A[j] & q[cur->idx].wpat[j]) != I->A[j]) ||
			    ((I->B[j] & q[cur->idx].wpat[j]) !=
			     q[cur->idx].wpat[j])) {
				stop_flag = 1;
			}
			j++;
		}

		if (stop_flag == 0) {	/* exemplo q[cur->idx] esta' contido em I */
			if (curbef == NULL) {
				*head = cur->next;
				free(cur);
				cur = *head;
			} else {
				curbef->next = cur->next;
				free(cur);
				cur = curbef->next;
			}
		} else {
			curbef = cur;
			cur = cur->next;
		}
	}

	return (1);
}

/*
     -------------------------------------------
     FUNCTION: itv_cover_wpat
     -------------------------------------------
*/

int				/*+ Purpose: check if some interval of a list contains a
				   w-pattern                                     + */ itv_cover_wpat(
													    itv_BX * p_list,	/*+ In: the pointer to the list of intervals      + */
													    unsigned int *wpat,	/*+ In: the w-pattern                             + */
													    int wzip	/*+ In: compacted w-pattern's size                + */
    )
/*+ Return: 1 if wpat in some interval, 0 otherwise                        +*/
{
	int label;
	int i;
	itv_BX *p;

	p = p_list;
	label = -1;

	while (p && (label == -1)) {

		label = p->label;

		for (i = 0; i < wzip; i++) {
			if ((wpat[i] & p->A[i]) != p->A[i]) {
				label = -1;
				break;
			}
			if ((wpat[i] & p->B[i]) != wpat[i]) {
				label = -1;
				break;
			}
		}
		p = p->next;
	}

	if (label != -1) {
		return (1);
	} else {
		return (0);
	}
}

/*
     -------------------------------------------
     FUNCTION: mtx_list_contain
     -------------------------------------------
*/

int				/*+ Purpose: check if some interval of a list contains a
				   w-pattern, and returns the respective label   + */ mtx_list_contain(
													      mtx_t * mtx,	/*+ In: the pointer to the list of intervals        + */
													      unsigned int *wpat,	/*+ In: the w-pattern                            + */
													      int wzip	/*+ In: compacted w-pattern's size                  + */
    )
/*+ Return: 1 if wpat in some interval, 0 otherwise                        +*/
{
	int label;
	int i;
	mtx_t *p;

	p = mtx;
	label = -1;

	while (p && (label == -1)) {

		label = p->I->label;

		for (i = 0; i < wzip; i++) {
			if ((wpat[i] & p->I->A[i]) != p->I->A[i]) {
				label = -1;
				break;
			}
			if ((wpat[i] & p->I->B[i]) != wpat[i]) {
				label = -1;
				break;
			}
		}
		p = p->next;
	}

	if (label != -1) {
		return (1);
	} else {
		return (0);
	}
}

/*
     -------------------------------------------
     FUNCTION: all_covered
     -------------------------------------------
*/

int /*+ Purpose: check wether all examples of a set are covered + */ all_covered(
											itv_t * itv,	/*+ In: the pointer to the list of intervals        + */
											mtm_t * mtm	/*+ In: set of classified examples                  + */
    )
/*+ Return: 1 if all of them are covered, 0 otherwise                      +*/
{
	mtm_BX *q;
	int wzip, i;

	wzip = size_of_zpat(itv_get_wsize(itv));

	q = (mtm_BX *) mtm->mtm_data;

	for (i = 0; i < mtm_get_nmtm(mtm); i++) {
		if (!itv_cover_wpat((itv_BX *) itv->head, q[i].wpat, wzip)) {
			return (0);
		}
	}

	return (1);

}

/*
     -------------------------------------------
     FUNCTION: all_covered2
     -------------------------------------------
*/

int /*+ Purpose: check wether all examples of a set are covered + */
all_covered2(
			   itv_t * itv,	/*+ In: the pointer to the list of intervals        + */
			   mtx_t * mx_head,	/*+ In/Out: list pointing to the intervals          + */
			   mtm_t * mtm	/*+ In: set of classified examples                  + */
    )
/*+ Return: 1 if all of them are covered, 0 otherwise                      +*/
{
	mtm_BX *q;
	int wzip, i;

	wzip = size_of_zpat(itv_get_wsize(itv));

	q = (mtm_BX *) mtm->mtm_data;

	for (i = 0; i < mtm_get_nmtm(mtm); i++) {
		if (!itv_cover_wpat((itv_BX *) itv->head, q[i].wpat, wzip)) {
			if (!mtx_list_contain(mx_head, q[i].wpat, wzip)) {
				return (0);
			}
		}
	}

	return (1);

}
