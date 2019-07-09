//! Example 01 - Debouncing with TIMER and EXTINT
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

/*!
  Toggle LED interruption.
*/
void vToggleLEDInterrupt(void* vpArgs){
  vDisableEXTINT(EXTINT_4);                                                     //Disabling external interruption on PINE4.
  vToggleGPIOPin(IO_GROUP_B, 7);                                                //Toggling LED.
  vResetTIMERCounter(TIMER_1);                                                  //Reseting TIMER_1 counter.
  vIgnoreTIMERRequest(TIMER_1, MASTER_TIMER);                                   //Ignoring all TIMER_1 interruptions.
  vEnableTIMER(TIMER_1, MASTER_TIMER);                                          //Enabling TIMER_1 interrupt.
}

/*!
  Debouncing interruption.
*/
void vDebouncingButton(void* vpArgs){
  vIgnoreEXTINTRequest(EXTINT_4);                                               //Ignoring all external interrutions on PINE4 while EXTINT_4 is disabled.
  vEnableEXTINT(EXTINT_4);                                                      //Enabling external interruption on PINE4.
  vDisableTIMER(TIMER_1, MASTER_TIMER);                                         //Disabling TIMER_1 interrupt.
}

/*!
  Main function.
*/
int main(void){
  /*!
    Setting PINB7 to output mode.
  */
  vSetGPIOPinMode(IO_GROUP_B, 7, OUTPUT_MODE);                                  //Setting PINE4 to output mode (LED).

  /*!
    Setting external interrupt mode.
  */
  vSetGPIOPinMode(IO_GROUP_E, 4, INPUT_MODE);                                   //Setting PINE4 to input mode (Button).
  vEnablePullUp(IO_GROUP_E, 4);                                                 //Enabling pullup resistor on PINE4.
  vDefineEXTINTActivationMode(EXTINT_4, FALLING_BOARD_INTERRUPT);               //Setting interrupt activation mode on EXTINT_4 (PINE4... View on Pinout map of your ATMega).
  vAttachEXTINTInterrupt(EXTINT_4, &vToggleLEDInterrupt, NULL);                 //Attaching external interruption function.
  vEnableEXTINT(EXTINT_4);                                                      //Enabling external interruption.

  /*!
    Setting TIMER_1.
  */
  vTIMERInit(TIMER_1);                                                          //Initializing TIMER_1.
  vSetTIMERPeriodMS(TIMER_1, 500);                                              //Setting TIMER_1 period.
  vAttachTIMERInterrupt(TIMER_1, MASTER_TIMER, &vDebouncingButton, NULL);       //Attaching TIMER_1 interruption function.
  vDisableTIMER(TIMER_1, MASTER_TIMER);                                         //Disabling TIMER_1.

  /*!
    Enabling all interruptions.
  */
  vEnableAllInterrupts();

  /*!
    Waiting for interruptions.
  */
  while(1);
}
