#include "common_driv.h"
#include "gpio.h"
#include "systick.h"

#define M_THOUSAND 1000UL
#define MAIN_FREQ  (80*M_THOUSAND*M_THOUSAND)
#define DIV_FACTOR M_THOUSAND
#define ONE_SECOND (MAIN_FREQ/DIV_FACTOR)

int main(void)
{
	bsp_init();

	/* Initialize PA_5 (LED), PB_13, PB_14, PB_15 */
	init_GPIO(GPIOA, 5UL);
	init_GPIO(GPIOB, 13UL);
	init_GPIO(GPIOB, 14UL);
	init_GPIO(GPIOB, 15UL);

	/* Generate interrupt after each 4 sec */
	/* ?Multiply by 50 */
    systick_init(4*50*ONE_SECOND);
    while(1UL)
    {
        static bool write_val = true;
        volatile uint32_t count=0;
        if (s_ticks==1)
        {
            if(count==6)
            {
                __disable_irq();
            }

        	write_val = (( write_val == true) ? false : true);
        	//__asm volatile ("BKPT #0");
            write_GPIO(GPIOA, 5UL, write_val);
            write_GPIO(GPIOB, 13UL, write_val);
            write_GPIO(GPIOB, 14UL, write_val);
            write_GPIO(GPIOB, 15UL, write_val);

            s_ticks=0;
            count++;
        }
    }
}

void User_intr_handler(void)
{
    while(1){};
}
