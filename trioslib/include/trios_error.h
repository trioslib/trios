#ifndef _trios_error_h_
#define _trios_error_h_

/*===========================================================================\
   pac_error.c
\========================================================================== */


int        /*+ Purpose: Prints the error messages of the pac routines      +*/
  trios_error( 
    int type,              /*+ In: A flag
                                   1 : print message
                                   0 : do not print message                +*/
    const char* format,          /*+ In: The error message and the arguments     +*/
  ... ) ;



void       /*+ Purpose: Prints the error messages of the pac routines     +*/
  trios_fatal( 
    const char* format,       /*+ In: The error message and the arguments       +*/
    ... ) ;



void            /*+ Purpose: print message for debigging purposes         +*/
  trios_debug(                                                                  
    const char* format,       /*+ In: The error message and the arguments       +*/
  ... ) ;



void            /*+ Purpose: print message for warning purposes           +*/
  trios_warning(                                                                  
    const char* format,       /*+ In: The warning message and the arguments     +*/
    ... ) ;


#endif
