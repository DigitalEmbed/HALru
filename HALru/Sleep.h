//! HALru Version 2.0b
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

#ifndef Sleep_H
#define Sleep_H

#ifdef __cplusplus
  extern "C" {
#endif

#include <avr/io.h>

//! Macro: Sleep Macros
/*!
  This macros are for facilitate the use of this library.
*/
#define   SLEEP_IDLE_MODE                         0
#define   SLEEP_ADC_NOISE_REDUCTION_MODE          1
#define   SLEEP_POWER_DOWN_MODE                   2
#define   SLEEP_POWER_SAVE_MODE                   3
#define   SLEEP_STANDBY_MODE                      6
#define   SLEEP_EXTENDED_STANDBY_MODE             7


#define   vSetSleepMode(ui8SleepMode)             vCopyBits(SMCR, 1, ui8SleepMode, 0, 3)

//! Macro: Sleep Enabler
/*!
  Enables sleep mode.
*/
#define   vSleepEnable()                          vSetBit(SMCR, SE)

//! Macro: Sleep Disabler
/*!
  Disables sleep mode.
*/
#define   vSleepDisable()                         vEraseBit(SMCR, SE)

//! Macro: Waiting for Interrupts
/*!
  Enables all interrupts and go to sleep.
*/
#define   vWaitingForInterrupts()                 vEnableAllInterrupts(); while(1) { vSleepEnable(); }

#ifdef __cplusplus
  }
#endif

#endif
