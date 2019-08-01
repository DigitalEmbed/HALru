//! Example 02 - Trating received USART messages
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
  Size of cpMessageBuffer macro.
*/
#define SIZE_OF_BUFFER 10

/*!
  RX interrupt function.
*/
void vCheckMessage(void* vpArgs){
  static char cpMessageBuffer[SIZE_OF_BUFFER + 1] = {0};
  static uint8_t ui8Counter = 0;
  if (ui8Counter < SIZE_OF_BUFFER){
    char cReceivedChar = ui8USARTReceiveByte(USART_0);
    printf("%c", cReceivedChar);
    if (cReceivedChar != '\n' && cReceivedChar != '\r' && cReceivedChar != '\b'){
      cpMessageBuffer[ui8Counter] = cReceivedChar;
      ui8Counter++;
    }
    else if (cReceivedChar == '\b' && ui8Counter > 0){
      ui8Counter--;
    }
    else if (cReceivedChar == '\n'){
      printf("Received message! Processing...\n");
      if (memcmp(cpMessageBuffer, "TurnOn", 6) == 0){
        vSetGPIOPin(IO_GROUP_B, 7);
        printf("LED turned on!\n");
      }
      else if (memcmp(cpMessageBuffer, "TurnOff", 7) == 0){
        vUnsetGPIOPin(IO_GROUP_B, 7);
        printf("LED turned off!\n");
      }
      else if (memcmp(cpMessageBuffer, "Toggle", 6) == 0){
        vToggleGPIOPin(IO_GROUP_B, 7);
        printf("LED toggled!\n");
      }
      else{
        printf("Invalid message!\n");
      }
      memset(cpMessageBuffer, 0, SIZE_OF_BUFFER);
      printf("Digited: ");
      ui8Counter = 0;
    }
  }
  else{
    printf("Buffer overloaded! Cleaning buffers...\n");
    printf("Digited: ");
    memset(cpMessageBuffer, 0, SIZE_OF_BUFFER);
    ui8Counter = 0;
  }
}

int main(void){
  /*!
    Configuring GPIO B7.
  */
  vSetGPIOPinMode(IO_GROUP_B, 7, OUTPUT_MODE);

  /*!
    Initializing USART on default mode:
    - ASYNCHRONOUS
    - RISING
    - NO_PARITY
    - Stop Bit 1
    - Word Size 8
    - TX and RX enabled
    - Baud Rate: 9600
  */
  vUSARTInit(USART_0);
  vSetUSARTBaudRate(USART_0, 115200);                                           //Setting baud rate to 115200  
  vAttachUSARTInterrupt(USART_0, RX_COMPLETE, &vCheckMessage, NULL);

  /*!
    Setting printf output.
  */
  vAttachPrintOutput(USART_0, &vUSARTSendByte);
  printf("Digited: ");

  /*!
    Waiting for interrupts...
  */
  vEnableAllInterrupts();
  while(1);
}
