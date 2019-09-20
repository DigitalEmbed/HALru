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

#ifndef Watchdog_H
#define Watchdog_H

#ifdef __cplusplus
  extern "C" {
#endif

#include <stdint.h>
#include <avr/io.h>

#define     WHATCHDOG_TIMER_16MS                    0
#define     WHATCHDOG_TIMER_32MS                    1
#define     WHATCHDOG_TIMER_64MS                    2
#define     WHATCHDOG_TIMER_125MS                   3
#define     WHATCHDOG_TIMER_250MS                   4
#define     WHATCHDOG_TIMER_500MS                   5
#define     WHATCHDOG_TIMER_1S                      6
#define     WHATCHDOG_TIMER_2S                      7
#define     WHATCHDOG_TIMER_4S                      8
#define     WHATCHDOG_TIMER_8S                      9

#define     DISABLED_MODE                           0
#define     INTERRUPT_MODE                          1
#define     RESET_MODE                              2
#define     INTERRUPT_RESET_MODE                    3

#define     vIgnoreWatchdogRequest()                vEraseBit(WDTCSR, WDIF)

void vStartWatchdog(uint8_t ui8Mode, uint8_t ui8Time);
void vStopWatchdog(void);
void vAttachWatchdogInterrupt(void (*vInterruptFunction)(void*), void* vpArgument);
void vDettachWatchdogInterrupt(void);

#ifdef __cplusplus
  }
#endif

#endif