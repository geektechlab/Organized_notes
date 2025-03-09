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
/*******************************************************************************
* Macros
*******************************************************************************/
#define TASK_STACK_SIZE        (1024*10)
#define TASK_PRIORITY          (configMAX_PRIORITIES - 3)
#define CLOCK_100_MHZ               (100000000u)
/*******************************************************************************
* Function Prototypes
********************************************************************************/
TaskHandle_t RedTaskHandle;

static volatile uint32_t count=0UL;
static volatile uint32_t idle_count=0UL;

/*******************************************************************************
* Function Prototypes
********************************************************************************/
void RedTask(void *argument)
{
	TickType_t xLastWakeTime;
	while(1)
	{
		 xLastWakeTime = xTaskGetTickCount();

		Cy_GPIO_Inv(LED_RGB_RED_PORT,LED_RGB_RED_PIN);

		/* This task should execute every 250 milliseconds exactly. As per
		 the vTaskDelay() function, time is measured in ticks, and the
		 pdMS_TO_TICKS() macro is used to convert milliseconds into ticks.
		 xLastWakeTime is automatically updated within vTaskDelayUntil(), so is not
		 explicitly updated by the task. */
		 vTaskDelayUntil( &xLastWakeTime, pdMS_TO_TICKS( 250 ) );

		 count++;
		 if(count>6UL)
		 {
			 printf("Red task, idle count=%d\r\n",idle_count);
			 vTaskDelete(NULL);
		 }
	}
}

void GreenTask(void *argument)
{
	while(1)
	{
		Cy_GPIO_Inv(LED_RGB_GREEN_PORT,LED_RGB_GREEN_PIN);

		vTaskDelay(pdMS_TO_TICKS( 250 ));

		 count++;
		 if(count>6UL)
		 {
			 printf("Green task, idle count=%d\r\n",idle_count);
			 vTaskDelete(NULL);
		 }
	}
}

void vApplicationIdleHook(void)
{
	idle_count++;
	//printf("idle task, idle count=%d\r\n",idle_count);
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
    printf("Idle task, chap 3, 161204.pdf \r\n");
    printf("**********************************************************\r\n");

    /* There must always be at least one task that can enter the Running state. To ensure this is the
     * case, an Idle task is automatically created by the scheduler when vTaskStartScheduler() is
     * called. */

    /* The idle task has the lowest possible priority (priority zero), to ensure it never prevents a
     * higher priority application task from entering the Running state. Although, it can be created
     * at higher priority. Running at the lowest priority ensures the Idle task is transitioned out
     * of the Running state as soon as a higher priority task enters the Ready state. */

    /* If an application uses the vTaskDelete() API function then it is essential that the Idle task
     * is not starved of processing time. This is because the Idle task is responsible for cleaning
     * up kernel resources after a task has been deleted */

    /* It is possible to add application specific functionality directly into the idle task through
     * the use of an idle hook (or idle callback) function—a function that is called automatically
     * by the idle task once per iteration of the idle task loop. */

    /* Common uses for the Idle task hook include:
     * - Executing low priority, background, or continuous processing functionality.
     * - Measuring the amount of spare processing capacity. (The idle task will run only when all
     * higher priority application tasks have no work to perform; so measuring the amount
     * of processing time allocated to the idle task provides a clear indication of how much
     * processing time is spare.)
     * - Placing the processor into a low power mode, providing an easy and automatic method of
     * saving power whenever there is no application processing to be performed (although the power
     * saving that can be achieved using this method is less than can be achieved by using the
     * tick-less idle mode described in Chapter 10, Low Power Support). How RTOS ensures task delay
     * when idle task goes into sleep/deep sleep ? how it manages to wakeup / by systick
     * interrupt ?
     */

    /* Idle task hook functions must adhere to the following rules:
     * - An Idle task hook function must never attempt to block or suspend. Note: Blocking the idle
     * task in any way could cause a scenario where no tasks are available to enter the Running state.
     * - If the application makes use of the vTaskDelete() API function, then the Idle task hook must
     * always return to its caller within a reasonable time period. This is because the Idle task is
     * responsible for cleaning up kernel resources after a task has been deleted. If the idle task
     * remains permanently in the Idle hook function, then this clean-up cannot occur. */
    if (xTaskCreate(GreenTask, "GreenTask", TASK_STACK_SIZE, NULL, TASK_PRIORITY, NULL) == pdPASS)
	{
		if (xTaskCreate(RedTask, "RedTask", TASK_STACK_SIZE, NULL, TASK_PRIORITY, NULL) == pdPASS)
		{
			/* Start the FreeRTOS scheduler. */
			vTaskStartScheduler();
		}
	}

	/* Should never get here. */
    CY_ASSERT(0);
}

/* [] END OF FILE */
