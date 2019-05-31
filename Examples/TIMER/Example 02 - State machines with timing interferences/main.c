//! Example 02 - State machines with timing interferences
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

//! Loop: State Machine
/*!
              main 
                |
                v
  #---->  vMasterFunction 
  |             |
  |             v
  |     vSubTimerAFunction
  |             |
  |             v 
  #-----vSubTimerBFunction
*/

#include <HALru.h>

/*!
  Master callback.
*/
void vMasterFunction(void* vpArgs){
  static uint8_t ui8BlinkCounter = 0;
  static uint16_t ui16Timer = 0;
  if (ui16Timer == 50){                                                         // After 500 ms (50 * ui8TimeBase)...
    ui16Timer = 0;
    if (ui8BlinkCounter == 10){                                                 // After LED blinks 10 times...
      vDisableTIMER(TIMER_1, MASTER_TIMER);                                     // Go to next state machine...
      vEnableTIMER(TIMER_1, SUBTIMER_A);
      ui8BlinkCounter = 0;
    }
    else{
      ui8BlinkCounter++;
    }
    vToggleGPIOPin(IO_GROUP_F, 0);                                              // Toggle LED pin...
  }
  else{
    ui16Timer++;
  }
}

/*!
  Sub-timer A callback.
*/
void vSubTimerAFunction(void* vpArgs){
  static uint8_t ui8BlinkCounter = 0;
  static uint16_t ui16Timer = 0;
  if (ui16Timer == 10){                                                         // After 100 ms (10 * ui8TimeBase)...
    ui16Timer = 0;
    if (ui8BlinkCounter == 50){                                                 // After LED blinks 50 times...
      vDisableTIMER(TIMER_1, SUBTIMER_A);                                       // Go to next state machine...
      vEnableTIMER(TIMER_1, SUBTIMER_B);
      ui8BlinkCounter = 0;
    }
    else{
      ui8BlinkCounter++;
    }
    vToggleGPIOPin(IO_GROUP_F, 1);                                              // Toggle LED pin...
  }
  else{
    ui16Timer++;
  }
}

/*!
  Sub-timer B callback.
*/
void vSubTimerBFunction(void* vpArgs){
  static uint8_t ui8BlinkCounter = 0;
  static uint16_t ui16Timer = 0;
  if (ui16Timer == 100){                                                        // After 1 s (100 * ui8TimeBase)...
    ui16Timer = 0;
    if (ui8BlinkCounter == 4){                                                  // After LED blinks 4 times...
      vDisableTIMER(TIMER_1, SUBTIMER_B);                                       // Go to next state machine...
      vEnableTIMER(TIMER_1, MASTER_TIMER);
      ui8BlinkCounter = 0;
    }
    else{
      ui8BlinkCounter++;
    }
    vToggleGPIOPin(IO_GROUP_F, 3);                                              // Toggle LED pin...
  }
  else{
    ui16Timer++;
  }
}

/*!
  Main function.
*/
int main(void){
  uint8_t ui8TimeBase = 10;
  vSetGPIOPinMode(IO_GROUP_F, 0, OUTPUT_MODE);
  vSetGPIOPinMode(IO_GROUP_F, 1, OUTPUT_MODE);
  vSetGPIOPinMode(IO_GROUP_F, 3, OUTPUT_MODE);
  vTIMERInit(TIMER_1);
  vSetTIMERPeriodMS(TIMER_1, ui8TimeBase);                                      // Setting time-base (10 ms)
  vAttachTIMERInterrupt(TIMER_1, MASTER_TIMER, &vMasterFunction, NULL);
  vAttachTIMERInterrupt(TIMER_1, SUBTIMER_A, &vSubTimerAFunction, NULL);
  vAttachTIMERInterrupt(TIMER_1, SUBTIMER_B, &vSubTimerBFunction, NULL);
  vEnableTIMER(TIMER_1, MASTER_TIMER);                                          // Enabling initial state
  vEnableAllInterrupts();
  while(1){
    //Waiting for interruptions...
  }
}
