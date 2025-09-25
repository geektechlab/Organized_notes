#include <stdint.h>

#ifndef THREAD_H
#define THREAD_H

/* Structure of hold state of task: */
typedef struct tcb{
	int32_t *stackptr;
	struct tcb *next_thread;
	uint32_t timeout;
	uint8_t thread_ID;
}tcbTypedef;

/* APIs to initialize kernel, create and yield tasks and launch kernel: */
void launch_p_kernel(uint32_t quanta);
void init_p_kernel(void);
uint8_t p_kernel_create_task (void (*task0)(void), void (*task1)(void), void (*task2)(void));
void p_kernel_yield_processor(void);
void p_kernel_add_thread(void (*task0)(void) , tcbTypedef *new , int32_t * stack, uint32_t size);

#endif /* THREAD_H */
