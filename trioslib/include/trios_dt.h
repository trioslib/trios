/*
 * Khoros: $Id: pacdt.h,v 1.5 2001/03/20 17:14:34 hirata Exp $
 */
 
/*
 * Copyright (C) 1993 - 1996, Khoral Research, Inc., ("KRI").
 * All rights reserved.  See $BOOTSTRAP/repos/license/License or run klicense.
 */


/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>> <<<<<<<<<<<<<<<<<<<<<<<<<<
   >>>>
   >>>>       Purpose: Definitions and prototypes for library pacdt
   >>>>
   >>>>       Toolbox: $RHJPACLIB
   >>>>
   >>>>    Written By: Your name goes here!
   >>>>
   >>>>          Date: 
   >>>>
   >>>> Modifications:
   >>>>
   >>>>>>>>>>>>>>>>>>>>>>>>>>>>>> <<<<<<<<<<<<<<<<<<<<<<<<<< */

#ifndef _pacdt_h_
#define _pacdt_h_

#ifdef __cplusplus
extern "C" {
#endif


	/*-------------------------------------*
	|       #include
	---------------------------------------*/


	/*-------------------------------------*
	|       #defines
	---------------------------------------*/

/* Change the following statement to use a different impurity measure. */

#define IMPURITY twoing
/* possible values are: maxminority                             */
/*			summinority				*/
/*			variance				*/
/*			info_gain				*/
/*			gini_index				*/
/*			twoing					*/


#define NO_OF_STD_ERRORS	0 /* used for cost complexity pruning, 
                                     in prune.c */
#define SEQUENTIAL 		0
#define BEST_FIRST 	 	1	
#define RANDOM                  2

#define CORRECT 		1
#define INCORRECT 		0

#define TRUE 			1
#define FALSE 			0

#define LEFT 			0
#define RIGHT 			1

#define LESS_THAN 		0
#define MORE_THAN 		1

#define MAX_COEFFICIENT 	1.0
#define MAX_NO_OF_ATTRIBUTES	100
#define MAX_DT_DEPTH 		150 
#define MAX_NO_OF_STAGNANT_PERTURBATIONS 	10
#define MAX_CART_CYCLES         100

#define TOLERANCE		0.0001
#define TOO_SMALL_THRESHOLD	2.0
#define TOO_SMALL_FOR_ANY_SPLIT		1	
#define TOO_SMALL_FOR_OBLIQUE_SPLIT	2 * no_of_dimensions

#define TRAIN			1
#define	TEST			2

#define LINESIZE 		255
#define MISSING_VALUE           -1.0 * HUGE

#define translatex(x) ((x - xmin) * (pmaxx - pminx) / (xmax - xmin) + pminx)
#define translatey(y) ((y - ymin) * (pmaxy - pminy) / (ymax - ymin) + pminy)


	/*-------------------------------------*
	|       typedefs
	---------------------------------------*/

typedef struct point
 {
   float *dimension;
   int category;
   double val; /*Value obtained by substituting this point in the 
                equation of the hyperplane under consideration.
                This field is maintained to avoid redundant
                computation. */
 } POINT ;

struct endpoint
 {
  float x,y;
 } ;

typedef struct edge
 {
  struct endpoint from,to;
 } EDGE ;

struct tree_node
 {
  float *coefficients;
  int *left_count, *right_count;
  struct tree_node *parent,*left,*right;
  int left_cat,right_cat;
  char label[MAX_DT_DEPTH];
  float alpha; /* used only in error_complexity pruning. */
  int no_of_points;
  EDGE edge; /* used only in the display module. */
 };

struct unidim
 {
  float value;
  int cat;
 } ;

struct test_outcome
 {
  float leaf_count,tree_depth;
  float accuracy;
  int *class; /*For each class, store the number of correctly
                classified examples and total number of examples */
 } ;

	/*-------------------------------------*
	|       global variable declarations
	---------------------------------------*/


	/*-------------------------------------*
	|       macros
	---------------------------------------*/


	/*-------------------------------------*
	|       routine definitions
	---------------------------------------*/

/*
void error(),
     free_ivector(),
     free_vector(), 
     free_dvector();

float myrandom(), 
      *vector();

double *dvector();

int *ivector();

float average(),
      sdev();
*/


/*
     -------------------------------------------
     FUNCTION: ltrainDT 
     -------------------------------------------
*/
 
int                   /*+ Purpose: Generates a decision tree from a mtm
			  file                                                +*/
  ltrainDT (
  char  *mtm_file,        /*+ In: mtm file                                    +*/
  int   oblique_mode,     /*+ In: uses oblique cutting                        +*/
  int   cart,             /*+ In: uses cart mode                              +*/
  int   nrestart,         /*+ In: number of restarts                          +*/
  int   njumps,           /*+ In: number of jumps                             +*/
  char  *log_file_name,   /*+ Out: name of the file to record the logs        +*/
  char  *dt               /*+ Out: name of the file to storage the tree       +*/
)
/*+ Return: 1 if OK, 0 if it fails                                            +*/ ;



/*
     -------------------------------------------
     FUNCTION: dtapplic0 - Gray-scale application
     -------------------------------------------
*/
 
int             /*+ Purpose: To transform an image by an operator          +*/
  dtapplic0(
    unsigned char  *ucpixels,  /*+ In: Image to be applied                 +*/
    unsigned char  *ucpixels1, /*+ In: Mask image                          +*/
    unsigned char  *ucpixels2, /*+ Out: resulting output image             +*/
    struct tree_node *root,    /*+ In: basis file                          +*/
    window_t  *win,    /*+ In: window                                      +*/
    int        nc,     /*+ In: number of columns of the image              +*/
    int        npixels /*+ In: number of pixels                            +*/
) ;
/*+ Return: 1 on success 0 on failure                                      +*/




/*
     -------------------------------------------
     FUNCTION: dtapplic1
     -------------------------------------------
*/
 
int             /*+ Purpose: To transform an image by an operator          +*/
  dtapplic1(
    unsigned char  *ucpixels,  /*+ In: Image to be applied                 +*/
    unsigned char  *ucpixels1, /*+ In: Mask image                          +*/
    unsigned char  *ucpixels2, /*+ Out: resulting output image             +*/
    struct tree_node *root,    /*+ In: basis file                          +*/
    window_t  *win,    /*+ In: window                                      +*/
    apert_t   *apt,    /*+ In: aperture                                    +*/
    int        nc,     /*+ In: number of columns of the image              +*/
    int        npixels /*+ In: number of pixels                            +*/
)
/*+ Return: 1 on success 0 on failure                                      +*/ ;



/*
     -------------------------------------------
     FUNCTION: dtapplic2
     -------------------------------------------
*/
 
int             /*+ Purpose: To transform an image by an operator          +*/
  dtapplic2(
    unsigned char  *ucpixels,  /*+ In: Image to be applied                 +*/
    unsigned char  *ucpixels1, /*+ In: Mask image                          +*/
    unsigned int   *uipixels2, /*+ Out: resulting output image             +*/
    struct tree_node *root,    /*+ In: basis file                          +*/
    window_t  *win,    /*+ In: window                                      +*/
    apert_t   *apt,    /*+ In: aperture                                    +*/
    int     nc,        /*+ In: number of columns of the image              +*/
    int     npixels    /*+ In: number of pixels                            +*/
)
/*+ Return: 1 on success 0 on failure                                      +*/ ;



/*
     -------------------------------------------
     FUNCTION: lapplicDT
     -------------------------------------------
*/

int             /*+ Purpose: Applies a decision tree on an image       +*/
  lapplicDT(
    char *f_appl,  /*+ In: Image filename to be applied                +*/
    char *dt,      /*+ In: decision tree spec file                     +*/
    char *f_mask,  /*+ In: Mask image filename                         +*/
    char *f_res    /*+ Out: resulting output image filename            +*/ 
)
/*+ Return 1 on success, 0 on failure                                  +*/ ;


/*
     -------------------------------------------
     FUNCTION: lapplicDTC
     -------------------------------------------
*/

int             /*+ Purpose: Applies a decision tree on a color image  +*/
  lapplicDTC(
    char *f_appl,  /*+ In: Image filename to be applied                +*/
    char *dt,      /*+ In: decision tree spec file                     +*/
    char *f_mask,  /*+ In: Mask image filename                         +*/
    char *f_res    /*+ Out: resulting output image filename            +*/ 
)
     /*+ Return 1 on success, 0 on failure                                  +*/ ;


	/* Ghostwriter Prototypes:  Do Not Erase This Comment */

#ifdef __cplusplus
}
#endif

#endif /* _pacdt_h_ */


/* Don't add after this point */


