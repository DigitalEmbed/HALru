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

#ifndef EXTINT_H
#define EXTINT_H

#ifdef __cplusplus
  extern "C" {
#endif

#include <stdint.h>
#include <avr/interrupt.h>
#include "HALru.h"

#define   EXTINT_0                                                              0
#define   EXTINT_1                                                              1

#if defined(ARDUINO_AVR_MEGA2560) || defined(ARDUINO_AVR_MEGA)
  #define   EXTINT_2                                                            2
  #define   EXTINT_3                                                            3
  #define   EXTINT_4                                                            4
  #define   EXTINT_5                                                            5
  #define   EXTINT_6                                                            6
  #define   EXTINT_7                                                            7
#endif

#define   LOW_LEVEL_INTERRUPT                                                   0
#define   LEVEL_CHANGE_INTERRUPT                                                1
#define   FALLING_BOARD_INTERRUPT                                               2
#define   RISING_BOARD_INTERRUPT                                                3

#define   vIgnoreEXTINTRequest(extInterruptPin)                                 vSetBit(EIFR, extInterruptPin)

#define   vForceEXTINTInterrupt(extInterruptPin)                                vEraseBit(EIFR, extInterruptPin)

#if defined(ARDUINO_AVR_MEGA2560) || defined(ARDUINO_AVR_MEGA)
  #define   vDefineEXTINTActivationMode(ui8InterruptPin, ui8ActivationMode)     if(ui8InterruptPin < 3){\
                                                                                  vCopyBits(EICRA, (uiDouble(ui8InterruptPin)), ui8ActivationMode, 0, 2);\
                                                                                }\
                                                                                else{\
                                                                                  vCopyBits(EICRB, (uiDouble(uiModule(ui8InterruptPin - 4))), ui8ActivationMode, 0, 2);\
                                                                                }
#else
  #define   vDefineEXTINTActivationMode(ui8InterruptPin, ui8ActivationMode)     vCopyBits(EICRA, (uiDouble(ui8InterruptPin)), ui8ActivationMode, 0, 2)
#endif

//! Type Definition: isr_extint_t and args_extint_t
/*!
  This typedef exist for organization purpose.
*/
typedef void(*isr_extint_t)(void*);
typedef void* args_extint_t;

void vEnableEXTINTInterrupt(uint8_t ui8InterruptPin);                                                         /*!< Void type function. */
void vDisableEXTINTInterrupt(uint8_t ui8InterruptPin);                                                        /*!< Void type function. */
void vAttachEXTINTInterrupt(uint8_t ui8InterruptPin, isr_extint_t vInterruptFunction, void* vpArgument);      /*!< Void type function. */
void vDettachEXTINTInterrupt(uint8_t ui8InterruptPin);                                                        /*!< Void type function. */

#ifdef __cplusplus
  }
#endif

#endif
