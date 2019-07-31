#include "ADC.h"


#if defined(ARDUINO_AVR_MEGA2560) || defined(ARDUINO_AVR_MEGA)
  #define   AMOUNT_OF_ADCS                              16
#else
  #define   AMOUNT_OF_ADCS                              9
#endif

#if defined(ARDUINO_AVR_MEGA2560) || defined(ARDUINO_AVR_MEGA)
  #define   vSetADCInput(ui8ADCInput)                   if (ui8ADCInput < 8){\
                                                          vEraseBit(ADCSRB, MUX5);\
                                                          vCopyBits(ADMUX, MUX0, ui8ADCInput, 0, 4);\
                                                        }\
                                                        else{\
                                                          vSetBit(ADCSRB, MUX5);\
                                                          vCopyBits(ADMUX, MUX0, uiModule(ui8ADCInput - 8), 0, 4);\
                                                        }

  #define   vDisableGPIOPin(ui8ADCInput)                if (ui8ADCInput < 8){\
                                                          vSetBit(DIDR0, ui8ADCInput);\
                                                        }\
                                                        else{\
                                                          vSetBit(DIDR2, uiModule(ui8ADCInput - 8));\
                                                        }

  #define   vEnableGPIOPin(ui8ADCInput)                 if (ui8ADCInput < 8){\
                                                          vEraseBit(DIDR0, ui8ADCInput);\
                                                        }\
                                                        else{\
                                                          vEraseBit(DIDR2, uiModule(ui8ADCInput - 8));\
                                                        }
#else
  #define   vSetADCInput(ui8ADC)                        vCopyBits(ADMUX, MUX0, ui8ADC, 0, 4)
  #define   vDisableGPIOPin(ui8ADCInput)                vSetBit(DIDR0, ui8ADCInput)
#endif

volatile isr_t isrADCInterruptVector[AMOUNT_OF_ADCS] = {{NULL, NULL}};
volatile uint16_t ui16ADCValuesVector[AMOUNT_OF_ADCS] = {65535};
volatile uint8_t ui8SelectedADCInput = 255;
volatile uint8_t ui8SafePrescaler = 0;

void vSetADCSamplingRate(uint32_t ui32SamplingRate){
  uint8_t ui8Counter = 0;
  uint32_t ui32ADCFrequency = 0;
  if (ui32SamplingRate == ADC_SAFE_SAMPLING_RATE){
    vSetADCPrescaler(ui8SafePrescaler);
  }
  else{
    for (ui8Counter = 8 ; ui8Counter > 0 ; ui8Counter--){
      if (ui32SamplingRate == ADC_SAFE_SAMPLING_RATE){
        if (ui8SafePrescaler == 0){
          ui32ADCFrequency = (DEVICE_CLOCK_HZ >> ui8Counter);
          if (ui32ADCFrequency >= 50000 && ui32ADCFrequency <= 200000){
            vSetADCPrescaler(ui8Counter);
            ui8SafePrescaler = ui8Counter;
          }
        }
        else{
          vSetADCPrescaler(ui8SafePrescaler);
        }
      }
      else if (ui32SamplingRate <= (uint32_t) ((DEVICE_CLOCK_HZ >> ui8Counter)/13)){
        vSetADCPrescaler(ui8Counter);
      }
    }
  }
}

void vAttachADCInterrupt(uint8_t ui8ADCInput, pfunc_t vInterruptFunction, void* vpArgument){
  if (ui8ADCInput < AMOUNT_OF_ADCS){
    isrADCInterruptVector[ui8ADCInput].vInterruptFunction = vInterruptFunction;
    isrADCInterruptVector[ui8ADCInput].vpArgument = vpArgument;
  }
}

void vDettachADCInterrupt(uint8_t ui8ADCInput){
  if (ui8ADCInput < AMOUNT_OF_ADCS){
    isrADCInterruptVector[ui8ADCInput].vInterruptFunction = NULL;
    isrADCInterruptVector[ui8ADCInput].vpArgument = NULL;
  }
}

uint16_t ui16GetADCValue(uint8_t ui8ADCInput){
  if (ui8ADCInput < AMOUNT_OF_ADCS){
    return ui16ADCValuesVector[ui8ADCInput];
  }
  return 65535;
}

void vSelectADCInput(uint8_t ui8ADCInput){
  if (ui8SelectedADCInput == 255){
    vDisableGPIOPin(ui8ADCInput);
    vSetADCInput(ui8ADCInput);
  }
  ui8SelectedADCInput = ui8ADCInput;
}

ISR(ADC_vect){
  static volatile uint8_t ui8ActualADCInput = 0;
  if (ui8ActualADCInput >= AMOUNT_OF_ADCS){
    ui8ActualADCInput = ui8SelectedADCInput;
  }
  else {
    ui16ADCValuesVector[ui8ActualADCInput] = ADCH;
    if (ui8ReadBit(ADMUX, ADLAR) == 0){
      ui16ADCValuesVector[ui8ActualADCInput] = (ui16ADCValuesVector[ui8ActualADCInput] << 8) + ADCL;
    }
    if (isrADCInterruptVector[ui8ActualADCInput].vInterruptFunction != NULL){
      isrADCInterruptVector[ui8ActualADCInput].vInterruptFunction(isrADCInterruptVector[ui8ActualADCInput].vpArgument);
    }
    if (ui8ActualADCInput != ui8SelectedADCInput){
      vStopADConvertion();
      vEnableGPIOPin(ui8ActualADCInput);
      ui8ActualADCInput = ui8SelectedADCInput;
      vDisableGPIOPin(ui8SelectedADCInput);
      vSetADCInput(ui8SelectedADCInput);
      vStarADConvertion();
    }
  }
}
