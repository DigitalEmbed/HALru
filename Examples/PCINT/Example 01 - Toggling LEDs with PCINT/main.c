//! Example 01 - Toggling LEDs with PCINT
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
  uint8_t* ui8pPin = (uint8_t*) vpArgs;
  vToggleGPIOPin(IO_GROUP_B, *ui8pPin);
}

int main(void){
  /*!
    Configuring output GPIOs.
  */
  static uint8_t ui8PinLED1 = 7;
  static uint8_t ui8PinLED2 = 6;
  vSetGPIOPinMode(IO_GROUP_B, ui8PinLED1, OUTPUT_MODE);
  vSetGPIOPinMode(IO_GROUP_B, ui8PinLED2, OUTPUT_MODE);

  /*!
    Configuring input GPIO B5.
  */
  vSetGPIOPinMode(IO_GROUP_B, 5, INPUT_MODE);
  vEnablePullUp(IO_GROUP_B, 5);
  vAttachPCINTInterrupt(PCINT_5, &vToggleLEDInterruption, &ui8PinLED1);
  vEnablePCINTPin(PCINT_5);

  /*!
    Configuring input GPIO B4.
  */
  vSetGPIOPinMode(IO_GROUP_B, 4, INPUT_MODE);
  vEnablePullUp(IO_GROUP_B, 4);
  vAttachPCINTInterrupt(PCINT_4, &vToggleLEDInterruption, &ui8PinLED2);
  vEnablePCINTPin(PCINT_4);

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
