#include <stdio.h>

int main()
{
    char p;
    char a[3]={'1','2','3'};
    printf("%ld %d %d %d\r\n",sizeof(p),sizeof(&a[0]),sizeof(a),sizeof(&a));

	char (*j)[20]; /* j is a pointer to an array of 20 char */ 
	j = (char (*)[20]) malloc( 20 ); 

	for(int i=0; i<20; i++)
	{
	    /* think why double dereference is used here */
		*(*(j+i))=i;
	}

    /* think why first sizeof gives just 8 size instead of 20 */
    printf("%ld %d %d %d\r\n",sizeof(j),sizeof(&j[0]),sizeof(*j),sizeof(&(j[0][0])));

    return 0;
}