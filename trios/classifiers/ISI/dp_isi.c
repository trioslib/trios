#include "trios_win.h"
#include "trios_itv.h"
#include "trios_mtm.h"
#include "paclearn_local.h"
#include "trios_error.h"
#include <stdio.h>
#include <stdlib.h>

/*#define _DEBUG_*/

int				/*+ Given a set of intervals and a set of classified
				   examples, check whether the set of intervals is
				   consistent with the examples                           + */ itv_consistency(
														      itv_t * itv,	/*+ In: intervals set                     + */
														      mtm_t * mtm	/*+ set of examples                       + */
    );

itv_t *lisi_memory(mtm_t * __mtm, itv_t * itv, int isi_type, int isi3_step,
		   int multi, int group_flag)
{
	int ngrouped, zeros, nmtm, ones, i, j;
	freq_node *f, *new_f;
	mtm_t *mtm;

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
		trios_malloc(((mtm_BX *) mtm->mtm_data)[i].wpat,
			     itv->wzip * sizeof(unsigned int), itv_t *,
			     "Error copying mtm");
		for (j = 0; j < itv->wzip; j++) {
			((mtm_BX *) mtm->mtm_data)[i].wpat[j] =
			    ((mtm_BX *) __mtm->mtm_data)[i].wpat[j];
		}
	}

	f = __mtm->mtm_freq;
	while (f != NULL) {
		trios_malloc(new_f, sizeof(freq_node), itv_t *,
			     "Error copying mtm");
		new_f->freq = f->freq;
		new_f->label = f->label;
		new_f->next = mtm->mtm_freq;
		mtm->mtm_freq = new_f;
		f = f->next;
	}

	/*mtm = __mtm; */
	nmtm = mtm->nmtm;
	zeros = mtm_count(mtm, 0);
	ones = mtm_count(mtm, 1);

	if (!group_flag) {
		ngrouped = zeros;
		if (!isi_basic(&itv, mtm, multi, isi_type, isi3_step,
			       0, NULL, NULL, NULL)) {
			itv_free(itv);
			mtm_free(mtm);
			return (itv_t *) trios_error(MSG,
						     "lisi: isi_basic() failed.");
		}
		mtm = NULL;
	} else {
		/*if (!isi_gen(&itv, mtm, multi, isi_type, isi3_step, log_step,
		   log_file, tmp_bas_file, win1, &ngrouped, &time_usr,
		   &time_sys)) {
		   win_free(win1);
		   win_free(win2);
		   itv_free(itv);
		   mtm_free(mtm);
		   return trios_error(MSG, "lisi: isi_gen() failed.");
		   } */
		mtm = NULL;
	}

	return itv;
}

