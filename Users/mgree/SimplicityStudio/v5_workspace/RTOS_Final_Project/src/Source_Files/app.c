/***************************************************************************//**
 * @file
 * @brief Top level application functions
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
#include "app.h"
#include "os.h"
#include "em_emu.h"
#include "glib.h"
#include "dmd.h"
#include "sl_board_control.h"
#include "sl_board_control_config.h"
#include "shieldCharger.h"
#include "platformDirection.h"
#include "physicsManager.h"
#include "laserController.h"
#include "HM_manager.h"
#include "ledOutput.h"
#include "lcdDisplay.h"
#include "gameMonitor.h"
#include <stdio.h>

/*******************************************************************************
* Task Variables
*******************************************************************************/
//Task Variables
static OS_TCB lcd_tcb;
static CPU_STK lcd_stack[LCD_DISPLAY_TASK_STACK_SIZE];
static OS_TCB led_tcb;
static CPU_STK led_stack[LED_OUTPUT_TASK_STACK_SIZE];
static OS_TCB shieldCharger_tcb;
static CPU_STK shieldCharger_stack[SHIELD_CHARGER_TASK_STACK_SIZE];
static OS_TCB platformDirection_tcb;
static CPU_STK platformDirection_stack[PLATFORM_DIRECTION_TASK_STACK_SIZE];
static OS_TCB physicsManager_tcb;
static CPU_STK physicsManager_stack[PHYSICS_MANAGER_TASK_STACK_SIZE];
static OS_TCB laserController_tcb;
static CPU_STK laserController_stack[LASER_CONTROLLER_TASK_STACK_SIZE];
static OS_TCB HM_manager_tcb;
static CPU_STK HM_manager_stack[HM_MANAGER_TASK_STACK_SIZE];
static OS_TCB gameMonitor_tcb;
static CPU_STK gameMonitor_stack[GAME_MONITOR_TASK_STACK_SIZE];


/*******************************************************************************
* ITC Variables
*******************************************************************************/
//Button Flag Group
static OS_FLAG_GRP BTN_Flags;

/*******************************************************************************
* Data Structures
*******************************************************************************/

/*******************************************************************************
 *********************   LOCAL FUNCTION PROTOTYPES   ***************************
 ******************************************************************************/
static void lcdDisplay_task(void *arg);
static void ledOutput_task(void *arg);
static void shieldCharger_task(void *arg);
static void platformDirection_task(void *arg);
static void physicsManager_task(void *arg);
static void laserController_task(void *arg);
static void HM_manager_task(void *arg);
static void gameMonitor_task(void *arg);

/*******************************************************************************
 **************************   INITAL FUNCTIONS   *******************************
 ******************************************************************************/


/***************************************************************************//**
 * Initialize application.
 ******************************************************************************/
void app_init(void)
{
  gpio_open();
  CAPSENSE_Init();
  buttonInput_init();
  sliderInput_init();
  ledOutput_init();
}

/***************************************************************************//**
 * Initialize LCD Display Task.
 ******************************************************************************/
void lcdDisplay_init(void)
{
  RTOS_ERR err;
  uint32_t status;

  //Enable the memory lcd
  status = sl_board_enable_display();
  EFM_ASSERT(status == SL_STATUS_OK);

   //Initialize the DMD support for memory lcd display
   status = DMD_init(0);
   EFM_ASSERT(status == DMD_OK);

   //Initialize the glib context */
   //status = GLIB_contextInit(&glibContext);
   EFM_ASSERT(status == GLIB_OK);

   //glibContext.backgroundColor = White;
   //glibContext.foregroundColor = Black;

   //Fill LCD with background color
   //GLIB_clear(&glibContext);

   //Use Narrow font
   //GLIB_setFont(&glibContext, (GLIB_Font_t *) &GLIB_FontNarrow6x8);

  // Create LCD Display Task
  OSTaskCreate(&lcd_tcb,
               "LCD Display Task",
               lcdDisplay_task,
               DEF_NULL,
               LCD_DISPLAY_TASK_PRIORITY,
               &lcd_stack[0],
               (LCD_DISPLAY_TASK_STACK_SIZE / 10u),
               LCD_DISPLAY_TASK_STACK_SIZE,
               0u,
               0u,
               DEF_NULL,
               (OS_OPT_TASK_STK_CLR),
               &err);
  EFM_ASSERT((RTOS_ERR_CODE_GET(err) == RTOS_ERR_NONE));
}

/***************************************************************************//**
 * Initialize LED Output Task.
 ******************************************************************************/
void ledOutput_init(void)
{
  RTOS_ERR err;

  // Create Blink Task
  OSTaskCreate(&led_tcb,
               "LED Output Task",
               ledOutput_task,
               DEF_NULL,
               LED_OUTPUT_TASK_PRIORITY,
               &led_stack[0],
               (LED_OUTPUT_TASK_STACK_SIZE / 10u),
               LED_OUTPUT_TASK_STACK_SIZE,
               0u,
               0u,
               DEF_NULL,
               (OS_OPT_TASK_STK_CLR),
               &err);
  EFM_ASSERT((RTOS_ERR_CODE_GET(err) == RTOS_ERR_NONE));
}

/***************************************************************************//**
 * Initialize Platform Direction Task.
 ******************************************************************************/
void platformDirection_init(void)
{
  RTOS_ERR err;

  // Create Blink Task
  OSTaskCreate(&platformDirection_tcb,
               "Platform Direction Task",
               platformDirection_task,
               DEF_NULL,
               PLATFORM_DIRECTION_TASK_PRIORITY,
               &platformDirection_stack[0],
               (PLATFORM_DIRECTION_TASK_STACK_SIZE / 10u),
               PLATFORM_DIRECTION_TASK_STACK_SIZE,
               0u,
               0u,
               DEF_NULL,
               (OS_OPT_TASK_STK_CLR),
               &err);
  EFM_ASSERT((RTOS_ERR_CODE_GET(err) == RTOS_ERR_NONE));
}

/***************************************************************************//**
 * Initialize Laser Controller Task.
 ******************************************************************************/
void laserController_init(void)
{
  RTOS_ERR err;

  // Create Blink Task
  OSTaskCreate(&laserController_tcb,
               "Laser Controller Task",
               laserController_task,
               DEF_NULL,
               LASER_CONTROLLER_TASK_PRIORITY,
               &laserController_stack[0],
               (LASER_CONTROLLER_TASK_STACK_SIZE / 10u),
               LASER_CONTROLLER_TASK_STACK_SIZE,
               0u,
               0u,
               DEF_NULL,
               (OS_OPT_TASK_STK_CLR),
               &err);
  EFM_ASSERT((RTOS_ERR_CODE_GET(err) == RTOS_ERR_NONE));
}

/***************************************************************************//**
 * Initialize Harkonnen Mass Manager Task.
 ******************************************************************************/
void HM_manager_init(void)
{
  RTOS_ERR err;

  // Create Blink Task
  OSTaskCreate(&HM_manager_tcb,
               "HM Manager Task",
               HM_manager_task,
               DEF_NULL,
               HM_MANAGER_TASK_PRIORITY,
               &HM_manager_stack[0],
               (HM_MANAGER_TASK_STACK_SIZE / 10u),
               HM_MANAGER_TASK_STACK_SIZE,
               0u,
               0u,
               DEF_NULL,
               (OS_OPT_TASK_STK_CLR),
               &err);
  EFM_ASSERT((RTOS_ERR_CODE_GET(err) == RTOS_ERR_NONE));
}

/***************************************************************************//**
 * Initialize Shield Charger Task.
 ******************************************************************************/
void shieldCharger_init(void)
{
  RTOS_ERR err;

  // Create Blink Task
  OSTaskCreate(&shieldCharger_tcb,
               "Shield Charger Task",
               shieldCharger_task,
               DEF_NULL,
               SHIELD_CHARGER_TASK_PRIORITY,
               &shieldCharger_stack[0],
               (SHIELD_CHARGER_TASK_STACK_SIZE / 10u),
               SHIELD_CHARGER_TASK_STACK_SIZE,
               0u,
               0u,
               DEF_NULL,
               (OS_OPT_TASK_STK_CLR),
               &err);
  EFM_ASSERT((RTOS_ERR_CODE_GET(err) == RTOS_ERR_NONE));
}

/***************************************************************************//**
 * Initialize Physics Manager Task.
 ******************************************************************************/
void physicsManager_init(void)
{
  RTOS_ERR err;

  // Create Blink Task
  OSTaskCreate(&physicsManager_tcb,
               "Physics Manager Task",
               physicsManager_task,
               DEF_NULL,
               PHYSICS_MANAGER_TASK_PRIORITY,
               &physicsManager_stack[0],
               (PHYSICS_MANAGER_TASK_STACK_SIZE / 10u),
               PHYSICS_MANAGER_TASK_STACK_SIZE,
               0u,
               0u,
               DEF_NULL,
               (OS_OPT_TASK_STK_CLR),
               &err);
  EFM_ASSERT((RTOS_ERR_CODE_GET(err) == RTOS_ERR_NONE));
}

/***************************************************************************//**
 * Initialize game Monitor Task.
 ******************************************************************************/
void gameMonitor_init(void)
{
  RTOS_ERR err;

  // Create Blink Task
  OSTaskCreate(&gameMonitor_tcb,
               "Game Monitor Task",
               gameMonitor_task,
               DEF_NULL,
               GAME_MONITOR_TASK_PRIORITY,
               &gameMonitor_stack[0],
               (GAME_MONITOR_TASK_STACK_SIZE / 10u),
               GAME_MONITOR_TASK_STACK_SIZE,
               0u,
               0u,
               DEF_NULL,
               (OS_OPT_TASK_STK_CLR),
               &err);
  EFM_ASSERT((RTOS_ERR_CODE_GET(err) == RTOS_ERR_NONE));
}

/*******************************************************************************
 ************************   GLOBAL FUNCTIONS   *********************************
 ******************************************************************************/

/***************************************************************************//**
 * Button Flags Function.
 * buttonFlags handles waking up the correct task when a GPIO interrupt is
 * triggered by pushing one of the buttons. Button0 (right) corresponds to the
 * laser function and Button1 (left) corresponds to the shielding function.
 * We want to flag the laser task upon the right button being pushed, and
 * we want to flag the shield task upon the left button being pushed or
 * released. Pressing both buttons at the same time should cancel each other
 * out.
 *
 * Under the current version of the project. The laser activation will break the
 * shield charging.
 ******************************************************************************/
void buttonEvent(int interruptDiv)
{
  bool button0pressed;
  bool button1pressed;

  //PinInGet returns 0 for pressed and 1 for not pressed
  bool button0 = GPIO_PinInGet(BUTTON0_port, BUTTON0_pin);
  bool button1 = GPIO_PinInGet(BUTTON1_port, BUTTON1_pin);

  //Check if both buttons are reading zero and set global to false if that is the case
  if (!button0 && !button1) button1pressed = false;
  else if(!button1) button1pressed = true;
  else button1pressed = false;

  //Check if both buttons are reading zero and set global to false if that is the case
  if (!button0 && !button1) button0pressed = false;
  else if(!button0) button0pressed = true;
  else button0pressed = false;

  RTOS_ERR err;

  if(interruptDiv == even) { //Button0 (right)

      if(button0pressed) {
          //Flag the laser controller
          OSFlagPost(&BTN_Flags, right_button, OS_OPT_POST_FLAG_SET, &err);
          if(err.Code != RTOS_ERR_NONE) EFM_ASSERT(false);
      }
      return;
  }

  if(interruptDiv == odd) { //Button1 (left)
      if(button1pressed) {
          //Flag the shield charger that the shield has been engaged
          OSFlagPost(&BTN_Flags, left_button_pressed, OS_OPT_POST_FLAG_SET, &err);
          if(err.Code != RTOS_ERR_NONE) EFM_ASSERT(false);
          return;
      }
      else {
          //Flag the shield charger that the shield has been released
          OSFlagPost(&BTN_Flags, left_button_released, OS_OPT_POST_FLAG_SET, &err);
          if(err.Code != RTOS_ERR_NONE) EFM_ASSERT(false);
          return;
      }
  }
}

/*******************************************************************************
 **************************   APPLICATION TASKS   ******************************
 ******************************************************************************/

/***************************************************************************//**
 * Shield Charger Task.
 *
 ******************************************************************************/
static void shieldCharger_task(void *arg)
{
    PP_UNUSED_PARAM(arg);

    //Pend on ISR Event Flags
    OS_FLAGS flags;

    RTOS_ERR err;
    while (1)
    {

    }
}

/***************************************************************************//**
 * Platform Direction Task.
 *
 * This tasks handles changes to the platform from the Capsense inputs.
 * The Capsense slider has 4 positions. Each position will correspond with a
 * direction and an amount of force that is applied to the platform.
 ******************************************************************************/
static void platformDirection_task(void *arg)
{
    PP_UNUSED_PARAM(arg);

    //Pend on ISR Event Flags
    OS_FLAGS flags;

    RTOS_ERR err;
    while (1)
    {

    }
}

/***************************************************************************//**
 * Physics Manager Task.
 *
 ******************************************************************************/
static void physicsManager_task(void *arg)
{
    PP_UNUSED_PARAM(arg);

    //Pend on ISR Event Flags
    OS_FLAGS flags;

    RTOS_ERR err;
    while (1)
    {

    }
}

/***************************************************************************//**
 * LED Output Task.
 *
 ******************************************************************************/
static void ledOutput_task(void *arg)
{
    PP_UNUSED_PARAM(arg);

    //Pend on ISR Event Flags
    OS_FLAGS flags;

    RTOS_ERR err;
    while (1)
    {

    }
}

/***************************************************************************//**
 * LCD Display Task.
 *
 ******************************************************************************/
static void lcdDisplay_task(void *arg)
{
    PP_UNUSED_PARAM(arg);

    //Pend on ISR Event Flags
    OS_FLAGS flags;

    RTOS_ERR err;
    while (1)
    {

    }
}

/***************************************************************************//**
 * Laser Controller Task.
 *
 ******************************************************************************/
static void laserController_task(void *arg)
{
    PP_UNUSED_PARAM(arg);

    //Pend on ISR Event Flags
    OS_FLAGS flags;

    RTOS_ERR err;
    while (1)
    {

    }
}

/***************************************************************************//**
 * Holtzman Mass Manager Task.
 *
 ******************************************************************************/
static void HM_manager_task(void *arg)
{
    PP_UNUSED_PARAM(arg);

    //Pend on ISR Event Flags
    OS_FLAGS flags;

    RTOS_ERR err;
    while (1)
    {

    }
}

/***************************************************************************//**
 * Game Monitor Task.
 *
 ******************************************************************************/
static void gameMonitor_task(void *arg)
{
    PP_UNUSED_PARAM(arg);

    //Pend on ISR Event Flags
    OS_FLAGS flags;

    RTOS_ERR err;
    while (1)
    {

    }
}
