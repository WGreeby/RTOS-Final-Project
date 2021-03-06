/***************************************************************************//**
 * @file
 * @brief Blink examples functions
 *******************************************************************************
 * # License
 * <b>Copyright 2020 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * The licensor of this software is Silicon Laboratories Inc. Your use of this
 * software is governed by the terms of Silicon Labs Master Software License
 * Agreement (MSLA) available at
 * www.silabs.com/about-us/legal/master-software-license-agreement. This
 * software is distributed to you in Source Code format and is governed by the
 * sections of the MSLA applicable to Source Code.
 *
 ******************************************************************************/

#ifndef LED_OUTPUT_H
#define LED_OUTPUT_H

#include "gpio.h"

#define LED_OUTPUT_TASK_STACK_SIZE      128

#define LED_OUTPUT_TASK_PRIORITY        20
/***************************************************************************//**
 * Initialize blink example
 ******************************************************************************/
void ledOutput_init(void);

#endif  // LED_OUTPUT_H
