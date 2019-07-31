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

#ifndef GPIO_H
#define GPIO_H

#ifdef __cplusplus
  extern "C" {
#endif

#include <stdint.h>
#include "avr/io.h"
#include "HALru.h"

//! Macro: GPIO Macros
/*!
  This macros are for facilitate the use of this library.
*/
#define   OUTPUT_MODE                                   0
#define   INPUT_MODE                                    1

#define   LOW_LEVEL                                     0
#define   HIGH_LEVEL                                    1

#define   IO_GROUP_B                                    &PINB
#define   IO_GROUP_C                                    &PINC
#define   IO_GROUP_D                                    &PIND

#if defined(ARDUINO_AVR_MEGA2560) || defined(ARDUINO_AVR_MEGA)
  #define   IO_GROUP_A                                  &PINA
  #define   IO_GROUP_E                                  &PINE
  #define   IO_GROUP_F                                  &PINF
  #define   IO_GROUP_G                                  &PING
  #define   IO_GROUP_H                                  &PINH
  #define   IO_GROUP_K                                  &PINK
  #define   IO_GROUP_L                                  &PINL
  #define   IO_GROUP_J                                  &PINJ
#endif

#define   regPIN(ioGroup)                               (ioGroup)
#define   regDDR(ioGroup)                               ((volatile uint8_t*) ioGroup + 1)
#define   regPORT(ioGroup)                              ((volatile uint8_t*) ioGroup + 2)

//! Macro: Set Pin Mode
/*!
  Set mode of a GPIO pin.
  \param ioGroup is an undefined parameter. It's the GPIO group pin (IO_GROUP_X).
  \param ui8Pin is an undefined parameter. It's the pin number of a GPIO group (0 to 7).
  \param ui8Mode is an undefined parameter. It's OUTPUT_MODE or INPUT_MODE.
*/
#define   vSetGPIOPinMode(ioGroup, ui8Pin, ui8Mode)     if (ui8Mode == OUTPUT_MODE){\
                                                          vSetBit(*regDDR(ioGroup), ui8Pin);\
                                                        }\
                                                        else{\
                                                          vEraseBit(*regDDR(ioGroup), ui8Pin);\
                                                        }

//! Macro: Enable GPIO PullUp Resistor
/*!
  Enable PullUp resistor on a GPIO Pin.
  \param ioGroup is an undefined parameter. It's the GPIO group pin (IO_GROUP_X).
  \param ui8Pin is an undefined parameter. It's the pin number of a GPIO group (0 to 7).
*/
#define   vEnablePullUp(ioGroup, ui8Pin)                vSetBit(*regPORT(ioGroup), ui8Pin)

//! Macro: Set GPIO Pin
/*!
  Write high logic-level on a GPIO Pin.
  \param ioGroup is an undefined parameter. It's the GPIO group pin (IO_GROUP_X).
  \param ui8Pin is an undefined parameter. It's the pin number of a GPIO group (0 to 7).
*/

#define   vSetGPIOPin(ioGroup, ui8Pin)                  vSetBit(*regPORT(ioGroup), ui8Pin)

//! Macro: Unset GPIO Pin
/*!
  Write low logic-level on a GPIO Pin.
  \param ioGroup is an undefined parameter. It's the GPIO group pin (IO_GROUP_X).
  \param ui8Pin is an undefined parameter. It's the pin number of a GPIO group (0 to 7).
*/
#define   vUnsetGPIOPin(ioGroup, ui8Pin)                vEraseBit(*regPORT(ioGroup), ui8Pin)

//! Macro: Toggle GPIO Pin
/*!
  Toggle the logic-level on a GPIO Pin.
  \param ioGroup is an undefined parameter. It's the GPIO group pin (IO_GROUP_X).
  \param ui8Pin is an undefined parameter. It's the pin number of a GPIO group (0 to 7).
*/
#define   vToggleGPIOPin(ioGroup, ui8Pin)               vInvertBit(*regPORT(ioGroup), ui8Pin)

//! Macro: Digital Write on a GPIO Pin
/*!
  Write a logic-level on a GPIO Pin.
  \param ioGroup is an undefined parameter. It's the GPIO group pin (IO_GROUP_X).
  \param ui8Pin is an undefined parameter. It's the pin number of a GPIO group (0 to 7).
  \param ui8Mode is an undefined parameter. It's HIGH_LEVEL or LOW_LEVEL.
*/
#define   vGPIODigitalWrite(ioGroup, ui8Pin, ui8Mode)   (ui8Mode == HIGH_LEVEL) ? vSetPin(ioGroup, ui8Pin) : vUnsetPin (ioGroup, ui8Pin)

//! Macro: Digital Read on a GPIO Pin
/*!
  Read a logic-level on a GPIO Pin.
  \param ioGroup is an undefined parameter. It's the GPIO group pin (IO_GROUP_X).
  \param ui8Pin is an undefined parameter. It's the pin number of a GPIO group (0 to 7).
  \return Returns the logic-level.
*/
#define   ui8GPIODigitalRead(ioGroup, ui8Pin)           ui8ReadBit(*regPORT(ioGroup), ui8Pin)

#ifdef __cplusplus
  }
#endif

#endif
