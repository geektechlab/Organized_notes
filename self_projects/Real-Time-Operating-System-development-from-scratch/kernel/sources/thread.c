#include "thread.h"

#define TOTAL_THREADS 3
#define Stacksize     100

uint32_t ms_prescalar;
void Kernel_Launch_scheduler(void) ;

/* Internal task control block if dynamic task creation is not supported */
struct tcb{
    int32_t *stackptr;
    struct tcb *next_thread;
};

typedef struct tcb tcbTypedef ;

tcbTypedef thread[TOTAL_THREADS];
tcbTypedef *current_thread;

/* Initialize stack */
int32_t tcb_stack[TOTAL_THREADS][Stacksize];

void SysTick_Handler(void)
{
    tick++ ; 
    if(tick % 10 == 0)
    {
        ICSR = (1U << 28);
    }
}

void Indicate_Pend_Call()
{
    toggle_GPIO(GPIOA, 5UL);
}

void p_kernel_init_stack(int thread_num)
{
    thread[thread_num].stackptr = &tcb_stack[thread_num][Stacksize-16] ;
    tcb_stack[thread_num][Stacksize - 1] = 0x01000000 ; /* xPSR |= (1<<24) ; -> xPSR is at the highest mem. addr. ie. at bottom of stack */
}

uint8_t p_kernel_create_task (void (*task0)(void), void (*task1)(void), void (*task2)(void))
{
    __disable_irq();

    thread[0].next_thread = &thread[1];
    thread[1].next_thread = &thread[2];
    thread[2].next_thread = &thread[0];

    kernel_init_stack(0);
    tcb_stack[0][Stacksize-2] = (int32_t)task0; /* Assign address of task0 to PC */

    kernel_init_stack(1);
    tcb_stack[1][Stacksize-2] = (int32_t)task1; /* Assign address of task0 to PC */

    kernel_init_stack(2);
    tcb_stack[2][Stacksize-2] = (int32_t)task2; /* Assign address of task0 to PC */

    current_thread = &thread[0];

    __enable_irq();

    return 1;
}

void p_init_kernel(void)
{
    __disable_irq();

    ms_prescalar = SystemCoreClock/1000U;
}

void p_launch_kernel(uint32_t quanta)
{
    SysTick -> CTRL = 0;
    SysTick -> VAL = 0;

    SYSPRI3 = (SYSPRI3&0x00FFFFFF) | (0xE0000000); /* Set Systick priority to lowest, ie 7 => (1<<29)|(1<<30)|(1<<31) */

    SysTick -> LOAD = (quanta*ms_prescalar) - 1; /* Scale Quanta to ms */
    SysTick -> CTRL = 0x00000007; /* Enable SysTick with Interrupts , (1<<2) indicated to use CPU CLK */
    Kernel_Launch_scheduler();
}

void p_kernel_delay(uint32_t delayms)
{
    current_thread -> timeout = delayms;
    ready_threads &= ~(1<<(current_thread->thread_ID - 1));
}

void p_kernel_add_thread(void (*task0)(void) , tcbTypedef *new1 , int32_t  *stack , uint32_t size)
{
    __disable_irq();

    if (thread_num==0)
    {
        new1->thread_ID = thread_num++;
        current_thread = new1;
        current_thread->next_thread = new1;
    }
    else
    {
        new1->thread_ID = ++thread_num;
        prev_thread.next_thread = current_thread->next_thread;
        current_thread->next_thread = new1;
        new1->next_thread = prev_thread.next_thread;
    }

    new1->stackptr = &stack[size-16];

    stack[size - 1] = 0x01000000 ; /* xPSR |= (1<<24) ; -> xPSR is at the highest mem. addr. ie. at bottom of stack */
    stack[size - 2] = (int32_t)task0;

    __enable_irq();
}

void kernel_yield_processor(void)
{
     ICSR = 0x10000000 ; /* (1<<28) on ICSR => set-pending PENDSV Interrupt */
}
