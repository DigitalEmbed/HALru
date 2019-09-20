#include "./Watchdog.h"
#include <stdio.h>
#include <avr/interrupt.h>
#include <EmbeddedTools.h>
#include "./Interrupts.h"

hal_isr_t isrWatchdogInterrupt = {NULL, NULL};

void vStartWatchdog(uint8_t ui8Mode, uint8_t ui8Time){
    uint8_t ui8BackupWDTCSR = 0;
    ui8BackupWDTCSR |= ((ui8Mode & 1) << WDIE) | ((ui8Mode & 2) << WDE) | ((ui8Time & 7) << WDP0) | ((ui8Time & 8) << 1);
    vEraseBit(SREG, 7);
    WDTCSR |= (1 << WDCE) | (1 << WDE);
    WDTCSR |= ui8BackupWDTCSR;
    vSetBit(SREG, 7);
}

void vStopWatchdog(void){
    vEraseBit(SREG, 7);
    MCUSR &= ~(1<<WDRF);
    WDTCSR |= (1 << WDCE) | (1 << WDE);
    WDTCSR = 0;
    vSetBit(SREG, 7);
}

void vAttachWatchdogInterrupt(void (*vInterruptFunction)(void*), void* vpArgument){
    isrWatchdogInterrupt.vInterruptFunction = vInterruptFunction;
    isrWatchdogInterrupt.vpArgument = vpArgument;
}

void vDettachWatchdogInterrupt(void){
    vAttachWatchdogInterrupt(NULL, NULL);
}

ISR(WDT_vect){
    if (isrWatchdogInterrupt.vInterruptFunction != NULL){
        isrWatchdogInterrupt.vInterruptFunction(isrWatchdogInterrupt.vpArgument);
    }
}