//! Example 02 - Sleeping and Transceiving into USB-Bluetooth With Indicator LEDs
/*!
  This simply example demonstrates the use of HALru.

  Notes:
    -> USB is connected in USART_0 port;
    -> Bluetooth is connected in USART_1 port;
    -> USB indicator LED is connected in PB7;
    -> Bluetooth indicator LED is connected in PB6;

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
  Blink USB interruption function.
*/
void vBlinkUSB(void* vpArgs){
  static volatile uint8_t ui8TimeCounter = 0;
  ui8TimeCounter++;
  if (ui8TimeCounter == 10){
    ui8TimeCounter = 0;
    vUnsetGPIOPin(IO_GROUP_B, 7);
    vDisableTIMER(TIMER_0, MASTER_TIMER);
  }
}

/*!
  Blink Bluetooth interruption function.
*/
void vBlinkBluetooth(void* vpArgs){
  static volatile uint8_t ui8TimeCounter = 0;
  ui8TimeCounter++;
  if (ui8TimeCounter == 10){
    ui8TimeCounter = 0;
    vUnsetGPIOPin(IO_GROUP_B, 7);
    vDisableTIMER(TIMER_2, MASTER_TIMER);
  }
}

/*!
  USB RX interruption function.
*/
void vUSB(void* vpArgs){
  static volatile char cReceivedChar;
  cReceivedChar = (char) ui8USARTReceiveByte(USART_0);
  vAttachPrintOutput(USART_0, &vUSARTSendByte);
  printf("Sending via Bluetooth: %c\n", cReceivedChar);
  vIgnoreTIMERRequest(TIMER_0, MASTER_TIMER);
  vResetTIMERCounter(TIMER_0);
  vEnableTIMER(TIMER_0, MASTER_TIMER);
  vSetGPIOPin(IO_GROUP_B, 7);
}

/*!
  Bluetooth RX interruption function.
*/
void vBluetooth(void* vpArgs){
  static volatile char cReceivedChar;
  cReceivedChar = (char) ui8USARTReceiveByte(USART_1);
  vAttachPrintOutput(USART_1, &vUSARTSendByte);
  printf("Received via Bluetooth: %c\n", cReceivedChar);
  vIgnoreTIMERRequest(TIMER_2, MASTER_TIMER);
  vResetTIMERCounter(TIMER_2);
  vEnableTIMER(TIMER_2, MASTER_TIMER);
  vSetGPIOPin(IO_GROUP_B, 6);
}

/*!
  Main function.
*/
int main(void){
  /*!
    Setting sleeping mode.
  */
  vTurnOffAllPeripherals();                                                     //Turning off all peripherals
  vTurnOnPeripheral(PERIPHERAL_TIMER_0);                                        //TIMER 0 turning on
  vTurnOnPeripheral(PERIPHERAL_TIMER_2);                                        //TIMER 2 turning on
  vTurnOnPeripheral(PERIPHERAL_USART_0);                                        //USART 0 turning on
  vTurnOnPeripheral(PERIPHERAL_USART_1);                                        //USART 1 turning on
  vSetSleepMode(SLEEP_IDLE_MODE);                                               //Setting sleep mode

  /*!
    Setting GPIOs.
  */
  vSetGPIOPinMode(IO_GROUP_B, 7, OUTPUT_MODE);
  vSetGPIOPinMode(IO_GROUP_B, 6, OUTPUT_MODE);

  /*!
    Setting TIMERs.
  */
  vTIMERInit(TIMER_0);
  vTIMERInit(TIMER_2);
  vSetTIMERPeriodMS(TIMER_0, 10);
  vSetTIMERPeriodMS(TIMER_2, 10);
  vAttachTIMERInterrupt(TIMER_0, MASTER_TIMER, &vBlinkUSB, NULL);
  vAttachTIMERInterrupt(TIMER_2, MASTER_TIMER, &vBlinkBluetooth, NULL);
  vEnableTIMER(TIMER_0, MASTER_TIMER);
  vEnableTIMER(TIMER_2, MASTER_TIMER);

  /*!
    Setting USARTs.
  */
  vUSARTInit(USART_0);
  vUSARTInit(USART_1);
  vAttachUSARTInterrupt(USART_0, RX_COMPLETE, vUSB, NULL);
  vAttachUSARTInterrupt(USART_1, RX_COMPLETE, vBluetooth, NULL);

  /*!
    Enabling all interruptions.
  */
  vEnableAllInterrupts();
  while(1){
    vSleepEnable();                                                             //Sleeping and waiting for interruptions...
  }
}
