#include "Reset.h"

uint8_t ui8GetSystemResetReason(void){
    uint8_t ui8ResetReason = MCUSR & 15;
    MCUSR &= 240;
    return ui8ResetReason;
}