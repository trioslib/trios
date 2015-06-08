#include "string.h"
#include "io_header.h"

/* #define _DEBUG_ */

/*!
    Read a header from a file.

    \param fd File descriptor.
    \return A pointer to a Header structure on success, NULL on failure.
*/

header_t *header_read(FILE * fd)
{
	char buffer[64];
	header_t *aHeader;
	int index1 = 0;

	do
		buffer[index1++] = (char)getc(fd);
	while ((buffer[index1 - 1] != EOF) && (index1 < 64));

	if (index1 != 64)
		return (header_t *) trios_error(1,
						"header_read: file is too short.");

	if (!(aHeader = (header_t *) malloc(sizeof(header_t)))) {
		return (header_t *) trios_error(1,
						"header_read: memory alocation error.");
	}
	strncpy(aHeader->fileType, buffer, 8);
	aHeader->fileType[8] = 0;
	strncpy(aHeader->rest, &buffer[8], 56);

	return (aHeader);
}

/*!
    Write a header to a file.

    \param fd File descriptor.
    \param aHeader Pointer to a Header.
*/

void header_write(FILE * fd, header_t * aHeader)
{
	int i;
	int aux;

	aux = strlen(aHeader->fileType);
	if (aux < 8) {
		for (i = aux; i < 8; i++)
			aHeader->fileType[i] = ' ';
	}
	aHeader->fileType[8] = '\0';

	fprintf(fd, "%s", aHeader->fileType);

	aux = strlen(aHeader->rest);
	if (aux < 56) {
		for (i = aux; i < 56; i++)
			aHeader->rest[i] = '#';
	}

	fprintf(fd, "%s\n", aHeader->rest);

}

/*!
    Compare two headers.

    \param aHeader First header.
    \param otherHeader Second header.
    \return 1 if they are equal, 0 otherwise.
*/

int header_compare(header_t * aHeader, header_t * otherHeader)
{
	return (!strcmp(aHeader->fileType, otherHeader->fileType));

}

/*!
    Checks if the header keyword matches a given keyword.

    \param fd File descriptor.
    \param keyword Keyword (up to 8 chars).
    \return 1 if they are match, 0 otherwise.
*/

int header_match(FILE * fd, const char *keyword)
{
	header_t *aHeader;
	int result;

	/* read file header */
	aHeader = header_read(fd);
	if (!aHeader) {
		return /*(apert_t *) */ trios_error(1,
						    "header_match: header_head() failed.");
	}
#ifdef _DEBUG_
	trios_debug("header_read() done");
#endif

	result = !((int)strcmp(aHeader->fileType, keyword));
	free(aHeader);

	return (result);
}
