
#include <trios.h>

/*
     -------------------------------------------
     FUNCTION: xpl_set_rootGG
     -------------------------------------------
*/

int /*+ Purpose: set a pointer to a XPL tree                    + */
xpl_set_rootGG(
			     xpl_t * xpl,	/*+ In/Out: pointer to the XPL                 + */
			     xpl_GG * root	/*+ In: pointer to the XPL tree                + */
    )
/*+ Return: 1 on success, 0 on failure                                     +*/
{
/*  author: Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)                 */
/*  date Wed Apr 23 1997                                                    */

	if ((xpl->type != GG) && (xpl->type != WKF) && (xpl->type != WKC)) {
		return trios_error(1, "Incompatible data types.");
	}
	xpl->root = (int *) root;

	return (1);
}


/*
     -------------------------------------------
     FUNCTION: GG_leftrotate
     -------------------------------------------
*/

void /*+ Purpose: rotate a subtree to the left                + */ GG_leftrotate(
											xpl_GG ** pp	/*+ In/Out: pointer to the subtree's root     + */
    )
/*+ Return: nothing                                                        +*/
{
/*  author: Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)                 */
/*  date Wed Apr 23 1997                                                    */

	xpl_GG *p, *r;

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
     FUNCTION: GG_rightrotate
     -------------------------------------------
*/

void /*+ Purpose: rotate a subtree to the right                + */
GG_rightrotate(
			     xpl_GG ** pp	/*+ In/Out: pointer to the subtree's root + */
    )
/*+ Return: nothing                                                        +*/
{
/*  author: Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)                 */
/*  date Wed Apr 23 1997                                                    */

	xpl_GG *p, *l;

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
     FUNCTION: xpl_GG_create
     -------------------------------------------
*/

xpl_GG *			/*+ Purpose: allocate a GG node                    + */
xpl_GG_create(int wsize		/*+ In:  window size                     + */
    )
/*+ Return: a pointer to a node of GG type                                 +*/
{
/*  author: Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)                 */
/*  date Wed Apr 23 1997                                                    */

	xpl_GG *p;

	p = (xpl_GG *) malloc(sizeof(xpl_GG));

	if (p == NULL) {
		return (xpl_GG *) trios_error(1, "Memory allocation failed.");
	}

	p->wpat = (char *) malloc(sizeof(char) * wsize);
	if (p->wpat == NULL) {
		free(p);
		return (xpl_GG *) trios_error(1, "Memory allocation failed.");
	}

	p->freqlist = NULL;
	p->bal = 0;
	p->left = p->right = NULL;

	return (p);
}


/*
     -------------------------------------------
     FUNCTION: xpl_GG_set_node
     -------------------------------------------
*/

void /*+ Purpose: set the fields of a GG node          + */ xpl_GG_set_node(
										   xpl_GG * p,	/*+ In/Out: pointer to the GG node            + */
										   char *wpat,	/*+ In: w-pattern                             + */
										   int wsize,	/*+ In: w-pattern size                        + */
										   freq_node * freqlist	/*+ In: pointer to list of frequencies        + */
    )
/*+ Return: nothing                                                        +*/
{
/*  author: Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)                 */
/*  date Wed Apr 30 1997                                                    */

	int j;

	/* sets the pattern string */
	for (j = 0; j < wsize; j++) {
		p->wpat[j] = wpat[j];
	}

	/* sets the frequency list */
	p->freqlist = freqlist;

}

/*
     -------------------------------------------
     FUNCTION: xpl_GG_insert
     -------------------------------------------
*/

int /*+ Purpose: insert a node in a GG tree (recursively)        + */
xpl_GG_insert(
			    xpl_t * xpl,	/*+ In/Out: pointer to the XPL structure         + */
			    xpl_GG ** pp,	/*+ In/Out: pointer to a subtree's root          + */
			    char *wpat,	/*+ In: w-pattern to be inserted                 + */
			    freq_node * freqlist	/*+ In: w-pattern list of frequencies and labels + */
    )
/*+ Return: -1 on failure, >= 0  on success                                   +*/
{
/*  author: Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)                    */
/*  date Wed Apr 30 1997                                                       */

/*  Modification by:  Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)          */
/*  Date: Tue Nov 18 1997                                                      */
/*  Mod: Bug fixed. Added a line to increment xpl->sum when a new node is      */
/*       created, or when a node's freqlist is modified                        */

	int deltaH = 0, st, wsize;
	xpl_GG *p;

#ifdef _DEBUG_
	freq_node *pq;
#endif


	wsize = xpl->wsize;

#ifdef _DEBUG_
	trios_debug("Entrou no xpl_GG_insert");
	for (i = 0; i < wsize; i++) {
		trios_debug("wpat[%d]=%d\n", i, wpat[i]);
	}
#endif

	p = *pp;
	if (p == NULL) {

#ifdef _DEBUG_
		trios_debug("Vai chamar o xpl_GG_create");
#endif

		*pp = p = xpl_GG_create(xpl->wsize);
		if (p == NULL) {
			(void) trios_error(MSG,
					   "xpl_GG_insert: xpl_GG_create() failed.");
			return (-1);
		}
		xpl->n_nodes++;
		xpl->sum += freq_sum(freqlist);
		xpl_GG_set_node(p, wpat, xpl->wsize, freqlist);
		deltaH = 1;
	} else {
		st = wpat_compareXG(p->wpat, wpat, xpl->wsize);
		if (st == 0) {

#ifdef _DEBUG_
			trios_debug
			    ("O w-pattern e' igual, vai ajustar as frequencias");
#endif

			xpl->sum += freq_sum(freqlist);

			if (!set_freq(freqlist, &p->freqlist)) {
				(void) trios_error(MSG,
						   "xpl_GG_insert: set_freq() failed.");
				return (-1);
			}
#ifdef _DEBUG_
			for (pq = p->freqlist; pq; pq = pq->next) {
				trios_debug("%d %d ", pq->label, pq->freq);
			}
			trios_debug("---------");
#endif

		} else if (st == LESS) {
			if (xpl_GG_insert(xpl, &p->right, wpat, freqlist)) {
				p->bal++;
				if (p->bal == 1)
					deltaH = 1;
				else if (p->bal == 2) {
					if (p->right->bal == -1)
						GG_rightrotate(&p->right);
					GG_leftrotate(pp);
				}
			}
		} else {
			if (xpl_GG_insert(xpl, &p->left, wpat, freqlist)) {
				p->bal--;
				if (p->bal == -1)
					deltaH = 1;
				else if (p->bal == -2) {
					if (p->left->bal == 1)
						GG_leftrotate(&p->left);
					GG_rightrotate(pp);
				}
			}
		}
	}
	return (deltaH);
}

/*
     -------------------------------------------
     FUNCTION: xpl_GG_free
     -------------------------------------------
*/

void /*+ Purpose: free memory area used by a GG node               + */
xpl_GG_free(
			  xpl_GG * p	/*+ In/Out: pointer to the GG node                  + */
    )
/*+ Return: nothing                                                        +*/
{
/*  author: Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)                 */
/*  date: Fri Nov 29 1996                                                   */

	free(p->wpat);
	freqlist_free(p->freqlist);
	free(p);

}


/*
     -------------------------------------------
     FUNCTION: xpl_GG_free_tree
     -------------------------------------------
*/

void /*+ Purpose: free memory area used by a GG tree      + */ xpl_GG_free_tree(
										       xpl_GG * p	/*+ In/Out: pointer to the GG tree        + */
    )
/*+ Return: nothing                                                        +*/
{
/*  author: Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)                 */
/*  date: Fri Nov 29 1996                                                   */

	if (p != NULL) {
		xpl_GG_free_tree(p->left);
		xpl_GG_free_tree(p->right);
		xpl_GG_free(p);
	}
}


/*
     -------------------------------------------
     FUNCTION: xpl_GG_merge
     -------------------------------------------
*/

int /*+ Purpose: merge two GG tree, recursively                + */ xpl_GG_merge(
											xpl_t * xpl,	/*+ In/Out: pointer to the XPL, containing the fisrt GG tree + */
											xpl_GG * p	/*+ In: pointer to the second GG tree                        + */
    )
/*+ Return: 1 on success, 0 on failure                                     +*/
{
/*  author: Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)                 */
/*  date Mon May  5 1997                                                    */

	xpl_GG *right;

	if (p != NULL) {
		right = p->right;

		if (!xpl_GG_merge(xpl, p->left)) {
			return trios_error(MSG,
					   "Recursive call to xpl_GG_merge() failed.");
		}

		if (!xpl_GG_merge(xpl, right)) {
			return trios_error(MSG,
					   "Recursive call to xpl_GG_merge() failed.");
		}


		if (xpl_GG_add(xpl, (xpl_GG **) (&xpl->root), p) == -1) {
			return trios_error(MSG,
					   "xpl_GG_merge: xpl_GG_add() failed.");
		}


		/* IMPORTANT REMARK */

		/* Here we could have used the following code instead of the previous one */

		/*
		   if(xpl_GG_insert(xpl, (xpl_GG **)(&xpl->root), p->wpat, p->fq0, p->fq1)==-1) {
		   return trios_error(MSG, "xpl_GG_merge: xpl_GG_insert() failed");
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
     FUNCTION: xpl_GG_add
     -------------------------------------------
*/

int /*+ Purpose: add a new node into a GG tree, recursively  + */ xpl_GG_add(
										    xpl_t * xpl,	/*+ In/Out: pointer to the XPL, containing the GG tree + */
										    xpl_GG ** p_cur,	/*+ In/Out: current node in the GG tree       + */
										    xpl_GG * p_new	/*+ In: pointer to the node to be added       + */
    )
/*+ Return: -1 on failure, >= 0  on success                                +*/
{
/*  author: Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)                 */
/*  date Wed May  7 1997                                                    */

	int deltaH = 0, st;
	xpl_GG *p;


	p = *p_cur;
	if (p == NULL) {
		p_new->right = p_new->left = NULL;
		p_new->bal = 0;
		*p_cur = p_new;
		deltaH = 1;
		xpl->n_nodes++;
		xpl->sum += freq_sum(p_new->freqlist);
	} else {
		st = wpat_compareXG(p->wpat, p_new->wpat, xpl->wsize);

		if (st == 0) {
			xpl->sum += freq_sum(p_new->freqlist);
			if (!set_freq(p_new->freqlist, &p->freqlist)) {
				(void) trios_error(MSG,
						   "xpl_GG_add: set_freq() failed.");
				return (-1);
			}
			p_new->freqlist = NULL;
			xpl_GG_free(p_new);
		} else if (st == -1) {
			if (xpl_GG_add(xpl, &p->right, p_new)) {
				p->bal++;
				if (p->bal == 1)
					deltaH = 1;
				else if (p->bal == 2) {
					if (p->right->bal == -1)
						GG_rightrotate(&p->right);
					GG_leftrotate(p_cur);
				}
			}
		} else {

			if (xpl_GG_add(xpl, &p->left, p_new)) {
				p->bal--;
				if (p->bal == -1)
					deltaH = 1;
				else if (p->bal == -2) {

					if (p->left->bal == 1)
						GG_leftrotate(&p->left);
					GG_rightrotate(p_cur);

				}
			}
		}
	}

	return (deltaH);
}

/*
     -------------------------------------------
     FUNCTION: tree2ll_GG_asc
     -------------------------------------------
*/

int				/*+ Purpose: change GG tree structure to the linked list
				   structure (ascendent ordering)                  + */ tree2ll_GG_asc(
													      xpl_GG * p,	/*+ In: GG tree structure                         + */
													      xpl_GG ** pp	/*+ Out: linked list structure                    + */
    )
/*+ Return: 1 on success, 0 on failure                                     +*/
{
/*  author: Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)                 */
/*  date: Fri Nov 29 1996                                                   */

	if (p != NULL) {
		if (!tree2ll_GG_asc(p->right, pp)) {
			return trios_error(MSG,
					   "Recursive call to tree2ll_GG_asc() failed.");
		}
		p->right = *pp;
		*pp = p;
		if (!tree2ll_GG_asc(p->left, pp)) {
			return trios_error(MSG,
					   "Recursive call to tree2ll_GG_asc() failed.");
		}
		p->left = NULL;
	}
	return (1);
}

/*
     -------------------------------------------
     FUNCTION: tree2ll_GG_desc
     -------------------------------------------
*/

int				/*+ Purpose: change GG tree structure to the linked list
				   structure (descendent ordering)                 + */ tree2ll_GG_desc(
													       xpl_GG * p,	/*+ In: GG tree structure                      + */
													       xpl_GG ** pp	/*+ Out: linked list structure                 + */
    )
/*+ Return: 1 on success, 0 on failure                                     +*/
{
/*  author: Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)                 */
/*  date: Fri Nov 29 1996                                                   */

	if (p != NULL) {
		if (!tree2ll_GG_desc(p->left, pp)) {
			return trios_error(MSG,
					   "Recursive call to tree2ll_GG_desc() failed.");
		}
		p->left = *pp;
		*pp = p;
		if (!tree2ll_GG_desc(p->right, pp)) {
			return trios_error(MSG,
					   "Recursive call to tree2ll_GG_desc() failed.");
		}
		p->right = NULL;
	}
	return (1);
}



/*
     -------------------------------------------
     FUNCTION: xpl_GG_sort
     -------------------------------------------
*/

int /*+ Purpose: sort GG XPL set                            + */ xpl_GG_sort(
										    xpl_GG * p,	/*+ In: pointer to XPL tree to be sorted         + */
										    xpl_GG ** pp	/*+ Out: sorted XPL tree                         + */
    )
/*+ Return: 1 on success, 0 on failure                                     +*/
{
/*  author: Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)                 */
/*  date: Fri Nov 29 1996                                                   */

	if (p != NULL) {
		if (!xpl_GG_sort(p->left, pp)) {
			return trios_error(MSG,
					   "Recursive call to xpl_GG_sort() failed.");
		}
		if (!xpl_GG_sort(p->right, pp)) {
			return trios_error(MSG,
					   "Recursive call to xpl_GG_sort() failed.");
		}
		xpl_GG_sort_insert(p, pp);
	}
	return (1);
}

/*
     -------------------------------------------
     FUNCTION: xpl_GG_sort_insert
     -------------------------------------------
*/

int				/*+ Purpose: insert a node in a GG XPL tree, balancing
				   it according to the frequency of examples         + */ xpl_GG_sort_insert(
														    xpl_GG * p,	/*+ In: node to be inserted                + */
														    xpl_GG ** pp	/*+ In/Out: sorted XPL tree                + */
    )
/*+ Return: >= 0 on success, -1 on failure                                 +*/
{
/*  author: Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)                 */
/*  date: Fri Nov 29 1996                                                   */

	int deltaH = 0;
	xpl_GG *q;

	q = *pp;
	if (q == NULL) {
		q = *pp = p;
		q->left = NULL;
		q->right = NULL;
		q->bal = 0;
		deltaH = 1;
	} else {

		if (freq_sum(p->freqlist) >= freq_sum(q->freqlist)) {
			if (xpl_GG_sort_insert(p, &q->right)) {
				q->bal++;
				if (q->bal == 1)
					deltaH = 1;
				else if (q->bal == 2) {
					if (q->right->bal == -1)
						GG_rightrotate(&q->right);
					GG_leftrotate(pp);
				}
			}
		} else {
			if (xpl_GG_sort_insert(p, &q->left)) {
				q->bal--;
				if (q->bal == -1)
					deltaH = 1;
				else if (q->bal == -2) {
					if (q->left->bal == 1)
						GG_leftrotate(&q->left);
					GG_rightrotate(pp);
				}
			}
		}
	}

	return (deltaH);
}
