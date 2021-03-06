#include "SoftPWM.h"

void vSoftPWMInterrupt(void* vpArgs);                                               /*!< Void type function. */

//! Function: SoftPWM Group Initializer
/*!
  Initializes a SoftPWM group.
  \param pwmGroup is a pwm_group_t pointer. It's the pwm group configurations.
  \param ioGroup is a volatile 8-bit pointer integer. It's the GPIO group (IO_GROUP_X).
  \param tmrGroup is a volatile 8-bit pointer integer. It's the TIMER group (TIMER_X).
*/
void vInitSoftPWM(pwm_group_t* pwmGroup, volatile uint8_t* ioGroup, volatile uint8_t* tmrGroup){
  pwmGroup->ioGroup = ioGroup;
  pwmGroup->tmrGroup = tmrGroup;
  pwmGroup->ui8TickCounter = 0;
  pwmGroup->ui8Resolution = 255;
  memset((uint8_t*) pwmGroup->ui8pActivedPinsVector, 255, 8*sizeof(uint8_t));
  memset((uint8_t*) pwmGroup->ui8pDutyCicleVector, 0, 8*sizeof(uint8_t));
  vTIMERInit(pwmGroup->tmrGroup);
}

//! Function: SoftPWM Disabler
/*!
  Disables a SoftPWM group.
  \param pwmGroup is a pwm_group_t pointer. It's the pwm group configurations.
*/
void vDisableSoftPWM(pwm_group_t* pwmGroup){
  vDisableTIMERInterrupt(pwmGroup->tmrGroup, MASTER_TIMER);
  *regPORT(pwmGroup->ioGroup) = 0;
}

//! Function: SoftPWM Enabler
/*!
  Enables a SoftPWM group.
  \param pwmGroup is a pwm_group_t pointer. It's the pwm group configurations.
*/
void vEnableSoftPWM(pwm_group_t* pwmGroup){
  vEnableTIMERInterrupt(pwmGroup->tmrGroup, MASTER_TIMER);
}

//! Function: SoftPWM Period (us) Seter
/*!
  Sets SoftPWM period (in us).
  \param pwmGroup is a pwm_group_t pointer. It's the pwm group configurations.
  \param ui32PeriodUS is a 32-bit integer. It's the period value, in microseconds.
*/
void vSetSoftPWMPeriodUS(pwm_group_t* pwmGroup, uint8_t ui8Resolution, uint32_t ui32PeriodUS){
  pwmGroup->ui8Resolution = ui8Resolution;
  vSetTIMERPeriodUS(pwmGroup->tmrGroup, ui32PeriodUS/ui8Resolution);
  vAttachTIMERInterrupt(pwmGroup->tmrGroup, MASTER_TIMER, &vSoftPWMInterrupt, pwmGroup);
}

//! Function: SoftPWM Period (us) Seter
/*!
  Sets SoftPWM period (in us).
  \param pwmGroup is a pwm_group_t pointer. It's the pwm group configurations.
  \param ui16FrequencyHZ is a 16-bit integer. It's the frequency value, in hertz.
*/
void vSetSoftPWMFrequencyHZ(pwm_group_t* pwmGroup, uint8_t ui8Resolution, uint16_t ui16FrequencyHZ){
  uint32_t ui32PeriodUS = 1000000/ui16FrequencyHZ;
  vSetSoftPWMPeriodUS(pwmGroup, ui8Resolution, ui32PeriodUS);
}

//! Function: SoftPWM Pin Setter
/*!
  Sets a pwm value on a SoftPWM pin.
  \param pwmGroup is a pwm_group_t pointer. It's the pwm group configurations.
  \param ui8Pin is a 8-bit integer. It's the pin number of a GPIO group (0 to 7).
  \param ui8DutyCicle is a 8-bit integer. It's the pwm duty cicle of the pin (0 to 255).
*/
void vSetSoftPWMDutyCicle(pwm_group_t* pwmGroup, uint8_t ui8Pin, uint8_t ui8DutyCicle){
  uint8_t ui8Counter = 0;
  if (ui8Pin > 7){
    return;
  }
  if (ui8DutyCicle == 0){
    vDisableSoftPWMPin(pwmGroup, ui8Pin);
    return;
  }
  if (ui8DutyCicle >= pwmGroup->ui8Resolution){
    vDisableSoftPWMPin(pwmGroup, ui8Pin);
    vSetGPIOPin(pwmGroup->ioGroup, ui8Pin);
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
  vIgnoreTIMERRequest(pwmGroup->tmrGroup, MASTER_TIMER);
  vResetTIMERCounter(pwmGroup->tmrGroup);
  vEnableTIMERInterrupt(pwmGroup->tmrGroup, MASTER_TIMER);
}

//! Function: SoftPWM Pin Unsetter
/*!
  Unsets a SoftPWM pin.
  \param pwmGroup is a pwm_group_t pointer. It's the pwm group configurations.
  \param ui8Pin is a 8-bit integer. It's the pin number of a GPIO group (0 to 7).
*/
void vDisableSoftPWMPin(pwm_group_t* pwmGroup, uint8_t ui8Pin){
  uint8_t ui8Counter = 0;
  if (ui8Pin > 7){
    return;
  }
  for (ui8Counter = 0 ; pwmGroup->ui8pActivedPinsVector[ui8Counter] != ui8Pin && ui8Counter < 8 ; ui8Counter++);
  if (ui8Counter == 8){
    return;
  }
  vDisableTIMERInterrupt(pwmGroup->tmrGroup, MASTER_TIMER);
  *regPORT(pwmGroup->ioGroup) = 0;
  for (ui8Counter = ui8Counter ; ui8Counter != 7 && pwmGroup->ui8pActivedPinsVector[ui8Counter + 1] != 255 ; ui8Counter++){
    pwmGroup->ui8pActivedPinsVector[ui8Counter] = pwmGroup->ui8pActivedPinsVector[ui8Counter + 1];
    pwmGroup->ui8pDutyCicleVector[ui8Counter] = pwmGroup->ui8pDutyCicleVector[ui8Counter + 1];
  }
  pwmGroup->ui8pActivedPinsVector[ui8Counter] = 255;
  pwmGroup->ui8pDutyCicleVector[ui8Counter] = 0;
  vIgnoreTIMERRequest(pwmGroup->tmrGroup, MASTER_TIMER);
  vResetTIMERCounter(pwmGroup->tmrGroup);
  if (ui8Counter > 0){
    vEnableTIMERInterrupt(pwmGroup->tmrGroup, MASTER_TIMER);
  }
}

//! Callback: SoftPWM Interruption
/*!
  It's callback function used in SoftPWM.
  \param vpArgs is a void pointer.
*/
void vSoftPWMInterrupt(void* vpArgs){
  uint8_t ui8Counter = 0;
  pwm_group_t* pwmGroup = (pwm_group_t*) vpArgs;
  if (pwmGroup->ui8TickCounter > pwmGroup->ui8Resolution){
    pwmGroup->ui8TickCounter = 0;
    for(ui8Counter = 0 ; pwmGroup->ui8pActivedPinsVector[ui8Counter] != 255 ; ui8Counter++){
      vSetGPIOPin(pwmGroup->ioGroup, pwmGroup->ui8pActivedPinsVector[ui8Counter]);
    }
  }
  else{
    pwmGroup->ui8TickCounter++;
    for(ui8Counter = 0 ; pwmGroup->ui8pActivedPinsVector[ui8Counter] != 255 ; ui8Counter++){
      if (pwmGroup->ui8pDutyCicleVector[ui8Counter] == pwmGroup->ui8TickCounter){
        vUnsetGPIOPin(pwmGroup->ioGroup, pwmGroup->ui8pActivedPinsVector[ui8Counter]);
      }
    }
  }
}
