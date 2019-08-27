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

#ifndef PWM_H
#define PWM_H

#ifdef __cplusplus
  extern "C" {
#endif

#include <stdint.h>
#include "HALru.h"

#define   PWM_GROUP_A                                      0
#define   PWM_GROUP_B                                      1
#define   PWM_GROUP_C                                      2

#define   PWM_AUTOMATIC_MODE                               0
#define   PWM_MANUAL_MODE                                  1

void vEnablePWMMode(volatile uint8_t* ui8pTIMERGroup, uint8_t ui8PWMMode);
void vDisablePWMMode(volatile uint8_t* ui8pTIMERGroup);
uint16_t ui16GetTIMERMaxDutyCicle(volatile uint8_t* ui8pTIMERGroup);
void vSetPWMAbsoluteDutyCicle(volatile uint8_t* ui8pTIMERGroup, uint8_t ui8PWMGroup, uint16_t ui16DutyCicle);
void vSetPWMRelativeDutyCicle(volatile uint8_t* ui8pTIMERGroup, uint8_t ui8PWMGroup, uint16_t ui16DutyCicle, uint16_t ui16Resolution);

#ifdef __cplusplus
  }
#endif

#endif
