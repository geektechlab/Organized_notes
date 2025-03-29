#include <stdio.h>

/* If function returns 0, it is treated as success. Any other value is treated as error.
whereas, if condition treats 0 as false, all others as true.

#define max(A, B) ((A) > (B) ? (A) : (B))

#define paste(front, back) front ## back

#define dprint(expr) printf(#expr " = %g\n", expr)

#define ISUNSIGNED(a) (a >= 0 && ~a >= 0)
Alternatively, assuming the argument is to be a type, one answer would use type casts: 
#define ISUNSIGNED(type) ((type)0 - 1 > 0)

#define SIZEOF(A) ((char *)(&(A)+1)-(char *)(&(A)))
#define SIZEOF(A) ((&A+1)-(&A)) will not work because subtraction of ptr is in elements not bytes, char element subtraction is okay because char takes 1 bytes, here A is int type so it will not work

#define SIZEOF_TYPE(TYPE) ((((TYPE) *)0)+1)

Sizeof pointer is dependent on machine types. sizeof(p) = &p+1 -&p

using (void *) we can have generic pointer. But with that, we can't dereference because size of datatype pointer assigned to is not available. Also we can not perform arithmetic operations like p+1 or p++.

void swap(int *a, int *b)
{
	/* This is wrong because a and b pointers are created on stack and just changing their pointed address will not affect actual memories pointed by those */
	/* Asked in BRCM */
	/*int *temp=b;
	b=a;
	a=temp;

	/* correct way is this */
	/*int temp=*b;
	*b=*a;
	*a=temp;
}

A string literal created by a pointer initialization is defined as read-only in ANSI C; the program will exhibit undefined behavior if it tries to change the literal by writing through p. Some implementations put string literals in the text segment, where they will be protected with read-only permission. Below creates the string literal: 
	char *p = "breadfruit";
This only works for a string literal. You can't expect to allocate space for, for example, a float literal: 
	float *pip = 3.141; // Bzzt! won't compile

An array can also be initialized with a string literal: 
	char a[] = "gooseberry";
In contrast to a pointer, an array initialized by a literal string is writable. The individual characters can later be changed. The following statement:
	strncpy(a, "black", 5);
gives the string in the array the new value "blackberry".

char message[] = "now is the time"; // an array 
char *pmessage = "now is the time"; // a pointer gets stored in .rodata

message is an array, just big enough to hold the sequence of characters and '\0' that 
initializes it. Individual characters within the array may be changed but amessage will always 
refer to the same storage. On the other hand, pmessage is a pointer, initialized to point to a 
string constant; the pointer may subsequently be modified to point elsewhere, but the result is 
undefined if you try to modify the string contents.

char *c = " Hello" // stores hello as constant and may store in flash so we can't change
char c[20]="Hello" // stores hello in stack, and using pointer to string we can modify

char c[20]="Hello";
c[1]='C'; // HCllo
c="D"; // error: assignment to expression with array type
*(c+3)='f'; // HClfo

char *c="Hello";
c[1]='C'; // this is undefined

int b[2][3] // b is pointer to array of 1-d array, not 2-D array

int c[3][4][5] // c is pointer to array of 2-d array, not 3-D array
int (*p)[4][5]=c is valid
*c or c[0] or &c[0][0] are same

int d[5] // d is pointer to integer value store in one location in array, not to 1-D array

malloc return generic void * pointer. So typecast while assigning.
int *p=(int *)malloc(3*sizeof(int)); // not sizeof(int *p), it will give sizeof integer pointer type not sizeof integer

void* calloc(size_t num, size_t size);
int *p=(int *)calloc(3,sizeof(int)); //allocated memory will be initialized with 0, so no garbage data. Also, since it accesses memeory to write 0, it guarantees that pages are allocated physically, not just lazy allocation.

free(<pointer>); //after free set pointer to NULL, to avoid dangling pointer. Wild pointer is which have no address assigned to. do not return pointer to local variable ( because those are not stack and memory address might not be valid outside function )

array of array char c[8][10]; char (*c)[10]; pointer to array 
array of pointer char *c[15]; char **c; pointer to pointer 
pointer to array char (*c)[64]; char (*c)[64]; doesn't change
pointer to pointer char **c; char **c; doesn't change

we can use pointers to store function addresses apart from variables. by dereferencing it, we can execute function. callback functions can be used for generic queue sort logic.
void (*signal(int sig, void (*func)(int)) ) (int); 

int add(int a, int b) {
    return a + b;
}
int main() {
    int (*fptr)(int, int); // Declare a function pointer that matches the signature of add() fuction
    fptr = &add; // Assign to add()
    printf("%d", fptr(10, 5)); // Call the function via ptr

union bits32_tag
{
	int whole; // one 32-bit value
	struct {char c0,c1,c2,c3;} byte; // four 8-bit bytes
} value;

typedef struct fruit
{
	int weight,
	price_per_lb
} fruit;

- Couple resource and mutex/semaphore functions using resource and function pointer to get a monitor like struct
- For cache improvement where two related items are being loaded, can be grouped in structure so that spatial locality is maintained and less capacity miss happen.

struct point origin, *pp; 
pp = &origin; 
printf("origin is (%d,%d)\n", (*pp).x, (*pp).y); 
printf("origin is (%d,%d)\n", pp->x, pp->y);

struct
{
	unsigned int is_keyword : 1; 
	unsigned int is_extern : 1; 
	unsigned int is_static : 1; 
} flags;
flags.is_extern = flags.is_static = 0; 

strcat(s,t) concatenate t to end of s
strncat(s,t,n) concatenate n characters of t to end of s
strcmp(s,t) return negative, zero, or positive for s < t, s == t, s > t
strncmp(s,t,n) same as strcmp but only in first n characters 
strcpy(s,t) copy t to s
strncpy(s,t,n) copy at most n characters of t to s
strlen(s) return length of s
strchr(s,c) return pointer to first c in s, or NULL if not present 
strrchr(s,c) return pointer to last c in s, or NULL if not present 

isalpha(c) non-zero if c is alphabetic, 0 if not 
isupper(c) non-zero if c is upper case, 0 if not 
islower(c) non-zero if c is lower case, 0 if not 
isdigit(c) non-zero if c is digit, 0 if not 
isalnum(c) non-zero if isalpha(c) or isdigit(c), 0 if not 
isspace(c) non-zero if c is blank, tab, newline, return, formfeed, vertical tab 
toupper(c) return c converted to upper case 
tolower(c) return c converted to lower case

for ( ... ) 
 for ( ... ) { 
 ... 
 if (disaster) 
 goto error; 
 } 
 ... 
 error:

x &= (x-1) deletes the rightmost 1-bit */