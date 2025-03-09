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
#include "event_groups.h"

/*******************************************************************************
* Macros
*******************************************************************************/
#define TASK_STACK_SIZE        (1024*10)
#define TASK_PRIORITY          (configMAX_PRIORITIES - 3)
#define CLOCK_100_MHZ          (100000000u)

/*******************************************************************************
* Function Prototypes
********************************************************************************/
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
    printf("task notifications, chap 9, 161204.pdf \r\n");
    printf("**********************************************************\r\n");

    /* It has been seen that applications that use FreeRTOS are structured as a set of independent
     * tasks, and that it is likely that these autonomous tasks will have to communicate with each
     * other so that, collectively, they can provide useful system functionality.  */

    /* Examples of communication objects include queues, event groups, and various different types of
     * semaphore. When a communication object is used, events and data are not sent directly to a
     * receiving task, or a receiving ISR, but are instead sent to the communication object. Likewise,
     * tasks and ISRs receive events and data from the communication object, rather than directly
     * from the task or ISR that sent the event or data. This is depicted in Figure 76. */

    /* ‘Task Notifications’ allow tasks to interact with other tasks, and to synchronize with ISRs,
     * without the need for a separate communication object. By using a task notification, a task or
     * ISR can send an event directly to the receiving task. This is depicted in Figure 77. */

    /* Task notification functionality is optional. To include task notification functionality set
     * configUSE_TASK_NOTIFICATIONS to 1 in FreeRTOSConfig.h. When configUSE_TASK_NOTIFICATIONS is
     * set to 1, each task has a ‘Notification State’, which can be either ‘Pending’ or ‘Not-Pending’,
     * and a ‘Notification Value’, which is a 32-bit unsigned integer. When a task receives a
     * notification, its notification state is set to pending. When a task reads its notification
     * value, its notification state is set to not-pending. A task can wait in the Blocked state,
     * with an optional time out, for its notification state to become pending. */

    /* Performance Benefits of Task Notifications
     * Using a task notification to send an event or data to a task is significantly faster than
     * using a queue, semaphore or event group to perform an equivalent operation. */

    /* RAM Footprint Benefits of Task Notifications
     * Likewise, using a task notification to send an event or data to a task requires significantly
     * less RAM than using a queue, semaphore or event group to perform an equivalent operation. This
     * is because each communication object (queue, semaphore or event group) must be created before
     * it can be used, whereas enabling task notification functionality has a fixed overhead of just
     * eight bytes of RAM per task. */

    /* Limitations of Task Notifications
     * Task notifications are faster and use less RAM than communication objects, but task
     * notifications cannot be used in all scenarios. This section documents the scenarios in
     * which a task notification cannot be used:
     * - Sending an event or data to an ISR
     * 		Communication objects can be used to send events and data from an ISR to a task, and
     * 		from a task to an ISR. Task notifications can be used to send events and data from an ISR
     * 		to a task, but they cannot be used to send events or data from a task to an ISR.
     * - Enabling more than one receiving task
     * 		A communication object can be accessed by any task or ISR that knows its handle (which
     * 		might be a queue handle, semaphore handle, or event group handle). Any number of tasks
     * 		and ISRs can process events or data sent to any given communication object. Task
     * 		notifications are sent directly to the receiving task, so can only be processed by the
     * 		task to which the notification is sent. However, this is rarely a limitation in practical cases
     * 		because, while it is common to have multiple tasks and ISRs sending to the same
     * 		communication object, it is rare to have multiple tasks and ISRs receiving from the same
     * 		communication object.
     * - Buffering multiple data items
     * 		A queue is a communication object that can hold more than one data item at a time. Data
     * 		that has been sent to the queue, but not yet received from the queue, is buffered inside
     * 		the queue object. Task notifications send data to a task by updating the receiving task’s
     * 		notification value. A task’s notification value can only hold one value at a time.
     * - Broadcasting to more than one task
     * 		An event group is a communication object that can be used to send an event to more than
     * 		one task at a time. Task notifications are sent directly to the receiving task, so can
     * 		only be processed by the receiving task.
     * - Waiting in the blocked state for a send to complete
     * 		If a communication object is temporarily in a state that means no more data or events can
     * 		be written to it (for example, when a queue is full no more data can be sent to the queue),
     * 		then tasks attempting to write to the object can optionally enter the Blocked state to
     * 		wait for their write operation to complete. If a task attempts to send a task
     * 		notification to a task that already has a notification pending, then it is not possible
     * 		for the sending task to wait in the Blocked state for the receiving task to reset its
     * 		notification state. As will be seen, this is rarely a limitation in practical cases in
     * 		which a task notification is used. */

	/* /* If all is well then main() will never reach here as the scheduler will now be running the
	 * tasks. If main() does reach here then it is likely that there was insufficient FreeRTOS heap
	 * memory available for the idle task to be created. Chapter 2 provides more information on heap
	 * memory management. */

	/* Should never get here. */
    CY_ASSERT(0);
}

/* [] END OF FILE */
