#include <unistd.h>
#include <sys/types.h>
#include <mqueue.h>
#include <fcntl.h>
#include <stdio.h>

int main()
{
	int status;
	mqd_t mqdes;

	mqdes = mq_open("/my_queue", O_CREAT|O_RDWR, 0666, NULL);
	perror("mqdes");
	getchar();
	status = mq_close(mqdes);          /* Close message queue  */
	status = mq_unlink("/my_queue"); /* Unlink message queue */
	return 0;
}
