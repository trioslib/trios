#include "trios_win.h"
#include "trios_itv.h"
#include "trios_mtm.h"
#include "paclearn_local.h"
#include <stdio.h>


/*#define _DEBUG_*/

#define _DEBUG_1_
/*#define _DEBUG_2_*/

/* #define _DEBUG_TIME_ */



/* macro to check whether the interval p_new is covered by some interval
   in the list pointed by p                                         */
#define Is_node_covered(covered, p_new, p, wzip)   \
        {                                          \
          itv_BX *p_aux ;                          \
          int flag;                                \
          int ii ;                                 \
                                                   \
          p_aux = p ;                              \
          covered = 0 ;                            \
          while(p_aux) {                           \
            flag = 1;                              \
            for(ii=0; ii<wzip ; ii++){             \
              if(((p_new->B[ii]&p_aux->B[ii])!=p_new->B[ii])    \
                || ((p_new->A[ii]&p_aux->A[ii])!=p_aux->A[ii])) {  \
                flag = 0;                          \
                break;                             \
	      }                                    \
            }                                      \
            if(flag == 1) {                        \
              covered = 1 ;                        \
              break ;                              \
            }                                      \
            p_aux = p_aux->next;                   \
          }                                        \
        }

/*
     -------------------------------------------
     FUNCTION: isi_basic
     -------------------------------------------
*/

int /*+ Purpose: Perform ISI by points (binary case)              + */ isi_basic(
											itv_t ** itv_i,	/*+ In/Out: starting intervals/output intervals        + */
											mtm_t * mtm_i,	/*+ In: examples to be extracted                       + */
											int multi,	/*+ In: if 1, indicates multi-classification           + */
											int isi_type,	/*+ In: ISI type (ISI-0, IIS-1, ISI-2, ISI-3)          + */
											int isi3_step,	/*+ In: minimum cover computation step for ISI-3       + */
											int log_step,	/*+ In: If not ZERO, log file will be written at each
													   time a multiple of log_step examples is processed. + */
											char *log_file,	/*+ In: log file name                                  + */
											char *tmp_bas_file,	/*+ In: File in which the set of interval at each
														   step will be written                       + */
											window_t * win	/*+ In: window (needed if log_step > 0)               + */
    )
/*+ Return: 1 on success, 0 on failure                                     +*/
{
	mtm_t *mtm0, *mtm1, *mtm;
	itv_t *itv, *itv_o;
	itv_BX *p_out;
	int map_type1, map_type2, wsize1, wsize2;
	int lb[256];
	int i, k;

#ifdef _DEBUG_2_
	mtm_BX *q;
	int j;
#endif

	/*GLlog_step = log_step;
	if (log_step) {
		/*
		   strcpy(GLlog_file, log_file) ;
		   strcat(GLlog_file, ".tmp") ;
		 
		GLlog_file = log_file;
		GLtmp_bas_file = tmp_bas_file;
		GLwin = win;
	}*/

	mtm = mtm_i;
	itv = *itv_i;

	/* get and compare mapping types types */
	map_type1 = itv_get_type(itv);
	map_type2 = mtm_get_type(mtm);

	if ((map_type2 != BB) && (map_type2 != BG)) {
		return trios_error(1,
				   "ISI not implemented for non-binary patterns");
	}

	if (map_type1 != map_type2) {
		trios_warning
		    ("Classified examples and starting intervals have different type.");
	}
	if (multi) {
		map_type2 = BG;
	} else {
		map_type2 = BB;
	}

	/* get wsize */
	wsize1 = itv_get_wsize(itv);
	wsize2 = mtm_get_wsize(mtm);
/*
#ifdef _DEBUG_1_
	trios_debug("itv:wsize=%d  mtm:wsize=%d", wsize1, wsize2);
	trios_debug("itv:nitv=%d  mtm:nmtm=%d", itv_get_nitv(itv),
		    mtm_get_nmtm(mtm));
#endif
*/

	if (wsize1 != wsize2) {
		return trios_error(1,
				   "Incompatible size. ISI can't be applied.");
	}

	/* No matter if the case is multi or not, label ZERO is */
	/* always the first to be extracted.                    */
	/* If case is binary, extraction of all ZEROS finishes  */
	/* the process. If case is multi, then first all ZEROS  */
	/* are extracted and then the elements of the classes   */
	/* with fewer examples are processed first              */
	/* The most dense class is not processed                */

	if (multi) {
		if ((k = isi_sort_labels(mtm, lb)) == -1) {
			return trios_error(MSG,
					   "isi_basic: isi_sort_labels() failed.");
		}
	}
	/* if output is binary, then examples with label 0 will be */
	/* extracted from the input interval                       */
	else {
		k = 1;
		lb[0] = 0;
		lb[1] = 1;
	}

#ifdef _DEBUG_
	trios_debug("Examples ordering: done. (k=%d)", k);
#endif

	/* allocate output itv_t structure */
	itv_o = itv_create(wsize2, map_type2, lb[0]);
	if (!itv_o) {
		return trios_error(1, "Memory allocation error.");
	}

	/* change all intervals with label 1 to 0 */
	/* this is done just to avoid a special treatment for */
	/* the first iteration of the loop below */
	itv_label(itv, 1, 0);

	p_out = NULL;

	for (i = 0; i < k; i++) {

		/* change all intervals with label lb[i] to lb[i+1] */
		itv_label(itv, lb[i], lb[i + 1]);

		/* put in mtm0 all minterms of mtm with label lb[i], and
		   in mtm1 all other minterms of mtm                 */
		if (!mtm_sep(mtm, lb[i], &mtm0, &mtm1)) {
			return trios_error(1, "isi_basic: mtm_sep() failed.");
		}
		mtm_free(mtm);

#ifdef _DEBUG_
		trios_debug("Examples with label %d = %d", lb[i], mtm0->nmtm);
#endif

#ifdef _DEBUG_2_
		trios_debug("To be extracted :");
		q = (mtm_BX *) mtm0->mtm_data;
		for (j = 0; j < mtm0->nmtm; j++) {
			trios_debug("%d", q[j].wpat[0]);
		}
		trios_debug("To be covered :");
		q = (mtm_BX *) mtm1->mtm_data;
		for (j = 0; j < mtm1->nmtm; j++) {
			trios_debug("%d", q[j].wpat[0]);
		}
#endif

		/* First iteration will not release mtm because it
		   will be done by the routine who called this one */
		/*    if(i>0) { mtm_free(mtm) ; mtm = NULL ; } */

		if (mtm1->nmtm != 0) {

			switch (isi_type) {

			case ISI_0:

				/* REMEMBER: isi_0() will free "mtm0", and modify "itv_i" */

				if (!(isi_0(&itv, mtm0))) {
					return trios_error(MSG,
							   "isi_basic: isi_0() failed.");
				}
				mtm_free(mtm0);

				if (!itv_filter(mtm1, itv)) {
					return trios_error(MSG,
							   "isi_basic: itv_filter() failed.");
				}

				if (!p_cover(mtm1, itv)) {
					return trios_error(MSG,
							   "isi_basic: p_cover() failed.");
				}
				break;

			case ISI_1:
				/* REMEMBER: isi_1() will free "mtm0" and modify "itv_i" */

				if (!(isi_1(&itv, mtm0, mtm1))) {
					return trios_error(MSG,
							   "isi_basic: isi_1() failed.");
				}
				mtm_free(mtm0);

				break;

			case ISI_2:
				if (!(isi_2(&itv, mtm0, mtm1))) {
					return trios_error(MSG,
							   "isi_basic: isi_2() failed.");
				}
				mtm_free(mtm0);
				break;

			case ISI_3:
				if (isi3_step == 0) {
					trios_warning
					    ("0 is not a valid step. 1 will be used\n");
					isi3_step = 1;
				}
				/* REMEMBER: isi_3() will free "mtm0"  and modify "itv_i" */

				if (!(isi_3(&itv, mtm0, mtm1, isi3_step))) {
					return trios_error(MSG,
							   "isi_basic: isi_3() failed.");
				}
				mtm_free(mtm0);
				break;

			default:
				return trios_error(1, "Invalid ISI type.");
			}

			/* For now, the intervals will be copied (added) to
			   the list p->out. They will be inserted into a 
			   itv_t structure at the end                        */
			add_itv_list_by_copying(&p_out, itv);
			itv_o->nitv += itv->nitv;

			/* Attention : do not release itv at this point because
			   it will be used as the starting interval for the next
			   iteration !!!!                                        */
		} else {
			mtm_free(mtm0);
			i = k;
		}

		mtm = mtm1;
	}

	mtm_free(mtm);		/* the one that called this routine can not free mtm */

	itv_free(itv);

	itv_o->head = (int *)p_out;
	*itv_i = itv_o;

	return (1);
}

/*
     -------------------------------------------
     FUNCTION: isi_gen
     -------------------------------------------
*/

int /*+ Purpose: ISI with groupment of examples            + */ isi_gen(
									       itv_t ** itv_i,	/*+ In: starting intervals                             + */
									       mtm_t * mtm_i,	/*+ In: set of classified examples                     + */
									       int multi,	/*+ In: if 1, indicates multi-classification           + */
									       int isi_type,	/*+ In: ISI type (ISI-0, IIS-1, ISI-2, ISI-3)          + */
									       int isi3_step,	/*+ In: minimum cover computation step for ISI-3       + */
									       int log_step,	/*+ In: If not ZERO, log file will be written at each
												   time a multiple of log_step examples is processed. + */
									       char *log_file,	/*+ In: log file name                                  + */
									       char *tmp_bas_file,	/*+ In: File in which the set of interval at each
													   step will be written                       + */
									       window_t * win,	/*+ In: window (needed if log_step > 0)                + */
									       int *ngrouped,	/*+ Out: number of intervals after grouping      + */
									       unsigned long *time_usr,	/*+ Out: grouping time                        + */
									       unsigned long *time_sys	/*+ Out: grouping time                        + */
    )
/*+ Return: 1 on success, 0 on failure                                     +*/
{
	itv_t *itv, *itv_o;
	mtm_t *mtm0, *mtm1, *mtm;
	itv_BX *itv_list1, *itv_list2 = NULL, *p_out;
	int map_type1, map_type2, wsize1, wsize2;
	int lb[256];
	int i, k, count;

	unsigned long int tu0, tuf, ts0, tsf;
	/*struct rusage rusage; */

#ifdef _DEBUG_
	trios_debug("Starting isi_gen()");
#endif

	/*GLlog_step = log_step;
	if (log_step) {
		/*
		   strcpy(GLlog_file, log_file) ;
		   strcat(GLlog_file, ".tmp") ;
		 
		GLlog_file = log_file;
		GLtmp_bas_file = tmp_bas_file;
		GLwin = win;
	}*/

	itv = *itv_i;
	mtm = mtm_i;

	/* get types */
	map_type1 = itv_get_type(itv);
	map_type2 = mtm_get_type(mtm);

	if ((map_type2 != BB) && (map_type2 != BG)) {
		return trios_error(1,
				   "ISI not implemented for non-binary patterns");
	}

	if (map_type1 != map_type2) {
		trios_warning
		    ("Classified examples and starting intervals have different type.");
	}
	if (multi) {
		map_type2 = BG;
	} else {
		map_type2 = BB;
	}

	/* get wsize */
	wsize1 = itv_get_wsize(itv);
	wsize2 = mtm_get_wsize(mtm);

	if (wsize1 != wsize2) {
		return trios_error(1,
				   "Incompatible size. ISI can't be applied.");
	}

	/* No matter if the case is multi or not, label ZERO is */
	/* always the first to be extracted.                    */
	/* If case is binary, extraction of all ZEROS finishes  */
	/* the process. If case is multi, then first all ZEROS  */
	/* are extracted and then the elements of the classes   */
	/* with fewer examples are processed first              */
	/* The most dense class is not processed                */

	if (multi) {
		if ((k = isi_sort_labels(mtm, lb)) == -1) {
			return trios_error(MSG,
					   "isi_gen: isi_sort_labels() failed.");
		}
	}
	/* if output is binary, then examples with label 0 will be */
	/* extracted from the input interval                       */
	else {
		k = 1;
		lb[0] = 0;
		lb[1] = 1;
	}

#ifdef _DEBUG_
	trios_debug("Examples ordering: done. (k=%d)", k);
#endif

	/* allocate output itv_t structure */
	itv_o = itv_create(wsize2, map_type2, lb[0]);
	if (!itv_o) {
		return trios_error(1, "Memory allocation error.");
	}

	/* change all intervals with label 1 to 0 */
	/* this is done just to avoid a special treatment for */
	/* the first iteration of the loop below */
	itv_label(itv, 1, 0);

	p_out = NULL;

	*ngrouped = 0;
	*time_usr = 0;
	*time_sys = 0;

	for (i = 0; i < k; i++) {

		count = 0;

		/* change all intervals with label lb[i] to lb[i+1] */
		itv_label(itv, lb[i], lb[i + 1]);

		/* put in mtm0 all minterms of mtm with label lb[i], and
		   in mtm1 all other minterms of mtm                 */
		if (!mtm_sep(mtm, lb[i], &mtm0, &mtm1)) {
			return trios_error(MSG, "isi_basic: mtm_sep() failed.");
		}
		mtm_free(mtm);

#ifdef _DEBUG_1_
		trios_debug("Examples %d before groupment = %d", lb[i],
			    mtm0->nmtm);
#endif

		if (mtm0->nmtm) {

			/*getrusage(RUSAGE_SELF, &rusage);
			   tu0 = rusage.ru_utime.tv_sec ;
			   ts0 = rusage.ru_stime.tv_sec ; */

			/* convert negative examples (those with label lb[i])
			   into (trivial) intervals */
			/*itv_list1 = build_itv_from_mtm(mtm0, lb[i]) ; */
			if (!itv_list1) {
				return trios_error(MSG,
						   "isi_gen: build_itv_from_mtm() failed.");
			}

			/* Apply the Quine-McCluskey's procedure to group minterms
			   with label lb[i]                                        */
			/*itv_list2 = pquine(itv_list1, wsize1) ; */
			if (!itv_list2) {
				return trios_error(MSG,
						   "isi_gen: quine_like() failed.");
			}

			/*getrusage(RUSAGE_SELF, &rusage);
			   tuf = rusage.ru_utime.tv_sec ;
			   tsf = rusage.ru_stime.tv_sec ;

			   *time_usr = *time_usr + (tuf - tu0);
			   *time_sys = *time_sys + (tsf - ts0); */

#ifdef _DEBUG_
			trios_debug("Grouping label %d done.", lb[i]);
#endif
		}

		/* There are examples to be extracted and covered */
		if (mtm0->nmtm != 0) {
			if (mtm1->nmtm != 0) {

				switch (isi_type) {

				case ISI_0:

					if (!(count = gisi_0(itv, itv_list2))) {
						return trios_error(MSG,
								   "isi_gen: gisi_0() failed.");
					}

					mtm_free(mtm0);

					if (!itv_filter(mtm1, itv)) {
						return trios_error(MSG,
								   "isi_gen: itv_filter() failed.");
					}

					if (!p_cover(mtm1, itv)) {
						return trios_error(MSG,
								   "isi_gen: p_cover() failed.");
					}
					break;

				case ISI_1:

					if (!
					    (count =
					     gisi_1(itv, itv_list2, mtm1))) {
						return trios_error(MSG,
								   "isi_gen: gisi_1() failed.");
					}
					mtm_free(mtm0);

					if (!itv_filter(mtm1, itv)) {
						return trios_error(MSG,
								   "isi_gen: itv_filter() failed.");
					}

					if (!p_cover(mtm1, itv)) {
						return trios_error(MSG,
								   "isi_gen: p_cover() failed.");
					}
					break;

				case ISI_3:
					if (isi3_step == 0) {
						trios_warning
						    ("0 is not a valid step. 1 will be used\n");
					}

					if (!
					    (count =
					     gisi_3(itv, itv_list2, mtm1,
						    isi3_step))) {
						return trios_error(MSG,
								   "isi_gen: gisi_3() failed.");
					}
					mtm_free(mtm0);
					break;

				default:
					return trios_error(1,
							   "ISI-2: Not implemented yet");
					break;
				}

				/* For now, the intervals will be copied (added) to
				   the list p->out. They will be inserted into a 
				   itv_t structure at the end                        */
				add_itv_list_by_copying(&p_out, itv);
				itv_o->nitv += itv->nitv;

#ifdef _DEBUG_1_
				trios_debug("Intervals with label %d = %d",
					    lb[i], count);
#endif
				*ngrouped = *ngrouped + count;
				/* Attention : do not release itv at this point because
				   it will be used as the starting interval for the next
				   iteration !!!!                                        */
			} else {
				mtm_free(mtm0);
			}
		} else {
			mtm_free(mtm0);
			add_itv_list_by_copying(&p_out, itv);
			itv_o->nitv += itv->nitv;
		}

		mtm = mtm1;

	}

	mtm_free(mtm);
	itv_free(itv);

	itv_o->head = (int *)p_out;
	*itv_i = itv_o;

	return (1);
}

/*
     -------------------------------------------
     FUNCTION: isi_sort_labels
     -------------------------------------------
*/

int				/*+ Purpose: sort labels of a classified examples set 
				   and return them in a vector                   + */ isi_sort_labels(
													     mtm_t * mtm,	/*+ In: examples to be removed                       + */
													     int *lb	/*+ In/Out: pre-allocated vector for the labels      + */
    )
/*+ Return: 1 on success, 0 on failure                                     +*/
{
	int i, j, aux, k;
	freq_node *p;
	int fq[256];

	/* lb[0] = 0 always. fq[ lb[i] ] <= fq[ lb[i+1] ], i>=1 */

	p = mtm->mtm_freq;
	lb[0] = 0;		/* first is always 0 */
	k = 1;

	while (p) {
		if (p->label != 0) {
			if (k > 255) {
				trios_fatal
				    ("Buffer overflow. Notify the support team (!)");
			}
			lb[k] = p->label;
			fq[k] = p->freq;
			k++;
		}
		p = p->next;
	}

	for (i = 1; i < k - 1; i++) {
		for (j = i + 1; j < k; j++) {
			if (fq[j] < fq[i]) {
				aux = fq[i];
				fq[i] = fq[j];
				fq[j] = aux;
				aux = lb[i];
				lb[i] = lb[j];
				lb[j] = aux;
			}
		}
	}

	/* OLD version : counting sort
	   k = 0;
	   lb[0] = 0 ; 
	   for(i=0; i<256; i++) {
	   fq[i] = 0 ;
	   }

	   while(p) {
	   if(p->label) {
	   k++ ;
	   if(k>255) {
	   trios_fatal("Buffer overflow. Notify the support team") ;
	   } 
	   lb[k] = p->label ;
	   }
	   p = p->next ;
	   }

	   q = (mtm_BX *)mtm->mtm_data ;
	   for(i=0; i<mtm->nmtm; i++) {
	   j=0 ;
	   while((lb[j]!=q[i].label) && (j<=k)) j++ ;
	   if(j > k) {
	   (void)trios_error(1, "Inconsistent data. Check input data.") ;
	   return(-1) ;
	   }   
	   fq[j] = fq[j] + q[i].fq ;
	   }

	   for(i=1; i<k; i++) {
	   for(j=i+1; j<=k; j++) {
	   if( fq[i] > fq[j] ) {
	   aux = fq[i] ;
	   fq[i] = fq[j] ;
	   fq[j] = aux ;
	   aux = lb[i] ;
	   lb[i] = lb[j] ;
	   lb[j] = aux ;
	   }
	   }
	   }
	 */

	return (k);

}

/*
     -------------------------------------------
     FUNCTION: isi_0
     -------------------------------------------
*/

int /*+ Purpose: ISI-0 by points                               + */ isi_0(
										 itv_t ** itv_i,	/*+ In/Out: intervals to be splitted/result          + */
										 mtm_t * mtm_i	/*+ In: examples to be removed                       + */
    )
/*+ Return: 1 on success, 0 on failure                                     +*/
{

	mtm_BX *q;
	itv_t *itv;
	itv_BX *p_list, *tmp0, *tmp, *p_new, *p_kill, *p_split;
	int stop_flag, covered;
	int wzip, nitv_new;
	int i, j, nmtm;

	itv = *itv_i;
	nmtm = mtm_get_nmtm(mtm_i);
	wzip = itv_get_wzip(itv);

	/* ----- program logic scheme ---------------------- *

	   for each example in "mtm0"
	   {
	   let mtm0[i] be the current example

	   p_kill = set of intervals that intercept example mtm0[i]

	   itv->head = set of intervals that does not intercept current example

	   for each interval em p_kill

	   p_split = set of intervals resulted from splitting p_kill
	   by the current example

	   p_new = set of intervals in p_split that are not covered by
	   some interval in itv->head ;

	   add p_new to itv_head

	   proceed to the next example

	   }
	   * ---------------------------------------------------- */

	/* for each example in "mtm0" */
	q = (mtm_BX *) mtm_i->mtm_data;

	for (i = 0; i < nmtm; i++) {

		
		p_list = (itv_BX *) itv->head;
		itv->head = NULL;
		itv->nitv = 0;
		p_kill = NULL;
		nitv_new = 0;

		/* put the intervals of p_list that intercept "q[i].wpat" in p_kill,
		   otherwise put in itv->head */

		while (p_list != NULL) {

			stop_flag = 0;
			tmp = p_list->next;
			j = 0;

			while ((j < wzip) && (!stop_flag)) {
				if (((p_list->A[j] & q[i].wpat[j]) !=
				     p_list->A[j])
				    || ((p_list->B[j] & q[i].wpat[j]) !=
					q[i].wpat[j])) {
					stop_flag = 1;
				}
				j++;
			}

			if (stop_flag) {
				p_list->next = (itv_BX *) itv->head;
				itv->head = (int *)p_list;
				itv->nitv++;
			} else {
				p_list->next = p_kill;
				p_kill = p_list;
			}
			p_list = tmp;

		}

		p_list = (itv_BX *) itv->head;	/* the list of intervals that do not contain
						   the current example to be extracted ... */
		p_new = NULL;

		/* for all intervals that intercepts q[i].wpat do ... */
		while (p_kill) {
			tmp0 = p_kill->next;

			p_split = NULL;

			/* compute p_kill-X */

			p_split =
			    itv_split_bywpat(p_kill, q[i].wpat, itv->wsize,
					     wzip);

			/* p_kill is an obsolete interval. Let us free it */
			itv_nodebx_free(p_kill);

			/* check if each new generated interval is already in p_list */
			while (p_split) {

				covered = 0;
				Is_node_covered(covered, p_split, p_list, wzip);

				tmp = p_split->next;

				if (!covered) {
					p_split->next = p_new;
					p_new = p_split;
					nitv_new++;
				} else {
					itv_nodebx_free(p_split);
				}
				p_split = tmp;
			}

			p_kill = tmp0;

		}

		tmp = p_new;
		if (p_new) {
			while (p_new->next)
				p_new = p_new->next;
			p_new->next = p_list;
			p_list = tmp;
		}
		itv->head = (int *)p_list;
		itv_set_nitv(itv, itv->nitv + nitv_new);

	}

	return (1);
}

/*
     -------------------------------------------
     FUNCTION: isi_1
     -------------------------------------------
*/

int /*+ Purpose: ISI-1 (point case)                            + */ isi_1(
										 itv_t ** itv_i,	/*+ In/Out: Intervals to be splitted / result       + */
										 mtm_t * mtm_i1,	/*+ In: examples to be removed from intervals       + */
										 mtm_t * mtm_i2	/*+ In: examples that need to be covered            + */
    )
/*+ Return: 1 on success, 0 on failure                                     +*/
{
	mtm_BX *q;
	itv_t *itv;
	itv_BX *p_list, *tmp0, *tmp, *p_new, *p_kill, *p_split;
	int stop_flag, covered;
	int wzip, nitv_new;
	int i, j, nmtm;

	itv = *itv_i;
	nmtm = mtm_get_nmtm(mtm_i1);
	wzip = itv_get_wzip(itv);

	/* ----- program logic scheme ---------------------- *

	   for each example in "mtm_i1"
	   {
	   let mtm_i1[i] be the current example

	   p_kill = set of intervals that intercept example mtm_i1[i]

	   itv->head = set of intervals that does not intercept current example

	   for each interval em p_kill

	   p_split = set of intervals resulted from splitting p_kill
	   by the current example

	   p_new = set of intervals in p_split that are not covered by
	   some interval in itv->head and contains at least
	   one example from mtm1 !!  <----- ***

	   add p_new to itv_head

	   proceed to the next example

	   }
	   * ---------------------------------------------------- */

	/* for each example in "mtm_i1" */
	q = (mtm_BX *) mtm_i1->mtm_data;

	for (i = 0; i < nmtm; i++) {


		p_list = (itv_BX *) itv->head;
		itv->head = NULL;
		itv->nitv = 0;
		p_kill = NULL;
		nitv_new = 0;

		/* put the intervals of p_list that intercept "q[i].wpat" in p_kill,
		   otherwise put in itv->head */

		while (p_list != NULL) {

			stop_flag = 0;
			tmp = p_list->next;
			j = 0;

			while ((j < wzip) && (!stop_flag)) {
				if (((p_list->A[j] & q[i].wpat[j]) !=
				     p_list->A[j])
				    || ((p_list->B[j] & q[i].wpat[j]) !=
					q[i].wpat[j])) {
					stop_flag = 1;
				}
				j++;
			}

			if (stop_flag) {
				p_list->next = (itv_BX *) itv->head;
				itv->head = (int *)p_list;
				itv->nitv++;
			} else {
				p_list->next = p_kill;
				p_kill = p_list;
			}
			p_list = tmp;

		}

		p_list = (itv_BX *) itv->head;	/* the list of intervals that do not contain
						   the current example to be extracted ... */
		p_new = NULL;

		/* for all intervals that intercepts q[i].wpat do ... */
		while (p_kill) {
			tmp0 = p_kill->next;

			p_split = NULL;

			/* compute p_kill-X */

			p_split =
			    itv_split_bywpat(p_kill, q[i].wpat, itv->wsize,
					     wzip);

			/* p_kill is an obsolete interval. Let us free it */
			itv_nodebx_free(p_kill);

			/* check if each new generated interval is already in p_list */
			while (p_split) {

				if (itv_contain_mtm(p_split, mtm_i2, wzip)) {
					covered = 0;
					Is_node_covered(covered, p_split,
							p_list, wzip);
				} else {
					covered = 1;
				}
				tmp = p_split->next;

				if (!covered) {
					p_split->next = p_new;
					p_new = p_split;
					nitv_new++;
				} else {
					itv_nodebx_free(p_split);
				}
				p_split = tmp;
			}

			p_kill = tmp0;

		}

		tmp = p_new;
		if (p_new) {
			while (p_new->next)
				p_new = p_new->next;
			p_new->next = p_list;
			p_list = tmp;
		}
		itv->head = (int *)p_list;
		itv_set_nitv(itv, itv->nitv + nitv_new);

	}

	if (!itv_filter(mtm_i2, itv)) {
		return trios_error(MSG, "isi_1: itv_filter() failed.");
	}

	if (!p_cover(mtm_i2, itv)) {
		return trios_error(MSG, "isi_1: p_cover() failed.");
	}

	return (1);
}

/*
     -------------------------------------------
     FUNCTION: isi_2
     -------------------------------------------
*/

int /*+ Purpose: ISI-2 (point case)                            + */ isi_2(
										 itv_t ** itv_i,	/*+ In/Out: intervals to be splitted / result       + */
										 mtm_t * mtm_i1,	/*+ In: examples to be removed from the intervals   + */
										 mtm_t * mtm_i2	/*+ In: examples that need to be covered            + */
    )
/*+ Return: 1 on success, 0 on failure                                     +*/
{
	idx_ll *ll_head, *ll_aux, *ll_yes, *ll_no;
	mtm_BX *q;
	itv_t *itv;
	itv_BX *p_list, *tmp0, *tmp, *p_new, *p_kill, *p_split;
	int stop_flag, covered;
	int wzip, nitv_new;
	int i, j, nmtm;

	/* ----- program logic scheme ---------------------- *

	   for each example in "mtm_i1"
	   {
	   let mtm_i1[i] be the current example

	   p_kill = set of intervals that intercept example mtm_i1[i]

	   itv->head = set of intervals that does not intercept current example

	   for each interval em p_kill

	   p_split = set of intervals resulted from splitting p_kill
	   by the current example

	   p_new = set of intervals in p_split that are not covered by
	   some interval in itv->head and contains at least
	   one example from mtm1 !!  <----- ***

	   se p_new union itv->head cover pos. examples, break   <--- ***

	   add p_new to itv_head

	   proceed to the next example

	   }
	   * ---------------------------------------------------- */

	/* create a list of indexes that point to positive minterms */
	if (!(ll_head = mtm_tbl2ll(mtm_i1))) {
		return trios_error(MSG, "isi_2: mtm_tbl2ll() failed.");
	}
	ll_yes = ll_no = 0;

	itv = *itv_i;
	nmtm = mtm_get_nmtm(mtm_i1);
	wzip = itv_get_wzip(itv);

	/* for each example in "mtm_i1" */
	q = (mtm_BX *) mtm_i1->mtm_data;

	for (i = 0; i < nmtm; i++) {


		p_list = (itv_BX *) itv->head;
		itv->head = NULL;
		itv->nitv = 0;
		p_kill = NULL;
		nitv_new = 0;

		/* put the intervals of p_list that intercept "q[i].wpat" in p_kill,
		   otherwise put in itv->head */

		while (p_list != NULL) {

			stop_flag = 0;
			tmp = p_list->next;
			j = 0;

			while ((j < wzip) && (!stop_flag)) {
				if (((p_list->A[j] & q[i].wpat[j]) !=
				     p_list->A[j])
				    || ((p_list->B[j] & q[i].wpat[j]) !=
					q[i].wpat[j])) {
					stop_flag = 1;
				}
				j++;
			}

			if (stop_flag) {
				p_list->next = (itv_BX *) itv->head;
				itv->head = (int *)p_list;
				itv->nitv++;
			} else {
				p_list->next = p_kill;
				p_kill = p_list;
			}
			p_list = tmp;

		}

		p_list = (itv_BX *) itv->head;	/* the list of intervals that do not contain
						   the current example to be extracted ... */
		p_new = NULL;

		/* */
		while (ll_head) {
			ll_aux = ll_head->next;
			if (itv_cover_wpat(p_list, q[ll_head->idx].wpat, wzip)) {
				ll_head->next = ll_yes;
				ll_yes = ll_head;
			} else {
				ll_head->next = ll_no;
				ll_no = ll_head;
			}
			ll_head = ll_aux;
		}

		/* for all intervals in p_kill that intercepts q[i].wpat do ... */
		while (p_kill) {
			tmp0 = p_kill->next;

			p_split = NULL;

			/* compute p_kill-X */

			p_split =
			    itv_split_bywpat(p_kill, q[i].wpat, itv->wsize,
					     wzip);

			/* p_kill is an obsolete interval. Let us free it */
			itv_nodebx_free(p_kill);

			/* check if each new generated interval is already in p_list */
			while (p_split) {

				if (itv_contain_mtm(p_split, mtm_i2, wzip)) {
					covered = 0;
					Is_node_covered(covered, p_split,
							p_list, wzip);
				} else {
					covered = 1;
				}
				tmp = p_split->next;

				if (!covered) {
					p_split->next = p_new;
					p_new = p_split;
					nitv_new++;
				} else {
					itv_nodebx_free(p_split);
				}
				p_split = tmp;
			}

			/* HERE I have to check if those intervals in p_list */
			/* plus the ones in p_new are sufficient to cover    */
			/* all examples in mtm_i2                            */
			ll_head = NULL;
			while (ll_no) {
				ll_aux = ll_no->next;
				if (itv_cover_wpat
				    (p_list, q[ll_no->idx].wpat, wzip)) {
					ll_no->next = ll_yes;
					ll_yes = ll_no;
				} else {
					ll_no->next = ll_head;
					ll_head = ll_no;
				}
				ll_no = ll_aux;
			}
			ll_no = ll_head;
			if (!ll_no) {
				if (tmp0) {
					tmp = tmp0;
					while (tmp0->next)
						tmp0 = tmp0->next;
					itv_nodebx_free(tmp0);
					tmp0 = NULL;
				}
			}

			p_kill = tmp0;

		}

		ll_head = ll_yes;
		ll_yes = NULL;

		tmp = p_new;
		if (p_new) {
			while (p_new->next)
				p_new = p_new->next;
			p_new->next = p_list;
			p_list = tmp;
		}
		itv->head = (int *)p_list;
		itv_set_nitv(itv, itv->nitv + nitv_new);

	}

	return (1);
}

/*
     -------------------------------------------
     FUNCTION: isi_3
     -------------------------------------------
*/

int /*+ Purpose: ISI-3 (point case)                            + */ isi_3(
										 itv_t ** itv_i,	/*+ In/Out: intervals to be splitted / result       + */
										 mtm_t * mtm_i1,	/*+ In: examples to be removed from the intervals   + */
										 mtm_t * mtm_i2,	/*+ In: examples that need to be covered            + */
										 int isi3_step	/*+ In: minimum cover computation step for ISI-3    + */
    )
/*+ Return: 1 on success, 0 on failure                                     +*/
{
	mtm_BX *q;
	itv_t *itv;
	itv_BX *p_list, *tmp0, *tmp, *p_new, *p_kill, *p_split;
	int stop_flag, covered;
	int wzip, nitv_new;
	int i, j, nmtm;

#ifdef _DEBUG_2_
	int i_aux;
	itv_BX *aux;
	trios_debug("isi_3");
#endif


	itv = *itv_i;
	nmtm = mtm_get_nmtm(mtm_i1);
	wzip = itv_get_wzip(itv);


	/* ----- program logic scheme ---------------------- *

	   for each example in "mtm_i1"
	   {
	   let mtm_i1[i] be the current example

	   p_kill = set of intervals that intercept example mtm_i1[i]

	   itv->head = set of intervals that does not intercept current example

	   for each interval em p_kill

	   p_split = set of intervals resulted from splitting p_kill
	   by the current example

	   p_new = set of intervals in p_split that are not covered by
	   some interval in itv->head and contains at least

	   one example from mtm1 !!  <----- ***

	   add p_new to itv_head

	   select essential(itv->head) !!  <----- ***

	   proceed to the next example

	   }
	   * ---------------------------------------------------- */

	/* for each example in "mtm_i1" */
	q = (mtm_BX *) mtm_i1->mtm_data;


	for (i = 0; i < nmtm; i++) {


		p_list = (itv_BX *) itv->head;
		itv->head = NULL;
		itv->nitv = 0;
		p_kill = NULL;
		nitv_new = 0;

		/* put the intervals of p_list that intercept "q[i].wpat" in p_kill,
		   otherwise put in itv->head. In other words, p_kill will contain
		   the intervals that will be splitted  */

		while (p_list != NULL) {
			stop_flag = 0;
			tmp = p_list->next;
			j = 0;

			while ((j < wzip) && (!stop_flag)) {
				if (((p_list->A[j] & q[i].wpat[j]) !=
				     p_list->A[j])
				    || ((p_list->B[j] & q[i].wpat[j]) !=
					q[i].wpat[j])) {
					stop_flag = 1;
				}
				j++;
			}

			if (stop_flag) {
				p_list->next = (itv_BX *) itv->head;
				itv->head = (int *)p_list;
				itv->nitv++;
			} else {
				p_list->next = p_kill;
				p_kill = p_list;
			}
			p_list = tmp;

		}

		p_list = (itv_BX *) itv->head;	/* the list of intervals that do not contain
						   the current example to be extracted ... */
		p_new = NULL;

		/* for all intervals that intercepts q[i].wpat do ... */
		while (p_kill) {
			tmp0 = p_kill->next;

			p_split = NULL;

			/* compute p_kill-X */

			p_split =
			    itv_split_bywpat(p_kill, q[i].wpat, itv->wsize,
					     wzip);

			/* p_kill is an obsolete interval. Let us free it  */
			itv_nodebx_free(p_kill);

			/* check if each new generated interval is already in p_list */
			while (p_split) {

				if (itv_contain_mtm(p_split, mtm_i2, wzip)) {
					covered = 0;
					Is_node_covered(covered, p_split,
							p_list, wzip);
				} else {
					covered = 1;
				}
				tmp = p_split->next;

				if (!covered) {
					p_split->next = p_new;
					p_new = p_split;
					nitv_new++;
				} else {
					itv_nodebx_free(p_split);
				}
				p_split = tmp;
			}

			p_kill = tmp0;

		}

		tmp = p_new;
		if (p_new) {
			while (p_new->next)
				p_new = p_new->next;
			p_new->next = p_list;
			p_list = tmp;
		}
		itv->head = (int *)p_list;
		itv_set_nitv(itv, itv->nitv + nitv_new);

#ifdef _DEBUG_2_
		trios_debug("\nAFTER SPLITTING by example %x", q[i].wpat[0]);
		i_aux = 0;
		for (aux = (itv_BX *) itv->head; aux; aux = aux->next) {
			i_aux++;
			trios_debug("%d A=%x B=%x label=%d", i_aux, aux->A[0],
				    aux->B[0], aux->label);
		}
#endif

		/* if number of examples processed is a multiple of isi3_step
		   or if this is the last iteration, then compute minimum cover */
		if ((((i + 1) % isi3_step) == 0) || (i == nmtm - 1)) {

			/* don't need to filter intervals that do not contain
			   1-labeled examples, since that is already done before 

			   if(!itv_filter(mtm_i2, itv)) {
			   return trios_error(MSG, "isi_3: itv_filter() failed.") ;
			   }
			 */


			if (!p_cover(mtm_i2, itv)) {
				return trios_error(MSG,
						   "isi_3: p_cover() failed.");
			}
		}

#ifdef _DEBUG_2_
		trios_debug("AFTER MINIMUN COVER SELECTION");
		i_aux = 0;
		for (aux = (itv_BX *) itv->head; aux; aux = aux->next) {
			i_aux++;
			trios_debug("%d A=%x B=%x", i_aux, aux->A[0],
				    aux->B[0]);
		}
		trios_debug("\n\n");
#endif

	}


	return (1);
}

/*
     -------------------------------------------
     FUNCTION: gisi_0
     -------------------------------------------
*/

int				/*+ Purpose: split all intervals of a list (of intervals)
				   by each interval in a second list             + */ gisi_0(
												    itv_t * itv,	/*+ In/Out: pointer to intervals list              + */
												    itv_BX * itv_list	/*+ In: the list of intervals that will be removed + */
    )
/*+ Return: number of intervals in the second list on success, 0 on failure +*/
{
	itv_BX *p, *tmp, *p_new, *p_kill, *p_split, *I;
	int stop_flag, covered;
	int wzip, nitv_new;
	int j, count;

	wzip = itv_get_wzip(itv);
	nitv_new = 0;
	count = 0;

	while (itv_list) {

		I = itv_list;

		p = (itv_BX *) itv->head;
		itv->head = NULL;
		p_kill = NULL;

		/* put all intervals of p that intercept I in p_kill, otherwise keep
		   them in p. In this loop itv->head is used as a temporary variable */
		while (p != NULL) {

			stop_flag = 0;
			tmp = p->next;
			j = 0;

			while ((j < wzip) && (!stop_flag)) {
				if (((p->A[j] | I->A[j]) & (p->B[j] & I->
							    B[j])) !=
				    (p->A[j] | I->A[j])) {
					stop_flag = 1;
				}
				j++;
			}

			if (stop_flag) {
				p->next = (itv_BX *) itv->head;
				itv->head = (int *)p;
			} else {
				p->next = p_kill;
				p_kill = p;
				itv->nitv--;
			}
			p = tmp;
		}

		p = (itv_BX *) itv->head;
		p_new = NULL;

#ifdef _DEBUG_2_
		trios_debug("os que NAO interceptam I=[%x,%x]", I->A[0],
			    I->B[0]);
		for (tmp = p; tmp; tmp = tmp->next) {
			trios_debug("A=%x B=%x", tmp->A[0], tmp->B[0]);
		}
		trios_debug("os que interceptam I");
		for (tmp = p_kill; tmp; tmp = tmp->next) {
			trios_debug("A=%x B=%x", tmp->A[0], tmp->B[0]);
		}
#endif

		/* for all intervals that intercepts I do ... */
		while (p_kill) {

			p_split = NULL;
			p_split = itv_split_byitv(p_kill, I, itv->wsize, wzip);

			while (p_split) {
				/* check if p_split is in p */
				covered = 0;
				Is_node_covered(covered, p_split, p, wzip);

				tmp = p_split->next;

				if (!covered) {
					p_split->next = p_new;
					p_new = p_split;
					nitv_new++;

#ifdef _DEBUG_2_
					trios_debug("Criou new node A=%x B=%x",
						    p_new->A[0], p_new->B[0]);
#endif

				} else {
					itv_nodebx_free(p_split);
				}

				p_split = tmp;
			}

			tmp = p_kill->next;
			itv_nodebx_free(p_kill);
			p_kill = tmp;

		}

		/* buil the list to be returned */
		Concatenate_lists(p, p_new);
		itv->head = (int *)p;
		itv_set_nitv(itv, itv->nitv + nitv_new);

		itv_list = itv_list->next;
		count++;
	}

	return (count);
}

/*
     -------------------------------------------
     FUNCTION: gisi_1
     -------------------------------------------
*/

int				/*+ Purpose: split all intervals of a list (of intervals)
				   by each interval in another list. Eliminates
				   new generated subintervals that do not contain
				   any example of mtm                             + */ gisi_1(
												     itv_t * itv,	/*+ In/Out: pointer to intervals list               + */
												     itv_BX * itv_list,	/*+ In: the interval that will be removed           + */
												     mtm_t * mtm	/*+ In: examples to be covered                      + */
    )
/*+ Return: number of intervals processed on success, 0 on failure           +*/
{
	itv_BX *p, *tmp, *p_new, *p_kill, *p_split, *I;
	int stop_flag, covered;
	int wzip, nitv_new;
	int j, count;

	wzip = itv_get_wzip(itv);
	nitv_new = 0;
	count = 0;

	while (itv_list) {

		I = itv_list;


		p = (itv_BX *) itv->head;
		itv->head = NULL;
		p_kill = NULL;

		/* put all intervals of p that intercept I in p_kill, otherwise keep
		   them in p */

		while (p != NULL) {

			stop_flag = 0;
			tmp = p->next;
			j = 0;

			while ((j < wzip) && (!stop_flag)) {
				if (((p->A[j] | I->A[j]) & (p->B[j] & I->
							    B[j])) !=
				    (p->A[j] | I->A[j])) {
					stop_flag = 1;
				}
				j++;
			}

			if (stop_flag) {
				p->next = (itv_BX *) itv->head;
				itv->head = (int *)p;
			} else {
				p->next = p_kill;
				p_kill = p;
				itv->nitv--;
			}
			p = tmp;
		}

		p = (itv_BX *) itv->head;
		p_new = NULL;

#ifdef _DEBUG_2_
		trios_debug("os que NAO interceptam I=[%x,%x]", I->A[0],
			    I->B[0]);
		for (tmp = p; tmp; tmp = tmp->next) {
			trios_debug("A=%x B=%x", tmp->A[0], tmp->B[0]);
		}
		trios_debug("os que interceptam I");
		for (tmp = p_kill; tmp; tmp = tmp->next) {
			trios_debug("A=%x B=%x", tmp->A[0], tmp->B[0]);
		}
#endif

		/* for all intervals that intercepts I do ... */
		while (p_kill) {

			p_split = NULL;

			/* compute L1-I */

			p_split = itv_split_byitv(p_kill, I, itv->wsize, wzip);

			while (p_split) {

				tmp = p_split->next;
				covered = 1;

				/* check if p_split contaim some positive example ... */

				if (itv_contain_mtm(p_split, mtm, wzip)) {

					/* check if p_split is in p */
					covered = 0;
					Is_node_covered(covered, p_split, p,
							wzip);

				}

				if (!covered) {
					p_split->next = p_new;
					p_new = p_split;
					nitv_new++;

#ifdef _DEBUG_2_
					trios_debug("Criou new node A=%x B=%x",
						    p_new->A[0], p_new->B[0]);
#endif

				} else {
					itv_nodebx_free(p_split);
				}

				p_split = tmp;
			}

			tmp = p_kill->next;
			itv_nodebx_free(p_kill);
			p_kill = tmp;

		}

		/* buil the list to be returned */
		Concatenate_lists(p, p_new);
		itv->head = (int *)p;
		itv_set_nitv(itv, itv->nitv + nitv_new);

		itv_list = itv_list->next;
		count++;
	}

	return (count);
}

/*
     -------------------------------------------
     FUNCTION: gisi_3
     -------------------------------------------
*/

int				/*+ Purpose: split all intervals of a list (of intervals)
				   by each interval in another interval list.
				   Computes minimum cover at each isi3_step intervals 
				   (of the list to be removed) are processed             + */ gisi_3(
													    itv_t * itv,	/*+ In/Out: list of intervals                     + */
													    itv_BX * itv_list,	/*+ In: list of intervals to be removed           + */
													    mtm_t * mtm,	/*+ In: examples to be covered                    + */
													    int isi3_step	/*+ In: minimum cover computation step for ISI-3  + */
    )
/*+ Return: number of intervals processed (removed) on success, 0 on failure +*/
{
	itv_BX *p, *tmp, *p_new, *p_kill, *p_split, *I;
	int stop_flag, covered;
	int wzip, nitv_new;
	int j, count;

	wzip = itv_get_wzip(itv);
	nitv_new = 0;
	count = 0;

	while (itv_list) {

		I = itv_list;


		p = (itv_BX *) itv->head;
		itv->head = NULL;
		p_kill = NULL;

		/* put all intervals of p that intercept I in p_kill, otherwise keep
		   them in p */

		while (p != NULL) {

			stop_flag = 0;
			tmp = p->next;
			j = 0;

			while ((j < wzip) && (!stop_flag)) {
				if (((p->A[j] | I->A[j]) & (p->B[j] & I->
							    B[j])) !=
				    (p->A[j] | I->A[j])) {
					stop_flag = 1;
				}
				j++;
			}

			if (stop_flag) {
				p->next = (itv_BX *) itv->head;
				itv->head = (int *)p;
			} else {
				p->next = p_kill;
				p_kill = p;
				itv->nitv--;
			}
			p = tmp;
		}

		p = (itv_BX *) itv->head;
		p_new = NULL;

#ifdef _DEBUG_2_
		trios_debug("os que NAO interceptam I=[%x,%x]", I->A[0],
			    I->B[0]);
		for (tmp = p; tmp; tmp = tmp->next) {
			trios_debug("A=%x B=%x", tmp->A[0], tmp->B[0]);
		}
		trios_debug("os que interceptam I");
		for (tmp = p_kill; tmp; tmp = tmp->next) {
			trios_debug("A=%x B=%x", tmp->A[0], tmp->B[0]);
		}
#endif

		/* for all intervals that intercepts I do ... */
		while (p_kill) {

			p_split = NULL;
			/* compute L1-I */

			p_split = itv_split_byitv(p_kill, I, itv->wsize, wzip);

			while (p_split) {

				tmp = p_split->next;
				covered = 1;

				/* check if p_split contaim some positive example ... */

				if (itv_contain_mtm(p_split, mtm, wzip)) {

					/* check if p_split is in p */
					covered = 0;
					Is_node_covered(covered, p_split, p,
							wzip);

				}

				if (!covered) {
					p_split->next = p_new;
					p_new = p_split;
					nitv_new++;

#ifdef _DEBUG_2_
					trios_debug("Criou new node A=%x B=%x",
						    p_new->A[0], p_new->B[0]);
#endif

				} else {
					itv_nodebx_free(p_split);
				}

				p_split = tmp;
			}

			tmp = p_kill->next;
			itv_nodebx_free(p_kill);
			p_kill = tmp;

		}

		/* buil the list to be returned */
		Concatenate_lists(p, p_new);
		itv->head = (int *)p;
		itv_set_nitv(itv, itv->nitv + nitv_new);

		count++;

		if ((count % isi3_step) == 0) {
			/* don't need to filter intervals that do not contain
			   1-labeled examples, since that is already done before 

			   if(!itv_filter(mtm, itv)) {
			   return trios_error(MSG, "gisi_3: itv_filter() failed.") ;
			   }
			 */

			if (!p_cover(mtm, itv)) {
				return trios_error(MSG,
						   "gisi_3: p_cover() failed.");
			}
		}

		itv_list = itv_list->next;
	}

	if ((count % isi3_step) != 0) {
		/* don't need to filter intervals that do not contain
		   1-labeled examples, since that is already done before 

		   if(!itv_filter(mtm, itv)) {
		   return trios_error(MSG, "gisi_3: itv_filter() failed.") ;
		   }
		 */

		if (!p_cover(mtm, itv)) {
			return trios_error(MSG, "gisi_3: p_cover() failed.");
		}
	}

	return (count);
}

/*
     -------------------------------------------
     FUNCTION: itv_split_bywpat
     -------------------------------------------
*/

itv_BX *			/*+ Purpose: Given an interval I, and a point X, computes
				   the set of intervals equivalent to I-{X}      + */
itv_split_bywpat(itv_BX * I,	/*+ In: interval to be splitted                      + */
		 unsigned int *wpat,	/*+ In: point which will split the interval      + */
		 int wsize,	/*+ In: w-pattern's size                             + */
		 int wzip	/*+ In: compacted w-pattern's size                   + */
    )
/*+ Return: a pointer to a set of intervals on success, NULL on failure    +*/
{
	itv_BX *p_list, *p;
	int j, lim, bit, i_wzip, nsh;

	lim = 0;		/* lim is a multiple of 32 (4bytes)   */
	i_wzip = 0;		/* index to wzip int's                */
	bit = 0;		/* the current bit position           */

	p_list = NULL;		/* pointer to the resulting set of intervals */

	/* Process each word of the w-pattern (wzip integers) */
	while (i_wzip < wzip) {

		lim = lim + NB;	/* the next stop   */
		nsh = 0;	/* number of shfts */

		while ((bit < wsize) && (bit < lim)) {
			if ((bitmsk[nsh] & ((wpat[i_wzip]) & (~I->A[i_wzip])))) {

				if (!(p = itv_nodebx_create(wzip))) {
					return (itv_BX *) trios_error(MSG,
								      "itv_split_bywpat: itv_nodebx_create() failed.");

				}

				for (j = 0; j < wzip; j++) {
					p->A[j] = I->A[j];
					p->B[j] = I->B[j];
				}

				p->B[i_wzip] = p->B[i_wzip] & (~bitmsk[nsh]);

				p->label = I->label;
				p->next = p_list;
				p_list = p;

			}

			else if ((bitmsk[nsh] & (~wpat[i_wzip] & I->B[i_wzip]))) {

				if (!(p = itv_nodebx_create(wzip))) {
					return (itv_BX *) trios_error(MSG,
								      "itv_split_bywpat: itv_nodebx_create() failed.");

				}

				for (j = 0; j < wzip; j++) {
					p->A[j] = I->A[j];
					p->B[j] = I->B[j];
				}
				p->A[i_wzip] |= bitmsk[nsh];	/* bitwise OR */
				p->label = I->label;
				p->next = p_list;
				p_list = p;

			}

			bit++;
			nsh++;
		}
		/* limit attained, increment i_wzip */
		i_wzip++;
	}

	return (p_list);
}

/*
     -------------------------------------------
     FUNCTION: itv_split_byitv
     -------------------------------------------
*/

itv_BX *			/*+ Purpose: Given two intervals, I1 and I2, computes a
				   third one, I, setting I = I1 \ I2             + */
itv_split_byitv(itv_BX * I1,	/*+ In: first interval                                     + */
		itv_BX * I2,	/*+ In: second interval                                    + */
		int wsize,	/*+ In: w-pattern's size                                   + */
		int wzip	/*+ In: compacted w-pattern's size                         + */
    )
/*+ Return: a pointer to an intervals set structure on success,
    NULL on failure                                                        +*/
{
	itv_BX *p, *p1;
	int j, lim, bit, i_wzip, nsh;

	lim = 0;		/* lim is a multiple of 32 (4bytes)    */
	i_wzip = 0;		/* index to wzip int's                   */
	bit = 0;		/* the current bit position */

	p = NULL;

	while (i_wzip < wzip) {

		lim = lim + NB;	/* the next stop */
		nsh = 0;

		while ((bit < wsize) && (bit < lim)) {
			if ((bitmsk[nsh] &
			     ((I2->A[i_wzip]) & (~I1->A[i_wzip])))) {

				if (!(p1 = itv_nodebx_create(wzip))) {
					return (itv_BX *) trios_error(MSG,
								      "itv_split_byitv: itv_nodebx_create() failed.");
				}

				for (j = 0; j < wzip; j++) {
					p1->A[j] = I1->A[j];
					p1->B[j] = I1->B[j];
				}

				p1->B[i_wzip] = p1->B[i_wzip] & (~bitmsk[nsh]);

				p1->label = I1->label;
				p1->next = p;
				p = p1;

			}

			else if ((bitmsk[nsh] &
				  (~I2->B[i_wzip] & I1->B[i_wzip]))) {

				if (!(p1 = itv_nodebx_create(wzip))) {
					return (itv_BX *) trios_error(MSG,
								      "itv_split_byitv: itv_nodebx_create() failed.");
				}

				for (j = 0; j < wzip; j++) {
					p1->A[j] = I1->A[j];
					p1->B[j] = I1->B[j];
				}
				p1->A[i_wzip] |= bitmsk[nsh];	/* bitwise OR */
				p1->label = I1->label;
				p1->next = p;
				p = p1;

			}

			bit++;
			nsh++;
		}
		/* limit attained, increment i_wzip */
		i_wzip++;
	}

	return (p);
}

/*
     -------------------------------------------
     FUNCTION: add_itv_list_by_copying
     -------------------------------------------
*/

int /*+ Purpose: copy a set of intervals to the end of a list   + */
add_itv_list_by_copying(
				      itv_BX ** p_list,	/*+ In/Out: list to which the set will be added     + */
				      itv_t * itv	/*+ In: the set of intervals to be copied           + */
    )
/*+ Return: 1 on success, 0 on failure                                     +*/
{
	itv_BX *p, *p1, *p2;
	int wzip, j;

	p1 = (itv_BX *) itv->head;
	p2 = *p_list;
	wzip = itv_get_wzip(itv);

	while (p1) {
		if (!(p = itv_nodebx_create(wzip))) {
			return trios_error(MSG,
					   "add...: itv_nodebx_create() failed.");
		}

		for (j = 0; j < wzip; j++) {
			p->A[j] = p1->A[j];
			p->B[j] = p1->B[j];
		}
		p->label = p1->label;

		p->next = p2;
		p2 = p;

		p1 = p1->next;
	}

	*p_list = p2;
	return (1);
}

/*
     -------------------------------------------
     FUNCTION: itv_contain_mtm
     -------------------------------------------
*/

int				/*+ Purpose: check wether the interval I contains any 
				   example of mtm                                + */ itv_contain_mtm(
													     itv_BX * I,	/*+ In: interval                                    + */
													     mtm_t * mtm,	/*+ In: classified examples set                     + */
													     int wzip	/*+ In: compacted w-pattern size                    + */
    )
/*+ Return: 1 if I contains any example, 0 otherwise                       +*/
{
	mtm_BX *q;
	int i, stop_flag, j;

	q = (mtm_BX *) mtm->mtm_data;

	for (i = 0; i < mtm_get_nmtm(mtm); i++) {

		stop_flag = 0;
		j = 0;

		while ((j < wzip) && (!stop_flag)) {
			if (((I->A[j] & q[i].wpat[j]) != I->A[j]) ||
			    ((I->B[j] & q[i].wpat[j]) != q[i].wpat[j])) {
				stop_flag = 1;
			}
			j++;
		}
		if (!stop_flag)
			return (1);
	}
	return (0);

}

/*
     -------------------------------------------
     FUNCTION: itv_filter
     -------------------------------------------
*/

int				/*+ Purpose: remove intervals that do not contain any
				   example of a given set of classified examples + */ itv_filter(
													mtm_t * mtm,	/*+ In: set of classified examples                  + */
													itv_t * itv	/*+ In/Out: set of intervals                        + */
    )
/*+ Return: 1 on success, 0 on failure                                     +*/
{

	itv_BX *p1, *p2;
	mtm_BX *q;
	int i, j, flg, stop_flag;

/* ------------------------------------------------------------
   Remove from the itv list those intervals that do not
   contain any example (in the mtm table).
   ------------------------------------------------------------ */

	p1 = (itv_BX *) itv->head;
	p2 = NULL;
	q = (mtm_BX *) mtm->mtm_data;

	while (p1 != NULL) {
		flg = 0;
		i = 0;

		while ((flg == 0) && (i < mtm->nmtm)) {

			stop_flag = 0;
			j = 0;

			while ((!stop_flag) && (j < itv->wzip)) {
				if (((p1->A[j] & q[i].wpat[j]) != p1->A[j]) ||
				    ((p1->B[j] & q[i].wpat[j]) !=
				     q[i].wpat[j])) {
					stop_flag = 1;
				}
				j++;
			}

			if (!stop_flag) {
				flg = 1;
			}
			i++;
		}

		if (flg == 0) {
			if (p2 == NULL) {
				itv->head = (int *)p1->next;
				itv_nodebx_free(p1);
				p1 = (itv_BX *) itv->head;
			} else {
				p2->next = p1->next;
				itv_nodebx_free(p1);
				p1 = p2->next;
			}
			itv->nitv--;
		} else {
			p2 = p1;
			p1 = p1->next;
		}
	}

	return (1);
}
