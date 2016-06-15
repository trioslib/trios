#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

int trios_error(int type, const char *format, ...)
{
	va_list args;

	if (type == 1) {
		fprintf(stderr, "\n *** ERROR: ");
		va_start(args, format);
		vfprintf(stderr, format, args);
		fprintf(stderr, "\n\n");
		va_end(args);
	} else if (type == 2) {
		va_start(args, format);
		vfprintf(stderr, format, args);
		fputc('\n', stderr);
		va_end(args);
	}

	return 0;
}

void trios_fatal(const char *format, ...)
{
	va_list args;

	fprintf(stderr, "\n *** FATAL ERROR: ");
	va_start(args, format);
	vfprintf(stderr, format, args);
	fprintf(stderr, "\n\n");
	va_end(args);

	exit(1);
}

void trios_debug(const char *format, ...)
{
	va_list args;

	fprintf(stderr, "(debug) ");
	va_start(args, format);
	vfprintf(stderr, format, args);
	fputc('\n', stderr);
	va_end(args);
}

void trios_warning(const char *format, ...)
{
	va_list args;

	fprintf(stderr, "\n *** WARNING: ");
	va_start(args, format);
	vfprintf(stderr, format, args);
	fprintf(stderr, "\n");
	va_end(args);
}
