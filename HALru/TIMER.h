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

#ifndef TIMER_H
#define TIMER_H

#ifdef __cplusplus
  extern "C" {
#endif

#include <stdint.h>
#include "avr/io.h"
#include "avr/interrupt.h"
#include "HALru.h"

//! Macro: TIMER Macros
/*!
  This macros are for facilitate the use of this library.
*/
#define     TIMER_0                                           &TCCR0A
#define     TIMER_1                                           &TCCR1A
#define     TIMER_2                                           &TCCR2A

#if defined(ARDUINO_AVR_MEGA2560) || defined(ARDUINO_AVR_MEGA)
  #define     TIMER_3                                         &TCCR3A
  #define     TIMER_4                                         &TCCR4A
  #define     TIMER_5                                         &TCCR5A
#endif

#if defined(ARDUINO_AVR_MEGA2560) || defined(ARDUINO_AVR_MEGA)
  #define   AMOUNT_OF_TIMERS    6
#else
  #define   AMOUNT_OF_TIMERS    3
#endif

#define     regTCCRA(tmrGroup)                                (tmrGroup)
#define     regTCCRB(tmrGroup)                                ((volatile uint8_t*) tmrGroup + 1)
#define     regTCCRC(tmrGroup)                                (tmrGroup != TIMER_0 && tmrGroup != TIMER_2) ? ((volatile uint8_t*) tmrGroup + 2) : NULL
#define     regTCNTL(tmrGroup)                                (tmrGroup != TIMER_0 && tmrGroup != TIMER_2) ? ((volatile uint8_t*) tmrGroup + 4) : ((volatile uint8_t*) tmrGroup + 2)
#define     regTCNTH(tmrGroup)                                (tmrGroup != TIMER_0 && tmrGroup != TIMER_2) ? ((volatile uint8_t*) tmrGroup + 5) : NULL
#define     regICRL(tmrGroup)                                 (tmrGroup != TIMER_0 && tmrGroup != TIMER_2) ? ((volatile uint8_t*) tmrGroup + 6) : NULL
#define     regICRH(tmrGroup)                                 (tmrGroup != TIMER_0 && tmrGroup != TIMER_2) ? ((volatile uint8_t*) tmrGroup + 7) : NULL
#define     regOCRAL(tmrGroup)                                (tmrGroup != TIMER_0 && tmrGroup != TIMER_2) ? ((volatile uint8_t*) tmrGroup + 8) : ((volatile uint8_t*) tmrGroup + 3)
#define     regOCRAH(tmrGroup)                                (tmrGroup != TIMER_0 && tmrGroup != TIMER_2) ? ((volatile uint8_t*) tmrGroup + 9) : NULL
#define     regOCRBL(tmrGroup)                                (tmrGroup != TIMER_0 && tmrGroup != TIMER_2) ? ((volatile uint8_t*) tmrGroup + 10) : ((volatile uint8_t*) tmrGroup + 4)
#define     regOCRBH(tmrGroup)                                (tmrGroup != TIMER_0 && tmrGroup != TIMER_2) ? ((volatile uint8_t*) tmrGroup + 11) : NULL

#if defined(ARDUINO_AVR_MEGA2560) || defined(ARDUINO_AVR_MEGA)
  #define     regOCRCL(tmrGroup)                              (tmrGroup != TIMER_0 && tmrGroup != TIMER_2) ? ((volatile uint8_t*) tmrGroup + 12) : NULL
  #define     regOCRCH(tmrGroup)                              (tmrGroup != TIMER_0 && tmrGroup != TIMER_2) ? ((volatile uint8_t*) tmrGroup + 13) : NULL
  #define     regTIMSK(tmrGroup)                              (tmrGroup == TIMER_0) ? &TIMSK0 :\
                                                              (tmrGroup == TIMER_1) ? &TIMSK1 :\
                                                              (tmrGroup == TIMER_2) ? &TIMSK2 :\
                                                              (tmrGroup == TIMER_3) ? &TIMSK3 :\
                                                              (tmrGroup == TIMER_4) ? &TIMSK4 : &TIMSK5
  #define     regTIFR(tmrGroup)                               (tmrGroup == TIMER_0) ? &TIFR0 :\
                                                              (tmrGroup == TIMER_1) ? &TIFR1 :\
                                                              (tmrGroup == TIMER_2) ? &TIFR2 :\
                                                              (tmrGroup == TIMER_3) ? &TIFR3 :\
                                                              (tmrGroup == TIMER_4) ? &TIFR4 : &TIFR5
#else
  #define     regTIMSK(tmrGroup)                              (tmrGroup == TIMER_0) ? &TIMSK0 :\
                                                              (tmrGroup == TIMER_1) ? &TIMSK1 : &TIMSK2
  #define     regTIFR(tmrGroup)                               (tmrGroup == TIMER_0) ? &TIFR0 :\
                                                              (tmrGroup == TIMER_1) ? &TIFR1 : &TIFR2
#endif

#define     COMnA1                                            7
#define     COMnA0                                            6
#define     COMnB1                                            5
#define     COMnB0                                            4
#define     COMnC1                                            3
#define     COMnC0                                            2
#define     WGMn1                                             1
#define     WGMn0                                             0

#define     ICNCn                                             7
#define     ICESn                                             6
#define     WGMn3                                             4
#define     WGMn2                                             3
#define     CSn2                                              2
#define     CSn1                                              1
#define     CSn0                                              0

#define     FOCnA                                             7
#define     FOCnB                                             6
#define     FOCnC                                             5

#define     ICIEn                                             5
#define     OCIEnC                                            3
#define     OCIEnB                                            2
#define     OCIEnA                                            1
#define     TOIEn                                             0

#define     ICFn                                              5
#define     OCFnC                                             3
#define     OCFnB                                             2
#define     OCFnA                                             1
#define     TOVn                                              0

#define     TIMER_PS_TURNED_OFF                               0
#define     TIMER_PS_1                                        1
#define     TIMER_PS_8                                        2
#define     TIMER_PS_32                                       3
#define     TIMER_PS_64                                       4
#define     TIMER_PS_128                                      5
#define     TIMER_PS_256                                      6
#define     TIMER_PS_1024                                     7
#define     TIMER_EXTERNAL_CLOCK_RISING_EDGE                  8
#define     TIMER_EXTERNAL_CLOCK_DESCENT_EDGE                 9

#define     MASTER_TIMER                                      0
#define     SUBTIMER_A                                        1
#define     SUBTIMER_B                                        2
#define     SUBTIMER_C                                        3

//! Function: TIMER Interrupt Rester
/*!
  Reset a TIMER counter.
  \param ui8pGroup is a is an undefined parameter. It's the TIMER group (TIMER_X).
*/
#define   vResetTIMERCounter(tmrGroup)                        *(regTCNTL(tmrGroup)) = 0;\
                                                              *(regTCNTH(tmrGroup)) = 0

//! Type Definition: isr_timer_t and args_timer_t
/*!
  This typedef exist for organization purpose.
*/
typedef void(*isr_timer_t)(void*);
typedef void* args_timer_t;

void vTIMERInit(volatile uint8_t* ui8pGroup);                                                                                         /*!< Void type function. */
void vEnableTIMER(volatile uint8_t* ui8pGroup, uint8_t ui8SubTimer);                                                                  /*!< Void type function. */
void vDisableTIMER(volatile uint8_t* ui8pGroup, uint8_t ui8SubTimer);                                                                 /*!< Void type function. */
void vSetTIMERPrescaler(volatile uint8_t* ui8pGroup, uint8_t ui8TIMERPrescaler);                                                      /*!< Void type function. */
void vSetTIMERCounterLimit(volatile uint8_t* ui8pGroup, uint16_t ui16TIMERLimit);                                                     /*!< Void type function. */
void vSetTIMERPeriodMS(volatile uint8_t* ui8pGroup, uint16_t ui16PeriodMS);                                                           /*!< Void type function. */
void vSetTIMERPeriodUS(volatile uint8_t* ui8pGroup, uint32_t ui32PeriodUS);                                                           /*!< Void type function. */
void vIgnoreTIMERRequest(volatile uint8_t* ui8pGroup, uint8_t ui8SubTimer);                                                           /*!< Void type function. */
void vForceTIMERInterrupt(volatile uint8_t* ui8pGroup, uint8_t ui8SubTimer);                                                          /*!< Void type function. */
void vAttachTIMERInterrupt(volatile uint8_t* ui8pGroup, uint8_t ui8SubTimer, isr_timer_t vInterruptFunction, void* vpArgument);       /*!< Void type function. */
void vDettachTIMERInterrupt(volatile uint8_t* ui8pGroup, uint8_t ui8SubTimer);                                                        /*!< Void type function. */

#ifdef __cplusplus
  }
#endif

#endif
