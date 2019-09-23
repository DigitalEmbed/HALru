#include "PWM.h"

#include <Print.h>
#include <USART.h>

//! Function: PWM Enabler
/*!
  Enables a PWM mode in a TIMER.
  \param ui8pTIMERGroup is a volatile 8-bit pointer integer. It's the TIMER group (TIMER_X).
  \param ui8PWMMode is a 8-bit integer. The avaliable modes are: PWM_AUTOMATIC_MODE and PWM_MANUAL_MODE.
*/
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

//! Function: PWM Disabler
/*!
  Disables a PWM mode in a TIMER.
  \param ui8pTIMERGroup is a volatile 8-bit pointer integer. It's the TIMER group (TIMER_X).
*/
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

//! Function: PWM Max Duty Cicle Getter
/*!
  Gets pwm max duty cicle.
  \param ui8pTIMERGroup is a volatile 8-bit pointer integer. It's the TIMER group (TIMER_X).
  \return Returns pwm max duty cicle.
*/
uint16_t ui16GetTIMERMaxDutyCicle(volatile uint8_t* ui8pTIMERGroup){
  if (ui8pTIMERGroup == NULL){
    return 0;
  }
  if (ui8pTIMERGroup == TIMER_0 || ui8pTIMERGroup == TIMER_2) {
    return (*(regOCRAL(ui8pTIMERGroup)));
  }
  else {
    uint8_t ui8MaxDutyCicle[2] = {(*(regICRL(ui8pTIMERGroup))), (*(regICRH(ui8pTIMERGroup)))};
    uint16_t ui16MaxDutyCicle = 0;
    memcpy(&ui16MaxDutyCicle, ui8MaxDutyCicle, sizeof(uint16_t));
    return ui16MaxDutyCicle;
  }
}

//! Function: PWM Max Duty Cicle Getter
/*!
  Gets pwm max duty cicle.
  \param ui8pTIMERGroup is a volatile 8-bit pointer integer. It's the TIMER group (TIMER_X).
  \param ui8PWMGroup is a unsigned 8-bit pointer integer. The avaliables PWM groups are: PWM_GROUP_X: A, B or C.
  \param ui16DutyCicle is a unsigned 16-bit pointer integer. It's absolute duty cicle value.
*/
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

//! Function: PWM Max Duty Cicle Getter
/*!
  Gets pwm max duty cicle.
  \param ui8pTIMERGroup is a volatile 8-bit pointer integer. It's the TIMER group (TIMER_X).
  \param ui8PWMGroup is a unsigned 8-bit pointer integer. The avaliables PWM groups are: PWM_GROUP_X: A, B or C.
  \param ui16DutyCicle is a unsigned 16-bit pointer integer. It's relative duty cicle value.
  \param ui16Resolution is a unsigned 16-bit pointer integer. It's a max duty cicle value.
*/
void vSetPWMRelativeDutyCicle(volatile uint8_t* ui8pTIMERGroup, uint8_t ui8PWMGroup, uint16_t ui16DutyCicle, uint16_t ui16Resolution){
  ui16DutyCicle = ((float) (ui16DutyCicle)/(ui16Resolution)) * ui16GetTIMERMaxDutyCicle(ui8pTIMERGroup);
  vSetPWMAbsoluteDutyCicle(ui8pTIMERGroup, ui8PWMGroup, ui16DutyCicle);
}
