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

#define RED_LED_MASK 0x0001
#define BLUE_LED_MASK 0x0002
#define GREEN_LED_MASK 0x0004

static TaskHandle_t recvTaskHandle = NULL;

/*******************************************************************************
* Function Prototypes
********************************************************************************/
static void recvTask( void* NotUsed )
{
	uint32_t notificationvalue = 0;
	while(1)
	{
		/* wait for the next notification value, clearing it to 0 after receiving */
		notificationvalue = ulTaskNotifyTake( pdTRUE, portMAX_DELAY );
		if((notificationvalue & RED_LED_MASK) != 0)
			Cy_GPIO_Clr(LED_RGB_RED_PORT,LED_RGB_RED_PIN);
		else
			Cy_GPIO_Set(LED_RGB_RED_PORT,LED_RGB_RED_PIN);

		if((notificationvalue & BLUE_LED_MASK) != 0)
			Cy_GPIO_Clr(LED_RGB_BLUE_PORT,LED_RGB_BLUE_PIN);
		else
			Cy_GPIO_Set(LED_RGB_BLUE_PORT,LED_RGB_BLUE_PIN);

		if((notificationvalue & GREEN_LED_MASK) != 0)
			Cy_GPIO_Clr(LED_RGB_GREEN_PORT,LED_RGB_GREEN_PIN);
		else
			Cy_GPIO_Set(LED_RGB_GREEN_PORT,LED_RGB_GREEN_PIN);
	}
}

void sendingTask( void* NotUsed )
{
	while(1)
	{
		/* send the notification to recvTask - turning on the Red LED
		 * since we're overwriting the value, even if another notification is pending
		 * there is no need to check the return value (see docs for xTaskNofify in header) */
		xTaskNotify( recvTaskHandle, RED_LED_MASK, eSetValueWithOverwrite);
		vTaskDelay(200);

		xTaskNotify( recvTaskHandle, BLUE_LED_MASK, eSetValueWithOverwrite);
		vTaskDelay(200);

		xTaskNotify( recvTaskHandle, GREEN_LED_MASK, eSetValueWithOverwrite);
		vTaskDelay(200);

		xTaskNotify( recvTaskHandle, RED_LED_MASK | BLUE_LED_MASK | GREEN_LED_MASK, eSetValueWithOverwrite);
		vTaskDelay(200);

		xTaskNotify( recvTaskHandle,0, eSetValueWithOverwrite);
		vTaskDelay(200);
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
    printf("task notifications, chap 9, 161204.pdf \r\n");
    printf("**********************************************************\r\n");

    /* Task notifications are a very powerful feature that can often be used in place of a binary
     * semaphore, a counting semaphore, an event group, and sometimes even a queue. This wide range
     * of usage scenarios can be achieved by using the xTaskNotify() API function to send a task
     * notification, and the xTaskNotifyWait() API function to receive a task notification. However,
     * in the majority of cases, the full flexibility provided by the xTaskNotify() and
     * xTaskNotifyWait() API functions is not required, and simpler functions would suffice.
     * Therefore, the xTaskNotifyGive() API function is provided as a simpler but less flexible
     * alternative to xTaskNotify(), and the ulTaskNotifyTake() API function is provided as a
     * simpler but less flexible alternative to xTaskNotifyWait(). */

    /* xTaskNotifyGive() sends a notification directly to a task, and increments (adds one to) the
     * receiving task’s notification value. Calling xTaskNotifyGive() will set the receiving task’s
     * notification state to pending, if it was not already pending. The xTaskNotifyGive() API (
     * actually macro ) function is provided to allow a task notification to be used as a lighter
     * weight and faster alternative to a binary or counting semaphore. vTaskNotifyGiveFromISR() is a
     * version of xTaskNotifyGive() that can be used in an interrupt service routine.*/

    /* ulTaskNotifyTake() allows a task to wait in the Blocked state for its notification value to be
     * greater than zero, and either decrements (subtracts one from) or clears the task’s
     * notification value before it returns. The ulTaskNotifyTake() API function is provided to allow
     * a task notification to be used as a lighter weight and faster alternative to a binary or
     * counting semaphore. If xClearCountOnExit argument is set to pdTRUE, then the calling task’s
     * notification value will be cleared to zero before the call to ulTaskNotifyTake() returns. If
     * xClearCountOnExit is set to pdFALSE, and the calling task’s notification value is greater than
     * zero, then the calling task’s notification value will be decremented before the call to
     * ulTaskNotifyTake() returns. The returned value is the calling task’s notification value before
     * it was either cleared to zero or decremented, as specified by the value of the
     * xClearCountOnExit parameter. If a block time was specified (xTicksToWait was not zero), and
     * the return value is not zero, then it is possible the calling task was placed into the Blocked
     * state, to wait for its notification value to be greater than zero, and its notification value
     * was updated before the block time expired. If a block time was specified (xTicksToWait was not
     * zero), and the return value is zero, then the calling task was placed into the Blocked state,
     * to wait for its notification value to be greater than zero, but the specified block time
     * expired before that happened. */

    /* IMP: Example 24. Using a task notification in place of a semaphore, method 1 */

    /* IMP: Example 25. Using a task notification in place of a semaphore, method 2 */

    /* Very IMP: Read from 334 - 353 */

	if ( xTaskCreate(recvTask, "recvTask", TASK_STACK_SIZE, NULL, 2, &recvTaskHandle) == pdPASS)
	{
		if ( xTaskCreate(sendingTask, "sendingTask", TASK_STACK_SIZE, NULL, 1, NULL) == pdPASS)
		{
			/* Start the FreeRTOS scheduler. */
			vTaskStartScheduler();
		}
	}

	/* /* If all is well then main() will never reach here as the scheduler will now be running the
	 * tasks. If main() does reach here then it is likely that there was insufficient FreeRTOS heap
	 * memory available for the idle task to be created. Chapter 2 provides more information on heap
	 * memory management. */

	/* Should never get here. */
    CY_ASSERT(0);
}

/* [] END OF FILE */
