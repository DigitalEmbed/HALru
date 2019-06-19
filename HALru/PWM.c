#include "PWM.h"

void vPWMInterrupt(void* vpArgs);

void vInitPWM(pwm_t* pwmGroup, volatile uint8_t* ioGroup, volatile uint8_t* tmrGroup){
  pwmGroup->ioGroup = ioGroup;
  pwmGroup->tmrGroup = tmrGroup;
  pwmGroup->ui8TickCounter = 0;
  memset((uint8_t*) pwmGroup->ui8pActivedPinsVector, 255, 8*sizeof(uint8_t));
  memset((uint8_t*) pwmGroup->ui8pDutyCicleVector, 0, 8*sizeof(uint8_t));
  vTIMERInit(pwmGroup->tmrGroup);
}

void vSetPWMPeriodUS(pwm_t* pwmGroup, uint32_t ui32PeriodUS){
  vSetTIMERPeriodUS(pwmGroup->tmrGroup, (ui32PeriodUS >> 8));
  vAttachTIMERInterrupt(pwmGroup->tmrGroup, MASTER_TIMER, &vPWMInterrupt, pwmGroup);
  vEnableTIMER(pwmGroup->tmrGroup, MASTER_TIMER);
}

void vSetPWMPin(pwm_t* pwmGroup, uint8_t ui8Pin, uint8_t ui8DutyCicle){
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

void vPWMInterrupt(void* vpArgs){
  uint8_t ui8Counter = 0;
  pwm_t* pwmGroup = (pwm_t*) vpArgs;
  pwmGroup->ui8TickCounter++;
  for(ui8Counter = 0 ; pwmGroup->ui8pActivedPinsVector[ui8Counter] != 255 ; ui8Counter++){
    if (pwmGroup->ui8pDutyCicleVector[ui8Counter] == pwmGroup->ui8TickCounter || pwmGroup->ui8TickCounter == 0){
      vToggleGPIOPin(pwmGroup->ioGroup, pwmGroup->ui8pActivedPinsVector[ui8Counter]);
    }
  }
}
