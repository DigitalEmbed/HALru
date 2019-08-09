#include "EXTINT.h"

//! Declarations: Private EXTINT Declarations
/*!
  This macros and variables are for facilitate the use of this library.
*/
#if defined(ARDUINO_AVR_MEGA2560) || defined(ARDUINO_AVR_MEGA)
  isr_timer_t isrExtInt[7] = {NULL};
  args_timer_t argExtInt[7] = {NULL};
#else
  isr_timer_t isrExtInt[2] = {NULL};
  args_timer_t argExtInt[2] = {NULL};
#endif

//! Function: EXTINT Interrupt Attacher
/*!
  Attach a EXTINT interruption function.
  \param ui8InterruptPin is a 8-bit pointer integer. It's the EXTINT pin (EXTINT_X).
  \param ui8SubTimer is a 8-bit integer. It's the prescaler timer.
  \param vInterruptFunction is a function pointer. It's the callback interruption.
  \param vpArgument is a void pointer. It's the callback argument.
*/
void vAttachEXTINTInterrupt(uint8_t ui8InterruptPin, isr_extint_t vInterruptFunction, void* vpArgument){
  isrExtInt[ui8InterruptPin] = vInterruptFunction;
  argExtInt[ui8InterruptPin] = vpArgument;
}

//! Function: EXTINT Interrupt Dettacher
/*!
  Dettach a EXTINT interruption function.
  \param ui8InterruptPin is a 8-bit pointer integer. It's the EXTINT pin (EXTINT_X).
*/
void vDettachEXTINTInterrupt(uint8_t ui8InterruptPin){
  vAttachEXTINTInterrupt(ui8InterruptPin, NULL, NULL);
}

ISR(INT0_vect){
  if (isrExtInt[0] != NULL){
    vDisableEXTINT(EXTINT_0);
    isrExtInt[0](argExtInt[0]);
    vEnableEXTINT(EXTINT_0);
  }
}

ISR(INT1_vect){
  if (isrExtInt[1] != NULL){
    vDisableEXTINT(EXTINT_1);
    isrExtInt[1](argExtInt[1]);
    vEnableEXTINT(EXTINT_1);
  }
}

#if defined(ARDUINO_AVR_MEGA2560) || defined(ARDUINO_AVR_MEGA)
  ISR(INT2_vect){
    if (isrExtInt[2] != NULL){
      vDisableEXTINT(EXTINT_2);
      isrExtInt[2](argExtInt[2]);
      vEnableEXTINT(EXTINT_2);
    }
  }

  ISR(INT3_vect){
    if (isrExtInt[3] != NULL){
      vDisableEXTINT(EXTINT_3);
      isrExtInt[3](argExtInt[3]);
      vEnableEXTINT(EXTINT_3);
    }
  }

  ISR(INT4_vect){
    if (isrExtInt[4] != NULL){
      vDisableEXTINT(EXTINT_4);
      isrExtInt[4](argExtInt[4]);
      vEnableEXTINT(EXTINT_4);
    }
  }

  ISR(INT5_vect){
    if (isrExtInt[5] != NULL){
      vDisableEXTINT(EXTINT_5);
      isrExtInt[5](argExtInt[5]);
      vEnableEXTINT(EXTINT_5);
    }
  }

  ISR(INT6_vect){
    if (isrExtInt[6] != NULL){
      vDisableEXTINT(EXTINT_6);
      isrExtInt[6](argExtInt[6]);
      vEnableEXTINT(EXTINT_6);
    }
  }

  ISR(INT7_vect){
    if (isrExtInt[7] != NULL){
      vDisableEXTINT(EXTINT_7);
      isrExtInt[7](argExtInt[7]);
      vEnableEXTINT(EXTINT_7);
    }
  }
#endif
