/*
 * Khoros: $Id: paclearn_local.h,v 1.8 2002/01/11 14:52:48 nina Exp $
 */

/*
 * Copyright (C) 1993 - 1996, Khoral Research, Inc., ("KRI").
 * All rights reserved.  See $BOOTSTRAP/repos/license/License or run klicense.
 */

/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>> <<<<<<<<<<<<<<<<<<<<<<<<<<
   >>>>
   >>>>       Purpose:	Private include file for library `paclearn'.
   >>>>			This is (should be) included by all source
   >>>>			files comprising the library.  This file includes
   >>>>			the public include file for the library:
   >>>>				<paclearn/paclearn.h>
   >>>>
   >>>>       Toolbox: $PACLIB
   >>>>
   >>>>    Written By: Your name goes here!
   >>>>
   >>>>          Date: 
   >>>>
   >>>> Modifications:
   >>>>
   >>>>>>>>>>>>>>>>>>>>>>>>>>>>>> <<<<<<<<<<<<<<<<<<<<<<<<<< */

#ifndef _paclearn_internals_h_
#define _paclearn_internals_h_

	/*-------------------------------------*
	|	#includes
	\--------------------------------------*/

/* #include <paclib.h> */

	/*-------------------------------------*
	|	#defines
	\--------------------------------------*/

	/*-------------------------------------*
	|	typedefs
	\--------------------------------------*/

typedef struct mtx_t {
	itv_BX *I;
	int *M;
	int dim;
	struct mtx_t *next;
} mtx_t;

/* structure to represent a graph node */
typedef struct {
	int flag;		/* flag used during the graph traversing */
	int idx;		/* index of the mtm table */
	int in_edge;		/* number of edges entering into this node */
	int out_edge;		/* number of edges leaving from this node */
	int in_ct;		/* used to keep the actual number of entering
				   edges analysed till a given moment during
				   the switching set computation process */
	idx_ll *child;		/* pointer to the child nodes */
	idx_ll *parent;		/* pointer to the parent nodes */
	int cost;		/* switching cost */
	int aux;		/* auxiliar field */
	int comp;		/* component number */
} gnode_t;

/* The inversion set graph depends on the mtm structure, for     */
/* the field idx of each node of the graph points to an index of */
/* the mtm table                                                 */

typedef struct {
	mtm_t *mtm;		/* pointer to the mtm structure */
	idx_ll *inv;		/* list of indexes in the mtm table, that
				   contains inversion set elements */
	idx_ll *inv_c;		/* list of indexes in the mtm table, for
				   elements in the complement of IS */
	unsigned int nnodes;	/* number of nodes in the graph */
	unsigned int nedges;	/* number of edges in the graph */
	unsigned int final_nodes;	/* number of nodes in the undecidable portion
					   of the graph at the end of the process  */
	double final_w;		/* weight of the undecidable portion */
	int subopt;		/* flag that indicates whether result is
				   suboptimal */
	idx_ll *root;		/* pointer to the root nodes of the graph */
	idx_ll *leaf;		/* pointer to the leaf nodes of the graph */
	gnode_t *gnod;		/* pointer to the graph */
} graph_t;

	/*-------------------------------------*
	|	global variable declarations
	\--------------------------------------*/

	/*-------------------------------------*
	|	macros
	\--------------------------------------*/

/* macro to concatenate two list of intervals */
#define Concatenate_lists(p, p_new)  \
        {                            \
          itv_BX *p_aux ;            \
                                     \
          p_aux = p_new ;            \
          if(p_new) {               \
            while(p_new->next) {     \
              p_new = p_new->next ;  \
            }                        \
            p_new->next = p ;        \
            p = p_aux ;              \
          }                          \
        }

	/*-------------------------------------*
	|	routine definitions
	\--------------------------------------*/

/*===========================================================================\
   ipac_xpl.c
\========================================================================== */

/*
     -------------------------------------------
     FUNCTION: collec_BB is in the global file
     -------------------------------------------
*/

/*===========================================================================\
   ipac_mtm.c 
\========================================================================== */


/*===========================================================================\
   ipac_mtmBG.c 
\========================================================================== */

int /*+ Purpose: Finds the moda of the labels                    + */ freq_moda(
										       freq_node * pqfreq,	/*+ In: pointer to a frequency node               + */
										       unsigned int sum, double freqmin,	/*+ In: Minimal frequency to consider an example  + */
										       int undflabel,	/*+ In: Value assigned to undecidable cases       + */
										       int comptype,	/*+ In: Type of computation to be done for 
													   assigning the label of the undecidable 
													   pattern                                   + */
										       int *wpat_label,	/*+ Out: Moda                                     + */
										       unsigned
										       int
										       *wpat_freq);

int /*+ Purpose: Finds the average of the labels                 + */
freq_average(
			   freq_node * qfreq,	/*+ In: pointer to a frequency node               + */
			   unsigned int sum, double freqmin,	/*+ In: Minimal frequency to consider an example  + */
			   int *wpat_label,	/*+ Out: Average                                 + */
			   unsigned int *wpat_freq);

int /*+ Purpose: Finds the median of the labels                  + */
freq_median(
			  freq_node * qfreq,	/*+ In: pointer to a frequency node               + */
			  unsigned int sum, double freqmin,	/*+ In: Minimal frequency to consider an example  + */
			  int *wpat_label,	/*+ Out: Average                                  + */
			  unsigned int *wpat_freq);

int /*+ Purpose: Finds the non-conflicting labels               + */
freq_absolute(
			    freq_node * pqfreq,	/*+ In: pointer to a frequency node               + */
			    unsigned int sum,	/*+ In: Total number of examples                  + */
			    double freqmin,	/*+ In: Minimal frequency to consider an example  + */
			    int undflabel,	/*+ In: Value assigned to undecidable cases       + */
			    int *wpat_label,	/*+ Out: Average                                  + */
			    unsigned int *wpat_freq);

int				/*+ Purpose: Finds the moda of the labels and decides what to 
				   do based on the estimator                       + */ freq_general(
													    freq_node * pqfreq,	/*+ In: pointer to a frequency node               + */
													    unsigned int sum,	/*+ In: Total number of examples                  + */
													    double freqmin,	/*+ In: Minimal frequency to consider an example  + */
													    double estimator,	/*+ In: Decision Estimator                        + */
													    int undflabel,	/*+ In: Value assigned to undecidable cases       + */
													    int comptype,	/*+ In: Type of computation to be done for 
																   assigning the label of the undecidable 
																   pattern                                   + */
													    int *wpat_label,	/*+ Out: Moda                                    + */
													    unsigned
													    int
													    *wpat_freq);

/*===========================================================================\
   ip_isi.c
\========================================================================== */

int				/*+ Purpose: sort labels of a classified examples set 
				   and return them in a vector                   + */ isi_sort_labels(
													     mtm_t * mtm,	/*+ In: examples to be removed                       + */
													     int *lb	/*+ In/Out: garbage intervals list                   + */
    );

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
									       int *ngrouped,	/*+ Out: number of intrevals after groupment     + */
									       unsigned
									       long
									       *time_usr,
									       unsigned
									       long
									       *time_sys);

int gisi_0(itv_t * itv,		/*+ In/Out: pointer to intervals list                     + */
	   itv_BX * I		/*+ In: the interval that will be removed                 + */
    );

int gisi_1(itv_t * itv,		/*+ In/Out: pointer to intervals list                     + */
	   itv_BX * I,		/*+ In: the interval that will be removed                 + */
	   mtm_t * mtm		/*+ In: examples to be covered                            + */
    );

int gisi_3(itv_t * itv, itv_BX * itv_list, mtm_t * mtm, int isi3_step);

/*
     -------------------------------------------
     FUNCTION: itv_extract
     -------------------------------------------
`*/

itv_BX *			/*+ Purpose: Given an interval I, and a point X, computes
				   the set of intervals equivalent to I-{X}      + */
itv_split_bywpat(itv_BX * I,	/*+ In: interval to be splitted                      + */
		 unsigned int *wpat, int wsize,	/*+ In: w-pattern's size                             + */
		 int wzip	/*+ In: compacted w-pattern's size                   + */
    )
/*+ Return: a pointer to a set of intervals on success, NULL on failure    +*/
    ;

itv_BX *itv_split_byitv(itv_BX * I1,	/*+ In: first interval                                     + */
			itv_BX * I2,	/*+ In: second interval                                    + */
			int wsize,	/*+ In: w-pattern's size                                   + */
			int wzip	/*+ In: compacted w-pattern's size                         + */
    );

/*
     -------------------------------------------
     FUNCTION: isi_basic
     -------------------------------------------
*/

int /*+ Purpose: Perform ISI by points (binary case)           + */ isi_basic(
										     itv_t ** itv_i,	/*+ In/Out: starting intervals                         + */
										     mtm_t * mtm_i,	/*+ In/Out: examples to be extracted                   + */
										     int multi,	/*+ In: if 1, indicates multi-classification           + */
										     int isi_type,	/*+ In: ISI type (ISI-0, IIS-1, ISI-2, ISI-3)          + */
										     int isi3_step,	/*+ In: minimum cover computation step for ISI-3       + */
										     int log_step,	/*+ In: If not ZERO, log file will be written at each
													   time a multiple of log_step examples is processed. + */
										     char *log_file,	/*+ In: log file name                                  + */
										     char *temp_bas_file,	/*+ In: File in which the set of interval at each
														   step will be written                       + */
										     window_t
										     *
										     win);

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
    ;

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
    ;

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
    ;

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
    ;

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
    ;

/*
     -------------------------------------------
     FUNCTION: itv_contain_mtm
     -------------------------------------------
*/

int /*+ Purpose: check wether I contains some example of mtm   + */
itv_contain_mtm(
			      itv_BX * I,	/*+ In: interval                                    + */
			      mtm_t * mtm,	/*+ In: classified examples set                     + */
			      int wzip	/*+ In:                                             + */
    )
/*+ Return: 1 if I contains some example, 0 otherwise                      +*/
    ;

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
    ;

/*===========================================================================\
   ip_mincover.c
\========================================================================== */

/*
     -------------------------------------------
     FUNCTION: pmincover
     -------------------------------------------
*/

int /*+ Purpose: compute the minimun cover of a set of minterms.+ */ pmincover(
										      mtm_t * mtm,	/*+ In: minterms                                    + */
										      itv_t * itv	/*+ In/Out: intervals/minimun cover                 + */
    )
/*+ Return: 1 on success, 0 on failure                                     +*/
    ;

/*
     -------------------------------------------
     FUNCTION: itv_add
     -------------------------------------------
*/

int				/*+ Purpose: add a list of intervals to a set of intervals
				   (do not check anything)                             + */ itv_add(
													   itv_t * itv,	/*+ In/Out: set of intervals                        + */
													   itv_BX ** p_essen	/*+ In/Out: list of intervals to be added           + */
    )
/*+ Return: 1 on success, 0 on failure                                     +*/
    ;

/*
     -------------------------------------------
     FUNCTION: mtm_tbl2ll
     -------------------------------------------
*/

idx_ll *			/*+ Purpose: create a list of indices pointing to the
				   examples of a set of classified examples      + */
mtm_tbl2ll(mtm_t * mtm		/*+ In: classified examples set                     + */
    )
/*+ Return: a pointer to a list of indices. If it is NULL, then either
     the examples set is empty or an error has occurred                    +*/
    ;

/*
     -------------------------------------------
     FUNCTION: itv2mp
     -------------------------------------------
*/

int /*+ Purpose: create auxiliar structure to hold intervals   + */ itv2mp(
										  itv_t * itv,	/*+ In/Out: set of intervals                         + */
										  mtx_t ** mx_head	/*+ In/Out: aux. structure pointing to the intervals + */
    )
/*+ Return: 1 on success, 0 on failure. The interval set will be empty     +*/
    ;

/*
     -------------------------------------------
     FUNCTION: essential_sel
     -------------------------------------------
*/

itv_BX *			/*+ Purpose: select essential intervals                    + */
essential_sel(mtx_t ** mx_head,	/*+ In/Out: list of intervals                       + */
	      mtm_t * mtm,	/*+ In: classified examples set                     + */
	      idx_ll ** ll_head	/*+ In/Out: index for classified examples set       + */
    )
/*+ Return: a pointer to a list with essential intervals                   +*/
    ;

/*
     -------------------------------------------
     FUNCTION: delete_mtm
     -------------------------------------------
*/

void				/*+ Purpose: delete from the list of indices, those that
				   are pointing to examples that are contained in a given
				   interval I                                             + */ delete_mtm(
														 mtm_t * mtm,	/*+ In: set of classified examples                  + */
														 idx_ll * p,	/*+ In: a element in the list of indices            + */
														 idx_ll ** ll_head,	/*+ In/Out: the list of indices                     + */
														 itv_BX * I	/*+ In: an interval                                 + */
    )
/*+ Return: nothing                                                        +*/
    ;

/*
     -------------------------------------------
     FUNCTION: p_cover
     -------------------------------------------
*/

int				/*+ Purpose: compute the minimun cover of a set of minterms.
				   At the end of the process, the non-essential intervals 
				   will be free + */ p_cover(
								    mtm_t * mtm,	/*+ In: minterms                                    + */
								    itv_t * itv	/*+ In/Out: intervals/minimun cover                 + */
    )
/*+ Return: 1 on success, 0 on failure                                     +*/
    ;

/*
     -------------------------------------------
     FUNCTION: set_matrix
     -------------------------------------------
*/

int				/*+ Purpose: given pointers for intervals and examples,
				   create create the prime implicant table that will be
				   used for removing the dominated rows (intervals)       + */ set_matrix(
														 mtx_t * mx_head,	/*+ In/Out: list pointing to some intervals         + */
														 mtm_t * mtm,	/*+ In: set of classified examples                  + */
														 idx_ll * ll_head	/*+ In: list of indices                             + */
    )
/*+ Return: 1 on success, 0 on failure                                     +*/
    ;

/*
     -------------------------------------------
     FUNCTION: del_dominated
     -------------------------------------------
*/

int				/*+ Purpose: delete dominated rows of the prime implicants
				   table                                         + */ del_dominated(
													   mtx_t ** mx_head,	/*+ In/Out: list pointing to the intervals          + */
													   mtm_t * mtm,	/*+ In: set of classified examples                  + */
													   idx_ll * ll_head	/*+ In: list of indices                             + */
    )
/*+ Return: 0 on failure, 2 if there is no modification and 1 if some rows
    were deleted                                                           +*/
    ;

/*
     -------------------------------------------
     FUNCTION: greedy_sel
     -------------------------------------------
*/

itv_BX *			/*+ Purpose: select a cover by the greedy method           + */
greedy_sel(mtx_t ** mx_head,	/*+ In/Out: list pointing to the intervals          + */
	   mtm_t * mtm,		/*+ In: set of classified examples                  + */
	   idx_ll * ll_head	/*+ In: list of indices                             + */
    )
/*+ Return: the selected subset of intervals                               +*/
    ;

/*
     -------------------------------------------
     FUNCTION: del_covered
     -------------------------------------------
*/

int				/*+ Purpose: delete, from a list of indices, those that
				   are pointing to examples that are contained in a given
				   interval I (this routine is similar to del_mtm, the
				   difference is the returning value)                      + */ del_covered(
														   mtm_t * mtm,	/*+ In: set of classified examples                  + */
														   idx_ll ** head,	/*+ In/Out: list of indices                         + */
														   itv_BX * I	/*+ In: interval                                    + */
    )
/*+ Return: 1 on success, 0 on failure                                     +*/
    ;

int				/*+ Purpose: check if some interval of a list contains a
				   w-pattern                                     + */ itv_cover_wpat(
													    itv_BX * p_list,	/*+ In: the pointer to the list of intervals      + */
													    unsigned int *wpat,	/*+ In: the w-pattern                             + */
													    int wzip	/*+ In: compacted w-pattern's size                + */
    );

/*===========================================================================\
   ip_newdec.c
\========================================================================== */


itv_t *				/*+ Purpose: group the examples into intervals          + */
group_x(mtm_t * mtm		/*+ In: set of classified examples                      + */
    );
/*+ Return: ITV structure on success or NULL on failure                   +*/

/*
     -------------------------------------------
     FUNCTION: build_itv_from_mtm
     -------------------------------------------
*/

itv_BX *			/*+ Purpose: create an interval with label ZERO for each
				   minterm with label "label" and put the intervals 
				   into a list of BX intervals + */
build_itv_from_mtm(mtm_t * mtm,	/*+ In: set of minterms               + */
		   int label	/*+ In: target label                  + */
    );
/*+ Return: a list of BX intervals                            +*/

/*
     -------------------------------------------
     FUNCTION: pquine
     -------------------------------------------
*/
itv_BX *			/*+ Purpose: given a list of trivial intervals, groups adjacent
				   ones in a iterative process (create a equivalent set
				   of maximal intervals)                                 + */
pquine(itv_BX * p,		/*+ In: pointer to an intervals list            + */
       int wsize		/*+ In: w-pattern size                          + */
    );
/*+ Return: a list of BX intervals on success or NULL on failure        +*/ ;

/*
     -------------------------------------------
     FUNCTION: grow_cubes
     -------------------------------------------
*/

int /*+ Purpose: group intervals that are adjacent to create new ones + */
grow_cubes(
			 itv_BX ** L,	/*+ In: table containing pointers to lists of intervals + */
			 itv_BX ** L_new,	/*+ In/Out: table containig pointers to lists of newly
						   created intervals                                   + */
			 int wsize,	/*+ In: w-pattern's size                                + */
			 int dim	/*+ In: current step in the iterative grouping process  + */
    );
/*+ Return: an integer                                            +*/

/*===========================================================================\
   ip_switch.c
\========================================================================== */

/* FIRST THREE ROUTINES NOT USED FOR NOW. THEY WILL BE USEFUL TO
   SEPARATE THE GRAPH IN INDEPENDENT COMPONENTS. WHEN ONCE TRIED,
   PRACTICE SHOWED THAT OFTEN ALL NODES ARE IN ONE BIG COMPONENT */

int /*+ Purpose:                                               + */ next_comp(void
    );

int /*+ Purpose:                                               + */ label_comp(
										      gnode_t * gnod, idx_ll * list,	/*+ In:                                             + */
										      unsigned
										      int
										      nnodes);

int /*+ Purpose:                                               + */ components(
										      graph_t * graph	/*+ In:                                             + */
    );
/*+ Return:                                                                +*/

int /*+ Purpose: update T1 (computes T1[i+1] from T1[i])       + */ update_T1(
										     graph_t * graph,	/*+ In/Out: Inversion set represented by a graph    + */
										     int cur,	/*+ In: given index of T1                           + */
										     idx_ll
										     **
										     garbage);

/*
     -------------------------------------------
     FUNCTION: update_T0
     -------------------------------------------
*/
int /*+ Purpose: update T0 (computes T0[1] to T0[cur])         + */ update_T0(
										     graph_t * graph,	/*+ In/Out: Inversion set represented by a graph    + */
										     int
										     cur,
										     idx_ll
										     **
										     garbage);
/*+ Return: 1 on success, 0 on failure                                     +*/

/*
     -------------------------------------------
     FUNCTION: switching_set
     -------------------------------------------
*/

int /*+ Purpose: compute the switching set from a graph        + */
switching_set(
			    graph_t * graph,	/*+ In/Out: Inversion set represented by a graph    + */
			    int k, idx_ll ** garbage);
/*+ Return: 1 on success, 0 on failure                                     +*/

/*
     -------------------------------------------
     FUNCTION: remove_x
     -------------------------------------------
*/

int /*+ Purpose:                                               + */ remove_x(
										    gnode_t * gnod,	/*+ In/Out: pointer to a graph structure            + */
										    idx_ll ** parents,	/*+ In/Out: current level at T1                     + */
										    int
										    x,
										    idx_ll
										    **
										    garbage);
/*+ Return:                                                                +*/

/*
     -------------------------------------------
     FUNCTION: remove_y
     -------------------------------------------
*/

int /*+ Purpose:                                               + */ remove_y(
										    gnode_t * gnod,	/*+ In/Out: pointer to a graph structure            + */
										    idx_ll ** child,	/*+ In/Out:                                         + */
										    int
										    y,
										    idx_ll
										    **
										    garbage);
/*+ Return:                                                                +*/

/*
     -------------------------------------------
     FUNCTION: up_weight
     -------------------------------------------
*/

int /*+ Purpose:                                               + */ up_weight(
										     gnode_t * gnod,	/*+ In/Out: pointer to a graph structure            + */
										     int
										     *combined,
										     idx_ll
										     **
										     garbage);
/*+ Return:                                                                +*/

/*
     -------------------------------------------
     FUNCTION: dw_weight
     -------------------------------------------
*/

int /*+ Purpose:                                               + */ dw_weight(
										     gnode_t * gnod,	/*+ In/Out: pointer to a graph structure            + */
										     int
										     *combined,
										     idx_ll
										     **
										     garbage);
/*+ Return:                                                                +*/

/*
     -------------------------------------------
     FUNCTION: clea_root_list
     -------------------------------------------
*/

int				/*+ Purpose: given a list of root nodes, removes from it
				   all nodes that are feasible (i.e., has negative cost)  + */ clea_root_list(
														     gnode_t * gnod,	/*+ In: graph structure                             + */
														     idx_ll
														     **
														     garbage);
/*+ Return: the new list                                                   +*/

/*
     -------------------------------------------
     FUNCTION: clea_leaf_list
     -------------------------------------------
*/

int				/*+ Purpose: given a list of root nodes, removes from it
				   all nodes that are feasible (i.e., has negative cost)  + */ clea_leaf_list(
														     gnode_t * gnod,	/*+ In: graph structure                             + */
														     idx_ll
														     **
														     garbage);
/*+ Return: the new list                                                   +*/

/*
     -------------------------------------------
     FUNCTION: upset_remove
     -------------------------------------------
*/

int /*+ Purpose:                                               + */ upset_remove(
											graph_t * graph,	/*+ In/Out: pointer to a graph structure            + */
											idx_ll
											**
											garbage);
/*+ Return:                                                                +*/

/*
     -------------------------------------------
     FUNCTION: lowset_remove
     -------------------------------------------
*/

int /*+ Purpose:                                               + */
lowset_remove(
			    graph_t * graph,	/*+ In/Out: pointer to a graph structure            + */
			    idx_ll ** garbage);
/*+ Return:                                                                +*/

/*
     -------------------------------------------
     FUNCTION: graph_free
     -------------------------------------------
*/

void /*+ Purpose: to free memory used by a graph structure      + */ graph_free(
										       graph_t * graph	/*+ In/Out: pointer to a graph structure            + */
    );
/*+ Return: nothing                                                        +*/

/*
     -------------------------------------------
     FUNCTION: pinvset_new
     -------------------------------------------
*/

int /*+ Purpose: find the inversion set (and its complement)   + */ pinvset(
										   mtm_t * mtm,	/*+ In: classified examples set                     + */
										   idx_ll ** inv,	/*+ Out: the inversion set                          + */
										   int *ninv,	/*+ Out: number of elements in the inversion set    + */
										   idx_ll ** inv_c	/*+ Out: complement of the inversion set            + */
    );
/*+ Return: 1 on success, 0 on failure                                     +*/

/*
     -------------------------------------------
     FUNCTION: pinvset
     -------------------------------------------
*/

int /*+ Purpose: find the inversion set (and its complement)   + */ pinvset_max(
										       mtm_t * mtm,	/*+ In: classified examples set                     + */
										       idx_ll ** inv,	/*+ Out: the inversion set                          + */
										       int *ninv,	/*+ Out: number of elements in the inversion set    + */
										       idx_ll ** inv_c	/*+ Out: complement of the inversion set            + */
    );
/*+ Return: 1 on success, 0 on failure                                     +*/

/*
     -------------------------------------------
     FUNCTION: set_parent
     -------------------------------------------
*/

int				/*+ Purpose: during the graph building process, given an
				   example (idx) that is being inserted as a new node,
				   verifies which already exitent nodes (among the nodes
				   between r and itself) are his parent nodes, and insert
				   edges linking it to the parent nodes found             + */ set_parent(
														 gnode_t * gnod,	/*+ In/Out: partially build graph                   + */
														 idx_ll * r,	/*+ In: current node to be examined                 + */
														 int idx,	/*+ In: index occupied by the current example       + */
														 mtm_BX * q,	/*+ In: classified examples (data part)             + */
														 int wzip	/*+ In: compressed examples size                    + */
    );
/*+ Return: -1 on failure; 0 if q is not descendent of r, >=1 otherwise    +*/

/*
     -------------------------------------------
     FUNCTION: build_graph
     -------------------------------------------
*/

graph_t *			/*+ Purpose: build the traversing graph of an inversion set + */
build_graph(mtm_t ** mtm,	/*+ In/Out: set of classified examples                 + */
	    idx_ll * inv,	/*+ In: list of ind. for elements of the inversion set + */
	    int ninv,		/*+ In: size of the inversion set                      + */
	    idx_ll * inv_c,	/*+ In: list of ind. for elem. of the compl. inv. set  + */
	    idx_ll ** garbage);
/*+ Return: the traversing graph on success, or NULL on failure            +*/

/*
     -------------------------------------------
     FUNCTION: print_graph
     -------------------------------------------
*/

void /*+ Purpose: print graph (for debugging)                   + */ print_graph(
											graph_t * graph,	/*+ In: graph                                       + */
											char *fname	/*+ In: name of the output file                     + */
    );
/*+ Return:                                                                +*/

/*
     -------------------------------------------
     FUNCTION: take_partition
     -------------------------------------------
*/

int				/*+ Purpose: given a graph for which the switching set has
				   already been computed, creates two list of indices: one
				   for the upper set nodes and the other for the lower set
				   nodes                                                  + */ take_partition(
														     graph_t * graph,	/*+ In: graph (after the swthing set computation)   + */
														     idx_ll ** upper,	/*+ Out: pointer to the upper set nodes             + */
														     idx_ll ** lower,	/*+ Out: pointer to the lower set nodes             + */
														     idx_ll
														     **
														     garbage);

/*
     -------------------------------------------
     FUNCTION: take_base
     -------------------------------------------
*/

int				/*+ Purpose: Given a graph which has the switching set
				   computed, take the minimal elements of the
				   upper set (base)                              + */ take_base(
												       graph_t * graph,	/*+ In: graph                                       + */
												       idx_ll ** base,	/*+ Out: list of indices pointing to the base elem. + */
												       int *base_size,	/*+ Out: the size of the base                       + */
												       double *swt_error,	/*+ Out: the error increase due to the switches     + */
												       int *swt_size,	/*+ Out: the size of the switching set              + */
												       idx_ll
												       **
												       garbage);
/*+ Return: 1 on success, 0 on failure                                     +*/

/*
     -------------------------------------------
     FUNCTION: convert_mtm2itv
     -------------------------------------------
*/

itv_t *				/*+ Purpose: given a subset of a classified examples set,
				   convert each element x of this subset in an
				   interval [x,I] (I=greatest element)           + */
convert_mtm2itv(mtm_t * mtm,	/*+ In: classified examples                         + */
		idx_ll * base,	/*+ In: subset of the classified examples           + */
		int base_size	/*+ In: the size of the subset                      + */
    );
/*+ Return: a pointer to an interval set, or NULL (failure)                +*/

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
/* Next routine is a supposedly improved implementation of the above
   equivalent one. However, in practice, I don't know why, it is not better.
   The basic changes made are : instead verifying whether an element
   pertains to an interval, here I check if it is greater (or smaller)
   relative to a given extremity of the interval (within the loop that
   compute the minimal elements of the 1-set).                             */

/*
     -------------------------------------------
     FUNCTION: take_base_new
     -------------------------------------------
*/

int				/*+ Purpose: Given a graph which has the switching set
				   computed, take the minimal elements of the
				   upper set (base)                              + */ take_base_new(
													   graph_t * graph,	/*+ In: graph                                       + */
													   idx_ll ** base,	/*+ Out: list of indices pointing to the base elem. + */
													   int *base_size,	/*+ Out: the size of the base                       + */
													   double *swt_error,	/*+ Out: the error increase due to the switches     + */
													   int *swt_size,	/*+ Out: the size of the switching set              + */
													   idx_ll
													   **
													   garbage);
/*+ Return: 1 on success, 0 on failure                                     +*/

/*===========================================================================\
   dp_switch.c
\========================================================================== */

/*
     -------------------------------------------
     FUNCTION: gcomp_cost
     -------------------------------------------
*/

int				/*+ Purpose: compute the switching costs for the nodes of
				   a graph, based on a beta probability density           + */ gcomp_cost(
														 graph_t * graph,	/*+ In: graph for which costs will be computed      + */
														 char *fname	/*+ In: name of a beta density table                + */
    );
/*+ Return: 1 on success, 0 on failure                                     +*/

/*
     -------------------------------------------
     FUNCTION: swt_output_log
     -------------------------------------------
*/
int				/*+ Purpose: write some info about the switching set
				   computation to the log file                   + */ swt_output_log(
													    char *fname,	/*+ In/Out: log file name                           + */
													    graph_t * graph,	/*+ In/Out: log file name                           + */
													    double mae_error,	/*+ In: expected mae error                          + */
													    double swt_error,	/*+ In: error increase due to the switchings        + */
													    int nswitches,	/*+ In: number of switches (switching set size)     + */
													    unsigned
													    long
													    int
													    time_usr1,
													    unsigned
													    long
													    int
													    time_sys1,
													    unsigned
													    long
													    int
													    time_usr2,
													    unsigned
													    long
													    int
													    time_sys2,
													    unsigned
													    long
													    int
													    time_usr3,
													    unsigned
													    long
													    int
													    time_sys3,
													    unsigned
													    long
													    int
													    time_usr4,
													    unsigned
													    long
													    int
													    time_sys4);
/*+ Return: 1 on success, 0 on failure                                     +*/

/*
     -------------------------------------------
     FUNCTION: compute_mae
     -------------------------------------------
*/
int /*+ Purpose: given a mtm table, computes the mae           + */ compute_mae(
										       mtm_t * mtm,	/*+ In: mtm table                                   + */
										       double *mae_error	/*+ Out: mae                                        + */
    );
/*+ Return: 1 on success, 0 on failure                                     +*/

/*===========================================================================\
   ip_shannon.c
\========================================================================== */

/*
     -------------------------------------------
     FUNCTION: itv_subitv
     -------------------------------------------
*/

itv_t *				/*+ Purpose: create one interval (1: [empty,W], 2:[o,W],
				   3:[empty,W-o])                                + */
itv_subitv(window_t * win,	/*+ In: window                                      + */
	   int itv_type		/*+ In: interval type (1: [empty,W], 2:[o,W],
				   3:[empty,W-o])                              + */
    );
/*+ Return: Pointer to ITV structure on success, NULL on failure           +*/

/*
     -------------------------------------------
     FUNCTION: which_var
     -------------------------------------------
*/

int				/*+ Purpose: given a set of examples (mtm) determines which
				   is the variable (direction) that most equatively
				   partitions the examples.                      + */ which_var(
												       mtm_t * mtm,	/*+ In: set of classified examples                  + */
												       int LIMIT	/*+ In: limit                                       + */
    );
/*
     -------------------------------------------
     FUNCTION: mtm_part
     -------------------------------------------
*/

int				/*+ Purpose: to partition the set of examples recursively
				   until each of the partitions has no more
				   then "threshold" examples                    + */ mtm_part(
												     mtm_t * mtm,	/*+ In: classified examples                         + */
												     itv_t * itv,	/*+ In: interval                                    + */
												     int threshold,	/*+ In: Maximun number of examples in each partition+ */
												     mtm_t ** MTM,	/*+ In/Out: vector of examples subsets              + */
												     itv_t ** ITV,	/*+ In/Out: vector of intervals                     + */
												     int *noper	/*+ In/Out: next partition to be built              + */
    );
/*+ Return: 1 on success, 0 on failure                                     +*/

int				/*+ Purpose: to partition the set of examples recursively
				   until each of the partitions has no more
				   than "threshold" examples CASE1              + */ mtm_part_1(
												       int NP,	/* In : processors number */
												       int LIMIT,	/* In: limit */
												       mtm_t * mtm,	/*+ In: classified examples                         + */
												       itv_t * itv,	/*+ In: interval                                    + */
												       mtm_t ** MTM,	/*+ In/Out: vector of examples subsets              + */
												       itv_t ** ITV,	/*+ In/Out: vector of intervals                     + */
												       int *noper	/*+ In/Out: next partition to be built              + */
    );

/*
     -------------------------------------------
     FUNCTION: sep_mtm
     -------------------------------------------
*/

int				/*+ Purpose: separate the set of classified examples in
				   disjoint subsets, according to the decomposition vaiables  + */ sep_mtm(
														  mtm_t * mtm,	/*+ In: classified examples                         + */
														  itv_t * itv,	/*+ In: interval                                    + */
														  int var,	/*+ In: Which direction the partition will be made ?+ */
														  mtm_t ** mtm0,	/*+ Out: examples with value(var)=0                 + */
														  mtm_t ** mtm1	/*+ Out: examples with value(var)=1                 + */
    );
/*+ Return: 1 on succes, 0 on failure                                      +*/

/*
     -------------------------------------------
     FUNCTION: itv_setvar
     -------------------------------------------
*/

int				/*+ Purpose: Given an interval, set the indicated variable
				   as a constant (thus creating two subintervals)         + */ itv_setvar(
														 itv_t * itv,	/*+ In: input interval                              + */
														 int var,	/*+ In: variable index                              + */
														 itv_t ** itv0,	/*+ Out: first output interval                      + */
														 itv_t ** itv1	/*+ Out: second output interval                     + */
    );
/*+ Return: 1 on success, 0 on failure                                     +*/
#endif				/* _paclearn_internals_h_ */
/* Don't add after this point */
