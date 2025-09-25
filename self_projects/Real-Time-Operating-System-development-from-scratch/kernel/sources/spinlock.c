#include "spinlock.h"

/* Initialize a spinlock and provide it a number of tokens: */
void p_os_spinlock_init(int32_t *semaphore, int32_t value)
{
    *semaphore = value;
}

/* Set a spinlock */
void p_os_spinlock_set(int32_t *semaphore)
{
    /* Atomic operation is required for spinlock set */
    __disable_irq();

    *semaphore += 1;

    __enable_irq();
}

/* Wait for a spinlock */
void p_os_spinlock_wait(int32_t *semaphore) {
    /* Atomic operation required for spinlock read */
    __disable_irq();

    /* Spin-lock while waiting for spinlock */
    while (*semaphore <= 0) {
        /* Allow SysTick interrupts while waiting */
        __enable_irq();
        __disable_irq();
    }

    /* This releases the spinlock */
    *semaphore -= 1;

    __enable_irq();
}
