#ifndef _trios_learn_h_
#define _trios_learn_h_

#ifdef __cplusplus
extern "C" {
#endif

#include "trios_multi.h"
#include "trios_mtm.h"
#include "trios_operator.h"



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

int computeMSEGG(dTree *gg_operator, window_t *win, imgset_t *test, double *acc);

/*!
 * \brief Partition an interval into disjoint intervals containing no more than "threshold" examples.
 * \param fname_i Classified examples file.
 * \param itv_type Interval type.
 * \param threshold Maximum number of examples in each partition.
 * \param mtm_pref Prefix of the output mtm_t files.
 * \param itv_pref Prefix of the output itv_t files.
 * \param n_itv Pointer to store the number of generated intervals.
 * \return 1 on success. 0 on failure.
 */
int lpartition_disk(char *fname_i,int itv_type,int threshold, char *mtm_pref, char *itv_pref, int *n_itv);

/*!
 * \brief Partition an interval into disjoint intervals containing no more than "threshold" examples.
 * \param win Window used.
 * \param mtm Classified examples.
 * \param itv_type Interval type.
 * \param threshold Maximum number of examples in each partition.
 * \param mtm_out Pointer to store the list of mtm_t generated.
 * \param itv_out Pointer to store the list of itv_t generated.
 * \param n_itv Pointer to the number of generated intervals.
 * \return
 */
int lpartition_memory(window_t *win, mtm_t *mtm, int itv_type, int threshold, mtm_t ***mtm_out, itv_t ***itv_out, int *n_itv);

/*!
 * Partitions the complete interval into several partitions of size threshold, executes ISI on each partition and concatenate the results. A lot faster than ISI on the complete interval.
 * \param win Operator's window.
 * \param mtm Classified examples set.
 * \param threshold Number of elements per interval.
 * \return The trained operator.
 */
itv_t *lisi_partitioned(window_t *win, mtm_t *mtm, int threshold);

#ifdef __cplusplus
}
#endif

#endif /* _trios_learn_h_ */




