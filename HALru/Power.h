//! HALru Version 1.0b
/*!
  This code file was written by Jorge Henrique Moreira Santana and is under
  the GNU GPLv3 license. All legal rights are reserved.

  Permissions of this copy left license are conditioned on making available
  complete source code of licensed works and modifications under the same
  license or the GNU GPLv3. Copyright and license notices must be preserved.
  Contributors provide an express grant of patent rights. However, a larger
  work using the licensed work through interfaces provided by the licensed
  work may be distributed under different terms and without source code for
  the larger work.

  * Permissions:
    -" Commercial use;
    -" Modification;
    -" Distribution;
    -" Patent Use;
    -" Private Use;

  * Limitations:
    -" Liability;
    -" Warranty;

  * Conditions:
    -" License and copyright notice;
    -" Disclose source;
    -" State changes;
    -" Same license (library);

  For more informations, check the LICENSE document. If you want to use a
  commercial product without having to provide the source code, send an email
  to jorge_henrique_123@hotmail.com to talk.
*/

#ifndef Power_H
#define Power_H

#ifdef __cplusplus
  extern "C" {
#endif

#include "HALru.h"
#include <avr/io.h>

#define   SLEEP_IDLE_MODE                         0
#define   SLEEP_ADC_NOISE_REDUCTION_MODE          1
#define   SLEEP_POWER_DOWN_MODE                   2
#define   SLEEP_POWER_SAVE_MODE                   3
#define   SLEEP_STANDBY_MODE                      6
#define   SLEEP_EXTENDED_STANDBY_MODE             7

#define   PERIPHERAL_ADC                          0
#define   PERIPHERAL_USART_0                      1
#define   PERIPHERAL_SPI                          2
#define   PERIPHERAL_TIMER_1                      3
#define   PERIPHERAL_TIMER_0                      5
#define   PERIPHERAL_TIMER_2                      6
#define   PERIPHERAL_I2C                          7

#if defined(ARDUINO_AVR_MEGA2560) || defined(ARDUINO_AVR_MEGA)
  #define   PERIPHERAL_USART_1                    10
  #define   PERIPHERAL_USART_2                    11
  #define   PERIPHERAL_USART_3                    12
  #define   PERIPHERAL_TIMER_3                    13
  #define   PERIPHERAL_TIMER_4                    14
  #define   PERIPHERAL_TIMER_5                    15
#endif

#define   vSetSleepMode(ui8SleepMode)             vCopyBits(SMCR, 1, ui8SleepMode, 0, 3)
#define   vSleepEnable()                          vSetBit(SMCR, SE)
#define   vSleepDisable()                         vEraseBit(SMCR, SE)

#if defined(ARDUINO_AVR_MEGA2560) || defined(ARDUINO_AVR_MEGA)
  #define   vTurnOffPeripheral(ui8Peripheral)     if (ui8Peripheral < 10){\
                                                    vSetBit(PRR0, ui8Peripheral);\
                                                  }\
                                                  else{\
                                                    vSetBit(PRR1, uiModule((ui8Peripheral - 10)));\
                                                  }

  #define   vTurnOffAllPeripherals()              PRR0 |= 239;\
                                                  PRR1 |= 63

  #define   vTurnOnPeripheral(ui8Peripheral)      if (ui8Peripheral < 10){\
                                                    vEraseBit(PRR0, ui8Peripheral);\
                                                  }\
                                                  else {\
                                                    vEraseBit(PRR1, uiModule((ui8Peripheral - 10)));\
                                                  }

  #define   vTurnOnAllPeripherals()               PRR0 &= 16;\
                                                  PRR1 &= 192
#else
  #define   vTurnOffPeripheral(ui8Peripheral)     vSetBit(PRR0, ui8Peripheral)
  #define   vTurnOffAllPeripherals()              PRR0 |= 239
  #define   vTurnOnPeripheral(ui8Peripheral)      vEraseBit(PRR0, ui8Peripheral)
  #define   vTurnOnAllPeripherals()               PRR0 &= 192
#endif

#define   vWaitingForInterrupts()                 vEnableAllInterrupts(); while(1) { vSleepEnable(); }

#ifdef __cplusplus
  }
#endif

#endif
