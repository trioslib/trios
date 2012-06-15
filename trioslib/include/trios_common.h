#ifndef _trios_common_h_
#define _trios_common_h_

#include <trios_error.h>
#include <trios_win.h>
#include "trios_imgset.h"

#include <trios_xpl.h>
#include "trios_mtm.h"


int
  lpaccmp(
    char *i1,
    char *i2
  ) ;


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




#endif /* _trios_common_h_ */

/* Don't add after this point */


