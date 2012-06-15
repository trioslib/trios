/*
 * Khoros: $Id: pacbasic.h,v 1.8 2002/01/11 14:53:58 nina Exp $
 */
 
/*
 * Copyright (C) 1993 - 1996, Khoral Research, Inc., ("KRI").
 * All rights reserved.  See $BOOTSTRAP/repos/license/License or run klicense.
 */


/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>> <<<<<<<<<<<<<<<<<<<<<<<<<<
   >>>>
   >>>>       Purpose: Definitions and prototypes for library pac_basic
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

#ifndef _pacbasic_h_
#define _pacbasic_h_


	/*-------------------------------------*
	|       #include
	---------------------------------------*/

#include <stdio.h>
/* #include </usr/lib/gcc/i486-linux-gnu/3.4.5/include/stddef.h> */
#include <stddef.h>
#include <stdlib.h> 
#include <malloc.h>
/* #include "/home/nina/dmalloc-4.8.2/dmalloc.h" */
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <stdarg.h>
/* #include </usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h> */
/* #include </usr/lib/gcc-lib/i386-linux/2.95.4/include/stdarg.h> */
/* #include </usr/lib/gcc-lib/i386-linux/2.95.4/include/varargs.h> */
/* #include <varargs.h> */


#include <trios_error.h>
#include <trios_win.h>
#include "trios_imgset.h"

#include <pac_xpl.h>
#include "trios_mtm.h"


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

/* Macro: Computes the number of bits ON of an integer (4 bytes) */
/* m_dim : the size vector                                       */
/* m_d : the size of the integer                                 */
/* m_X : the integer                                             */
#define Comp_Dim(m_X, m_dim, m_d)    \
        {                            \
          int m_k;                   \
                                     \
          m_d = 0 ;                  \
          m_k = m_X & 0xff ;         \
          m_d = m_d + m_dim[m_k] ;   \
          m_k = (m_X>>8)&0xff ;      \
          m_d = m_d + m_dim[m_k] ;   \
          m_k = (m_X>>16)&0xff ;     \
          m_d = m_d + m_dim[m_k] ;   \
          m_k = (m_X>>24)&0xff ;     \
          m_d = m_d + m_dim[m_k] ;   \
        }      


	/*-------------------------------------*
	|       routine definitions
	---------------------------------------*/


/*===========================================================================\
   pac_cmp.c
\========================================================================== */

int
  lpaccmp(
    char *i1,
    char *i2
  ) ;


/*===========================================================================\
   pac_math.c
\========================================================================== */

long pac_gen_seed(void ) ;

float ran1(long *idum) ;
 
float ran2x(long *idum) ;
 
float ran2y(long *idum) ;

float **matrix(long nrl, long nrh, long ncl, long nch) ;

float *vector(long nl, long nh) ;

void amoeba(float **p, float y[], int ndim, double ftol,
	float (*funk)(float [], float [], int), int *nfunk,
        float *px, int npx) ;

void free_matrix(float **m, long nrl, long nrh, long ncl, long nch) ;

void free_vector(float *v, long nl, long nh) ;

void free_fvector(float *v, int nl, int nh) ;

double beta(double z, double w) ;
 
/*double rint(double x) ;*/

float amotry(float **p, float y[], float psum[], int ndim,
	float (*funk)(float [], float [], int), int ihi, double fac,
        float *px, int npx) ;

float mylog2(double x) ;

float myrandom(double above, double below) ;

float average(float *a, int n) ;

float min(float *a, int n) ;

float sdev(float *a, int n) ;

int *ivector(int nl, int nh) ;

float *fvector(int nl, int nh) ;

double *dvector(int nl, int nh) ;

void free_ivector(int *v, int nl, int nh) ;

void free_dvector(double *v, int nl, int nh) ;

void generate_random_hyperplane(float *array_name, int length, double max_value) ;

/*===========================================================================\
   pac_common.c
\========================================================================== */

/*
     -------------------------------------------
     FUNCTION: wpat_dimension
     -------------------------------------------
*/
 
int             /*+ Purpose: Computes the number of bits ON in a w-pattern +*/
  wpat_dimension(                                                             
    unsigned int  *wpat,
    int  wzip          /*+ In: w-pattern compacted size                    +*/
)
/*+ Return: number of bits ON in the w-pattern                             +*/ ;

/*
     -------------------------------------------
     FUNCTION: wpat_order
     -------------------------------------------
*/
 
int          /*+ Purpose: verify the order relation between two w-patterns +*/
  wpat_order(                                                               
    unsigned int  *wpat1,
    unsigned int  *wpat2, 
    int  wzip          /*+ In: compacted w-pattern size                    +*/
)
/*+ Return: 3 if they cannot be compared, 0 if they are equal, 1 if 
            wpat1 < wpat2, and 2 if wpat1 > wpat2                          +*/ ;

/*
     -------------------------------------------
     FUNCTION: wpat_greater_than
     -------------------------------------------
*/
 
int          /*+ Purpose: verify whether a given w-pattern is greater than
                          another one                                      +*/
  wpat_greater_than(                                                               
    unsigned int  *wpat1,
    unsigned int  *wpat2,
    int  wzip          /*+ In: compacted w-pattern size                    +*/
) ;
/*+ Return: 1 if wpat1 > wpat2, 0 otherwise                                +*/


/** NOTE: 2007.01.06
    The "offset_create" function was created solely for use by the wrappers.
    Notice that in several places throughout the paclib a function will
    return a error message related with a function "offset_create", but in
    general that function will be calling "malloc" directly, rather than 
    using the "offset_create" function declared here.
**/

int* /*+ Purpose: create the offset vector according to window size +*/
    offset_create(
        int wsize /*+ In: window size +*/
    );
/*+ Return: Pointer to offset vector if successful or NULL in case of
            failure +*/


            
void  /*+ Purpose: set the offset vector according to a window and an image +*/
  offset_set(
    int      *offset, /*+ In/Out: the offset vector                         +*/
    window_t *win,    /*+ In: pointer to the window                         +*/
    int       width,  /*+ In: the width of the image                        +*/
    int       band    /*+ In: band of the window                            +*/ 
    ) ;


/*
     -------------------------------------------
     FUNCTION: wpat_compare
     -------------------------------------------
*/
 
int             /*+ Purpose: compare two w-patterns (taken as integers)    +*/
  wpat_compare(                                                               
    unsigned int  *wpat1,
    unsigned int  *wpat2,
    int  wzip          /*+ In: compacted w-pattern size                    +*/
) ;
/*+ Return: 0 if they are equal, -1 if the first is less and 1 if the first
            is greater than the second                                     +*/


/*
     -------------------------------------------
     FUNCTION: wpat_compareXG
     -------------------------------------------
*/
 
int             /*+ Purpose: compare two XG w-patterns                     +*/
  wpat_compareXG(                                                             
    char  *wpat1,      /*+ In: first w-pattern                             +*/
    char  *wpat2,      /*+ In: second w-pattern                            +*/
    int  wsize         /*+ In: w-pattern size                              +*/
) ;
/*+ Return: 0 if they are equal, -1 if the first is less and 1 if the first
            is greater than the second                                     +*/


/*
     -------------------------------------------
     FUNCTION: wpat_hamming
     -------------------------------------------
*/
 
int             /*+ Purpose: computes the Hamming distance between two
                    binary w-patterns                                      +*/
  wpat_hamming(                                                                
    unsigned int *wpat1,
    unsigned int *wpat2,
    int    wzip        /*+ In: compacted w-pattern's size                  +*/
) ;
/*+ Return: an integer between 0 and n (where n stands for wsize)          +*/

/*
     -------------------------------------------
     FUNCTION: size_of_zpat
     -------------------------------------------
*/

int               /*+ Purpose: calculate the size of compacted w-pattern +*/
  size_of_zpat(
    int  wsize    /*+ size of w-pattern                                  +*/
  ) ;
/*+ Return: the compacted w-pattern size                                 +*/
 


freq_node *              /*+ Purpose: Creates a node for the frequency list +*/
 freq_node_create(
    int label,           /*+ In: label of the pattern                       +*/
    unsigned int freq    /*+ In: frequency of the pattern                   +*/
) ;


 
void
  freqlist_free( 
    freq_node *freq_list      /*+ In: frequency list to be free          +*/
) ;


 
int
  set_freq(                                                           
    freq_node *freqlstin,     /*+ In: pointer to the input frequency list  +*/
    freq_node **freqlist      /*+ In/Out: pointer to the frequency list    +*/
) ;



/*
     -------------------------------------------
     FUNCTION: FREQ_SUM
     -------------------------------------------
*/
 
unsigned int    /*+ Purpose: Sums the frequencies of w-pattern             +*/
  freq_sum(   
    freq_node  *freqlist     /*+ In: frequency list of a w-pattern         +*/
) ;
/*+ Return: The sum of the frequencies of the labels that appeared in the 
            w-pattern                                                      +*/





	/* Ghostwriter Prototypes:  Do Not Erase This Comment */


#endif /* _pacbasic_h_ */

/* Don't add after this point */


