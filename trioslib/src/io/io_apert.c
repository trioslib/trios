#include "trios.h"
#include "io_local.h"
/* #define _DEBUG_ */

/*
     -------------------------------------------
     FUNCTION: apert_read
     -------------------------------------------
*/

apert_t *			/*+ Purpose: Read a file with a description of an aperture  + */
apert_read(char *fname		/*+ In: String with the name of the file                + */
    )
/*+ Return: structure apert_t on success, NULL on failure                +*/
{
/* author:  Nina S. Tomita & Roberto Hirata Jr. (nina@ime.usp.br)         */
/* date: Thu Feb 17 2000                                                  */


	FILE *fd;
	apert_t *apt;



	/* open file */
	fd = fopen(fname, "r");
	if (fd == NULL) {
		return (apert_t *) trios_error(1, "File (%s) open failed.",
					       fname);
	}

	/* check file header */
	if (!header_match(fd, "APERTURE")) {
		fclose(fd);
		return (apert_t *) trios_error(1, "Unrecognised file format.");
	}

#ifdef _DEBUG_
	trios_debug("vai entrar no apert_read_data");
#endif

	if (NULL == (apt = apert_read_data(fd))) {
		fclose(fd);
		return (apert_t *) trios_error(MSG,
					       "apert_read: apert_read_data() failed.");
	}

	fclose(fd);
	return (apt);

}



/*
     -------------------------------------------
     FUNCTION: apert_read_data
     -------------------------------------------
*/

apert_t *			/*+ Purpose: Read a description of an aperture   + */
apert_read_data(FILE * fd	/*+ In: file descriptor                         + */
    )
/*+ Return: structure apert_t on success, NULL on failure         +*/
{
/* author:  Nina S. Tomita & Roberto Hirata Jr. (nina@ime.usp.br)  */
/* date: Thu Feb 17 2000                                           */


	char tag, dot;
	int i, stop;
	int *ki, *ko, *vplace, nbands;
	apert_t *apt;


	stop = 0;
	ki = NULL;
	ko = NULL;
	vplace = NULL;
	nbands = 1;		/* default value               */


	while (!stop) {

		while (((dot = (char) fgetc(fd)) != '.')
		       && (dot != (char) EOF));

		if (dot == (char) EOF) {
			fclose(fd);
			return (apert_t *) trios_error(1,
						       "Unexpected end of file. No tag found.");
		}

		tag = (char) fgetc(fd);

		switch (tag) {

		case 'b':
			if (ki || ko || vplace) {
				trios_warning
				    ("Tag .b must appear before the others\
                 in order to be effective. Number of bands=1 was assumed.");
				if (1 != fscanf(fd, "%d", &i)) {
					fclose(fd);
					trios_fatal
					    ("Unexpected data or end of file.");
				}
			} else {
				if (1 != fscanf(fd, "%d", &nbands)) {
					fclose(fd);
					trios_fatal
					    ("Unexpected data or end of file.");
				}

				if (!nbands) {
					fclose(fd);
					return (apert_t *) trios_error(1,
								       "Invalid number of bands.");
				}
			}

			break;

		case 'k':
			ki = (int *) malloc(sizeof(int) * nbands);
			if (!ki) {
				fclose(fd);
				return (apert_t *) trios_error(1,
							       "Memory allocation failed.");
			}
			for (i = 0; i < nbands; i++) {
				if (1 != fscanf(fd, "%d", &ki[i])) {
					fclose(fd);
					trios_fatal
					    ("Unexpected data or end of file.");
				}
			}
			break;

		case 'l':
			ko = (int *) malloc(sizeof(int) * nbands);
			if (!ko) {
				fclose(fd);
				return (apert_t *) trios_error(1,
							       "Memory allocation failed.");
			}
			for (i = 0; i < nbands; i++) {
				if (1 != fscanf(fd, "%d", &ko[i])) {
					fclose(fd);
					trios_fatal
					    ("Unexpected data or end of file.");
				}
			}
			break;

		case 'p':
			vplace = (int *) malloc(sizeof(int) * nbands);
			if (!vplace) {
				fclose(fd);
				return (apert_t *) trios_error(1,
							       "Memory allocation failed.");
			}
			for (i = 0; i < nbands; i++) {
				if (1 != fscanf(fd, "%d", &vplace[i])) {
					fclose(fd);
					trios_fatal
					    ("Unexpected data or end of file.");
				}
			}
			break;

		case 'd':
			stop = 1;
			break;

		default:
			(void) trios_error(1, "Unexpected tag %c ", tag);
			return (apert_t *) trios_error(1,
						       " File format error.");
		}
	}


	/* If input range is not given, then neither the output range nor the */
	/* vertical placement should be given too                             */
	if ((!ki && ko) || (!ki && vplace)) {
		fclose(fd);
		return (apert_t *) trios_error(1,
					       "Nonsense to specify output range or vertical placement\
     without input range");
	}

	/* if ki is not given ki=0, ko=0 and vplace=VP_center */
	if (!ki) {
		ki = (int *) malloc(sizeof(int) * nbands);
		if (!ki) {
			fclose(fd);
			return (apert_t *) trios_error(1,
						       "Memory allocation failed.");
		}

		for (i = 0; i < nbands; i++) {
			ki[i] = 0;
		}
	}

	/* If ko is not given                     */
	/*   then if ki is given  ko=ki           */
	/*        else ki=ko=0                    */
	if (!ko) {
		ko = (int *) malloc(sizeof(int) * nbands);
		if (!ko) {
			fclose(fd);
			return (apert_t *) trios_error(1,
						       "Memory allocation failed.");
		}

		for (i = 0; i < nbands; i++) {
			ko[i] = ki[i];
		}
	}

	/* If vplace is not given                   */
	/*   then if ki is given vplace=VP_center   */
	/*        else vplace=VP_center             */
	if (!vplace) {
		vplace = (int *) malloc(sizeof(int) * nbands);
		if (!vplace) {
			fclose(fd);
			return (apert_t *) trios_error(1,
						       "Memory allocation failed.");
		}

		for (i = 0; i < nbands; i++) {
			vplace[i] = VP_Center;
		}
	}

	/* test if vpcenter is 1 or 2 */
	for (i = 0; i < nbands; i++) {
		if ((vplace[i] != VP_Center) && (vplace[i] != VP_Median)) {
			return (apert_t *) trios_error(1, "The window has an odd \
                                      vertical placement",
						       vplace);
		}
	}

	if (NULL == (apt = apert_create_set(nbands, ki, ko, vplace))) {
		return (apert_t *) trios_error(MSG,
					       "apert_read_data: apert_create() failed.");
	}

	return (apt);

}



/*
     -------------------------------------------
     FUNCTION: apert_write
     -------------------------------------------
*/

int /*+ Purpose: Write to a file a description of an aperture   + */ apert_write(
											char *fname,	/*+ In: file name                                     + */
											apert_t * apt	/*+ In: aperture structure                            + */
    )
/*+ Return: 1 on success, NULL on failure                               +*/
{
/* author:  Nina S. Tomita & Roberto Hirata Jr. (nina@ime.usp.br)        */
/* date: Thu Feb 17 2000                                                  */

	header_t aptHeader = { "APERTURE", "" };
	FILE *fd;

#ifdef _DEBUG_
	trios_debug("Satrting apert_write().");
#endif

	/* open file */
	fd = fopen(fname, "w");
	if (fd == NULL) {
		return trios_error(1, "File (%s) open failed.", fname);
	}
#ifdef _DEBUG_
	trios_debug("fopen() done.");
#endif

	/* writes file header */
	header_write(fd, &aptHeader);

	apert_write_data(fd, apt);

	fclose(fd);
	return (1);
}



/*
     -------------------------------------------
     FUNCTION: apert_write_data
     -------------------------------------------
*/

void /*+ Purpose: Write a data description of an aperture  + */ apert_write_data(
											FILE * fd,	/*+ In: file descriptor                           + */
											apert_t * apt	/*+ In: aperture structure                        + */
    )
/*+ Return: nothing                                                 +*/
{
/* author:  Nina S. Tomita & Roberto Hirata Jr. (nina@ime.usp.br)    */
/* date: Thu Feb 17 2000                                             */

	int nbands, *ki, *ko, *vplace;
	int i;


	nbands = apert_get_nbands(apt);
	ki = apert_get_ki(apt);
	ko = apert_get_ko(apt);
	vplace = apert_get_vplace(apt);

#ifdef _DEBUG_
	trios_debug("nbands = %d", nbands);
#endif

	fprintf(fd, "%s %d\n", ".b", nbands);

	if (ki) {
		fprintf(fd, "%s", ".k");
		for (i = 0; i < nbands; i++) {
			fprintf(fd, " %d", ki[i]);
		}
		fprintf(fd, "\n");

		if (ko) {
			fprintf(fd, "%s", ".l");
			for (i = 0; i < nbands; i++) {
				fprintf(fd, " %d", ko[i]);
			}
			fprintf(fd, "\n");
		}
		if (vplace) {
			fprintf(fd, "%s", ".p");
			for (i = 0; i < nbands; i++) {
				fprintf(fd, " %d", vplace[i]);
			}
			fprintf(fd, "\n");
		}
	}

	fprintf(fd, ".d\n");

}
