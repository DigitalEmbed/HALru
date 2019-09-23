#include "./System.h"

//! X-Macro: Sleep Configuration Method List
/*!
  This macros are for automatizate the confection of this library.
*/
#define SLEEP_CONFIGURATION_METHOD_LIST\
  X(SetMode1, SLEEP_IDLE_MODE)\
  X(SetMode2, SLEEP_ADC_NOISE_REDUCTION_MODE)\
  X(SetMode3, SLEEP_POWER_DOWN_MODE)\
  X(SetMode4, SLEEP_POWER_SAVE_MODE)\
  X(SetMode5, SLEEP_STANDBY_MODE)\
  X(SetMode6, SLEEP_EXTENDED_STANDBY_MODE)

//! X-Macro: Peripherals Configuration Method List
/*!
  This macros are for automatizate the confection of this library.
*/
#if defined(ARDUINO_AVR_MEGA2560) || defined(ARDUINO_AVR_MEGA)
  #define PERIPHERALS_CONFIGURATION_METHOD_LIST\
    X(ADConverter, ADC)\
    X(USART0, USART_0)\
    X(USART1, USART_1)\
    X(USART2, USART_2)\
    X(USART3, USART_3)\
    X(SPI, SPI)\
    X(TIMER0, TIMER_0)\
    X(TIMER1, TIMER_1)\
    X(TIMER2, TIMER_2)\
    X(TIMER3, TIMER_3)\
    X(TIMER4, TIMER_4)\
    X(TIMER5, TIMER_5)\
    X(I2C, I2C)
#else
  #define PERIPHERALS_CONFIGURATION_METHOD_LIST\
    X(ADConverter, ADC)\
    X(USART0, USART_0)\
    X(SPI, SPI)\
    X(TIMER0, TIMER_0)\
    X(TIMER1, TIMER_1)\
    X(TIMER2, TIMER_2)\
    X(I2C, I2C)
#endif

/*!
  Prototype public "methods".
*/
static void vPublicSleep(void);                                                   /*!< void "method". */
static void vPublicTurnOnAllPeripherals(void);                                    /*!< void "method". */
static void vPublicTurnOffAllPeripherals(void);                                   /*!< void "method". */
static void vPublicIgnoreWatchdogRequest(void);                                   /*!< void "method". */
static void vPublicForceReset(void);                                              /*!< void "method". */
static void vPublicForceFullReset(void);                                          /*!< void "method". */
static uint8_t ui8PublicCheckWatchdog(void);                                      /*!< uint8_t "method". */
static uint8_t ui8PublicCheckBrownOut(void);                                      /*!< uint8_t "method". */
static uint8_t ui8PublicCheckExternal(void);                                      /*!< uint8_t "method". */
static uint8_t ui8PublicCheckPowerOn(void);                                       /*!< uint8_t "method". */

//! X-Macro: Sleep Mode Function Setter Prototype Creator
/*!
  This macros are for automatizate the confection of this library.
*/
#define X(MethodName, ui8MacroArgument)\
  static void vPublic##MethodName(void);

  SLEEP_CONFIGURATION_METHOD_LIST
#undef X

//! X-Macro: Peripherals Mode Function Setter Prototype Creator
/*!
  This macros are for automatizate the confection of this library.
*/
#define X(MethodName, ui8MacroArgument)\
  static void vPublicTurnOn##MethodName(void);\
  static void vPublicTurnOff##MethodName(void);

  PERIPHERALS_CONFIGURATION_METHOD_LIST
#undef X

/*!
  System object "constructor".
*/
const system_manager_t System PROGMEM = {
  .Power = {
    .Sleep = {
      #define X(MethodName, ui8MacroArgument)\
        .v##MethodName = &vPublic##MethodName,
      
        SLEEP_CONFIGURATION_METHOD_LIST
      #undef X
      .vSleep = &vPublicSleep,
    },
    #define X(MethodName, ui8MacroArgument)\
      .MethodName = {\
        .vTurnOn = &vPublicTurnOn##MethodName,\
        .vTurnOff = &vPublicTurnOff##MethodName\
      },

      PERIPHERALS_CONFIGURATION_METHOD_LIST
    #undef X
    .AllPeripherals = {
      .vTurnOn = &vPublicTurnOnAllPeripherals,
      .vTurnOff = &vPublicTurnOffAllPeripherals
    }
  },
  .Watchdog = {
    .Interrupt = {
      .vAttach = &vAttachWatchdogInterrupt,
      .vDetach = &vDetachWatchdogInterrupt
    },
    .vIgnoreRequest = &vPublicIgnoreWatchdogRequest,
    .vStart = &vStartWatchdog,
    .vStop = &vStopWatchdog
  },
  .Reset = {
    .vForceReset = &vPublicForceReset,
    .vForceFullReset = &vPublicForceFullReset,
    .Blame = {
      .ui8CheckWatchdog = &ui8PublicCheckWatchdog,
      .ui8CheckBrownOut = &ui8PublicCheckBrownOut,
      .ui8CheckExternal = &ui8PublicCheckExternal,
      .ui8CheckPowerOn = &ui8PublicCheckPowerOn
    }
  }
};

/*!
  Definition public "methods".
*/

//! X-Macro: Sleep Mode Function Setter Creator
/*!
  This macros are for automatizate the confection of this library.
*/
#define X(MethodName, ui8MacroArgument)\
  static void vPublic##MethodName(void){\
    vSetSleepMode(ui8MacroArgument);\
  }

  SLEEP_CONFIGURATION_METHOD_LIST
#undef X

//! X-Macro: Peripherals Mode Function Setter Creator
/*!
  This macros are for automatizate the confection of this library.
*/
#define X(MethodName, ui8MacroArgument)\
  static void vPublicTurnOn##MethodName(void){\
    vTurnOnPeripheral(PERIPHERAL_##ui8MacroArgument);\
  }\
  static void vPublicTurnOff##MethodName(void){\
    vTurnOffPeripheral(PERIPHERAL_##ui8MacroArgument);\
  }

  PERIPHERALS_CONFIGURATION_METHOD_LIST
#undef X

//! Public Method: Sleep Enabler
/*!
  Enables sleep mode.
*/
static void vPublicSleep(void){
  vSleepEnable();
}

//! Public Method: All Peripherals Enabler
/*!
  Turn on all peripherals.
*/
static void vPublicTurnOnAllPeripherals(void){
  vTurnOnAllPeripherals();
}

//! Public Method: All Peripherals Disabler
/*!
  Turn off all peripherals.
*/
static void vPublicTurnOffAllPeripherals(void){
  vTurnOffAllPeripherals();
}

//! Public Method: Watchdog Request Ignorer
/*!
  Ignore watchdog request.
*/
static void vPublicIgnoreWatchdogRequest(void){
  vIgnoreWatchdogRequest();
}

//! Public Method: System Restarter
/*!
  Restart the microcontroler.
*/
static void vPublicForceReset(void){
  vForceSystemReset(RESET_MODE);
}

//! Public Method: System Full Restarter
/*!
  Calls watchdog interruption and, in sequence, restart the microcontroler.
*/
static void vPublicForceFullReset(void){
  vForceSystemReset(INTERRUPT_RESET_MODE);
}

//! Public Method: Watchdog Restart Checker
/*!
  Checks if occurred system restart by watchdog.
  \return Returns 1 in positive case, or 0 in contrary case.
*/
static uint8_t ui8PublicCheckWatchdog(void){
  return ui8WatchdogResetOccurred();
}

//! Public Method: Brown Out Restart Checker
/*!
  Checks if occurred system restart by brown out.
  \return Returns 1 in positive case, or 0 in contrary case.
*/
static uint8_t ui8PublicCheckBrownOut(void){
  return ui8BrownOutResetOccurred();
}

//! Public Method: External Restart Checker
/*!
  Checks if occurred system restart by external pin.
  \return Returns 1 in positive case, or 0 in contrary case.
*/
static uint8_t ui8PublicCheckExternal(void){
  return ui8ExternalResetOccurred();
}

//! Public Method: Power On Restart Checker
/*!
  Checks if occurred system restart by power suply on VCC pin.
  \return Returns 1 in positive case, or 0 in contrary case.
*/
static uint8_t ui8PublicCheckPowerOn(void){
  return ui8PowerOnResetOccurred();
}