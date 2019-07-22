//! Example 01 - Sleeping and Blinking a LED
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
  Toggle pin interruption function.
*/
void vBlink(void* vpArgs){
  static uint8_t ui8TimeCounter = 0;
  ui8TimeCounter++;
  if (ui8TimeCounter == 100){
    ui8TimeCounter = 0;
    vToggleGPIOPin(IO_GROUP_B, 7);
  }
}

/*!
  Main function.
*/
int main(void){
  vTurnOffAllPeripherals();                                             //Turning off all peripherals
  vTurnOnPeripheral(PERIPHERAL_TIMER_2);                                //Timer 2 turning on
  vSetGPIOPinMode(IO_GROUP_B, 7, OUTPUT_MODE);
  vSetSleepMode(SLEEP_POWER_SAVE_MODE);                                 //Seting sleep mode
  vTIMERInit(TIMER_2);
  vSetTIMERPeriodMS(TIMER_2, 10);
  vAttachTIMERInterrupt(TIMER_2, MASTER_TIMER, &vBlink, NULL);
  vEnableTIMER(TIMER_2, MASTER_TIMER);
  vEnableAllInterrupts();
  while(1){
    vSleepEnable();                                                     //Sleeping and waiting for interruptions...
  }
}
