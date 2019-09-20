#include "EXTINT.h"
#include <stdio.h>
#include <avr/interrupt.h>
#include "./Configs.h"
#include "./Interrupts.h"

//! Declarations: Private EXTINT Declarations
/*!
  This macros and variables are for facilitate the use of this library.
*/
#if defined(ARDUINO_AVR_MEGA2560) || defined(ARDUINO_AVR_MEGA)
  volatile hal_isr_t isrEXTINTArray[7] = {{NULL, NULL}};
#else
  volatile hal_isr_t isrEXTINTArray[2] = {{NULL, NULL}};
#endif

uint8_t ui8EnabledEXTINT = 0;

//! Function: EXTINT Interrupt Enabler
/*!
  Enable a EXTINT interruption.
  \param ui8InterruptPin is a 8-bit pointer integer. It's the EXTINT pin (EXTINT_X).
*/
void vEnableEXTINTInterrupt(uint8_t ui8InterruptPin){
  vSetBit(EIMSK, ui8InterruptPin);
  ui8EnabledEXTINT = EIMSK;
}

//! Function: EXTINT Interrupt Disnabler
/*!
  Disable a EXTINT interruption.
  \param ui8InterruptPin is a 8-bit pointer integer. It's the EXTINT pin (EXTINT_X).
*/
void vDisableEXTINTInterrupt(uint8_t ui8InterruptPin){
  vEraseBit(EIMSK, ui8InterruptPin);
  ui8EnabledEXTINT = EIMSK;
}

//! Function: EXTINT Interrupt Attacher
/*!
  Attach a EXTINT interruption function.
  \param ui8InterruptPin is a 8-bit pointer integer. It's the EXTINT pin (EXTINT_X).
  \param vInterruptFunction is a function pointer. It's the callback interruption.
  \param vpArgument is a void pointer. It's the callback argument.
*/
void vAttachEXTINTInterrupt(uint8_t ui8InterruptPin, isr_extint_t vInterruptFunction, void* vpArgument){
  isrEXTINTArray[ui8InterruptPin].vInterruptFunction = vInterruptFunction;
  isrEXTINTArray[ui8InterruptPin].vpArgument = vpArgument;
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
  if (isrEXTINTArray[0].vInterruptFunction != NULL){
    vEraseBit(EIMSK, 0);
    isrEXTINTArray[0].vInterruptFunction(isrEXTINTArray[0].vpArgument);
    if (ui8ReadBit(ui8EnabledEXTINT, 0) == 1){
      vSetBit(EIMSK, 0);
    }
  }
}

ISR(INT1_vect){
  if (isrEXTINTArray[1].vInterruptFunction != NULL){
    vEraseBit(EIMSK, 1);
    isrEXTINTArray[1].vInterruptFunction(isrEXTINTArray[1].vpArgument);
    if (ui8ReadBit(ui8EnabledEXTINT, 1) == 1){
      vSetBit(EIMSK, 1);
    }
  }
}

#if defined(ARDUINO_AVR_MEGA2560) || defined(ARDUINO_AVR_MEGA)
  ISR(INT2_vect){
    if (isrEXTINTArray[2].vInterruptFunction != NULL){
      vEraseBit(EIMSK, 2);
      isrEXTINTArray[2].vInterruptFunction(isrEXTINTArray[2].vpArgument);
      if (ui8ReadBit(ui8EnabledEXTINT ,2) == 1){
        vSetBit(EIMSK, 2);
      }
    }
  }

  ISR(INT3_vect){
    if (isrEXTINTArray[3].vInterruptFunction != NULL){
      vEraseBit(EIMSK, 3);
      isrEXTINTArray[3].vInterruptFunction(isrEXTINTArray[3].vpArgument);
      if (ui8ReadBit(ui8EnabledEXTINT, 3) == 1){
        vSetBit(EIMSK, 3);
      }
    }
  }

  ISR(INT4_vect){
    if (isrEXTINTArray[4].vInterruptFunction != NULL){
      vEraseBit(EIMSK, 4);
      isrEXTINTArray[4].vInterruptFunction(isrEXTINTArray[4].vpArgument);
      if (ui8ReadBit(ui8EnabledEXTINT, 4) == 1){
        vSetBit(EIMSK, 4);
      }
    }
  }

  ISR(INT5_vect){
    if (isrEXTINTArray[5].vInterruptFunction != NULL){
      vEraseBit(EIMSK, 5);
      isrEXTINTArray[5].vInterruptFunction(isrEXTINTArray[5].vpArgument);
      if (ui8ReadBit(ui8EnabledEXTINT, 5) == 1){
        vSetBit(EIMSK, 5);
      }
    }
  }

  ISR(INT6_vect){
    if (isrEXTINTArray[6].vInterruptFunction != NULL){
      vEraseBit(EIMSK, 6);
      isrEXTINTArray[6].vInterruptFunction(isrEXTINTArray[6].vpArgument);
      if (ui8ReadBit(ui8EnabledEXTINT, 6) == 1){
        vSetBit(EIMSK, 6);
      }
    }
  }

  ISR(INT7_vect){
    if (isrEXTINTArray[7].vInterruptFunction != NULL){
      vEraseBit(EIMSK, 7);
      isrEXTINTArray[7].vInterruptFunction(isrEXTINTArray[7].vpArgument);
      if (ui8ReadBit(ui8EnabledEXTINT, 7) == 1){
        vSetBit(EIMSK, 7);
      }
    }
  }
#endif
