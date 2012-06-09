#include <pacbasic.h>
#include <pacio.h>
#include "pacio_local.h"

/* #define _DEBUG_ */

/*  Modification by:  Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)       */
/*  Date: Tue Jan  6 1998                                                   */
/*  Mod: changed to support new tag .p. This is an optional tag that        */
/*       indicates that probabilities Px and p(y|x) are present in the file */

/*  Modification by:  Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)       */
/*  Date: Tue Jan 13 1998                                                   */
/*  Mod: Modified to include WK support                                     */

/*  Modification by:  Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)       */
/*  Date: Tue Mar 10 1998                                                   */
/*  Mod: Probabilities P(x) and p(1|x) changed from "float" to "double"     */

/* Modification by:  Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)       */
/* Date: Wed Feb 17 2000                                                   */
/* Mod: Window structure (1) changed to support multiple bands, (2) no     */
/*      longer holds information about aperture.                           */
/*      New structure "apert_t" was created.                               */
/*      Modifications were made to adequate to the structure changing.     */


/*!
    Reads a classified examples set from a file.

    \param fname File name.
    \param win Window used.
    \return A mtm_t structure or NULL on failure.
*/

mtm_t *mtm_read(char *fname, window_t **win, apert_t  **apt) {
/*  author: Nina S. Tomita, R. Hirata Jr.   (nina@ime.usp.br)                */
/*  date: Thu Oct 24 1996                                                    */

/*  Modification: by  Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)        */
/*  Date: Tue Nov  4 1997                                                    */
/*  Mod:  A new file format was introduced                                   */

/*  Modification by:  Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)        */
/*  Date: Fri Nov 14 1997                                                    */
/*  Mod: New routines to read gray-scale classified examples                 */

/*  Modification by:  Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)       */
/*  Date: Tue Jan  6 1998                                                   */
/*  Mod: changed to support new tag .p. This is an optional tag that        */
/*       indicates that probabilities Px and p(y|x) are present in the file */
/*       It is compatible only with type 0 (.t 0)                           */

/*  Modification by:  Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)       */
/*  Date: Thu Aug 19 1999                                                   */
/*  Mod: mtm_BX and mtm_GX structure changed. For mtm_BX, field Px and      */
/*       px were changed to fq and fq1, respectively. For mtm_GX, a new     */
/*       field fq was introduced. fq keeps the number of times a w-pattern  */
/*       has been observed during sampling. fq1 keeps how many times        */
/*       the label 1 was observed assigned to the w-pattern                 */

/*  Date: Thu Feb 17 2000                                                   */
/*  Mod: New parameter (aperture)                                           */

  FILE     *fd ;
  mtm_t    *mtm ;
  unsigned int *bwpat ;
  char     *wpat ;
  unsigned int nmtm, freq, fq, fq1  ;
  int      value, type, wsize, wzip, label, i, j;
  char     tag, dot ;
  int      tags_read, stop, comp_prob ;

#ifdef _DEBUG_
pac_debug("Entering MTM_READ()") ;
#endif

  *apt = NULL ;
  tags_read = 0 ;
  stop = 0 ;
  comp_prob = 0 ;

  /* open file -----------------------------------------------------------*/

  if((fd=fopen(fname, "r"))==NULL) {
    return (mtm_t *)trios_error(1, "File (%s) open failed.", fname) ;
  }
 
  /* read & check file header ------------------------------------------- */
  if(!header_match(fd, "MINTERM ")) {
    fclose(fd) ;
    return (mtm_t *)trios_error(1, "File header does not match.") ;
  }


  while(!stop)  {
    
    while(((dot=(char)fgetc(fd)) != '.') && (dot!=(char)EOF)) ;

    if(dot==(char)EOF) {
      fclose(fd) ;
      return (mtm_t *)trios_error(1, "Unexpected end of file. No tag found.") ;
    }

    tag = (char)fgetc(fd) ;

    switch (tag) {

    /* get type ------------------------------------- */
  
    case 't': 
      if(1 != fscanf(fd, "%d", &type)) {
        fclose(fd) ;
        trios_fatal("Unexpected data or end of file") ;
      }
      tags_read++ ;
      break ;

    /* get number of minterms                                           */

    case 'n':
      if(1 != fscanf(fd, "%d", &nmtm)) {
        fclose(fd) ;
        trios_fatal("Unexpected data or end of file") ;
      }
      tags_read++ ;
      break ;


    /* read window information ---------------------------------------- */

    case 'W':
      if(NULL==(*win = win_read_data(fd))) {
	fclose(fd) ;
	return (mtm_t *)trios_error(MSG, "mtm_read: win_read_data() failed.") ;
      }
      tags_read++ ;
      break ;

    /* read aperture information -------------------------------------- */
    case 'A': {
        return trios_error(1, "Aperture operators not supported yet.");
        /*if(NULL==(*apt = apert_read_data(fd))) {
        	fclose(fd) ;
	        return (mtm_t *)pac_error(MSG, "mtm_read: apert_read_data() failed.") ;
        }
        break ;*/
    }

    /* read frequency vector ------------------------------------ */

    case 'f':
      /* frequency of the labels */
      /* this is a redundant information in the file, but it will be
         kept to enable quick analysis of the data, concerning
         how many times each label occurs in the file.
         However, there is no need to build this list, since it is 
         built by the MTM_insertion routine below.
      */
 
      do{
	if(1 != fscanf(fd, "%d", &freq)) {
          fclose(fd) ;
          trios_fatal("Unexpected data or end of file") ;
        }
 
        if (freq) {
          if(1 != fscanf(fd, "%d", &label)) {
            fclose(fd) ;
            trios_fatal("Unexpected data or end of file") ;
          }
          /*                  See Remark Above
          if((freqnode=freq_node_create(label, freq))==NULL) {
            free(wpat) ;
            return (mtm_t *)pac_error(MSG,
                   "mtm_read: freq_node_create() failed.") ;
          }
	    
          if (!set_freq(freqnode, &freqlist)) {
            free(wpat) ;
            return (mtm_t *)pac_error(MSG, "mtm_read: xpl_set_freq() failed.") ;
          }
          */
        }
      }
      while(freq) ;

      tags_read++ ;
      break ;
      
    case 'd' : 
      stop = 1 ;
      break ;

    case 'p' :
      comp_prob = 1 ;
      break ;

    default : 
      fclose(fd);   
      (void)trios_error(1,"Unexpected tag %c ",tag) ;
      return (mtm_t *)trios_error(1, " File format error") ;
    }
  }


  if(tags_read!=4) {
    fclose(fd) ;
    return (mtm_t *)trios_error(1,
      "Missing Parameters. Looking for .t, .n, .f or .W") ;
  }

#ifdef _DEBUG_
pac_debug("Reading the header: done.") ;
#endif

  if((comp_prob) && (type != BB)) {
    fclose(fd) ;
    return (mtm_t *)trios_error(1,
      "Incompatible tags. Tag .p allowed only when type is BB (.t 0)") ;
  }


  /* read examples  ------------------------------------------ */

  switch(type) {
    case BB:
    case BG:

#ifdef _DEBUG_
pac_debug("Case BB or BG") ;
#endif
      wsize = win_get_wsize(*win);

      if(NULL==(mtm = mtm_create(wsize, type, nmtm))) {
	fclose(fd);
	return (mtm_t *)trios_error(MSG, "mtm_read: mtm_create() failed.") ;
      }

      mtm_set_comp_prob(mtm, comp_prob) ;

      wzip = size_of_zpat(wsize) ;
      
      if((bwpat = (int *)malloc(sizeof(int)*wzip)) == NULL) {
        fclose(fd) ;
        win_free(*win) ;
        mtm_free(mtm) ;
	return 
	  (mtm_t *)trios_error(1,"Memory allocation error.") ;
      }

      /* reading loop -----------------------------------------------------*/

      if(!comp_prob) {
        for(i= 0; i<nmtm ; i++) {
  
	  /* read minterms ------------------------------------------------ */

	  for(j=0; j < wzip; j++) {
	    if(1 != fscanf(fd, "%x ", &bwpat[j])) {
              free(bwpat) ;
              win_free(*win) ;
              mtm_free(mtm) ;
              fclose(fd) ;
              trios_fatal("Unexpected data or end of file") ;
            }
	  }

	  if(2 != fscanf(fd,"%d %d", &label, &fq) ) {
            free(bwpat) ;
            win_free(*win) ;
            mtm_free(mtm) ;
            fclose(fd) ;
            trios_fatal("Unexpected data or end of file") ;
          }

	  if(!mtm_BX_insert(mtm, i, wzip, bwpat, label, fq, 0)) {
            free(bwpat) ;
            win_free(*win) ;
            mtm_free(mtm) ;
            fclose(fd) ;
            return (mtm_t *)trios_error(MSG,"mtm_read: mtm_BX_insert() failed.") ;
          }
        }
      }
      else {
        for(i= 0; i<nmtm ; i++) {
  
	  /* read minterms ------------------------------------------------ */

	  for(j=0; j < wzip; j++) {
	    if(1 != fscanf(fd, "%x ", &bwpat[j])) {
              free(bwpat) ;
              win_free(*win) ;
              mtm_free(mtm) ;
              fclose(fd) ;
              trios_fatal("Unexpected data or end of file") ;
            }
	  }

	  if(3 != fscanf(fd,"%d %d %d", &label, &fq, &fq1)) {
            free(bwpat) ;
            win_free(*win) ;
            mtm_free(mtm) ;
            fclose(fd) ;
            trios_fatal("Unexpected data or end of file") ;
          }

	  if(!mtm_BX_insert(mtm, i, wzip, bwpat, label, fq, fq1)) {
            free(bwpat) ;
            win_free(*win) ;
            mtm_free(mtm) ;
            fclose(fd) ;
            return (mtm_t *)trios_error(MSG, "mtm_read: mtm_BX_insert() failed.") ;
          }
        }
      }
      free(bwpat) ;
      break ;

    case GG:
    case WKC:
    case WKF:
    case WK3F:
    case WK3C:
    case WKGG2F:
    case WKGG2C:
    case GG3:

#ifdef _DEBUG_
pac_debug("Case GG") ;
#endif

      wsize = win_get_wsize(*win);

      if(NULL==(mtm = mtm_create(wsize, type, nmtm))) {
	fclose(fd);
	return (mtm_t *)trios_error(MSG, "mtm_read: mtm_create() failed.") ;
      }

      mtm_set_comp_prob(mtm, comp_prob) ;

      if((wpat = (char *) malloc(sizeof(char)*wsize)) == NULL) {
        fclose(fd) ;
        win_free(*win) ;
        mtm_free(mtm) ;
	return 
	  (mtm_t *)trios_error(1, "Memory allocation error.") ;
      }

      /* reading loop --------------------------------- */

#ifdef _DEBUG_
pac_debug("Now it will read the minterms") ;
#endif

      for(i= 0; i<nmtm ; i++) {
  
	/* read minterms --------------------------------------------------*/

	for(j=0; j < wsize; j++) {
	  if(1 != fscanf(fd, "%d ", &value)) {
            free(wpat) ;
            win_free(*win) ;
            mtm_free(mtm) ;
            fclose(fd) ;
            trios_fatal("Unexpected data or end of file") ;
          }
	  wpat[j] = (char) value ;
	}

	if(2 != fscanf(fd,"%d %d", &label, &fq)) {
          free(wpat) ;
          win_free(*win) ;
          mtm_free(mtm) ;
          fclose(fd) ;
          trios_fatal("Unexpected data or end of file") ;
        }
 
	if(!mtm_GX_insert(mtm, i, wsize, wpat, label, fq)) {
          free(wpat) ;
          win_free(*win) ;
          mtm_free(mtm) ;
          fclose(fd) ;
          return (mtm_t *)trios_error(MSG, "mtm_read: mtm_GX_insert() failed.") ;
        }
      }
      free(wpat) ;
      break ;
      
    default:
      win_free(*win) ;
      fclose(fd) ;
      return (mtm_t *)trios_error(1, "Invalid mapping type.") ;

  }

  fclose(fd) ;

#ifdef _DEBUG_
pac_debug("Now it will get out") ;
#endif

  return(mtm) ;
}



/*!
    Writes a classified examples set to a file.

    \param fname File name.
    \param mtm Classified examples structure.
    \param win Window used.
    \return 1 on success. 0 on failure.
*/

int mtm_write(char *fname, mtm_t *mtm, window_t *win, apert_t *apt) {
/*  author: Nina S. Tomita, R. Hirata Jr.  (nina@ime.usp.br)                */
/*  date: Thu Oct 24 1996                                                   */

/*  Modification: by  Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)       */
/*  Date: Tue Nov  4 1997                                                   */
/*  Mod: A new file format write was implemented                            */

/*  Modification by:  Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)       */
/*  Date: Fri Nov 14 1997                                                   */
/*  Mod: New routines for writing classified examples                       */

/*  Modification by:  Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)       */
/*  Date: Tue Jan  6 1998                                                   */
/*  Mod: changed to support new tag .p. This is an optional tag that        */
/*       indicates that probabilities Px and p(y|x) are present in the file */
/*       It is compatible only with type 0 (.t 0)                           */

/*  Modification by:  Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)       */
/*  Date: Tue Aug 10 1999                                                   */
/*  Mod: Inserted code to treat the list of frequencies                     */

/*  Modification by:  Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)       */
/*  Date: Thu Aug 19 1999                                                   */
/*  Mod: mtm_BX and mtm_GX structure changed. For mtm_BX, field Px and      */
/*       px were changed to fq and fq1, respectively. For mtm_GX, a new     */
/*       field fq was introduced. fq keeps the number of times a w-pattern  */
/*       has been observed during sampling. fq1 keeps how many times        */
/*       the label 1 was observed assigned to thw w-pattern                 */

/*  Date: Thu Feb 17 2000                                                   */
/*  Mod: New parameter (aperture)                                           */


  header_t mtmHeader = { "MINTERM ", ""};
  FILE     *fd ;
  mtm_BX   *table_BX ;
  mtm_GX   *table_GX ; 
  freq_node *freqlist ;
  unsigned int nmtm ;
  int      type, wzip, wsize, wsize1, freqsize ;
  int      i, j, comp_prob ;
  unsigned char value ;


#ifdef _DEBUG_
pac_debug("Entrou no MTM_WRITE") ;
#endif

  freqsize = 0 ;

  fd = fopen(fname, "w") ;
  if(fd == NULL) 
    return trios_error(1, "File (%s) open failed.", fname) ;
  
#ifdef _DEBUG_
pac_debug("abriu arquivo\n") ;
#endif

  header_write(fd, &mtmHeader);
#ifdef _DEBUG_
pac_debug("escreveu header no arquivo") ;
#endif

  type = mtm_get_type(mtm) ;

#ifdef _DEBUG_
pac_debug("type=%d", type) ;
#endif

  fprintf(fd, "%s %d \n", ".t", type) ;

  nmtm = mtm_get_nmtm(mtm) ;
  
  fprintf(fd, "%s %d \n", ".n", nmtm) ;

  fprintf(fd, "%s\n", ".W") ;

  win_write_data(fd, win);

  if((type > 3) && (type < 10)) {
    return trios_error(1, "Aperture operators not supported yet.");
    /*fprintf(fd, "%s\n", ".A") ;
    apert_write_data(fd, apt);*/
  }
 

  fprintf(fd, "%s\n", ".f") ;
    
  freqlist = (freq_node *) mtm->mtm_freq ;
 
  while(freqlist) {
    fprintf(fd, "%d %d ", freqlist->freq, freqlist->label) ;
    
    freqlist = freqlist->next ;    
  }
  
  fprintf(fd, "%d\n",0) ;


  /* up to now, probabilities will be printed only if type==BB */
  comp_prob = mtm_get_comp_prob(mtm) ;
  if((comp_prob) && (type==BB)) {
    fprintf(fd, "%s\n", ".p") ;
  }
  else{
    comp_prob = 0 ;
  }

  fprintf(fd, "%s\n", ".d") ;

#ifdef _DEBUG_
pac_debug("Header data writing: done") ;
#endif

  wsize = mtm_get_wsize(mtm) ;

  wzip = size_of_zpat(wsize) ;


  switch(type) {
  
    case BB:
    case BG:

      table_BX = (mtm_BX *)mtm->mtm_data ; 

      if(!comp_prob) {
        for (i = 0; i < nmtm; i++) {
	  for ( j = 0; j < wzip; j++) 
	    fprintf(fd, "%x ", table_BX[i].wpat[j]); 
	  fprintf(fd, "%d %d\n", table_BX[i].label, table_BX[i].fq);
        }
      }
      else {
        for (i = 0; i < nmtm; i++) {
	  for ( j = 0; j < wzip; j++) 
	    fprintf(fd, "%x ", table_BX[i].wpat[j]); 
 	  fprintf(fd, "%d %d %d\n",
            table_BX[i].label, table_BX[i].fq, table_BX[i].fq1);
        }
      }
      break ;

    case GG:
    case GG3:

      table_GX = (mtm_GX *)mtm->mtm_data ; 

      for (i = 0; i < nmtm; i++) {
	for (j = 0; j < wsize; j++) {
          value = (unsigned char)table_GX[i].wpat[j] ;
          fprintf(fd, "%d ", value);
        } 
	fprintf(fd, "%d %d\n", table_GX[i].label, table_GX[i].fq);
      }
      break ;

    case WKC:
    case WKF:
    case WK3F:
    case WK3C:

      table_GX = (mtm_GX *)mtm->mtm_data ; 

      for (i = 0; i < nmtm; i++) {
	for (j = 0; j < wsize; j++) {
          fprintf(fd, "%d ", table_GX[i].wpat[j]);
        } 
	fprintf(fd, "%d %d\n", table_GX[i].label, table_GX[i].fq);
      }
      break ;

    case WKGG2F:
    case WKGG2C:

      table_GX = (mtm_GX *)mtm->mtm_data ;

      wsize1 = win_get_band_wsize(win, 1) ;

      for (i = 0; i < nmtm; i++) {
        /* print band R */
	for (j = 0; j < wsize1; j++) {
          fprintf(fd, "%d ", table_GX[i].wpat[j]);
        } 

        /* print band G */
        for(j=wsize1; j<wsize; j++) {
          value = (unsigned char)table_GX[i].wpat[j] ;
          fprintf(fd, "%d ", value);
        }

        /* print label info */
	fprintf(fd, "%d %d\n", table_GX[i].label, table_GX[i].fq);
      }
      break ;

    default:
      fclose(fd) ;
      return trios_error(1, "Invalid mapping type.");
      break;
  }


  fclose(fd) ;
  return(1) ;
}




