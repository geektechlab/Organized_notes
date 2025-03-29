#include "stdio.h"

/* it is not possible to assign non integer values to enum like string */
typedef enum day
{
	sunday = 1,
	monday,
	tuesday
} day_e;

int main()
{
	day_e day_name = sunday;

	switch(day_name)
	{
		case monday:
			printf("mistake monday\r\n");
		case sunday:
			printf("sunday\r\n");
		case tuesday:
			printf("tuesday\r\n");
			break;
		default:
			printf("no default hit\r\n");
	}

	return 0;
}