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
static volatile void * usrHeapAddr=NULL;
static volatile bool usrMsgRcvd=false;
static volatile char usr_str[200]="";

/*******************************************************************************
* Function Prototypes
********************************************************************************/
void displayString(void *argument)
{
	while(1)
	{
		Cy_GPIO_Inv(LED_ORANGE_PORT,LED_ORANGE_PIN);

		if(usrMsgRcvd == true)
		{
			printf("Received string=%s\r\n\n",usrHeapAddr);

			vPortFree(usrHeapAddr);

			usrMsgRcvd = false;
		}
	}
}

void inputString(void *argument)
{
	while(1)
	{
	    /* Standard library functions can be used but would cause some events missing because of
	     * bigger processing time of all underlying calls and would miss some data because of that. */
		scanf("%s",usr_str);

		/* usrMsgRcvd check is required otherwise usrHeapAddr may get updated causing incorrect output */
		if(usrMsgRcvd == false)
		{
			usrHeapAddr = pvPortMalloc(strlen(usr_str)+1);
			printf("usrHeapAddr=%p\r\n",usrHeapAddr);

			if(usrHeapAddr != NULL)
			{
				memcpy(usrHeapAddr,usr_str,strlen(usr_str));
				*((char *)(usrHeapAddr+strlen(usr_str)))='\0';
				usrMsgRcvd=true;
			}
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
    printf("Challenge: create two separate tasks. One listens for input over UART (from the Serial\r\n");
    printf("Monitor). Upon receiving a newline character (\‘\\n\’), the task allocates a new section\r\n");
    printf("of heap memory (using pvPortMalloc()) and stores the string up to the newline character\r\n");
    printf("in that section of heap. It then notifies the second task that a message is ready. The\r\n");
    printf("second task waits for notification from the first task. When it receives that\r\n");
    printf("notification, it prints the message in heap memory to the Serial Monitor. Finally, it\r\n");
    printf("deletes the allocated heap memory (using vPortFree()).\r\n");
    printf("- Video: https://www.youtube.com/watch?v=Qske3yZRW5I&list=PLEBQazB0HUyQ4hAPU1cJED6t3DU0h34bz&index=4&ab_channel=DigiKey\r\n");
    printf("- Blog: https://www.digikey.in/en/maker/projects/introduction-to-rtos-solution-to-part-4-memory-management/6d4dfcaa1ff84f57a2098da8e6401d9c\r\n");
    printf("*****************************************************************************************************************************************************\r\n");

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
