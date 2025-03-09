/***************************************************************************//**
* \file main.c
* \version 1.0
*
* \brief
* Objective:
*    This code example demonstrates USB HID interface class operation by
*    implementing a 3-button mouse. When the code is running, the mouse cursor
*    moves in the square.
*
* \note
* To connect USB Device (PSoC6) to the USB Host use USB receptacle J10 for 
* CY8CPROTO-062-4343W.
*
********************************************************************************
* \copyright
* Copyright 2017-2019 Cypress Semiconductor Corporation
* SPDX-License-Identifier: Apache-2.0
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*******************************************************************************/

#include "cy_pdl.h"
#include "cy_usbfs_dev_drv.h"
#include "cycfg_usbdev.h"
#include "cycfg.h"
#include "stdio_user.h"
#include <stdio.h>

/***************************************************************************
* Interrupt configuration
***************************************************************************/
static void USBD_IsrHigh(void);
static void USBD_IsrMedium(void);
static void USBD_IsrLow(void);

const cy_stc_sysint_t UsbDevIntrHigh =
{
    .intrSrc = (IRQn_Type) usb_interrupt_hi_IRQn,
    .intrPriority = 5U,
};
const cy_stc_sysint_t UsbDevIntrMedium =
{
    .intrSrc = (IRQn_Type) usb_interrupt_med_IRQn,
    .intrPriority = 6U,
};
const cy_stc_sysint_t UsbDevIntrLow =
{
    .intrSrc = (IRQn_Type) usb_interrupt_lo_IRQn,
    .intrPriority = 7U,
};


/***************************************************************************
* Global variables
***************************************************************************/

/* Map USB Configurator generated defines */
#define USBD_devices    usb_devices
#define USBD_devConfig  usb_devConfig
#define USBD_hidConfig  usb_hidConfig

cy_stc_usbfs_dev_drv_context_t  USBD_context;
cy_stc_usb_dev_context_t        USBD_devContext;
cy_stc_usb_dev_hid_context_t    USBD_hidContext;


/***************************************************************************
* Mouse movement defines and variables
***************************************************************************/
#define MOUSE_DATA_LEN      (3U)
#define CURSOR_STEP_PLUS    ((uint8_t)  (5))
#define CURSOR_STEP_MINUS   ((uint8_t) (-5))
#define CURSOR_X_POS        (1U)
#define CURSOR_Y_POS        (2U)

/* Mouse packet array: buttons (1st byte), X (2nd byte), Y (3rd byte) */
CY_USB_DEV_ALLOC_ENDPOINT_BUFFER(mouseData, MOUSE_DATA_LEN);

typedef enum
{
    INCREMENT_X,
    INCREMENT_Y,
    DECREMENT_X,
    DECREMENT_Y,
} cy_en_mouse_movement_t;


/*******************************************************************************
* Function Name: MouseMovement
****************************************************************************//**
*
* Implements mouse movement and writes report to be read by the host.
*
*******************************************************************************/
void MouseMovement(void)
{
    const uint32_t mouseInEp = 1UL;
    const uint32_t stepsNumber = 96;

    uint8_t counter = 0U;

    cy_en_mouse_movement_t move = INCREMENT_X;

    for(;;)
    {
        if (0U == counter)
        {
            counter = stepsNumber;

            switch(move)
            {
                case INCREMENT_X:
                    mouseData[CURSOR_X_POS] = CURSOR_STEP_PLUS;
                    mouseData[CURSOR_Y_POS] = 0U;

                    move = INCREMENT_Y;
                    break;

                case INCREMENT_Y:
                    mouseData[CURSOR_X_POS] = 0U;
                    mouseData[CURSOR_Y_POS] = CURSOR_STEP_PLUS;

                    move = DECREMENT_X;
                    break;

                case DECREMENT_X:
                    mouseData[CURSOR_X_POS] = CURSOR_STEP_MINUS;
                    mouseData[CURSOR_Y_POS] = 0U;

                    move = DECREMENT_Y;
                    break;

                case DECREMENT_Y:
                    mouseData[CURSOR_X_POS] = 0U;
                    mouseData[CURSOR_Y_POS] = CURSOR_STEP_MINUS;

                    move = INCREMENT_X;
                    break;
            }
        }

        /* Update mouse position */
        Cy_USB_Dev_WriteEpBlocking(mouseInEp, mouseData, MOUSE_DATA_LEN,
                                    CY_USB_DEV_WAIT_FOREVER, &USBD_devContext);

        counter--;
        Cy_SysLib_Delay(10UL);
    }
}

//#define ENABLE_DS     /* Enables Deep Sleep */

cy_stc_scb_uart_context_t UART_context;

/*******************************************************************************
* Function Name: main
****************************************************************************//**
*
* Code example flow:
* 1. Configures USB Device middleware and driver.
* 2. Configures HID Class.
* 3. Configures interrupts for USB operation.
* 4. Enable interrupts and USB Device.
*    Note that The Cy_USB_Dev_Connect functions waits for enumeration
*    completion.
* 5. Execute mouse movement.
*
*******************************************************************************/
int main(void)
{
    cy_en_usb_dev_status_t status;
    
    /* Set up internal routing, pins, and clock-to-peripheral connections */
    init_cycfg_all();

    Cy_SCB_UART_Init(UART_HW, &UART_config, &UART_context);
    Cy_SCB_UART_Enable(UART_HW);

    printf("Initializing USB...\r\n");

    /* Initialize USB */
    status = Cy_USB_Dev_Init(USBD_HW, &USBD_config, &USBD_context,
                             &USBD_devices[0], &USBD_devConfig, &USBD_devContext);
    if (CY_USB_DEV_SUCCESS != status)
    {
        /* Initialization error - stop execution */
        while(1);
    }

    status = Cy_USB_Dev_HID_Init(&USBD_hidConfig, &USBD_hidContext, &USBD_devContext);
    if (CY_USB_DEV_SUCCESS != status)
    {
        /* HID Initialization error - stop execution */
        while(1);
    }

    /* Initialize and enable USB interrupts */
    (void) Cy_SysInt_Init(&UsbDevIntrHigh,   &USBD_IsrHigh);
    (void) Cy_SysInt_Init(&UsbDevIntrMedium, &USBD_IsrMedium);
    (void) Cy_SysInt_Init(&UsbDevIntrLow,    &USBD_IsrLow);

    NVIC_EnableIRQ(UsbDevIntrHigh.intrSrc);
    NVIC_EnableIRQ(UsbDevIntrMedium.intrSrc);
    NVIC_EnableIRQ(UsbDevIntrLow.intrSrc);

    /* enable interrupts */
    __enable_irq();

    /* Make device appear on the bus */
    Cy_USB_Dev_Connect(true, CY_USB_DEV_WAIT_FOREVER, &USBD_devContext);

    /* Control Mouse Movement */
    MouseMovement();

    for(;;)
    {
    }
}


/*******************************************************************************
* Function Name: USBD_IsrHigh
****************************************************************************//**
*
* Implements USBD High interrupt.
*
*******************************************************************************/
static void USBD_IsrHigh(void)
{
    /* Call interrupt processing */
    Cy_USBFS_Dev_Drv_Interrupt(USBD_HW,
                               Cy_USBFS_Dev_Drv_GetInterruptCauseHi(USBD_HW),
                               &USBD_context);
}


/*******************************************************************************
* Function Name: USBD_IsrMedium
****************************************************************************//**
*
* Implements USBD Medium interrupt.
*
*******************************************************************************/
static void USBD_IsrMedium(void)
{
    /* Call interrupt processing */
    Cy_USBFS_Dev_Drv_Interrupt(USBD_HW,
                               Cy_USBFS_Dev_Drv_GetInterruptCauseMed(USBD_HW),
                               &USBD_context);
}


/*******************************************************************************
* Function Name: USBD_IsrLow
****************************************************************************//**
*
* Implements USBD Low interrupt.
*
*******************************************************************************/
static void USBD_IsrLow(void)
{
    /* Call interrupt processing */
    Cy_USBFS_Dev_Drv_Interrupt(USBD_HW,
                               Cy_USBFS_Dev_Drv_GetInterruptCauseLo(USBD_HW),
                               &USBD_context);
}


/* [] END OF FILE */
