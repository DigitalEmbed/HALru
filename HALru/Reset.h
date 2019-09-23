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

//! Macro: Reset Macros
/*!
  This macros are for facilitate the use of this library.
*/
#define     WATCHDOG_RESET                                      4
#define     BROWN_OUT_RESET                                     3
#define     EXTERNAL_RESET                                      2
#define     POWER_ON_RESET                                      1

//! Macro: System Reset Forcer
/*!
  Forces system reset.
  /param ui8Mode is a unsigned 8-bit integer. The available modes are: DISABLED_MODE, INTERRUPT_MODE, RESET_MODE and INTERRUPT_RESET_MODE.
*/
#define     vForceSystemReset(ui8Mode)                          vEraseBit(SREG, 7);\
                                                                WDTCSR |= (1 << WDCE) | (1 << WDE);\
                                                                WDTCSR |= ((ui8Mode & 1) << WDIE) | ((ui8Mode & 2) << WDE);\
                                                                vSetBit(SREG, 7);\
                                                                while(1);

//! Macro: Watchdog Restart Checker
/*!
  Checks if occurred system restart by watchdog.
  \return Returns 1 in positive case, or 0 in contrary case.
*/
#define     ui8WatchdogResetOccurred()                          ui8ReadBit(ui8GetSystemResetReason(), WATCHDOG_RESET)

//! Macro: Brown Out Restart Checker
/*!
  Checks if occurred system restart by brown out.
  \return Returns 1 in positive case, or 0 in contrary case.
*/
#define     ui8BrownOutResetOccurred()                          ui8ReadBit(ui8GetSystemResetReason(), BROWN_OUT_RESET)

//! Macro: External Restart Checker
/*!
  Checks if occurred system restart by reset pin.
  \return Returns 1 in positive case, or 0 in contrary case.
*/
#define     ui8ExternalResetOccurred()                          ui8ReadBit(ui8GetSystemResetReason(), EXTERNAL_RESET)

//! Macro: Power On Restart Checker
/*!
  Checks if occurred system restart by power suply on VCC pin.
  \return Returns 1 in positive case, or 0 in contrary case.
*/
#define     ui8PowerOnResetOccurred()                           ui8ReadBit(ui8GetSystemResetReason(), POWER_ON_RESET)

uint8_t ui8GetSystemResetReason(void);

#ifdef __cplusplus
  }
#endif

#endif
