//! winTerface Version 1.0b
/*!
  This code file was written by Jorge Henrique Moreira Santana and is under
  the GNU GPLv3 license. All legal rights are reserved.

  Permissions of this copyleft license are conditioned on making available
  complete source code of licensed works and modifications under the same
  license or the GNU GPLv3. Copyright and license notices must be preserved.
  Contributors provide an express grant of patent rights. However, a larger
  work using the licensed work through interfaces provided by the licensed
  work may be distributed under different terms and without source code for
  the larger work.

  * Permissions:
    -> Commercial use;
    -> Modification;
    -> Distribuition;
    -> Patent Use;
    -> Private Use;

  * Limitations:
    -> Liability;
    -> Warranty;

  * Conditions:
    -> License and copyright notice;
    -> Disclose source;
    -> State changes;
    -> Same license (library);

  For more informations, check the LICENSE document. If you want to use a
  commercial product without having to provide the source code, send an email
  to jorge_henrique_123@hotmail.com to talk.
*/

#ifndef GPIO_Class_H
#define GPIO_Class_H

#ifdef __cplusplus
  extern "C" {
#endif

#include "HALru.h"
#include <avr/pgmspace.h>

//! Macro: Interrupt Mode Macros
/*!
  This macros are for facilitate the use of this library.
*/
#define   LOW_LEVEL_MODE                                                  LOW_LEVEL_INTERRUPT
#define   LEVEL_CHANGE_MODE                                               LEVEL_CHANGE_INTERRUPT
#define   FALLING_BOARD_MODE                                              FALLING_BOARD_INTERRUPT
#define   RISING_BOARD_MODE                                               RISING_BOARD_INTERRUPT

//! Macro: gpio_t "object constructor"
/*!
  "Construct" a gpio_t "object".
  \param ui8pGPIOGroup is an undefined parameter. It's the GPIO group pin (ui8p_GROUP_X).
  \param ui8Pin is an undefined parameter. It's the pin number of a GPIO group (0 to 7).
  \return Returns the object.
*/
#define   NewGPIO(ioGroup, ioPin)                                         {.ui8pGPIOGroup = ioGroup, .ui8pTIMERGroup = NULL, .ui8Pin = ioPin, .ui8PWMGroup = 4}

//! Type Definition: gpio_t
/*!
  This is a "class" of gpio_t type.
*/
typedef struct {
  volatile uint8_t* ui8pGPIOGroup;                                                                                                /*!< volatile uint8_t pointer type. */
  volatile uint8_t* ui8pTIMERGroup;                                                                                               /*!< volatile uint8_t pointer type. */
  uint8_t ui8Pin;                                                                                                                 /*!< uint8_t type. */
  uint8_t ui8PWMGroup;                                                                                                            /*!< uint8_t type. */
} gpio_t;

//! Type Definition: hpwm_manager_t
/*!
  This is a "class" of hpwm_manager_t type.
*/
typedef struct {
  void (*vSetFrequencyHZ)(gpio_t* ioObjectGPIO, uint32_t ui32FrequencyHZ);                                                        /*!< void "method". */
  void (*vSetPeriodUS)(gpio_t* ioObjectGPIO, uint32_t ui32PeriodUS);                                                              /*!< void "method". */
  void (*vSetAutomaticMode)(gpio_t* ioObjectGPIO);                                                                                /*!< void "method". */
  void (*vSetManualMode)(gpio_t* ioObjectGPIO);                                                                                   /*!< void "method". */
  void (*vDisable)(gpio_t* ioObjectGPIO);                                                                                         /*!< void "method". */
  uint16_t (*ui16GetMaxDutyCicle)(gpio_t* ioObjectGPIO);                                                                          /*!< uint16_t "method". */
  void (*vSetAbsoluteDutyCicle)(gpio_t* ioObjectGPIO, uint16_t ui16DutyCicle);                                                    /*!< void "method". */
  void (*vSetRelativeDutyCicle)(gpio_t* ioObjectGPIO, uint16_t ui16DutyCicle, uint16_t ui16Resolution);                           /*!< void "method". */
} hpwm_manager_t;

//! Type Definition: resistor_manager_t
/*!
  This is a "class" of resistor_manager_t type.
*/
typedef struct {
  void (*vEnable)(gpio_t* ioObjectGPIO);                                                                                          /*!< void "method". */
  void (*vDisable)(gpio_t* ioObjectGPIO);                                                                                         /*!< void "method". */
} resistor_manager_t;

//! Type Definition: isr_gpio_manager_t
/*!
  This is a "class" of isr_gpio_manager_t type.
*/
typedef struct {
  void (*vAttach)(gpio_t* ioObjectGPIO, uint8_t ui8ActivationMode, void (*vInterruptFunction)(void*), void* vpArgument);          /*!< void "method". */
  void (*vDetach)(gpio_t* ioObjectGPIO);                                                                                          /*!< void "method". */
  void (*vEnable)(gpio_t* ioObjectGPIO);                                                                                          /*!< void "method". */
  void (*vDisable)(gpio_t* ioObjectGPIO);                                                                                         /*!< void "method". */
} isr_gpio_manager_t;

//! Type Definition: GPIO_manager_t
/*!
  This is a "class" of GPIO_manager_t type.
*/
typedef struct {
  void (*vSetOutputMode)(gpio_t* ioObjectGPIO);                                                                                   /*!< void "method". */
  void (*vSetInputMode)(gpio_t* ioObjectGPIO);                                                                                    /*!< void "method". */
  void (*vTurnOn)(gpio_t* ioObjectGPIO);                                                                                          /*!< void "method". */
  void (*vTurnOff)(gpio_t* ioObjectGPIO);                                                                                         /*!< void "method". */
  void (*vToggle)(gpio_t* ioObjectGPIO);                                                                                          /*!< void "method". */
  void (*vWrite)(gpio_t* ioObjectGPIO, uint8_t ui8LogicalLevel);                                                                  /*!< void "method". */
  uint8_t (*ui8Read)(gpio_t* ioObjectGPIO);                                                                                       /*!< uint8_t "method". */
  hpwm_manager_t PWM;                                                                                                             /*!< PWM manager "object". */
  resistor_manager_t PullUp;                                                                                                      /*!< PullUp resistor manager "object". */
  isr_gpio_manager_t Interrupt;                                                                                                   /*!< Interrupt manager "object". */
} gpio_manager_t;

extern const gpio_manager_t GPIO PROGMEM;                                                                                         /*!< GPIO manager "object". */

#ifdef __cplusplus
  }
#endif

#endif
