/*
 * Khoros: $Id: pacdt_local.h,v 1.4 2001/03/20 17:35:23 hirata Exp $
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


#ifndef _pacdt_internals_h_
#define _pacdt_internals_h_



	/*-------------------------------------*
	|	#includes
	\--------------------------------------*/


	/*-------------------------------------*
	|	#defines
	\--------------------------------------*/


	/*-------------------------------------*
	|	typedefs
	\--------------------------------------*/
typedef int (*qsort_compare_type)(const void *, const void *);

	/*-------------------------------------*
	|	global variable declarations
	\--------------------------------------*/


	/*-------------------------------------*
	|	macros
	\--------------------------------------*/


	/*-------------------------------------*
	|	routine definitions
	\--------------------------------------*/


/* --------- classify.c ---------------------------- */


struct test_outcome estimate_accuracy(
				      POINT **points,
				      int no_of_points,
				      struct tree_node *root) ;



/*
     -------------------------------------------
     FUNCTION: classify_wpat - Labels a wpattern
     -------------------------------------------
*/
 
int             /*+ Purpose: To label a wpattern                           +*/
  classify_wpat(
    unsigned char *wpat,     /*+ In: pointer to an array of configurations +*/
    struct tree_node *root   /*+ In: root if the tree                      +*/
) ;
/*+ Return: The label of a wpattern                                        +*/



/* --------- color_classify.c ---------------------------- */

/*
     -------------------------------------------
     FUNCTION: dt_applicWK3F
     -------------------------------------------
*/

int             /*+ Purpose: To transform a color image by an operator     +*/
  dtapplicWK3F(
    unsigned char  *ucpixels1, /*+ In: Image to be applied                 +*/
    unsigned char  *ucpixels2, /*+ In: Mask image                          +*/
    unsigned char  *ucpixels3, /*+ Out: resulting output image             +*/
    struct tree_node *root,    /*+ In: basis file                          +*/
    window_t  *win,    /*+ In: window                                      +*/
    apert_t   *apt,    /*+ In: aperture                                    +*/
    int        width,  /*+ In: number of columns of the image              +*/
    int        npixels /*+ In: number of pixels                            +*/
) ;
/*+ Return: 1 on success 0 on failure                                      +*/


/*
     -------------------------------------------
     FUNCTION: dt_applicWK3C
     -------------------------------------------
*/

int             /*+ Purpose: To transform a color image by an operator     +*/
  dtapplicWK3C(
    unsigned char  *ucpixels1, /*+ In: Image to be applied                 +*/
    unsigned char  *ucpixels2, /*+ In: Mask image                          +*/
    unsigned int   *uipixels3, /*+ Out: resulting output image             +*/
    struct tree_node *root,    /*+ In: basis file                          +*/
    window_t  *win,    /*+ In: window                                      +*/
    apert_t   *apt,    /*+ In: aperture                                    +*/
    int        width,  /*+ In: number of columns of the image              +*/
    int        npixels /*+ In: number of pixels                            +*/
) ;
/*+ Return: 1 on success 0 on failure                                      +*/




/*
     -------------------------------------------
     FUNCTION: dt_applicWKGG2C
     -------------------------------------------
*/

int             /*+ Purpose: To transform a color image by an operator     +*/
  dtapplicWKGG2C(
    unsigned char  *ucpixels1, /*+ In: Image to be applied                 +*/
    unsigned char  *ucpixels2, /*+ In: Mask image                          +*/
    unsigned int   *uipixels3, /*+ Out: resulting output image             +*/
    struct tree_node *root,    /*+ In: basis file                          +*/
    window_t  *win,    /*+ In: window                                      +*/
    apert_t   *apt,    /*+ In: aperture                                    +*/
    int        width,  /*+ In: number of columns of the image              +*/
    int        npixels /*+ In: number of pixels                            +*/
) ;





/*
     -------------------------------------------
     FUNCTION: dt_applicWKGG2F
     -------------------------------------------
*/

int             /*+ Purpose: To transform a color image by an operator     +*/
  dtapplicWKGG2F(
    unsigned char  *ucpixels1, /*+ In: Image to be applied                 +*/
    unsigned char  *ucpixels2, /*+ In: Mask image                          +*/
    unsigned char  *ucpixels3, /*+ Out: resulting output image             +*/
    struct tree_node *root,    /*+ In: basis file                          +*/
    window_t  *win,    /*+ In: window                                      +*/
    apert_t   *apt,    /*+ In: aperture                                    +*/
    int        width,  /*+ In: number of columns of the image              +*/
    int        npixels /*+ In: number of pixels                            +*/
) ;



/*
     -------------------------------------------
     FUNCTION: dtapplicGG3
     -------------------------------------------
*/

int             /*+ Purpose: To transform a color image by an operator     +*/
  dtapplicGG3(
    unsigned char  *ucpixels1, /*+ In: Image to be applied                 +*/
    unsigned char  *ucpixels2, /*+ In: Mask image                          +*/
    unsigned char  *uipixels3, /*+ Out: resulting output image             +*/
    struct tree_node *root,    /*+ In: basis file                          +*/
    window_t  *win,    /*+ In: window                                      +*/
    int        width,  /*+ In: number of columns of the image              +*/
    int        npixels /*+ In: number of pixels                            +*/
) ;



/* --------------------- compute_impurity.c ---------------- */

float compute_impurity(int cur_no_of_points) ;

void set_counts(POINT **cur_points, int cur_no_of_points, int flag) ;

void reset_counts(void) ;

int largest_element(int *array, int count) ;

int stop_splitting(void) ;

void find_values(POINT **cur_points, int cur_no_of_points) ; 


/* ------ impurity_measures.c ------------------ */

float maxminority(void) ;

float summinority(void) ;

float variance(void) ;

int var_compare(int *p1, int *p2) ;

float info_gain(void) ;

float gini_index(void) ;

float twoing(void) ;

/* --------- io_dt.c ---------------- */

int                 /*+ Purpose: Reads a dt file                            +*/
  dt_read(
    char *fname,    /*+ In:  name of the file of the decision tree          +*/
    int      *type, /*+ Out: type of the operator                           +*/
    window_t **win, /*+ Out: pointer to window structure                    +*/
    apert_t  **apt  /*+ Out: pointer to aperture structure                  +*/
    ) ;

int                  /*+ Purpose: Writes the miniterms collection to a file  +*/
  dt_write(
    char *fname,      /*+ In:  name of the file of the decision tree          +*/
    int      type,    /*+ In: type of the operator                            +*/
    window_t *win,    /*+ In: pointer to window structure                     +*/ 
    apert_t  *apt     /*+ In: pointer to apert  structure                     +*/ 
    ) ;

/* --------- mktree.c --------------------------- */

POINT **allocate_point_array(POINT **array_name, int size, int prev_size) ;

int allocate_structures(int no_of_points) ;

int deallocate_structures(int no_of_points) ;

int shuffle_points(POINT **array_name, int count) ;

struct tree_node *build_tree(
			     struct point **points,
			     int no_of_training_points,
			     char *dt_file ) ;
struct tree_node *build_subtree(
                  char *node_str,
                  POINT **cur_points,
                  int cur_no_of_points) ;
float cart_split(
		 POINT **cur_points,
		 int cur_no_of_points,
     char *cur_label) ;
struct tree_node *create_tree_node(void) ;

float oblique_split(
     POINT **cur_points,
     int cur_no_of_points,
     char *cur_label) ;

int alter_coefficients(
		       struct point **cur_points,
		       int cur_no_of_points) ;


float axis_parallel_split(
			  POINT **cur_points,
     int cur_no_of_points) ;

int print_log_and_exit(void) ;

int normalize_data(
      struct point **points,
      int no_of_points) ;
int unnormalize_data(
     struct point **points,
     int no_of_points) ;

int unnormalize_hyperplane(void) ;


/* ------ perturb.c ------------------------------ */

float cart_perturb(
		   POINT **cur_points,
		   int cur_no_of_points, 
		   int cur_coeff,
                   double cur_error) ;

float cart_perturb_constant(
			    POINT **cur_points,
			    int cur_no_of_points,
                            double cur_error) ;

float suggest_perturbation(
			   POINT **cur_points,
			   int cur_no_of_points,
			   int cur_coeff,
			   double cur_error);

float perturb_randomly(
		       POINT **cur_points,
		       int cur_no_of_points,
		       double cur_error,
		       char *cur_label) ;

int compare(struct unidim  *ptr1, struct unidim *ptr2) ;

float linear_split(int no_of_eff_points) ;


/* --------- tree_util.c ---------------------------- */

struct tree_node *read_tree(char *decision_tree) ;

void read_subtree(struct tree_node *root, FILE *dtree) ;

struct tree_node *read_hp(FILE *dtree) ;

int isleftchild(struct tree_node *x, struct tree_node *y) ;

int isrightchild(struct tree_node *x, struct tree_node *y) ;

int read_header(FILE *dtree) ;

int leaf_count(struct tree_node *cur_node) ;


int tree_depth(struct tree_node *cur_node) ;

void write_tree(struct tree_node *root, char *dt_file) ;

void write_subtree(struct tree_node *cur_node, FILE *dtree) ;

void write_hp(struct tree_node *cur_node, FILE *dtree) ;

void write_header(FILE *dtree) ;

#endif /* _pacdt_internals_h_ */
/* Don't add after this point */

