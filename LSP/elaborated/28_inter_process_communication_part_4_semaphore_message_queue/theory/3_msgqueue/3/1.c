#include <unistd.h>
#include <sys/types.h>
#include <mqueue.h>
#include <fcntl.h>
#include <stdio.h>

int main()
{
	int status;
	mqd_t mqdes;
	struct mq_attr attr;

	mqdes = mq_open("/my_queue", O_CREAT|O_RDWR, 0666, NULL);
	perror("mqdes");
	mq_send(mqdes, "hello world", sizeof("hello world"), 10);
	perror("mq_send");
	status = mq_close(mqdes);          /* Close message queue  */
	return 0;
}
