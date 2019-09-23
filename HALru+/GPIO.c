#include "./GPIO.h"

/*!
  Prototype private "methods".
*/
static volatile uint8_t* ui8pPrivateGetTIMERGroup(gpio_t* ioObjectGPIO);                                                                          /*!< uint8_t "method". */
static uint8_t ui8PrivateGetPWMGroup(gpio_t* ioObjectGPIO);                                                                                       /*!< uint8_t "method". */
static uint8_t ui8PrivateGetEXTINTPin(gpio_t* ioObjectGPIO);                                                                                      /*!< uint8_t "method". */
static uint8_t ui8PrivateVerifyPCINTPin(gpio_t* ioObjectGPIO);                                                                                    /*!< uint8_t "method". */
static uint8_t ui8PrivateVerifyEXTINTPin(gpio_t* ioObjectGPIO);                                                                                   /*!< uint8_t "method". */
static uint8_t ui8PrivateGetPCINTPin(gpio_t* ioObjectGPIO);                                                                                       /*!< uint8_t "method". */

/*!
  Prototype public "methods".
*/
static void vPublicOutputMode(gpio_t* ioObjectGPIO);                                                                                              /*!< void "method". */
static void vPublicInputMode(gpio_t* ioObjectGPIO);                                                                                               /*!< void "method". */
static void vPublicEnablePullUp(gpio_t* ioObjectGPIO);                                                                                            /*!< void "method". */
static void vPublicDisablePullUp(gpio_t* ioObjectGPIO);                                                                                           /*!< void "method". */
static void vPublicSet(gpio_t* ioObjectGPIO);                                                                                                     /*!< void "method". */
static void vPublicUnset(gpio_t* ioObjectGPIO);                                                                                                   /*!< void "method". */
static void vPublicToggle(gpio_t* ioObjectGPIO);                                                                                                  /*!< void "method". */
static void vPublicWrite(gpio_t* ioObjectGPIO, uint8_t ui8LogicalLevel);                                                                          /*!< void "method". */
static uint8_t ui8PublicRead(gpio_t* ioObjectGPIO);                                                                                               /*!< uint8_t "method". */
static void vPublicSetAutomaticMode(gpio_t* ioObjectGPIO);                                                                                        /*!< void "method". */
static void vPublicSetManualMode(gpio_t* ioObjectGPIO);                                                                                           /*!< void "method". */
static void vPublicDisablePWM(gpio_t* ioObjectGPIO);                                                                                              /*!< void "method". */
static uint16_t ui16PublicGetMaxDutyCicle(gpio_t* ioObjectGPIO);                                                                                  /*!< uint16_t "method". */
static void vPublicSetAbsoluteDutyCicle(gpio_t* ioObjectGPIO, uint16_t ui16DutyCicle);                                                            /*!< void "method". */
static void vPublicSetRelativeDutyCicle(gpio_t* ioObjectGPIO, uint16_t ui16DutyCicle, uint16_t ui16Resolution);                                   /*!< void "method". */
static void vPublicSetFrequencyHZ(gpio_t* ioObjectGPIO, uint32_t ui32FrequencyHZ);                                                                /*!< void "method". */
static void vPublicSetPeriodUS(gpio_t* ioObjectGPIO, uint32_t ui32PeriodUS);                                                                      /*!< void "method". */
static void vPublicAttachInterrupt(gpio_t* ioObjectGPIO, uint8_t ui8ActivationMode, void (*vInterruptFunction)(void*), void* vpArgument);         /*!< void "method". */
static void vPublicDetachInterrupt(gpio_t* ioObjectGPIO);                                                                                        /*!< void "method". */
static void vPublicEnableInterrupt(gpio_t* ioObjectGPIO);                                                                                         /*!< void "method". */
static void vPublicDisableInterrupt(gpio_t* ioObjectGPIO);                                                                                        /*!< void "method". */

/*!
  GPIO object "constructor".
*/
const gpio_manager_t GPIO PROGMEM = {
  .vSetOutputMode = &vPublicOutputMode,
  .vSetInputMode = &vPublicInputMode,
  .vTurnOn = &vPublicSet,
  .vTurnOff = &vPublicUnset,
  .vToggle = &vPublicToggle,
  .vWrite = &vPublicWrite,
  .ui8Read = &ui8PublicRead,
  .PWM = {
    .vSetPeriodUS = &vPublicSetPeriodUS,
    .vSetFrequencyHZ = &vPublicSetFrequencyHZ,
    .vSetAutomaticMode = &vPublicSetAutomaticMode,
    .vSetManualMode = &vPublicSetManualMode,
    .vDisable = &vPublicDisablePWM,
    .ui16GetMaxDutyCicle = &ui16PublicGetMaxDutyCicle,
    .vSetAbsoluteDutyCicle = &vPublicSetAbsoluteDutyCicle,
    .vSetRelativeDutyCicle = &vPublicSetRelativeDutyCicle
  },
  .PullUp = {
    .vEnable = &vPublicEnablePullUp,
    .vDisable = &vPublicDisablePullUp
  },
  .Interrupt = {
    .vAttach = &vPublicAttachInterrupt,
    .vDetach = &vPublicDetachInterrupt,
    .vEnable = &vPublicEnableInterrupt,
    .vDisable = &vPublicDisableInterrupt
  }
};

/*!
  Definition private "methods".
*/

//! Private Method: TIMER Group Getter
/*!
  Get the TIMER group of a pin.
  \param ioObjectGPIO is a gpio_t "object".
  \return Returns TIMER group pointer.
*/
static volatile uint8_t* ui8pPrivateGetTIMERGroup(gpio_t* ioObjectGPIO){
  
  #if defined(ARDUINO_AVR_MEGA2560) || defined(ARDUINO_AVR_MEGA)
    if (ioObjectGPIO->ui8pGPIOGroup == IO_GROUP_G) {
      return (ioObjectGPIO->ui8Pin == 5) ? TIMER_0 : NULL;
    }
    else if (ioObjectGPIO->ui8pGPIOGroup == IO_GROUP_B) {
      return (ioObjectGPIO->ui8Pin >= 5 && ioObjectGPIO->ui8Pin <= 7) ? TIMER_1 : (ioObjectGPIO->ui8Pin == 4) ? TIMER_2 : NULL;
    }
    else if (ioObjectGPIO->ui8pGPIOGroup == IO_GROUP_E) {
      return (ioObjectGPIO->ui8Pin >= 3 && ioObjectGPIO->ui8Pin <= 5) ? TIMER_3 : NULL;
    }
    else if (ioObjectGPIO->ui8pGPIOGroup == IO_GROUP_H) {
      return (ioObjectGPIO->ui8Pin >= 3 && ioObjectGPIO->ui8Pin <= 5) ? TIMER_4 : (ioObjectGPIO->ui8Pin == 6) ? TIMER_2 : NULL;
    }
    else if (ioObjectGPIO->ui8pGPIOGroup == IO_GROUP_L) {
      return (ioObjectGPIO->ui8Pin >= 3 && ioObjectGPIO->ui8Pin <= 5) ? TIMER_5 : NULL;
    }
  #else
    if (ioObjectGPIO->ui8pGPIOGroup == IO_GROUP_D) {
      return (ioObjectGPIO->ui8Pin == 3) ? TIMER_2 : (ioObjectGPIO->ui8Pin == 5 || ioObjectGPIO->ui8Pin == 6) ? TIMER_0 : NULL;
    }
    else if (ioObjectGPIO->ui8pGPIOGroup == IO_GROUP_B) {
      return (ioObjectGPIO->ui8Pin == 3) ? TIMER_2 : (ioObjectGPIO->ui8Pin == 1 || ioObjectGPIO->ui8Pin == 2) ? TIMER_1 : NULL;
    }
  #endif

  return NULL;
}

//! Private Method: PWM Group Getter
/*!
  Get the PWM group of a pin.
  \param ioObjectGPIO is a gpio_t "object".
  \return Returns PWM group.
*/
static uint8_t ui8PrivateGetPWMGroup(gpio_t* ioObjectGPIO){
  
  #if defined(ARDUINO_AVR_MEGA2560) || defined(ARDUINO_AVR_MEGA)
    if ((ioObjectGPIO->ui8pGPIOGroup == IO_GROUP_E || ioObjectGPIO->ui8pGPIOGroup == IO_GROUP_H || ioObjectGPIO->ui8pGPIOGroup == IO_GROUP_L) && (ioObjectGPIO->ui8Pin >= 3 && ioObjectGPIO->ui8Pin <= 5)){
      return ioObjectGPIO->ui8Pin - 3;
    }
    else if (ioObjectGPIO->ui8pGPIOGroup == IO_GROUP_B && (ioObjectGPIO->ui8Pin >= 5 && ioObjectGPIO->ui8Pin <= 7)){
      return ioObjectGPIO->ui8Pin - 5;
    }
    else if (ioObjectGPIO->ui8pGPIOGroup == IO_GROUP_G && ioObjectGPIO->ui8Pin >= 5){
      return PWM_GROUP_B;
    }
    else if((ioObjectGPIO->ui8pGPIOGroup == IO_GROUP_B || ioObjectGPIO->ui8pGPIOGroup == IO_GROUP_H) && (ioObjectGPIO->ui8Pin == 4 || ioObjectGPIO->ui8Pin == 6)){
      return (ioObjectGPIO->ui8Pin << 1) + 4;
    }
  #else
    if (ioObjectGPIO->ui8pGPIOGroup == IO_GROUP_D) {
      return (ioObjectGPIO->ui8Pin == 6) ? PWM_GROUP_A : (ioObjectGPIO->ui8Pin == 5 || ioObjectGPIO->ui8Pin == 3) ? PWM_GROUP_B : 4;
    }
    else if (ioObjectGPIO->ui8pGPIOGroup == IO_GROUP_B) {
      return (ioObjectGPIO->ui8Pin == 2) ? PWM_GROUP_B : (ioObjectGPIO->ui8Pin == 1 || ioObjectGPIO->ui8Pin == 3) ? PWM_GROUP_A : 4;
    }
  #endif

  return 4;
}

//! Private Method: EXTINT Pin Checker
/*!
  Check if a pin has external interrupt compatibility.
  \param ioObjectGPIO is a gpio_t "object".
  \return Returns 1 if that pin has external interrupt compatibility, or 0 in contrary case.
*/
static uint8_t ui8PrivateVerifyEXTINTPin(gpio_t* ioObjectGPIO){
  
  #if defined(ARDUINO_AVR_MEGA2560) || defined(ARDUINO_AVR_MEGA)
    return (ioObjectGPIO->ui8pGPIOGroup == IO_GROUP_D && (ioObjectGPIO->ui8Pin >= 0 || ioObjectGPIO->ui8Pin <= 3)) || (ioObjectGPIO->ui8pGPIOGroup == IO_GROUP_E && (ioObjectGPIO->ui8Pin >= 4 || ioObjectGPIO->ui8Pin <= 7));
  #else 
    return ioObjectGPIO->ui8pGPIOGroup == IO_GROUP_D && (ioObjectGPIO->ui8Pin == 2 || ioObjectGPIO->ui8Pin == 3);
  #endif

}

//! Private Method: PCINT Pin Checker
/*!
  Check if a pin has change interrupt compatibility.
  \param ioObjectGPIO is a gpio_t "object".
  \return Returns 1 if that pin has change interrupt compatibility, or 0 in contrary case.
*/
static uint8_t ui8PrivateVerifyPCINTPin(gpio_t* ioObjectGPIO){

  #if defined(ARDUINO_AVR_MEGA2560) || defined(ARDUINO_AVR_MEGA)
    return (ioObjectGPIO->ui8pGPIOGroup == IO_GROUP_B) || (ioObjectGPIO->ui8pGPIOGroup == IO_GROUP_K) || (ioObjectGPIO->ui8pGPIOGroup == IO_GROUP_J && ioObjectGPIO->ui8Pin != 7) || (ioObjectGPIO->ui8pGPIOGroup == IO_GROUP_E && ioObjectGPIO->ui8Pin == 0);
  #else
    return 1;
  #endif

  return 255;
}

//! Private Method: EXTINT Pin Getter
/*!
  Get a equivalent EXTINT pin.
  \param ioObjectGPIO is a gpio_t "object".
  \return Returns EXTINT equivalent pin.
*/
static uint8_t ui8PrivateGetEXTINTPin(gpio_t* ioObjectGPIO){
  if (ui8PrivateVerifyEXTINTPin(ioObjectGPIO) == 1){

    #if defined(ARDUINO_AVR_MEGA2560) || defined(ARDUINO_AVR_MEGA)
      return ioObjectGPIO->ui8Pin;
    #else
      return ioObjectGPIO->ui8Pin - 2;
    #endif

  }
  return 255;
}

//! Private Method: PCINT Pin Getter
/*!
  Get a equivalent PCINT pin.
  \param ioObjectGPIO is a gpio_t "object".
  \return Returns PCINT equivalent pin.
*/
static uint8_t ui8PrivateGetPCINTPin(gpio_t* ioObjectGPIO){
  uint8_t ui8ConversionFactor = 0;
  if (ui8PrivateVerifyPCINTPin(ioObjectGPIO) == 1) {
    
    #if defined(ARDUINO_AVR_MEGA2560) || defined(ARDUINO_AVR_MEGA)
      ui8ConversionFactor = (ioObjectGPIO->ui8pGPIOGroup == IO_GROUP_B) ? 0 : (ioObjectGPIO->ui8pGPIOGroup == IO_GROUP_K) ? 16 : (ioObjectGPIO->ui8pGPIOGroup == IO_GROUP_E) ? 8 : 9;
    #else
      ui8ConversionFactor = (ioObjectGPIO->ui8pGPIOGroup == IO_GROUP_B) ? 0 : (ioObjectGPIO->ui8pGPIOGroup == IO_GROUP_C) ? 8 : 16;
    #endif

    return ioObjectGPIO->ui8Pin + ui8ConversionFactor;
  }
  return 255;
}

/*!
  Definition public "methods".
*/

//! Public Method: Output Mode Setter
/*!
  Set a GPIO "object" to output mode.
  \param ioObjectGPIO is a gpio_t "object".
*/
static void vPublicOutputMode(gpio_t* ioObjectGPIO){
  ioObjectGPIO->ui8pTIMERGroup = ui8pPrivateGetTIMERGroup(ioObjectGPIO);
  if (ioObjectGPIO->ui8pTIMERGroup != NULL){
    ioObjectGPIO->ui8PWMGroup = ui8PrivateGetPWMGroup(ioObjectGPIO);
  }
  vSetGPIOPinMode(ioObjectGPIO->ui8pGPIOGroup, ioObjectGPIO->ui8Pin, OUTPUT_MODE); 
}

//! Public Method: Input Mode Setter
/*!
  Set a GPIO "object" to input mode and enable pull-up resistor.
  \param ioObjectGPIO is a gpio_t "object".
*/
static void vPublicInputMode(gpio_t* ioObjectGPIO){
  ioObjectGPIO->ui8pTIMERGroup = NULL;
  ioObjectGPIO->ui8PWMGroup = 4;
  vSetGPIOPinMode(ioObjectGPIO->ui8pGPIOGroup, ioObjectGPIO->ui8Pin, INPUT_MODE);
}

//! Public Method: PullUp Resistor Enabler
/*!
  Enable pull-up resistor.
  \param ioObjectGPIO is a gpio_t "object".
*/
static void vPublicEnablePullUp(gpio_t* ioObjectGPIO){
  vEnableGPIOPullUp(ioObjectGPIO->ui8pGPIOGroup, ioObjectGPIO->ui8Pin); 
}

//! Public Method: PullUp Resistor Disabler
/*!
  Disable pull-up resistor.
  \param ioObjectGPIO is a gpio_t "object".
*/
static void vPublicDisablePullUp(gpio_t* ioObjectGPIO){
  vDisableGPIOPullUp(ioObjectGPIO->ui8pGPIOGroup, ioObjectGPIO->ui8Pin); 
}

//! Public Method: GPIO High Logical Level Setter
/*!
  Set high logical level on a GPIO.
  \param ioObjectGPIO is a gpio_t "object".
*/
static void vPublicSet(gpio_t* ioObjectGPIO){
  vSetGPIOPin(ioObjectGPIO->ui8pGPIOGroup, ioObjectGPIO->ui8Pin);
}

//! Public Method: GPIO Low Logical Level Setter
/*!
  Set low logical level on a GPIO.
  \param ioObjectGPIO is a gpio_t "object".
*/
static void vPublicUnset(gpio_t* ioObjectGPIO){
  vUnsetGPIOPin(ioObjectGPIO->ui8pGPIOGroup, ioObjectGPIO->ui8Pin);
}

//! Public Method: GPIO Logical Level Toggler
/*!
  Toggle GPIO logical level.
  \param ioObjectGPIO is a gpio_t "object".
*/
static void vPublicToggle(gpio_t* ioObjectGPIO){
  vToggleGPIOPin(ioObjectGPIO->ui8pGPIOGroup, ioObjectGPIO->ui8Pin);
}

//! Public Method: GPIO Logical Level Writer
/*!
  Write a logical level on a GPIO.
  \param ioObjectGPIO is a gpio_t "object".
  \param ui8LogicalLevel is a unsigned 8-bit integer. That is a logical level (HIGH_LEVEL or LOW_LEVEL).
*/
static void vPublicWrite(gpio_t* ioObjectGPIO, uint8_t ui8LogicalLevel){
  vGPIODigitalWrite(ioObjectGPIO->ui8pGPIOGroup, ioObjectGPIO->ui8Pin, ui8LogicalLevel);
}

//! Public Method: GPIO Logical Level Reader
/*!
  Read a GPIO logical level.
  \param ioObjectGPIO is a gpio_t "object".
*/
static uint8_t ui8PublicRead(gpio_t* ioObjectGPIO){
  return ui8GPIODigitalRead(ioObjectGPIO->ui8pGPIOGroup, ioObjectGPIO->ui8Pin);
}

//! Public Method: PWM Frequency Setter
/*!
  Set PWM frequency.
  \param ioObjectGPIO is a gpio_t "object".
  \param ui32Frequency is a 32-bits unsigned integer. It's a frequency, in hertz.
*/
static void vPublicSetFrequencyHZ(gpio_t* ioObjectGPIO, uint32_t ui32FrequencyHZ){
  if (ioObjectGPIO->ui8pTIMERGroup != NULL){
    vSetTIMERFrequencyHZ(ioObjectGPIO->ui8pTIMERGroup, ui32FrequencyHZ);
  }
}

//! Public Method: PWM Period Setter
/*!
  Set PWM period.
  \param ioObjectGPIO is a gpio_t "object".
  \param ui32Period is a 32-bits unsigned integer. It's a period, in micro seconds.
*/
static void vPublicSetPeriodUS(gpio_t* ioObjectGPIO, uint32_t ui32PeriodUS){
  if (ioObjectGPIO->ui8pTIMERGroup != NULL){
    vSetTIMERPeriodUS(ioObjectGPIO->ui8pTIMERGroup, ui32PeriodUS);
  }
}

//! Public Method: PWM Automatic Setter
/*!
  Set automatic mode in PWM.
  \param ioObjectGPIO is a gpio_t "object".
*/
static void vPublicSetAutomaticMode(gpio_t* ioObjectGPIO){
  if (ioObjectGPIO->ui8pTIMERGroup != NULL){
    vTIMERInit(ioObjectGPIO->ui8pTIMERGroup);
    vEnablePWMMode(ioObjectGPIO->ui8pTIMERGroup, PWM_AUTOMATIC_MODE);
  }
}

//! Public Method: PWM Manual Setter
/*!
  Set manual mode in PWM.
  \param ioObjectGPIO is a gpio_t "object".
*/
static void vPublicSetManualMode(gpio_t* ioObjectGPIO){
  if (ioObjectGPIO->ui8pTIMERGroup != NULL){
    vTIMERInit(ioObjectGPIO->ui8pTIMERGroup);
    vEnablePWMMode(ioObjectGPIO->ui8pTIMERGroup, PWM_MANUAL_MODE);
  }
}

//! Public Method: PWM Disabler
/*!
  Disable a PWM group.
  \param ioObjectGPIO is a gpio_t "object".
*/
static void vPublicDisablePWM(gpio_t* ioObjectGPIO){
  if (ioObjectGPIO->ui8pTIMERGroup != NULL){
    vDisablePWMMode(ioObjectGPIO->ui8pTIMERGroup);
  }
}

//! Public Method: PWM Max Duty Cicle Getter
/*!
  Get a PWM max duty cicle.
  \param ioObjectGPIO is a gpio_t "object".
  \return Returns PWM max duty cicle.
*/
static uint16_t ui16PublicGetMaxDutyCicle(gpio_t* ioObjectGPIO){
  if (ioObjectGPIO->ui8pTIMERGroup != NULL){
    return ui16GetTIMERMaxDutyCicle(ioObjectGPIO->ui8pTIMERGroup);
  }
  return 0;
}

//! Public Method: PWM Absolute Duty Cicle Setter
/*!
  Set PWM duty cicle.
  \param ioObjectGPIO is a gpio_t "object".
  \param ui16DutyCicle is a 16-bits unsigned integer. It's the duty cicle value.
*/
static void vPublicSetAbsoluteDutyCicle(gpio_t* ioObjectGPIO, uint16_t ui16DutyCicle){
  if (ioObjectGPIO->ui8pTIMERGroup != NULL){
    vSetPWMAbsoluteDutyCicle(ioObjectGPIO->ui8pTIMERGroup, ioObjectGPIO->ui8PWMGroup, ui16DutyCicle);
  }
}

//! Public Method: PWM Relative Duty Cicle Setter
/*!
  Set PWM duty cicle relative of a value.
  \param ioObjectGPIO is a gpio_t "object".
  \param ui16DutyCicle is a 16-bits unsigned integer. It's the duty cicle value.
  \param ui16Resolution is a 16-bits unsigned integer. It's equivalent of a max pwm duty cicle.
*/
static void vPublicSetRelativeDutyCicle(gpio_t* ioObjectGPIO, uint16_t ui16DutyCicle, uint16_t ui16Resolution){
  if (ioObjectGPIO->ui8pTIMERGroup != NULL){
    vSetPWMRelativeDutyCicle(ioObjectGPIO->ui8pTIMERGroup, ioObjectGPIO->ui8PWMGroup, ui16DutyCicle, ui16Resolution);
  }
}

//! Public Method: GPIO Interrupt Function Attacher
/*!
  If the pin is compatible, attach a callback function and configure it to use with a external interrupt.
  \param ioObjectGPIO is a gpio_t "object".
  \param ui8ActivationMode is a 8-bits unsigned integer. It's the activation mode of interruption (LOW_LEVEL_MODE, LEVEL_CHANGE_MODE, FALLING_BOARD_MODE or RISING_BOARD_MODE).
  \param vInterruptFunction is a function pointer. It's the callback interruption.
  \param vpArgument is a void pointer. It's the callback argument.
*/
static void vPublicAttachInterrupt(gpio_t* ioObjectGPIO, uint8_t ui8ActivationMode, void (*vInterruptFunction)(void*), void* vpArgument){
  if (ui8PrivateVerifyEXTINTPin(ioObjectGPIO) == 1){
    vDefineEXTINTActivationMode(ui8PrivateGetEXTINTPin(ioObjectGPIO), ui8ActivationMode);
    vAttachEXTINTInterrupt(ui8PrivateGetEXTINTPin(ioObjectGPIO), vInterruptFunction, vpArgument);
  }
  else{
    if (ui8ActivationMode == LEVEL_CHANGE_MODE && ui8PrivateVerifyPCINTPin(ioObjectGPIO) == 1){
      vEnablePCINTGroup(ioObjectGPIO->ui8pGPIOGroup);
      vAttachPCINTInterrupt(ui8PrivateGetPCINTPin(ioObjectGPIO), vInterruptFunction, vpArgument);
    }
  }
}

//! Public Method: GPIO Interrupt Function Detacher
/*!
  Detach a callback interrupt function.
  \param ioObjectGPIO is a gpio_t "object".
*/
static void vPublicDetachInterrupt(gpio_t* ioObjectGPIO){
  if (ui8PrivateVerifyEXTINTPin(ioObjectGPIO) == 1){
    vDetachEXTINTInterrupt(ui8PrivateGetEXTINTPin(ioObjectGPIO));
  }
  else if (ui8PrivateVerifyPCINTPin(ioObjectGPIO) == 1){
    vDetachPCINTInterrupt(ui8PrivateGetPCINTPin(ioObjectGPIO));
  }
}

//! Public Method: GPIO Interrupt Function Enabler
/*!
  If the pin is compatible, enable external interruption.
  \param ioObjectGPIO is a gpio_t "object".
*/
static void vPublicEnableInterrupt(gpio_t* ioObjectGPIO){
  if (ui8PrivateVerifyEXTINTPin(ioObjectGPIO) == 1){
    vEnableEXTINTInterrupt(ui8PrivateGetEXTINTPin(ioObjectGPIO));
  }
  else if (ui8PrivateVerifyPCINTPin(ioObjectGPIO) == 1){
    vEnablePCINTPin(ui8PrivateGetPCINTPin(ioObjectGPIO));
  }
}

//! Public Method: GPIO Interrupt Function Disabler
/*!
  If the pin is compatible, disable external interruption.
  \param ioObjectGPIO is a gpio_t "object".
*/
static void vPublicDisableInterrupt(gpio_t* ioObjectGPIO){
  if (ui8PrivateVerifyEXTINTPin(ioObjectGPIO) == 1){
    vDisableEXTINTInterrupt(ui8PrivateGetEXTINTPin(ioObjectGPIO));
  }
  else if (ui8PrivateVerifyPCINTPin(ioObjectGPIO) == 1){
    vDisablePCINTPin(ui8PrivateGetPCINTPin(ioObjectGPIO));
  }
}