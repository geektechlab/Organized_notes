#include <stdint.h>
#include "startup.h"

volatile static uint32_t s_ticks=0;

#ifdef CI_ENABLED
    void (*vector[])(void) __attribute__ ((section (".isr_vector"))) = {
        (void (*)(void))STACK_START,
        (void (*)(void))0x0UL,
    };
#else
    void reset_handler(void) __attribute__ ((weak, alias("default_reset_handler")));
    void PendSV_Handler(void) __attribute__ ((weak, alias("default_PendSV_Handler")));
    void SysTick_Handler(void) __attribute__ ((weak, alias("default_SysTick_Handler")));
    void (*vector[])(void) __attribute__ ((section (".isr_vector"))) = {
        (void (*)(void))STACK_START,
        (void (*)(void))(&reset_handler),
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        (void (*)(void))(&PendSV_Handler)
        (void (*)(void))(&SysTick_Handler)
    };
#endif

void default_handler(void){
    while(1){};
}

void default_reset_handler(void) __attribute__((naked))
{
    uint32_t size = (uint32_t)&_edata - (uint32_t)&_sdata;
    uint32_t *pDst = (uint32_t*)&_sdata;
    uint32_t *pSrc = (uint32_t*)&_sldata;

    for(uint32_t i =0 ; i<size; i++)
    {
        *pDst++ = *pSrc++;
    }

    size = (uint32_t)&_ebss - (uint32_t)&_sbss;
    pDst = (uint32_t*)&_sbss;

    for(uint32_t i =0 ; i<size; i++)
    {
        *pDst++ = 0;
    }

    main();
}

void Incr_tick_count(void)
{
    s_ticks++;
}

void default_SysTick_Handler(void) __attribute__((naked))
{
    /* Saves R0-R3 , R12 , LR ,PC & PSR Register -> EXCEPTION Stack Frame */

    /* Call Incr_tick_count() to show increment handler call count */
    __asm volatile ("PUSH {LR}");
    __asm volatile ("LDR R0,=Incr_tick_count");
    __asm volatile ("BLX R0");
    __asm volatile ("POP {LR}");

    /* Save Previous Context */
    __asm volatile ("CPSID"); /*Change Processor State -> Disable Interrupts */
    __asm volatile ("PUSH {R4-R11}"); /* Push registers on the stack except the stack frame registers */
    __asm volatile ("LDR R0,=current_thread"); /* R0 = &current_thread, Points to current thread's TCB, whose quanta has exhausted, so we are saving the context */
    __asm volatile ("LDR R1,[R0]"); /*R1 = current_thread */
    __asm volatile ("STR SP,[R1]"); /*current_thread/StackPtr = SP => Saves Stack Pointer value at current_thread addr which is StackPtr */

    /* Load next thread & it's stack */
    __asm volatile ("LDR R1,[R1,#4]") /* R1 = current_thread -> nextpt ;  navigates to 2nd entry of tcb which is *next_thread */
    __asm volatile ("STR R1,[R0]"); /* current_thread = R1 ; Store next_thread in current_thread , current_thread = next_thread */
    /* above two mean => current_thread = current_thread -> next_thread ; */

    __asm volatile ("LDR SP,[R1]") /*SP = current_thread -> stackptr */

    /* Load New Context */
    __asm volatile ("POP {R4-R11}");
    __asm volatile ("CPSIE"); /* Change Processor State -> Enable Interrupts */
    __asm volatile ("BX"); /* Restores the new thread's registers , including PC , enabling new thread's execution */
}

void PendSV_Handler(void)
{
    /* Saves R0-R3 , R12 , LR ,PC & PSR Register -> EXCEPTION Stack Frame */

    /* Call Indicate_Pend_Call() to show PendSV handler is running */
    __asm volatile ("PUSH {LR}");
    __asm volatile ("LDR R0,=Indicate_Pend_Call");
    __asm volatile ("BLX R0");
    __asm volatile ("POP {LR}");

    /* Save Previous Context */
    __asm volatile ("CPSID"); /*Change Processor State -> Disable Interrupts */
    __asm volatile ("PUSH {R4-R11}"); /* Push registers on the stack except the stack frame registers */
    __asm volatile ("LDR R0,=current_thread"); /* R0 = &current_thread, Points to current thread's TCB, whose quanta has exhausted, so we are saving the context */
    __asm volatile ("LDR R1,[R0]"); /*R1 = current_thread */
    __asm volatile ("STR SP,[R1]"); /*current_thread/StackPtr = SP => Saves Stack Pointer value at current_thread addr which is StackPtr */

    /* Load next thread & it's stack */
    __asm volatile ("LDR R1,[R1,#4]") /* R1 = current_thread -> nextpt ;  navigates to 2nd entry of tcb which is *next_thread */
    __asm volatile ("STR R1,[R0]"); /* current_thread = R1 ; Store next_thread in current_thread , current_thread = next_thread */
    /* above two mean => current_thread = current_thread -> next_thread ; */

    __asm volatile ("LDR SP,[R1]") /*SP = current_thread -> stackptr */

    /* Load New Context */
    __asm volatile ("POP {R4-R11}");
    __asm volatile ("CPSIE"); /* Change Processor State -> Enable Interrupts */
    __asm volatile ("BX"); /* Restores the new thread's registers , including PC , enabling new thread's execution */
}
