//! Example 01 - Echo
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
  Toggle LED interruption. The mode indicated (FrontBurnerMode) in the function is to allow an interrupt interrupt.
*/
void vToggleLEDInterruption(void* vpArgs) FrontBurnerMode{
  vToggleGPIOPin(IO_GROUP_B, 7);
} EndFrontBurner;

/*!
  Echo interruption.
*/
void vEcho(void* vpArgs) FrontBurnerMode{
  static volatile char cReceivedChar = 0;

  /*!
    Atomic is used to indicate critial section.
  */
  Atomic{
    cReceivedChar = ui8USARTReceiveByte(USART_0);
  } EndAtomic;

  vUSARTSendByte(USART_0, cReceivedChar);
} EndFrontBurner;

/*!
  Main function.
*/
int main(void){
  /*!
    Configuring GPIO B7.
  */
  vSetGPIOPinMode(IO_GROUP_B, 7, OUTPUT_MODE);

  /*!
    Configuring USART0.
  */
  vSetUSARTSync(USART_0, ASYNCHRONOUS);
  vSetUSARTActivationEdge(USART_0, RISING);
  vSetUSARTParity(USART_0, NO_PARITY);
  vSetUSARTStopBit(USART_0, 1);
  vSetUSARTDataSize(USART_0, 8);
  vSetUSARTBaudRate(USART_0, 115200);
  vAttachUSARTInterrupt(USART_0, RX_COMPLETE, &vEcho, NULL);
  vAttachUSARTInterrupt(USART_0, TX_COMPLETE, &vToggleLEDInterruption, NULL);
  vEnableTX(USART_0);
  vEnableRX(USART_0);
  vEnableUSARTInterrupt(USART_0, RX_COMPLETE);
  vEnableUSARTInterrupt(USART_0, TX_COMPLETE);

  /*!
    Enabling all interruptions.
  */
  vEnableAllInterrupts();

  /*!
    Waiting for interruptions.
  */
  while(1);
}
