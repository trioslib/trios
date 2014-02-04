#include <trios.h>

/* #define _DEBUG_ */
/* #define _DEBUG_2_ */

/*
     -------------------------------------------
     FUNCTION: xpl_set_rootBB
     -------------------------------------------
*/

int /*+ Purpose: set a pointer to a XPL tree                    + */
xpl_set_rootBB(
			     xpl_t * xpl,	/*+ In/Out: pointer to the XPL                 + */
			     xpl_BB * root	/*+ In: pointer to the XPL tree                + */
    )
/*+ Return: 1 on success, 0 on failure                                     +*/
{
	if (xpl->type != BB) {
		return trios_error(1, "Incompatible types of data.");
	}
	xpl->root = (int *)root;
	return (1);
}

/*
     -------------------------------------------
     FUNCTION: BB_leftrotate
     -------------------------------------------
*/

void /*+ Purpose: rotate a subtree to the left                + */ BB_leftrotate(
											xpl_BB ** pp	/*+ In/Out: pointer to the subtree's root     + */
    )
/*+ Return: nothing                                                        +*/
{
	xpl_BB *p, *r;

	p = *pp;
	*pp = r = p->right;
	p->right = r->left;
	r->left = p;
	p->bal--;
	if (r->bal > 0)
		p->bal -= r->bal;
	r->bal--;
	if (p->bal < 0)
		r->bal += p->bal;
}

/*
     -------------------------------------------
     FUNCTION: BB_rightrotate
     -------------------------------------------
*/

void /*+ Purpose: rotate a subtree to the right                + */
BB_rightrotate(
			     xpl_BB ** pp	/*+ In/Out: pointer to the subtree's root + */
    )
/*+ Return: nothing                                                        +*/
{
	xpl_BB *p, *l;

	p = *pp;
	*pp = l = p->left;
	p->left = l->right;
	l->right = p;
	p->bal++;
	if (l->bal < 0)
		p->bal -= l->bal;
	l->bal++;
	if (p->bal > 0)
		l->bal += p->bal;
}

/*
     -------------------------------------------
     FUNCTION: xpl_BB_create
     -------------------------------------------
*/

xpl_BB *			/*+ Purpose: allocate a BB node                    + */
xpl_BB_create(int wzip		/*+ In: compacted window size             + */
    )
/*+ Return: a pointer to a node of BB type                                 +*/
{
	xpl_BB *p;

	p = (xpl_BB *) malloc(sizeof(xpl_BB));
	if (p == NULL) {
		return (xpl_BB *) trios_error(1, "Memory allocation failed.");
	}
	p->fq0 = 0;
	p->fq1 = 0;
	p->bal = 0;
	p->left = p->right = NULL;
	p->wpat = (unsigned int *)malloc(sizeof(int) * wzip);
	if (p->wpat == NULL) {
		free(p);
		return (xpl_BB *) trios_error(1, "Memory allocation failed.");
	}
	return (p);
}

/*
     -------------------------------------------
     FUNCTION: xpl_BB_set_node
     -------------------------------------------
*/

void /*+ Purpose: set the fields of a BB node          + */ xpl_BB_set_node(
										   xpl_BB * p,	/*+ In/Out: pointer to the BB node            + */
										   unsigned int *wpat,	/*+ In: w-pattern                             + */
										   int wzip,	/*+ In: w-pattern size                        + */
										   unsigned int fq0,	/*+ In: negative frequency of the w-pattern   + */
										   unsigned int fq1	/*+ In: positive frequency of the w-pattern   + */
    )
/*+ Return: nothing                                                        +*/
{
	int j;

	p->fq0 += fq0;
	p->fq1 += fq1;
	for (j = 0; j < wzip; j++) {
		p->wpat[j] = wpat[j];
	}
}

/*
     -------------------------------------------
     FUNCTION: xpl_BB_insert
     -------------------------------------------
*/

int /*+ Purpose: insert a node in a BB tree (recursively)     + */ xpl_BB_insert(
											xpl_t * xpl,	/*+ In/Out: pointer to the XPL structure      + */
											xpl_BB ** pp,	/*+ In/Out: pointer to a subtree's root       + */
											unsigned int *wpat,	/*+ In: w-pattern to be inserted              + */
											unsigned int fq0,	/*+ In: w-pattern's negative frequency        + */
											unsigned int fq1	/*+ In: w-pattern's positive frequency        + */
    )
/*+ Return: -1 on failure, >= 0  on success                                +*/
{
	int deltaH = 0, st;
	xpl_BB *p;

#ifdef _DEBUG_2_
	if (wpat[0] == 0) {
		trios_debug("A zero will be inserted");
	}
#endif

	p = *pp;
	if (p == NULL) {
		*pp = p = xpl_BB_create(xpl->wzip);
		if (p == NULL) {
			(void)trios_error(MSG,
					  "xpl_BB_insert: xpl_BB_create() failed.");
			return (-1);
		}
		xpl_BB_set_node(p, wpat, xpl->wzip, fq0, fq1);
		deltaH = 1;
		xpl->n_nodes++;
		xpl->sum += (fq0 + fq1);
	} else {
		st = wpat_compare(p->wpat, wpat, xpl->wzip);
		if (st == 0) {
			p->fq0 += fq0;
			p->fq1 += fq1;
			xpl->sum += (fq0 + fq1);
		} else if (st == LESS) {
			if (xpl_BB_insert(xpl, &p->right, wpat, fq0, fq1)) {
				p->bal++;
				if (p->bal == 1)
					deltaH = 1;
				else if (p->bal == 2) {
					if (p->right->bal == -1)
						BB_rightrotate(&p->right);
					BB_leftrotate(pp);
				}
			}
		} else {
			if (xpl_BB_insert(xpl, &p->left, wpat, fq0, fq1)) {
				p->bal--;
				if (p->bal == -1)
					deltaH = 1;
				else if (p->bal == -2) {
					if (p->left->bal == 1)
						BB_leftrotate(&p->left);
					BB_rightrotate(pp);
				}
			}
		}
	}
	return (deltaH);
}

/*
     -------------------------------------------
     FUNCTION: xpl_BB_free
     -------------------------------------------
*/

void /*+ Purpose: free memory area used by a BB node               + */
xpl_BB_free(
			  xpl_BB * p	/*+ In/Out: pointer to the BB node                  + */
    )
/*+ Return: nothing                                                        +*/
{
	free(p->wpat);
	free(p);
}

/*
     -------------------------------------------
     FUNCTION: xpl_BB_free_tree
     -------------------------------------------
*/

void /*+ Purpose: free memory area used by a BB tree      + */ xpl_BB_free_tree(
										       xpl_BB * p	/*+ In/Out: pointer to the BB tree        + */
    )
/*+ Return: nothing                                                        +*/
{
	if (p != NULL) {
		xpl_BB_free_tree(p->left);
		xpl_BB_free_tree(p->right);
		xpl_BB_free(p);
	}
}

/*
     -------------------------------------------
     FUNCTION: xpl_BB_merge
     -------------------------------------------
*/

int /*+ Purpose: merge two BB tree, recursively                + */ xpl_BB_merge(
											xpl_t * xpl,	/*+ In/Out: pointer to the XPL, containing the fisrt BB tree + */
											xpl_BB * p	/*+ In: pointer to the second BB tree + */
    )
/*+ Return: 1 on success, 0 on failure                                     +*/
{
	xpl_BB *right;

	if (p != NULL) {
		right = p->right;

		if (!xpl_BB_merge(xpl, p->left)) {
			return trios_error(MSG,
					   "xpl_BB_merge: xpl_BB_merge() failed");
		}

		if (!xpl_BB_merge(xpl, right)) {
			return trios_error(MSG,
					   "xpl_BB_merge: xpl_BB_merge() failed");
		}

		if (xpl_BB_add(xpl, (xpl_BB **) (&xpl->root), p) == -1) {
			return trios_error(MSG,
					   "xpl_BB_merge: xpl_BB_add() failed");
		}

		/* IMPORTANT REMARK */

		/* Here we could have used the following code instead of the previous one */

		/*
		   if(xpl_BB_insert(xpl, (xpl_BB **)(&xpl->root), p->wpat, p->fq0, p->fq1)==-1) {
		   return trios_error(MSG, "xpl_BB_merge: xpl_BB_insert() failed");
		   }
		 */

		/* The difference is that in the prior one we use the already allocated 
		   nodes in the merge and in the later one we have to allocate space
		   in memory to merge a new w-pattern and to free the whole xpl tree
		   that is being merged                                                 */

	}
	return (1);
}

/*
     -------------------------------------------
     FUNCTION: xpl_BB_add
     -------------------------------------------
*/

int /*+ Purpose: add a new node into a BB tree, recursively  + */ xpl_BB_add(
										    xpl_t * xpl,	/*+ In/Out: pointer to the XPL, containing the BB tree + */
										    xpl_BB ** p_cur,	/*+ In/Out: current node in the BB tree       + */
										    xpl_BB * p_new	/*+ In: pointer to the node to be added       + */
    )
/*+ Return: -1 on failure, >= 0  on success                                +*/
{
	int deltaH = 0, st;
	xpl_BB *p;

	p = *p_cur;
	if (p == NULL) {
		p_new->right = p_new->left = NULL;
		p_new->bal = 0;
		*p_cur = p_new;
		deltaH = 1;
		xpl->n_nodes++;
		xpl->sum += (p_new->fq0 + p_new->fq1);
	} else {
		st = wpat_compare(p->wpat, p_new->wpat, xpl->wzip);

		if (st == 0) {
			p->fq0 += p_new->fq0;
			p->fq1 += p_new->fq1;
			xpl->sum += (p_new->fq0 + p_new->fq1);
			xpl_BB_free(p_new);
		} else if (st == -1) {
			if (xpl_BB_add(xpl, &p->right, p_new)) {
				p->bal++;
				if (p->bal == 1)
					deltaH = 1;
				else if (p->bal == 2) {
					if (p->right->bal == -1)
						BB_rightrotate(&p->right);
					BB_leftrotate(p_cur);
				}
			}
		} else {

			if (xpl_BB_add(xpl, &p->left, p_new)) {
				p->bal--;
				if (p->bal == -1)
					deltaH = 1;
				else if (p->bal == -2) {

					if (p->left->bal == 1)
						BB_leftrotate(&p->left);
					BB_rightrotate(p_cur);

				}
			}
		}
	}

	return (deltaH);
}

/*
     -------------------------------------------
     FUNCTION: tree2ll_BB_asc
     -------------------------------------------
*/

int				/*+ Purpose: change BB tree structure to the linked list
				   structure (ascendent ordering)                  + */ tree2ll_BB_asc(
													      xpl_BB * p,	/*+ In: BB tree structure                         + */
													      xpl_BB ** pp	/*+ Out: linked list structure                    + */
    )
/*+ Return: 1 on success, 0 on failure                                     +*/
{
	if (p != NULL) {
		if (!tree2ll_BB_asc(p->right, pp)) {
			return trios_error(MSG, "tree2xpl_BB() failed.");
		}
		p->right = *pp;
		*pp = p;
		if (!tree2ll_BB_asc(p->left, pp)) {
			return trios_error(MSG, "tree2xpl_BB() failed.");
		}
		p->left = NULL;
	}
	return (1);
}

/*
     -------------------------------------------
     FUNCTION: tree2ll_BB_desc
     -------------------------------------------
*/

int				/*+ Purpose: change BB tree structure to the linked list
				   structure (descendent ordering)                 + */ tree2ll_BB_desc(
													       xpl_BB * p,	/*+ In: BB tree structure                      + */
													       xpl_BB ** pp	/*+ Out: linked list structure                 + */
    )
/*+ Return: 1 on success, 0 on failure                                     +*/
{
	if (p != NULL) {
		if (!tree2ll_BB_desc(p->left, pp)) {
			return trios_error(MSG, "tree2xpl_BB() failed.");
		}
		p->left = *pp;
		*pp = p;
		if (!tree2ll_BB_desc(p->right, pp)) {
			return trios_error(MSG, "tree2xpl_BB() failed.");
		}
		p->right = NULL;
	}
	return (1);
}

/*
     -------------------------------------------
     FUNCTION: xpl_sort_BB
     -------------------------------------------
*/

int /*+ Purpose: sort BB XPL set                            + */ xpl_sort_BB(
										    xpl_BB * p,	/*+ In: pointer to XPL tree to be sorted         + */
										    xpl_BB ** pp	/*+ Out: sorted XPL tree                         + */
    )
/*+ Return: 1 on success, 0 on failure                                     +*/
{
	if (p != NULL) {
		if (!xpl_sort_BB(p->left, pp)) {
			return trios_error(MSG,
					   "xpl_sort_BB: recursive call to xpl_sort_BB() failed.");
		}
		if (!xpl_sort_BB(p->right, pp)) {
			return trios_error(MSG,
					   "xpl_sort_BB: recursive call to xpl_sort_BB() failed.");
		}
		(void)xpl_sort_insert(p, pp);
	}
	return (1);
}

/*
     -------------------------------------------
     FUNCTION: xpl_sort_insert
     -------------------------------------------
*/

int				/*+ Purpose: insert a node in a BB XPL tree, balancing     
				   it according to the frequency of examples         + */ xpl_sort_insert(
														 xpl_BB * p,	/*+ In: node to be inserted                + */
														 xpl_BB ** pp	/*+ In/Out: sorted XPL tree                + */
    )
/*+ Return: >= 0 on success, -1 on failure                                 +*/
{
	int deltaH = 0;
	xpl_BB *q;

	q = *pp;
	if (q == NULL) {
		q = *pp = p;
		q->left = NULL;
		q->right = NULL;
		q->bal = 0;
		deltaH = 1;
	} else {

		if ((p->fq0 + p->fq1) >= (q->fq0 + q->fq1)) {
			if (xpl_sort_insert(p, &q->right)) {
				q->bal++;
				if (q->bal == 1)
					deltaH = 1;
				else if (q->bal == 2) {
					if (q->right->bal == -1)
						BB_rightrotate(&q->right);
					BB_leftrotate(pp);
				}
			}
		} else {
			if (xpl_sort_insert(p, &q->left)) {
				q->bal--;
				if (q->bal == -1)
					deltaH = 1;
				else if (q->bal == -2) {
					if (q->left->bal == 1)
						BB_leftrotate(&q->left);
					BB_rightrotate(pp);
				}
			}
		}
	}

	return (deltaH);
}
