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

#ifndef HALru_H
#define HALru_H

#ifdef __cplusplus
  extern "C" {
#endif

typedef void (*pfunc_t)(void*);
typedef struct {
  pfunc_t vInterruptFunction;
  void* vpArgument;
} isr_t;

/*!
  MemoryManager system libraries.
*/
#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "EmbeddedTools.h"
#include "Atomic.h"
#include "Print.h"
#include "Power.h"
#include "GPIO.h"
#include "USART.h"
#include "TIMER.h"
#include "SoftPWM.h"
#include "EXTINT.h"
#include "PCINT.h"
#include "ADC.h"

//! Configuration: Micro controler configurations
/*!
  Configure the micro controler here.
*/
#define   DEVICE_CLOCK_HZ             16000000

//! Macro: Enable All Interrupts
/*!
  Enable all interrupts.
*/
#define   vEnableAllInterrupts()      sei()

//! Macro: Disable All Interrupts
/*!
  Disable all interrupts.
*/
#define   vDisableAllInterrupts()     cli()

#ifdef __cplusplus
  }
#endif

#endif
