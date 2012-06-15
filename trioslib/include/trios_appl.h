/*
 * Khoros: $Id: pacappl.h,v 1.5 2000/02/28 17:47:18 hirata Exp $
 */

/*
 * Copyright (C) 1993 - 1996, Khoral Research, Inc., ("KRI").
 * All rights reserved.  See $BOOTSTRAP/repos/license/License or run klicense.
 */


/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>> <<<<<<<<<<<<<<<<<<<<<<<<<<
   >>>>
   >>>>       Purpose: Definitions and prototypes for library pac_io
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

#ifndef _pacappl_h_
#define _pacappl_h_


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


int lappl0 ( /*+ Faz a aplicacao da fase 0 +*/
    char *f_appl,  /*+ In: image on which operator will be applied         +*/
    char *f_mask,  /*+ In: Mask image filename                             +*/
    char *f_win,   /*+ In: Arquivo com as descricoes das janelas           +*/
    char *p_operadores,  /*+ In: prefixo dos operadores de saida           +*/
    char *p_resultante   /*+ In: prefixo das imagens resultantes           +*/
    ) ;
/*+ Return: 1 on success, 0 on failure                                   +*/

/*
     -------------------------------------------
     FUNCTION: lsapplic
     -------------------------------------------
*/

int             /*+ Purpose: Applies an operator basis on an image         +*/
  lsapplic(
    char *f_appl,  /*+ In: image on which operator will be applied         +*/
    char *f_basis, /*+ In: basis file                                      +*/
    char *f_mask,  /*+ In: Mask image filename                             +*/
    int  cv,       /*+ In: on template matching, looks only to pixels
		     of the window with the same label as the center's one +*/
    int  hash_flag,/*+ In: Switch between the normal and hash application  +*/
    int  on_value, /*+ In: value of the output image pixel                 +*/
    char *f_out,   /*+ Out: resulting output image filename                +*/
    int  dx,       /*+ In: deslocamento do centro da janela nas linhas     +*/
    int  dy        /*+ In: deslocamento do centro da janela nas colunas    +*/
    );

/*===========================================================================\
   dpac_appl.c
\========================================================================== */

int             /*+ Purpose: Applies an operator basis on an image         +*/
  lpapplic(
    char *f_appl,  /*+ In: image on which operator will be applied         +*/
    char *f_basis, /*+ In: basis file                                      +*/
    char *f_mask,  /*+ In: Mask image filename                             +*/
    int  cv,       /*+ In: on template matching, looks only to pixels
		     of the window with the same label as the center's one +*/
    int  hash_flag,/*+ In: Switch between the normal and hash application  +*/
    int  on_value, /*+ value to be assigned to the ON-points               +*/
    char *f_out    /*+ Out: resulting output image filename                +*/
    ) ;



/*===========================================================================\
   dapplmm.c
\========================================================================== */

/*
     -------------------------------------------
     FUNCTION: lpapplmm
     -------------------------------------------
*/

int             /*+ Purpose: Computes the supremum of a set of erosions
                             by flat structuring elements (for the moment,
                             mask image is not being processed         +*/
  lpapplmm(
    char *f_appl,  /*+ In: Input image filename                        +*/
    char *f_basis, /*+ In: basis file                                  +*/
    char *f_mask,  /*+ In: Mask image filename                         +*/
    char *f_out    /*+ In: resulting output image filename             +*/
) ;
/*+ Return 1 on success, 0 on failure                                  +*/


/* In file ipac_appl2.c */
int            /*+ Applies phase 2 operator +*/
  apply2 (
    unsigned short **us_input, /*+ In: sequence of images from previous
                                       phase +*/
    unsigned char  *uc_mask, /*+ In: mask+*/
    unsigned char  *uc_output, /*+ Out: output image +*/
    itv_t          *itv, /*+ In: interval object specifying operator +*/
    int            wsize, /*+ In: window size+*/
    int            nwin, /*+ In: number of windows +*/
    window_t       **win,
    int            width,     /*+ In: width, common to all images  +*/
    int            npixels /*+ In: number of pixels +*/
   );


#endif /* _pacappl_h_ */
/* Don't add after this point */
