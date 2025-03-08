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
			 vTaskDelete(NULL);
		 }
	}
}

void vApplicationIdleHook(void)
{
	while(1)
	{
		//Cy_GPIO_Inv(LED_RGB_RED_PORT,LED_RGB_RED_PIN);
		//vTaskDelay(10);
		printf("idle task\r\n");
	}
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
    printf("Basic task description and task delay, chap 3, 161204.pdf \r\n");
    printf("**********************************************************\r\n");

    /* Each task is a small program in its own right. It has an entry point, will normally run
     * forever within an infinite loop, and will not exit. FreeRTOS tasks must not be allowed to
     * return from their implementing function in any way. They must not contain a ‘return’ statement
     * and must not be allowed to execute past the end of the function. If a task is no longer
     * required, it should instead be explicitly deleted. Generally, idle thread removes allocated
     * stack space for TCB for deleted task. But it doesn't delete user allocated stack space. It has
     * to be deleted manually. */

    /* Only memory allocated to a task by the kernel itself will be freed automatically when the task
     * is deleted. Any memory or other resource that the implementation of the task allocated must be
     * freed explicitly. */

    /* A single task function definition can be used to create any number of tasks. Each created task
     * being a separate execution instance, with its own stack and its own copy of any automatic
     * (stack) variables defined within the task itself. */

    /* A task transitioned from the Not Running state to the Running state is said to have been
     * ‘switched in’ or ‘swapped in’. Conversely, a task transitioned from the Running state to the
     * Not Running state is said to have been ‘switched out’ or ‘swapped out’. The FreeRTOS scheduler
     * is the only entity that can switch a task in and out. */

    /* refer Figure 15. Full task state machine at 93 page */

    /* The task that is actually running (using processing time) is in the Running state. On a single
     * core processor there can only be one task in the Running state at any given time. */

    /* Use vTaskDelay() over normal delay ( just for loop etc. ) so that task having delay can go in
     * blocked state for specified time and meanwhile processor can pick other / low priority tasks
     * instead of just wasting time in executing loop
     */

    /* Difference between vTaskDelay() and vTaskDelayUntil()
     * - https://www.udemy.com/course/mastering-rtos-hands-on-with-freertos-arduino-and-stm32fx/learn/lecture/25694570#overview
     * Suppose we use vTaskDelay() in one task and when some first instructions of task get executed,
     * one high priority task comes in ready state and preempts this task. Then vTaskDelay() will get
     * executed only when this task will be running again. So, actual delay will be grater than the
     * wanted and task will be phase shifted. To avoid this issue, vTaskDelayUntil() can be used
     * which will run/move the task into ready state at after each exact specified intervals and
     * thus removing phase shift. Look into Udemy video for more details. */

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
