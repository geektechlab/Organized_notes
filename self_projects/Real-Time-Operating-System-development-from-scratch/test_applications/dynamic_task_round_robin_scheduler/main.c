#include "common_driv.h"
#include "gpio.h"
#include "systick.h"
#include "thread.h"

uint32_t count0,count1,count2 ; 

/* Application task 1 */
void Task1(void)
{
    while(1)
    {
        count0++;
        write_GPIO(GPIOA, 5UL, true);
        for(int i = 0 ; i<200 ; i++);
        write_GPIO(GPIOA, 5UL, false);
    }
}

/* Application task 2 */
void Task2(void)
{
    while(1)
    {
        count0++;
        write_GPIO(GPIOB, 13UL, true);
        for(int i = 0 ; i<200 ; i++);
        write_GPIO(GPIOB, 13UL, false);
    }
}

/* Application task 3 */
void Task3(void)
{
    while(1)
    {
        count0++;
        write_GPIO(GPIOB, 14UL, true);
        for(int i = 0 ; i<200 ; i++);
        write_GPIO(GPIOB, 14UL, false);
    }
}

int main(void)
{
    bsp_init();

    /* Initialize PA_5 (LED), PB_13, PB_14 */
    init_GPIO(GPIOA, 5UL);
    init_GPIO(GPIOB, 13UL);
    init_GPIO(GPIOB, 14UL);

    tcbTypedef new1,task0,task1,task2;

    int32_t stack0[40];
    p_kernel_add_thread(&Task0 , &task0 , stack0 , sizeof(stack0));

    int32_t stack2[40];
    p_kernel_add_thread(&Task3 , &new1 , stack2 , sizeof(stack2));

    int32_t stack1[40];
    p_kernel_add_thread(&Task1 , &task1 , stack1 , sizeof(stack1));

    int32_t stack3[40];
    p_kernel_add_thread(&Task2 , &task2 , stack3, sizeof(stack3));

    p_init_kernel();
    p_kernel_create_task(&Task1,&Task2,&Task3);
    p_launch_kernel(10);
}

void User_intr_handler(void)
{
    while(1){};
}
