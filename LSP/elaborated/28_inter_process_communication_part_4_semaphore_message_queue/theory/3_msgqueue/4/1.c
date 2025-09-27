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
	char buffer[8192];
	//char buffer[12];
	int msg_priority = -1;

	mqdes = mq_open("/my_queue", O_CREAT|O_RDWR, 0666, NULL);
	perror("mqdes");
	mq_receive(mqdes, buffer, sizeof(buffer), &msg_priority);
	perror("mq_receive");
	printf("Priority:%d\tBuffer:%s\n", msg_priority, buffer);
	status = mq_close(mqdes);          /* Close message queue  */
	return 0;
}
