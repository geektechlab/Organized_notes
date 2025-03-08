/*******************************************************************************
* File Name: cycfg_pins.h
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

#if !defined(CYCFG_PINS_H)
#define CYCFG_PINS_H

#include "cycfg_notices.h"
#include "cy_gpio.h"
#include "cycfg_routing.h"
#if defined (CY_USING_HAL)
    #include "cyhal_hwmgr.h"
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL_LITE)
    #include "cyhal_hw_types.h"
#endif //defined (CY_USING_HAL_LITE)

#if defined(__cplusplus)
extern "C" {
#endif

#define ioss_0_port_0_pin_0_ENABLED 1U
#define ioss_0_port_0_pin_0_PORT GPIO_PRT0
#define ioss_0_port_0_pin_0_PORT_NUM 0U
#define ioss_0_port_0_pin_0_PIN 0U
#define ioss_0_port_0_pin_0_NUM 0U
#define ioss_0_port_0_pin_0_DRIVEMODE CY_GPIO_DM_ANALOG
#define ioss_0_port_0_pin_0_INIT_DRIVESTATE 1
#ifndef ioss_0_port_0_pin_0_HSIOM
    #define ioss_0_port_0_pin_0_HSIOM HSIOM_SEL_GPIO
#endif
#define ioss_0_port_0_pin_0_IRQ ioss_interrupts_gpio_0_IRQn
#if defined (CY_USING_HAL)
    #define ioss_0_port_0_pin_0_HAL_PORT_PIN P0_0
    #define ioss_0_port_0_pin_0 P0_0
    #define ioss_0_port_0_pin_0_HAL_IRQ CYHAL_GPIO_IRQ_NONE
    #define ioss_0_port_0_pin_0_HAL_DIR CYHAL_GPIO_DIR_INPUT 
    #define ioss_0_port_0_pin_0_HAL_DRIVEMODE CYHAL_GPIO_DRIVE_ANALOG
#endif //defined (CY_USING_HAL)
#define ioss_0_port_0_pin_1_ENABLED 1U
#define ioss_0_port_0_pin_1_PORT GPIO_PRT0
#define ioss_0_port_0_pin_1_PORT_NUM 0U
#define ioss_0_port_0_pin_1_PIN 1U
#define ioss_0_port_0_pin_1_NUM 1U
#define ioss_0_port_0_pin_1_DRIVEMODE CY_GPIO_DM_ANALOG
#define ioss_0_port_0_pin_1_INIT_DRIVESTATE 1
#ifndef ioss_0_port_0_pin_1_HSIOM
    #define ioss_0_port_0_pin_1_HSIOM HSIOM_SEL_GPIO
#endif
#define ioss_0_port_0_pin_1_IRQ ioss_interrupts_gpio_0_IRQn
#if defined (CY_USING_HAL)
    #define ioss_0_port_0_pin_1_HAL_PORT_PIN P0_1
    #define ioss_0_port_0_pin_1 P0_1
    #define ioss_0_port_0_pin_1_HAL_IRQ CYHAL_GPIO_IRQ_NONE
    #define ioss_0_port_0_pin_1_HAL_DIR CYHAL_GPIO_DIR_INPUT 
    #define ioss_0_port_0_pin_1_HAL_DRIVEMODE CYHAL_GPIO_DRIVE_ANALOG
#endif //defined (CY_USING_HAL)
#define LED_RED_ENABLED 1U
#define LED_RED_PORT GPIO_PRT13
#define LED_RED_PORT_NUM 13U
#define LED_RED_PIN 7U
#define LED_RED_NUM 7U
#define LED_RED_DRIVEMODE CY_GPIO_DM_STRONG
#define LED_RED_INIT_DRIVESTATE 1
#ifndef ioss_0_port_13_pin_7_HSIOM
    #define ioss_0_port_13_pin_7_HSIOM HSIOM_SEL_GPIO
#endif
#define LED_RED_HSIOM ioss_0_port_13_pin_7_HSIOM
#define LED_RED_IRQ ioss_interrupts_gpio_13_IRQn
#if defined (CY_USING_HAL)
    #define LED_RED_HAL_PORT_PIN P13_7
    #define LED_RED P13_7
    #define LED_RED_HAL_IRQ CYHAL_GPIO_IRQ_NONE
    #define LED_RED_HAL_DIR CYHAL_GPIO_DIR_BIDIRECTIONAL 
    #define LED_RED_HAL_DRIVEMODE CYHAL_GPIO_DRIVE_STRONG
#endif //defined (CY_USING_HAL)
#define ioss_0_port_14_pin_0_ENABLED 1U
#define ioss_0_port_14_pin_0_PORT GPIO_PRT14
#define ioss_0_port_14_pin_0_PORT_NUM 14U
#define ioss_0_port_14_pin_0_PIN 0U
#define ioss_0_port_14_pin_0_NUM 0U
#define ioss_0_port_14_pin_0_DRIVEMODE CY_GPIO_DM_ANALOG
#define ioss_0_port_14_pin_0_INIT_DRIVESTATE 1
#ifndef ioss_0_port_14_pin_0_HSIOM
    #define ioss_0_port_14_pin_0_HSIOM HSIOM_SEL_GPIO
#endif
#define ioss_0_port_14_pin_0_IRQ ioss_interrupts_gpio_14_IRQn
#if defined (CY_USING_HAL)
    #define ioss_0_port_14_pin_0_HAL_PORT_PIN P14_0
    #define ioss_0_port_14_pin_0 P14_0
    #define ioss_0_port_14_pin_0_HAL_IRQ CYHAL_GPIO_IRQ_NONE
    #define ioss_0_port_14_pin_0_HAL_DIR CYHAL_GPIO_DIR_INPUT 
    #define ioss_0_port_14_pin_0_HAL_DRIVEMODE CYHAL_GPIO_DRIVE_ANALOG
#endif //defined (CY_USING_HAL)
#define ioss_0_port_14_pin_1_ENABLED 1U
#define ioss_0_port_14_pin_1_PORT GPIO_PRT14
#define ioss_0_port_14_pin_1_PORT_NUM 14U
#define ioss_0_port_14_pin_1_PIN 1U
#define ioss_0_port_14_pin_1_NUM 1U
#define ioss_0_port_14_pin_1_DRIVEMODE CY_GPIO_DM_ANALOG
#define ioss_0_port_14_pin_1_INIT_DRIVESTATE 1
#ifndef ioss_0_port_14_pin_1_HSIOM
    #define ioss_0_port_14_pin_1_HSIOM HSIOM_SEL_GPIO
#endif
#define ioss_0_port_14_pin_1_IRQ ioss_interrupts_gpio_14_IRQn
#if defined (CY_USING_HAL)
    #define ioss_0_port_14_pin_1_HAL_PORT_PIN P14_1
    #define ioss_0_port_14_pin_1 P14_1
    #define ioss_0_port_14_pin_1_HAL_IRQ CYHAL_GPIO_IRQ_NONE
    #define ioss_0_port_14_pin_1_HAL_DIR CYHAL_GPIO_DIR_INPUT 
    #define ioss_0_port_14_pin_1_HAL_DRIVEMODE CYHAL_GPIO_DRIVE_ANALOG
#endif //defined (CY_USING_HAL)
#define ioss_0_port_5_pin_0_ENABLED 1U
#define ioss_0_port_5_pin_0_PORT GPIO_PRT5
#define ioss_0_port_5_pin_0_PORT_NUM 5U
#define ioss_0_port_5_pin_0_PIN 0U
#define ioss_0_port_5_pin_0_NUM 0U
#define ioss_0_port_5_pin_0_DRIVEMODE CY_GPIO_DM_HIGHZ
#define ioss_0_port_5_pin_0_INIT_DRIVESTATE 1
#ifndef ioss_0_port_5_pin_0_HSIOM
    #define ioss_0_port_5_pin_0_HSIOM HSIOM_SEL_GPIO
#endif
#define ioss_0_port_5_pin_0_IRQ ioss_interrupts_gpio_5_IRQn
#if defined (CY_USING_HAL)
    #define ioss_0_port_5_pin_0_HAL_PORT_PIN P5_0
    #define ioss_0_port_5_pin_0 P5_0
    #define ioss_0_port_5_pin_0_HAL_IRQ CYHAL_GPIO_IRQ_NONE
    #define ioss_0_port_5_pin_0_HAL_DIR CYHAL_GPIO_DIR_INPUT 
    #define ioss_0_port_5_pin_0_HAL_DRIVEMODE CYHAL_GPIO_DRIVE_NONE
#endif //defined (CY_USING_HAL)
#define ioss_0_port_5_pin_1_ENABLED 1U
#define ioss_0_port_5_pin_1_PORT GPIO_PRT5
#define ioss_0_port_5_pin_1_PORT_NUM 5U
#define ioss_0_port_5_pin_1_PIN 1U
#define ioss_0_port_5_pin_1_NUM 1U
#define ioss_0_port_5_pin_1_DRIVEMODE CY_GPIO_DM_STRONG_IN_OFF
#define ioss_0_port_5_pin_1_INIT_DRIVESTATE 1
#ifndef ioss_0_port_5_pin_1_HSIOM
    #define ioss_0_port_5_pin_1_HSIOM HSIOM_SEL_GPIO
#endif
#define ioss_0_port_5_pin_1_IRQ ioss_interrupts_gpio_5_IRQn
#if defined (CY_USING_HAL)
    #define ioss_0_port_5_pin_1_HAL_PORT_PIN P5_1
    #define ioss_0_port_5_pin_1 P5_1
    #define ioss_0_port_5_pin_1_HAL_IRQ CYHAL_GPIO_IRQ_NONE
    #define ioss_0_port_5_pin_1_HAL_DIR CYHAL_GPIO_DIR_OUTPUT 
    #define ioss_0_port_5_pin_1_HAL_DRIVEMODE CYHAL_GPIO_DRIVE_STRONG
#endif //defined (CY_USING_HAL)

extern const cy_stc_gpio_pin_config_t ioss_0_port_0_pin_0_config;
#if defined (CY_USING_HAL)
    extern const cyhal_resource_inst_t ioss_0_port_0_pin_0_obj;
#endif //defined (CY_USING_HAL)
extern const cy_stc_gpio_pin_config_t ioss_0_port_0_pin_1_config;
#if defined (CY_USING_HAL)
    extern const cyhal_resource_inst_t ioss_0_port_0_pin_1_obj;
#endif //defined (CY_USING_HAL)
extern const cy_stc_gpio_pin_config_t LED_RED_config;
#if defined (CY_USING_HAL)
    extern const cyhal_resource_inst_t LED_RED_obj;
#endif //defined (CY_USING_HAL)
extern const cy_stc_gpio_pin_config_t ioss_0_port_14_pin_0_config;
#if defined (CY_USING_HAL)
    extern const cyhal_resource_inst_t ioss_0_port_14_pin_0_obj;
#endif //defined (CY_USING_HAL)
extern const cy_stc_gpio_pin_config_t ioss_0_port_14_pin_1_config;
#if defined (CY_USING_HAL)
    extern const cyhal_resource_inst_t ioss_0_port_14_pin_1_obj;
#endif //defined (CY_USING_HAL)
extern const cy_stc_gpio_pin_config_t ioss_0_port_5_pin_0_config;
#if defined (CY_USING_HAL)
    extern const cyhal_resource_inst_t ioss_0_port_5_pin_0_obj;
#endif //defined (CY_USING_HAL)
extern const cy_stc_gpio_pin_config_t ioss_0_port_5_pin_1_config;
#if defined (CY_USING_HAL)
    extern const cyhal_resource_inst_t ioss_0_port_5_pin_1_obj;
#endif //defined (CY_USING_HAL)

void init_cycfg_pins(void);
void reserve_cycfg_pins(void);

#if defined(__cplusplus)
}
#endif


#endif /* CYCFG_PINS_H */
