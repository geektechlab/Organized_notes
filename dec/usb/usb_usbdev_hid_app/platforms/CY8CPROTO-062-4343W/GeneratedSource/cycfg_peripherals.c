/*******************************************************************************
* File Name: cycfg_peripherals.c
*
* Description:
* Peripheral Hardware Block configuration
* This file was automatically generated and should not be modified.
* Configurator Backend 3.10.0
* device-db 4.100.0.4486
* mtb-pdl-cat1 3.9.0.0
*
********************************************************************************
* Copyright 2023 Cypress Semiconductor Corporation (an Infineon company) or
* an affiliate of Cypress Semiconductor Corporation.
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
********************************************************************************/

#include "cycfg_peripherals.h"

#define USBD_INTR_LVL_SEL (CY_USBFS_DEV_DRV_SET_SOF_LVL(0x1U) | \
                 CY_USBFS_DEV_DRV_SET_BUS_RESET_LVL(0x2U) | \
                 CY_USBFS_DEV_DRV_SET_EP0_LVL(0x2U) | \
                 CY_USBFS_DEV_DRV_SET_LPM_LVL(0x0U) | \
                 CY_USBFS_DEV_DRV_SET_ARB_EP_LVL(0x0U) | \
                 CY_USBFS_DEV_DRV_SET_EP1_LVL(0x1U) | \
                 CY_USBFS_DEV_DRV_SET_EP2_LVL(0x1U) | \
                 CY_USBFS_DEV_DRV_SET_EP3_LVL(0x1U) | \
                 CY_USBFS_DEV_DRV_SET_EP4_LVL(0x1U) | \
                 CY_USBFS_DEV_DRV_SET_EP5_LVL(0x1U) | \
                 CY_USBFS_DEV_DRV_SET_EP6_LVL(0x1U) | \
                 CY_USBFS_DEV_DRV_SET_EP7_LVL(0x1U) | \
                 CY_USBFS_DEV_DRV_SET_EP8_LVL(0x1U))

const cy_stc_scb_uart_config_t UART_config = 
{
    .uartMode = CY_SCB_UART_STANDARD,
    .enableMutliProcessorMode = false,
    .smartCardRetryOnNack = false,
    .irdaInvertRx = false,
    .irdaEnableLowPowerReceiver = false,
    .oversample = 12,
    .enableMsbFirst = false,
    .dataWidth = 8UL,
    .parity = CY_SCB_UART_PARITY_NONE,
    .stopBits = CY_SCB_UART_STOP_BITS_1,
    .enableInputFilter = false,
    .breakWidth = 11UL,
    .dropOnFrameError = false,
    .dropOnParityError = false,
    .receiverAddress = 0x0UL,
    .receiverAddressMask = 0x0UL,
    .acceptAddrInFifo = false,
    .enableCts = false,
    .ctsPolarity = CY_SCB_UART_ACTIVE_LOW,
    .rtsRxFifoLevel = 0UL,
    .rtsPolarity = CY_SCB_UART_ACTIVE_LOW,
    .rxFifoTriggerLevel = 63UL,
    .rxFifoIntEnableMask = 0UL,
    .txFifoTriggerLevel = 63UL,
    .txFifoIntEnableMask = 0UL,
};
#if defined (CY_USING_HAL) || defined(CY_USING_HAL_LITE)
    const cyhal_resource_inst_t UART_obj = 
    {
        .type = CYHAL_RSC_SCB,
        .block_num = 5U,
        .channel_num = 0U,
    };
#endif //defined (CY_USING_HAL) || defined(CY_USING_HAL_LITE)
#if defined(CY_USING_HAL_LITE)
    const cyhal_clock_t UART_clock = 
    {
        .block = CY_SYSCLK_DIV_8_BIT,
        .channel = 0,
    };
#endif //defined(CY_USING_HAL_LITE)
#if defined (CY_USING_HAL)
    cyhal_clock_t UART_clock;
    const cyhal_resource_inst_t UART_clk_rsrc = 
    {
        .type = CYHAL_RSC_CLOCK,
        .block_num = CY_SYSCLK_DIV_8_BIT,
        .channel_num = 0,
    };
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL) || defined(CY_USING_HAL_LITE)
    const cyhal_uart_configurator_t UART_hal_config = 
    {
        .resource = &UART_obj,
        .config = &UART_config,
        .clock = &UART_clock,
    #if defined (CY_USING_HAL)
        .gpios = {.pin_tx = P5_1, .pin_rts = NC, .pin_cts = NC},
    #endif //defined (CY_USING_HAL)
    };
#endif //defined (CY_USING_HAL) || defined(CY_USING_HAL_LITE)
const cy_stc_usbfs_dev_drv_config_t USBD_config = 
{
    .mode = CY_USBFS_DEV_DRV_EP_MANAGEMENT_CPU,
    .epAccess = CY_USBFS_DEV_DRV_USE_8_BITS_DR,
    .epBuffer = NULL,
    .epBufferSize = 0U,
    .dmaConfig[0] = NULL,
    .dmaConfig[1] = NULL,
    .dmaConfig[2] = NULL,
    .dmaConfig[3] = NULL,
    .dmaConfig[4] = NULL,
    .dmaConfig[5] = NULL,
    .dmaConfig[6] = NULL,
    .dmaConfig[7] = NULL,
    .enableLpm = false,
    .intrLevelSel = USBD_INTR_LVL_SEL,
};
#if defined (CY_USING_HAL)
    const cyhal_resource_inst_t USBD_obj = 
    {
        .type = CYHAL_RSC_USB,
        .block_num = 0U,
        .channel_num = 0U,
    };
#endif //defined (CY_USING_HAL)


void init_cycfg_peripherals(void)
{
#if defined (CY_USING_HAL)
    cyhal_clock_get(&UART_clock, &UART_clk_rsrc);
#endif //defined (CY_USING_HAL)
    Cy_SysClk_PeriphAssignDivider(PCLK_SCB5_CLOCK, CY_SYSCLK_DIV_8_BIT, 0U);
    Cy_SysClk_PeriphAssignDivider(PCLK_USB_CLOCK_DEV_BRS, CY_SYSCLK_DIV_16_BIT, 0U);
}

void reserve_cycfg_peripherals(void)
{
#if defined (CY_USING_HAL)
    cyhal_hwmgr_reserve(&UART_obj);
    cyhal_hwmgr_reserve(&USBD_obj);
#endif //defined (CY_USING_HAL)
}
