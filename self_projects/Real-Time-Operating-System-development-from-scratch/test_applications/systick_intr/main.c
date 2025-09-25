#include "startup.h"
#include "common.h"
#include "gpio.h"

const uint32_t pra_const = 1UL;
char *pra_arr="Hello";
char pra_arr_1[]="Hello";

void NVIC_SetPriority_local(int32_t IRQn, uint32_t priority)
{
	uint32_t nvic_priority_bits = 4UL;
	uint8_t * scb_shb_addr = (uint8_t *)(0xE000E000UL+0x0D00UL+0x018UL);

	/* Set SHB[11] register from SCB */
    *(scb_shb_addr+((((uint32_t)IRQn) & 0xFUL)-4U)) = (uint8_t)((priority << (8U - nvic_priority_bits)) & (uint32_t)0xFFUL);
};

uint32_t SysTick_Config_local(uint32_t ticks)
{
  /* Systick tick count is not possible greater than 24-bits */
  if ((ticks - 1UL) > 0xFFFFFFUL)
  {
    return (1UL);
  }

  /* Set Systick reload value using LOAD register */
  *((uint32_t *)(0xE000E000UL+0x0010UL+0x04UL))  = (uint32_t)(ticks - 1UL); /* SCS_BASE + Systick offset + LOAD register offset */
  /* Set Systick priority in NVIC */
  //NVIC_SetPriority_local (-15, 0xFUL); /* set Priority for Systick Interrupt */
  /* Load Systick value in VAL register */
  *((uint32_t *)(0xE000E000UL+0x0010UL+0x08UL)) = 0UL; /* SCS_BASE + Systick offset + VAL register offset */
  /* Enable Systick timer and IRQ using CTRL register */
  *((uint32_t *)(0xE000E000UL+0x0010UL+0x0UL))  = 0x7UL;     /* SCS_BASE + Systick offset + CTRL register offset */
  return (0UL);                                                     /* Function successful */
};

int main(void)
{
    //toggle_LED();
	
	/* argument is SystemCoreClock/(1/time)), eg. for 1ms (4000000/(1/0.001)) */
	SysTick_Config_local(4000000U/(1U));

	(*((uint32_t*)0x40021000+60)) = 0x1;

	//__asm volatile ("cpsie i" : : : "memory");

	(void)pra_const;
	(void)pra_arr;
	(void)pra_arr_1;
}

void User_intr_handler(void)
{
    while(1){};
}

