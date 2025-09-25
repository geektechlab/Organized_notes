#include "common_driv.h"
#include "gpio.h"
#include "systick.h"
#include "semaphore.h"

uint32_t count0,count1;

static idle_count=0;

/* Semaphores for tasks to use */
uint32_t semaphore_1, semaphore_2;

/* Application task 1 */
void Task1(void)
{
    while(1)
    {
        p_os_semaphore_wait(&semaphore_1);
        count0++;
        /* idle_count can be checked here to find CPU utilization */
        p_os_semaphore_set(&semaphore_2);
    }
}

/* Application task 2 */
void Task2(void)
{
    while(1)
    {
        p_os_semaphore_wait(&semaphore_1);
        count1++;
        p_os_semaphore_set(&semaphore_2);
    }
}

/* Idle task */
void Idle_Task(void)
{
    static systick_call_count=s_ticks;
    while(1)
    {
        if(((systick_call_count - s_ticks)>1) || ((systick_call_count - s_ticks) == 1))
        {
            idle_count++;
            systick_call_count=s_ticks;
        }
    }
}

/*  Here we are taking two semaphores to avoid race condition.
    If take only one, then there is a possibility that the task releasing semaphore will immediately acquire it again */
int main(void)
{
    bsp_init();

    /* Initialize PA_5 (LED), PB_13, PB_14 */
    init_GPIO(GPIOA, 5UL);
    init_GPIO(GPIOB, 13UL);

    /* Set up semaphores for tasks to use */
    p_os_semaphore_init(&semaphore_1, 1);
    p_os_semaphore_init(&semaphore_2, 0);

    p_init_kernel();
    p_kernel_create_task(&Task1,&Task2);
    p_launch_kernel(10);
}

void User_intr_handler(void)
{
    while(1){};
}
