/*How many time “embedded” will print in this program?*/

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

/* embedded will not get printed because address space gets overwritten */

int main()
{       
	if( execl("/bin/ls","ls",NULL) == -1){         
		perror("execl");
		exit(1);
	}
	printf("embedded\n");
	return 0;
}
