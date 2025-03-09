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
TaskHandle_t GreenTaskHandle;

static volatile uint32_t count=0UL;
static volatile uint32_t idle_count=0UL;

/*******************************************************************************
* Function Prototypes
********************************************************************************/
void RedTask(void *argument)
{
	UBaseType_t uxPriority;
	while(1)
	{
		uxPriority = uxTaskPriorityGet(NULL);
		Cy_GPIO_Inv(LED_RGB_RED_PORT,LED_RGB_RED_PIN);

		//vTaskDelay(pdMS_TO_TICKS( 250 ));

		printf("Red Task current priority=%d\r\n", uxPriority);
		printf("Lowering Red task priority\r\n");
		vTaskPrioritySet( NULL, ( uxPriority - 2 ) );

		/* After lowering red task priority, green task will be executed immediately and this print
		 * will come after green task.
		 */
		printf("After lowering red task priority, executed green task and came back\r\n");
	}
}

void GreenTask(void *argument)
{
	UBaseType_t uxPriority;
	while(1)
	{
		uxPriority = uxTaskPriorityGet(NULL);
		Cy_GPIO_Inv(LED_RGB_GREEN_PORT,LED_RGB_GREEN_PIN);

		//vTaskDelay(pdMS_TO_TICKS( 250 ));

		printf("Green Task current priority=%d\r\n", uxPriority);
		printf("Increasing red task priority\r\n");
		vTaskPrioritySet( RedTaskHandle, ( uxTaskPriorityGet(RedTaskHandle) + 2 ) );

		/* After increasing red task priority, red task will be executed immediately and this print
		 * will come after red task.
		 */
		printf("After increasing red task priority, executed red task and came back\r\n");
	}
}

void vApplicationIdleHook(void)
{
	idle_count++;
	printf("idle task, idle count=%d\r\n",idle_count);
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
    printf("task priority, chap 3, 161204.pdf \r\n");
    printf("**********************************************************\r\n");

    /* The scheduler will always select the highest Ready state task as the task to enter the
     * Running state. the task with the highest relative priority will always be the task selected
     * by the scheduler to be in the Running state. Task starts to run (enters the Running state)
     * as soon as it has the highest relative priority.*/

    /* here red task and green task will change each others priorities and always be running, leaving
     * processor never empty. So, idle task will never executed.
     */
    if (xTaskCreate(GreenTask, "GreenTask", TASK_STACK_SIZE, NULL, TASK_PRIORITY+1, &GreenTaskHandle) == pdPASS)
	{
		if (xTaskCreate(RedTask, "RedTask", TASK_STACK_SIZE, NULL, TASK_PRIORITY+2, &RedTaskHandle) == pdPASS)
		{
			/* Start the FreeRTOS scheduler. */
			vTaskStartScheduler();
		}
	}

	/* Should never get here. */
    CY_ASSERT(0);
}

/* [] END OF FILE */
