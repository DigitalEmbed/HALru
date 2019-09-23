#include "./Watchdog.h"
#include <stdio.h>
#include <avr/interrupt.h>
#include <EmbeddedTools.h>
#include "./Interrupts.h"

hal_isr_t isrWatchdogInterrupt = {NULL, NULL};                                  /*!< hal_isr_t type. */

//! Function: Watchdog Starter
/*!
  Starts watch dog clock.
  \param ui8Mode is an unsigned 8-bit integer. The avaliable modes are: DISABLED_MODE, INTERRUPT_MODE, RESET_MODE and INTERRUPT_RESET_MODE.
  \param ui8Time is an unsigned 8-bit integer. The avaliable times are WHATCHDOG_TIMER_X, 64MS, 125MS, 250MS, 500MS, 1S, 2S, 4S and 8S.
*/
void vStartWatchdog(uint8_t ui8Mode, uint8_t ui8Time){
    uint8_t ui8BackupWDTCSR = 0;
    ui8BackupWDTCSR |= ((ui8Mode & 1) << WDIE) | ((ui8Mode & 2) << WDE) | ((ui8Time & 7) << WDP0) | ((ui8Time & 8) << 1);
    vEraseBit(SREG, 7);
    WDTCSR |= (1 << WDCE) | (1 << WDE);
    WDTCSR |= ui8BackupWDTCSR;
    vSetBit(SREG, 7);
}

//! Function: Watchdog Stopper
/*!
  Stops watch dog clock.
*/
void vStopWatchdog(void){
    vEraseBit(SREG, 7);
    MCUSR &= ~(1<<WDRF);
    WDTCSR |= (1 << WDCE) | (1 << WDE);
    WDTCSR = 0;
    vSetBit(SREG, 7);
}

//! Function: Watchdog Interrupt Attacher
/*!
  Attaches a watchdog interruption function.
  \param vInterruptFunction is a function pointer. It's the callback interruption.
  \param vpArgument is a void pointer. It's the callback argument.
*/
void vAttachWatchdogInterrupt(void (*vInterruptFunction)(void*), void* vpArgument){
    isrWatchdogInterrupt.vInterruptFunction = vInterruptFunction;
    isrWatchdogInterrupt.vpArgument = vpArgument;
}

//! Function: Watchdog Interrupt Detacher
/*!
  Detaches a watchdog interruption function.
*/
void vDetachWatchdogInterrupt(void){
    vAttachWatchdogInterrupt(NULL, NULL);
}

//! Callbacks: Watchdog Interruptions
/*!
  Callbacks of Watchdog hardware interruptions.
*/
ISR(WDT_vect){
    if (isrWatchdogInterrupt.vInterruptFunction != NULL){
        isrWatchdogInterrupt.vInterruptFunction(isrWatchdogInterrupt.vpArgument);
    }
}