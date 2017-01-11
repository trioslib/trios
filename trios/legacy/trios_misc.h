#ifndef _trios_misc_h_
#define _trios_misc_h_

#include <stdlib.h>


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

extern unsigned int bitmsk[32] ;

int set_freq(freq_node * freqlstin, freq_node ** freqlist);
freq_node *freq_node_create(int label, unsigned int freq);
void freqlist_free(freq_node * freq_list);

int size_of_zpat(int wsize);


#endif
