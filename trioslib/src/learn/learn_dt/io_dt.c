#include <trios.h>

/*#define _DEBUG_*/
/* #define _PRINFO_ */

int dt_read ( char *fname, int *type, window_t **win, apert_t **apt);
int dt_write( char *fname, int  type, window_t  *win, apert_t  *apt);
 


extern int no_of_categories ;
extern int *map ;

/*
     -------------------------------------------
     FUNCTION: dt_read
     -------------------------------------------
*/

int                 /*+ Purpose: Reads a dt file                            +*/
  dt_read(
    char *fname,    /*+ In:  name of the file of the decision tree          +*/
    int      *type, /*+ Out: type of the operator                           +*/
    window_t **win, /*+ Out: pointer to window structure                    +*/
    apert_t  **apt  /*+ Out: pointer to aperture structure                  +*/
    )
/*+ Return: Pointer to the name of the dt file                              +*/
{
/*  author: Nina S. Tomita, R. Hirata Jr.   (nina@ime.usp.br)                */
/*  date: Fri Jan 16 1998                                                    */

  FILE     *fd ;
  header_t dtHeader= {"DTSPECS ", ""} ;
  header_t *aHeader ;

  char  tag, dot ;
  int   i, tags_read, stop ;

#ifdef _DEBUG_
trios_debug("Entrou no DT_READ.") ;
#endif

  tags_read = 0 ;
  stop = 0 ;

  /* open file -----------------------------------------------------------*/

  if((fd=fopen(fname, "r"))==NULL) {
    return (int)trios_error(1, "File (%s) open failed.", fname) ;
  }
 
  /* read & check file header ------------------------------------------- */

  aHeader = header_read(fd);
  if(!aHeader) {
    return (int)trios_error(1, "Error : unrecognized file header.") ;
  }

  if(!header_compare(aHeader, &dtHeader)) {
    fclose(fd) ;
    return (int)trios_error(1, "Error : wrong file header.") ;
  }

  while(!stop)  {
    
    while(((dot=(char)fgetc(fd)) != '.') && (dot!=(char)EOF)) ;

    if(dot==(char)EOF) {
      fclose(fd) ;
      return (int)trios_error(1, "Unexpected end of file. No tag found.") ;
    }

    tag = (char)fgetc(fd) ;

    switch (tag) {

    /* get type ------------------------------------- */
  
    case 't': 
      fscanf(fd, "%d", type);
      tags_read++ ;
      break ;

    /* read window information ---------------------------------------- */

    case 'W':
      if(NULL==(*win = win_read_data(fd))) {
	fclose(fd) ;
    return (int)trios_error(MSG, "dt_read: win_read_data() failed.") ;
      }
      tags_read++ ;
      break ;


    case 'A':
      /*if(NULL==(*apt = apert_read_data(fd))) {
	fclose(fd) ;
    return (int)trios_error(MSG, "dt_read: win_read_data() failed.") ;
      }*/
      break ;


    /* read frequency vector ------------------------------------ */

    case 'm':

      fscanf(fd, "%d", &no_of_categories) ;
      
      if (NULL == (map = (int *) malloc(sizeof(int)*(no_of_categories+1)))) 
    return trios_error(MSG,"dt_read: malloc failed for map") ;

      map[0] = 0 ;

      for(i=1; i <= no_of_categories ; i++)
	fscanf(fd, "%d", &map[i]);

      tags_read++ ;
      break ;

    case 'd' : 
      stop = 1 ;
      break ;
 
    default : 
      fclose(fd);   
      (void)trios_error(1," dt_read: Unexpected tag %c ",tag) ;
      return (int)trios_error(1, " File format error") ;
    }
  }

  if(tags_read!=3)
    return (int) trios_error(1,
      "dt_read: Missing Parameters. Look for .t, .n, .f or .W") ;

#ifdef _DEBUG_
trios_debug("Leu Data header") ;
#endif

  fclose(fd) ;

#ifdef _DEBUG_
trios_debug("Now it will get out") ;
#endif

  return(1) ;
}



/*
     -------------------------------------------
     FUNCTION: dt_write
     -------------------------------------------
*/

int                  /*+ Purpose: Writes the miniterms collection to a file  +*/
  dt_write(
    char *fname,      /*+ In:  name of the file of the decision tree          +*/
    int      type,    /*+ In: type of the operator                            +*/
    window_t *win,    /*+ In: pointer to window structure                     +*/ 
    apert_t  *apt     /*+ In: pointer to apert  structure                     +*/ 
)
/*+ Return: 1 on success, 0 on failure                                        +*/ 
{
/*  author: Nina S. Tomita, R. Hirata Jr.  (nina@ime.usp.br)                */
/*  date: Fri Jan 16 1998                                                   */

  header_t dtHeader = { "DTSPECS ", ""};
  FILE     *fd ;

  int i ;

#ifdef _DEBUG_
trios_debug("Entrou no DT_WRITE") ;
#endif

  fd = fopen(fname, "w") ;
  if(fd == NULL) 
    return trios_error(1, "write_dt : file open error.") ;
  
#ifdef _DEBUG_
trios_debug("File opened.") ;
#endif

  header_write(fd, &dtHeader);

#ifdef _DEBUG_
trios_debug("Header written.") ;
#endif

  fprintf(fd, "%s %d \n", ".t", type) ;



  fprintf(fd, "%s\n", ".W") ;

  win_write_data(fd, win);

#ifdef _DEBUG_
trios_debug("Window written.") ;
#endif

  if ((type > 3)&&(type < 10)) {

    fprintf(fd, "%s\n", ".A") ;
    
    /*apert_write_data(fd, apt);*/
    
  }

#ifdef _DEBUG_
trios_debug("Aperture written.") ;
#endif

  fprintf(fd, "%s %d\n", ".m", no_of_categories) ;

  for(i=1; i <= no_of_categories; i++) {
    fprintf(fd, "%d ", map[i]);
  }

  fprintf(fd," \n");

  fprintf(fd, "%s\n", ".d") ; 

  fclose(fd) ;
  return(1) ;

}






