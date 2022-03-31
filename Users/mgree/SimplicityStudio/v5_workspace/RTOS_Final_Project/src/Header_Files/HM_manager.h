/***************************************************************************//**
 * @file
 * @brief Harkonnen Mass Manager Task Header File
 ******************************************************************************/

#ifndef HM_MANAGER_H
#define HM_MANAGER_H

#include "gpio.h"

#define HM_MANAGER_TASK_STACK_SIZE      128

#define HM_MANAGER_TASK_PRIORITY        20
/***************************************************************************//**
 * Initialize blink example
 ******************************************************************************/
void HM_manager_init(void);

#endif  // HM_MANAGER_H
