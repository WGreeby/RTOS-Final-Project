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

#ifndef APP_H
#define APP_H

#include "cmu.h"
#include "gpio.h"
#include "capsense.h"

//Button interrupt enumeration
enum buttonInt {
  even,
  odd
};

//Button Flags Enumerations
enum btn_flag_enum {
  left_button_pressed = (1u << 0),
  right_button_pressed = (1u << 1),
  left_button_released = (1u << 2),
  shield_expired = (1u << 3),
  right_button_released = (1u << 4),
  all_flags = (left_button_pressed | right_button | left_button_released | shield_expired | right_button_released)
};

// Game Data
struct gameData {
  // Data Structure Version
  struct GameRevision rev;
  // Gravity [m/s^2]
  float gravity;
  // Canyon Size [cm]
  int canyonSize;
  // Holtzman Masses ------------ Remember to change size later
  struct HM masses[50];
  // Platform
  struct Platform platform;
  // Holtzman Shield
  struct HMShield shield;
  // Laser
  struct Laser laser;
};

// Data Structure Version
struct GameRevision {
    float version;
    int dateMonth;
    int dateDay;
    int dateYear;
};

// Holtzman Mass
struct HM {
  //Display Diameter [cm]
  int dispDiameter;
  //Initial Conditions [Struct]
  //Initial Velocity [cm/s]
  float velocity;
  //Initial Horizontal Position [mm]
  float initialHPos;
  //User Definied Mode Input [Struct]

};

// Platform
struct Platform {
  // Max Force [N]
  float maxForce;
  // Mass [kg]
  float mass;
  // Length [cm]
  float length;
  // Bounce Mode [Struct]
  struct BounceMode bounceMode;
  // Automatic Control [T/F]
  bool autoControl;
  //Force applied to the platform [N]
  float currentForce;
};

// Holtzman Shield
struct HMShield {
  // Exclusively Passive Bounce Kinetic Energy Reduction [%]
  float exclusivePB_KE_Reduction;
  // Boost [Struct]
  struct Boost boost;
  // Shield Engaged []
  bool shieldBoostEngaged;
};

// Laser
struct Laser {
  // Number of Activations
  int activationCount;
  // Automatic Control [T/F]
  bool autoControl;
  // Engaged [T/F]
  bool engaged;
};

//Velocity
struct Velocity {
  float vx;
  float vy;
};

//Boost
struct Boost {
  float KEincrease;
  float armingWindow;
  float rechargeTime;
};



void app_init(void);
void buttonEvent(int interruptDiv);

#endif  // APP_H
