#include "trios.h"

/* #define _DEBUG_ */



/*
     -------------------------------------------
     FUNCTION: apert_create
     -------------------------------------------
*/

apert_t *			/*+ Purpose: create an aperture structure                    + */
apert_create(int nbands		/*+ In: Number of bands                                  + */
    )
/*+ Return: a pointer to the aperture on success, NULL on failure          +*/
{
/* author: Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)                  */
/* date: Thu Feb 17 2000                                                    */

	apert_t *apt;

	apt = (apert_t *) malloc(sizeof(apert_t));
	if (apt == NULL) {
		return (apert_t *) trios_error(1, "Memory allocation failed.");
	}

	apt->nbands = nbands;
	apt->ki = (int *) malloc(sizeof(int) * nbands);
	if (!apt->ki) {
		free(apt);
		return (apert_t *) trios_error(1, "Memory allocation failed.");
	}
	apt->ko = (int *) malloc(sizeof(int) * nbands);
	if (!apt->ko) {
		free(apt->ki);
		free(apt);
		return (apert_t *) trios_error(1, "Memory allocation failed.");
	}
	apt->vplace = (int *) malloc(sizeof(int) * nbands);
	if (!apt->vplace) {
		free(apt->ki);
		free(apt->ko);
		free(apt);
		return (apert_t *) trios_error(1, "Memory allocation failed.");
	}

	return (apt);
}

/*
     -------------------------------------------
     FUNCTION: apert_create_set
     -------------------------------------------
*/

apert_t *			/*+ Purpose: create an aperture structure and set data       + */
apert_create_set(int nbands,	/*+ In: Number of bands                                  + */
		 int *ki,	/*+ In: vector of input ranges                           + */
		 int *ko,	/*+ In: vector of output ranges                          + */
		 int *vplace	/*+ In: vector of vertical placements                    + */
    )
/*+ Return: a pointer to the aperture on success, NULL on failure          +*/
{
/* author: Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)                  */
/* date: Thu Feb 17 2000                                                    */

	apert_t *apt;
	int i;

	if (!(apt = apert_create(nbands))) {
		return (apert_t *) trios_error(MSG, "apert_create() failed");
	}

	apt->nbands = nbands;

	for (i = 0; i < nbands; i++) {
		apert_set_band_ki(apt, i + 1, ki[i]);
		apert_set_band_ko(apt, i + 1, ko[i]);
		apert_set_band_vplace(apt, i + 1, vplace[i]);
	}

	return (apt);
}


/*
     -------------------------------------------
     FUNCTION: apert_free
     -------------------------------------------
*/

void /*+ Purpose: free the memory area of an aperture            + */ apert_free(
											apert_t * apt	/*+ In/Out: a pointer to an aperture          + */
    )
/*+ Return: nothing                                                        +*/
{
/* author: Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)                  */
/* date: Thu Feb 17 2000                                                    */

	if (apt) {
		if (apt->ki)
			free(apt->ki);
		if (apt->ko)
			free(apt->ko);
		if (apt->vplace)
			free(apt->vplace);
		free(apt);
	}

}


/*
     -------------------------------------------
     FUNCTION: apert_get_nbands
     -------------------------------------------
*/

int /*+ Purpose: gives the number of bands of an aperture        + */
apert_get_nbands(
			       apert_t * apt	/*+ In: pointer to an aperture                        + */
    )
/*+ Return: the number of bands of the aperture                            +*/
{
/* author: Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)                  */
/* date: Thu Feb 17 2000                                                    */

	return (apt->nbands);
}

/*
     -------------------------------------------
     FUNCTION: apert_get_band_ki
     -------------------------------------------
*/

int				/*+ Purpose: gives the input range of a given band
				   of an aperture                                  + */ apert_get_band_ki(
														 apert_t * apt,	/*+ In: pointer to an aperture                        + */
														 int band	/*+ In: band                                          + */
    )
/*+ Return: the input range of the given band of the aperture              +*/
{
/* author: Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)                  */
/* date: Thu Feb 17 2000                                                    */

	return (apt->ki[band - 1]);
}

/*
     -------------------------------------------
     FUNCTION: apert_get_ki
     -------------------------------------------
*/

int *				/*+ Purpose: gives the pointer to the vector of input ranges
				   of an aperture                                  + */
apert_get_ki(apert_t * apt	/*+ In: pointer to an aperture                        + */
    )
/*+ Return: the pointer to the vector of input ranges of the aperture      +*/
{
/* author: Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)                  */
/* date: Thu Feb 17 2000                                                    */

	return (apt->ki);
}

/*
     -------------------------------------------
     FUNCTION: apert_get_band_ko
     -------------------------------------------
*/

int				/*+ Purpose: gives the output range of a given band
				   of an aperture                                   + */ apert_get_band_ko(
														  apert_t * apt,	/*+ In: pointer to an aperture                        + */
														  int band	/*+ In: band                                          + */
    )
/*+ Return: the output range of the given band of the aperture             +*/
{
/* author: Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)                  */
/* date: Thu Feb 17 2000                                                    */

	return (apt->ko[band - 1]);
}


/*
     -------------------------------------------
     FUNCTION: apert_get_ko
     -------------------------------------------
*/

int *				/*+ Purpose: gives the pointer to the vector of output ranges
				   of an aperture                                   + */
apert_get_ko(apert_t * apt	/*+ In: pointer to an aperture                         + */
    )
/*+ Return: the pointer to the vector of output ranges of the aperture      +*/
{
/* author: Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)                  */
/* date: Thu Feb 17 2000                                                    */

	return (apt->ko);
}


/*
     -------------------------------------------
     FUNCTION: apert_get_band_vplace
     -------------------------------------------
*/

int				/*+ Purpose: gives the vertical placement of a givem band
				   of an aperture                                   + */ apert_get_band_vplace(
														      apert_t * apt,	/*+ In: pointer to an aperture                       + */
														      int band	/*+ In: band                                         + */
    )
/*+ Return: the vertical placement of the given band of the aperture       +*/
{
/* author: Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)                  */
/* date: Thu Feb 17 2000                                                    */

	return (apt->vplace[band - 1]);
}

/*
     -------------------------------------------
     FUNCTION: apert_get_vplace
     -------------------------------------------
*/

int *				/*+ Purpose: gives the pointer to the vector of vertical
				   placements of an aperture                         + */
apert_get_vplace(apert_t * apt	/*+ In: pointer to an aperture                         + */
    )
/*+ Return: the pointer to the vector of vertical placements of the aperture +*/
{
/* author: Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)                  */
/* date: Thu Feb 17 2000                                                    */

	return (apt->vplace);
}


/*
     -------------------------------------------
     FUNCTION: apert_set_nbands
     -------------------------------------------
*/

void /*+ Purpose: sets the number of bands of an aperture        + */
apert_set_nbands(
			       apert_t * apt,	/*+ In/Out: pointer to an aperture                     + */
			       int nbands	/*+ In: number of bands                                + */
    )
/*+ Return: nothing                                                        +*/
{
/* author: Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)                  */
/* date: Thu Feb 17 2000                                                    */

	apt->nbands = nbands;
}

/*
     -------------------------------------------
     FUNCTION: apert_set_ki
     -------------------------------------------
*/

void /*+ Purpose: sets the vector of input ranges of an aperture + */
apert_set_ki(
			   apert_t * apt,	/*+ In/Out: pointer to an aperture                       + */
			   int *ki	/*+ In: pointer to the vector of input ranges            + */
    )
/*+ Return: nothing                                                        +*/
{
/* author: Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)                  */
/* date: Thu Feb 17 2000                                                    */

	apt->ki = ki;
}

/*
     -------------------------------------------
     FUNCTION: apert_set_ko
     -------------------------------------------
*/

void /*+ Purpose: sets the vector of output ranges of an aperture + */
apert_set_ko(
			   apert_t * apt,	/*+ In/Out: pointer to an aperture                    + */
			   int *ko	/*+ In: pointer to the vector of output ranges        + */
    )
/*+ Return: nothing                                                        +*/
{
/* author: Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)                  */
/* date: Thu Feb 17 2000                                                    */

	apt->ko = ko;
}


/*
     -------------------------------------------
     FUNCTION: apert_set_vplace
     -------------------------------------------
*/

void				/*+ Purpose: sets the vector of vertical placements of
				   an aperture                                    + */ apert_set_vplace(
													       apert_t * apt,	/*+ In/Out: pointer to an aperture                    + */
													       int *vplace	/*+ In: pointer to the vector of vertical placements  + */
    )
/*+ Return: nothing                                                        +*/
{
/* author: Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)                  */
/* date: Thu Feb 17 2000                                                    */

	apt->vplace = vplace;
}


/*
     -------------------------------------------
     FUNCTION: apert_set_band_ki
     -------------------------------------------
*/

void /*+ Purpose: sets input range of a given band of an aperture + */
apert_set_band_ki(
				apert_t * apt,	/*+ In/Out: pointer to an aperture                       + */
				int band,	/*+ In: a band number of the aperture                    + */
				int ki	/*+ In: input range                                      + */
    )
/*+ Return: nothing                                                        +*/
{
/* author: Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)                  */
/* date: Thu Feb 17 2000                                                    */

	apt->ki[band - 1] = ki;
}

/*
     -------------------------------------------
     FUNCTION: apert_set_band_ko
     -------------------------------------------
*/

void /*+ Purpose: sets output range of a given band of an aperture + */
apert_set_band_ko(
				apert_t * apt,	/*+ In/Out: pointer to an aperture                    + */
				int band,	/*+ In: a band number of the aperture                 + */
				int ko	/*+ In: output range                                  + */
    )
/*+ Return: nothing                                                        +*/
{
/* author: Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)                  */
/* date: Thu Feb 17 2000                                                    */

	apt->ko[band - 1] = ko;
}


/*
     -------------------------------------------
     FUNCTION: apert_set_band_vplace
     -------------------------------------------
*/

void				/*+ Purpose: sets the vertical placement of a given band of
				   an aperture                                    + */ apert_set_band_vplace(
														    apert_t * apt,	/*+ In/Out: pointer to an aperture                    + */
														    int band,	/*+ In: a band number of the aperture                 + */
														    int vplace	/*+ In: vertical placement                            + */
    )
/*+ Return: nothing                                                        +*/
{
/* author: Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)                  */
/* date: Thu Feb 17 2000                                                    */

	apt->vplace[band - 1] = vplace;
}
