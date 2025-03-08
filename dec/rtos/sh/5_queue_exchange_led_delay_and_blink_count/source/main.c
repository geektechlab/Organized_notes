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
#include "queue.h"

/*******************************************************************************
* Macros
*******************************************************************************/
#define TASK_STACK_SIZE        (1024*10)
#define TASK_PRIORITY          (configMAX_PRIORITIES - 3)
#define CLOCK_100_MHZ               (100000000u)
/*******************************************************************************
* Function Prototypes
********************************************************************************/
static QueueHandle_t ledCmdQueue = NULL;
static QueueHandle_t ledCmdQueue2 = NULL;

static volatile void * usrHeapAddr=NULL;
static volatile bool usrMsgRcvd=false;
static volatile char usr_str[200]="";

/*******************************************************************************
* Function Prototypes
********************************************************************************/
void displayString(void *argument)
{
	volatile uint32_t blinkRate=50;
	uint32_t tickCount=0;
	while(1)
	{
		if(xQueueReceive(ledCmdQueue, &blinkRate, 0) == pdTRUE)
		{
			tickCount=0;
		}

		Cy_GPIO_Inv(LED_ORANGE_PORT,LED_ORANGE_PIN);
		tickCount++;

		if((tickCount%100)==0)
		{
			xQueueSend(ledCmdQueue2, &tickCount, 0);
		}

		vTaskDelay(blinkRate);
	}
}

void inputString(void *argument)
{
	uint32_t blinkRate=0;
	volatile uint32_t tickCount=0;
	while(1)
	{
	    /* Standard library functions can be used but would cause some events missing because of
	     * bigger processing time of all underlying calls and would miss some data because of that. */

		/* It is a blocking function, so till it gets user input it will block displaying tickCount
		 * from other task.
		 */
		scanf("%s",usr_str);

		if (strncmp(usr_str, "delay", 5) == 0)
		{
			scanf("%s",usr_str);
			blinkRate=atoi(usr_str);

			xQueueSend(ledCmdQueue, &blinkRate, 0);
		}

		if(xQueueReceive(ledCmdQueue2, &tickCount, 0) == pdTRUE)
		{
			printf("tickCount=%d\r\n",tickCount);
		}
	}
}

void vApplicationIdleHook(void)
{
	while(1)
	{
		printf("1_sh idle task\r\n");
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

    printf("*****************************************************************************************************************************************************\r\n");
    printf("Challenge: One task performs basic echo on Serial. If it sees \"delay\" followed by a number, it sends the number (in a queue) to the second task.\r\n");
    printf("If it receives a message in a second queue, it prints it to the console. The second task blinks an LED. When it gets a message from the first queue\r\n");
    printf("(number), it updates the blink delay to that number. Whenever the LED blinks 100 times, the second task sends a message to the first task to\r\n");
    printf("be printed.\r\n");
    printf("- Video: https://www.youtube.com/watch?v=pHJ3lxOoWeI&list=PLEBQazB0HUyQ4hAPU1cJED6t3DU0h34bz&index=5&ab_channel=DigiKey\r\n");
    printf("- Blog: https://www.digikey.in/en/maker/projects/introduction-to-rtos-solution-to-part-5-freertos-queue-example/72d2b361f7b94e0691d947c7c29a03c9\r\n");
    printf("*****************************************************************************************************************************************************\r\n");

    /* We prefer queue over global variable because it global variable can hold only one item and can
     * be used between two tasks only. Because third task may update the value in between. Also suppose,
     * global variable is a big structure and one task might be updating it and in between other task
     * preemts and writes into it corrupting the variable. Mutex/semaphores/atomic operations also
     * can fix this.*/
    ledCmdQueue = xQueueCreate(2, sizeof(uint32_t));
    CY_ASSERT(ledCmdQueue != NULL);

    ledCmdQueue2 = xQueueCreate(2, sizeof(uint32_t));
    CY_ASSERT(ledCmdQueue2 != NULL);

    /* Create the task */
	if (xTaskCreate(inputString, "inputString", TASK_STACK_SIZE, NULL, TASK_PRIORITY, NULL) == pdPASS)
	{
		if (xTaskCreate(displayString, "displayString", TASK_STACK_SIZE, NULL, TASK_PRIORITY, NULL) == pdPASS)
		{
			/* Start the FreeRTOS scheduler. */
			vTaskStartScheduler();
		}
	}

	/* Should never get here. */
    CY_ASSERT(0);
}

/* [] END OF FILE */
