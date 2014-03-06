#include <stdio.h>
#include "trios_xpl.h"
#include "trios_win.h"
#include "trios_error.h"

/*!
    Write BB examples extracted from a gray-level image to use with stackfd.

    \param fd File descriptor.
    \param p Example tree.
*/
void swt_BB_write_tree(FILE * fd, xpl_BB * p)
{
	int i;

	if (p != NULL) {

		swt_BB_write_tree(fd, p->left);

		fprintf(fd, "%d ", p->wpat[0]);

		fprintf(fd, "%d %d\n", p->fq0, p->fq1);

		swt_BB_write_tree(fd, p->right);
	}
}

int swt_write(char *fname, xpl_t * xpl, window_t * win)
{
	FILE *fd;
	int type, wzip, wsize;
	xpl_BB *p;

	fd = fopen(fname, "w");
	if (fd == NULL)
		return trios_error(1, "write_xpl : file open error.");

#ifdef _DEBUG_
	trios_debug("abriu arquivo\n");
#endif

	fprintf(fd, "%d\n", xpl->wsize);
	fprintf(fd, "%d\n", xpl_get_n_nodes(xpl));
	fprintf(fd, "%d\n", xpl_get_sum(xpl));

	swt_BB_write_tree(fd, (xpl_BB *) (xpl->root));
	fclose(fd);
	return (1);

}
