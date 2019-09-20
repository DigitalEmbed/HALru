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

#ifndef Reset_H
#define Reset_H

#ifdef __cplusplus
  extern "C" {
#endif

#include <avr/io.h>
#include <EmbeddedTools.h>

#define     WATCHDOG_RESET                                      4
#define     BROWN_OUT_RESET                                     3
#define     EXTERNAL_RESET                                      2
#define     POWER_ON_RESET                                      1

#define     vForceSystemReset(ui8Mode)                          vEraseBit(SREG, 7);\
                                                                WDTCSR |= (1 << WDCE) | (1 << WDE);\
                                                                WDTCSR |= ((ui8Mode & 1) << WDIE) | ((ui8Mode & 2) << WDE);\
                                                                vSetBit(SREG, 7);\
                                                                while(1);

#define     ui8WatchdogResetOccurred(ui8ResetReason)            ui8ReadBit(ui8ResetReason, WATCHDOG_RESET)
#define     ui8BrownOutResetOccurred(ui8ResetReason)            ui8ReadBit(ui8ResetReason, BROWN_OUT_RESET)
#define     ui8ExternalResetOccurred(ui8ResetReason)            ui8ReadBit(ui8ResetReason, EXTERNAL_RESET)
#define     ui8PowerOnResetOccurred(ui8ResetReason)             ui8ReadBit(ui8ResetReason, POWER_ON_RESET)

uint8_t ui8GetSystemResetReason(void);

#ifdef __cplusplus
  }
#endif

#endif
