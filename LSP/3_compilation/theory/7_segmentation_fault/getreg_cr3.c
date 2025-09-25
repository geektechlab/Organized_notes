#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

typedef unsigned long u64;

static u64 get_cr3(void)
{
	/* Pro Tip: x86 ABI: query a register's value by moving it's value into RAX.
	 * [RAX] is returned by the function! */
	__asm__ __volatile__(
		    "movq %cr3, %rax");	/* at&t syntax: movq <src_reg>, <dest_reg> */
}

int main(void)
{
	printf("Hello, inline assembly:\n [CR3] = 0x%lx\n", get_cr3());
	exit(0);
}

