#include "trios_win.h"
#include "trios_mtm.h"
#include "trios_itv.h"
#include "trios_misc.h"

 /*#define _DEBUG_ */

/*
     -------------------------------------------
     FUNCTION: itv_get_nitv
     -------------------------------------------
*/

int /*+ Purpose: give the number of intervals in a set of intervals  + */
itv_get_nitv(
			   itv_t * itv	/*+ In: pointer to the ITV structure                   + */
    )
/*+ Return: a  positive integer                                            +*/
{
	if (itv != NULL) {
		return (itv->nitv);
	} else {
		return trios_error(1, "Internal error.");
	}
}

/*
     -------------------------------------------
     FUNCTION: itv_get_wsize
     -------------------------------------------
*/

int /*+ Purpose: give the size of w-pattern                     + */
itv_get_wsize(
			    itv_t * itv	/*+ In: pointer to the ITV structure           + */
    )
/*+ Return: a positive integer                                             +*/
{
	return (itv->wsize);
}

/*
     -------------------------------------------
     FUNCTION: itv_get_head
     -------------------------------------------
*/

int				/*+ Purpose: give the pointer to the header of the list
				   of intervals                                   + */
*itv_get_head(itv_t * itv	/*+ In: pointer to the ITV structure         + */
    )
/*+ Return: a pointer to integer (it must be converted to the adequate
    type to be used)                                                       +*/
{
	return (itv->head);
}

/*
     -------------------------------------------
     FUNCTION: itv_get_deflabel
     -------------------------------------------
*/

int /*+ Purpose: gives the default label associated to the intervals set + */
itv_get_deflabel(
			       itv_t * itv	/*+ In: pointer to the ITV structure               + */
    )
/*+ Return: an integer (default label)                                     +*/
{
	return (itv->deflabel);
}

/*
     -------------------------------------------
     FUNCTION: itv_get_type
     -------------------------------------------
*/

int /*+ Purpose: give the type : BB, BG, GB or GG                 + */
itv_get_type(
			   itv_t * itv	/*+ In: pointer to the ITV structure               + */
    )
/*+ Return: an integer                                                     +*/
{
	return (itv->type);
}

/*
     -------------------------------------------
     FUNCTION: itv_get_wzip
     -------------------------------------------
*/

int /*+ Purpose: give  the compacted w-pattern size             + */
itv_get_wzip(
			   itv_t * itv	/*+ In: pointer to the ITV structure                 + */
    )
/*+ Return: a positive integer                                             +*/
{
	return (itv->wzip);
}

/*
     -------------------------------------------
     FUNCTION: itv_get_maxlabel
     -------------------------------------------
*/

int /*+ Purpose: gives the maximum label found in a set of intervals + */
itv_get_maxlabel(
			       itv_t * itv	/*+ In: pointer to the ITV structure                 + */
    )
/*+ Return: a positive integer (maximum label)                             +*/
{
	return (itv->maxlabel);
}

/*
     -------------------------------------------
     FUNCTION: itv_set_nitv
     -------------------------------------------
*/

void /*+ Purpose: set the number of intervals in a ITV structure      + */
itv_set_nitv(
			   itv_t * itv,	/*+ In/Out: pointer to the ITV structure               + */
			   int nitv	/*+ In: label to be set as the number of intervals     + */
    )
/*+ Return: nothing                                                        +*/
{
	itv->nitv = nitv;
}

/*
     -------------------------------------------
     FUNCTION: itv_set_wsize
     -------------------------------------------
*/

void /*+ Purpose: set the w-pattern size of an intervals set    + */
itv_set_wsize(
			    itv_t * itv,	/*+ In/Out: pointer to intervals set               + */
			    int wsize	/*+ In: w-pattern's size to be set                 + */
    )
/*+ Return: nothing                                                        +*/
{
	itv->wsize = wsize;
}

/*
     -------------------------------------------
     FUNCTION: itv_set_head
     -------------------------------------------
*/

void /*+ Purpose: set the head of the intervals list of an intervals set + */
itv_set_head(
			   itv_t * itv,	/*+ In/Out: intervals set                    + */
			   int *head	/*+ In: pointer to intervals list head       + */
    )
/*+ Return: nothing                                                        +*/
{
	itv->head = head;
}

/*
     -------------------------------------------
     FUNCTION: itv_set_deflabel
     -------------------------------------------
*/

void /*+ Purpose: set the default label in an intervals set    + */
itv_set_deflabel(
			       itv_t * itv,	/*+ In/Out: intervals set                 + */
			       int deflabel	/*+ In: default label                     + */
    )
/*+ Return: nothing                                                        +*/
{
	itv->deflabel = deflabel;
}

/*
     -------------------------------------------
     FUNCTION: itv_set_type
     -------------------------------------------
*/

void /*+ Purpose: set the type in an intervals set                  + */
itv_set_type(
			   itv_t * itv,	/*+ In/Out: intervals set                             + */
			   int type	/*+ In: type to be set (BB, BG, GB or GG)             + */
    )
/*+ Return: nothing                                                        +*/
{
	itv->type = type;
}

/*
     -------------------------------------------
     FUNCTION: itv_set_wzip
     -------------------------------------------
*/

void /*+ Purpose: set the compacted w-pattern's size of an intervals set + */
itv_set_wzip(
			   itv_t * itv,	/*+ In/Out: intervals set                         + */
			   int wzip	/*+ In: compacted w-pattern's size                + */
    )
/*+ Return: nothing                                                        +*/
{
	itv->wzip = wzip;
}

/*
     -------------------------------------------
     FUNCTION: itv_set_maxlabel
     -------------------------------------------
*/

void /*+ Purpose: set the maximum label of a set of intervals    + */
itv_set_maxlabel(
			       itv_t * itv,	/*+ In/Out: pointer to the ITV structure             + */
			       int maxlabel	/*+ In: maximum label                                + */
    )
/*+ Return: a positive integer (maximum label)                             +*/
{
	itv->maxlabel = maxlabel;

}

/*
     -------------------------------------------
     FUNCTION: itv_create
     -------------------------------------------
*/

itv_t *				/*+ Purpose: create a ITV structure                          + */
itv_create(int wsize,		/*+ In: w-pattern size                                 + */
	   int type,		/*+ In: type (BB, BG, GB or GG)                        + */
	   int deflabel		/*+ In: default label to be assigned to the ITV set    + */
    )
/*+ Return: a pointer to a ITV structure on success, 0 on failure          +*/
{
	itv_t *itv;
	int wzip;

	itv = (itv_t *) malloc(sizeof(itv_t));
	if (itv == NULL) {
		return (itv_t *) trios_error(1, "Memory allocation failed.");
	}

	itv_set_wsize(itv, wsize);

	wzip = size_of_zpat(wsize);

	itv_set_wzip(itv, wzip);

	itv_set_head(itv, NULL);

	itv_set_nitv(itv, 0);

	itv_set_deflabel(itv, deflabel);

	itv_set_maxlabel(itv, 0);

	itv_set_type(itv, type);

	return (itv);

}

/*
     -------------------------------------------
     FUNCTION: itv_free
     -------------------------------------------
*/

void /*+ Purpose: free memory area used by a ITV structure        + */ itv_free(
										       itv_t * itv	/*+ In: intervals set                          + */
    )
/*+ Return: nothing                                                        +*/
{
	if (itv != NULL) {
		itv_list_free(itv_get_head(itv), itv_get_type(itv));
	}
	free(itv);
}

/*
     -------------------------------------------
     FUNCTION: itv_list_free
     -------------------------------------------
*/

void /*+ Purpose: free a intervals list                         + */
itv_list_free(
			    int *head,	/*+ In: head of intervals list                       + */
			    int type	/*+ In: type (BB, BG or GG)                          + */
    )
/*+ Return: nothing                                                        +*/
{
	itv_BX *local_head, *p;
	itv_GX *local_head_gx, *q;

	if ((type == BB) || (type == BG)) {
		local_head = (itv_BX *) head;
		while (local_head) {
			p = local_head->next;
			itv_nodebx_free(local_head);
			local_head = p;
		}
	}

	if (type == GG) {
		local_head_gx = (itv_GX *) head;
		while (local_head_gx) {
			q = local_head_gx->next;
			itv_nodegx_free(local_head_gx);
			local_head_gx = q;
		}
	}

}

/*
     -------------------------------------------
     FUNCTION: itv_nodebx_free
     -------------------------------------------
*/

void /*+ Purpose: free memory area used by an interval of BX type     + */
itv_nodebx_free(
			      itv_BX * p	/*+ In: pointer to the interval               + */
    )
/*+ Return: nothing                                                        +*/
{
	free(p->A);
	free(p->B);
	free(p);
}

itv_GX *			/*+ Purpose: create an interval of GX type              + */
itv_nodegx_create(int wsize	/*+ In: size of a w-pattern                   + */
    )
/*+ Return: pointer to an interval of GX type                              +*/
{
/* author: Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)                  */
/* date: Fri Jul 14 2000                                                    */

	itv_GX *p;

	p = (itv_GX *) malloc(sizeof(itv_GX));
	if (p == NULL) {
		return (itv_GX *) trios_error(1, "Memory allocation failed.");
	}

	p->A = (char *)malloc(sizeof(char) * wsize);
	if (p->A == NULL) {
		free(p);
		return (itv_GX *) trios_error(1, "Memory allocation failed.");
	}

	p->B = (char *)malloc(sizeof(char) * wsize);
	if (p->B == NULL) {
		free(p->A);
		free(p);
		return (itv_GX *) trios_error(1, "Memory allocation failed.");
	}

	p->plabels = NULL;
	p->label = 0;
	p->size = 0;		/* added on Oct 31, 2000 - Nina */
	p->next = NULL;
	p->nindexes = 0;
	return (p);

}

itv_BX *			/*+ Purpose: create an interval of BX type              + */
itv_nodebx_create(int wzip	/*+ In: compacted size of w-pattern           + */
    )
/*+ Return: pointer to an interval of BX type                              +*/
{
	itv_BX *p;

	p = (itv_BX *) malloc(sizeof(itv_BX));
	if (p == NULL) {
		return (itv_BX *) trios_error(1, "Memory allocation failed.");
	}

	p->A = (unsigned int *)malloc(sizeof(int) * wzip);
	if (p->A == NULL) {
		free(p);
		return (itv_BX *) trios_error(1, "Memory allocation failed.");
	}

	p->B = (unsigned int *)malloc(sizeof(int) * wzip);
	if (p->B == NULL) {
		free(p->A);
		free(p);
		return (itv_BX *) trios_error(1, "Memory allocation failed.");
	}

	p->label = 0;
	p->next = NULL;

	return (p);

}

void /*+ Purpose: free memory area used by an interval of GX type     + */
itv_nodegx_free(
			      itv_GX * p	/*+ In: pointer to the interval               + */
    )
/*+ Return: nothing                                                        +*/
{
/* author: Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)                  */
/* date: Fri Jul 14 2000                                                    */

	free(p->A);
	free(p->B);
	if (p->plabels) {
		free(p->plabels);
	}
	free(p);
}

/*
     -------------------------------------------
     FUNCTION: itvbx_set
     -------------------------------------------
*/

void /*+ Purpose: set some data  of an interval                        + */
itvbx_set(
			itv_BX * p,	/*+ In/Out: pointer to the interval                 + */
			unsigned int *A,	/*+ In: left extremity of the interval              + */
			unsigned int *B,	/*+ In: right extremity of the interval             + */
			int wzip,	/*+ In: compacted size of w-pattern                 + */
			int label,	/*+ In: label assigned to the interval              + */
			itv_BX * next	/*+ In: if the interval is linked in a list of
					   intervals, this must be the pointer to the
					   next interval                               + */
    )
/*+ Return: nothing                                                        +*/
{
	int i;

	for (i = 0; i < wzip; i++) {
		p->A[i] = A[i];
		p->B[i] = B[i];
	}

	p->label = label;
	p->next = next;

}

/*
     -------------------------------------------
     FUNCTION: itv_dimension
     -------------------------------------------
*/

int /*+ Purpose: compute the dimension of an interval          + */
itv_dimension(
			    itv_BX * p,	/*+ In: the interval                                + */
			    int wzip	/*+ In: compacted w-pattern's size                  + */
    )
/*+ Return: an integer between 0 and n (where n stands for wsize)          +*/
{
	int dim;
	int Xor;
	int i, j;

	dim = 0;

	for (i = 0; i < wzip; i++) {

		Xor = (p->A[i] ^ p->B[i]);

		for (j = 0; j < NB; j++) {
			if (Xor & bitmsk[j]) {
				dim++;
			}
		}
	}

	return (dim);
}

/*
     -------------------------------------------
     FUNCTION: itv_equal
     -------------------------------------------
*/

int /*+ Purpose: check if two intervals are identical          + */ itv_equal(
										     itv_BX * itv1,	/*+ In: first interval                              + */
										     itv_BX * itv2,	/*+ In: second interval                             + */
										     int wzip	/*+ In: compacted w-pattern's size                  + */
    )
/*+ Return: 1 if they are equal, 0 otherwise                               +*/
{

	int i;

	for (i = 0; i < wzip; i++) {
		if ((itv1->A[i] != itv2->A[i]) || (itv1->B[i] != itv2->B[i])) {
			return (0);
		}
	}

	return (1);

}

/*
     -------------------------------------------
     FUNCTION: itv_and
     -------------------------------------------
*/

itv_BX *			/*+ Purpose: compute  the intersection of two intervals    + */
itv_and(itv_BX * itv1,		/*+ In: fisrt interval                              + */
	itv_BX * itv2,		/*+ In: second interval                             + */
	int wzip		/*+ In: compacted w-pattern's size                  + */
    )
/*+ Return: pointer to the resulting interval (NULL if the interval is empty),
    -1 on failure                                                          +*/
{
	itv_BX *itv;
	int i;

	itv = itv_nodebx_create(wzip);
	if (!itv) {
		(void)trios_error(MSG, "itv_and: itv_nodebx_create() failed.");
		return ((itv_BX *) (-1));
	}

	for (i = 0; i < wzip; i++) {
		itv->A[i] = (itv1->A[i] | itv2->A[i]);
		itv->B[i] = (itv1->B[i] & itv2->B[i]);
		if ((itv->A[i] & itv->B[i]) != itv->A[i]) {
			itv_nodebx_free(itv);
			return (NULL);	/* resulting interval is empty */
		}
	}

	return (itv);
}

/*
     -------------------------------------------
     FUNCTION: itv_group
     -------------------------------------------
*/

itv_BX *			/*+ Purpose: group two adjacent intervals                  + */
itv_group(itv_BX * itv1,	/*+ In: first interval                              + */
	  itv_BX * itv2,	/*+ In: second interval                             + */
	  int wzip		/*+ In: compacted w-pattern's size                  + */
    )
/*+ Return: a pointer to an interval on success, NULL on failure           +*/
{

	itv_BX *itv;
	int j;

	itv = itv_nodebx_create(wzip);
	if (!itv) {
		return (itv_BX *) trios_error(MSG,
					      "itv_group: itv_nodebx_create() failed.");
	}

	for (j = 0; j < wzip; j++) {
		itv->A[j] = (itv1->A[j] & itv2->A[j]);
		itv->B[j] = (itv1->B[j] | itv2->B[j]);
	}
	itv->label = 0;

	return (itv);
}

/*
     -------------------------------------------
     FUNCTION: itv_select
     -------------------------------------------
*/

itv_t *				/*+ Purpose: separate all intervals, with a given label, from
				   a set of intervals                            + */
itv_select(itv_t * itv,		/*+ In/Out: interval set                            + */
	   int label		/*+ In: the label                                   + */
    )
/*+ Return: a pointer to an interval set (a subset of the input set,
            containing only intervals with the specified label) on success,
            NULL on failure                                                +*/
{
	itv_t *itv_new;
	itv_BX *p1, *tmp;
	/*itv_GX *pgx, *tmp_gx ; */
	int type;

	type = itv_get_type(itv);

	/* create a structure to hold the subset to be separated */
	if (!(itv_new = itv_create(itv->wsize, itv->type, 0))) {
		return (itv_t *) trios_error(MSG,
					     "itv_select: itv_create() failed.");
	}

	if ((type == BB) || (type == BG)) {

		p1 = (itv_BX *) itv->head;
		itv->head = (int *)NULL;

		while (p1) {
			tmp = p1->next;
			if (p1->label == label) {
				p1->next = (itv_BX *) itv_new->head;
				itv_new->head = (int *)p1;
				itv_new->nitv++;
				itv->nitv--;
			} else {
				p1->next = (itv_BX *) itv->head;
				itv->head = (int *)p1;
			}
			p1 = tmp;
		}

	}

	if ((type == GG) || (type == WKC) || (type == WKF)) {
		return (itv_t *) trios_error(MSG,
					     "itv_read: type GG not implemented yet");
		/*pgx = (itv_GX *)itv->head ;
		   itv->head = (int *)NULL ;

		   while(pgx) {
		   // TODO: maybe it should be 'pgx' instead of 'p1'
		   tmp_gx = p1->next;
		   if(pgx->label == label) {
		   pgx->next = (itv_GX *)itv_new->head ;
		   itv_new->head = (int *) pgx ;
		   itv_new->nitv++ ;
		   itv->nitv-- ;
		   }
		   else {
		   pgx->next = (itv_GX *)itv->head ;
		   itv->head = (int *)pgx ;
		   }
		   // TODO: maybe it should be 'tmp_gx' instead pf 'tmp'
		   pgx = tmp ;
		   } */

	}

	return (itv_new);	/* the set to be returned can be empty */

}

/*
     -------------------------------------------
     FUNCTION: itv_sort
     -------------------------------------------
*/

int /*+ Purpose: sort intervals according to its dimension + */ itv_sort(
										itv_t * itv	/*+ In/Out: intervals structure                        + */
    )
/*+ Return: 1 on success, 0 on failure                                 +*/
{
	itv_BX **p_vector;
	itv_BX *p, *tmp;
	int i, dim, wsize, wzip;

	wsize = itv_get_wsize(itv);
	wzip = size_of_zpat(wsize);

	/* create an auxiliar structure */
	if (!(p_vector = (itv_BX **) malloc((wsize + 1) * sizeof(itv_BX *)))) {
		return trios_error(1, "Memory allocation failed.");
	}

	for (i = 0; i <= wsize; i++) {
		p_vector[i] = NULL;
	}

	p = (itv_BX *) itv->head;

	/* compute the dimension of each interval and group them according
	   to the same dimension                                           */
	while (p != NULL) {
		dim = itv_dimension(p, wzip);
		tmp = p->next;
		p->next = p_vector[dim];
		p_vector[dim] = p;
		p = tmp;
	}

	itv_set_head(itv, NULL);

	/* reorganize the intervals list putting that have lower dimension
	   at the end of the list and so on.                               */
	for (i = 0; i <= wsize; i++) {

		p = p_vector[i];

		if (p) {
			while (p->next != NULL) {
				p = p->next;
			}
			p->next = (itv_BX *) itv->head;
			itv->head = (int *)p_vector[i];
		}

	}

	return (1);
}

/*
     -------------------------------------------
     FUNCTION: itv_contain
     -------------------------------------------
*/

int /*+ Purpose: check if an interval contains a w-pattern     + */ itv_contain(
										       itv_BX * p_itv,	/*+ In: the pointer to the interval               + */
										       unsigned int *wpat,	/*+ In: the w-pattern                             + */
										       int wzip	/*+ In: compacted w-pattern's size                + */
    )
/*+ Return: 1 if the w-pattern is contained in the interval, 0 otherwise   +*/
{
	int i;

	for (i = 0; i < wzip; i++) {

		if ((wpat[i] & p_itv->A[i]) != p_itv->A[i]) {
			return (0);
		}
		if ((wpat[i] & p_itv->B[i]) != wpat[i]) {
			return (0);
		}
	}

	return (1);
}

/*
     -------------------------------------------
     FUNCTION: itv_label
     -------------------------------------------
*/

int				/*+ Purpose: given an interval set and a label "old_label",
				   changes the label of the intervals set
				   with label "old_label" by "new_label"         + */ itv_label(
												       itv_t * itv,	/*+ In/Out: intervals set                           + */
												       int old_label,	/*+ In: old label                                   + */
												       int new_label	/*+ In: new label                                   + */
    )
/*+ Return: 1 on success, 0 on failure                                     +*/
{

	int type;

	type = itv_get_type(itv);

	switch (type) {
	case BB:
	case BG:
		{
			itv_BX *p;

			p = (itv_BX *) itv->head;

			while (p) {
				if (p->label == old_label) {
					p->label = new_label;
				}
				p = p->next;
			}
		}
		break;

	case GG:
	case WKC:
	case WKF:{
			return trios_error(MSG, "Operator not supported");
			/*itv_GX *p ;

			   p = (itv_GX *)itv->head ;

			   while(p) {
			   if(p->label == old_label) {
			   p->label = new_label ;
			   }
			   p = p->next ;
			   } */
		}
		break;
	}

	return (1);
}

/*
     -------------------------------------------
     FUNCTION: itv_list_contain
     -------------------------------------------
*/

int				/*+ Purpose: check if some interval of a list contains a
				   w-pattern, and returns the respective label   + */ itv_list_contain(
													      itv_t * itv,	/*+ In: the pointer to the list of intervals       + */
													      unsigned int *wpat,	/*+ In: the w-pattern                              + */
													      int wzip	/*+ In: compacted w-pattern's size                 + */
    )
/*+ Return: the label of the interval that contains the w-pattern, or the
            default label value otherwise                                  +*/
{
	int label;
	int i;
	itv_BX *p;

	p = (itv_BX *) itv->head;
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

	if (label == -1) {
		return (itv->deflabel);
	} else {
		return (label);
	}
}

void /*+ Purpose: set some data  of an interval                        + */
itvgx_set(
			itv_GX * p,	/*+ In/Out: pointer to the interval                 + */
			char *A,	/*+ In: left extremity of the interval              + */
			char *B,	/*+ In: right extremity of the interval             + */
			int wsize,	/*+ In: compacted size of w-pattern                 + */
			int label,	/*+ In: label assigned to the interval              + */
			double size,	/*+ In: size of the interval                        + */
			int nindexes,	/*+ In: number of indexes                           + */
			int *plabels,	/*+ In: pointer to indexes                          + */
			itv_GX * next	/*+ In: if the interval is linked in a list of
					   intervals, this must be the pointer to the
					   next interval                               + */
    )
/*+ Return: nothing                                                        +*/
{
/* author: Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)                  */
/* date: Fri Jul 14 2000                                                    */

	int i;

	for (i = 0; i < wsize; i++) {
		p->A[i] = A[i];
		p->B[i] = B[i];
	}

	p->label = label;
	p->size = size;		/* added on Oct 31, 2000 - Nina */
	p->next = next;
	p->nindexes = nindexes;
	p->plabels = plabels;

}

double /*+ Purpose: compute the size of a gray-scale interval + */ itvgx_size(
										     itv_GX * p,	/*+ In: the interval                                + */
										     int wsize	/*+ In: w-pattern's size                            + */
    )
/*+ Return: an integer between 0 and maximum size. It assumes that input
            interval is a valid one.                                       +*/
{
/* author: Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)                  */
/* date: Tue Oct 31 2000                                                    */

	double size;
	int i;

	size = 1;
	for (i = 0; i < wsize; i++) {
		size = size * ((double)p->B[i] - (double)p->A[i] + 1.0);
	}

	return (size);
}

/*
     -------------------------------------------
     FUNCTION: itv_gen_itv
     -------------------------------------------
*/

itv_t *				/*+ Purpose: create one interval (1: [empty,W], 2:[o,W],
				   3:[empty,W-o])                                + */
itv_gen_itv(window_t * win,	/*+ In: window                                      + */
	    int itv_type,	/*+ In: interval type (1: [empty,W], 2:[o,W],
				   3:[empty,W-o])                              + */
	    int map_type,	/*+ In: mapping type (BB=0 or BG=1 or GG=2
				   or WKC=3 or WKF=4)            + */
	    int k0,		/*+ In: Input range for aperture                    + */
	    int label,		/*+ In: label to be assigned to the interval        + */
	    int def_label	/*+ In: label outside the interval (default label)  + */
    )
/*+ Return: Pointer to ITV structure on success, NULL on failure           +*/
{
	int wsize, wzip, i, cx, cy, count;
	char *Ag, *Bg;
	unsigned int *A, *B;
	itv_t *itv;
	itv_BX *p;
	itv_GX *pg;

#ifdef _DEBUG_
	trios_debug("itv_gen_itv: entered");
#endif

	wsize = win_get_band_wsize(win, 1);
	wzip = size_of_zpat(wsize);

	switch (map_type) {
	case BB:
	case BG:
		/* allocates space for the patterns of the interval [A,B] */
		if ((A = (unsigned int *)malloc(sizeof(int) * wzip)) == NULL) {
			return (itv_t *) trios_error(1,
						     "itv_gen_itv: memory allocation error.");
		}
		if ((B = (unsigned int *)malloc(sizeof(int) * wzip)) == NULL) {
			return (itv_t *) trios_error(1,
						     "itv_gen_itv: memory allocation error.");
		}

		for (i = 0; i < wzip; i++) {
			A[i] = B[i] = 0;
		}

		/* Fill the first wsize bits of B (from left to right) with 1    */
		for (i = 0; i < wsize; i++) {
			B[i / 32] = B[i / 32] | bitmsk[i % 32];
		}

		if (itv_type != 1) {	/* identity or complement operator's base */

			/* compute the central point and also the */
			/* window point associated to it          */
			cy = (win_get_height(win)) / 2;
			cx = (win_get_width(win)) / 2;

#ifdef _DEBUG_
			trios_debug("Central points (%d,%d)", cx, cy);
#endif

			if (win->windata[cy * win_get_width(win) + cx] != 0) {
				count = 0;
				for (i = 0; i < cy * win_get_width(win) + cx;
				     i++) {
					if (win->windata[i])
						count++;
				}

				if (itv_type == 2) {
					/* turn on, in A, the point correspondent to the origin */
					A[count / 32] =
					    A[count / 32] | bitmsk[count % 32];
				} else {
					/* turn off, in B, the point correspondent to the origin */
					B[count / 32] =
					    (B[count / 32] &
					     (~bitmsk[count % 32]));
				}
			}

			else {	/* window does not contain origin */
				trios_warning
				    ("Window does not contain origin.");
				trios_warning
				    ("Type 1 (constant 1 operator) base will be generated");
			}
		}

		/* create structure to hold the interval */
		if (NULL == (itv = itv_create(wsize, map_type, def_label))) {
			win_free(win);
			return (itv_t *) trios_error(MSG,
						     "itv_gen_itv: itv_create() failed.");
		}

		/* the itv_t structure will contain, actually, just one interval */
		p = itv_nodebx_create(wzip);
		itvbx_set(p, A, B, wzip, label, (itv_BX *) (itv->head));
		itv->head = (int *)p;
		itv->nitv = 1;

		free(A);
		free(B);

		break;

	case GG:
		/* allocates space for the patterns of the interval [A,B] */
		if ((Ag = (char *)malloc(sizeof(char) * wsize)) == NULL) {
			return (itv_t *) trios_error(1,
						     "itv_gen_itv: memory allocation error.");
		}
		if ((Bg = (char *)malloc(sizeof(char) * wsize)) == NULL) {
			return (itv_t *) trios_error(1,
						     "itv_gen_itv: memory allocation error.");
		}

		for (i = 0; i < wsize; i++) {
			Ag[i] = 0;
			Bg[i] = (char)255;
		}

		/* create structure to hold the interval */
		if (NULL == (itv = itv_create(wsize, map_type, def_label))) {
			win_free(win);
			return (itv_t *) trios_error(MSG,
						     "itv_gen_itv: itv_create() failed.");
		}

		/* the itv_t structure will contain, actually, just one interval */
		pg = itv_nodegx_create(wsize);
		itvgx_set(pg, Ag, Bg, wsize, label, 0, 0, NULL,
			  (itv_GX *) (itv->head));
		pg->size = itvgx_size(pg, wsize);	/* added on Oct 31, 2000 - Nina */
		itv->head = (int *)pg;
		itv->nitv = 1;

		free(Ag);
		free(Bg);
		break;

	case WKC:
	case WKF:
		return (itv_t *) trios_error(MSG, "Operator not supported");
		/*
		   #ifdef _DEBUG_
		   trios_debug("itv_gen_itv: WKC or WKF entered") ;
		   #endif */
		/* allocates space for the patterns of the interval [A,B] *//*
		   if((Ag = (char *)malloc(sizeof(char)*wsize)) == NULL) {
		   return (itv_t *)trios_error(1, "itv_gen_itv: memory allocation error.") ;
		   }
		   if((Bg = (char *)malloc(sizeof(char)*wsize)) == NULL) {
		   return (itv_t *)trios_error(1, "itv_gen_itv: memory allocation error.") ;
		   }

		   for(i=0; i<wsize; i++) {
		   Ag[i] = - k0 ;
		   Bg[i] = k0 ;
		   }

		   #ifdef _DEBUG_
		   for(i=0; i<wsize; i++) {
		   trios_debug("Ag[%d] = %d, Bg[%d] = %d", i, Ag[i], i, Bg[i]) ;
		   }
		   #endif */

		/* create structure to hold the interval *//*
		   if(NULL==(itv = itv_create(wsize, map_type, def_label))) {
		   win_free(win) ;
		   return (itv_t *)trios_error(MSG, "itv_gen_itv: itv_create() failed.") ;
		   } */

		/* the itv_t structure will contain, actually, just one interval

		   pg = itv_nodegx_create(wsize) ;
		   itvgx_set(pg, Ag, Bg, wsize, label, 0, 0, NULL, (itv_GX *)(itv->head)) ;
		   pg->size = itvgx_size(pg, wsize) ; *//* added on Oct 31, 2000 - Nina *//*
		   itv->head = (int *)pg ;
		   itv->nitv = 1 ;

		   free(Ag); free(Bg) ;

		   #ifdef _DEBUG_
		   trios_debug("itv_gen_itv: WKC or WKF out") ;
		   #endif
		 */
		break;

	default:
		return (itv_t *) trios_error(MSG,
					     "itv_gen_itv: type not implemented yet.");
	}

	return (itv);

}

/*
     -------------------------------------------
     FUNCTION: itv_consistency
     -------------------------------------------
*/

int				/*+ Given a set of intervals and a set of classified
				   examples, check whether the set of intervals is
				   consistent with the examples                           + */ itv_consistency(
														      itv_t * itv,	/*+ In: intervals set                     + */
														      mtm_t * mtm	/*+ set of examples                       + */
    )
/*+ Return: 1 if they are consistent, 0 if they are not consistent         +*/
{
/* author: Nina S. Tomita                                                   */
/* date: Mar 21 2002                                                        */

	int i, wzip;
	mtm_BX *table_BX;

	wzip = size_of_zpat(itv_get_wsize(itv));

	/* For each example with label 1, check whether it belongs to some
	   of the intervals                                                 */
	table_BX = (mtm_BX *) mtm->mtm_data;
	for (i = 0; i < mtm_get_nmtm(mtm); i++) {
		if (table_BX[i].label == 1) {
			if (!itv_list_contain(itv, table_BX[i].wpat, wzip)) {
				return 0;
			}
		} else {
			if (itv_list_contain(itv, table_BX[i].wpat, wzip)) {
				return 0;
			}
		}
	}

	return (1);
}
