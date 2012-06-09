#include <pacbasic.h>
#include <pacio.h>
#include "pacio_local.h"

/* #define _DEBUG_ */
/* #define _DEBUG_1_ */

/* Modification by:  Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)       */
/* Date: Wed Feb 17 2000                                                   */
/* Mod: Window structure (1) changed to support multiple bands, (2) no     */
/*      longer holds information about aperture. Modifications were made   */
/*      to adequate to the structure changing.                             */


/*!
    Read a window structure from a file.

    \param fname File name.
    \return Window read from file. NULL on failure.
*/

window_t *win_read(char *fname) {
/* author:  Nina S. Tomita & Roberto Hirata Jr. (nina@ime.usp.br)         */
/* date: Thu Oct 17 1996                                                  */


  FILE      *fd ;
  window_t  *win;                 /* window            */


  
  /* open file */
  fd = fopen(fname, "r") ;
  if(fd == NULL) {
    return (window_t *)trios_error(1, "File (%s) open failed.", fname) ;
  }
  
  /* read & check file header ------------------------------------------- */
  if(!header_match(fd, "WINSPEC ")) {
    fclose(fd) ;
    return (window_t *)trios_error(1, "File header does not match.") ;
  }
  
#ifdef _DEBUG_ 
  pac_debug("Leu header") ;  
#endif

#ifdef _DEBUG_ 
  pac_debug("vai entrar no read_win_data") ;  
#endif

  if(NULL==(win = win_read_data(fd))) {
    fclose(fd) ;
    return (window_t *)trios_error(MSG, "win_read: win_read_data() failed.") ;
  }

  fclose(fd) ;
  return(win) ;
}


/*!
    Read a description of the window.

    \param fd File descriptor.
    \return Window with the read data. NULL on failure.
*/

window_t *win_read_data(FILE *fd) {
/* author:  Nina S. Tomita & Roberto Hirata Jr. (nina@ime.usp.br)  */
/*  date: Thu Oct 17 1996                                          */

/*  Modification by:  Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)       */
/*  Date: Thu Oct 30 1997                                                   */
/*  Mod: The specification of the file format was changed to include tags   */
/*       (.h, .w, etc). The reading procedure was modified to read this new */
/*       format. It was also included a new parameter wkrange.              */

/*  Modification by:  Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)       */
/*  Date: Wed Jun  3 1998                                                   */
/*  Mod: Modified to include the vertical placement                         */

/*  Modification by:  Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)       */
/*  Date: Fri Oct 29 1999                                                   */
/*  Mod: Modified to include support to different input/output ranges       */

/* Date: Wed Feb 16 2000                                                   */
/* Mod: Window structure (1) changed to support multiple bands, (2) no     */
/*      longer holds information about aperture. Modifications were made   */
/*      to adequate to the structure changing.                             */


  char     tag, dot ;
  int      i, j, k, pt, tags_read, stop ;
  int      height, width, nbands ;
  window_t *win ;


  stop = 0 ;
  tags_read = 0 ;  /* No mandatory data read, yet */
  nbands = 1 ;     /* default value */


  while(!stop)  {
    
    while(((dot=(char)fgetc(fd)) != '.') && (dot!=(char)EOF)) ;

    if(dot==(char)EOF) {
      fclose(fd) ;
      return (window_t *)trios_error(1, "Unexpected end of file. No tag found.") ;
    }

    tag = (char)fgetc(fd) ;

    switch (tag) {
      
    case 'h': 
      if(1 != fscanf(fd, "%d", &height)) {
        fclose(fd) ;
        trios_fatal("Unexpected data or end of file") ;
      }  
      tags_read++ ;
      break ;

    case 'w':
      if(1 != fscanf(fd, "%d", &width )) {
        fclose(fd) ;
        trios_fatal("Unexpected data or end of file") ;
      }
      tags_read++ ;
      break ;

    case 'b':
      if(1 != fscanf(fd, "%d", &nbands )) {
        fclose(fd) ;
        trios_fatal("Unexpected data or end of file") ;
      }
      break ;

    case 'd' : 
      stop = 1 ;
      break ;

    default : 
      (void)trios_error(1,"Unexpected tag %c ", tag) ;
      return (window_t *)trios_error(1, " File format error") ;
    }
  }

  if(tags_read != 2) {
    return (window_t *)trios_error(1, "Window width or height is missing.") ;
  }


  if(NULL == (win = win_create(height, width, nbands))) {
    return (window_t *)trios_error(MSG, "win_read_data: win_create() failed.") ;
  }

  
  /* data reading */
  for(k=1; k <= nbands; k++) {
    for(i=0; i < height; i++) {
      for(j=0; j < width; j++) {

	if(fscanf(fd, "%d ", &pt) != 1) {
	  win_free(win) ;
	  return
	    (window_t *)trios_error(1,"Unexpected data or end of file.") ;
	}

	if((pt != 0) && (pt != 1)) {
	  win_free(win) ;
	  return (window_t *)trios_error(1, "Data must be 0 or 1.") ;
	}
	
	if(!win_set_point( i , j , k, pt , win )) {
	  win_free(win) ;
	  return (window_t *)trios_error(MSG, "win_read_data: win_set() failed.") ;
	}
      }
    }
  }
   
  return(win);
}


/*!
    Write a file with the window data.

    \param fname File name.
    \param win Window to write.
    \return 1 on success. 0 on failure.
*/

int win_write(char *fname, window_t *win) {
/* author:  Nina S. Tomita & Roberto Hirata Jr. (nina@ime.usp.br)        */
/*  date: Thu Oct 17 1996                                                */


  header_t winHeader = {"WINSPEC ", ""};
  FILE     *fd ;

#ifdef _DEBUG_
pac_debug("Entrei no win_write.");
#endif

  /* open file */
  fd = fopen(fname, "w") ;
  if(fd == NULL) {
    return trios_error(1, "File (%s) open failed.", fname) ;
  }
  
#ifdef _DEBUG_
pac_debug("Passei pelo fopen().");
#endif

  /* writes file header */
  header_write(fd, &winHeader);
  
  win_write_data(fd, win);

  fprintf(fd, "\n") ;

  fclose(fd) ;

  return(1) ;
}


/*!
    Write the window data to a file.

    \param fd File descriptor.
    \param win Window to write.
*/

void win_write_data(FILE *fd, window_t *win) {
/* author  Nina S. Tomita & Roberto Hirata Jr. (nina@ime.usp.br)     */
/* date: Thu Oct 17 1996                                             */

/*  Modification by:  Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)       */
/*  Date: Fri Oct 31 1997                                                   */
/*  Mod: The specification of the file format was changed to include tags   */
/*  (.h, .w, etc). The writing procedure was modified to write this new     */
/*  format. It was also included a new parameter wkrange                    */

/*  Modification by:  Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)       */
/*  Date: Wed Jun  3 1998                                                   */
/*  Mod: Modified to include the vertical placement                         */

/*  Modification by:  Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)       */
/*  Date: Fri Oct 29 1999                                                   */
/*  Mod: Modified to include support to different input/output ranges       */

/* Date: Wed Feb 16 2000                                                   */
/* Mod: Window structure (1) changed to support multiple bands, (2) no     */
/*      longer holds information about aperture. Modifications were made   */
/*      to adequate to the structure changing.                             */

  int      width, height, nbands ;
  int      i, j, k ;
   

  width = win_get_width(win) ;
  height = win_get_height(win) ;
  nbands = win_get_nbands(win) ;

#ifdef _DEBUG_1_
  pac_debug("%d %d %d\n", height, width, nbands) ;
#endif

  fprintf(fd, "%s %d\n", ".h", height) ;
  fprintf(fd, "%s %d\n", ".w", width) ;

  /* Default value for nbands is 1. Therefore it is written
     to the file only if it is not 1                        */
  if(nbands > 1) fprintf(fd, "%s %d\n", ".b", nbands) ;

  fprintf(fd, "%s\n", ".d") ;

  for(k=1; k <= nbands ; k++) {
    for (i=0; i < height; i++) {
      for(j=0; j < width; j++) {
	fprintf(fd, "%d ", win_get_point(i, j, k, win)) ;
      }
      fprintf(fd, "\n") ;
    }
    if(k < nbands) fprintf(fd, "\n") ;
  }

}
