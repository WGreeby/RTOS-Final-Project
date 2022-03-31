#include <stdint.h>
#include <stdbool.h>
#include "app.h"
#include "cpu.h"
#include "cmu.h"
#include "em_chip.h"
#include "os.h"
#include "bsp_os.h"
#include "os_trace.h"
#include "em_emu.h"

/***************************************************************************//**
 * @brief
 *   Interrupt handler to service pressing of buttons
 ******************************************************************************/
void GPIO_EVEN_IRQHandler(void)
{
  OSIntEnter();
  //Referenced from Silicon Lab's reference code for GPIO interrupts
  GPIO_IntClear(0x5555); //Clear even interrupts
  //Post Event Flags for buttonInput0
  buttonFlags(even);
  OSIntExit();
}

/***************************************************************************//**
 * @brief
 *   Interrupt handler to service pressing of buttons
 ******************************************************************************/
void GPIO_ODD_IRQHandler(void)
{
  OSIntEnter();
  //Referenced from Silicon Lab's reference code for GPIO interrupts
  GPIO_IntClear(0xAAAA); //Clear odd interrupts
  //Post Event Flags for buttonInput1
  buttonEvent(odd);
  OSIntExit();
}

int main(void)
{
  EMU_DCDCInit_TypeDef dcdcInit = EMU_DCDCINIT_DEFAULT;
  CMU_HFXOInit_TypeDef hfxoInit = CMU_HFXOINIT_DEFAULT;

  /* Chip errata */
  CHIP_Init();

  /* Init DCDC regulator and HFXO with kit specific parameters */
  /* Init DCDC regulator and HFXO with kit specific parameters */
  /* Initialize DCDC. Always start in low-noise mode. */
  EMU_EM23Init_TypeDef em23Init = EMU_EM23INIT_DEFAULT;
  EMU_DCDCInit(&dcdcInit);
  em23Init.vScaleEM23Voltage = emuVScaleEM23_LowPower;
  EMU_EM23Init(&em23Init);
  CMU_HFXOInit(&hfxoInit);

  /* Switch HFCLK to HFRCO and disable HFRCO */
  CMU_OscillatorEnable(cmuOsc_HFRCO, true, true);
  CMU_ClockSelectSet(cmuClock_HF, cmuSelect_HFRCO);
  CMU_OscillatorEnable(cmuOsc_HFXO, false, false);

  cmu_open();

  BSP_SystemInit();                                           /* Initialize System.                                   */
  RTOS_ERR  err;

  CPU_Init();
  OS_TRACE_INIT();
  OSInit(&err);
  EFM_ASSERT(RTOS_ERR_CODE_GET(err) == RTOS_ERR_NONE); //Check for OS initialization errors


  //Initialize the application
  app_init();
  //Enable the button interrupts
  NVIC_EnableIRQ(GPIO_EVEN_IRQn);
  NVIC_EnableIRQ(GPIO_ODD_IRQn);
  GPIO_ExtIntConfig(BUTTON0_port, BUTTON0_pin, true, true, true);
  GPIO_ExtIntConfig(BUTTON1_port, BUTTON1_pin, true, true, true);


  //Start the operating system
  OSStart(&err);
  EFM_ASSERT(RTOS_ERR_CODE_GET(err) == RTOS_ERR_NONE); //Check for OS start errors

}
