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
  volatile uint8_t ui8ActivedMasterTimer[6] = {0};
  volatile uint8_t ui8ActivedSubTimerA[6] = {0};
  volatile uint8_t ui8ActivedSubTimerB[4] = {0};
  volatile uint8_t ui8ActivedSubTimerC[4] = {0};
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
  isr_timer_t isrSubTimerB = NULL;
  args_timer_t argSubTimerB = NULL;
  volatile uint8_t ui8ActivedMasterTimer[3] = {0};
  volatile uint8_t ui8ActivedSubTimerA[3] = {0};
  volatile uint8_t ui8ActivedSubTimerB = 0;
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
  volatile uint8_t ui8TIMERNumber = usrGetTIMERNumber(ui8pGroup);
  if (ui8SubTimer == MASTER_TIMER){
    ui8ActivedMasterTimer[ui8TIMERNumber] = 1;
    if (ui8pGroup == TIMER_0 || ui8pGroup == TIMER_2){
      vSetBit(*(regTIMSK(ui8pGroup)), OCIEnA);
    }
    else {
      vSetBit(*(regTIMSK(ui8pGroup)), ICIEn);
    }
  }
  else{
    if(ui8SubTimer == SUBTIMER_A){
      ui8ActivedSubTimerA[ui8TIMERNumber] = 1;
      if (ui8pGroup == TIMER_0 || ui8pGroup == TIMER_2){
        vSetBit(*(regTIMSK(ui8pGroup)), OCIEnB);
      }
      else{
        vSetBit(*(regTIMSK(ui8pGroup)), OCIEnA);
      }
    }
    else if(ui8SubTimer == SUBTIMER_B && ui8pGroup != TIMER_0 && ui8pGroup != TIMER_2){
      #if defined(ARDUINO_AVR_MEGA2560) || defined(ARDUINO_AVR_MEGA)
        ui8ActivedSubTimerB[ui8TIMERNumber] = 1;
      #else
        ui8ActivedSubTimerB = 1;
      #endif
      vSetBit(*(regTIMSK(ui8pGroup)), OCIEnB);
    }
    #if defined(ARDUINO_AVR_MEGA2560) || defined(ARDUINO_AVR_MEGA)
      else if(ui8SubTimer == SUBTIMER_C && ui8pGroup != TIMER_0 && ui8pGroup != TIMER_2){
        ui8ActivedSubTimerC[ui8TIMERNumber] = 1;
        vSetBit(*(regTIMSK(ui8pGroup)), OCIEnC);
      }
    #endif
  }
}

//! Function: TIMER Disabler
/*!
  Disable a TIMER.
  \param ui8pGroup is a volatile 8-bit pointer integer. It's the TIMER group (TIMER_X).
  \param ui8SubTimer is a 8-bit integer. It's the sub-timer.
*/
void vDisableTIMER(volatile uint8_t* ui8pGroup, uint8_t ui8SubTimer){
  volatile uint8_t ui8TIMERNumber = usrGetTIMERNumber(ui8pGroup);
  if (ui8SubTimer == MASTER_TIMER){
    ui8ActivedMasterTimer[ui8TIMERNumber] = 0;
    if (ui8pGroup == TIMER_0 || ui8pGroup == TIMER_2){
      vEraseBit(*(regTIMSK(ui8pGroup)), OCIEnA);
    }
    else {
      vEraseBit(*(regTIMSK(ui8pGroup)), ICIEn);
    }
  }
  else{
    if(ui8SubTimer == SUBTIMER_A){
      ui8ActivedSubTimerA[ui8TIMERNumber] = 0;
      if (ui8pGroup == TIMER_0 || ui8pGroup == TIMER_2){
        vEraseBit(*(regTIMSK(ui8pGroup)), OCIEnB);
      }
      else{
        vEraseBit(*(regTIMSK(ui8pGroup)), OCIEnA);
      }
    }
    else if(ui8SubTimer == SUBTIMER_B && ui8pGroup != TIMER_0 && ui8pGroup != TIMER_2){
      #if defined(ARDUINO_AVR_MEGA2560) || defined(ARDUINO_AVR_MEGA)
        ui8ActivedSubTimerB[ui8TIMERNumber] = 0;
      #else
        ui8ActivedSubTimerB = 0;
      #endif
      vEraseBit(*(regTIMSK(ui8pGroup)), OCIEnB);
    }
    #if defined(ARDUINO_AVR_MEGA2560) || defined(ARDUINO_AVR_MEGA)
      else if(ui8SubTimer == SUBTIMER_C && ui8pGroup != TIMER_0 && ui8pGroup != TIMER_2){
        ui8ActivedSubTimerC[ui8TIMERNumber] = 0;
        vEraseBit(*(regTIMSK(ui8pGroup)), OCIEnC);
      }
    #endif
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
  \param ui32PeriodUS is a 32-bit integer. It's the period value, in microseconds.
*/
void vSetTIMERPeriodUS(volatile uint8_t* ui8pGroup, uint32_t ui32PeriodUS){
  uint8_t ui8PrescalerCounter = 0;
  uint8_t ui8Prescaler[7] = {0, 3, 5, 6, 7, 8, 10};
  uint32_t ui32CounterLimit = 0;
  for (ui8PrescalerCounter = 0 ; ui8PrescalerCounter < 7 ; ui8PrescalerCounter++){
    ui32CounterLimit = (ui32PeriodUS) * (DEVICE_CLOCK_HZ >> ui8Prescaler[ui8PrescalerCounter])/1000000 - 1;
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
  \param ui8pGroup is a volatile 8-bit pointer integer. It's the TIMER group (TIMER_X).
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
      #if defined(ARDUINO_AVR_MEGA2560) || defined(ARDUINO_AVR_MEGA)
        isrSubTimerB[ui8TIMERNumber] = vInterruptFunction;
        argSubTimerB[ui8TIMERNumber] = vpArgument;
      #else
        isrSubTimerB = vInterruptFunction;
        argSubTimerB = vpArgument;
      #endif
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
  \param ui8pGroup is a volatile 8-bit pointer integer. It's the TIMER group (TIMER_X).
  \param ui8SubTimer is a 8-bit integer. It's the prescaler timer.
*/
void vDettachTIMERInterrupt(volatile uint8_t* ui8pGroup, uint8_t ui8SubTimer){
  vAttachTIMERInterrupt(ui8pGroup, ui8SubTimer, NULL, NULL);
}

//! Function: TIMER Interrupt Ignorer
/*!
  Ignore a TIMER interruption flag.
  \param ui8pGroup is a volatile 8-bit pointer integer. It's the TIMER group (TIMER_X).
  \param ui8SubTimer is a 8-bit integer. It's the prescaler timer.
*/
void vIgnoreTIMERRequest(volatile uint8_t* ui8pGroup, uint8_t ui8SubTimer){
  if (ui8pGroup == TIMER_0 || ui8pGroup == TIMER_2){
    if (ui8SubTimer == MASTER_TIMER){
      vSetBit(*(regTIFR(ui8pGroup)), OCFnA);
    }
    else{
      vSetBit(*(regTIFR(ui8pGroup)), OCFnB);
    }
  }
  else{
    switch (ui8SubTimer) {
      case MASTER_TIMER:
      {
        vSetBit(*(regTIFR(ui8pGroup)), ICFn);
      }
      break;

      case SUBTIMER_A:
      {
        vSetBit(*(regTIFR(ui8pGroup)), OCFnA);
      }
      break;

      case SUBTIMER_B:
      {
        vSetBit(*(regTIFR(ui8pGroup)), OCFnB);
      }
      break;

      case SUBTIMER_C:
      {
        vSetBit(*(regTIFR(ui8pGroup)), OCFnC);
      }
      break;

    }
  }
}

//! Function: TIMER Interrupt Forcer
/*!
  Force a TIMER interruption.
  \param ui8pGroup is a volatile 8-bit pointer integer. It's the TIMER group (TIMER_X).
  \param ui8SubTimer is a 8-bit integer. It's the prescaler timer.
*/
void vForceTIMERInterrupt(volatile uint8_t* ui8pGroup, uint8_t ui8SubTimer){
  if (ui8pGroup == TIMER_0 || ui8pGroup == TIMER_2){
    if (ui8SubTimer == MASTER_TIMER){
      vEraseBit(*(regTIFR(ui8pGroup)), OCFnA);
    }
    else{
      vEraseBit(*(regTIFR(ui8pGroup)), OCFnB);
    }
  }
  else{
    switch (ui8SubTimer) {
      case MASTER_TIMER:
      {
        vEraseBit(*(regTIFR(ui8pGroup)), ICFn);
      }
      break;

      case SUBTIMER_A:
      {
        vEraseBit(*(regTIFR(ui8pGroup)), OCFnA);
      }
      break;

      case SUBTIMER_B:
      {
        vEraseBit(*(regTIFR(ui8pGroup)), OCFnB);
      }
      break;

      case SUBTIMER_C:
      {
        vEraseBit(*(regTIFR(ui8pGroup)), OCFnC);
      }
      break;

    }
  }
}

//! Callbacks: TIMER Interruptions
/*!
  Callbacks of TIMER hardware interruptions.
*/
ISR(TIMER0_COMPA_vect){
  if (isrMasterTimer[0] != NULL){
    vEraseBit(*(regTIMSK(TIMER_0)), OCIEnA);
    isrMasterTimer[0](argMasterTimer[0]);
    if (ui8ActivedMasterTimer[0] == 1){
      vSetBit(*(regTIMSK(TIMER_0)), OCIEnA);
    }
  }
}

ISR(TIMER0_COMPB_vect){
  if (isrSubTimerA[0] != NULL){
    vEraseBit(*(regTIMSK(TIMER_0)), OCIEnA);
    isrSubTimerA[0](argSubTimerA[0]);
    if (ui8ActivedSubTimerA[0] == 1){
      vSetBit(*(regTIMSK(TIMER_0)), OCIEnA);
    }
  }
}

ISR(TIMER1_CAPT_vect){
  if (isrMasterTimer[1] != NULL){
    vEraseBit(*(regTIMSK(TIMER_1)), ICIEn);
    isrMasterTimer[1](argMasterTimer[1]);
    if (ui8ActivedMasterTimer[1] == 1){
      vSetBit(*(regTIMSK(TIMER_1)), ICIEn);
    }
  }
}

ISR(TIMER1_COMPA_vect){
  if (isrSubTimerA[1] != NULL){
    vEraseBit(*(regTIMSK(TIMER_1)), OCIEnA);
    isrSubTimerA[1](argSubTimerA[1]);
    if (ui8ActivedSubTimerA[1] == 1){
      vSetBit(*(regTIMSK(TIMER_1)), OCIEnA);
    }
  }
}

ISR(TIMER1_COMPB_vect){
  #if defined(ARDUINO_AVR_MEGA2560) || defined(ARDUINO_AVR_MEGA)
    if (isrSubTimerB[1] != NULL){
      vEraseBit(*(regTIMSK(TIMER_1)), OCIEnB);
      isrSubTimerB[1](argSubTimerB[1]);
      if (ui8ActivedSubTimerB[1] == 1){
        vSetBit(*(regTIMSK(TIMER_1)), OCIEnB);
      }
    }
  #else
    if (isrSubTimerB != NULL){
      vEraseBit(*(regTIMSK(TIMER_1)), OCIEnB);
      isrSubTimerB(argSubTimerB);
      if (ui8ActivedSubTimerB == 1){
        vSetBit(*(regTIMSK(TIMER_1)), OCIEnB);
      }
    }
  #endif
}

ISR(TIMER2_COMPA_vect){
  if (isrMasterTimer[2] != NULL){
    vEraseBit(*(regTIMSK(TIMER_2)), OCIEnA);
    isrMasterTimer[2](argMasterTimer[2]);
    if (ui8ActivedMasterTimer[2] == 1){
      vSetBit(*(regTIMSK(TIMER_2)), OCIEnA);
    }
  }
}

ISR(TIMER2_COMPB_vect){
  if (isrSubTimerA[2] != NULL){
    vEraseBit(*(regTIMSK(TIMER_2)), OCIEnA);
    isrSubTimerA[2](argSubTimerA[2]);
    if (ui8ActivedSubTimerA[2] == 1){
      vSetBit(*(regTIMSK(TIMER_2)), OCIEnA);
    }
  }
}

#if defined(ARDUINO_AVR_MEGA2560) || defined(ARDUINO_AVR_MEGA)
  ISR(TIMER1_COMPC_vect){
    if (isrSubTimerC[1] != NULL){
      vEraseBit(*(regTIMSK(TIMER_1)), OCIEnC);
      isrSubTimerC[1](argSubTimerC[1]);
      if (ui8ActivedSubTimerC[1] == 1){
        vSetBit(*(regTIMSK(TIMER_1)), OCIEnC);
      }
    }
  }

  ISR(TIMER3_CAPT_vect){
    if (isrMasterTimer[3] != NULL){
      vEraseBit(*(regTIMSK(TIMER_3)), ICIEn);
      isrMasterTimer[3](argMasterTimer[3]);
      if (ui8ActivedMasterTimer[3] == 1){
        vSetBit(*(regTIMSK(TIMER_3)), ICIEn);
      }
    }
  }

  ISR(TIMER3_COMPA_vect){
    if (isrSubTimerA[3] != NULL){
      vEraseBit(*(regTIMSK(TIMER_3)), OCIEnA);
      isrSubTimerA[3](argSubTimerA[3]);
      if (ui8ActivedSubTimerA[3] == 1){
        vSetBit(*(regTIMSK(TIMER_3)), OCIEnA);
      }
    }
  }

  ISR(TIMER3_COMPB_vect){
    if (isrSubTimerB[3] != NULL){
      vEraseBit(*(regTIMSK(TIMER_3)), OCIEnB);
      isrSubTimerB[3](argSubTimerB[3]);
      if (ui8ActivedSubTimerB[3] == 1){
        vSetBit(*(regTIMSK(TIMER_3)), OCIEnB);
      }
    }
  }

  ISR(TIMER3_COMPC_vect){
    if (isrSubTimerC[3] != NULL){
      vEraseBit(*(regTIMSK(TIMER_3)), OCIEnC);
      isrSubTimerC[3](argSubTimerC[3]);
      if (ui8ActivedSubTimerC[3] == 1){
        vSetBit(*(regTIMSK(TIMER_3)), OCIEnC);
      }
    }
  }

  ISR(TIMER4_CAPT_vect){
    if (isrMasterTimer[4] != NULL){
      vEraseBit(*(regTIMSK(TIMER_4)), ICIEn);
      isrMasterTimer[4](argMasterTimer[4]);
      if (ui8ActivedMasterTimer[4] == 1){
        vSetBit(*(regTIMSK(TIMER_4)), ICIEn);
      }
    }
  }

  ISR(TIMER4_COMPA_vect){
    if (isrSubTimerA[4] != NULL){
      vEraseBit(*(regTIMSK(TIMER_4)), OCIEnA);
      isrSubTimerA[4](argSubTimerA[4]);
      if (ui8ActivedSubTimerA[4] == 1){
        vSetBit(*(regTIMSK(TIMER_4)), OCIEnA);
      }
    }
  }

  ISR(TIMER4_COMPB_vect){
    if (isrSubTimerB[4] != NULL){
      vEraseBit(*(regTIMSK(TIMER_4)), OCIEnB);
      isrSubTimerB[4](argSubTimerB[4]);
      if (ui8ActivedSubTimerB[4] == 1){
        vSetBit(*(regTIMSK(TIMER_4)), OCIEnB);
      }
    }
  }

  ISR(TIMER4_COMPC_vect){
    if (isrSubTimerC[4] != NULL){
      vEraseBit(*(regTIMSK(TIMER_4)), OCIEnC);
      isrSubTimerC[4](argSubTimerC[4]);
      if (ui8ActivedSubTimerC[4] == 1){
        vSetBit(*(regTIMSK(TIMER_4)), OCIEnC);
      }
    }
  }

  ISR(TIMER5_CAPT_vect){
    if (isrMasterTimer[5] != NULL){
      vEraseBit(*(regTIMSK(TIMER_5)), ICIEn);
      isrMasterTimer[5](argMasterTimer[5]);
      if (ui8ActivedMasterTimer[5] == 1){
        vSetBit(*(regTIMSK(TIMER_5)), ICIEn);
      }
    }
  }

  ISR(TIMER5_COMPA_vect){
    if (isrSubTimerA[5] != NULL){
      vEraseBit(*(regTIMSK(TIMER_5)), OCIEnA);
      isrSubTimerA[5](argSubTimerA[5]);
      if (ui8ActivedSubTimerA[5] == 1){
        vSetBit(*(regTIMSK(TIMER_5)), OCIEnA);
      }
    }
  }

  ISR(TIMER5_COMPB_vect){
    if (isrSubTimerB[5] != NULL){
      vEraseBit(*(regTIMSK(TIMER_5)), OCIEnB);
      isrSubTimerB[5](argSubTimerB[5]);
      if (ui8ActivedSubTimerB[5] == 1){
        vSetBit(*(regTIMSK(TIMER_5)), OCIEnB);
      }
    }
  }

  ISR(TIMER5_COMPC_vect){
    if (isrSubTimerC[5] != NULL){
      vEraseBit(*(regTIMSK(TIMER_5)), OCIEnC);
      isrSubTimerC[5](argSubTimerC[5]);
      if (ui8ActivedSubTimerC[5] == 1){
        vSetBit(*(regTIMSK(TIMER_5)), OCIEnC);
      }
    }
  }
#endif
