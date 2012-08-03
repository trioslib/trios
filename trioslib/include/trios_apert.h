/* Aperturedefinition */

#ifndef _trios_apert_h_
#define _trios_apert_h_

#define VP_Center 1
#define VP_Median 2

#define MAXWINKRANGE 255

/*!
 * This structure defines an aperture (vertical limitation of the gray levels).
 */
typedef struct {
  int   nbands ;    /*!< number of bands */
  int  *ki ;        /*!< range in the graylevel of observed images */
  int  *ko ;        /*!< range in the graylevel of ideal images    */
  int  *vplace ;    /*!< vertical placement of the window          */
} apert_t ;


/*===========================================================================\
   pac_apert.c
\========================================================================== */

apert_t *     /*+ Purpose: create an aperture structure                    +*/
  apert_create(
    int nbands    /*+ In: Number of bands                                  +*/
    ) ;

apert_t *     /*+ Purpose: create an aperture structure and set data       +*/
  apert_create_set(
    int nbands,   /*+ In: Number of bands                                  +*/
    int *ki,      /*+ In: vector of input ranges                           +*/
    int *ko,      /*+ In: vector of output ranges                          +*/
    int *vplace   /*+ In: vector of vertical placements                    +*/
    ) ;

void           /*+ Purpose: free the memory area of an aperture            +*/
  apert_free(
    apert_t *apt             /*+ In/Out: a pointer to an aperture          +*/
    ) ;

int           /*+ Purpose: gives the number of bands of an aperture        +*/
  apert_get_nbands(
    apert_t *apt     /*+ In: pointer to an aperture                        +*/
    ) ;

int *         /*+ Purpose: gives the pointer to the vector of input ranges
                           of an aperture                                  +*/
  apert_get_ki(
    apert_t *apt     /*+ In: pointer to an aperture                        +*/
    ) ;

int           /*+ Purpose: gives the input range of a given band 
                           of an aperture                                  +*/
  apert_get_band_ki(
    apert_t *apt,    /*+ In: pointer to an aperture                        +*/
    int      band    /*+ In: band                                          +*/
    ) ;

int *        /*+ Purpose: gives the pointer to the vector of output ranges
                           of an aperture                                   +*/
  apert_get_ko(
    apert_t *apt     /*+ In: pointer to an aperture                         +*/
    ) ;

int
  apert_get_band_ko(
    apert_t *apt,    /*+ In: pointer to an aperture                        +*/
    int      band    /*+ In: band                                          +*/
    ) ;

int
  apert_get_band_vplace(
    apert_t *apt,    /*+ In: pointer to an aperture                        +*/
    int      band    /*+ In: band                                          +*/
    ) ;


int *        /*+ Purpose: gives the pointer to the vector of vertical
                          placements of an aperture                         +*/
  apert_get_vplace(
    apert_t *apt     /*+ In: pointer to an aperture                         +*/
    ) ;

void           /*+ Purpose: sets the number of bands of an aperture        +*/
  apert_set_nbands(
    apert_t *apt,   /*+ In/Out: pointer to an aperture                     +*/
    int     nbands  /*+ In: number of bands                                +*/
    ) ;

void           /*+ Purpose: sets the vector of input ranges of an aperture +*/
  apert_set_ki(
    apert_t *apt, /*+ In/Out: pointer to an aperture                       +*/
    int     *ki   /*+ In: pointer to the vector of input ranges            +*/
    ) ;

void           /*+ Purpose: sets the vector of output ranges of an aperture +*/
  apert_set_ko(
    apert_t *apt,    /*+ In/Out: pointer to an aperture                    +*/
    int     *ko      /*+ In: pointer to the vector of output ranges        +*/
    ) ;

void           /*+ Purpose: sets the vector of vertical placements of
                            an aperture                                    +*/
  apert_set_vplace(
    apert_t *apt,    /*+ In/Out: pointer to an aperture                    +*/
    int     *vplace  /*+ In: pointer to the vector of vertical placements  +*/
    ) ;


void           /*+ Purpose: sets input range of a given band of an aperture +*/
  apert_set_band_ki(
    apert_t *apt, /*+ In/Out: pointer to an aperture                       +*/
    int     band, /*+ In: a band number of the aperture                    +*/
    int     ki    /*+ In: input range                                      +*/
    ) ;

void           /*+ Purpose: sets output range of a given band of an aperture +*/
  apert_set_band_ko(
    apert_t *apt,    /*+ In/Out: pointer to an aperture                    +*/
    int     band,    /*+ In: a band number of the aperture                 +*/
    int     ko       /*+ In: output range                                  +*/
    ) ;

void           /*+ Purpose: sets the vertical placement of a given band of
                            an aperture                                    +*/
  apert_set_band_vplace(
    apert_t *apt,    /*+ In/Out: pointer to an aperture                    +*/
    int     band,    /*+ In: a band number of the aperture                 +*/
    int     vplace   /*+ In: vertical placement                            +*/
    ) ;

#endif /* _trios_apert_h_ */




