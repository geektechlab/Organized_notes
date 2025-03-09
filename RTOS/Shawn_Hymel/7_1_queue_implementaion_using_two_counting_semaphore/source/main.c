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
#include "semphr.h"

/*******************************************************************************
* Macros
*******************************************************************************/
#define TASK_STACK_SIZE        (1024*10)
#define TASK_PRIORITY          (configMAX_PRIORITIES - 3)
#define CLOCK_100_MHZ          (100000000u)
/*******************************************************************************
* Function Prototypes
********************************************************************************/
static volatile uint32_t buf[5]={-1,-1,-1,-1,-1};
static uint32_t front=0;
static uint32_t back=0;
SemaphoreHandle_t semPtr_Counting_empty = NULL;
SemaphoreHandle_t semPtr_Counting_filled = NULL;
SemaphoreHandle_t mutexPtr_buff_protect = NULL;

/* We have to use 2 semaphore here: One to signal that producer has filled one item
 * to indicate consumer to consume. Other to indicate consumer has consumed one item
 * which indicates producer it can fill if it wants. Here we are using counting semaphore,
 * because of which we can use buf more than 1. Single counting semaphore will not work because
 * that producer will use that semaphore to indicate buffer is filled but there is no semaphore/indicator to
 * use by consumer to know if some valid data is in buffer. Think if consumer starts reading empty buffer.
 *
 * Here we are initializing semPtr_Counting_empty as 5 so that producer can take it 5 times. Because
 * semaphore will be available 5 times to take it. Whereas, semPtr_Counting_filled initialized as 5
 * so that consumer gets blocked on that until some data is put into buffer.
 */

/*******************************************************************************
* Function Prototypes
********************************************************************************/
void producerTask(void *argument)
{
	static uint32_t var=0;
	while(1)
	{
		xSemaphoreTake(semPtr_Counting_empty, portMAX_DELAY);

		xSemaphoreTake(mutexPtr_buff_protect, portMAX_DELAY);
		buf[front]=var;
		var=(var+1)%5;
		front=((front+1)%5);
		xSemaphoreGive(mutexPtr_buff_protect);

		xSemaphoreGive(semPtr_Counting_filled);
	}
}

void displayTask(void *argument)
{
	while(1)
	{
	    /* Standard library functions can be used but would cause some events missing because of
	     * bigger processing time of all underlying calls and would miss some data because of that. */

		/* It is a blocking function, so till it gets user input it will block displaying tickCount
		 * from other task.
		 */
		xSemaphoreTake(semPtr_Counting_filled, portMAX_DELAY);

		xSemaphoreTake(mutexPtr_buff_protect, portMAX_DELAY);
		printf("Received character=%d by task=%c\r\n",buf[back],(*(char *)argument));
		back=((back+1)%5);
		xSemaphoreGive(mutexPtr_buff_protect);

		xSemaphoreGive(semPtr_Counting_empty);
	}
}

void vApplicationIdleHook(void)
{
	while(1)
	{
		//printf("idle task handler from app\r\n");
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
    printf("Challenge: use a mutex and counting semaphores to protect the shared buffer so that each number (0 to 4) is printed exactly 3 times to the\r\n");
    printf("Serial monitor (in any order). Do not use queues to do this!.\r\n");
    printf("- Video: https://www.youtube.com/watch?v=5JcMtbA9QEE&list=PLEBQazB0HUyQ4hAPU1cJED6t3DU0h34bz&index=17&ab_channel=DigiKey\r\n");
    printf("- Blog: https://www.digikey.in/en/maker/projects/introduction-to-rtos-solution-to-part-7-freertos-semaphore-example/51aa8660524c4daba38cba7c2f5baba7\r\n");
    printf("*****************************************************************************************************************************************************\r\n");

    mutexPtr_buff_protect = xSemaphoreCreateMutex();
    CY_ASSERT(NULL != mutexPtr_buff_protect);
    semPtr_Counting_empty = xSemaphoreCreateCounting( 5, 5 );
	CY_ASSERT(semPtr_Counting_empty != NULL);
	semPtr_Counting_filled = xSemaphoreCreateCounting( 5, 0 );
	CY_ASSERT(semPtr_Counting_filled != NULL);

	/* Create the task */
	if (xTaskCreate(producerTask, "producerTask1", TASK_STACK_SIZE, "1", TASK_PRIORITY, NULL) == pdPASS)
	{
		if (xTaskCreate(producerTask, "producerTask2", TASK_STACK_SIZE, "2", TASK_PRIORITY, NULL) == pdPASS)
		{
			if (xTaskCreate(producerTask, "producerTask3", TASK_STACK_SIZE, "3", TASK_PRIORITY, NULL) == pdPASS)
			{
				if (xTaskCreate(displayTask, "displayTask1", TASK_STACK_SIZE, "1", TASK_PRIORITY, NULL) == pdPASS)
				{
					if (xTaskCreate(displayTask, "displayTask2", TASK_STACK_SIZE, "2", TASK_PRIORITY, NULL) == pdPASS)
					{
						/* Start the FreeRTOS scheduler. */
						vTaskStartScheduler();
					}
				}
			}
		}
	}

	/* Should never get here. */
    CY_ASSERT(0);
}

/* [] END OF FILE */
