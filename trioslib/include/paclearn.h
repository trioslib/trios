/*
 * Khoros: $Id: paclearn.h,v 1.8 2002/01/11 14:53:59 nina Exp $
 */

/*
 * Copyright (C) 1993 - 1996, Khoral Research, Inc., ("KRI").
 * All rights reserved.  See $BOOTSTRAP/repos/license/License or run klicense.
 */


/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>> <<<<<<<<<<<<<<<<<<<<<<<<<<
   >>>>
   >>>>       Purpose: Definitions and prototypes for library pac_learn
   >>>>
   >>>>       Toolbox: $PACLIB
   >>>>
   >>>>    Written By: Nina S. Tomita and Roberto Hirata Jr.
   >>>>
   >>>>          Date:
   >>>>
   >>>> Modifications:
   >>>>
   >>>>>>>>>>>>>>>>>>>>>>>>>>>>>> <<<<<<<<<<<<<<<<<<<<<<<<<< */

#ifndef _paclearn_h_
#define _paclearn_h_


	/*-------------------------------------*
	|       #include
	---------------------------------------*/


	/*-------------------------------------*
	|       #defines
	---------------------------------------*/


	/*-------------------------------------*
	|       typedefs
	---------------------------------------*/


	/*-------------------------------------*
	|       global variable declarations
	---------------------------------------*/


	/*-------------------------------------*
	|       macros
	---------------------------------------*/


	/*-------------------------------------*
	|       routine definitions
	---------------------------------------*/



/*===========================================================================\
   dpac_mtm.c
\========================================================================== */

/*
     -------------------------------------------
     FUNCTION: LDECISION
     -------------------------------------------
*/

int        /*+ Purpose: Builds a classified examples table from a set of
	                observed examples                                  +*/
  ldecision(
    char *fname_i,   /*+ In: observed set of examples file name            +*/
    int   comp_prob, /*+ In: flag indicating if the probabilities should be
                            computed. (0=NO, 1=YES)                        +*/
    double freqmin,   /*+ In: Minimum frequency of an example (in %)
                            (if this percentage is not satisfied, then
                            example will be eliminated)                    +*/
    double estim,     /*+ In: Decision Estimator                            +*/
    int   comptype,  /*+ In: type of computation for the undecidable cases +*/
    int   undflabel, /*+ In: Value to be assigned to the undecidable
                             examples, if comptype==1                      +*/
    char *fname_o    /*+ Out: Output file name                             +*/
) ;




/*===========================================================================\
   dp_isigray.c
\========================================================================== */


int                   /*+ Purpose: ISI-GRAY algorithm                      +*/
  lisigray(
    char *fname_i1,   /*+ In: Classified examples file                     +*/
    char *fname_i2,   /*+ In: Starting intervals file                      +*/
    int  isi_step,    /*+ In: minimum cover computation step for ISI-GRAY  +*/
    char *fname_o,    /*+ Out: resulting intervals                         +*/
    int  label,       /*+ In: default label to assign to intervals         +*/
    int  def_label,   /*+ In: default label to assign to intervals         +*/
    int  sort_itv,    /*+ In: Sort the intervals if 1 or not if 0          +*/
    int  log_step,    /*+ In: If not ZERO, log file will be written at
                              each time a multiple of log_step examples is
                              processed. If log_file name is not given, log
                              will be written to stdout                    +*/
    char *log_file,   /*+ In: log file name. If not NULL, then (1) if log_step
                              is not ZERO, intermediary status will be written
                              in this file, (2) final summary report will
                              be written in this file.                     +*/
    char *tmp_bas_file /*+ In: if not NULL, the set of intervals at each
			       step will be written is this file           +*/
    ) ;


/*===========================================================================\
   dp_isi.c
\========================================================================== */

int
  lisi(
    char *fname_i1,   /*+ In: Classified examples file                     +*/
    char *fname_i2,   /*+ In: Starting intervals file                      +*/
    int  isi_type,    /*+ In: 0 (ISI-0), 1 (ISI-1), 2 (ISI-2), 3 (ISI-3)   +*/
    int  isi3_step,   /*+ In: minimum cover computation step for ISI-3     +*/
    int  multi,       /*+ In: if 1, indicates multi-classification         +*/
    int  group_flag,  /*+ In: if 1, examples will be grouped before extraction +*/
    char *fname_o,    /*+ Out: resulting intervals                         +*/
    int  log_step,
    char *log_file,
    char *tmp_bas_file
    ) ;


/*===========================================================================\
   dp_mincover.c
\========================================================================== */

/*
     -------------------------------------------
     FUNCTION: lmincover
     -------------------------------------------
*/

int             /*+ Purpose: computes an approximate minimun cover         +*/
  lmincover(
    char *fname1,      /*+ In: minterms file name                          +*/
    char *fname2,      /*+ In: intervals file name                         +*/
    char *fname3       /*+ Out: minimun cover                              +*/
)
/*+ Return: 1 on success, 0 on failure                                     +*/ ;


/*===========================================================================\
   dp_newdec.c
\========================================================================== */

/*
     -------------------------------------------
     FUNCTION: Ldecision_other
     -------------------------------------------
*/

int         /*+ Purpose: given a set of examples, make decisions to assign
                a label to each example, and create a set of classified
                examples (this is different of the usual decision, because
                it considers a factor of inertia)                          +*/
  ldecision_other(
    char *fname_i,     /*+ In: set of examples                              +*/
    int  lambda,       /*+ In: factor of inertia                            +*/
    char *fname_o      /*+ Out: set of classified examples                  +*/
) ;
/*+ Return: 1 on success 0 on failure                                       +*/



/*===========================================================================\
   dp_group.c
\========================================================================== */

/*
     -------------------------------------------
     FUNCTION: lgroup
     -------------------------------------------
*/

int                /*+ Purpose: group the minterms (w-patterns) with same
                       labels in maximal intervals                      +*/
  lgroup(
    char *fname_i, /*+ In: set of classified examples file              +*/
    char *fname_o  /*+ In: set of intervals                             +*/
)
                   /*+ Return: 1 on success, 0 on failure               +*/ ;




/*===========================================================================\
   dp_switch.c
\========================================================================== */

/*
     -------------------------------------------
     FUNCTION: lswitch
     -------------------------------------------
*/
int             /*+ Purpose: given a set of classified examples computes
                             the optimal switching set +*/
  lswitch(
    char *fname_i1,    /*+ In: set of classified examples                  +*/
    int  k,
    char *fname_o1,    /*+ In: the base of the resulting increasing oper.  +*/
    char *fname_o2     /*+ In: the log file name                           +*/
) ;



/*===========================================================================\
   dp_shannon.c
\========================================================================== */

int                   /*+ Purpose: Learning based on Shannon Decomposition +*/
  lshannon(
    char *fname_i,    /*+ In: Classified examples file                     +*/
    int  threshold,   /*+ In: Maximun number of examples allowed in each
                              partition.                                   +*/
    int  itv_type,    /*+ In: Starting interval type                       +*/
    int  isi_type,    /*+ In: 0 (ISI-0), 1 (ISI-1), 2 (ISI-2), 3 (ISI-3)   +*/
    int  isi3_step,   /*+ In: minimum cover computation step for ISI-3     +*/
    char *fname_o,    /*+ Out: resulting intervals                         +*/
    char *log_file    /*+ Out: Log file                                    +*/
    ) ;


/*===========================================================================\
   dp_itvconcat.c
\========================================================================== */

int litvconcat( char *fname_i, int nfiles, char *fname_o);


/*===========================================================================\
   dp_partition.c
\========================================================================== */

int  lpartition( char *fname_i, int  itv_type, int  threshold, char *mtm_pref,
                  char *itv_pref, char *fname_o);


int          /*+ Purpose: Partition an interval into disjoint intervals
                 containing no more than a specified number of examples
Modificatio of lpartition : it divides intervals based on processors number
+*/
  lpartition_1(
    char *fname_i,    /*+ In: Classified examples file                     +*/
    int  itv_type,    /*+ In: Starting interval type                       +*/
    int  NP,          /* In: processors number */
    int  LIMIT,       /*+ In: limit                                       +*/
    char *mtm_pref,   /*+ In: Path/Prefix of output MTM files              +*/
    char *itv_pref,   /*+ In: Path/Prefix of output ITV files              +*/
    char *fname_o     /*+ Out: output log file (contains number of intervals
                               generated                                   +*/
  ) ;


/*===========================================================================\
   ip_train2.c
\========================================================================== */
int ltrain2_isi(
    xpl_t *xpl,    /*+ In: Examples collected from previous phase images +*/
    int wsize,     /*+ In: Window size +*/
    char *fname_o /*+ Out: Resulting operator                        + */
);




	/* Ghostwriter Prototypes:  Do Not Erase This Comment */

#endif /* _paclearn_h_ */
/* Don't add after this point */




