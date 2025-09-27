#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

/* NOTE! We assume that the PDF reader app 'evince' is
 * installed at this location
 */

const char *pdf_reader_app="/usr/bin/evince";

static int exec_pdf_reader_app(char *pdfdoc)
{
	char * const pdf_argv[] = {"evince", pdfdoc, 0};

	if (execv(pdf_reader_app, pdf_argv) < 0) {
		perror("execv failed\n");
		return -1;
	}
	return 0; /* never reached */
}

int main(int argc, char **argv)
{
	if (argc < 2) {
		fprintf(stderr, "Usage: %s {pathname_of_doc.pdf}\n"
			, argv[0]);
		exit(EXIT_FAILURE);
	}
	
	if (exec_pdf_reader_app(argv[1]) < 0)
		perror("exec pdf function failed\n");
	
	exit (EXIT_SUCCESS);
}
