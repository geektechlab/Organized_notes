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

        /* Yield to give control to other task */
        p_kernel_yield_processor()
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

        /* Yield to give control to other task */
        p_kernel_yield_processor()
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

        /* Yield to give control to other task */
        p_kernel_yield_processor()
    }
}

int main(void)
{
    bsp_init();

    /* Initialize PA_5 (LED), PB_13, PB_14 */
    init_GPIO(GPIOA, 5UL);
    init_GPIO(GPIOB, 13UL);
    init_GPIO(GPIOB, 14UL);

    p_init_kernel() ; 
    p_kernel_create_task(&Task1,&Task2,&Task3) ; 
    p_launch_kernel(10);
}

void User_intr_handler(void)
{
    while(1){};
}
