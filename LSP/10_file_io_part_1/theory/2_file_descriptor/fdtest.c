/* [ ./fdtest 2 > op.txt ] will redirect standard error messages to op.txt and same applies to other outputs like [ ./fdtest 1 > op.txt ] also [ ./fdtest 2 > std_err.txt 1 > std_output.txt ] */

#include <stdio.h>
#include <unistd.h>

int main()
{
	char buffer[256];
	int length;
	/* STDOUT_FILENO etc. are defined in /usr/include. Do [ cd /usr/include | grep -nr 'STDOUT_FILENO' ] */
	/* to check if write is system call, run [ man 2 write ] and if comes then it is system call. Also we can check it's arguents etc. from that command */
	//write(STDOUT_FILENO, "Hello STDOUT\n", sizeof("Hello STDOUT\n"));
	//write(STDERR_FILENO, "Hello STDERR\n", sizeof("Hello STDERR\n"));
	/* if enable read then [ ./fdtest <string> ] wil print <string> */
	length = read(STDIN_FILENO, buffer, sizeof(buffer));
	buffer[length] = '\0';
	printf("Data Read from console:%s\n", buffer);
	return 0;
}
