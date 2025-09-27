#include <stdio.h>

/* run [ man tmpfile ] and find more details */

int main(void)
{
	static char mesg[] =
		"Here's Linux magic!";
	FILE *fp;
	char buf[BUFSIZ];

	fp = tmpfile();				/* Get temp file */
	fprintf(fp, "%s", mesg);		/* Write to it */
	fflush(fp);				/* Force it out */

	getchar();
	rewind(fp);				/* Move to front */
	fgets(buf, sizeof buf, fp);		/* Read contents */

	printf("Got back <%s>\n", buf);		/* Print retrieved data */

	fclose(fp);				/* Close file, goes away */
	return 0;				/* All done */
}
