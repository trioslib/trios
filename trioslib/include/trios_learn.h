#ifndef _trios_learn_h_
#define _trios_learn_h_

#ifdef __cplusplus
extern "C" {
#endif

#include "trios_multi.h"

/*!
 * Builds a classified examples table from a set of observed examples.
 *
 * \param xpl Observed examples.
    \param comp_prob A flag indicating if the probabiities should be computed (0=NO, 1=YES).
    \param freqmin Minimum frequency. If the example has frequency below this value it is ignored.
    \param estim Decision estimator.
    \param comptype Type of computation for undecidable cases.
    \param undflabel Label to use in the undecidable cases, if comptype == 1.
 * \return The classified example set.
 */
mtm_t *ldecision_memory(xpl_t *xpl, int comp_prob, double freqmin, double estim, int comptype, int undflabel);

/*!
    Builds a classified examples table from a set of observed examples.

    \param fname_i File name of the observed examples.
    \param comp_prob A flag indicating if the probabiities should be computed (0=NO, 1=YES).
    \param freqmin Minimum frequency. If the example has frequency below this value it is ignored.
    \param estim Decision estimator.
    \param comptype Type of computation for undecidable cases.
    \param undflabel Label to use in the undecidable cases, if comptype == 1.
    \return 1 on success. 0 on failure.
*/
int ldecision_disk(char *fname_i, int comp_prob, double freqmin, double estim, int comptype, int undflabel, char *fname_o);

/*!
 * ISI algorithm for boolean function minimization.
 * \param mtm Classified examples.
 * \param itv Starting intervals.
    \param isi_type    0 (ISI-0), 1 (ISI-1), 2 (ISI-2), 3 (ISI-3)
    \param isi3_step   Minimum cover computation step for ISI-3
    \param multi    If 1, indicates multi-classification
    \param group_flag  If 1, examples will be grouped before extraction
 * \return
 */
itv_t *lisi_memory(mtm_t *mtm, itv_t *itv, int isi_type, int isi3_step, int multi, int group_flag);

/*!
    ISI algorithm for boolean function minimization.

    \param fname_i1   Classified examples file
    \param fname_i2   Starting intervals file
    \param isi_type    0 (ISI-0), 1 (ISI-1), 2 (ISI-2), 3 (ISI-3)
    \param isi3_step   Minimum cover computation step for ISI-3
    \param multi    If 1, indicates multi-classification
    \param group_flag  If 1, examples will be grouped before extraction
    \param fname_o    Resulting intervals
    \param log_step
    \param log_file
    \param tmp_bas_file

    \return 1 on success. 0 on failure. The resulting operator is written to fname_o.
*/
int lisi_disk(char *fname_i1, char *fname_i2, int isi_type, int isi3_step, int multi, int group_flag, char *fname_o, int log_step, char *log_file,char *tmp_bas_file);

/*!
    Computes an approximate minimum cover.
    \param fname1 Classified example input file.
    \param fname2 Intervals input file.
    \param fname3 Minimum cover output file.
    \return 1 on success. 0 on failure. The resulting cover is written to fname3.
*/

int lmincover(char *fname1, char *fname2, char *fname3);

dTree *ltrainGG_memory(mtm_t *mtm);


int computeMAEBB(itv_t *bb_operator, window_t *win, imgset_t *test, double *acc);

int computeMAEBBmulti(multi_level_operator_t *bb_operator, imgset_t *test, double *acc);


#ifdef __cplusplus
}
#endif

#endif /* _trios_learn_h_ */




