/* runtime loading instead of linktime */

/* this method is useful for linking libraries in large count because we open and load library in memory, use whatever is required and close the library */

/* gcc main.c -o main -I.. -ldl */
/* dlopen, dlsym, dlclose etc. are present in ldl library, thats why we are passing -ldl in above command */

#include "arith.h"
#include <stdio.h>
#include <dlfcn.h>

int main()
{
	int x= 10, y = 20;
	void *handle;
	int (*fnptr)(int,int);

    /* dlopen opens so and stires in handle variable */
	/* we have 
	handle = dlopen("../libarith.so", RTLD_NOW);
	if(handle == NULL){
        printf(" Failed to load library \n");
        exit(2);
    }

    /* here dlsym is fetching symbol for add() function and storing in fnptr pointer variable */
	fnptr = dlsym(handle, "add");
	printf("add:%d\n", fnptr(x, y));
	/* after reading, it is closing so through handle */
	dlclose(handle);

	return 0;
}
