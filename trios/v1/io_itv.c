#include "stdlib.h"
#include "string.h"
#include "trios_itv.h"
#include "trios_misc.h"
#include "trios_error.h"
#include "io_header.h"

/* #define _DEBUG_ */

/*!
    Read an interval set from a file.

    \param fname File name.
    \param win Window structure.
    \return A pointer to a itv_t structure, NULL on failure.
    \sa itv_t
 */

itv_t *itv_read(char *fname, window_t ** win /*, apert_t **apt */ )
{
	FILE *fd;

	itv_t *itv;

	int nitv, deflabel, maxlabel, type, label, wsize, wzip;
	int i, j;

	char tag, dot;
	int tags_read, stop;

	tags_read = 0;
	stop = 0;
  /**apt = NULL ;*/

	/* Open the file */
	if ((fd = fopen(fname, "r")) == NULL) {
		return (itv_t *) trios_error(1, "File (%s) open failed.",
					     fname);
	}

	/* read & check file header ------------------------------------------- */
	if (!header_match(fd, "ITVSPEC ")) {
		fclose(fd);
		return (itv_t *) trios_error(1, "File header does not match.");
	}

	while (!stop) {

		while (((dot = (char)fgetc(fd)) != '.') && (dot != (char)EOF)) ;

		if (dot == (char)EOF) {
			fclose(fd);
			return (itv_t *) trios_error(MSG,
						     "Unexpected end of file. No tag found.");
		}

		tag = (char)fgetc(fd);

		switch (tag) {

			/* get type ------------------------------------- */

		case 't':
			if (1 != fscanf(fd, "%d", &type)) {
				fclose(fd);
				trios_fatal
				    ("itv(1) Unexpected data or end of file");
			}
			tags_read++;
			break;

			/* get number of intervals                                          */

		case 'n':
			if (1 != fscanf(fd, "%d", &nitv)) {
				fclose(fd);
				trios_fatal
				    ("itv(2) Unexpected data or end of file");
			}
			tags_read++;
			break;

			/* get the default label                                            */

		case 'l':
			if (1 != fscanf(fd, "%d", &deflabel)) {
				fclose(fd);
				trios_fatal
				    ("itv(3) Unexpected data or end of file");
			}
			tags_read++;
			break;

			/* read window information ---------------------------------------- */
		case 'W':
			if (NULL == (*win = win_read_data(fd))) {
				fclose(fd);
				return (itv_t *) trios_error(MSG,
							     "itv_read: win_read_data() failed.");
			}
			tags_read++;
			break;

			/* read aperture information -------------------------------------- */
		case 'A':
			return (itv_t *) trios_error(MSG,
						     "mtm_read: Aperture operators are not supported yet.");
			/* Aperture: this is an optional information */
			/*if(NULL==(*apt = apert_read_data(fd))) {
			   fclose(fd) ;
			   return (itv_t *)trios_error(MSG, "mtm_read: apert_read_data() failed.") ;
			   } */
			break;

		case 'd':
			stop = 1;
			break;

		default:
			fclose(fd);
			(void)trios_error(1, "Unexpected tag %c ", tag);
			return (itv_t *) trios_error(1, " File format error");
		}
	}

	if (tags_read != 4) {
		fclose(fd);
		return (itv_t *) trios_error(1,
					     "Missing Parameters. Looking for .t, .n, .l or .W");
	}

	wsize = win_get_wsize(*win);
	wzip = size_of_zpat(wsize);

	maxlabel = 0;

	/* read intervals  ------------------------------------------ */
	switch (type) {

	case BB:
	case BG:{
			unsigned int *A, *B;
			itv_BX *p, *last = NULL;

			wzip = size_of_zpat(wsize);

			if (NULL == (itv = itv_create(wsize, type, deflabel))) {
				fclose(fd);
				win_free(*win);
				return (itv_t *) trios_error(MSG,
							     "itv_read: itv_create() failed.");
			}

			if ((A =
			     (unsigned int *)malloc(sizeof(int) * wzip)) ==
			    NULL) {
				win_free(*win);
				itv_free(itv);
				fclose(fd);
				return
				    (itv_t *) trios_error(1,
							  "Memory allocation error.");
			}

			if ((B =
			     (unsigned int *)malloc(sizeof(int) * wzip)) ==
			    NULL) {
				win_free(*win);
				itv_free(itv);
				fclose(fd);
				free(A);
				return
				    (itv_t *) trios_error(1,
							  "Memory allocation error.");
			}

			/* reading loop --------------------------------- */
			for (i = 0; i < nitv; i++) {

				for (j = 0; j < wzip; j++) {
					if (fscanf(fd, "%x", &A[j]) != 1) {
						win_free(*win);
						itv_free(itv);
						fclose(fd);
						trios_fatal
						    ("itv(5) Unexpected data or end of file");
					}
				}

				for (j = 0; j < wzip; j++) {
					if (fscanf(fd, "%x", &B[j]) != 1) {
						win_free(*win);
						itv_free(itv);
						fclose(fd);
						trios_fatal
						    ("itv(6) Unexpected data or end of file");
					}
				}

				if (fscanf(fd, "%d", &label) != 1) {
					win_free(*win);
					itv_free(itv);
					fclose(fd);
					trios_fatal
					    ("itv(6) Unexpected data or end of file");
				}

				if (label > maxlabel) {
					maxlabel = label;
				}

				p = itv_nodebx_create(wzip);

				itvbx_set(p, A, B, wzip, label, NULL);
				if (itv->head) {
					last->next = p;
					last = p;
				} else {
					itv->head = (int *)p;
					last = (itv_BX *) p;
				}
			}

			free(A);
			free(B);
			itv->nitv = nitv;
			itv->type = type;
			itv->wsize = wsize;
			itv->wzip = wzip;
			itv->maxlabel = maxlabel;
		}
		break;

	case GG:
	case GB:
		return (itv_t *) trios_error(MSG, "itv_read: type GG not implemented yet");
	case WKC:
	case WKF:{
			return (itv_t *) trios_error(MSG, "itv_read: type GG not implemented yet");
			/*itv_GX  *p, *last=NULL ;

			   if(NULL==(itv = itv_create(wsize, type, deflabel))) {
			   fclose(fd);
			   win_free(*win) ;
			   return (itv_t *)trios_error(MSG, "itv_read: itv_create() failed.") ;
			   }

			   if((A = (char *)malloc(sizeof(char)*wsize)) == NULL) {
			   win_free(*win) ;
			   itv_free(itv) ;
			   fclose(fd) ;
			   return 
			   (itv_t *)trios_error(1, "Memory allocation error.") ;
			   }

			   if((B = (char *)malloc(sizeof(char)*wsize)) == NULL) {
			   win_free(*win) ;
			   itv_free(itv) ;
			   fclose(fd) ;
			   free(A) ;
			   return 
			   (itv_t *)trios_error(1, "Memory allocation error.") ;
			   }

			   /* reading loop --------------------------------- *//*
			   for(i=0; i<nitv ; i++) {

			   for(j=0; j<wsize; j++) {
			   if(fscanf(fd, "%d", &A[j]) != 1) {
			   win_free(*win) ;
			   itv_free(itv) ;
			   fclose(fd) ;
			   trios_fatal("itv(5) Unexpected data or end of file") ;
			   }
			   }

			   for(j=0; j<wsize; j++) {
			   if(fscanf(fd, "%d", &B[j]) != 1) {
			   win_free(*win) ;
			   itv_free(itv) ;
			   fclose(fd) ;
			   trios_fatal("itv(6) Unexpected data or end of file") ;
			   }
			   }

			   if(fscanf(fd, "%d", &label) != 1) {
			   win_free(*win) ;
			   itv_free(itv) ;
			   fclose(fd) ;
			   trios_fatal("itv(6) Unexpected data or end of file") ;
			   }

			   if(label > maxlabel) {
			   maxlabel = label ;
			   }

			   p = itv_nodegx_create(wsize) ;

			   itvgx_set(p, A, B, wsize, label, 0, 0, NULL, NULL) ;
			   p->size = itvgx_size(p, wsize) ; /* added on Oct 31, 2000 - Nina */
			/*
			   p->next = (itv_GX *)itv->head ;
			   itv->head = (int *)p ;
			   }

			   free(A) ;
			   free(B) ;
			   itv->nitv = nitv ;
			   itv->type = type ;
			   itv->wsize = wsize ;
			   itv->maxlabel = maxlabel ; */
		}
		break;

	default:
		fclose(fd);
		return (itv_t *) trios_error(1,
					     "Read ITV works only for BB, BG, GG, GB, WKC, WKF");
	}

	fclose(fd);
	return (itv);

}

/*!
    Write an interval set to a file.

    \param fname File name.
    \param itv Interval set.
    \param win Window.
    \return 1 on success. 0 on failure.
*/

int itv_write(char *fname, itv_t * itv, window_t * win /*apert_t *apt */ )
{
	header_t itvHeader = { "ITVSPEC ", "" };
	FILE *fd;
	int wsize, wzip, type;
	int j;

#ifdef _DEBUG_
	trios_debug("Entering ITV_WRITE()");
#endif

	/* Open file */
	fd = fopen(fname, "w");
	if (fd == NULL)
		return trios_error(1, "File (%s) open error.", fname);

	/* write header */
	header_write(fd, &itvHeader);

	/* get information */
	type = itv_get_type(itv);
	wzip = itv_get_wzip(itv);
	wsize = itv_get_wsize(itv);

	/* write information */
	fprintf(fd, "%s %d\n", ".t", type);
	fprintf(fd, "%s %d\n", ".n", itv_get_nitv(itv));
	fprintf(fd, "%s %d\n", ".l", itv_get_deflabel(itv));

	fprintf(fd, "%s\n", ".W");

	/* write window data */
	win_write_data(fd, win);

	if ((type > 3) && (type < 10)) {
		fprintf(fd, "%s\n", ".A");
		return trios_error(MSG,
				   "itv_write: Aperture operators are not supported yet.");
		/*apert_write_data(fd, apt); */
	}

	fprintf(fd, "%s\n", ".d");

	/* write intervals set */
	switch (type) {

	case BB:
	case BG:{
			itv_BX *p;

			for (p = (itv_BX *) itv_get_head(itv); p; p = p->next) {

				for (j = 0; j < wzip; j++) {
					fprintf(fd, "%x ", p->A[j]);
				}
				for (j = 0; j < wzip; j++) {
					fprintf(fd, "%x ", p->B[j]);
				}
				fprintf(fd, "%d\n", p->label);
			}
		}
		break;

	case GB:
	case GG:{
			return trios_error(MSG,
					   "itv_read: type GG not implemented yet");
			/*
			   itv_GX *p ;

			   for(p = (itv_GX *)itv_get_head(itv); p; p=p->next) {

			   for(j=0; j<wsize; j++) {
			   fprintf(fd, "%d ", (unsigned char)p->A[j]) ;
			   }
			   for(j=0; j<wsize; j++) {
			   fprintf(fd, "%d ", (unsigned char)p->B[j]) ;
			   }
			   fprintf(fd, "%d\n", (unsigned char)p->label) ;
			   } */
		}
		break;

	case WKC:
	case WKF:{
			return trios_error(MSG,
					   "itv_read: type GG not implemented yet");
			/*itv_GX *p ;

			   #ifdef _DEBUG_
			   trios_debug("Entering WKC or WKF") ;
			   #endif

			   for(p = (itv_GX *)itv_get_head(itv); p; p=p->next) {

			   for(j=0; j<wsize; j++) {
			   fprintf(fd, "%d ", p->A[j]) ;
			   }
			   for(j=0; j<wsize; j++) {
			   fprintf(fd, "%d ", p->B[j]) ;
			   }
			   fprintf(fd, "%d\n", p->label) ;
			   } */
		}
		break;

	default:
		fclose(fd);
		return trios_error(1,
				   "Write ITV works only for BB, BG, GG, GB, WKC, WKF");
		break;
	}

	fclose(fd);
	return (1);

}
