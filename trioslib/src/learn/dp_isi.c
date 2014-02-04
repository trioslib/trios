#include "trios_win.h"
#include "trios_itv.h"
#include "trios_mtm.h"
#include "trios_io.h"
#include <trios_xpl.h>
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

int lisi_disk(char *fname_i1, char *fname_i2, int isi_type, int isi3_step,
	      int multi, int group_flag, char *fname_o, int log_step,
	      char *log_file, char *tmp_bas_file)
{
	window_t *win1, *win2;
	apert_t *apt, *apt2;
	mtm_t *mtm;
	itv_t *itv;
	int nmtm, zeros = 0, ones = 0, ngrouped;
	FILE *fd;
	unsigned long time_usr, time_sys;

	if (log_step != 0) {
		if (!log_file) {
			trios_warning("Log will be written to stdout.");
		}
		if (!tmp_bas_file) {
			trios_warning
			    ("To save temporary basis, specify a file name.");
		}
	} else {
		if (tmp_bas_file) {
			trios_warning
			    ("To save temporary basis, specify a log step.");
		}
	}

	/* read classified examples */

	if (!(mtm = mtm_read(fname_i1, &win1, &apt))) {
		return trios_error(MSG, "lisi: mtm_read() failed to read %s",
				   fname_i1);
	}
#ifdef _DEBUG_
	trios_debug("Reading classified examples file: done.");
#endif

	/* get some info for the summary file */
	nmtm = mtm->nmtm;
	zeros = mtm_count(mtm, 0);
	ones = mtm_count(mtm, 1);

#ifdef _DEBUG_
	trios_debug("Examples with label 0 = %d", zeros);
	trios_debug("Examples with label 1 = %d", ones);
#endif

	/* read starting intervals */
	if (!(itv = itv_read(fname_i2, &win2 /*, &apt2 */ ))) {
		win_free(win1);
		mtm_free(mtm);
		return trios_error(MSG, "lisi: itv_read() failed to read %s",
				   fname_i2);
	}
#ifdef _DEBUG_
	trios_debug("Reading starting intervals: done.");
#endif

	/* IF win1 != win2 ----> ERROR !! */

	if (!group_flag) {
		ngrouped = zeros;
		if (!isi_basic(&itv, mtm, multi, isi_type, isi3_step,
			       log_step, log_file, tmp_bas_file, win1)) {
			win_free(win1);
			win_free(win2);
			itv_free(itv);
			mtm_free(mtm);
			return trios_error(MSG, "lisi: isi_basic() failed.");
		}
		mtm = NULL;
	} else {
		if (!isi_gen(&itv, mtm, multi, isi_type, isi3_step, log_step,
			     log_file, tmp_bas_file, win1, &ngrouped, &time_usr,
			     &time_sys)) {
			win_free(win1);
			win_free(win2);
			itv_free(itv);
			mtm_free(mtm);
			return trios_error(MSG, "lisi: isi_gen() failed.");
		}
		mtm = NULL;
	}

	/* write resulting intervals to the output file */
	if (!itv_write(fname_o, itv, win1 /*, apt */ )) {
		win_free(win1);
		win_free(win2);
		itv_free(itv);
		mtm_free(mtm);
		return trios_error(MSG, "lisi: itv_write() failed to write %s",
				   fname_o);
	}
#ifdef _DEBUG_
	trios_debug("Writing intervals: done.");
#endif

	/* write summary if needed */
	if (log_file) {
		fd = fopen(log_file, "w");
		fprintf(fd, "mtm_file=%s\n", fname_i1);
		fprintf(fd, "ISI type=%d\n", isi_type);
		fprintf(fd, "ISI3 step=%d\n", isi3_step);
		fprintf(fd, "Examples=%d\n", nmtm);
		fprintf(fd, "Ones=%d\n", ones);
		fprintf(fd, "Zeros=%d\n", zeros);
		if (group_flag) {
			fprintf(fd, "Grouped Zeros=%d\n", ngrouped);
			fprintf(fd, "gtime=%lu+%lu\n", time_usr, time_sys);
		}
		fprintf(fd, "basesize=%d\n", itv->nitv);
	}

	/* read classified examples */
	/*
	   if(!(mtm=mtm_read(fname_i1, &win1, &apt))) {
	   return trios_error(MSG, "lisi: mtm_read() failed to read %s", fname_i1) ;
	   }
	   if(itv_consistency(itv, mtm)) {
	   printf("Reultado consistente\n") ;
	   }
	   else {
	   printf("Reultado inconsitente ou dados invalidos\n") ;
	   }
	   mtm_free(mtm) ;
	 */

	/* free memory */
	win_free(win1);
	win_free(win2);
	itv_free(itv);

	/*if(apt) apert_free(apt) ; */

	return (1);
}
