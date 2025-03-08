/******************************************************************************
* File Name: main.c
*
* Description: This code example demonstrates displaying graphics on an E-Ink
* display using EmWin graphics library and the AppWizard GUI design tool.
*
* Related Document: See README.md
*
*******************************************************************************
* Copyright 2021-2023, Cypress Semiconductor Corporation (an Infineon company) or
* an affiliate of Cypress Semiconductor Corporation.  All rights reserved.
*
* This software, including source code, documentation and related
* materials ("Software") is owned by Cypress Semiconductor Corporation
* or one of its affiliates ("Cypress") and is protected by and subject to
* worldwide patent protection (United States and foreign),
* United States copyright laws and international treaty provisions.
* Therefore, you may use this Software only as provided in the license
* agreement accompanying the software package from which you
* obtained this Software ("EULA").
* If no EULA applies, Cypress hereby grants you a personal, non-exclusive,
* non-transferable license to copy, modify, and compile the Software
* source code solely for use in connection with Cypress's
* integrated circuit products.  Any reproduction, modification, translation,
* compilation, or representation of this Software except as specified
* above is prohibited without the express written permission of Cypress.
*
* Disclaimer: THIS SOFTWARE IS PROVIDED AS-IS, WITH NO WARRANTY OF ANY KIND,
* EXPRESS OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, NONINFRINGEMENT, IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. Cypress
* reserves the right to make changes to the Software without notice. Cypress
* does not assume any liability arising out of the application or use of the
* Software or any product or circuit described in the Software. Cypress does
* not authorize its products for use in any products where a malfunction or
* failure of the Cypress product may reasonably be expected to result in
* significant property damage, injury or death ("High Risk Product"). By
* including Cypress's product in a High Risk Product, the manufacturer
* of such system or application assumes all risk of such use and in doing
* so agrees to indemnify Cypress against all liability.
*******************************************************************************/

#include "cyhal.h"
#include "cybsp.h"
#include "cy_retarget_io.h"
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"

/*******************************************************************************
* Macros
*******************************************************************************/
#define TASK_STACK_SIZE        (1024*10)
#define TASK_PRIORITY          (configMAX_PRIORITIES - 3)
#define CLOCK_100_MHZ          (100000000u)

/* The periods assigned to the one-shot and auto-reload timers are 3.333 second and half a second
 * respectively. */
#define original_PERIOD    pdMS_TO_TICKS( 500 )

TimerHandle_t xBacklightTimer;

static volatile uint32_t count=0;

/*******************************************************************************
* Function Prototypes
********************************************************************************/
static void prvBacklightTimerCallback( TimerHandle_t xTimer )
{
	TickType_t xTimeNow = xTaskGetTickCount();

	/* Print the time at which the backlight was turned off. */
	printf("Timer expired, turning backlight OFF at time=%d\r\n",xTimeNow);
	count=0;
}

static void vResetTask( void *pvParameters )
{
	const TickType_t xShortDelay = pdMS_TO_TICKS( 50 );
	TickType_t xTimeNow;

	while(1)
	{
		xTimeNow = xTaskGetTickCount();

		/* Reset the software timer. If the backlight was previously off, then this call
		will start the timer. If the backlight was previously on, then this call will
		restart the timer. A real application may read key presses in an interrupt. If
		this function was an interrupt service routine then xTimerResetFromISR() must be
		used instead of xTimerReset(). */

		if(count<6)
		{
			printf("Resetting timer=%d\r\n",xTimeNow);
			xTimerReset(xBacklightTimer,xShortDelay);
		}
		vTaskDelay(200);
		count++;
	}
}


void vApplicationIdleHook(void)
{
	//printf("idle task\r\n");
}

/*******************************************************************************
* Global Variables
*******************************************************************************/
/* This enables RTOS aware debugging. */
volatile int uxTopUsedPriority;

/*******************************************************************************
* Function Name: int main(void)
********************************************************************************
*
* Summary: This is the main for this code example.  This function initializes
*          the BSP, creates the eInk_task and starts the scheduler.
*
* Parameters:
*  None
*
* Return:
*  int
*
*******************************************************************************/
int main(void)
{
    cy_rslt_t result ;
#if defined (CY_DEVICE_SECURE)
    cyhal_wdt_t wdt_obj;

    /* Clear watchdog timer so that it doesn't trigger a reset */
    result = cyhal_wdt_init(&wdt_obj, cyhal_wdt_get_max_timeout_ms());
    CY_ASSERT(CY_RSLT_SUCCESS == result);
    cyhal_wdt_free(&wdt_obj);
#endif

    /* This enables RTOS aware debugging in OpenOCD */
    uxTopUsedPriority = configMAX_PRIORITIES - 1 ;

    /* Initialize the device and board peripherals */
    result = cybsp_init();
    CY_ASSERT(result == CY_RSLT_SUCCESS);

    /* Initialize retarget-io to use the debug UART port */
    result = cy_retarget_io_init(CYBSP_DEBUG_UART_TX, CYBSP_DEBUG_UART_RX,
                                 CY_RETARGET_IO_BAUDRATE);
    CY_ASSERT(result == CY_RSLT_SUCCESS);

    /* To avoid compiler warning */
    (void)result;

    /* Enable global interrupts. */
    __enable_irq();

    /* \x1b[2J\x1b[;H - ANSI ESC sequence for clear screen */
    printf("\x1b[2J\x1b[;H");

    printf("**********************************************************\r\n");
    printf("software timer period change demo, chap 4, 161204.pdf \r\n");
    printf("**********************************************************\r\n");

    /* In a multitasking system there is potential for error if one task starts to access a resource:
     * - Accessing Peripherals
     * - Read, Modify, Write Operations: In ARM, variable is first read from memory into a register,
     *   modified within the register, and then written back to memory
     * - Non-atomic Access to Variables: Updating multiple members of a structure, or updating a
     *   variable that is larger than the natural word size of the architecture (for example,
     *   updating a 32-bit variable on a 16-bit machine), are examples of non-atomic operations. If
     *   they are interrupted, they can result in data loss or corruption.
     * - Function Reentrancy: A function is ‘reentrant’ if it is safe to call the function from more
     *   than one task, or from both tasks and interrupts. Reentrant functions are said to be ‘thread
     *   safe’ because they can be accessed from more than one thread of execution without the risk
     *   of data or logical operations becoming corrupted. Each task maintains its own stack and its
     *   own set of processor (hardware) register values. If a function does not access any data other
     *   than data stored on the stack or held in a register, then the function is reentrant, and
     *   thread safe. Listing 112 is an example of a reentrant function. Listing 113 is an example of
     *   a function that is not reentrant.
     */

    /* To ensure data consistency is maintained at all times access to a resource that is shared
     * between tasks, or between tasks and interrupts, must be managed using a ‘mutual exclusion’
     * technique. The goal is to ensure that, once a task starts to access a shared resource that is
     * not re-entrant and not thread-safe, the same task has exclusive access to the resource until
     * the resource has been returned to a consistent state. */

    /* Basic Critical Sections */

    /* Basic critical sections are regions of code that are surrounded by calls to the macros
     * taskENTER_CRITICAL() and taskEXIT_CRITICAL(), respectively. Critical sections are also known
     * as critical regions. taskENTER_CRITICAL() and taskEXIT_CRITICAL() do not take any parameters,
     * or return a value. */

    /* Critical sections implemented in this way are a very crude method of providing mutual
     * exclusion. They work by disabling interrupts, either completely, or up to the interrupt
     * priority set by configMAX_SYSCALL_INTERRUPT_PRIORITY—depending on the FreeRTOS port being used.
     * Pre-emptive context switches can occur only from within an interrupt, so, as long as
     * interrupts remain disabled, the task that called taskENTER_CRITICAL() is guaranteed to remain
     * in the Running state until the critical section is exited. Basic critical sections must be
     * kept very short, otherwise they will adversely affect interrupt response times. Every call to
     * taskENTER_CRITICAL() must be closely paired with a call to taskEXIT_CRITICAL(). */

    /* It is safe for critical sections to become nested, because the kernel keeps a count of the
     * nesting depth. The critical section will be exited only when the nesting depth returns to
     * zero — which is when one call to taskEXIT_CRITICAL() has been executed for every preceding
     * call to taskENTER_CRITICAL(). Calling taskENTER_CRITICAL() and taskEXIT_CRITICAL() is the only
     * legitimate way for a task to alter the interrupt enable state of the processor on which
     * FreeRTOS is running. Altering the interrupt enable state by any other means will invalidate
     * the macro’s nesting count. taskENTER_CRITICAL() and taskEXIT_CRITICAL() do not end in ‘FromISR’,
     * so must not be called from an interrupt service routine. taskENTER_CRITICAL_FROM_ISR() is an
     * interrupt safe version of taskENTER_CRITICAL(), and taskEXIT_CRITICAL_FROM_ISR() is an
     * interrupt safe version of taskEXIT_CRITICAL(). The interrupt safe versions are only provided
     * for FreeRTOS ports that allow interrupts to nest—they would be obsolete in ports that do not
     * allow interrupts to nest. taskENTER_CRITICAL_FROM_ISR() returns a value that must be passed
     * into the matching call to taskEXIT_CRITICAL_FROM_ISR(). */

    /* It is wasteful to use more processing time executing the code that enters and then
     * subsequently exits a critical section, than executing the code actually being protected by the
     * critical section. Basic critical sections are very fast to enter, very fast to exit, and
     * always deterministic, making their use ideal when the region of code being protected is very
     * short. */

    /* Suspending (or Locking) the Scheduler */

    /* Critical sections can also be created by suspending the scheduler. Suspending the scheduler is
     * sometimes also known as ‘locking’ the scheduler. Basic critical sections protect a region of
     * code from access by other tasks and by interrupts. A critical section implemented by
     * suspending the scheduler only protects a region of code from access by other tasks, because
     * interrupts remain enabled. */

    /* A critical section that is too long to be implemented by simply disabling interrupts can,
     * instead, be implemented by suspending the scheduler. However, interrupt activity while the
     * scheduler is suspended can make resuming (or ‘un-suspending’) the scheduler a relatively long
     * operation, so consideration must be given to which is the best method to use in each case. */

    /* The scheduler is suspended by calling vTaskSuspendAll(). Suspending the scheduler prevents a
     * context switch from occurring, but leaves interrupts enabled. If an interrupt requests a
     * context switch while the scheduler is suspended, then the request is held pending, and is
     * performed only when the scheduler is resumed (un-suspended). */

    /* The scheduler is resumed (un-suspended) by calling xTaskResumeAll(). */

    /* It is safe for calls to vTaskSuspendAll() and xTaskResumeAll() to become nested, because the
     * kernel keeps a count of the nesting depth. The scheduler will be resumed only when the nesting
     * depth returns to zero—which is when one call to xTaskResumeAll() has been executed for every
     * preceding call to vTaskSuspendAll(). */

	/* Should never get here. */
    CY_ASSERT(0);
}

/* [] END OF FILE */
