#ifndef _pac_mtm_h_
#define _pac_mtm_h_

#ifdef __cplusplus
extern "C" {
#endif


#include "trios_misc.h"

#define MODA               5 
#define AVERAGE           10 
#define MEDIAN            15
#define MABS              20 
#define MINIMUM           25
#define MAXIMUM           30
#define AMPLITUDE         35
#define MODA_DISCRETE     40
#define AVERAGE_DISCRETE  45

/*  Modification by:  Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)       */
/*  Date: Tue Jan  6 1998                                                   */
/*  Mod: Added two new fields to the structure mtm_bx (Px and px)           */
/*  Mod: Added a new field to the structure mtm_t (comp_prob)               */

/*  Modification by:  Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)       */
/*  Date: Wed Apr 29 1998                                                   */
/*  Mod: Added definitions for MAE and MSE                                  */ 

/*  Modification by:  Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)       */
/*  Date: Tue Jul 27 1999                                                   */
/*  Mod: Changed the definitions for MAE and MSE to MODA_DISCRETE           */
/*  Mod: and AVERAGE_DISCRETE                                               */

/*  Modification by:  Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)       */
/*  Date: Thu Jul 29 1999                                                   */
/*  Mod: The labels of all patterns have been changed to int                */

/*  Modification by:  Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)       */
/*  Date: Thu Aug 19 1999                                                   */
/*  Mod: mtm_BX and mtm_GX structure changed. For mtm_BX, field PX and      */
/*       px were changed to fq and fq1, respectively. For mtm_GX, a new     */
/*       field fq was introduced. fq keeps the number of times a w-pattern  */
/*       has been observed during sampling. fq1 keeps how many times        */
/*       the label 1 was observed assigned to the w-pattern                 */

/*!
    Structure that holds a classified example for operators with gray-level input.
 */
typedef struct mtm_gx {
  int *wpat ;             /*!< pointer to w-pattern             */
  int   label ;             /*!< label attributed to the pattern  */
  unsigned int fq ;         /*!< frequency of the w-pattern       */
} mtm_GX ;


/*!
    Structure that holds a classified example for operators with binary input.
 */
typedef struct mtm_bx {
  unsigned int   *wpat ;    /*!< pointer to w-pattern                       */
  int            label ;    /*!< label attributed to the pattern            */
  unsigned int   fq ;       /*!< frequency of w-pattern                     */
  unsigned int   fq1 ;      /*!< frequency of w-pattern assigned to label 1 */
} mtm_BX ;


/*!
    Structure that holds the classified examples table.
  */
typedef struct {
  int    wsize ;       /*!< size of w-pattern                      */
  char   type ;        /*!< type of examples: BB, BG, GB or GG     */
  unsigned int nmtm ;  /*!< number of miniterms                    */
  unsigned int nsum ;  /*!< total number of examples (sum of fq's) */
  int    comp_prob ;   /*!< if 1, indicates that probabilities were computed */
  freq_node *mtm_freq; /*!< pointer to list of frequencies */
  int   *mtm_data ;    /*!< pointer to the table                   */
} mtm_t ;



/*!
 *Alloc classified example structure.
 *
 *\param wsize W-pattern size.
 *\param type Transformation type. Only BB is supported.
 *\param nmtm Number of classified examples.
 *\return Created mtm_t structure.
*/
mtm_t *mtm_create(int wsize, int type, unsigned int nmtm);


/*!
 * Free memory used by a classified examples structure.
 * \param mtm Pointer to the structure.
 */
void mtm_free(mtm_t *mtm);



/*!
 * Get the size of W-patterns.
 *\param mtm Pointer to the structure.
 *\return The size of the W-patterns.
*/
int mtm_get_wsize(mtm_t *mtm);



/*!
 *Get the type of the labeled examples.
 *\param mtm Pointer to the structure.
 *\return The type of the W-pattern. Only BB is supported.
 */
int mtm_get_type(mtm_t *mtm);


/*!
 * Get the number of classified examples.
 *\param mtm Pointer to the structure.
 *\return The number of classified examples.
 */
unsigned int mtm_get_nmtm(mtm_t *mtm);


/*!
 * Get the number of analysed examples.
 *\param mtm Pointer to the structure.
 *\return The number of analysed examples.
 */
unsigned int mtm_get_nsum(mtm_t *mtm);



/*!
 * Get the comp_prob flag value.
 *\param mtm Pointer to the structure.
 *\return The flag value.
 *\sa mtm_t
 */
int mtm_get_comp_prob(mtm_t *mtm);


/*!
 * Get the list of frequencies.
 *\param mtm Pointer to the structure.
 *\return The list of frequencies.
 */
freq_node *mtm_get_freqlist(mtm_t *mtm_i);


/*!
 * Set the size of W-pattern.
 *\param mtm Pointer to the structure.
 *\param
 */
void mtm_set_wsize(mtm_t *mtm, int wsize);




/*!
 * Set the type of the labeled example.
 *\param mtm Pointer to the structure.
 *\param type Type of the W-patterns.
 */
void mtm_set_type(mtm_t *mtm, int type);



/*!
 * Set the number of classified examples.
 *\param mtm Pointer to the structure.
 *\param nmtm Number of classified examples.
 */
void mtm_set_nmtm(mtm_t *mtm, unsigned int nmtm);



/*!
 * Set the number of analysed examples.
 *\param mtm Pointer to the structure.
 *\param nsum Number of analysed examples.
 */
void mtm_set_nsum(mtm_t *mtm, unsigned int nsum);



/*!
 * Set the comp_prob flag value.
 *\param mtm Pointer to the structure.
 *\param comp_prob New value.
 */
void mtm_set_comp_prob(mtm_t *mtm, int comp_prob);



/*!
 * Inserts a classified example in the structure.
 *\param mtm Pointer to the structure.
 *\param index1 Insert position.
 *\param wzip W-pattern size in bytes.
 *\param wpat W-pattern.
 *\param label Label of the example.
 *\param fq Frequency of the W-pattern.
 *\param fq1 Frequency of the W-pattern with label 1.
 *\return 1 on success. 0 on failure.
 *\sa mtm_t
 */
int mtm_BX_insert(mtm_t *mtm, int index1, int wzip, unsigned int *wpat, int label, unsigned int fq, unsigned int fq1);


/*!
 *Separate examples of a given class k.
 *
 *\param mtm_i Pointer to mtm_t structure.
 *\param k Class to be separated.
 *\param mtm_o1 Examples of class k
 *\param mtm_o2 Examples of class different than k.
*/
 
int mtm_sep(mtm_t *mtm_i, int k, mtm_t **mtm_o1, mtm_t **mtm_o2);


/*!
 * Count the number of examples in the class k.
 *
 *\param mtm_i Pointer to mtm_t structure.
 *\param k Class number.
 *\return The number of examples in class k.
*/ 
unsigned int mtm_count(mtm_t *mtm_i, int k);

/*!
 * Computes the MAE of a mtm_t structure.
 *
 * \param mtm Pointer to mtm_t structure.
 * \param mae_error MAE error.
 * \return 1 on success. 0 on failure.
 */
int mtm_compute_mae(mtm_t *mtm, double *mae_error);


/*!
 *Search examples table for a given W-pattern.
 *
 * \param nmtm Size of the table
 * \param mtm Pointer to mtm_t structure.
 * \param wpat W-pattern.
 * \param wzip Size of the W-pattern.
 * \return Index on success. 0 on failure.
 */
int mtm_search_wpat_BX(unsigned int nmtm, mtm_t *mtm, unsigned int *wpat, int wzip);


/*!
 * Index mtm_t by frequencies.
 *
 * \param mtm_i Pointer to mtm_t structure.
 * \param indexes Vector of indexes.
 * \return 1 on success. 0 on failure.
 */
unsigned int mtm_index_by_freq_count(mtm_t *mtm_i, int *indexes);

#ifdef __cplusplus
}
#endif


#endif


