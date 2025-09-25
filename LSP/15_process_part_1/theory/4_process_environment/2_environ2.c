#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>

//Historically, most UNIX systems have provided a third argument to the main
//function that is the address of the environment list

/* just run ./environ2 */

main(int argc,char *argv[],char *envp[]){
	int i;
	printf(" No of arguments = %d\n",argc);
	for(i=0;i<argc;i++)
		printf("%d    %s\n",i,argv[i]);
	i=0;
	while(1)
	{
		if(envp[i] == NULL)
			break;
		printf("%s\n",envp[i]);
		i++;
	}
}
