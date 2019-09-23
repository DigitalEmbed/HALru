#include "TIMER.h"
#include <stdio.h>
#include <avr/interrupt.h>
#include <EmbeddedTools.h>
#include "./Configs.h"
#include "./Interrupts.h"

//! Declarations: Private TIMER Declarations
/*!
  These macros and variables are for facilitate the use of this library.
  - ActivedTIMERs (For AVR Mega 2560):
      MSB                                      MASTER_TIMER         SUBTIMER_A       SUBTIMER_B    SUBTIMER_C
      [-][-][-][-][-][-][-][-][-][-][-][-]  [5][4][3][2][1][0]  [5][4][3][2][1][0]  [5][4][3][1]  [5][4][3][1]

  - ActivedTIMERs (For AVR Mega 328p):
      MSB    MASTER      STA     STB
      [-]  [2][1][0]  [2][1][0]  [1]
*/
#if defined(ARDUINO_AVR_MEGA2560) || defined(ARDUINO_AVR_MEGA)
  hal_isr_t isrMasterTimer[6] = {{NULL, NULL}};
  hal_isr_t isrSubTimerA[6] = {{NULL, NULL}};
  hal_isr_t isrSubTimerB[4] = {{NULL, NULL}};
  hal_isr_t isrSubTimerC[4] = {{NULL, NULL}};
  volatile uint32_t ui32ActivedTIMERs = 0;
  #define   usrGetTIMERNumber(usrGroup)                       usrGroup == TIMER_0 ? 0 :\
                                                              usrGroup == TIMER_1 ? 1 :\
                                                              usrGroup == TIMER_2 ? 2 :\
                                                              usrGroup == TIMER_3 ? 3 :\
                                                              usrGroup == TIMER_4 ? 4 : 5
#else
  hal_isr_t isrMasterTimer[3] = {{NULL, NULL}};
  hal_isr_t isrSubTimerA[3] = {{NULL, NULL}};
  hal_isr_t isrSubTimerB = {NULL, NULL};
  volatile uint8_t ui8ActivedTIMERs = 0;
  #define   usrGetTIMERNumber(usrGroup)                       usrGroup == TIMER_0 ? 0 :\
                                                              usrGroup == TIMER_1 ? 1 : 2
#endif

//! Function: TIMER Initializer
/*!
  Initializes a TIMER.
  \param ui8pTIMERGroup is a volatile 8-bit pointer integer. It's the TIMER group (TIMER_X).
*/
void vTIMERInit(volatile uint8_t* ui8pTIMERGroup){
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

//! Function: TIMER Enabler
/*!
  Enables a TIMER.
  \param ui8pTIMERGroup is a volatile 8-bit pointer integer. It's the TIMER group (TIMER_X).
  \param ui8SubTimer is a 8-bit integer. It's the sub-timer.
*/
void vEnableTIMERInterrupt(volatile uint8_t* ui8pTIMERGroup, uint8_t ui8SubTimer){
  volatile uint8_t ui8TIMERNumber = usrGetTIMERNumber(ui8pTIMERGroup);
  if (ui8SubTimer == MASTER_TIMER){
    if (ui8pTIMERGroup == TIMER_0 || ui8pTIMERGroup == TIMER_2){
      vSetBit(*(regTIMSK(ui8pTIMERGroup)), OCIEnA);
    }
    else {
      vSetBit(*(regTIMSK(ui8pTIMERGroup)), ICIEn);
    }
    #if defined(ARDUINO_AVR_MEGA2560) || defined(ARDUINO_AVR_MEGA)
      vSetBit(ui32ActivedTIMERs, ui8TIMERNumber + 14);
    #else
      vSetBit(ui8ActivedTIMERs, ui8TIMERNumber + 4);
    #endif
  }
  else{
    if(ui8SubTimer == SUBTIMER_A){
      if (ui8pTIMERGroup == TIMER_0 || ui8pTIMERGroup == TIMER_2){
        vSetBit(*(regTIMSK(ui8pTIMERGroup)), OCIEnB);
      }
      else{
        vSetBit(*(regTIMSK(ui8pTIMERGroup)), OCIEnA);
      }
      #if defined(ARDUINO_AVR_MEGA2560) || defined(ARDUINO_AVR_MEGA)
        vSetBit(ui32ActivedTIMERs, ui8TIMERNumber + 8);
      #else
        vSetBit(ui8ActivedTIMERs, ui8TIMERNumber + 1);
      #endif
    }
    else if(ui8SubTimer == SUBTIMER_B && ui8pTIMERGroup != TIMER_0 && ui8pTIMERGroup != TIMER_2){
      vSetBit(*(regTIMSK(ui8pTIMERGroup)), OCIEnB);
      #if defined(ARDUINO_AVR_MEGA2560) || defined(ARDUINO_AVR_MEGA)
        if (ui8TIMERNumber == 1){
          vSetBit(ui32ActivedTIMERs, ui8TIMERNumber + 3);
        }
        else{
          vSetBit(ui32ActivedTIMERs, ui8TIMERNumber + 2);
        }
      #else
        vSetBit(ui8ActivedTIMERs, 0);
      #endif
    }
    #if defined(ARDUINO_AVR_MEGA2560) || defined(ARDUINO_AVR_MEGA)
      else if(ui8SubTimer == SUBTIMER_C && ui8pTIMERGroup != TIMER_0 && ui8pTIMERGroup != TIMER_2){
        vSetBit(*(regTIMSK(ui8pTIMERGroup)), OCIEnC);
        if (ui8TIMERNumber == 1){
          vSetBit(ui32ActivedTIMERs, uiModule(ui8TIMERNumber - 1));
        }
        else{
          vSetBit(ui32ActivedTIMERs, uiModule(ui8TIMERNumber - 2));
        }
      }
    #endif
  }
}

//! Function: TIMER Disabler
/*!
  Disables a TIMER.
  \param ui8pTIMERGroup is a volatile 8-bit pointer integer. It's the TIMER group (TIMER_X).
  \param ui8SubTimer is a 8-bit integer. It's the sub-timer.
*/
void vDisableTIMERInterrupt(volatile uint8_t* ui8pTIMERGroup, uint8_t ui8SubTimer){
  volatile uint8_t ui8TIMERNumber = usrGetTIMERNumber(ui8pTIMERGroup);
  if (ui8SubTimer == MASTER_TIMER){
    if (ui8pTIMERGroup == TIMER_0 || ui8pTIMERGroup == TIMER_2){
      vEraseBit(*(regTIMSK(ui8pTIMERGroup)), OCIEnA);
    }
    else {
      vEraseBit(*(regTIMSK(ui8pTIMERGroup)), ICIEn);
    }
    #if defined(ARDUINO_AVR_MEGA2560) || defined(ARDUINO_AVR_MEGA)
      vEraseBit(ui32ActivedTIMERs, ui8TIMERNumber + 14);
    #else
      vEraseBit(ui8ActivedTIMERs, ui8TIMERNumber + 4);
    #endif
  }
  else{
    if(ui8SubTimer == SUBTIMER_A){
      if (ui8pTIMERGroup == TIMER_0 || ui8pTIMERGroup == TIMER_2){
        vEraseBit(*(regTIMSK(ui8pTIMERGroup)), OCIEnB);
      }
      else{
        vEraseBit(*(regTIMSK(ui8pTIMERGroup)), OCIEnA);
      }
      #if defined(ARDUINO_AVR_MEGA2560) || defined(ARDUINO_AVR_MEGA)
        vEraseBit(ui32ActivedTIMERs, ui8TIMERNumber + 8);
      #else
        vEraseBit(ui8ActivedTIMERs, ui8TIMERNumber + 1);
      #endif
    }
    else if(ui8SubTimer == SUBTIMER_B && ui8pTIMERGroup != TIMER_0 && ui8pTIMERGroup != TIMER_2){
      vEraseBit(*(regTIMSK(ui8pTIMERGroup)), OCIEnB);
      #if defined(ARDUINO_AVR_MEGA2560) || defined(ARDUINO_AVR_MEGA)
        if (ui8TIMERNumber == 1){
          vEraseBit(ui32ActivedTIMERs, ui8TIMERNumber + 3);
        }
        else{
          vEraseBit(ui32ActivedTIMERs, ui8TIMERNumber + 2);
        }
      #else
        vEraseBit(ui8ActivedTIMERs, 0);
      #endif
    }
    #if defined(ARDUINO_AVR_MEGA2560) || defined(ARDUINO_AVR_MEGA)
      else if(ui8SubTimer == SUBTIMER_C && ui8pTIMERGroup != TIMER_0 && ui8pTIMERGroup != TIMER_2){
        vEraseBit(*(regTIMSK(ui8pTIMERGroup)), OCIEnC);
        if (ui8TIMERNumber == 1){
          vEraseBit(ui32ActivedTIMERs, uiModule(ui8TIMERNumber - 1));
        }
        else{
          vEraseBit(ui32ActivedTIMERs, uiModule(ui8TIMERNumber - 2));
        }
      }
    #endif
  }
}

//! Function: TIMER Prescaler Seter
/*!
  Sets a prescaler TIMER.
  \param ui8pTIMERGroup is a volatile 8-bit pointer integer. It's the TIMER group (TIMER_X).
  \param ui8SubTimer is a 8-bit integer. It's the prescaler timer.
*/
void vSetTIMERPrescaler(volatile uint8_t* ui8pTIMERGroup, uint8_t ui8TIMERPrescaler){
  if (ui8pTIMERGroup != TIMER_2){
    if (ui8TIMERPrescaler > TIMER_PS_8 && ui8TIMERPrescaler <= TIMER_PS_64){
      ui8TIMERPrescaler--;
    }
    else if(ui8TIMERPrescaler > TIMER_PS_64){
      ui8TIMERPrescaler -= 2;
    }
  }
  vCopyBits(*regTCCRB(ui8pTIMERGroup), CSn0, ui8TIMERPrescaler, 0, 3);
}

//! Function: TIMER Counter Seter
/*!
  Sets a counter TIMER.
  \param ui8pTIMERGroup is a volatile 8-bit pointer integer. It's the TIMER group (TIMER_X).
  \param ui16TIMERLimit is a 16-bit integer. It's the counter timer.
*/
void vSetTIMERCounterLimit(volatile uint8_t* ui8pTIMERGroup, uint16_t ui16TIMERLimit){
  if ((ui8pTIMERGroup == TIMER_0 || ui8pTIMERGroup == TIMER_2) && ui16TIMERLimit < 256){
    if (ui8ReadBit(*regTCCRA(ui8pTIMERGroup), WGMn0) == 0){
      *(regOCRAL(ui8pTIMERGroup)) = ui16TIMERLimit & 255;
      *(regOCRBL(ui8pTIMERGroup)) = (ui16TIMERLimit >> 1) & 255;
    }
    else{
      if (ui8ReadBit(*regTCCRA(ui8pTIMERGroup), WGMn1) == 1){
        *(regOCRAL(ui8pTIMERGroup)) = ui16TIMERLimit & 255;
      }
    }
  }
  else{
    if (ui8ReadBit(*regTCCRB(ui8pTIMERGroup), WGMn2) == 1){
      *(regICRH(ui8pTIMERGroup)) = (ui16TIMERLimit >> 8) & 255;
      *(regICRL(ui8pTIMERGroup)) = (ui16TIMERLimit) & 255;
      *(regOCRAH(ui8pTIMERGroup)) = (ui16TIMERLimit >> 10) & 255;
      *(regOCRAL(ui8pTIMERGroup)) = (ui16TIMERLimit >> 2) & 255;
      *(regOCRBH(ui8pTIMERGroup)) = (ui16TIMERLimit >> 9) & 255;
      *(regOCRBL(ui8pTIMERGroup)) = (ui16TIMERLimit >> 1) & 255;
      #if defined(ARDUINO_AVR_MEGA2560) || defined(ARDUINO_AVR_MEGA)
        *(regOCRCH(ui8pTIMERGroup)) =  (((ui16TIMERLimit >> 1) + (ui16TIMERLimit >> 2)) >> 8) & 255;
        *(regOCRCL(ui8pTIMERGroup)) =  ((ui16TIMERLimit >> 1) + (ui16TIMERLimit >> 2)) & 255;
      #endif
    }
    else{
      if (ui8ReadBit(*regTCCRA(ui8pTIMERGroup), WGMn1) == 1){
        *(regICRH(ui8pTIMERGroup)) = (ui16TIMERLimit >> 8) & 255;
        *(regICRL(ui8pTIMERGroup)) = (ui16TIMERLimit) & 255;
      }
    }
  }
}

//! Function: TIMER Period (ms) Seter
/*!
  Sets TIMER period (in ms).
  \param ui8pTIMERGroup is a volatile 8-bit pointer integer. It's the TIMER group (TIMER_X).
  \param ui16PeriodMS is a 16-bit integer. It's the period value, in miliseconds.
*/
void vSetTIMERPeriodMS(volatile uint8_t* ui8pTIMERGroup, uint16_t ui16PeriodMS){
  if (((ui8pTIMERGroup == TIMER_2 || ui8pTIMERGroup == TIMER_0) && ui16PeriodMS < 17) || ((ui8pTIMERGroup != TIMER_2 && ui8pTIMERGroup != TIMER_0) && ui16PeriodMS < 4194)){
    vSetTIMERPrescaler(ui8pTIMERGroup, TIMER_PS_1024);
    vSetTIMERCounterLimit(ui8pTIMERGroup, ui16PeriodMS * (DEVICE_CLOCK_HZ >> 10)/1000 - 1);
  }
}

//! Function: TIMER Period (us) Seter
/*!
  Sets TIMER period (in us).
  \param ui8pTIMERGroup is a volatile 8-bit pointer integer. It's the TIMER group (TIMER_X).
  \param ui32PeriodUS is a 32-bit integer. It's the period value, in microseconds.
*/
void vSetTIMERPeriodUS(volatile uint8_t* ui8pTIMERGroup, uint32_t ui32PeriodUS){
  uint8_t ui8PrescalerCounter = 0;
  uint8_t ui8Prescaler[7] = {0, 3, 5, 6, 7, 8, 10};
  uint32_t ui32CounterLimit = 0;
  for (ui8PrescalerCounter = 0 ; ui8Prescaler[ui8PrescalerCounter] < 7 ; ui8PrescalerCounter++){
    if (ui8pTIMERGroup != TIMER_2 && (ui8Prescaler[ui8PrescalerCounter] == 5 || ui8Prescaler[ui8PrescalerCounter] == 7)){
      ui8PrescalerCounter++;
    }
    ui32CounterLimit = ((float) (DEVICE_CLOCK_HZ >> ui8Prescaler[ui8PrescalerCounter])/1000000) * (ui32PeriodUS) - 1;
    if (ui32CounterLimit < 65535 && ui32CounterLimit > 10){
      vSetTIMERPrescaler(ui8pTIMERGroup, ui8PrescalerCounter + 1);
      vSetTIMERCounterLimit(ui8pTIMERGroup, (uint16_t) ui32CounterLimit);
      break;
    }
  }
}

//! Function: TIMER Interrupt Attacher
/*!
  Attaches a TIMER interruption function.
  \param ui8pTIMERGroup is a volatile 8-bit pointer integer. It's the TIMER group (TIMER_X).
  \param ui8SubTimer is a 8-bit integer. It's the prescaler timer.
  \param vInterruptFunction is a function pointer. It's the callback interruption.
  \param vpArgument is a void pointer. It's the callback argument.
*/
void vAttachTIMERInterrupt(volatile uint8_t* ui8pTIMERGroup, uint8_t ui8SubTimer, void (*vInterruptFunction)(void*), void* vpArgument){
  uint8_t ui8TIMERNumber = usrGetTIMERNumber(ui8pTIMERGroup);
  switch(ui8SubTimer){

    case MASTER_TIMER:
    {
      isrMasterTimer[ui8TIMERNumber].vInterruptFunction = vInterruptFunction;
      isrMasterTimer[ui8TIMERNumber].vpArgument = vpArgument;
      break;
    }

    case SUBTIMER_A:
    {
      isrSubTimerA[ui8TIMERNumber].vInterruptFunction = vInterruptFunction;
      isrSubTimerA[ui8TIMERNumber].vpArgument = vpArgument;
      break;
    }

    case SUBTIMER_B:
    {
      #if defined(ARDUINO_AVR_MEGA2560) || defined(ARDUINO_AVR_MEGA)
        isrSubTimerB[ui8TIMERNumber].vInterruptFunction = vInterruptFunction;
        isrSubTimerB[ui8TIMERNumber].vpArgument = vpArgument;
      #else
        isrSubTimerB.vInterruptFunction = vInterruptFunction;
        isrSubTimerB.vpArgument = vpArgument;
      #endif
      break;
    }

    #if defined(ARDUINO_AVR_MEGA2560) || defined(ARDUINO_AVR_MEGA)
      case SUBTIMER_C:
      {
        isrSubTimerC[ui8TIMERNumber].vInterruptFunction = vInterruptFunction;
        isrSubTimerC[ui8TIMERNumber].vpArgument = vpArgument;
        break;
      }
    #endif

  }
}

//! Function: TIMER Interrupt Detacher
/*!
  Detaches a TIMER interruption function.
  \param ui8pTIMERGroup is a volatile 8-bit pointer integer. It's the TIMER group (TIMER_X).
  \param ui8SubTimer is a 8-bit integer. It's the prescaler timer.
*/
void vDetachTIMERInterrupt(volatile uint8_t* ui8pTIMERGroup, uint8_t ui8SubTimer){
  vAttachTIMERInterrupt(ui8pTIMERGroup, ui8SubTimer, NULL, NULL);
}

//! Function: TIMER Interrupt Ignorer
/*!
  Ignores a TIMER interruption flag.
  \param ui8pTIMERGroup is a volatile 8-bit pointer integer. It's the TIMER group (TIMER_X).
  \param ui8SubTimer is a 8-bit integer. It's the prescaler timer.
*/
void vIgnoreTIMERRequest(volatile uint8_t* ui8pTIMERGroup, uint8_t ui8SubTimer){
  if (ui8pTIMERGroup == TIMER_0 || ui8pTIMERGroup == TIMER_2){
    if (ui8SubTimer == MASTER_TIMER){
      vSetBit(*(regTIFR(ui8pTIMERGroup)), OCFnA);
    }
    else{
      vSetBit(*(regTIFR(ui8pTIMERGroup)), OCFnB);
    }
  }
  else{
    switch (ui8SubTimer) {
      case MASTER_TIMER:
      {
        vSetBit(*(regTIFR(ui8pTIMERGroup)), ICFn);
      }
      break;

      case SUBTIMER_A:
      {
        vSetBit(*(regTIFR(ui8pTIMERGroup)), OCFnA);
      }
      break;

      case SUBTIMER_B:
      {
        vSetBit(*(regTIFR(ui8pTIMERGroup)), OCFnB);
      }
      break;

      case SUBTIMER_C:
      {
        vSetBit(*(regTIFR(ui8pTIMERGroup)), OCFnC);
      }
      break;

    }
  }
}

//! Function: TIMER Interrupt Forcer
/*!
  Forces a TIMER interruption.
  \param ui8pTIMERGroup is a volatile 8-bit pointer integer. It's the TIMER group (TIMER_X).
  \param ui8SubTimer is a 8-bit integer. It's the prescaler timer.
*/
void vForceTIMERInterrupt(volatile uint8_t* ui8pTIMERGroup, uint8_t ui8SubTimer){
  if (ui8pTIMERGroup == TIMER_0 || ui8pTIMERGroup == TIMER_2){
    if (ui8SubTimer == MASTER_TIMER){
      vEraseBit(*(regTIFR(ui8pTIMERGroup)), OCFnA);
    }
    else{
      vEraseBit(*(regTIFR(ui8pTIMERGroup)), OCFnB);
    }
  }
  else{
    switch (ui8SubTimer) {
      case MASTER_TIMER:
      {
        vEraseBit(*(regTIFR(ui8pTIMERGroup)), ICFn);
      }
      break;

      case SUBTIMER_A:
      {
        vEraseBit(*(regTIFR(ui8pTIMERGroup)), OCFnA);
      }
      break;

      case SUBTIMER_B:
      {
        vEraseBit(*(regTIFR(ui8pTIMERGroup)), OCFnB);
      }
      break;

      case SUBTIMER_C:
      {
        vEraseBit(*(regTIFR(ui8pTIMERGroup)), OCFnC);
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
  if (isrMasterTimer[0].vInterruptFunction != NULL){
    vEraseBit(*(regTIMSK(TIMER_0)), OCIEnA);
    isrMasterTimer[0].vInterruptFunction(isrMasterTimer[0].vpArgument);
    #if defined(ARDUINO_AVR_MEGA2560) || defined(ARDUINO_AVR_MEGA)
      if (ui8ReadBit(ui32ActivedTIMERs, 14) == 1){
    #else
      if (ui8ReadBit(ui8ActivedTIMERs, 4) == 1){
    #endif
      vSetBit(*(regTIMSK(TIMER_0)), OCIEnA);
    }
  }
}

ISR(TIMER0_COMPB_vect){
  if (isrSubTimerA[0].vInterruptFunction != NULL){
    vEraseBit(*(regTIMSK(TIMER_0)), OCIEnA);
    isrSubTimerA[0].vInterruptFunction(isrSubTimerA[0].vpArgument);
    #if defined(ARDUINO_AVR_MEGA2560) || defined(ARDUINO_AVR_MEGA)
      if (ui8ReadBit(ui32ActivedTIMERs, 8) == 1){
    #else
      if (ui8ReadBit(ui8ActivedTIMERs, 1) == 1){
    #endif
      vSetBit(*(regTIMSK(TIMER_0)), OCIEnA);
    }
  }
}

ISR(TIMER1_CAPT_vect){
  if (isrMasterTimer[1].vInterruptFunction != NULL){
    vEraseBit(*(regTIMSK(TIMER_1)), ICIEn);
    isrMasterTimer[1].vInterruptFunction(isrMasterTimer[1].vpArgument);
    #if defined(ARDUINO_AVR_MEGA2560) || defined(ARDUINO_AVR_MEGA)
      if (ui8ReadBit(ui32ActivedTIMERs, 15) == 1){
    #else
      if (ui8ReadBit(ui8ActivedTIMERs, 5) == 1){
    #endif
      vSetBit(*(regTIMSK(TIMER_1)), ICIEn);
    }
  }
}

ISR(TIMER1_COMPA_vect){
  if (isrSubTimerA[1].vInterruptFunction != NULL){
    vEraseBit(*(regTIMSK(TIMER_1)), OCIEnA);
    isrSubTimerA[1].vInterruptFunction(isrSubTimerA[1].vpArgument);
    #if defined(ARDUINO_AVR_MEGA2560) || defined(ARDUINO_AVR_MEGA)
      if (ui8ReadBit(ui32ActivedTIMERs, 9) == 1){
    #else
      if (ui8ReadBit(ui8ActivedTIMERs, 2) == 1){
    #endif
      vSetBit(*(regTIMSK(TIMER_1)), OCIEnA);
    }
  }
}

ISR(TIMER1_COMPB_vect){
  #if defined(ARDUINO_AVR_MEGA2560) || defined(ARDUINO_AVR_MEGA)
    if (isrSubTimerB[1].vInterruptFunction != NULL){
      vEraseBit(*(regTIMSK(TIMER_1)), OCIEnB);
      isrSubTimerB[1].vInterruptFunction(isrSubTimerB[1].vpArgument);
      if (ui8ReadBit(ui32ActivedTIMERs, 4) == 1){
        vSetBit(*(regTIMSK(TIMER_1)), OCIEnB);
      }
    }
  #else
    if (isrSubTimerB.vInterruptFunction != NULL){
      vEraseBit(*(regTIMSK(TIMER_1)), OCIEnB);
      isrSubTimerB.vInterruptFunction(isrSubTimerB.vpArgument);
      if (ui8ReadBit(ui8ActivedTIMERs, 0) == 1){
        vSetBit(*(regTIMSK(TIMER_1)), OCIEnB);
      }
    }
  #endif
}

ISR(TIMER2_COMPA_vect){
  if (isrMasterTimer[2].vInterruptFunction != NULL){
    vEraseBit(*(regTIMSK(TIMER_2)), OCIEnA);
    isrMasterTimer[2].vInterruptFunction(isrMasterTimer[2].vpArgument);
    #if defined(ARDUINO_AVR_MEGA2560) || defined(ARDUINO_AVR_MEGA)
      if (ui8ReadBit(ui32ActivedTIMERs, 16) == 1){
    #else
      if (ui8ReadBit(ui8ActivedTIMERs, 6) == 1){
    #endif
      vSetBit(*(regTIMSK(TIMER_2)), OCIEnA);
    }
  }
}

ISR(TIMER2_COMPB_vect){
  if (isrSubTimerA[2].vInterruptFunction != NULL){
    vEraseBit(*(regTIMSK(TIMER_2)), OCIEnA);
    isrSubTimerA[2].vInterruptFunction(isrSubTimerA[2].vpArgument);
    #if defined(ARDUINO_AVR_MEGA2560) || defined(ARDUINO_AVR_MEGA)
      if (ui8ReadBit(ui32ActivedTIMERs, 10) == 1){
    #else
      if (ui8ReadBit(ui8ActivedTIMERs, 3) == 1){
    #endif
      vSetBit(*(regTIMSK(TIMER_2)), OCIEnA);
    }
  }
}

#if defined(ARDUINO_AVR_MEGA2560) || defined(ARDUINO_AVR_MEGA)
  ISR(TIMER1_COMPC_vect){
    if (isrSubTimerC[1].vInterruptFunction != NULL){
      vEraseBit(*(regTIMSK(TIMER_1)), OCIEnC);
      isrSubTimerC[1].vInterruptFunction(isrSubTimerC[1].vpArgument);
      if (ui8ReadBit(ui32ActivedTIMERs, 0) == 1){
        vSetBit(*(regTIMSK(TIMER_1)), OCIEnC);
      }
    }
  }

  ISR(TIMER3_CAPT_vect){
    if (isrMasterTimer[3].vInterruptFunction != NULL){
      vEraseBit(*(regTIMSK(TIMER_3)), ICIEn);
      isrMasterTimer[3].vInterruptFunction(isrMasterTimer[3].vpArgument);
      if (ui8ReadBit(ui32ActivedTIMERs, 17) == 1){
        vSetBit(*(regTIMSK(TIMER_3)), ICIEn);
      }
    }
  }

  ISR(TIMER3_COMPA_vect){
    if (isrSubTimerA[3].vInterruptFunction != NULL){
      vEraseBit(*(regTIMSK(TIMER_3)), OCIEnA);
      isrSubTimerA[3].vInterruptFunction(isrSubTimerA[3].vpArgument);
      if (ui8ReadBit(ui32ActivedTIMERs, 11) == 1){
        vSetBit(*(regTIMSK(TIMER_3)), OCIEnA);
      }
    }
  }

  ISR(TIMER3_COMPB_vect){
    if (isrSubTimerB[3].vInterruptFunction != NULL){
      vEraseBit(*(regTIMSK(TIMER_3)), OCIEnB);
      isrSubTimerB[3].vInterruptFunction(isrSubTimerB[3].vpArgument);
      if (ui8ReadBit(ui32ActivedTIMERs, 5) == 1){
        vSetBit(*(regTIMSK(TIMER_3)), OCIEnB);
      }
    }
  }

  ISR(TIMER3_COMPC_vect){
    if (isrSubTimerC[3].vInterruptFunction != NULL){
      vEraseBit(*(regTIMSK(TIMER_3)), OCIEnC);
      isrSubTimerC[3].vInterruptFunction(isrSubTimerC[3].vpArgument);
      if (ui8ReadBit(ui32ActivedTIMERs, 1) == 1){
        vSetBit(*(regTIMSK(TIMER_3)), OCIEnC);
      }
    }
  }

  ISR(TIMER4_CAPT_vect){
    if (isrMasterTimer[4].vInterruptFunction != NULL){
      vEraseBit(*(regTIMSK(TIMER_4)), ICIEn);
      isrMasterTimer[4].vInterruptFunction(isrMasterTimer[4].vpArgument);
      if (ui8ReadBit(ui32ActivedTIMERs, 18) == 1){
        vSetBit(*(regTIMSK(TIMER_4)), ICIEn);
      }
    }
  }

  ISR(TIMER4_COMPA_vect){
    if (isrSubTimerA[4].vInterruptFunction != NULL){
      vEraseBit(*(regTIMSK(TIMER_4)), OCIEnA);
      isrSubTimerA[4].vInterruptFunction(isrSubTimerA[4].vpArgument);
      if (ui8ReadBit(ui32ActivedTIMERs, 12) == 1){
        vSetBit(*(regTIMSK(TIMER_4)), OCIEnA);
      }
    }
  }

  ISR(TIMER4_COMPB_vect){
    if (isrSubTimerB[4].vInterruptFunction != NULL){
      vEraseBit(*(regTIMSK(TIMER_4)), OCIEnB);
      isrSubTimerB[4].vInterruptFunction(isrSubTimerB[4].vpArgument);
      if (ui8ReadBit(ui32ActivedTIMERs, 6) == 1){
        vSetBit(*(regTIMSK(TIMER_4)), OCIEnB);
      }
    }
  }

  ISR(TIMER4_COMPC_vect){
    if (isrSubTimerC[4].vInterruptFunction != NULL){
      vEraseBit(*(regTIMSK(TIMER_4)), OCIEnC);
      isrSubTimerC[4].vInterruptFunction(isrSubTimerC[4].vpArgument);
      if (ui8ReadBit(ui32ActivedTIMERs, 2) == 1){
        vSetBit(*(regTIMSK(TIMER_4)), OCIEnC);
      }
    }
  }

  ISR(TIMER5_CAPT_vect){
    if (isrMasterTimer[5].vInterruptFunction != NULL){
      vEraseBit(*(regTIMSK(TIMER_5)), ICIEn);
      isrMasterTimer[5].vInterruptFunction(isrMasterTimer[5].vpArgument);
      if (ui8ReadBit(ui32ActivedTIMERs, 19) == 1){
        vSetBit(*(regTIMSK(TIMER_5)), ICIEn);
      }
    }
  }

  ISR(TIMER5_COMPA_vect){
    if (isrSubTimerA[5].vInterruptFunction != NULL){
      vEraseBit(*(regTIMSK(TIMER_5)), OCIEnA);
      isrSubTimerA[5].vInterruptFunction(isrSubTimerA[5].vpArgument);
      if (ui8ReadBit(ui32ActivedTIMERs, 13) == 1){
        vSetBit(*(regTIMSK(TIMER_5)), OCIEnA);
      }
    }
  }

  ISR(TIMER5_COMPB_vect){
    if (isrSubTimerB[5].vInterruptFunction != NULL){
      vEraseBit(*(regTIMSK(TIMER_5)), OCIEnB);
      isrSubTimerB[5].vInterruptFunction(isrSubTimerB[5].vpArgument);
      if (ui8ReadBit(ui32ActivedTIMERs, 7) == 1){
        vSetBit(*(regTIMSK(TIMER_5)), OCIEnB);
      }
    }
  }

  ISR(TIMER5_COMPC_vect){
    if (isrSubTimerC[5].vInterruptFunction != NULL){
      vEraseBit(*(regTIMSK(TIMER_5)), OCIEnC);
      isrSubTimerC[5].vInterruptFunction(isrSubTimerC[5].vpArgument);
      if (ui8ReadBit(ui32ActivedTIMERs, 3) == 1){
        vSetBit(*(regTIMSK(TIMER_5)), OCIEnC);
      }
    }
  }
#endif
