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

#ifndef LASER_CONTROLLER_H
#define LASER_CONTROLLER_H

#include "gpio.h"

#define LASER_CONTROLLER_TASK_STACK_SIZE      128

#define LASER_CONTROLLER_TASK_PRIORITY        20
/***************************************************************************//**
 * Initialize blink example
 ******************************************************************************/
void laserController_init(void);

#endif  // LASER_CONTROLLER_H
