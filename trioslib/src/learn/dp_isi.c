#include <paclib_kernel.h>
#include "paclearn_local.h"

#define _DEBUG_ 


int             /*+ Given a set of intervals and a set of classified
                    examples, check whether the set of intervals is
                    consistent with the examples                           +*/
  itv_consistency(                                                                  
    itv_t    *itv,               /*+ In: intervals set                     +*/
    mtm_t    *mtm                /*+ set of examples                       +*/
    ) ;


/*
     -------------------------------------------
     FUNCTION: lisi
     -------------------------------------------
*/

int                   /*+ Purpose: ISI algorithm                           +*/
  lisi(
    char *fname_i1,   /*+ In: Classified examples file                     +*/
    char *fname_i2,   /*+ In: Starting intervals file                      +*/
    int  isi_type,    /*+ In: 0 (ISI-0), 1 (ISI-1), 2 (ISI-2), 3 (ISI-3)   +*/
    int  isi3_step,   /*+ In: minimum cover computation step for ISI-3     +*/
    int  multi,       /*+ In: if 1, indicates multi-classification         +*/
    int  group_flag,  /*+ In: if 1, examples will be grouped before extraction +*/
    char *fname_o,    /*+ Out: resulting intervals                         +*/
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
  )
/*+ Return: 1 on success, 0 on failure                                     +*/
{
/*  author: Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)                 */
/*  date: Tue Dec  9 1997                                                   */

/*  Date: Mon Feb 14 2000                                                   */
/*  Mod: Added parameters and code needed for the generation of intermediary*/
/*    and final log files                                                   */

/* Date: Wed Feb 17 2000                                                   */
/* Mod: Window structure (1) changed to support multiple bands, (2) no     */
/*      longer holds information about aperture.                           */
/*      New structure "apert_t" was created.                               */
/*      Modifications were made to adequate to the structure changing.     */

/* Date: Dec 20 2001                                                       */
/* Mod: Garbage collection de-implemented                                  */

  window_t *win1, *win2 ;
  /*apert_t  *apt, *apt2 ;*/
  mtm_t    *mtm ;
  itv_t    *itv ;
  int       nmtm, zeros=0, ones=0, ngrouped ;
  FILE      *fd ;
  unsigned long time_usr, time_sys ;


  if(log_step!=0) {
    if(!log_file) {
      trios_warning("Log will be written to stdout.") ;
    }
    if(!tmp_bas_file) {
      trios_warning("To save temporary basis, specify a file name.") ;
    }
  }
  else {
    if(tmp_bas_file) {
      trios_warning("To save temporary basis, specify a log step.") ;
    }
  }


  /* read classified examples */

  if(!(mtm=mtm_read(fname_i1, &win1/*, &apt*/))) {
    return trios_error(MSG, "lisi: mtm_read() failed to read %s", fname_i1) ;
  }

#ifdef _DEBUG_
trios_debug("Reading classified examples file: done.") ;
#endif

  /* get some info for the summary file */   
  nmtm = mtm->nmtm ;
  zeros = mtm_count(mtm, 0) ;
  ones = mtm_count(mtm, 1) ;

#ifdef _DEBUG_
trios_debug("Examples with label 0 = %d", zeros) ;
trios_debug("Examples with label 1 = %d", ones) ;
#endif


  /* read starting intervals */
  if(!(itv=itv_read(fname_i2, &win2/*, &apt2*/))) {
    win_free(win1) ;
    mtm_free(mtm) ;
    return trios_error(MSG, "lisi: itv_read() failed to read %s", fname_i2) ;
  }

#ifdef _DEBUG_
trios_debug("Reading starting intervals: done.") ;
#endif



  /* IF win1 != win2 ----> ERROR !! */

  if(!group_flag) {
    ngrouped = zeros ;
    if(!isi_basic(&itv, mtm, multi, isi_type, isi3_step,
		  log_step, log_file, tmp_bas_file, win1)) {
      win_free(win1) ;
      win_free(win2) ;
      itv_free(itv) ;
      mtm_free(mtm) ;
      return trios_error(MSG, "lisi: isi_basic() failed.") ;
    }
    mtm = NULL ;
  }
  else {
    if(!isi_gen(&itv, mtm, multi, isi_type, isi3_step, log_step,
		log_file, tmp_bas_file, win1, &ngrouped, &time_usr,
		&time_sys)) {
      win_free(win1) ;
      win_free(win2) ;
      itv_free(itv) ;
      mtm_free(mtm) ;
      return trios_error(MSG, "lisi: isi_gen() failed.") ;
    }
    mtm = NULL ;
  }


  /* write resulting intervals to the output file */
  if(!itv_write(fname_o, itv, win1/*, apt*/)) {
    win_free(win1) ;
    win_free(win2) ;
    itv_free(itv) ;
    mtm_free(mtm) ;
    return trios_error(MSG, "lisi: itv_write() failed to write %s", fname_o) ;
  }

#ifdef _DEBUG_
trios_debug("Writing intervals: done.") ;
#endif

  /* write summary if needed */
  if(log_file) {
    fd = fopen(log_file, "w") ;
    fprintf(fd, "mtm_file=%s\n", fname_i1) ;
    fprintf(fd, "ISI type=%d\n", isi_type) ;
    fprintf(fd, "ISI3 step=%d\n", isi3_step) ;
    fprintf(fd, "Examples=%d\n", nmtm) ;
    fprintf(fd, "Ones=%d\n", ones) ;
    fprintf(fd, "Zeros=%d\n", zeros) ;
    if(group_flag) { 
      fprintf(fd, "Grouped Zeros=%d\n", ngrouped) ;
      fprintf(fd, "gtime=%d+%d\n", time_usr, time_sys) ;
    }
    fprintf(fd, "basesize=%d\n", itv->nitv) ;
  }


  /* read classified examples */
  /*
  if(!(mtm=mtm_read(fname_i1, &win1, &apt))) {
    return pac_error(MSG, "lisi: mtm_read() failed to read %s", fname_i1) ;
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
  win_free(win1) ;
  win_free(win2) ;
  itv_free(itv) ;

  /*if(apt) apert_free(apt) ;*/


  return(1) ;
}


