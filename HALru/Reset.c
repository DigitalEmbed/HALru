#include "Reset.h"

//! Function: System Reset Reason Getter
/*!
  Get system reset reason.
  \return Returns system reset reason
*/
uint8_t ui8GetSystemResetReason(void){
    static uint16_t ui16ResetReason = 65535;
    if (ui16ResetReason == 65535){
        ui16ResetReason = (MCUSR & 15) & 255;
        MCUSR &= 240;
    }
    return (uint8_t) ui16ResetReason & 255;
}