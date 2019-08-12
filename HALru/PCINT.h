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

#ifndef PCINT_H
#define PCINT_H

#ifdef __cplusplus
  extern "C" {
#endif

#include <stdint.h>
#include <avr/interrupt.h>
#include "HALru.h"

#define   PCINT_0                                     0
#define   PCINT_1                                     1
#define   PCINT_2                                     2
#define   PCINT_3                                     3
#define   PCINT_4                                     4
#define   PCINT_5                                     5
#define   PCINT_6                                     6
#define   PCINT_7                                     7

#define   PCINT_8                                     8
#define   PCINT_9                                     9
#define   PCINT_10                                    10
#define   PCINT_11                                    11
#define   PCINT_12                                    12
#define   PCINT_13                                    13
#define   PCINT_14                                    14
#define   PCINT_15                                    15

#define   PCINT_16                                    16
#define   PCINT_17                                    17
#define   PCINT_18                                    18
#define   PCINT_19                                    19
#define   PCINT_20                                    20
#define   PCINT_21                                    21
#define   PCINT_22                                    22
#define   PCINT_23                                    23

#define   PCINT_GROUP_0                               0
#define   PCINT_GROUP_1                               1
#define   PCINT_GROUP_2                               2

void vEnablePCINTPin(uint8_t ui8InterruptPin);
void vDisablePCINTPin(uint8_t ui8InterruptPin);
void vEnablePCINTGroup(volatile uint8_t* ui8Group);
void vDisablePCINTGroup(volatile uint8_t* ui8Group);
void vAttachPCINTInterrupt(uint8_t ui8InterruptPin, isr_pfunc_t vInterruptFunction, void* vpArgument);
void vDettachPCINTInterrupt(uint8_t ui8InterruptPin);

#ifdef __cplusplus
  }
#endif

#endif
