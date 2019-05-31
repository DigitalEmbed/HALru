//! Example 01 - Best Blink a LED
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
void vInterruptFunction(void* vpArgs){
  vToggleGPIOPin(IO_GROUP_B, 7);                                                // Toggle logic level on PB7.
}

/*!
  Main function.
*/
int main(void){
  vSetGPIOPinMode(IO_GROUP_B, 7, OUTPUT_MODE);                                  // Configuring PB7.
  vTIMERInit(TIMER_1);                                                          // Initializing TIMER 1.
  vSetTIMERPeriodMS(TIMER_1, 1000);                                             // Setting TIMER_1 to 1 second.
  vAttachTIMERInterrupt(TIMER_1,
                        MASTER_TIMER,                                           // Type of timer interruption.
                        &vInterruptFunction,                                    // Callback function.
                        NULL);                                                  // Argument of callback function.
  vEnableTIMER(TIMER_1, MASTER_TIMER);                                          // Enabling MASTER_TIMER of TIMER_1.
  vEnableAllInterrupts();                                                       // Enabling all interrutpions.
  while(1){
    //Waiting for interruptions...
  }
}
