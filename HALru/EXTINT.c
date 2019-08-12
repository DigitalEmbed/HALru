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

uint8_t ui8EnabledEXTINT = 0;

//! Function: EXTINT Interrupt Enabler
/*!
  Enable a EXTINT interruption.
  \param ui8InterruptPin is a 8-bit pointer integer. It's the EXTINT pin (EXTINT_X).
*/
void vEnableEXTINT(uint8_t ui8InterruptPin){
  vSetBit(EIMSK, ui8InterruptPin);
  EIMSK = ui8EnabledEXTINT;
}

//! Function: EXTINT Interrupt Disnabler
/*!
  Disable a EXTINT interruption.
  \param ui8InterruptPin is a 8-bit pointer integer. It's the EXTINT pin (EXTINT_X).
*/
void vDisableEXTINT(uint8_t ui8InterruptPin){
  vEraseBit(EIMSK, ui8InterruptPin);
  EIMSK = ui8EnabledEXTINT;
}

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
    vEraseBit(EIMSK, 0);
    isrExtInt[0](argExtInt[0]);
    if (ui8ReadBit(ui8EnabledEXTINT, 0) == 1){
      vSetBit(EIMSK, 0);
    }
  }
}

ISR(INT1_vect){
  if (isrExtInt[1] != NULL){
    vEraseBit(EIMSK, 1);
    isrExtInt[1](argExtInt[1]);
    if (ui8ReadBit(ui8EnabledEXTINT, 1) == 1){
      vSetBit(EIMSK, 1);
    }
  }
}

#if defined(ARDUINO_AVR_MEGA2560) || defined(ARDUINO_AVR_MEGA)
  ISR(INT2_vect){
    if (isrExtInt[2] != NULL){
      vEraseBit(EIMSK, 2);
      isrExtInt[2](argExtInt[2]);
      if (ui8ReadBit(ui8EnabledEXTINT ,2) == 1){
        vSetBit(EIMSK, 2);
      }
    }
  }

  ISR(INT3_vect){
    if (isrExtInt[3] != NULL){
      vEraseBit(EIMSK, 3);
      isrExtInt[3](argExtInt[3]);
      if (ui8ReadBit(ui8EnabledEXTINT, 3) == 1){
        vSetBit(EIMSK, 3);
      }
    }
  }

  ISR(INT4_vect){
    if (isrExtInt[4] != NULL){
      vEraseBit(EIMSK, 4);
      isrExtInt[4](argExtInt[4]);
      if (ui8ReadBit(ui8EnabledEXTINT, 4) == 1){
        vSetBit(EIMSK, 4);
      }
    }
  }

  ISR(INT5_vect){
    if (isrExtInt[5] != NULL){
      vEraseBit(EIMSK, 5);
      isrExtInt[5](argExtInt[5]);
      if (ui8ReadBit(ui8EnabledEXTINT, 5) == 1){
        vSetBit(EIMSK, 5);
      }
    }
  }

  ISR(INT6_vect){
    if (isrExtInt[6] != NULL){
      vEraseBit(EIMSK, 6);
      isrExtInt[6](argExtInt[6]);
      if (ui8ReadBit(ui8EnabledEXTINT, 6) == 1){
        vSetBit(EIMSK, 6);
      }
    }
  }

  ISR(INT7_vect){
    if (isrExtInt[7] != NULL){
      vEraseBit(EIMSK, 7);
      isrExtInt[7](argExtInt[7]);
      if (ui8ReadBit(ui8EnabledEXTINT, 7) == 1){
        vSetBit(EIMSK, 7);
      }
    }
  }
#endif
