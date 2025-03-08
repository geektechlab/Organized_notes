/*******************************************************************************
* File Name: cycfg_pins.c
*
* Description:
* Pin configuration
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

#include "cycfg_pins.h"

const cy_stc_gpio_pin_config_t ioss_0_port_0_pin_0_config = 
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_ANALOG,
    .hsiom = ioss_0_port_0_pin_0_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .intMask = 0UL,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
    .driveSel = CY_GPIO_DRIVE_1_2,
    .vregEn = 0UL,
    .ibufMode = 0UL,
    .vtripSel = 0UL,
    .vrefSel = 0UL,
    .vohSel = 0UL,
};
#if defined (CY_USING_HAL)
    const cyhal_resource_inst_t ioss_0_port_0_pin_0_obj = 
    {
        .type = CYHAL_RSC_GPIO,
        .block_num = ioss_0_port_0_pin_0_PORT_NUM,
        .channel_num = ioss_0_port_0_pin_0_PIN,
    };
#endif //defined (CY_USING_HAL)
const cy_stc_gpio_pin_config_t ioss_0_port_0_pin_1_config = 
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_ANALOG,
    .hsiom = ioss_0_port_0_pin_1_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .intMask = 0UL,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
    .driveSel = CY_GPIO_DRIVE_1_2,
    .vregEn = 0UL,
    .ibufMode = 0UL,
    .vtripSel = 0UL,
    .vrefSel = 0UL,
    .vohSel = 0UL,
};
#if defined (CY_USING_HAL)
    const cyhal_resource_inst_t ioss_0_port_0_pin_1_obj = 
    {
        .type = CYHAL_RSC_GPIO,
        .block_num = ioss_0_port_0_pin_1_PORT_NUM,
        .channel_num = ioss_0_port_0_pin_1_PIN,
    };
#endif //defined (CY_USING_HAL)
const cy_stc_gpio_pin_config_t LED_RED_config = 
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_STRONG,
    .hsiom = LED_RED_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .intMask = 0UL,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
    .driveSel = CY_GPIO_DRIVE_1_2,
    .vregEn = 0UL,
    .ibufMode = 0UL,
    .vtripSel = 0UL,
    .vrefSel = 0UL,
    .vohSel = 0UL,
};
#if defined (CY_USING_HAL)
    const cyhal_resource_inst_t LED_RED_obj = 
    {
        .type = CYHAL_RSC_GPIO,
        .block_num = LED_RED_PORT_NUM,
        .channel_num = LED_RED_PIN,
    };
#endif //defined (CY_USING_HAL)
const cy_stc_gpio_pin_config_t ioss_0_port_14_pin_0_config = 
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_ANALOG,
    .hsiom = ioss_0_port_14_pin_0_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .intMask = 0UL,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
    .driveSel = CY_GPIO_DRIVE_1_2,
    .vregEn = 0UL,
    .ibufMode = 0UL,
    .vtripSel = 0UL,
    .vrefSel = 0UL,
    .vohSel = 0UL,
};
#if defined (CY_USING_HAL)
    const cyhal_resource_inst_t ioss_0_port_14_pin_0_obj = 
    {
        .type = CYHAL_RSC_GPIO,
        .block_num = ioss_0_port_14_pin_0_PORT_NUM,
        .channel_num = ioss_0_port_14_pin_0_PIN,
    };
#endif //defined (CY_USING_HAL)
const cy_stc_gpio_pin_config_t ioss_0_port_14_pin_1_config = 
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_ANALOG,
    .hsiom = ioss_0_port_14_pin_1_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .intMask = 0UL,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
    .driveSel = CY_GPIO_DRIVE_1_2,
    .vregEn = 0UL,
    .ibufMode = 0UL,
    .vtripSel = 0UL,
    .vrefSel = 0UL,
    .vohSel = 0UL,
};
#if defined (CY_USING_HAL)
    const cyhal_resource_inst_t ioss_0_port_14_pin_1_obj = 
    {
        .type = CYHAL_RSC_GPIO,
        .block_num = ioss_0_port_14_pin_1_PORT_NUM,
        .channel_num = ioss_0_port_14_pin_1_PIN,
    };
#endif //defined (CY_USING_HAL)
const cy_stc_gpio_pin_config_t ioss_0_port_5_pin_0_config = 
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_HIGHZ,
    .hsiom = ioss_0_port_5_pin_0_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .intMask = 0UL,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
    .driveSel = CY_GPIO_DRIVE_1_2,
    .vregEn = 0UL,
    .ibufMode = 0UL,
    .vtripSel = 0UL,
    .vrefSel = 0UL,
    .vohSel = 0UL,
};
#if defined (CY_USING_HAL)
    const cyhal_resource_inst_t ioss_0_port_5_pin_0_obj = 
    {
        .type = CYHAL_RSC_GPIO,
        .block_num = ioss_0_port_5_pin_0_PORT_NUM,
        .channel_num = ioss_0_port_5_pin_0_PIN,
    };
#endif //defined (CY_USING_HAL)
const cy_stc_gpio_pin_config_t ioss_0_port_5_pin_1_config = 
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_STRONG_IN_OFF,
    .hsiom = ioss_0_port_5_pin_1_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .intMask = 0UL,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
    .driveSel = CY_GPIO_DRIVE_1_2,
    .vregEn = 0UL,
    .ibufMode = 0UL,
    .vtripSel = 0UL,
    .vrefSel = 0UL,
    .vohSel = 0UL,
};
#if defined (CY_USING_HAL)
    const cyhal_resource_inst_t ioss_0_port_5_pin_1_obj = 
    {
        .type = CYHAL_RSC_GPIO,
        .block_num = ioss_0_port_5_pin_1_PORT_NUM,
        .channel_num = ioss_0_port_5_pin_1_PIN,
    };
#endif //defined (CY_USING_HAL)


void init_cycfg_pins(void)
{
    Cy_GPIO_Pin_Init(LED_RED_PORT, LED_RED_PIN, &LED_RED_config);
    Cy_GPIO_Pin_Init(ioss_0_port_14_pin_0_PORT, ioss_0_port_14_pin_0_PIN, &ioss_0_port_14_pin_0_config);
    Cy_GPIO_Pin_Init(ioss_0_port_14_pin_1_PORT, ioss_0_port_14_pin_1_PIN, &ioss_0_port_14_pin_1_config);
    Cy_GPIO_Pin_Init(ioss_0_port_5_pin_0_PORT, ioss_0_port_5_pin_0_PIN, &ioss_0_port_5_pin_0_config);
    Cy_GPIO_Pin_Init(ioss_0_port_5_pin_1_PORT, ioss_0_port_5_pin_1_PIN, &ioss_0_port_5_pin_1_config);
}

void reserve_cycfg_pins(void)
{
#if defined (CY_USING_HAL)
    cyhal_hwmgr_reserve(&ioss_0_port_0_pin_0_obj);
    cyhal_hwmgr_reserve(&ioss_0_port_0_pin_1_obj);
    cyhal_hwmgr_reserve(&LED_RED_obj);
    cyhal_hwmgr_reserve(&ioss_0_port_14_pin_0_obj);
    cyhal_hwmgr_reserve(&ioss_0_port_14_pin_1_obj);
    cyhal_hwmgr_reserve(&ioss_0_port_5_pin_0_obj);
    cyhal_hwmgr_reserve(&ioss_0_port_5_pin_1_obj);
#endif //defined (CY_USING_HAL)
}
