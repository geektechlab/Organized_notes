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
    printf("task scheduling description, chap 3, 161204.pdf \r\n");
    printf("**********************************************************\r\n");

    /* scheduling algorithm can be changed using the configUSE_PREEMPTION and configUSE_TIME_SLICING
     * configuration constants defined in FreeRTOSConfig.h. */

    /* A third configuration constant, configUSE_TICKLESS_IDLE, also affects the scheduling algorithm,
     * as its use can result in the tick interrupt being turned off completely for extended periods.
     * configUSE_TICKLESS_IDLE is an advanced option provided specifically for use in applications
     * that must minimize their power consumption. configUSE_TICKLESS_IDLE is described in Chapter 10,
     * Low Power Support. */

    /* In all possible configurations the FreeRTOS scheduler will ensure tasks that share a priority
     * are selected to enter the Running state in turn. This ‘take it in turn’ policy is often
     * referred to as ‘Round Robin Scheduling’. A Round Robin scheduling algorithm does not
     * guarantee time is shared equally between tasks of equal priority, only that Ready state tasks
     * of equal priority will enter the Running state in turn. This is because any task may get
     * blocked or may change priority ? is it the reason ? */

    /* - Fixed Priority Scheduling algorithms described as ‘Fixed Priority’ do not change the
     * priority assigned to the tasks being scheduled, but also do not prevent the tasks themselves
     * from changing their own priority, or that of other tasks.
     *
     * - Pre-emptive Pre-emptive scheduling algorithms will immediately ‘pre-empt’ the Running state
     * task if a task that has a priority higher than the Running state task enters the Ready state.
     * Being pre-empted means being involuntarily (without explicitly yielding or blocking) moved out
     * of the Running state and into the Ready state to allow a different task to enter the Running
     * state.
     *
     * - Time Slicing Time slicing is used to share processing time between tasks of equal priority,
     * even when the tasks do not explicitly yield or enter the Blocked state. Scheduling algorithms
     * described as using ‘Time Slicing’ will select a new task to enter the Running state at the
     * end of each time slice if there are other Ready state tasks that have the same priority as
     * the Running task. A time slice is equal to the time between two RTOS tick interrupts. */

    /* Prioritized Pre-emptive Scheduling with Time Slicing
     * - configUSE_PREEMPTION 1
     * - configUSE_TIME_SLICING 1
     *
     * if there is some application task having same priority as idle task, then
     * configIDLE_SHOULD_YIELD compile time configuration constant can be used to change how the Idle
     * task is scheduled:
     * - If configIDLE_SHOULD_YIELD is set to 0 then the Idle task will remain in the Running state
     * for the entirety of its time slice, unless it is preempted by a higher priority task.
     * - If configIDLE_SHOULD_YIELD is set to 1 then the Idle task will yield (voluntarily give up
     * whatever remains of its allocated time slice) on each iteration of its loop if there are other
     * Idle priority tasks in the Ready state.
     *
     * Figure 26, 27, 28
     */

    /* Prioritized Pre-emptive Scheduling (without Time Slicing)
     * Prioritized Preemptive Scheduling without time slicing maintains the same task selection and
     * pre-emption algorithms as described in the previous algorithm, but does not use time slicing to
     * share processing time between tasks of equal priority.
     *
     * - configUSE_PREEMPTION 1
     * - configUSE_TIME_SLICING 0
     *
     * in previous algorithm if time slicing is used, and there is more than one ready state task at
     * the highest priority that is able to run, then the scheduler will select a new task to enter
     * the Running state during each RTOS tick interrupt (a tick interrupt marking the end of a time
     * slice).
     *
     * If time slicing is not used, then the scheduler will only select a new task to enter the
     * Running state when either:
     * - A higher priority task enters the Ready state.
     * - The task in the Running state enters the Blocked or Suspended state.
     *
     * There are fewer task context switches when time slicing is not used than when time slicing is
     * used. Therefore, turning time slicing off results in a reduction in the scheduler’s processing
     * overhead. However, turning time slicing off can also result in tasks of equal priority
     * receiving greatly different amounts of processing time, a scenario demonstrated by Figure 29.
     *
     * Figure 29 Execution pattern that demonstrates how tasks of equal priority can receive hugely
     * different amounts of processing time when time slicing is not used
     */

    /* Co-operative Scheduling
     *
     * - configUSE_PREEMPTION 0
     * - configUSE_TIME_SLICING Any value
     *
     * When the co-operative scheduler is used, a context switch will only occur when the Running
     * state task enters the Blocked state, or the Running state task explicitly yields (manually
     * requests a re-schedule) by calling taskYIELD(). Tasks are never pre-empted, so time slicing
     * cannot be used.
     *
     * Figure 30 Execution pattern demonstrating the behavior of the co-operative scheduler
     *
     * In a multi-tasking application the application writer must take care that a resource is not
     * accessed by more than one task simultaneously, as simultaneous access could corrupt the
     * resource. For example, UART.
     *
     * It is normally easier to avoid problems caused by simultaneous access when the co-operative
     * scheduler is used than when the pre-emptive scheduler is used ( instead use mutex, sempahore etc. ) :
     * - When the pre-emptive scheduler is used the Running state task can be pre-empted at any time,
     * including when a resource it is sharing with another task is in an inconsistent state. As just
     * demonstrated by the UART example, leaving a resource in an inconsistent state can result in
     * data corruption.
     * - When the co-operative scheduler is used the application writer controls when a switch to
     * another task can occur. The application writer can therefore ensure a switch to another task
     * does not occur while a resource is in an inconsistent state.
     * - In the above UART example, the application writer can ensure Task 1 does not leave the
     * Running state until its entire string has been written to the UART, and in doing so,removing
     * the possibility of the string being corrupted by the activates of another task.
     *
     * As demonstrated in Figure 30, systems will be less responsive when the co-operative scheduler
     * is used than when the pre-emptive scheduler is used:
     * - When the pre-emptive scheduler is used the scheduler will start running a task immediately
     * that the task becomes the highest priority Ready state task. This is often essential in
     * real-time systems that must respond to high priority events within a defined time period.
     * - When the co-operative scheduler is used a switch to a task that has become the highest
     * priority Ready state task is not performed until the Running state task enters the Blocked
     * state or calls taskYIELD().
     */

    while(1){}

    /* Should never get here. */
    CY_ASSERT(0);
}

/* [] END OF FILE */
