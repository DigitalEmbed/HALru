//! Example 02 - Toggling LEDs on low logic level with debounced PCINT
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
void vToggleLEDInterruption(void* vpArgs){
  static volatile uint8_t ui8Counter = 0;
  ui8Counter = ui8Counter == 0 ? 1 : 0;
  if (ui8Counter == 1){                                                         //Toggling LED with high logic level.
    vToggleGPIOPin(IO_GROUP_B, 7);
  }
  vDisablePCINTPin(PCINT_5);
  vResetTIMERCounter(TIMER_1);
  vIgnoreTIMERRequest(TIMER_1, MASTER_TIMER);
  vEnableTIMER(TIMER_1, MASTER_TIMER);
}

/*!
  Debouncing interruption.
*/
void vDebouncingButton(void* vpArgs){
  vEnablePCINTPin(PCINT_5);
  vDisableTIMER(TIMER_1, MASTER_TIMER);
}

int main(void){
  /*!
    Configuring output GPIO B7.
  */
  static uint8_t ui8PinLED1 = 7;
  vSetGPIOPinMode(IO_GROUP_B, ui8PinLED1, OUTPUT_MODE);

  /*!
    Configuring input GPIO B5.
  */
  vSetGPIOPinMode(IO_GROUP_B, 5, INPUT_MODE);
  vEnablePullUp(IO_GROUP_B, 5);
  vAttachPCINTInterrupt(PCINT_5, &vToggleLEDInterruption, &ui8PinLED1);
  vEnablePCINTPin(PCINT_5);

  /*!
    Setting TIMER_1.
  */
  vTIMERInit(TIMER_1);
  vSetTIMERPeriodMS(TIMER_1, 500);
  vAttachTIMERInterrupt(TIMER_1, MASTER_TIMER, &vDebouncingButton, NULL);
  vDisableTIMER(TIMER_1, MASTER_TIMER);

  /*!
    Enabling goup B GPIOs PCINT interruptions.
  */
  vEnablePCINTGroup(IO_GROUP_B);

  /*!
    Enabling all interruptions.
  */
  vEnableAllInterrupts();

  /*!
    Waiting for interruptions.
  */
  while(1);
}
