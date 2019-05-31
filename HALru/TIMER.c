#include "TIMER.h"

//! Declarations: Private TIMER Declarations
/*!
  This macros and variables are for facilitate the use of this library.
*/
#if defined(ARDUINO_AVR_MEGA2560) || defined(ARDUINO_AVR_MEGA)
  isr_timer_t isrMasterTimer[6] = {NULL};
  args_timer_t argMasterTimer[6] = {NULL};
  isr_timer_t isrSubTimerA[6] = {NULL};
  args_timer_t argSubTimerA[6] = {NULL};
  isr_timer_t isrSubTimerB[4] = {NULL};
  args_timer_t argSubTimerB[4] = {NULL};
  isr_timer_t isrSubTimerC[4] = {NULL};
  args_timer_t argSubTimerC[4] = {NULL};
  #define   usrGetTIMERNumber(usrGroup)                       usrGroup == TIMER_0 ? 0 :\
                                                              usrGroup == TIMER_1 ? 1 :\
                                                              usrGroup == TIMER_2 ? 2 :\
                                                              usrGroup == TIMER_3 ? 3 :\
                                                              usrGroup == TIMER_4 ? 4 : 5
#else
  isr_timer_t isrMasterTimer[3] = {NULL};
  args_timer_t argMasterTimer[3] = {NULL};
  isr_timer_t isrSubTimerA[3] = {NULL};
  args_timer_t argSubTimerA[3] = {NULL};
  isr_timer_t isrSubTimerB[1] = {NULL};
  args_timer_t argSubTimerB[1] = {NULL};
  #define   usrGetTIMERNumber(usrGroup)                       usrGroup == TIMER_0 ? 0 :\
                                                              usrGroup == TIMER_1 ? 1 : 2
#endif

//! Function: TIMER Initializer
/*!
  Initialize a TIMER.
  \param ui8pGroup is a volatile 8-bit pointer integer. It's the TIMER group (TIMER_X).
*/
void vTIMERInit(volatile uint8_t* ui8pGroup){
  if (ui8pGroup == TIMER_0 || ui8pGroup == TIMER_2){
    vSetBit(*regTCCRA(ui8pGroup), WGMn1);
  }
  else{
    vSetBit(*regTCCRB(ui8pGroup), WGMn2);
    vSetBit(*regTCCRB(ui8pGroup), WGMn3);
  }
}

//! Function: TIMER Enabler
/*!
  Enable a TIMER.
  \param ui8pGroup is a volatile 8-bit pointer integer. It's the TIMER group (TIMER_X).
  \param ui8SubTimer is a 8-bit integer. It's the sub-timer.
*/
void vEnableTIMER(volatile uint8_t* ui8pGroup, uint8_t ui8SubTimer){
  if (ui8SubTimer == MASTER_TIMER){
    if (ui8pGroup == TIMER_0 || ui8pGroup == TIMER_2){
      vSetBit(*(regTIMSK(ui8pGroup)), OCIEnA);
    }
    else {
      vSetBit(*(regTIMSK(ui8pGroup)), ICIEn);
    }
  }
  else{
    if(ui8SubTimer == SUBTIMER_A){
      if (ui8pGroup == TIMER_0 || ui8pGroup == TIMER_2){
        vSetBit(*(regTIMSK(ui8pGroup)), OCIEnB);
      }
      else{
        vSetBit(*(regTIMSK(ui8pGroup)), OCIEnA);
      }
    }
    else if(ui8SubTimer == SUBTIMER_B && ui8pGroup != TIMER_0 && ui8pGroup != TIMER_2){
      vSetBit(*(regTIMSK(ui8pGroup)), OCIEnB);
    }
    else if(ui8SubTimer == SUBTIMER_C && ui8pGroup != TIMER_0 && ui8pGroup != TIMER_2){
      vSetBit(*(regTIMSK(ui8pGroup)), OCIEnC);
    }
  }
}

//! Function: TIMER Disabler
/*!
  Disable a TIMER.
  \param ui8pGroup is a volatile 8-bit pointer integer. It's the TIMER group (TIMER_X).
  \param ui8SubTimer is a 8-bit integer. It's the sub-timer.
*/
void vDisableTIMER(volatile uint8_t* ui8pGroup, uint8_t ui8SubTimer){
  if (ui8SubTimer == MASTER_TIMER){
    if (ui8pGroup == TIMER_0 || ui8pGroup == TIMER_2){
      vEraseBit(*(regTIMSK(ui8pGroup)), OCIEnA);
    }
    else {
      vEraseBit(*(regTIMSK(ui8pGroup)), ICIEn);
    }
  }
  else{
    if(ui8SubTimer == SUBTIMER_A){
      if (ui8pGroup == TIMER_0 || ui8pGroup == TIMER_2){
        vEraseBit(*(regTIMSK(ui8pGroup)), OCIEnB);
      }
      else{
        vEraseBit(*(regTIMSK(ui8pGroup)), OCIEnA);
      }
    }
    else if(ui8SubTimer == SUBTIMER_B && ui8pGroup != TIMER_0 && ui8pGroup != TIMER_2){
      vEraseBit(*(regTIMSK(ui8pGroup)), OCIEnB);
    }
    else if(ui8SubTimer == SUBTIMER_C && ui8pGroup != TIMER_0 && ui8pGroup != TIMER_2){
      vEraseBit(*(regTIMSK(ui8pGroup)), OCIEnC);
    }
  }
}

//! Function: TIMER Prescaler Seter
/*!
  Set a prescaler TIMER.
  \param ui8pGroup is a volatile 8-bit pointer integer. It's the TIMER group (TIMER_X).
  \param ui8SubTimer is a 8-bit integer. It's the prescaler timer.
*/
void vSetTIMERPrescaler(volatile uint8_t* ui8pGroup, uint8_t ui8TIMERPrescaler){
  if (ui8pGroup != TIMER_2){
    if (ui8TIMERPrescaler > TIMER_PS_8 && ui8TIMERPrescaler <= TIMER_PS_64){
      ui8TIMERPrescaler--;
    }
    else if(ui8TIMERPrescaler > TIMER_PS_64){
      ui8TIMERPrescaler -= 2;
    }
  }
  vCopyBits(*regTCCRB(ui8pGroup), CSn0, ui8TIMERPrescaler, 0, 3);
}

//! Function: TIMER Counter Seter
/*!
  Set a counter TIMER.
  \param ui8pGroup is a volatile 8-bit pointer integer. It's the TIMER group (TIMER_X).
  \param ui16TIMERLimit is a 16-bit integer. It's the counter timer.
*/
void vSetTIMERCounterLimit(volatile uint8_t* ui8pGroup, uint16_t ui16TIMERLimit){
  if ((ui8pGroup == TIMER_0 || ui8pGroup == TIMER_2) && ui16TIMERLimit < 256){
    *(regOCRAL(ui8pGroup)) = ui16TIMERLimit & 255;
    *(regOCRBL(ui8pGroup)) = (ui16TIMERLimit >> 1) & 255;
  }
  else{
    *(regICRH(ui8pGroup)) = (ui16TIMERLimit >> 8) & 255;
    *(regICRL(ui8pGroup)) = (ui16TIMERLimit) & 255;
    *(regOCRAH(ui8pGroup)) = (ui16TIMERLimit >> 10) & 255;
    *(regOCRAL(ui8pGroup)) = (ui16TIMERLimit >> 2) & 255;
    *(regOCRBH(ui8pGroup)) = (ui16TIMERLimit >> 9) & 255;
    *(regOCRBL(ui8pGroup)) = (ui16TIMERLimit >> 1) & 255;
    #if defined(ARDUINO_AVR_MEGA2560) || defined(ARDUINO_AVR_MEGA)
      *(regOCRCH(ui8pGroup)) =  (((ui16TIMERLimit >> 1) + (ui16TIMERLimit >> 2)) >> 8) & 255;
      *(regOCRCL(ui8pGroup)) =  ((ui16TIMERLimit >> 1) + (ui16TIMERLimit >> 2)) & 255;
    #endif
  }
}

//! Function: TIMER Period (ms) Seter
/*!
  Set TIMER period (in ms).
  \param ui8pGroup is a volatile 8-bit pointer integer. It's the TIMER group (TIMER_X).
  \param ui16PeriodMS is a 16-bit integer. It's the period value, in miliseconds.
*/
void vSetTIMERPeriodMS(volatile uint8_t* ui8pGroup, uint16_t ui16PeriodMS){
  if (((ui8pGroup == TIMER_2 || ui8pGroup == TIMER_0) && ui16PeriodMS < 17) || ((ui8pGroup != TIMER_2 && ui8pGroup != TIMER_0) && ui16PeriodMS < 4194)){
    vSetTIMERPrescaler(ui8pGroup, TIMER_PS_1024);
    vSetTIMERCounterLimit(ui8pGroup, ui16PeriodMS * (DEVICE_CLOCK_HZ >> 10)/1000 - 1);
  }
}

//! Function: TIMER Period (us) Seter
/*!
  Set TIMER period (in us).
  \param ui8pGroup is a volatile 8-bit pointer integer. It's the TIMER group (TIMER_X).
  \param ui32PeriodUS is a 16-bit integer. It's the period value, in microseconds.
*/
void vSetTIMERPeriodUS(volatile uint8_t* ui8pGroup, uint32_t ui32PeriodUS){
  uint8_t ui8PrescalerCounter = 0;
  uint8_t ui8Prescaler[7] = {0, 3, 5, 6, 7, 8, 10};
  uint32_t ui32CounterLimit = 0;
  for (ui8PrescalerCounter = 0 ; ui8PrescalerCounter < 7 ; ui8PrescalerCounter++){
    ui32CounterLimit = (ui32PeriodUS/1000000) * (DEVICE_CLOCK_HZ >> ui8Prescaler[ui8PrescalerCounter]) - 1;
    if (ui32CounterLimit < 65535 && ui32CounterLimit > 10){
      vSetTIMERPrescaler(ui8pGroup, ui8PrescalerCounter + 1);
      vSetTIMERCounterLimit(ui8pGroup, (uint16_t) ui32CounterLimit);
      break;
    }
  }
}

//! Function: TIMER Interrupt Attacher
/*!
  Attach a TIMER interruption function.
  \param ui8pGroup is a volatile 8-bit pointer integer. It's the TIEMR group (TIMER_X).
  \param ui8SubTimer is a 8-bit integer. It's the prescaler timer.
  \param vInterruptFunction is a function pointer. It's the callback interruption.
  \param vpArgument is a void pointer. It's the callback argument.
*/
void vAttachTIMERInterrupt(volatile uint8_t* ui8pGroup, uint8_t ui8SubTimer, isr_timer_t vInterruptFunction, void* vpArgument){
  uint8_t ui8TIMERNumber = usrGetTIMERNumber(ui8pGroup);
  switch(ui8SubTimer){

    case MASTER_TIMER:
    {
      isrMasterTimer[ui8TIMERNumber] = vInterruptFunction;
      argMasterTimer[ui8TIMERNumber] = vpArgument;
      break;
    }

    case SUBTIMER_A:
    {
      isrSubTimerA[ui8TIMERNumber] = vInterruptFunction;
      argSubTimerA[ui8TIMERNumber] = vpArgument;
      break;
    }

    case SUBTIMER_B:
    {
      isrSubTimerB[ui8TIMERNumber] = vInterruptFunction;
      argSubTimerB[ui8TIMERNumber] = vpArgument;
      break;
    }

    #if defined(ARDUINO_AVR_MEGA2560) || defined(ARDUINO_AVR_MEGA)
      case SUBTIMER_C:
      {
        isrSubTimerC[ui8TIMERNumber] = vInterruptFunction;
        argSubTimerC[ui8TIMERNumber] = vpArgument;
        break;
      }
    #endif

  }
}

//! Function: TIMER Interrupt Dettacher
/*!
  Dettach a TIMER interruption function.
  \param ui8pGroup is a volatile 8-bit pointer integer. It's the TIEMR group (TIMER_X).
  \param ui8SubTimer is a 8-bit integer. It's the prescaler timer.
*/
void vDettachTIMERInterrupt(volatile uint8_t* ui8pGroup, uint8_t ui8SubTimer){
  uint8_t ui8TIMERNumber = usrGetTIMERNumber(ui8pGroup);
  switch(ui8SubTimer){

    case MASTER_TIMER:
    {
      isrMasterTimer[ui8TIMERNumber] = NULL;
      argMasterTimer[ui8TIMERNumber] = NULL;
      break;
    }

    case SUBTIMER_A:
    {
      isrSubTimerA[ui8TIMERNumber] = NULL;
      argSubTimerA[ui8TIMERNumber] = NULL;
      break;
    }

    case SUBTIMER_B:
    {
      isrSubTimerB[ui8TIMERNumber] = NULL;
      argSubTimerB[ui8TIMERNumber] = NULL;
      break;
    }

    #if defined(ARDUINO_AVR_MEGA2560) || defined(ARDUINO_AVR_MEGA)
      case SUBTIMER_C:
      {
        isrSubTimerC[ui8TIMERNumber] = NULL;
        argSubTimerC[ui8TIMERNumber] = NULL;
        break;
      }
    #endif

  }
}

//! Callbacks: TIMER Interruptions
/*!
  Callbacks of TIMER hardware interruptions.
*/
ISR(TIMER0_COMPA_vect){
  if (isrMasterTimer[0] != NULL){
    isrMasterTimer[0](argMasterTimer[0]);
  }
}

ISR(TIMER0_COMPB_vect){
  if (isrSubTimerA[0] != NULL){
    isrSubTimerA[0](argSubTimerA[0]);
  }
}

ISR(TIMER1_CAPT_vect){
  if (isrMasterTimer[1] != NULL){
    isrMasterTimer[1](argMasterTimer[1]);
  }
}

ISR(TIMER1_COMPA_vect){
  if (isrSubTimerA[1] != NULL){
    isrSubTimerA[1](argSubTimerA[1]);
  }
}

ISR(TIMER1_COMPB_vect){
  if (isrSubTimerB[1] != NULL){
    isrSubTimerB[1](argSubTimerB[1]);
  }
}

ISR(TIMER2_COMPA_vect){
  if (isrMasterTimer[2] != NULL){
    isrMasterTimer[2](argMasterTimer[2]);
  }
}

ISR(TIMER2_COMPB_vect){
  if (isrSubTimerA[2] != NULL){
    isrSubTimerA[2](argSubTimerA[2]);
  }
}

#if defined(ARDUINO_AVR_MEGA2560) || defined(ARDUINO_AVR_MEGA)
  ISR(TIMER1_COMPC_vect){
    if (isrSubTimerC[1] != NULL){
      isrSubTimerC[1](argSubTimerC[1]);
    }
  }

  ISR(TIMER3_CAPT_vect){
    if (isrMasterTimer[3] != NULL){
      isrMasterTimer[3](argMasterTimer[3]);
    }
  }

  ISR(TIMER3_COMPA_vect){
    if (isrSubTimerA[3] != NULL){
      isrSubTimerA[3](argSubTimerA[3]);
    }
  }

  ISR(TIMER3_COMPB_vect){
    if (isrSubTimerB[3] != NULL){
      isrSubTimerB[3](argSubTimerB[3]);
    }
  }

  ISR(TIMER3_COMPC_vect){
    if (isrSubTimerC[3] != NULL){
      isrSubTimerC[3](argSubTimerC[3]);
    }
  }

  ISR(TIMER4_CAPT_vect){
    if (isrMasterTimer[4] != NULL){
      isrMasterTimer[4](argMasterTimer[4]);
    }
  }

  ISR(TIMER4_COMPA_vect){
    if (isrSubTimerA[4] != NULL){
      isrSubTimerA[4](argSubTimerA[4]);
    }
  }

  ISR(TIMER4_COMPB_vect){
    if (isrSubTimerB[4] != NULL){
      isrSubTimerB[4](argSubTimerB[4]);
    }
  }

  ISR(TIMER4_COMPC_vect){
    if (isrSubTimerC[4] != NULL){
      isrSubTimerC[4](argSubTimerC[4]);
    }
  }

  ISR(TIMER5_CAPT_vect){
    if (isrMasterTimer[5] != NULL){
      isrMasterTimer[5](argMasterTimer[5]);
    }
  }

  ISR(TIMER5_COMPA_vect){
    if (isrSubTimerA[5] != NULL){
      isrSubTimerA[5](argSubTimerA[5]);
    }
  }

  ISR(TIMER5_COMPB_vect){
    if (isrSubTimerB[5] != NULL){
      isrSubTimerB[5](argSubTimerB[5]);
    }
  }

  ISR(TIMER5_COMPC_vect){
    if (isrSubTimerC[5] != NULL){
      isrSubTimerC[5](argSubTimerC[5]);
    }
  }
#endif
