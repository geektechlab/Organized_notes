#include "common_driv.h"

void _close(void) {}
void _lseek(void) {} 
void _read(void)  {}
void _write(void) {}

void bsp_init(void)
{	
	// Enable the clock to GPIO port A
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
	
	// Enable the clock to GPIO port B
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;
}

