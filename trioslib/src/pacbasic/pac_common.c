#include <pacbasic.h>

/* #define _DEBUG_ */
#define BITMSK(POSITION) ( 1 << (POSITION) )

unsigned int bitmsk[NB] =
           { 1, 2, 4, 8, 0x10, 0x20, 0x40, 0x80, 0x100, 0x200, 0x400,
             0x800, 0x1000, 0x2000, 0x4000, 0x8000, 0x10000, 0x20000,
             0x40000, 0x80000, 0x100000, 0x200000, 0x400000, 0x800000,
             0x1000000, 0x2000000, 0x4000000, 0x8000000, 0x10000000,
             0x20000000, 0x40000000, 0x80000000  } ;

/* The vector DIM is used to compute the number of bits ON  */
/* in a sequence of bytes (typically a vector of int)       */
/* DIM[i] gives the number of bits ON the number i contains */
/* (0 <= i <= 255)                                          */
int DIM[256] = {
     /*   0 */  0,
     /*   1 */  1,
     /*   2 */  1,
     /*   3 */  2,
     /*   4 */  1,
     /*   5 */  2,
     /*   6 */  2,
     /*   7 */  3,
     /*   8 */  1,
     /*   9 */  2,
     /*  10 */  2,
     /*  11 */  3,
     /*  12 */  2,
     /*  13 */  3,
     /*  14 */  3,
     /*  15 */  4,
     /*  16 */  1,
     /*  17 */  2,
     /*  18 */  2,
     /*  19 */  3,
     /*  20 */  2,
     /*  21 */  3,
     /*  22 */  3,
     /*  23 */  4,
     /*  24 */  2,
     /*  25 */  3,
     /*  26 */  3,
     /*  27 */  4,
     /*  28 */  3,
     /*  29 */  4,
     /*  30 */  4,
     /*  31 */  5,
     /*  32 */  1,
     /*  33 */  2,
     /*  34 */  2,
     /*  35 */  3,
     /*  36 */  2,
     /*  37 */  3,
     /*  38 */  3,
     /*  39 */  4,
     /*  40 */  2,
     /*  41 */  3,
     /*  42 */  3,
     /*  43 */  4,
     /*  44 */  3,
     /*  45 */  4,
     /*  46 */  4,
     /*  47 */  5,
     /*  48 */  2,
     /*  49 */  3,
     /*  50 */  3,
     /*  51 */  4,
     /*  52 */  3,
     /*  53 */  4,
     /*  54 */  4,
     /*  55 */  5,
     /*  56 */  3,
     /*  57 */  4,
     /*  58 */  4,
     /*  59 */  5,
     /*  60 */  4,
     /*  61 */  5,
     /*  62 */  5,
     /*  63 */  6,
     /*  64 */  1,
     /*  65 */  2,
     /*  66 */  2,
     /*  67 */  3,
     /*  68 */  2,
     /*  69 */  3,
     /*  70 */  3,
     /*  71 */  4,
     /*  72 */  2,
     /*  73 */  3,
     /*  74 */  3,
     /*  75 */  4,
     /*  76 */  3,
     /*  77 */  4,
     /*  78 */  4,
     /*  79 */  5,
     /*  80 */  2,
     /*  81 */  3,
     /*  82 */  3,
     /*  83 */  4,
     /*  84 */  3,
     /*  85 */  4,
     /*  86 */  4,
     /*  87 */  5,
     /*  88 */  3,
     /*  89 */  4,
     /*  90 */  4,
     /*  91 */  5,
     /*  92 */  4,
     /*  93 */  5,
     /*  94 */  5,
     /*  95 */  6,
     /*  96 */  2,
     /*  97 */  3,
     /*  98 */  3,
     /*  99 */  4,
     /* 100 */  3,
     /* 101 */  4,
     /* 102 */  4,
     /* 103 */  5,
     /* 104 */  3,
     /* 105 */  4,
     /* 106 */  4,
     /* 107 */  5,
     /* 108 */  4,
     /* 109 */  5,
     /* 110 */  5,
     /* 111 */  6,
     /* 112 */  3,
     /* 113 */  4,
     /* 114 */  4,
     /* 115 */  5,
     /* 116 */  4,
     /* 117 */  5,
     /* 118 */  5,
     /* 119 */  6,
     /* 120 */  4,
     /* 121 */  5,
     /* 122 */  5,
     /* 123 */  6,
     /* 124 */  5,
     /* 125 */  6,
     /* 126 */  6,
     /* 127 */  7,
     /* 128 */  1,
     /* 129 */  2,
     /* 130 */  2,
     /* 131 */  3,
     /* 132 */  2,
     /* 133 */  3,
     /* 134 */  3,
     /* 135 */  4,
     /* 136 */  2,
     /* 137 */  3,
     /* 138 */  3,
     /* 139 */  4,
     /* 140 */  3,
     /* 141 */  4,
     /* 142 */  4,
     /* 143 */  5,
     /* 144 */  2,
     /* 145 */  3,
     /* 146 */  3,
     /* 147 */  4,
     /* 148 */  3,
     /* 149 */  4,
     /* 150 */  4,
     /* 151 */  5,
     /* 152 */  3,
     /* 153 */  4,
     /* 154 */  4,
     /* 155 */  5,
     /* 156 */  4,
     /* 157 */  5,
     /* 158 */  5,
     /* 159 */  6,
     /* 160 */  2,
     /* 161 */  3,
     /* 162 */  3,
     /* 163 */  4,
     /* 164 */  3,
     /* 165 */  4,
     /* 166 */  4,
     /* 167 */  5,
     /* 168 */  3,
     /* 169 */  4,
     /* 170 */  4,
     /* 171 */  5,
     /* 172 */  4,
     /* 173 */  5,
     /* 174 */  5,
     /* 175 */  6,
     /* 176 */  3,
     /* 177 */  4,
     /* 178 */  4,
     /* 179 */  5,
     /* 180 */  4,
     /* 181 */  5,
     /* 182 */  5,
     /* 183 */  6,
     /* 184 */  4,
     /* 185 */  5,
     /* 186 */  5,
     /* 187 */  6,
     /* 188 */  5,
     /* 189 */  6,
     /* 190 */  6,
     /* 191 */  7,
     /* 192 */  2,
     /* 193 */  3,
     /* 194 */  3,
     /* 195 */  4,
     /* 196 */  3,
     /* 197 */  4,
     /* 198 */  4,
     /* 199 */  5,
     /* 200 */  3,
     /* 201 */  4,
     /* 202 */  4,
     /* 203 */  5,
     /* 204 */  4,
     /* 205 */  5,
     /* 206 */  5,
     /* 207 */  6,
     /* 208 */  3,
     /* 209 */  4,
     /* 210 */  4,
     /* 211 */  5,
     /* 212 */  4,
     /* 213 */  5,
     /* 214 */  5,
     /* 215 */  6,
     /* 216 */  4,
     /* 217 */  5,
     /* 218 */  5,
     /* 219 */  6,
     /* 220 */  5,
     /* 221 */  6,
     /* 222 */  6,
     /* 223 */  7,
     /* 224 */  3,
     /* 225 */  4,
     /* 226 */  4,
     /* 227 */  5,
     /* 228 */  4,
     /* 229 */  5,
     /* 230 */  5,
     /* 231 */  6,
     /* 232 */  4,
     /* 233 */  5,
     /* 234 */  5,
     /* 235 */  6,
     /* 236 */  5,
     /* 237 */  6,
     /* 238 */  6,
     /* 239 */  7,
     /* 240 */  4,
     /* 241 */  5,
     /* 242 */  5,
     /* 243 */  6,
     /* 244 */  5,
     /* 245 */  6,
     /* 246 */  6,
     /* 247 */  7,
     /* 248 */  5,
     /* 249 */  6,
     /* 250 */  6,
     /* 251 */  7,
     /* 252 */  6,
     /* 253 */  7,
     /* 254 */  7,
     /* 255 */  8 } ;



/*
     -------------------------------------------
     FUNCTION: wpat_dimension
     -------------------------------------------
*/

int             /*+ Purpose: Computes the number of bits ON in a w-pattern +*/
  wpat_dimension(
    unsigned int *wpat,/*+ In: binary w-pattern                            +*/
    int  wzip          /*+ In: w-pattern compacted size                    +*/
)
/*+ Return: number of bits ON in the w-pattern                             +*/
{

/*  author: Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)                 */
/*  date: Thu Dec 18 1997                                                   */

  int i, dimension, aux ;

  dimension = 0 ;
  for(i=0; i<wzip; i++) {
    Comp_Dim(wpat[i], DIM, aux) ;

    dimension += aux ;
  }

  return(dimension) ;
}


/*
     -------------------------------------------
     FUNCTION: wpat_order
     -------------------------------------------
*/

int          /*+ Purpose: verify the order relation between two w-patterns +*/
  wpat_order(
    unsigned int *wpat1, /*+ In: first w-pattern                           +*/
    unsigned int *wpat2, /*+ In: second w-pattern                          +*/
    int  wzip          /*+ In: compacted w-pattern size                    +*/
)
/*+ Return: 3 if they cannot be compared, 0 if they are equal, 1 if
            wpat1 < wpat2, and 2 if wpat1 > wpat2                          +*/
{

/*  author: Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)                 */
/*  date: Fri Jan  2 1998                                                   */

/*  Modification by:  Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)       */
/*  Date: Fri Jul 30 1999                                                   */
/*  Mod: bug found and fixed. Code 3 was being returned in some cases       */
/*       where right code was 2. Now the routine also returns code 0,       */
/*       meaning both patterns are the same.                                */

  int i ;
  int retcode ;


  retcode = 0 ;

  for(i=wzip-1; i>=0; i--) {
    if(wpat1[i]!=wpat2[i]) {
      if((wpat1[i] & wpat2[i]) == wpat1[i]) {
        retcode = retcode | 1 ;
      }
      else {
        if((wpat1[i] & wpat2[i]) == wpat2[i]) {
          retcode = retcode | 2 ;
        }
        else retcode = retcode | 3 ;
      }

    }
  }

  return(retcode) ;
}


/*
     -------------------------------------------
     FUNCTION: wpat_greater_than
     -------------------------------------------
*/

int          /*+ Purpose: verify whether a given w-pattern is greater than
                          another one                                      +*/
  wpat_greater_than(
    unsigned int *wpat1,    /*+ In: first w-pattern                        +*/
    unsigned int *wpat2,    /*+ In: second w-pattern                       +*/
    int  wzip          /*+ In: compacted w-pattern size                    +*/
)
/*+ Return: 1 if wpat1 > wpat2, 0 otherwise                                +*/
{

/*  author: Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)                 */
/*  Date: Fri Jul 30 1999                                                   */

  int i ;

  for(i=0; i<wzip; i++) {
    if((wpat1[i] & wpat2[i]) != wpat2[i] )
      return(0) ;
  }

  return(1) ;
}

/*
     -------------------------------------------
     FUNCTION: offset_create
     -------------------------------------------
*/
int* /*+ Purpose: create the offset vector according to window size +*/
    offset_create(
        int wsize /*+ In: window size +*/
    )
    /*+ Return: Pointer to offset vector if successful or NULL in case of
            failure +*/
{
/*  author: Carlos S. Santos. (csantos@ime.usp.br) */
/*  date: Sat Jan 06 2007 */
    int *offset;
    offset = (int *) malloc(sizeof(int)*wsize);
    return offset;
}

/*
     -------------------------------------------
     FUNCTION: offset_set
     -------------------------------------------
*/

void  /*+ Purpose: set the offset vector according to a window and an image +*/
  offset_set(
    int      *offset, /*+ In/Out: the offset vector                         +*/
    window_t *win,    /*+ In: pointer to the window                         +*/
    int       width,  /*+ In: the width of the image                        +*/
    int       band    /*+ In: band of the window                            +*/
  )
/*+ Return: nothing                                                         +*/
{
/*  author: Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)                  */
/*  date: Mon Oct 21 1996                                                    */

/* Date: Thu Feb 17 2000                                                   */
/* Mod: Window structure changed to support multiple bands. Changes to     */
/*      adequate to the structure changing (parameter "band")              */

  int  h_low, h_high, w_low, w_high ;
  int  i, j, k, l ;
  char *windata ;


  h_high = (win_get_height(win)) / 2 ;
  h_low = (win_get_height(win)-1) / 2 ;

  w_high = (win_get_width(win)) / 2 ;
  w_low = (win_get_width(win)-1) / 2 ;

  /*
     Calculate the relative position of pixels associated to each point
     of the window in relation to the pixel in the center of the
     window.
  */

  k = win_get_height(win)*win_get_width(win)*(band-1) ;
  l = 0 ;
  windata = win_get_data(win) ;

  for(i=-h_low; i<=h_high; i++) {
    for (j=-w_low; j<=w_high; j++) {
      if(windata[k] != 0) {
        offset[l] = i*width + j ;
        l++ ;
      }
      k++ ;
    }
  }
}


/*
     -------------------------------------------
     FUNCTION: wpat_compare
     -------------------------------------------
*/

int             /*+ Purpose: compare two w-patterns (taken as integers)    +*/
  wpat_compare(
    unsigned int *wpat1,  /*+ In: first w-pattern                          +*/
    unsigned int *wpat2,  /*+ In: second w-pattern                         +*/
    int  wzip             /*+ In: compacted w-pattern size                 +*/
)
/*+ Return: 0 if they are equal, -1 if the first is less and 1 if the first
            is greater than the second                                     +*/
{

/*  author: Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)                 */
/*  date: Thu Nov 28 1996                                                   */

  int i ;

  for(i=wzip-1; i>=0; i--) {
    if(wpat1[i] > wpat2[i]) return(1) ;
    if(wpat1[i] < wpat2[i]) return(-1) ;
  }

  return(0) ;
}


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
)
/*+ Return: 0 if they are equal, -1 if the first is less and 1 if the first
            is greater than the second                                     +*/
{

/*  author: Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)                 */
/*  date Thu Apr 24 1997                                                    */

  int i ;

  for(i=wsize-1; i>=0; i--) {
    if(wpat1[i] > wpat2[i]) return(1) ;
    if(wpat1[i] < wpat2[i]) return(-1) ;
  }

  return(0) ;
}


/*
     -------------------------------------------
     FUNCTION: wpat_hamming
     -------------------------------------------
*/

int             /*+ Purpose: computes the Hamming distance between two
                    binary w-patterns                                      +*/
  wpat_hamming(
    unsigned int *wpat1,   /*+ In: first w-pattern                         +*/
    unsigned int *wpat2,   /*+ In: second w-pattern                        +*/
    int    wzip           /*+ In: compacted w-pattern's size               +*/
)
/*+ Return: an integer between 0 and n (where n stands for wsize)          +*/
{

/*  author: Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)                 */
/*  date: Fri Nov 22 1996                                                   */

  int wpat_xor, dist ;
  int i, j ;

  dist = 0 ;
  for(i=0; i<wzip; i++) {
    wpat_xor = (wpat1[i]^wpat2[i]) ;   /* exclusive OR */
    for(j=0; j<NB; j++) {
      dist = dist + ((wpat_xor>>j)&1) ;
    }
  }

  return(dist) ;
}


/*
     -------------------------------------------
     FUNCTION: size_of_zpat
     -------------------------------------------
*/

int               /*+ Purpose: calculate the size of compacted w-pattern +*/
  size_of_zpat(
    int  wsize    /*+ size of w-pattern                                  +*/
  )
/*+ Return: the compacted w-pattern size                                 +*/
{
/*  author: Nina S. Tomita                                                */
/*  date: Mon Nov 25 1996                                                 */

  return((wsize+31)/32) ;
}


/*======================================================================== */
/*        Routines to do the treatment of the list of frequencies          */

/*
     -------------------------------------------
     FUNCTION: freq_node_create
     -------------------------------------------
*/

freq_node *              /*+ Purpose: Creates a node for the frequency list +*/
 freq_node_create(
    int label,           /*+ In: label of the pattern                       +*/
    unsigned int freq    /*+ In: frequency of the pattern                   +*/
)
/*+ Return: pointer to the created node                                     +*/
{
/*  author: Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)                  */
/*  date: Thu Apr 24 1997                                                    */

/*  Modification by:  Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)        */
/*  Date: Thu Jul 29 1999                                                    */
/*  Mod: Changed label to int                                                */

  freq_node *p ;


  p = (freq_node *)malloc(sizeof(freq_node)) ;
  if(p == NULL) {
    return (freq_node *)pac_error(1, "Memory allocation failed.") ;
  }

  p->freq = freq ;
  p->label = label ;
  p->next = NULL ;

  return(p) ;

}


/*
     -------------------------------------------
     FUNCTION: freqlist_free
     -------------------------------------------
*/

void           /*+ Purpose: To free a frequency list                     +*/
  freqlist_free(
    freq_node *freq_list      /*+ In: frequency list to be free          +*/
)
/*+ Return: nothing                                                      +*/
{
/*  author: Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)               */
/*  date: Tue Apr 29 1997                                                 */

  /* A non-recursive algorithm will be used here */

  freq_node *p, *paux ;


  p = freq_list ;

  while(p) {

    paux = p->next ;
    free(p) ;
    p = paux ;

  }

}


/*
     -------------------------------------------
     FUNCTION: set_freq
     -------------------------------------------
*/

int /*+ Purpose: Searches for each label of the input freq list in the
        frequency list and adjusts its frequency if it exists, if it
        doesn't exist, puts the label into the list                        +*/
  set_freq(
    freq_node *freqlstin,     /*+ In: pointer to the input frequency list  +*/
    freq_node **freqlist      /*+ In/Out: pointer to the frequency list    +*/
)
/*+ Return: 1 on success, 0 on failure                                     +*/
{
/*  author: Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)                 */
/*  date: Fri Apr 25 1997                                                   */

/*  Modification by:  Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)       */
/*  Date: Thu Jul 29 1999                                                   */
/*  Mod: Changed label to int                                               */

  int  label ;
  int  freq ;
  freq_node *pfreq, *pfreqprev, *qfreq, *qfreqnext;


  if (*freqlist==NULL) {
    *freqlist = freqlstin ;

#ifdef _DEBUG_
pac_debug("Entrou no xpl_set_freq com freqlist = null");
pac_debug("label1=%d , freq1=%d\n", freqlstin->label, freqlstin->freq) ;
#endif // _DEBUG_

  }

  else {

#ifdef _DEBUG_
pac_debug("Entrou no xpl_set_freq");
pac_debug("label1=%d , freq1=%d\n", freqlstin->label, freqlstin->freq) ;
#endif // _DEBUG_

    qfreq = freqlstin ;
    pfreqprev = NULL ;
    pfreq = *freqlist ;

    while(qfreq) {

      label = qfreq->label ;
      freq  = qfreq->freq ;

      qfreqnext = qfreq->next ;

      while( (pfreq) && (pfreq->label > label) ) {
	pfreqprev = pfreq ;
	pfreq = pfreq->next ;
      }

      /* Has reached the end of the current list of frequencies? YES,  */
      /* then insert the remaining of the input list to the end of the */
      /* current one                                                   */

      if (pfreq==NULL) {

	pfreqprev->next = qfreq ;
	qfreq = NULL ;

      }

      else { /* No, has the node the searched label ? */

	if (pfreq->label == label) { /* YES, then update information  */

	  pfreq->freq += freq ;
	  free(qfreq) ;
	}

	else { /* NO, then insert the node. */

	  if (pfreqprev == NULL) {

	    qfreq->next = pfreq ;
	    *freqlist = qfreq ;

	  }

	  else {

            qfreq->next = pfreq ;
	    pfreqprev->next = qfreq ;

	  }

	}

      }

      qfreq = qfreqnext ;

    }

  }

  return(1) ;

}


/*
     -------------------------------------------
     FUNCTION: FREQ_SUM
     -------------------------------------------
*/

unsigned int    /*+ Purpose: Sums the frequencies of w-pattern             +*/
  freq_sum(
    freq_node  *freqlist     /*+ In: frequency list of a w-pattern         +*/
)
/*+ Return: The sum of the frequencies of the labels that appeared in the
            w-pattern                                                      +*/
{
/*  author: Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)                 */
/*  date: Thu May  8 1997                                                   */

  freq_node *p ;
  unsigned int sum ;


  p = freqlist ;
  sum = 0 ;

  while(p) {

    sum += p->freq ;
    p = p->next ;

  }

  return(sum) ;

}
