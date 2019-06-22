#include "SoftPWM.h"

void vSoftPWMInterrupt(void* vpArgs);                                               /*!< Void type function. */

//! Function: SoftPWM Group Initializer
/*!
  Initialize a SoftPWM group.
  \param pwmGroup is a pwm_group_t pointer. It's the pwm group configurations.
  \param ioGroup is a volatile 8-bit pointer integer. It's the GPIO group (IO_GROUP_X).
  \param tmrGroup is a volatile 8-bit pointer integer. It's the TIMER group (TIMER_X).
*/
void vInitSoftPWM(pwm_group_t* pwmGroup, volatile uint8_t* ioGroup, volatile uint8_t* tmrGroup){
  pwmGroup->ioGroup = ioGroup;
  pwmGroup->tmrGroup = tmrGroup;
  pwmGroup->ui8TickCounter = 0;
  memset((uint8_t*) pwmGroup->ui8pActivedPinsVector, 255, 8*sizeof(uint8_t));
  memset((uint8_t*) pwmGroup->ui8pDutyCicleVector, 0, 8*sizeof(uint8_t));
  vTIMERInit(pwmGroup->tmrGroup);
}

//! Function: SoftPWM Disabler
/*!
  Disable a SoftPWM group.
  \param pwmGroup is a pwm_group_t pointer. It's the pwm group configurations.
*/
void vDisableSoftPWM(pwm_group_t* pwmGroup){
  vDisableTIMER(pwmGroup->tmrGroup, MASTER_TIMER);
  *regPORT(pwmGroup->ioGroup) = 0;
}

//! Function: SoftPWM Period (us) Seter
/*!
  Set SoftPWM period (in us).
  \param pwmGroup is a pwm_group_t pointer. It's the pwm group configurations.
  \param ui32PeriodUS is a 32-bit integer. It's the period value, in microseconds.
*/
void vSetSoftPWMPeriodUS(pwm_group_t* pwmGroup, uint32_t ui32PeriodUS){
  vSetTIMERPeriodUS(pwmGroup->tmrGroup, (ui32PeriodUS >> 8));
  vAttachTIMERInterrupt(pwmGroup->tmrGroup, MASTER_TIMER, &vSoftPWMInterrupt, pwmGroup);
  vEnableTIMER(pwmGroup->tmrGroup, MASTER_TIMER);
}

//! Function: SoftPWM Period (us) Seter
/*!
  Set SoftPWM period (in us).
  \param pwmGroup is a pwm_group_t pointer. It's the pwm group configurations.
  \param ui16FrequencyHZ is a 16-bit integer. It's the frequency value, in hertz.
*/
void vSetSoftPWMFrequencyHZ(pwm_group_t* pwmGroup, uint16_t ui16FrequencyHZ){
  uint32_t ui32PeriodUS = 1000000/ui16FrequencyHZ;
  vSetSoftPWMPeriodUS(pwmGroup, ui32PeriodUS);
}

//! Function: SoftPWM Pin Setter
/*!
  Set a pwm value on a SoftPWM pin.
  \param pwmGroup is a pwm_group_t pointer. It's the pwm group configurations.
  \param ui8Pin is a 8-bit integer. It's the pin number of a GPIO group (0 to 7).
  \param ui8DutyCicle is a 8-bit integer. It's the pwm duty cicle of the pin (0 to 255).
*/
void vSetSoftPWMPin(pwm_group_t* pwmGroup, uint8_t ui8Pin, uint8_t ui8DutyCicle){
  uint8_t ui8Counter = 0;
  if (ui8Pin > 7){
    return;
  }
  for (ui8Counter = 0 ; pwmGroup->ui8pActivedPinsVector[ui8Counter] != 255 ; ui8Counter++){
    if (pwmGroup->ui8pActivedPinsVector[ui8Counter] == ui8Pin){
      break;
    }
  }
  if (pwmGroup->ui8pActivedPinsVector[ui8Counter] == 255){
    vSetGPIOPinMode(pwmGroup->ioGroup, ui8Pin, OUTPUT_MODE);
    pwmGroup->ui8pActivedPinsVector[ui8Counter] = ui8Pin;
  }
  pwmGroup->ui8pDutyCicleVector[ui8Counter] = ui8DutyCicle;
}

//! Callback: SoftPWM Interruption
/*!
  It's a callback function used in SoftPWM.
  \param vpArgs is a void pointer.
*/
void vSoftPWMInterrupt(void* vpArgs){
  uint8_t ui8Counter = 0;
  pwm_group_t* pwmGroup = (pwm_group_t*) vpArgs;
  pwmGroup->ui8TickCounter++;
  for(ui8Counter = 0 ; pwmGroup->ui8pActivedPinsVector[ui8Counter] != 255 ; ui8Counter++){
    if (pwmGroup->ui8pDutyCicleVector[ui8Counter] == pwmGroup->ui8TickCounter){
      vUnsetGPIOPin(pwmGroup->ioGroup, pwmGroup->ui8pActivedPinsVector[ui8Counter]);
    }
    else if (pwmGroup->ui8TickCounter == 0){
      vSetGPIOPin(pwmGroup->ioGroup, pwmGroup->ui8pActivedPinsVector[ui8Counter]);
    }
  }
}
