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

/*******************************************************************************
* Function Prototypes
********************************************************************************/
void RedTask(void *argument)
{
	while(1)
	{
		Cy_GPIO_Inv(LED_RED_PORT,LED_RED_PIN);
		printf("RedTask\r\n");
		vTaskDelay(100);
		//Cy_SysLib_Delay(1000U);
		//taskYIELD();
	}
}

void GreenTask(void *argument)
{
	volatile int count=0;
	while(1)
	{
		while(count<6)
		{
			Cy_GPIO_Inv(LED_RGB_GREEN_PORT,LED_RGB_GREEN_PIN);
			printf("GreenTask\r\n");
			/* We can't use non-blocking RTOS delay here if we don't want to execute low priority
			 * tasks instead of green task in between */
			//vTaskDelay(100);
			Cy_SysLib_Delay(1000U);
			//taskYIELD();
			count++;
		}

		/* Create RedTask from GreenTask instead of creating from main and before running scheduler */
		if (xTaskCreate(RedTask, "RedTask", TASK_STACK_SIZE, NULL, TASK_PRIORITY, &RedTaskHandle) != pdPASS)
		{
			/* Force assert if RedTask creation fails */
		    configASSERT(true);
		}

		/* deletes itself ( GreenTask ) */
		vTaskDelete(NULL);
	}
}

void OrangeTask(void *argument)
{
	volatile int count=0;
	while(1)
	{
		Cy_GPIO_Inv(LED_ORANGE_PORT,LED_ORANGE_PIN);
		printf("OrangeTask\r\n");
		vTaskDelay(100);
		//Cy_SysLib_Delay(1000U);
		//taskYIELD();
		count++;
		/* deletes RedTask */
		if(count==31)
		{
			/* RedTaskHandle is declared above */
			vTaskDelete(RedTaskHandle);
		}
		if(count==60)
		{
			/* deletes itself ( OrangeTask ) */
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
    printf("task creation example, chap 3, 161204.pdf \r\n");
    printf("**********************************************************\r\n");

    /* Create the task */

    /* pcName: it isa  descriptive name for the task. This is not used by FreeRTOS in any way. It is
     * can be used purely as a debugging aid, for example, SEGGER Systemview. */

    /* usStackDepth: it tells the kernel how large to make the stack. The value specifies the number
     * of words the stack can hold, not the number of bytes. For example, if the stack is 32-bits
     * wide and usStackDepth is passed in as 100, then 400 bytes of stack space will be allocated
     * (100 * 4 bytes). The stack depth multiplied by the stack width must not exceed the maximum
     * value that can be contained in a variable of type uint16_t. The size of the stack used by the
     * Idle task is defined by the application defined constant configMINIMAL_STACK_SIZE. */

    /* uxPriority: it defines the priority at which the task will execute. Priorities can be assigned
     * from 0, which is the lowest priority, to (configMAX_PRIORITIES – 1), which is the highest
     * priority. Passing a uxPriority value above (configMAX_PRIORITIES – 1) will result in the
     * priority assigned to the task being capped silently to the maximum legitimate value. */

    /* If the stack grows down then allocate the stack then the TCB so the stack does not grow into
     * the TCB.  Likewise if the stack grows up then allocate the TCB then the stack. This is because
     * heap allocation is done in increasing order ( grows up ) and stack may grow up/down based on
     * architecture .*/

    /* TCB ( Task control block ) is a structure which holds various parameters related to task. Once,
     * memory allocation for TCB and task stack is successful then TCB pxNewTCB->pxStack is updated
     * with value pointing to task stack location. If not successful, allocated memories are freed.
     */

    /* Then TCB stores an task allocation method type ( static/dynamic ), so that if task gets deleted
     * later, then associated TCB and task stack memories can be freed.
     */

    /* If we are using MPU wrappers, then if we want then we can run our task in privileged mode also.
     * Later, we can store MPU regions' info in TCB structure members.
     */

    /* Then we can fill the task stack with some known value e.g. ( 0xa5 ) */

    /* Check start of stack alignment so that data fetches are aligned with boundary and will take
     * single cycle to load/store data onto the stack. After this calculate end of stack using start
     * of stack address and size of stack and store it in TCB at pxNewTCB->pxEndOfStack.
     */

    /* Then store the name of task in TCB at pxNewTCB->pcTaskName. It will store only upto
     * configMAX_TASK_NAME_LEN characters and remaining characters will be discarded. Generally, task
     * names are useful for debugging purpose such as in SEGGER Systemview software.
     */

    /* Then task priority is stored in TCB at pxNewTCB->uxPriority structure member variable. Value of
     * priority is capped at configMAX_PRIORITIES and if user assigns priority greater than this value
     * then task priority is set to configMAX_PRIORITIES-1 in TCB.
     */

    /* If Mutexes are used then task's base priority is also set to task priority passed by user in
     * TCB at pxNewTCB->uxBasePriority. At task initialization, value of pxNewTCB->uxBasePriority and
     * pxNewTCB->uxPriority are same.
     */

    /* Then state of task ( blocked, ready, running, suspended etc. ) is maintained in pxNewTCB->xStateListItem
     * list ? and it gets initialized. Also, a task event list pxNewTCB->xEventListItem gets
     * initialized.
     */

    /* In pxNewTCB->xStateListItem structure store pointer to current TCB so that two way link between
     * state list and TCB can be established. Also, in pxNewTCB->xEventListItem, store task TCB and
     * (configMAX_PRIORITIES - uxPriority) value so that it maintain event lists in priority order.
     */

    /* If we want to use reentrant newlib library, then we need to allocate separate memory by freertos.
     * In our implementation , it is supported only in ARM and IAR toolchains. For more details about
     * reentrancy in FreeRTOS, refer Carmine Noviello book.
     */

    /* Then we initialize the first stack frame such that it resembles a stack frame created by actual
     * context switch. This is necessary because when PendSV handler will start this task first time,
     * it will need some stack frame to restore context.
     */

    /* Stack frame for CM4:
     * Since CM4 uses full descending stack, first decrement top of stack pointer ( pxNewTCB->pxTopOfStack )
     * and then assign content. First, xPSR ( 0x01000000 ) is pushed. Then push PC value with bit-0 clear, as it
     * is loaded into the PC on exit from an ISR. Here, PC will point to task's function ( task
     * handler ). Then push LR and it should point to function which will assert/throw error because
     * this task should unstack to jump to LR. Because this stack frame is initial stack frame. Then
     * push R12, R3, R2, R1 in the sequence. Here we can keep those values' dummy or zero because this
     * is first stack frame. Then we need to push R0 which indicates parameter/argument to task function.
     * So, we can push pointer to parameter to task. Then we need to push initial EXEC_RETURN ( 0xfffffffd ).
     * Then we need to push R11, R10, R9, R8, R7, R6, R5, R4 in the sequence and for the same reason as
     * earlier, we can leave those values' dummy or zero.
     */

    /* Stack frame for CM33:
     * All CM4 steps are valid for CM33 as well. After those steps, if MPU is enabled then CONTROL register
     * value is pushed to indicate if code is privileged or not. Then end of the stack value is pushed
     * for PSPLIM register. Then if trustzone is enabled, secure context value is pushed ( portNO_SECURE_CONTEXT ).
     */

    /* Then this task can be added into ready list. This operation achieved in critical section. Generally,
     * critical section is created by disabling all interrupts and later those are enabled while coming
     * out of critical section.
     */

    /* In that first increment global number indicating current task count. If there are no current task
     * ( pxCurrentTCB is NULL ) then assign pxCurrentTCB to current task's TCB.
     */

    /* If this is a first task, then we need to initialize task lists also. FreeRTOS maintains pxReadyTasksLists[configMAX_PRIORITIES]
     * to maintain list tasks based on priority, xDelayedTaskList1 and xDelayedTaskList2 to maintain
     * delayed tasks and delayed tasks that have overflowed the current tick count respectively ?,
     * xTasksWaitingTermination for tasks that have been deleted but memory not yet freed and generally
     * get freed by idle task and xSuspendedTaskList list to maintain list of suspended tasks.
     */

    /* Then add this task into ready list and list maintaining states ( xStateListItem ) ? and come
     * out of critical section.
     */

    /* Then if scheduler is running, check if current running task has less priority that this task.
     * If yes, then yield current task and pend the PendSV interrupt to switch the tasks and start
     * running this new task.
     */
    if (xTaskCreate(GreenTask, "GreenTask", TASK_STACK_SIZE, NULL, TASK_PRIORITY+2, NULL) == pdPASS)
	{
		if (xTaskCreate(OrangeTask, "OrangeTask", TASK_STACK_SIZE, NULL, TASK_PRIORITY, NULL) == pdPASS)
		{
			/* Start the FreeRTOS scheduler. */
			vTaskStartScheduler();
		}
	}

	/* Should never get here. */
    CY_ASSERT(0);
}

/* [] END OF FILE */
