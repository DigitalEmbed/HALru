//! winTerface Version 1.0b
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

#ifndef System_Class_H
#define System_Class_H

#ifdef __cplusplus
  extern "C" {
#endif

#include "HALru.h"
#include <avr/pgmspace.h>

//! Type Definition: peripheral_manager_t
/*!
  This is a "class" of peripheral_manager_t type.
*/
typedef struct {
    void (*vTurnOn)(void);                                                              /*!< void "method". */
    void (*vTurnOff)(void);                                                             /*!< void "method". */
} peripheral_manager_t;

//! Type Definition: sleep_manager_t
/*!
  This is a "class" of sleep_manager_t type.
*/
typedef struct {
    void (*vSetMode1)(void);                                                            /*!< void "method". */
    void (*vSetMode2)(void);                                                            /*!< void "method". */
    void (*vSetMode3)(void);                                                            /*!< void "method". */
    void (*vSetMode4)(void);                                                            /*!< void "method". */
    void (*vSetMode5)(void);                                                            /*!< void "method". */
    void (*vSetMode6)(void);                                                            /*!< void "method". */
    void (*vSleep)(void);                                                               /*!< void "method". */
} sleep_manager_t;

//! Type Definition: power_manager_t
/*!
  This is a "class" of power_manager_t type.
*/
typedef struct {
    sleep_manager_t Sleep;                                                              /*!< Sleep manager "object". */
    peripheral_manager_t AllPeripherals;                                                /*!< AllPeripherals manager "object". */
    peripheral_manager_t ADConverter;                                                   /*!< ADConverter manager "object". */
    peripheral_manager_t USART0;                                                        /*!< USART0 manager "object". */
    #if defined(ARDUINO_AVR_MEGA2560) || defined(ARDUINO_AVR_MEGA)
        peripheral_manager_t USART1;                                                    /*!< USART1 manager "object". */
        peripheral_manager_t USART2;                                                    /*!< USART2 manager "object". */
        peripheral_manager_t USART3;                                                    /*!< USART3 manager "object". */
    #endif
    peripheral_manager_t SPI;                                                           /*!< SPI manager "object". */
    peripheral_manager_t TIMER0;                                                        /*!< TIMER0 manager "object". */
    peripheral_manager_t TIMER1;                                                        /*!< TIMER1 manager "object". */
    peripheral_manager_t TIMER2;                                                        /*!< TIMER2 manager "object". */
    #if defined(ARDUINO_AVR_MEGA2560) || defined(ARDUINO_AVR_MEGA)
        peripheral_manager_t TIMER3;                                                    /*!< TIMER3 manager "object". */
        peripheral_manager_t TIMER4;                                                    /*!< TIMER4 manager "object". */
        peripheral_manager_t TIMER5;                                                    /*!< TIMER5 manager "object". */
    #endif
    peripheral_manager_t I2C;                                                           /*!< I2C manager "object". */
} power_manager_t;

//! Type Definition: isr_watchdog_object_t
/*!
  This is a "class" of isr_watchdog_object_t type.
*/
typedef struct{
  void (*vAttach)(void (*vInterruptFunction)(void*), void* vpArgument);                 /*!< void "method". */
  void (*vDetach)(void);                                                                /*!< void "method". */
} isr_watchdog_object_t;

//! Type Definition: watchdog_manager_t
/*!
  This is a "class" of watchdog_manager_t type.
*/
typedef struct {
  isr_watchdog_object_t Interrupt;                                                      /*!< Interrupt manager "object". */
  void (*vIgnoreRequest)(void);                                                         /*!< void "method". */
  void (*vStart)(uint8_t ui8Mode, uint8_t ui8Time);                                     /*!< void "method". */
  void (*vStop)(void);                                                                  /*!< void "method". */
} watchdog_manager_t;

//! Type Definition: reset_reason_manager_t
/*!
  This is a "class" of reset_reason_manager_t type.
*/
typedef struct{
  uint8_t (*ui8CheckWatchdog)(void);                                                    /*!< uint8_t "method". */
  uint8_t (*ui8CheckBrownOut)(void);                                                    /*!< uint8_t "method". */
  uint8_t (*ui8CheckExternal)(void);                                                    /*!< uint8_t "method". */
  uint8_t (*ui8CheckPowerOn)(void);                                                     /*!< uint8_t "method". */
}reset_reason_manager_t;

//! Type Definition: reset_manager_t
/*!
  This is a "class" of reset_manager_t type.
*/
typedef struct{
  void (*vForceReset)(void);                                                            /*!< void "method". */
  void (*vForceFullReset)(void);                                                        /*!< void "method". */
  reset_reason_manager_t Blame;                                                         /*!< Blame manager "object". */
}reset_manager_t;

//! Type Definition: system_manager_t
/*!
  This is a "class" of system_manager_t type.
*/
typedef struct {
  power_manager_t Power;                                                                /*!< Power manager "object". */
  watchdog_manager_t Watchdog;                                                          /*!< Watchdog manager "object". */
  reset_manager_t Reset;                                                                /*!< Reset manager "object". */
} system_manager_t;

extern const system_manager_t System PROGMEM;                                           /*!< System manager "object". */

#ifdef __cplusplus
  }
#endif

#endif
