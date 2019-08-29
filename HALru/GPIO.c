#include "GPIO.h"

static void vPrivateMethodOutputMode(gpio_t* ioPin);                          /*!< void "method". */
static void vPrivateMethodInputMode(gpio_t* ioPin);                           /*!< void "method". */
static void vPrivateMethodEnablePullUp(gpio_t* ioPin);                        /*!< void "method". */
static void vPrivateMethodDisablePullUp(gpio_t* ioPin);                       /*!< void "method". */
static void vPrivateMethodSet(gpio_t* ioPin);                                 /*!< void "method". */
static void vPrivateMethodUnset(gpio_t* ioPin);                               /*!< void "method". */
static void vPrivateMethodToggle(gpio_t* ioPin);                              /*!< void "method". */
static void vPrivateMethodWrite(gpio_t* ioPin, uint8_t ui8LogicalLevel);      /*!< void "method". */
static uint8_t ui8PrivateMethodRead(gpio_t* ioPin);                           /*!< uint8_t "method". */

/*!
  GPIO object "constructor".
*/
gpio_manager_t GPIO = {
  &vPrivateMethodOutputMode,
  &vPrivateMethodInputMode,
  &vPrivateMethodEnablePullUp,
  &vPrivateMethodDisablePullUp,
  &vPrivateMethodSet,
  &vPrivateMethodUnset,
  &vPrivateMethodToggle,
  &vPrivateMethodWrite,
  &ui8PrivateMethodRead
};

//! Private Method: Output Mode Setter
/*!
  Set a GPIO "object" to output mode.
  \param ioPin is a gpio_t "object".
*/
static void vPrivateMethodOutputMode(gpio_t* ioPin){
  vSetGPIOPinMode(ioPin->ui8pGroup, ioPin->ui8Pin, OUTPUT_MODE); 
}

//! Private Method: Input Mode Setter
/*!
  Set a GPIO "object" to input mode and enable pull-up resistor.
  \param ioPin is a gpio_t "object".
*/
static void vPrivateMethodInputMode(gpio_t* ioPin){
  vSetGPIOPinMode(ioPin->ui8pGroup, ioPin->ui8Pin, INPUT_MODE);
  vEnableGPIOPullUp(ioPin->ui8pGroup, ioPin->ui8Pin);
}

//! Private Method: PullUp Resistor Enabler
/*!
  Enable pull-up resistor.
  \param ioPin is a gpio_t "object".
*/
static void vPrivateMethodEnablePullUp(gpio_t* ioPin){
  vEnableGPIOPullUp(ioPin->ui8pGroup, ioPin->ui8Pin); 
}

//! Private Method: PullUp Resistor Disabler
/*!
  Disable pull-up resistor.
  \param ioPin is a gpio_t "object".
*/
static void vPrivateMethodDisablePullUp(gpio_t* ioPin){
  vDisableGPIOPullUp(ioPin->ui8pGroup, ioPin->ui8Pin); 
}

//! Private Method: GPIO High Logical Level Setter
/*!
  Set high logical level on a GPIO.
  \param ioPin is a gpio_t "object".
*/
static void vPrivateMethodSet(gpio_t* ioPin){
  vSetGPIOPin(ioPin->ui8pGroup, ioPin->ui8Pin);
}

//! Private Method: GPIO Low Logical Level Setter
/*!
  Set low logical level on a GPIO.
  \param ioPin is a gpio_t "object".
*/
static void vPrivateMethodUnset(gpio_t* ioPin){
  vUnsetGPIOPin(ioPin->ui8pGroup, ioPin->ui8Pin);
}

//! Private Method: GPIO Logical Level Toggler
/*!
  Toggle GPIO logical level.
  \param ioPin is a gpio_t "object".
*/
static void vPrivateMethodToggle(gpio_t* ioPin){
  vToggleGPIOPin(ioPin->ui8pGroup, ioPin->ui8Pin);
}

//! Private Method: GPIO Logical Level Writer
/*!
  Write a logical level on a GPIO.
  \param ioPin is a gpio_t "object".
  \param ui8LogicalLevel is a unsigned 8-bit integer. That is a logical level (HIGH_LEVEL or LOW_LEVEL).
*/
static void vPrivateMethodWrite(gpio_t* ioPin, uint8_t ui8LogicalLevel){
  vGPIODigitalWrite(ioPin->ui8pGroup, ioPin->ui8Pin, ui8LogicalLevel);
}

//! Private Method: GPIO Logical Level Reader
/*!
  Read a GPIO logical level.
  \param ioPin is a gpio_t "object".
*/
static uint8_t ui8PrivateMethodRead(gpio_t* ioPin){
  return ui8GPIODigitalRead(ioPin->ui8pGroup, ioPin->ui8Pin);
}