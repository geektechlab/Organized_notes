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
	mq_getattr(mqdes, &attr);
	perror("mq_getattr");
	printf("Flags:%lu\n", attr.mq_flags);
	printf("Maximum message of queue:%lu\n", attr.mq_maxmsg);
	printf("Maximum message size (bytes):%lu\n", attr.mq_msgsize);
	printf("Number of messages currently in queue:%lu\n", attr.mq_curmsgs);
	status = mq_close(mqdes);          /* Close message queue  */
	//status = mq_unlink("/my_queue"); /* Unlink message queue */
	return 0;
}
