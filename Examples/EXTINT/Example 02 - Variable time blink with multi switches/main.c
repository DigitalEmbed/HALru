//! Example 02 - Variable time blink with multi switches
/*!
  This simply example demonstrates the use of HALru.

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

#include <HALru.h>

#define   DEBOUNCING_TIME_MS          200                                                                     //Debouncing time of buttons, in miliseconds.
#define   DEBOUNCING_BASE_TIME_MS     10                                                                      //On 8-bit TIMERs, the max value is 16.
#define   BLINK_LED_BASE_TIME_MS      100                                                                     //Blink LED base time.
#define   BLINK_LED_MAX_TIME_MS       1000                                                                    //Max blink LED time.
#define   BLINK_LED_MIN_TIME_MS       100                                                                     //Min blink LED time.

/*!
  It's used for save switch configurations.
*/
typedef struct{
  uint8_t ui8EXTINT;
  volatile uint8_t* ui8pTIMER;
  uint8_t ui8Counter;
}configs_t;

uint16_t ui16BlinkTime = (BLINK_LED_MAX_TIME_MS - BLINK_LED_MIN_TIME_MS)/(2*BLINK_LED_BASE_TIME_MS);          //Initial blink time is the median of BLINK_LED_MAX_TIME_MS and BLINK_LED_MIN_TIME_MS.
uint16_t ui16Counter = 0;                                                                                     //Blink time counter.

/*!
  Toggle LED interruption.
*/
void vSwitchInterrupt(void* vpArgs){
  configs_t* cPIN = (configs_t*) vpArgs;                                                                      //Getting pin configurations.
  vDisableEXTINT(cPIN->ui8EXTINT);                                                                            //Disabling external interruption on PINX.
  if (cPIN->ui8EXTINT == EXTINT_4 && ui16BlinkTime < (BLINK_LED_MAX_TIME_MS/BLINK_LED_BASE_TIME_MS)){         //If the ui16BlinkTime is between BLINK_LED_MAX_TIME_MS and BLINK_LED_MIN_TIME_MS
    ui16BlinkTime++;
  }
  else if (cPIN->ui8EXTINT == EXTINT_5 && ui16BlinkTime > (BLINK_LED_MIN_TIME_MS/BLINK_LED_BASE_TIME_MS)){    //If the ui16BlinkTime is between BLINK_LED_MAX_TIME_MS and BLINK_LED_MIN_TIME_MS
    ui16BlinkTime--;
  }
  ui16Counter = 0;                                                                                            //Reseting Blink time counter.
  vResetTIMERCounter(cPIN->ui8pTIMER);                                                                        //Reseting TIMER_X counter.
  vIgnoreTIMERRequest(cPIN->ui8pTIMER, MASTER_TIMER);                                                         //Ignoring all TIMER_X interruptions.
  vEnableTIMER(cPIN->ui8pTIMER, MASTER_TIMER);                                                                //Enabling TIMER_X interrupt.
}

/*!
  Debouncing interruption.
*/
void vDebouncing(void* vpArgs){
  configs_t* cPIN = (configs_t*) vpArgs;                                                                      //Getting pin configurations.
  cPIN->ui8Counter++;
  if (cPIN->ui8Counter >= DEBOUNCING_TIME_MS/DEBOUNCING_BASE_TIME_MS){                                        //After DEBOUNCING_TIME_MS ms...
    cPIN->ui8Counter = 0;
    vIgnoreEXTINTRequest(cPIN->ui8EXTINT);                                                                    //Ignoring all external interrutions on PIN while EXTINT_X is disabled.
    vEnableEXTINT(cPIN->ui8EXTINT);                                                                           //Enabling external interruption on PIN.
    vDisableTIMER(cPIN->ui8pTIMER, MASTER_TIMER);                                                             //Disabling TIMER_X interrupt.
  }
}

/*!
  Blink function.
*/
void vBlinkLED(void* vpArgs){
  ui16Counter++;                                                                                              //Incrementing time counter.
  if (ui16Counter >= ui16BlinkTime){                                                                          //If time counter is overflowed...
    ui16Counter = 0;                                                                                          //Reset time counter.
    vToggleGPIOPin(IO_GROUP_B, 7);                                                                            //Toggling LED.
  }
}

/*!
  Main function.
*/
int main(void){
  /*!
    Storing configurations of buttons.
  */
  configs_t cPINE4 = {EXTINT_4, TIMER_0, 0};
  configs_t cPINE5 = {EXTINT_5, TIMER_2, 0};

  /*!
    Setting TIMER_1 for PINB7 blinking.
  */
  vTIMERInit(TIMER_1);                                                                                        //Initializing TIMER_1.
  vSetTIMERPeriodMS(TIMER_1, BLINK_LED_BASE_TIME_MS);                                                         //Setting TIMER_1 period.
  vAttachTIMERInterrupt(TIMER_1, MASTER_TIMER, &vBlinkLED, NULL);                                             //Attaching TIMER_1 interruption function.
  vEnableTIMER(TIMER_1, MASTER_TIMER);                                                                        //Enabling TIMER_1.

  /*!
    Setting PINB7 to output mode.
  */
  vSetGPIOPinMode(IO_GROUP_B, 7, OUTPUT_MODE);                                                                //Setting PINE4 to output mode (LED).

  /*!
    Setting external interrupt mode on PINE4.
  */
  vSetGPIOPinMode(IO_GROUP_E, 4, INPUT_MODE);                                                                 //Setting PINE4 to input mode (Button).
  vEnablePullUp(IO_GROUP_E, 4);                                                                               //Enabling pullup resistor on PINE4.
  vDefineEXTINTActivationMode(EXTINT_4, RISING_BOARD_INTERRUPT);                                              //Setting interrupt activation mode on EXTINT_4 (PINE4... View on Pinout map of your ATMega).
  vAttachEXTINTInterrupt(EXTINT_4, &vSwitchInterrupt, &cPINE4);                                               //Attaching external interruption function.
  vEnableEXTINT(EXTINT_4);                                                                                    //Enabling external interruption.

  /*!
    Setting TIMER_0 for PINE4 debouncing.
  */
  vTIMERInit(TIMER_0);                                                                                        //Initializing TIMER_0.
  vSetTIMERPeriodMS(TIMER_0, DEBOUNCING_BASE_TIME_MS);                                                        //Setting TIMER_0 period.
  vAttachTIMERInterrupt(TIMER_0, MASTER_TIMER, &vDebouncing, &cPINE4);                                        //Attaching TIMER_0 interruption function.
  vDisableTIMER(TIMER_0, MASTER_TIMER);                                                                       //Disabling TIMER_0.

  /*!
    Setting external interrupt mode on PINE5.
  */
  vSetGPIOPinMode(IO_GROUP_E, 5, INPUT_MODE);                                                                 //Setting PINE5 to input mode (Button).
  vEnablePullUp(IO_GROUP_E, 5);                                                                               //Enabling pullup resistor on PINE5.
  vDefineEXTINTActivationMode(EXTINT_5, RISING_BOARD_INTERRUPT);                                              //Setting interrupt activation mode on EXTINT_5 (PINE5... View on Pinout map of your ATMega).
  vAttachEXTINTInterrupt(EXTINT_5, &vSwitchInterrupt, &cPINE5);                                               //Attaching external interruption function.
  vEnableEXTINT(EXTINT_5);                                                                                    //Enabling external interruption.

  /*!
    Setting TIMER_2 for PINE5 debouncing.
  */
  vTIMERInit(TIMER_2);                                                                                        //Initializing TIMER_2.
  vSetTIMERPeriodMS(TIMER_2, DEBOUNCING_BASE_TIME_MS);                                                        //Setting TIMER_2 period (on 8-bit TIMERs, the max value is 16).
  vAttachTIMERInterrupt(TIMER_2, MASTER_TIMER, &vDebouncing, &cPINE5);                                        //Attaching TIMER_2 interruption function.
  vDisableTIMER(TIMER_2, MASTER_TIMER);                                                                       //Disabling TIMER_2.

  /*!
    Enabling all interruptions.
  */
  vEnableAllInterrupts();

  /*!
    Waiting for interruptions.
  */
  while(1);
}
