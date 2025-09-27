#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/mman.h>

#define CHILD 0

int main(){
	const int SIZE = 4096;
	int fd;
	char buf[2];
	pid_t pid;	
	int childstatus;
	const char *name = "myshm";
	const char *message= "Hello World";
	void *ptr;

	/* create the shared memory segment */
        fd = shm_open(name, O_CREAT | O_RDWR, 0666);
        if (fd == -1) {
                perror("creating shared memory failed\n");
                exit(-1);
        }
	/* configure the size of the shared memory segment */
        ftruncate(fd,SIZE);

	/* now map the shared memory segment in the address space of the process */
       // ptr = mmap(0,SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
        ptr = mmap(0,SIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);
        if (ptr == MAP_FAILED) {
                fprintf(stderr, "Map failed\n");
                return -1;
        }


	pid = fork();
	if(pid == CHILD){
		sprintf(ptr,"%s",message);;
		close(fd);
	}	
	// parent
	else{
		wait(&childstatus);
		printf(" in parent %s\n",ptr);
		close(fd);
	}
}
