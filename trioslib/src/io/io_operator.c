#include "trios.h"
#include <string.h>
#include "stdlib.h"

static int write_window(image_operator_t * iop, FILE * operator_file,
			char *path, int file_name_idx, char temp_string[])
{
	sprintf(temp_string, "%s-files/window", path);
	if (!win_write(temp_string, iop->win)) {
		return trios_error(MSG, "Error writing window at %s",
				   temp_string);
	}
	sprintf(temp_string, "%s-files/window", path + file_name_idx);
	fprintf(operator_file, ".w\n%s\n", temp_string);
	return 1;
}

static int write_collec(image_operator_t * iop, FILE * operator_file,
			char *path, int file_name_idx, char temp_string[])
{
	if (iop->collec == NULL)
		return 1;
	sprintf(temp_string, "%s-files/collec", path);
	if (!xpl_write(temp_string, iop->collec, iop->win, iop->apt)) {
		return trios_error(MSG, "Error writing xpl at %s", temp_string);
	}
	sprintf(temp_string, "%s-files/collec", path + file_name_idx);
	fprintf(operator_file, ".xpl\n%s\n", temp_string);
	return 1;
}

static int write_decision(image_operator_t * iop, FILE * operator_file,
			  char *path, int file_name_idx, char temp_string[])
{
	if (iop->decision == NULL)
		return 1;
	sprintf(temp_string, "%s-files/decision", path);
	if (!mtm_write(temp_string, iop->decision, iop->win, iop->apt)) {
		return trios_error(MSG, "Error writing mtm at %s", temp_string);
	}
	sprintf(temp_string, "%s-files/decision", path + file_name_idx);
	fprintf(operator_file, ".mtm\n%s\n", temp_string);
	return 1;
}

static int write_operator(image_operator_t * iop, FILE * operator_file,
			  char *path, int file_name_idx, char temp_string[])
{
	if (iop->bb == NULL && iop->gg == NULL)
		return 1;
	sprintf(temp_string, "%s-files/operator", path);
	if (iop->type == BB) {
		if (!itv_write(temp_string, iop->bb, iop->win)) {
			return trios_error(MSG, "Error writing itv BB at %s",
					   temp_string);
		}
		sprintf(temp_string, "%s-files/operator", path + file_name_idx);
		fprintf(operator_file, ".bb\n%s\n", temp_string);
	} else if (iop->type == GG || iop->type == GB || iop->type == WKC
		   || iop->type == WKF) {
		if (!write_tree2(temp_string, iop->gg)) {
			return trios_error(MSG, "Error writing tree at %s",
					   temp_string);
		}
		sprintf(temp_string, "%s-files/operator", path + file_name_idx);
		fprintf(operator_file, ".tree\n%s\n", temp_string);
	} else {
		return trios_error(MSG, "Operator not supported");
	}
	return 1;
}

static int write_apert(image_operator_t * iop, FILE * operator_file, char *path,
		       int file_name_idx, char temp_string[])
{
	if (iop->apt == NULL)
		return 1;
	sprintf(temp_string, "%s-files/apert", path);
	if (!apert_write(temp_string, iop->apt)) {
		return trios_error(MSG, "Error writing apert at %s",
				   temp_string);
	}
	sprintf(temp_string, "%s-files/apert", path + file_name_idx);
	fprintf(operator_file, ".apert\n%s\n", temp_string);
	return 1;
}

int image_operator_write(char *path, image_operator_t * iop)
{
	char temp_string[2048];
#ifdef WINDOWS
	char separator = '\\';
#else
	char separator = '/';
#endif
	int i, file_name_idx;
	FILE *operator_file = NULL;
	operator_file = fopen(path, "w");
	if (operator_file == NULL) {
		return trios_error(MSG, "Failed to open file %s\n", path);
	}
	if (iop->type == BB) {
		fprintf(operator_file, ".t\nBB\n");
	} else if (iop->type == GG) {
		fprintf(operator_file, ".t\nGG\n");
	} else if (iop->type == GB) {
		fprintf(operator_file, ".t\nGB\n");
	} else if (iop->type == WKC) {
		fprintf(operator_file, ".t\nWKC\n");
	} else if (iop->type == WKF) {
		fprintf(operator_file, ".t\nWKF\n");
	}

	sprintf(temp_string, "mkdir %s-files", path);
	system(temp_string);

	for (i = strlen(path); i > 0; i--) {
		if (path[i - 1] == separator)
			break;
	}
	file_name_idx = i;

	if (!write_window(iop, operator_file, path, file_name_idx, temp_string)) {
		return 0;
	}
	/*if (!write_collec(iop, operator_file, path, file_name_idx, temp_string)) {
	   return 0;
	   }
	   if (!write_decision(iop, operator_file, path, file_name_idx, temp_string)) {
	   return 0;
	   } */
	if (!write_operator
	    (iop, operator_file, path, file_name_idx, temp_string)) {
		return 0;
	}
	if (!write_apert(iop, operator_file, path, file_name_idx, temp_string)) {
		return 0;
	}
	fclose(operator_file);
	return 1;
}

image_operator_t *image_operator_read(char *path)
{
	image_operator_t *iop;
	char temp_string[2048];
#ifdef WINDOWS
	char separator = '\\';
#else
	char separator = '/';
#endif
	FILE *f;
	window_t *win;
	apert_t *apt;

	f = fopen(path, "r");
	if (f == NULL) {
		return (image_operator_t *) trios_error(MSG,
							"Failed to open file %s.",
							path);
	}
	trios_malloc(iop, sizeof(image_operator_t), image_operator_t *,
		     "Failed to alloc image operator");
	if (fscanf(f, "%*s%s", temp_string) != 1) {
		return (image_operator_t *) trios_error(MSG,
							"Failed to read file %s.",
							path);
	}
	if (temp_string[0] == 'B' && temp_string[1] == 'B') {
		iop->type = BB;
		sprintf(temp_string, "%s-files/window", path);
		iop->win = win_read(temp_string);
		if (iop->win == NULL) {
			return (image_operator_t *) trios_error(MSG,
								"Failed to read window at %s.",
								temp_string);
		}

		sprintf(temp_string, "%s-files/collec", path);
		iop->collec = xpl_read(temp_string, &win, NULL);
		if (iop->collec == NULL) {
			trios_error(MSG,
				    "Failed to read xpl at %s. Continuing...",
				    temp_string);
		} else {
			win_free(win);
		}

		sprintf(temp_string, "%s-files/decision", path);
		iop->decision = mtm_read(temp_string, &win, NULL);
		if (iop->decision == NULL) {
			trios_error(MSG,
				    "Failed to read mtm at %s. Continuing...",
				    temp_string);
		} else {
			win_free(win);
		}

		sprintf(temp_string, "%s-files/operator", path);
		iop->bb = itv_read(temp_string, &win);
		if (iop->bb == NULL) {
			return (image_operator_t *) trios_error(MSG,
								"Failed to read operator at %s.",
								temp_string);
		} else {
			win_free(win);
		}

		iop->apt = NULL;
		iop->gg = NULL;
	} else if (temp_string[0] == 'G') {
		if (temp_string[1] == 'G') {
			iop->type = GG;
		} else if (temp_string[1] == 'B') {
			iop->type = GB;
		}
		sprintf(temp_string, "%s-files/window", path);
		iop->win = win_read(temp_string);
		if (iop->win == NULL) {
			return (image_operator_t *) trios_error(MSG,
								"Failed to read window at %s.",
								temp_string);
		}

		sprintf(temp_string, "%s-files/collec", path);
		iop->collec = xpl_read(temp_string, &win, NULL);
		if (iop->collec == NULL) {
			trios_error(MSG,
				    "Failed to read xpl at %s. Continuing...",
				    temp_string);
		} else {
			win_free(win);
		}

		sprintf(temp_string, "%s-files/decision", path);
		iop->decision = mtm_read(temp_string, &win, NULL);
		if (iop->decision == NULL) {
			trios_error(MSG,
				    "Failed to read mtm at %s. Continuing...",
				    temp_string);
		} else {
			win_free(win);
		}

		sprintf(temp_string, "%s-files/operator", path);
		iop->gg = read_tree2(temp_string);
		if (iop->gg == NULL) {
			return (image_operator_t *) trios_error(MSG,
								"Failed to read operator at %s.",
								temp_string);
		}

		iop->apt = NULL;
		iop->bb = NULL;
	} else if (temp_string[0] == 'W' && temp_string[1] == 'K') {
		if (temp_string[2] == 'F') {
			iop->type = WKF;
		} else if (temp_string[2] == 'C') {
			iop->type = WKC;
		}
		sprintf(temp_string, "%s-files/window", path);
		iop->win = win_read(temp_string);
		if (iop->win == NULL) {
			return (image_operator_t *) trios_error(MSG,
								"Failed to read window at %s.",
								temp_string);
		}

		sprintf(temp_string, "%s-files/collec", path);
		iop->collec = xpl_read(temp_string, &win, NULL);
		if (iop->collec == NULL) {
			trios_error(MSG,
				    "Failed to read xpl at %s. Continuing...",
				    temp_string);
		} else {
			win_free(win);
		}

		sprintf(temp_string, "%s-files/decision", path);
		iop->decision = mtm_read(temp_string, &win, NULL);
		if (iop->decision == NULL) {
			trios_error(MSG,
				    "Failed to read mtm at %s. Continuing...",
				    temp_string);
		} else {
			win_free(win);
		}

		sprintf(temp_string, "%s-files/operator", path);
		iop->gg = read_tree2(temp_string);
		if (iop->gg == NULL) {
			return (image_operator_t *) trios_error(MSG,
								"Failed to read operator at %s.",
								temp_string);
		}

		sprintf(temp_string, "%s-files/apert", path);
		iop->apt = apert_read(temp_string);
		if (iop->apt == NULL) {
			return (image_operator_t *) trios_error(MSG,
								"Failed to read aperture at %s.",
								temp_string);
		}

		iop->bb = NULL;

	} else {
		return (image_operator_t *) trios_error(MSG,
							"Unknown operator type.",
							path);
	}

	fclose(f);
	return iop;
}
