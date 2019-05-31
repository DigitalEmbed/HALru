//! Example 01 - Blink a LED
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
  vSetGPIOPinMode(IO_GROUP_B,                                                   // This parameter defines the IO group.
                  7,                                                            // This parameter defines the pin of a IO group.
                  OUTPUT_MODE);                                                 // This parameter defines the operation mode.

  /*!
    A bad implementation of a delay... Avoid using delays...
  */
  volatile uint32_t ui32BadTimer = 0;
  while(1){
    vToggleGPIOPin(IO_GROUP_B, 7);                                              // Logic level pin inverting.
    for (ui32BadTimer = 0 ; ui32BadTimer < 100000 ; ui32BadTimer++);            // "Delay"
  }
}