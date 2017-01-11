#ifndef _trios_learn_h_
#define _trios_learn_h_

#ifdef __cplusplus
extern "C" {
#endif

#include "trios_mtm.h"
#include "trios_itv.h"
#include "trios_win.h"

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




