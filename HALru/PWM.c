#include "PWM.h"

void vEnablePWMMode(volatile uint8_t* ui8pTIMERGroup, uint8_t ui8PWMMode){
  if (ui8pTIMERGroup == TIMER_0 || ui8pTIMERGroup == TIMER_2){
    vSetBit(*regTCCRA(ui8pTIMERGroup), WGMn0);
    vSetBit(*regTCCRA(ui8pTIMERGroup), WGMn1);
    if (ui8PWMMode == PWM_AUTOMATIC_MODE){
      vSetTIMERPrescaler(ui8pTIMERGroup, TIMER_PS_1024);
      vEraseBit(*regTCCRA(ui8pTIMERGroup), WGMn2);
    }
    else{
      vSetBit(*regTCCRA(ui8pTIMERGroup), WGMn2);
    }
  }
  else{
    vSetBit(*regTCCRB(ui8pTIMERGroup), WGMn2);
    if (ui8PWMMode == PWM_AUTOMATIC_MODE){
      vSetTIMERPrescaler(ui8pTIMERGroup, TIMER_PS_256);
      vSetBit(*regTCCRA(ui8pTIMERGroup), WGMn0);
      vEraseBit(*regTCCRA(ui8pTIMERGroup), WGMn1);
      vEraseBit(*regTCCRB(ui8pTIMERGroup), WGMn3);
    }
    else{
      vEraseBit(*regTCCRA(ui8pTIMERGroup), WGMn0);
      vSetBit(*regTCCRA(ui8pTIMERGroup), WGMn1);
      vSetBit(*regTCCRB(ui8pTIMERGroup), WGMn3);
    }
    #if defined(ARDUINO_AVR_MEGA2560) || defined(ARDUINO_AVR_MEGA)
      vEraseBit(*(regTCCRA(ui8pTIMERGroup)), COMnC0);
      vSetBit(*(regTCCRA(ui8pTIMERGroup)), COMnC1);
    #endif
  }
  vEraseBit(*(regTCCRA(ui8pTIMERGroup)), COMnA0);
  vEraseBit(*(regTCCRA(ui8pTIMERGroup)), COMnB0);
  vSetBit(*(regTCCRA(ui8pTIMERGroup)), COMnA1);
  vSetBit(*(regTCCRA(ui8pTIMERGroup)), COMnB1);
}

void vDisablePWMMode(volatile uint8_t* ui8pTIMERGroup){
  vEraseBit(*regTCCRA(ui8pTIMERGroup), WGMn0);
  vEraseBit(*regTCCRA(ui8pTIMERGroup), WGMn1);
  vEraseBit(*regTCCRB(ui8pTIMERGroup), WGMn2);
  vEraseBit(*regTCCRB(ui8pTIMERGroup), WGMn3);
  vEraseBit(*(regTCCRA(ui8pTIMERGroup)), COMnA0);
  vEraseBit(*(regTCCRA(ui8pTIMERGroup)), COMnB0);
  vEraseBit(*(regTCCRA(ui8pTIMERGroup)), COMnA1);
  vEraseBit(*(regTCCRA(ui8pTIMERGroup)), COMnB1);
  #if defined(ARDUINO_AVR_MEGA2560) || defined(ARDUINO_AVR_MEGA)
    vEraseBit(*(regTCCRA(ui8pTIMERGroup)), COMnC0);
    vEraseBit(*(regTCCRA(ui8pTIMERGroup)), COMnC1);
  #endif
  if (ui8pTIMERGroup == TIMER_0 || ui8pTIMERGroup == TIMER_2){
    vSetBit(*regTCCRA(ui8pTIMERGroup), WGMn1);
  }
  else{
    vSetBit(*regTCCRB(ui8pTIMERGroup), WGMn2);
    vSetBit(*regTCCRB(ui8pTIMERGroup), WGMn3);
  }
}

uint16_t ui16GetTIMERMaxDutyCicle(volatile uint8_t* ui8pTIMERGroup){
  if (ui8pTIMERGroup == TIMER_0 || ui8pTIMERGroup == TIMER_2) {
    return (*(regOCRAL(ui8pTIMERGroup)));
  }
  else {
    return (*(regICRL(ui8pTIMERGroup))) | ((*(regICRH(ui8pTIMERGroup))) << 8);
  }
}

void vSetPWMAbsoluteDutyCicle(volatile uint8_t* ui8pTIMERGroup, uint8_t ui8PWMGroup, uint16_t ui16DutyCicle){
  if (ui8pTIMERGroup == TIMER_0 || ui8pTIMERGroup == TIMER_2){
    if (ui8PWMGroup == PWM_GROUP_A){
      *(regOCRAL(ui8pTIMERGroup)) = ui16DutyCicle & 255;
    }
    else{
      *(regOCRBL(ui8pTIMERGroup)) = ui16DutyCicle & 255;
    }
  }
  else{
    switch(ui8PWMGroup){
      case PWM_GROUP_A:
      {
        *(regOCRAH(ui8pTIMERGroup)) = (ui16DutyCicle >> 8) & 255;
        *(regOCRAL(ui8pTIMERGroup)) = (ui16DutyCicle) & 255;
      }
      break;

      case PWM_GROUP_B:
      {
        *(regOCRBH(ui8pTIMERGroup)) = (ui16DutyCicle >> 8) & 255;
        *(regOCRBL(ui8pTIMERGroup)) = (ui16DutyCicle) & 255;
      }
      break;

      #if defined(ARDUINO_AVR_MEGA2560) || defined(ARDUINO_AVR_MEGA)
        case PWM_GROUP_C:
        {
          *(regOCRCH(ui8pTIMERGroup)) = (ui16DutyCicle >> 8) & 255;
          *(regOCRCL(ui8pTIMERGroup)) = (ui16DutyCicle) & 255;
        }
        break;
      #endif
    }
  }
}

void vSetPWMRelativeDutyCicle(volatile uint8_t* ui8pTIMERGroup, uint8_t ui8PWMGroup, uint16_t ui16DutyCicle, uint16_t ui16Resolution){
  ui16DutyCicle = ((float) (ui16DutyCicle)/(ui16Resolution)) * ui16GetTIMERMaxDutyCicle(ui8pTIMERGroup);
  vSetPWMAbsoluteDutyCicle(ui8pTIMERGroup, ui8PWMGroup, ui16DutyCicle);
}
