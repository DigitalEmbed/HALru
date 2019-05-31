//! Example 02 - Reading a Pin
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
  Main function.
*/
int main(void){
  vSetGPIOPinMode(IO_GROUP_B, 7, OUTPUT_MODE);                                  // Configuring PB7.
  vSetGPIOPinMode(IO_GROUP_B, 6, INPUT_MODE);                                   // Configuring PB6.
  uint8_t ui8PresentGPIOValue = 0;
  uint8_t ui8PreviousGPIOValue = 0;
  while(1){
    ui8PreviousGPIOValue = ui8PresentGPIOValue;
    ui8PresentGPIOValue = ui8GPIODigitalRead(IO_GROUP_B, 6);
    if (ui8PreviousGPIOValue == 0 && ui8PresentGPIOValue == 1){                 // On a rising edge on PB6...
      vSetGPIOPin(IO_GROUP_B, 7);                                               // Write high logic level on PB7.
    }
    else{
      vUnsetGPIOPin(IO_GROUP_B, 7);                                             // Write low logic level on PB7.
    }
  }
}