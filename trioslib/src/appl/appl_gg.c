#include <trios.h>

extern int no_of_dimensions, no_of_categories;
extern FILE *logfile;
extern char misclassified_data[LINESIZE];
extern int *map ;


int             /*+ Purpose: Applies a decision tree on an image       +*/
  lapplicDT(
    char *f_appl,  /*+ In: Image filename to be applied                +*/
    char *dt,      /*+ In: decision tree spec file                     +*/
    char *f_mask,  /*+ In: Mask image filename                         +*/
    char *f_res    /*+ Out: resulting output image filename            +*/
)
/*+ Return 1 on success, 0 on failure                                  +*/
{
/*  author: Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)             */
/*  date: Wed Jan 14 1998                                               */

/* Modification by:  Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)        */
/* Date: Fri Feb 11 2000                                                    */
/* Mod: (1) treatment of mask image was condensed in a single routine       */
/*      (set_mask()). (2) updated to conform to the new release of MMACH    */
/*      (changes in mm_conv_norm()).                                        */

  unsigned char  *ucpixels ;  /*  Image to be applied                 */
  unsigned char  *ucpixels1 ; /*  Mask image                          */
  unsigned char  *ucpixels2 ; /*  resulting output image  (char)      */
  unsigned int   *uipixels2 ; /*  resulting output image   (int)      */

  char dt_file[LINESIZE], train_data[LINESIZE];


  int    width, height, npixels, type, output_type = sz8BIT ;

  struct tree_node *root = NULL ;

  img_t *img_tmp ; /* temporary pointer */

  img_t *img_appl, /* Image to be applied         */
          *img_mask, /* Mask image                  */
          *img_res ; /* resulting output image      */

  window_t *win ;

  apert_t  *apt ;



  strcpy(dt_file, dt);

  if(!dt_read(dt_file, &type, &win, &apt)) {
    return trios_error(MSG, "lapplicDT: dt_read() failed.") ;
  }

  printf( "SDFSDFSDFSDFSDF\n\n\n");

#ifdef _DEBUG_
trios_debug("type = %d",type) ;
#endif

  if (type==WKC) {
    output_type = sz32BIT;
  }

  /* read application image file */
  img_appl = img_readPGM(f_appl) ;
  if(!(img_appl)) {
    return trios_error(MSG, "lapplicDT: img_readPGM() failed.") ;
  }

  width = img_get_width(img_appl) ;
  height = img_get_height(img_appl) ;

  if(img_get_pixel_size(img_appl) != sz8BIT) {
      img_tmp = img_convert_type(img_appl, sz8BIT);
      img_free(img_appl);
      /*img_tmp = img_create(width, height, 1, sz8BIT ) ;

/*      if(!mm_conv_norm(img_appl, img_tmp, sz8BIT, 0, 0)) { */
/*        return trios_error(MSG, "lapplicDT: mm_conv_norm() failed.") ; */
/*      }


    if(!mm_conv(img_appl, img_tmp, sz8BIT)) {
      return trios_error(MSG, "lapplicDT: mm_conv() failed.") ;
    }*/

    img_appl = img_tmp ;
    img_tmp = NULL ;
  }

  /* read or create and set mask image border to Zero */
  if(!(img_mask = set_mask(f_mask, width, height, win))) {
    img_free(img_appl) ;
    return trios_error(MSG, "lpapplic: set_mask() failed.") ;
  }


  /* read decision tree file */
  sprintf(dt_file,"%s.dt",dt_file) ;
  printf( "READ DT\n");
  if ((root = read_tree(dt_file)) == NULL) {
    img_free(img_appl) ;
    img_free(img_mask) ;
    return trios_error(MSG, "lapplicDT: read_tree() failed.") ;
  }
  printf("FD\n");
#ifdef _DEBUG_
trios_debug("fmask = %s",f_mask) ;
#endif

  /* creates output image, BYTE by default */
  img_res = img_create(width, height, 1, output_type) ;
  if(!(img_res)) {
    img_free(img_appl) ;
    img_free(img_mask) ;
    return trios_error(MSG, "lapplicDT: img_create() failed.") ;
  }


  ucpixels  = (unsigned char *)img_get_data(img_appl) ;
  ucpixels1 = (unsigned char *)img_get_data(img_mask) ;

  if (type==WKF)
    ucpixels2 = (unsigned char *)img_get_data(img_res) ;
  else uipixels2 = (unsigned int *)img_get_data(img_res) ;


  /* run dtapplic                         */
  npixels = width * height;
#ifdef _DEBUG_
  trios_debug("width=%d, npixels=%d, type=%d", width, npixels, type) ;
#endif

  switch(type) {
    case GG:
      dtapplic0(ucpixels, ucpixels1, ucpixels2, root, win, width, npixels) ;
      break;
    case WKF:
      /*dtapplic1(ucpixels, ucpixels1, ucpixels2, root, win, apt, width, npixels) ;
      break ;*/
    case WKC:
      /*dtapplic2(ucpixels, ucpixels1, uipixels2, root, win, apt, width, npixels) ;
      break ;*/
    default: trios_error(1, "not implemented yet.") ;
  }

  img_writePGM(f_res, img_res);
  img_free(img_appl) ;
  img_free(img_mask) ;
  img_free(img_res) ;

  return(1) ;

}




int             /*+ Purpose: To label a wpattern                           +*/
  classify_wpat(
    unsigned char *wpat,     /*+ In: pointer to an array of configurations +*/
    struct tree_node *root   /*+ In: root if the tree                      +*/
)
/*+ Return: The label of a wpattern                                        +*/
{

/* author: Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)                  */
/*  date: Sun Feb 20 2000                                                   */

  int    j ;
  int    label ;
  double sum = 0 ;

  struct tree_node *cur_node ;

  cur_node = root;
  while (cur_node != NULL) {
    sum = cur_node->coefficients[no_of_dimensions+1];
    for (j=1; j<=no_of_dimensions; j++)
      sum += cur_node->coefficients[j] * wpat[j-1] ;
    if (sum < 0) {
      if (cur_node->left != NULL)
    cur_node = cur_node->left;
      else {
    label = cur_node->left_cat ;
    break;
      }
    }

    else {
      if (cur_node->right != NULL)
    cur_node = cur_node->right;
      else {
    label = cur_node->right_cat ;
    break;
      }
    }
  }

  return(label) ;

}












int             /*+ Purpose: To transform an image by an operator          +*/
  dtapplic0(
    unsigned char  *ucpixels,  /*+ In: Image to be applied                 +*/
    unsigned char  *ucpixels1, /*+ In: Mask image                          +*/
    unsigned char  *ucpixels2, /*+ Out: resulting output image             +*/
    struct tree_node *root,    /*+ In: basis file                          +*/
    window_t  *win,    /*+ In: window                                      +*/
    int        nc,     /*+ In: number of columns of the image              +*/
    int        npixels /*+ In: number of pixels                            +*/
)
/*+ Return: 1 on success 0 on failure                                      +*/
{

/* author: Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)                  */
/*  date: Wed Jan 14 1998  */

  unsigned char *wpat ;          /* pointer to an array of configurations */

  int    label = 0 ;
  int    i, j, k ;

  int   *offset ;          /* pointer to an array of offsets */

  int    wsize ;


  wsize = win_get_wsize(win) ;

  offset = (int *)malloc(sizeof(int)*wsize) ;
  if(offset==NULL) {
    return (int )trios_error(MSG, "papplic: offset_create() failed.") ;
  }
  printf("E dtapplic0\n");
#ifdef _DEBUG_
trios_debug("GG: It will call offset_set") ;
#endif

  /* set vector of offsets */
  offset_set(offset, win, nc, 1) ;

#ifdef _DEBUG_
trios_debug("It called offset_set") ;
#endif

#ifdef _DEBUG_2_
for(i=0; i<wsize; i++) {
  trios_debug("offset[%d]=%d\n", i, offset[i]) ;
}
#endif

  wpat = (unsigned char *)malloc(sizeof(char)*wsize) ;
  if(wpat == NULL) {
    return (int )trios_error(1,"Memory allocation failed.") ;
  }

#ifdef _DEBUG_
  trios_debug("Before the loop") ;
#endif

  for(i = 0; i < npixels; i++) {         /* shifts the window */

    if(ucpixels1[i] != 0) {              /* mask condition satisfied */

      for(j = 0; j < wsize; j++) wpat[j] = 0 ;     /* blank w-pattern */

      /* gets the w-pattern centered at the point j */

      for(j = 0; j < wsize; j++) {  /* for each point of the window...*/

    k = i + offset[j] ;
    if (k < npixels && k >= 0)
    wpat[j] = ucpixels[k] ;

      }

#ifdef _DEBUG_
trios_debug("Pattern coletado");
#endif

#ifdef _DEBUG_2_
 for(j=0;j<wsize;j++) {
   trios_debug("wpat[%d]=%d\n", j, wpat[j]) ;
 }
#endif

      /* checks if the configuration is not in the intervals */

      label = classify_wpat(wpat, root) ;

#ifdef _DEBUG_2_
       trios_debug("achou label=%d \n", label) ;
#endif

      /* if found, put the gray-level to the output image */

      ucpixels2[i] = (unsigned char) map[label] ;

    }

  }

#ifdef _DEBUG_
trios_debug("after the loop") ;
#endif

  return(1) ;

}

