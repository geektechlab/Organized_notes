#include "common_driv.h"
#include "gpio.h"
#include "systick.h"
#include "spinlock.h"

uint32_t count0,count1 ;

/* Semaphores for tasks to use */
uint32_t semaphore_1, semaphore_2;

/* Application task 1 */
void Task1(void)
{
    while(1)
    {
        p_os_spinlock_wait(&semaphore_1);
        count0++;
        p_os_spinlock_set(&semaphore_2);
    }
}

/* Application task 2 */
void Task2(void)
{
    while(1)
    {
        p_os_spinlock_wait(&semaphore_1);
        count1++;
        p_os_spinlock_set(&semaphore_2);
    }
}

/*  Here we are taking two spinlocks to avoid race condition.
    If take only one, then there is a possibility that the task releasing semaphore will immediately acquire it again */
int main(void)
{
    bsp_init();

    /* Initialize PA_5 (LED), PB_13, PB_14 */
    init_GPIO(GPIOA, 5UL);
    init_GPIO(GPIOB, 13UL);

    /* Set up semaphores for tasks to use */
    p_os_spinlock_init(&semaphore_1, 1);
    p_os_spinlock_init(&semaphore_2, 0);

    p_init_kernel();
    p_kernel_create_task(&Task1,&Task2);
    p_launch_kernel(10);
}

void User_intr_handler(void)
{
    while(1){};
}
