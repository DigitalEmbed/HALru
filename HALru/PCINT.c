#include "PCINT.h"

#if defined(ARDUINO_AVR_MEGA2560) || defined(ARDUINO_AVR_MEGA)
  volatile uint8_t ui8BeforeStatePCINT[3] = {0};
  volatile uint8_t ui8ActualStatePCINT[3] = {0};
#else
  volatile uint8_t ui8BeforeStatePCINT[3] = {0};
  volatile uint8_t ui8ActualStatePCINT[3] = {0};
#endif

hal_isr_t isrPCINTArray[3][8] = {{{NULL, NULL}}};

void vEnablePCINTPin(uint8_t ui8InterruptPin){
  if (ui8InterruptPin <= 7 && ui8ReadBit(PCMSK0, ui8InterruptPin) == 0){
    vSetBit(PCMSK0, ui8InterruptPin);
  }
  else if(ui8InterruptPin > 7 && ui8InterruptPin < 16 && ui8ReadBit(PCMSK1, uiModule(ui8InterruptPin - 8)) == 0){
    vSetBit(PCMSK1, uiModule(ui8InterruptPin - 8));
  }
  else if(ui8ReadBit(PCMSK2, uiModule(ui8InterruptPin - 16)) == 0){
    vSetBit(PCMSK2, uiModule(ui8InterruptPin - 16));
  }
}

void vDisablePCINTPin(uint8_t ui8InterruptPin){
  if (ui8InterruptPin <= 7 && ui8ReadBit(PCMSK0, ui8InterruptPin) == 1){
    vEraseBit(PCMSK0, ui8InterruptPin);
  }
  else if(ui8InterruptPin > 7 && ui8InterruptPin < 16 && ui8ReadBit(PCMSK1, uiModule(ui8InterruptPin - 8)) == 1){
    vEraseBit(PCMSK1, uiModule(ui8InterruptPin - 8));
  }
  else if (ui8ReadBit(PCMSK2, uiModule(ui8InterruptPin - 16)) == 1){
    vEraseBit(PCMSK2, uiModule(ui8InterruptPin - 16));
  }
}

void vAttachPCINTInterrupt(uint8_t ui8InterruptPin, isr_pfunc_t vInterruptFunction, void* vpArgument){
  if (ui8InterruptPin <= 7){
    isrPCINTArray[0][ui8InterruptPin].vInterruptFunction = vInterruptFunction;
    isrPCINTArray[0][ui8InterruptPin].vpArgument = vpArgument;
  }
  else if(ui8InterruptPin > 7 && ui8InterruptPin < 16){
    isrPCINTArray[1][ui8InterruptPin - 8].vInterruptFunction = vInterruptFunction;
    isrPCINTArray[1][ui8InterruptPin - 8].vpArgument = vpArgument;
  }
  else {
    isrPCINTArray[2][ui8InterruptPin - 16].vInterruptFunction = vInterruptFunction;
    isrPCINTArray[2][ui8InterruptPin - 16].vpArgument = vpArgument;
  }
}

void vDettachPCINTInterrupt(uint8_t ui8InterruptPin){
  vAttachPCINTInterrupt(ui8InterruptPin, NULL, NULL);
}

ISR(PCINT0_vect){
  ui8BeforeStatePCINT[0] = ui8ActualStatePCINT[0];
  ui8ActualStatePCINT[0] = *(IO_GROUP_B);
  volatile uint8_t ui8ActivedPins = ui8BeforeStatePCINT[0] ^ ui8ActualStatePCINT[0];
  volatile uint8_t ui8Counter = 0;
  for (ui8Counter = 0 ; ui8Counter <= 7 ; ui8Counter++){
    if (ui8ReadBit(ui8ActivedPins, ui8Counter) == 1 && isrPCINTArray[0][ui8Counter].vInterruptFunction != NULL){
      vEraseBit(PCICR, 0);
      isrPCINTArray[0][ui8Counter].vInterruptFunction(isrPCINTArray[0][ui8Counter].vpArgument);
      vSetBit(PCICR, 0);
    }
  }
}

ISR(PCINT1_vect){
  ui8BeforeStatePCINT[1] = ui8ActualStatePCINT[1];
  #if defined(ARDUINO_AVR_MEGA2560) || defined(ARDUINO_AVR_MEGA)
    ui8ActualStatePCINT[1] = (volatile uint8_t) (((*(IO_GROUP_E)) & 1) | (*(IO_GROUP_J) << 1));
  #else
    ui8ActualStatePCINT[1] = *(IO_GROUP_C);
  #endif
  volatile uint8_t ui8ActivedPins = ui8BeforeStatePCINT[1] ^ ui8ActualStatePCINT[1];
  volatile uint8_t ui8Counter = 0;
  for (ui8Counter = 0 ; ui8Counter <= 7 ; ui8Counter++){
    if (ui8ReadBit(ui8ActivedPins, ui8Counter) == 1 && isrPCINTArray[1][ui8Counter].vInterruptFunction != NULL){
      vEraseBit(PCICR, 1);
      isrPCINTArray[1][ui8Counter].vInterruptFunction(isrPCINTArray[1][ui8Counter].vpArgument);
      vSetBit(PCICR, 1);
    }
  }
}

ISR(PCINT2_vect){
  ui8BeforeStatePCINT[2] = ui8ActualStatePCINT[2];
  #if defined(ARDUINO_AVR_MEGA2560) || defined(ARDUINO_AVR_MEGA)
    ui8ActualStatePCINT[2] = *(IO_GROUP_K);
  #else
    ui8ActualStatePCINT[2] = *(IO_GROUP_D);
  #endif
  volatile uint8_t ui8ActivedPins = ui8BeforeStatePCINT[2] ^ ui8ActualStatePCINT[2];
  volatile uint8_t ui8Counter = 0;
  for (ui8Counter = 0 ; ui8Counter <= 7 ; ui8Counter++){
    if (ui8ReadBit(ui8ActivedPins, ui8Counter) == 1 && isrPCINTArray[2][ui8Counter].vInterruptFunction != NULL){
      vEraseBit(PCICR, 2);
      isrPCINTArray[2][ui8Counter].vInterruptFunction(isrPCINTArray[2][ui8Counter].vpArgument);
      vSetBit(PCICR, 2);
    }
  }
}
