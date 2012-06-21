/*
 * Khoros: $Id: pacio_local.h,v 1.2 2000/02/28 17:55:42 hirata Exp $
 */
 
/*
 * Copyright (C) 1993 - 1996, Khoral Research, Inc., ("KRI").
 * All rights reserved.  See $BOOTSTRAP/repos/license/License or run klicense.
 */


/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>> <<<<<<<<<<<<<<<<<<<<<<<<<<
   >>>>
   >>>>       Purpose:	Private include file for library `pacio'.
   >>>>			This is (should be) included by all source
   >>>>			files comprising the library.  This file includes
   >>>>			the public include file for the library:
   >>>>				<pacio/pacio.h>
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


#ifndef _pacio_internals_h_
#define _pacio_internals_h_



	/*-------------------------------------*
	|	#includes
	\--------------------------------------*/

/* #include <paclib.h> */
/* #include "io_header.h" */ /* this will be in pac/paclib/include for a while */


	/*-------------------------------------*
	|	#defines
	\--------------------------------------*/


	/*-------------------------------------*
	|	typedefs
	\--------------------------------------*/


	/*-------------------------------------*
	|	global variable declarations
	\--------------------------------------*/


	/*-------------------------------------*
	|	macros
	\--------------------------------------*/


	/*-------------------------------------*
	|	routine definitions
	\--------------------------------------*/


int              /*+ Purpose: Computes the label of the color bands              +*/
  comp_c_label(                                                       
	  int       plabel,  /*+ In: Color label in integer format               +*/
	  int       koR,     /*+ In: Output range for Red band                   +*/
	  int       koG,     /*+ In: Output range for Green band                 +*/
	  int       koB,     /*+ In: Output range for Blue band                  +*/
	  int       *labelR, /*+ Out: Red label                                  +*/
	  int       *labelG, /*+ Out: Green label                                +*/
	  int       *labelB  /*+ Out: Blue label                                 +*/
	  )
/*+ Return: 1 on success                                                         +*/ ;


void             /*+ Purpose: Write a BB examples file                       +*/
  xpl_BB_write_tree(
    FILE *fd,    /*+ In: file descriptor where the examples will be written  +*/
    xpl_BB *p,   /*+ In: Pointer to the examples tree                        +*/
    int wzip     /*+ In: Number of bytes used by the compacted window        +*/
    ) ;





/* --------- io_showitv.c ---------------------------------- */



/* --------- io_showxpl.c ---------------------------------- */

int 
  xpl_show_basicinfo(
    FILE  *fd,
    xpl_t *xpl,     
    int   select_type,
    int   sort_type,  
    int   order_type, 
    int   from,       
    int   to 
) ;


int 
  xpl_show_BB(
    FILE  *fd,
    xpl_t *xpl,
    window_t *win,
    int   select_type,
    int   from,       
    int   to
  ) ;


int
  xpl_show_BG(
    FILE  *fd,
    xpl_t *xpl,
    window_t *win,
    int   select_type,
    int   from,       
    int   to
  ) ;


int
  xpl_show_WK(
    FILE  *fd,         /*+ In: output file descriptor                   +*/ 
    xpl_t *xpl,        /*+ In: XPL set                                  +*/
    window_t *win,     /*+ In: Window                                   +*/
    int   select_type, /*+ In: type of examples to be showed
                              1 : All
                              2 : only conflicting ones
                              3 : only non-conflicting ones             +*/
    int   from,        /*+ In: range start                              +*/
    int   to           /*+ In: range end                                +*/
  ) ;



/* --------- io_showmtm.c ---------------------------------- */

/*
     -------------------------------------------
     FUNCTION: mtm_show_basicinfo
     -------------------------------------------
*/
 
int mtm_show_basicinfo /*+ Purpose: write out some basic 
                           information concerning the MTM set+*/
  ( 
    FILE *fd,          /*+ In: output file descriptor                    +*/
    mtm_t *mtm,        /*+ In: MTM set                                   +*/
    char *fname_i,     /*+ In: MTM file name                             +*/
    int  from,         /*+ In: range start                               +*/
    int  to            /*+ In: range end                                 +*/
) ;
/*+ Returns: 1 on success, 0 on failure +*/




#endif /* _pacio_internals_h_ */
/* Don't add after this point */

