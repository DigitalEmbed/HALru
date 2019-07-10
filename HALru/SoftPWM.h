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

#ifndef SoftPWM_H
#define SoftPWM_H

#ifdef __cplusplus
  extern "C" {
#endif

#include <stdint.h>
#include <string.h>
#include "GPIO.h"
#include "TIMER.h"

//! Type Definition: pwm_group_t struct.
/*!
  This struct creates a pwm group "object".
*/
typedef struct{
  volatile uint8_t* ioGroup;
  volatile uint8_t* tmrGroup;
  volatile uint8_t ui8TickCounter;
  volatile uint8_t ui8pActivedPinsVector[8];
  volatile uint8_t ui8pDutyCicleVector[8];
} pwm_group_t;

void vInitSoftPWM(pwm_group_t* pwmGroup, volatile uint8_t* ioGroup, volatile uint8_t* tmrGroup);      /*!< Void type function. */
void vDisableSoftPWM(pwm_group_t* pwmGroup);                                                          /*!< Void type function. */
void vEnableSoftPWM(pwm_group_t* pwmGroup);                                                           /*!< Void type function. */
void vSetSoftPWMPeriodUS(pwm_group_t* pwmGroup, uint32_t ui32PeriodUS);                               /*!< Void type function. */
void vSetSoftPWMFrequencyHZ(pwm_group_t* pwmGroup, uint16_t ui16FrequencyHZ);                         /*!< Void type function. */
void vUnsetSoftPWMPin(pwm_group_t* pwmGroup, uint8_t ui8Pin);                                         /*!< Void type function. */
void vSetSoftPWMPin(pwm_group_t* pwmGroup, uint8_t ui8Pin, uint8_t ui8DutyCicle);                     /*!< Void type function. */

#ifdef __cplusplus
  }
#endif

#endif
