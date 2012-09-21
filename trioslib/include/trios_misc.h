#ifndef _trios_misc_h_
#define _trios_misc_h_

#ifdef __cplusplus
extern "C" {
#endif

#define  MSG     2    /*!< parameter for routine pac_error()
 0: does not print error message
 1: print error message with "ERROR" in front of the message
 2: print error message without "ERROR" in front of the message */

#define  NB      32
#define  NB_1    31

/* Mapping types */
#define  BB      0  /*!< Binary to binary                             */
#define  BG      1  /*!< Binary to grayscale (multiclassification)    */
#define  GB      2  /*!< Grayscale to binary (probably obsolete)      */
#define  GG      3  /*!< Grayscale to grayscale                       */
#define  WKF     4  /*!< Restricted grayscale to restricted grayscale */
#define  WKC     5  /*!< Restricted grayscale to grayscale            */
#define  WKGG2F  6  /*!< The hybrid of WK and GG to color images      */
#define  WKGG2C  7  /*!< The hybrid of WK and GG to color images      */
#define  WK3F    8  /*!< The equivalent of WKF to color images        */
#define  WK3C    9  /*!< The equivalent of WKC to color images        */
#define  GG3    10  /*!< The equivalent of GG to color images         */


#ifndef FLT_MAX

#define FLT_MAX 3.40282347e+38F
#endif

#ifndef HUGE
#define HUGE FLT_MAX
#endif


/* defines for routines from NRC */
#define NR_END 1 
#define FREE_ARG char*

#define FTOL 1.0e-6 /* fractional convergence tolerance for beta
                       parameter estimation                      */

#define PI 3.14159265358979323846264338
#define IM1 2147483563
#define IM2 2147483399
#define AM1 (1.0/IM1)
#define IMM1 (IM1-1)
#define IA1 40014
#define IA2 40692
#define IQ1 53668
#define IQ2 52774
#define IR1 12211
#define IR2 3791
#define NTAB 32
#define NDIV1 (1+IMM1/NTAB)
#define EPSILON 1.2e-7
#define RNMX (1.0-EPSILON)
#define NBINS 100
#define NRAND 40000
#define IA 16807
#define IM 2147483647
#define AM (1.0/IM)
#define IQ 127773
#define IR 2836
#define NDIV (1+(IM-1)/NTAB)

#define NOT_A_LABEL 63498  /*!< This constant is used to identify a label */


/*!

 */
typedef struct freq_node {
        int label ;               /*!< label of the pattern            */
    unsigned int freq ;       /*!< frequency of the pattern        */
    struct freq_node *next;   /*!< pointer to the next found label */
} freq_node ;

/*!
    Linked list of indexes.
 */
typedef struct idx_ll {
  int    idx ;
  struct idx_ll *next ;
} idx_ll ;


	/*-------------------------------------*
	|       global variable declarations
	---------------------------------------*/

extern unsigned int bitmsk[32] ;

#ifdef __cplusplus
}
#endif


#endif






