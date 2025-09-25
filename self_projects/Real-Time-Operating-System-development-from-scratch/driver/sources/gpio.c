#include "gpio.h"

void init_GPIO(GPIO_TypeDef * base_addr, uint32_t pin_num)
{
	base_addr->MODER &=~(GPIO_MODER_MODE0 << (pin_num*2));
	base_addr->MODER |=((GPIO_MODE & GPIO_MODER_MODE0) << (pin_num*2));
}

void toggle_GPIO(GPIO_TypeDef * base_addr, uint32_t pin_num)
{
    static int pra_pin_state = 0;

    while(1)
    {
        pra_pin_state = (~pra_pin_state);
        if(pra_pin_state != 0UL)
        {
            base_addr->BSRR = GPIO_BSRR_BS5(pin_num);
        }
        else
        {
            base_addr->BRR = GPIO_BRR_BR5(pin_num);
        }
        for(uint32_t test_pra=0UL;test_pra<50000UL;test_pra++){};
    }
}

void invert_GPIO(GPIO_TypeDef * base_addr, uint32_t pin_num)
{
    static int pra_pin_state = 0;

    pra_pin_state = (~pra_pin_state);
    if(pra_pin_state != 0UL)
    {
        base_addr->BSRR = GPIO_BSRR_BS5(pin_num);
    }
    else
    {
        base_addr->BRR = GPIO_BRR_BR5(pin_num);
    }
}

void write_GPIO(GPIO_TypeDef * base_addr, uint32_t pin_num, bool set)
{
    if(set != false)
    {
        base_addr->BSRR = GPIO_BSRR_BS5(pin_num);
    }
    else
    {
        base_addr->BRR = GPIO_BRR_BR5(pin_num);
    }
}

