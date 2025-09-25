#include "common_driv.h"
#include "gpio.h"
#include "systick.h"
#include "mailbox.h"

uint32_t count0,count1 ;

/* Application task 1 */
void Task1(void)
{
    uint32_t send_data=0;
    while(1)
    {
        p_os_mailbox_send(send_data);
        send_data++;
        count0++;
    }
}

/* Application task 2 */
void Task2(void)
{
    volatile rcvd_data=0;
    while(1)
    {
        rcvd_data = p_os_mailbox_recv();
        count1++;
        (void)rcvd_data;
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

    /* Set up mailbox for tasks to use */
    p_os_mailbox_init();

    p_init_kernel();
    p_kernel_create_task(&Task1,&Task2);
    p_launch_kernel(10);
}

void User_intr_handler(void)
{
    while(1){};
}
