#include <pacbasic.h>
#include <pacio.h>
#include "pacio_local.h"

/* #define _DEBUG_ */
/* #define _DEBUG_1_ */
/* #define _DEBUG_2_ */


/*
     -------------------------------------------
     FUNCTION: lshowxpl
     -------------------------------------------
*/

int                 /*+ Purpose: show the elements of an example set +*/  
  lshowxpl(
    char *fname_i,    /*+ In: XPL file name                           +*/
    int  select_type, /*+ In: type of examples to be showed
                              1 : All
                              2 : only conflicting ones
                              3 : only non-conflicting ones           +*/
    int  from,        /*+ In: range start                             +*/
    int  to,          /*+ In: range end                               +*/
    int  sort_type,   /*+ In: key for sorting :
                              1 : regular (by w-pattern)
                              2 : by frequency                        +*/
    int  order_type,  /*+ In: ordering type :
                              1 : ascendent
                              2 : descendent                          +*/
    char *fname_o     /*+ In: output XPL show file                    +*/
  )
/*+ Return: 1 on success, 0 on failure                                +*/
{
/* author: Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)             */
/* date: Tue Nov 12 1996                                               */

/*  Modification by:  Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)       */
/*  Date: Tue Aug 24 1999                                                   */
/*  Mod: modifications for implementation of BG and WK types                */

  xpl_t    *xpl ;
  window_t *win ;
  apert_t  *apt=NULL ;
  int      n_nodes ;
  int      map_type ;
  FILE     *fd ;
  


  /* read XPL file */
  if(!(xpl=xpl_read(fname_i, &win, &apt))) {
    return pac_error(MSG, "lshowxpl: xpl_read() failed.") ;
  }

#ifdef _DEBUG_
pac_debug("Examples file reading: done.") ;
#endif

  n_nodes = xpl_get_n_nodes(xpl) ;
  map_type = xpl_get_type(xpl) ;


  /* Adjust the search range */
  if(to > n_nodes) to = n_nodes ;
  if(from > to) {
    from = 1 ;
    to = n_nodes ;
  }


  /* Sort examples by frequency, when neccessary */
  if(sort_type==2) {
    if(!xpl_sort(xpl)) {
      xpl_free(xpl) ;
      win_free(win) ;
      return pac_error(MSG, "lshowxpl : xpl_sort() failed.") ;
    }
#ifdef _DEBUG_
pac_debug("Sorting : done") ;
#endif
  }


  /* convert tree structure to linked list structure */
  /* if order_type==1 then use ascending order, else use descending order */
  if(!xpl_tree2ll(xpl, order_type)) {
    xpl_free(xpl) ;
    win_free(win) ;
    return pac_error(MSG, "lshowxpl : xpl_tree2ll() failed.") ;
  }
#ifdef _DEBUG_
pac_debug("Converted to LL") ;
#endif


  /* open output file */
  fd = fopen(fname_o, "w") ;
  if(fd == NULL) {
    return pac_error(1, "File (%s) open failed.", fname_o) ;
  }

  /* write some basic informations about the examples set */
  if(!xpl_show_basicinfo(
       fd, xpl, select_type, sort_type, order_type, from, to)) {
    fclose(fd) ;
    return pac_error(MSG, "lshowxpl: xpl_show_basicinfo() failed.") ;
  }


  switch(map_type) {
  case BB :
    if(!xpl_show_BB(fd, xpl, win, select_type, from, to)) {
      return pac_error(MSG, "lshowxpl: xpl_show_BB() failed.") ;
    }
    break ;
  
  case BG :
    pac_error(MSG, "This operator is not supported yet");
    /*if(!xpl_show_BG(fd, xpl, win, select_type, from, to)) {
      return pac_error(MSG, "lshowxpl: xpl_show_BG() failed.") ;
    }*/
    break ;
  
  case GG :
  case WKF:
  case WKC:
    pac_error(MSG, "This operator is not supported yet");
    /*if(!xpl_show_WK(fd, xpl, win, select_type, from, to)) {
      return pac_error(MSG, "lshowxpl: xpl_show_WK() failed.") ;
    }*/
    break ;

  case WK3F:
  case WKGG2F:
    pac_error(MSG, "This operator is not supported yet");
    /*if(!xpl_show_3Bto3B(fd, xpl, win, apt, select_type, from, to)) {
      return pac_error(MSG, "lshowxpl: xpl_show_BG() failed.") ;
    }*/
    break ;

  case WKGG2C:
  case WK3C:
  case GG3:
    pac_error(MSG, "This operator is not supported yet");
    /*if(!xpl_show_3Bto1B(fd, xpl, win, apt, select_type, from, to)) {
      return pac_error(MSG, "lshowxpl: xpl_show_BG() failed.") ;
    }*/
    break ;
 
  case GB :
  default:
    return pac_error(1, "lshowxpl: Type %d is not implemented yet.", map_type) ;

  }

  fclose(fd) ;
  xpl_free(xpl) ;
  win_free(win) ;
  //apert_free(apt) ;

  return(1) ;

}



/*
     -------------------------------------------
     FUNCTION: xpl_show_basicinfo
     -------------------------------------------
*/

int           /*+ Purpose: write some basic information concerning
                             to a XPL set                                +*/
  xpl_show_basicinfo(
    FILE  *fd,
    xpl_t *xpl,        /*+ In: XPL set                                   +*/
    int   select_type, /*+ In: type of examples to be showed
                              1 : All
                              2 : only conflicting ones
                              3 : only non-conflicting ones              +*/
    int   sort_type,   /*+ In: ordering type :
                              1 : regular (by w-pattern)
                              2 : by frequency                           +*/
    int   order_type,  /*+ In: ordering type :
                              1 : ascendent
                              2 : descendent                             +*/
    int   from,        /*+ In: range start                               +*/
    int   to           /*+ In: range end                                 +*/
)
/*+ Return: 1 on success, 0 on failure                                   +*/
{
/* author: Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)                */
/* date: Tue Nov 12 1996                                                  */


  int n_nodes, ncflt ;


  /* get number of conflicting examples */
  if((ncflt = xpl_conflicts(xpl))==-1) {
    return pac_error(MSG, "lshowxpl : xpl_conflicts() failed.") ;
  }

  n_nodes = xpl_get_n_nodes(xpl) ;

  /* write the file name, number of distinct examples, total number of examples
     and the number of conflicting examples  */
  fprintf(fd, "*Number of DISTINCT examples : %d\n", n_nodes) ;
  fprintf(fd, "*TOTAL number of examples : %d\n", xpl_get_sum(xpl)) ;
  fprintf(fd, "*Number of CONFLICTING examples : %d\n\n", ncflt) ;

  /* write the type of examples that will be showed */
  fprintf(fd, "*Examples showed below :\n") ;
  switch(select_type) {
    case 1 : {
      fprintf(fd, "   [X] All\n") ;
      fprintf(fd, "   [ ] only CONFLICTING ones\n") ;
      fprintf(fd, "   [ ] only NON-CONFLICTING ones\n") ;
    }
    break ;
    case 2 : {
      fprintf(fd, "   [ ] All\n") ;
      fprintf(fd, "   [X] only CONFLICTING ones\n") ;
      fprintf(fd, "   [ ] only NON-CONFLICTING ones\n") ;
    }
    break ;
    case 3 : {
      fprintf(fd, "   [ ] All\n") ;
      fprintf(fd, "   [ ] only CONFLICTING ones\n") ;
      fprintf(fd, "   [X] only NON-CONFLICTING ones\n") ;
    }
    break ;
  }

  /* write the range especification */
  switch(sort_type) {
    case 1 :
      fprintf(fd,
    "  (The RANGE is from %d to %d, with REGULAR ordering)\n", from, to) ;
      break ;
    case 2 :
      fprintf(fd,
    "  (The RANGE is from %d to %d, with ordering BY FREQUENCY)\n", from, to) ;
      break ;
  }

  return(1) ;

}


/*
     -------------------------------------------
     FUNCTION: xpl_show_BB
     -------------------------------------------
*/

int           /*+ Purpose: write the examples from a BB XPL set to a file +*/
  xpl_show_BB(
    FILE  *fd,         /*+ In: output file descriptor                   +*/ 
    xpl_t *xpl,        /*+ In: XPL set                                  +*/
    window_t *win,     /*+ In: Window                                   +*/
    int   select_type, /*+ In: type of examples to be showed
                              1 : All
                              2 : only conflicting ones
                              3 : only non-conflicting ones             +*/
    int   from,        /*+ In: range start                              +*/
    int   to           /*+ In: range end                                +*/
  )
/*+ Return: 1 on success, 0 on failure                                  +*/
{
/* author: Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)               */
/* date: Tue Nov 12 1996                                                 */

  xpl_BB   *head ;
  int      counter = 1 ;
  int      row_length, i ;
  show_t   *show ;


#ifdef _DEBUG_
pac_debug("Inside xpl_show_BB") ;
#endif


  /* create show structure */ 
  show = show_create(win, BB, 1) ;
  if(!show) {
    return pac_error(MSG, "lshowxpl: show_create() failed") ;
  }


#ifdef _DEBUG_
pac_debug("created show structure") ;
#endif


  fprintf(fd, "\n*Example's TYPE : Binary to Binary\n\n") ;


  row_length = show->row_length ;
  fprintf(fd, " [order]  [w-pattern]   [0-frequency] [1-frequency]\n\n") ;


  /* skip first "from-1" examples */
  head = (xpl_BB *)xpl->root ;
  while(counter < from) {
    head=head->right ;
    counter++ ;
  }


  switch(select_type) {
    case 1 :   /* All */
    {
      while(counter <= to) {

        wpat2vecB(head->wpat, show) ;

        for(i=0; i<=show->up_half; i++) {
          fprintf(fd, "         %s\n", &show->wpat_vec[row_length*i]) ;
	}

        fprintf(fd, " %5d)  %s    %d  %d\n", counter,
          &show->wpat_vec[row_length*show->mid_row], head->fq0, head->fq1) ;

        for(i=show->mid_row+1; i<=show->up; i++) {
          fprintf(fd, "         %s\n", &show->wpat_vec[row_length*i]) ;
	}
        fprintf(fd, "\n") ;
       
        head = head->right ;

        counter++ ;
      }
    }
    break ;

    case 2 :   /* only conflicting examples */
    {
      while(counter <= to) {
        wpat2vecB(head->wpat, show) ;
        if((head->fq1 != 0) && (head->fq0 != 0)) {
	  for(i=0; i<=show->up_half; i++) {
	    fprintf(fd, "         %s\n", &show->wpat_vec[row_length*i]) ;
	  }
	  fprintf(fd, " %5d)  %s    %d  %d\n", counter,
	    &show->wpat_vec[row_length*i], head->fq0, head->fq1) ;
	  for(i=show->mid_row+1; i<=show->up; i++) {
	    fprintf(fd, "         %s\n", &show->wpat_vec[row_length*i]) ;
	  }
	  fprintf(fd, "\n") ;    
        }       
        head = head->right ;
        counter++ ;
      }
    }
    break ;

    case 3 :   /* only non-conflicting examples */
    {
      while(counter <= to) {
        wpat2vecB(head->wpat, show) ;
        if((head->fq1 == 0) || (head->fq0 == 0)) {
	  for(i=0; i<=show->up_half; i++) {
	    fprintf(fd, "         %s\n", &show->wpat_vec[row_length*i]) ;
	  }
	  fprintf(fd, " %5d)  %s    %d  %d\n", counter,
	    &show->wpat_vec[row_length*i], head->fq0, head->fq1) ;
	  for(i=show->mid_row+1; i<=show->up; i++) {
	    fprintf(fd, "         %s\n", &show->wpat_vec[row_length*i]) ;
	  }
	  fprintf(fd, "\n") ;
        }       
        head = head->right ;
        counter++ ;
      }
    }
  }

  return(1) ;
}



/*
     -------------------------------------------
     FUNCTION: xpl_show_BG
     -------------------------------------------
*/

int           /*+ Purpose: write the examples from a BB XPL set to a file +*/
  xpl_show_BG(
    FILE  *fd,         /*+ In: output file descriptor                   +*/ 
    xpl_t *xpl,        /*+ In: XPL set                                  +*/
    window_t *win,     /*+ In: Window                                   +*/
    int   select_type, /*+ In: type of examples to be showed
                              1 : All
                              2 : only conflicting ones
                              3 : only non-conflicting ones             +*/
    int   from,        /*+ In: range start                              +*/
    int   to           /*+ In: range end                                +*/
  )
/*+ Return: 1 on success, 0 on failure                                  +*/
{
/* author: Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)               */
/* date: Tue Aug 24 1999                                                 */

  xpl_BG    *head ;
  int       counter = 1 ;
  int       row_length, i ;
  freq_node *p ;
  int       npat ;
  show_t    *show ;


#ifdef _DEBUG_
pac_debug("Inside xpl_show_BG") ;
#endif

  /* create show structure */ 
  show = show_create(win, BB, 1) ;
  if(!show) {
    return pac_error(MSG, "lshowxpl: show_create() failed") ;
  }

  fprintf(fd, "\n*Example's TYPE : Binary to Gray level\n\n") ;

  row_length = show->row_length ;
  fprintf(fd, " [order]  [label]  [freq.]  [w-pattern]\n\n") ;

  /* skip first "from-1" examples */
  head = (xpl_BG *)xpl->root ;
  while(counter < from) {
    head=head->right ;
    counter++ ;
  }


  switch(select_type) {
    case 1 :   /* All */
    {
      while(counter <= to) {

        p = head->freqlist ;
        wpat2vecB(head->wpat, show) ;
        npat = 0 ;

        /* print w-pattern */
        for(i=0; i<=show->up_half; i++) {
          if(p) {
            fprintf(fd, "%11d  %6d       %s\n", p->label, p->freq,
            &show->wpat_vec[row_length*i]) ;
            npat = npat + p->freq ; 
            p = p->next ;
          }
          else {
            fprintf(fd, "%11s  %6s       %s\n", "---", "---",
            &show->wpat_vec[row_length*i]) ;
          }
	}

        if(p) {
          fprintf(fd, "%5d)  %3d  %6d       %s\n", counter,
          p->label, p->freq, &show->wpat_vec[row_length*show->mid_row]) ;
          npat = npat + p->freq ; 
          p = p->next ;
        }
        else {
          fprintf(fd, "%5d)  %3s  %6s       %s\n", counter, "---", "---",
          &show->wpat_vec[row_length*show->mid_row]) ;
        } 

        for(i=show->mid_row+1; i<=show->up; i++) {
          if(p) {
            fprintf(fd, "%11d  %6d       %s\n", p->label, p->freq,
            &show->wpat_vec[row_length*i]) ;
            npat = npat + p->freq ; 
            p = p->next ;
          }
          else {
            fprintf(fd, "%11s  %6s       %s\n", "---", "---",
            &show->wpat_vec[row_length*i]) ;
          }
	}
        while(p) {
          fprintf(fd, "%11d  %6d\n", p->label, p->freq) ;
          npat = npat + p->freq ; 
          p = p->next ;
        }

        fprintf(fd, "%11s  %6d\n\n\n", "", npat) ;
       
        head = head->right ;
        counter++ ;
      }
    }
    break ;

    case 2 :   /* only conflicting examples */
    {
      while(counter <= to) {
        p = head->freqlist ;
        if(p->next) {
          wpat2vecB(head->wpat, show) ;
          npat = 0 ;

          for(i=0; i<=show->up_half; i++) {
            if(p) {
              fprintf(fd, "%11d  %6d       %s\n", p->label, p->freq,
              &show->wpat_vec[row_length*i]) ;
              npat = npat + p->freq ; 
              p = p->next ;
            }
            else {
              fprintf(fd, "%25s %s\n", " ", &show->wpat_vec[row_length*i]) ;
            } 
	  }

          if(p) {
            fprintf(fd, "%5d)  %3d  %6d       %s\n", counter,
            p->label, p->freq, &show->wpat_vec[row_length*show->mid_row]) ;
            npat = npat + p->freq ; 
            p = p->next ;
          }
          else {
            fprintf(fd, "%5d)  %12s %s\n",
            counter, &show->wpat_vec[row_length*show->mid_row]) ;
          } 
          for(i=show->mid_row+1; i<=show->up; i++) {
            if(p) {
              fprintf(fd, "%11d  %6d       %s\n", p->label, p->freq,
              &show->wpat_vec[row_length*i]) ;
              npat = npat + p->freq ; 
              p = p->next ;
            }
            else {
              fprintf(fd, "%25s %s\n", " ", &show->wpat_vec[row_length*i]) ;
            } 
	  }
          while(p) {
            fprintf(fd, "%11d  %6d\n", p->label, p->freq) ;
            npat = npat + p->freq ; 
            p = p->next ;
          }
          fprintf(fd, "%11s  %6d\n\n\n", "", npat) ;
        }     
        head = head->right ;
        counter++ ;
      }
    }
    break ;

    case 3 :   /* only non-conflicting examples */
    {
      while(counter <= to) {
        p = head->freqlist ;
        if(!(p->next)) {
          wpat2vecB(head->wpat, show) ;
          npat = 0 ;

          for(i=0; i<=show->up_half; i++) {
            if(p) {
              fprintf(fd, "%11d  %6d       %s\n", p->label, p->freq,
              &show->wpat_vec[row_length*i]) ;
              npat = npat + p->freq ; 
              p = p->next ;
            }
            else {
              fprintf(fd, "%25s %s\n", " ", &show->wpat_vec[row_length*i]) ;
            } 
	  }

          if(p) {
            fprintf(fd, "%5d)  %3d  %6d       %s\n", counter,
            p->label, p->freq, &show->wpat_vec[row_length*show->mid_row]) ;
            npat = npat + p->freq ; 
            p = p->next ;
          }
          else {
            fprintf(fd, "%5d)  %12s %s\n",
            counter, &show->wpat_vec[row_length*show->mid_row]) ;
          } 
          for(i=show->mid_row+1; i<=show->up; i++) {
            if(p) {
              fprintf(fd, "%11d  %6d       %s\n", p->label, p->freq,
              &show->wpat_vec[row_length*i]) ;
              npat = npat + p->freq ; 
              p = p->next ;
            }
            else {
              fprintf(fd, "%25s %s\n", " ", &show->wpat_vec[row_length*i]) ;
            } 
	  }
          while(p) {
            fprintf(fd, "%11d  %6d\n", p->label, p->freq) ;
            npat = npat + p->freq ; 
            p = p->next ;
          }

          fprintf(fd, "%11s  %6d\n\n\n", "", npat) ;
        }     

        head = head->right ;
        counter++ ;
      }
    }
  }

  return(1) ;
}



/*
     -------------------------------------------
     FUNCTION: xpl_show_WK
     -------------------------------------------
*/

int           /*+ Purpose: write the examples from a BB XPL set to a file +*/
  xpl_show_WK(
    FILE  *fd,         /*+ In: output file descriptor                   +*/ 
    xpl_t *xpl,        /*+ In: XPL set                                  +*/
    window_t *win,     /*+ In: Window                                   +*/
    int   select_type, /*+ In: type of examples to be showed
                              1 : All
                              2 : only conflicting ones
                              3 : only non-conflicting ones             +*/
    int   from,        /*+ In: range start                              +*/
    int   to           /*+ In: range end                                +*/
  )
/*+ Return: 1 on success, 0 on failure                                  +*/
{
/* author: Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)               */
/* date: Tue Aug 24 1999                                                 */

  xpl_GG    *head ;
  int       counter = 1 ;
  int       row_length, i ;
  freq_node *p ;
  int       npat ;
  int       map_type ;
  show_t    *show ;


#ifdef _DEBUG_
  pac_debug("Inside xpl_show_WK") ;
#endif


  map_type = xpl_get_type(xpl) ;
  if(map_type==GG) {
    fprintf(fd, "\n*Example's TYPE : Gray to Gray\n\n") ;
  } 
  else if(map_type==WKF) {
    fprintf(fd, "\n*Example's TYPE : Aperture Filter\n\n") ;
  } 
  else if(map_type==WKC) {
    fprintf(fd, "\n*Example's TYPE : Aperture Classificator\n\n") ;
  } 


  /* create show structure */ 
  show = show_create(win, map_type, 1) ;
  if(!show) {
    return pac_error(MSG, "lshowxpl: show_create() failed") ;
  }


  row_length = show->row_length ;
  fprintf(fd, " [order]  [label]  [freq.]  [w-pattern]\n\n") ;

  /* skip first "from-1" examples */
  head = (xpl_GG *)xpl->root ;
  while(counter < from) {
    head=head->right ;
    counter++ ;
  }


  switch(select_type) {
    case 1 :   /* All */
    {
      while(counter <= to) {

        p = head->freqlist ;
        if(map_type==GG) wpat2vecUG(head->wpat, show) ;
        else wpat2vecG(head->wpat, show) ;
        npat = 0 ;

        for(i=0; i<=show->up_half; i++) {
          if(p) {
            fprintf(fd, "%11d  %6d       %s\n", p->label, p->freq,
            &show->wpat_vec[row_length*i]) ;
            npat = npat + p->freq ;
            p = p->next ;
          }
          else {
            fprintf(fd, "%11s  %6s       %s\n", "---", "---",
            &show->wpat_vec[row_length*i]) ;
          }   
	}

        if(p) {
          fprintf(fd, "%5d)  %3d  %6d       %s\n", counter,
          p->label, p->freq, &show->wpat_vec[row_length*show->mid_row]) ;
          npat = npat + p->freq ;
          p = p->next ;
        }
        else {
          fprintf(fd, "%5d)  %3s  %6s       %s\n", counter, "---", "---",
          &show->wpat_vec[row_length*show->mid_row]) ;
        } 

        for(i=show->mid_row+1; i<=show->up; i++) {
          if(p) {
            fprintf(fd, "%11d  %6d       %s\n", p->label, p->freq,
            &show->wpat_vec[row_length*i]) ;
            npat = npat + p->freq ;
            p = p->next ;
          }
          else {
            fprintf(fd, "%11s  %6s       %s\n", "---", "---",
            &show->wpat_vec[row_length*i]) ;
          }   
	}
        while(p) {
          fprintf(fd, "%11d  %6d\n", p->label, p->freq) ;
          npat = npat + p->freq ;
          p = p->next ;
        }
        fprintf(fd, "%11s  %6d\n\n\n", "", npat) ;
       
        head = head->right ;

        counter++ ;
      }
    }
    break ;

    case 2 :   /* only conflicting examples */
    {
      while(counter <= to) {
        p = head->freqlist ;
        if(p->next) {
          wpat2vecG(head->wpat, show) ;
          npat = 0 ;

          for(i=0; i<=show->up_half; i++) {
            if(p) {
              fprintf(fd, "%11d  %6d       %s\n", p->label, p->freq,
              &show->wpat_vec[row_length*i]) ;
              npat = npat + p->freq ;
              p = p->next ;
            }
            else {
              fprintf(fd, "%11s  %6s       %s\n", "---", "---",
              &show->wpat_vec[row_length*i]) ;
            }   
	  }

          if(p) {
            fprintf(fd, "%5d)  %3d  %6d       %s\n", counter,
            p->label, p->freq, &show->wpat_vec[row_length*show->mid_row]) ;
            npat = npat + p->freq ;
            p = p->next ;
          }
          else {
            fprintf(fd, "%5d)  %3s  %6s       %s\n", counter, "---", "---",
            &show->wpat_vec[row_length*show->mid_row]) ;
          } 

          for(i=show->mid_row+1; i<=show->up; i++) {
            if(p) {
              fprintf(fd, "%11d  %6d       %s\n", p->label, p->freq,
              &show->wpat_vec[row_length*i]) ;
              npat = npat + p->freq ;
              p = p->next ;
            }
            else {
              fprintf(fd, "%11s  %6s       %s\n", "---", "---",
              &show->wpat_vec[row_length*i]) ;
            }   
	  }
          while(p) {
            fprintf(fd, "%11d  %6d\n", p->label, p->freq) ;
            npat = npat + p->freq ;
            p = p->next ;
          }
          fprintf(fd, "%11s  %6d\n\n\n", "", npat) ;
        }
        head = head->right ;
        counter++ ;
      }
    }
    break ;

    case 3 :   /* only non-conflicting examples */
    {
      while(counter <= to) {
        p = head->freqlist ;
        if(!(p->next)) {
          wpat2vecG(head->wpat, show) ;
          npat = 0 ;

          for(i=0; i<=show->up_half; i++) {
            if(p) {
              fprintf(fd, "%11d  %6d       %s\n", p->label, p->freq,
              &show->wpat_vec[row_length*i]) ;
              npat = npat + p->freq ;
              p = p->next ;
            }
            else {
              fprintf(fd, "%11s  %6s       %s\n", "---", "---",
              &show->wpat_vec[row_length*i]) ;
            }   
	  }

          if(p) {
            fprintf(fd, "%5d)  %3d  %6d       %s\n", counter,
            p->label, p->freq, &show->wpat_vec[row_length*show->mid_row]) ;
            npat = npat + p->freq ;
            p = p->next ;
          }
          else {
            fprintf(fd, "%5d)  %3s  %6s       %s\n", counter, "---", "---",
            &show->wpat_vec[row_length*show->mid_row]) ;
          } 

          for(i=show->mid_row+1; i<=show->up; i++) {
            if(p) {
              fprintf(fd, "%11d  %6d       %s\n", p->label, p->freq,
              &show->wpat_vec[row_length*i]) ;
              npat = npat + p->freq ;
              p = p->next ;
            }
            else {
              fprintf(fd, "%11s  %6s       %s\n", "---", "---",
              &show->wpat_vec[row_length*i]) ;
            }   
	  }
          while(p) {
            fprintf(fd, "%11d  %6d\n", p->label, p->freq) ;
            npat = npat + p->freq ;
            p = p->next ;
          }
          fprintf(fd, "%11s  %6d\n\n\n", "", npat) ;
        }
        head = head->right ;
        counter++ ;
      }
    }
  }

  return(1) ;
}

