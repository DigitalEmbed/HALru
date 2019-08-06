//! HALru Version 1.0b
/*!
  This code file was written by Jorge Henrique Moreira Santana and is under
  the GNU GPLv3 license. All legal rights are reserved.

  Permissions of this copy left license are conditioned on making available
  complete source code of licensed works and modifications under the same
  license or the GNU GPLv3. Copyright and license notices must be preserved.
  Contributors provide an express grant of patent rights. However, a larger
  work using the licensed work through interfaces provided by the licensed
  work may be distributed under different terms and without source code for
  the larger work.

  * Permissions:
    -" Commercial use;
    -" Modification;
    -" Distribution;
    -" Patent Use;
    -" Private Use;

  * Limitations:
    -" Liability;
    -" Warranty;

  * Conditions:
    -" License and copyright notice;
    -" Disclose source;
    -" State changes;
    -" Same license (library);

  For more informations, check the LICENSE document. If you want to use a
  commercial product without having to provide the source code, send an email
  to jorge_henrique_123@hotmail.com to talk.
*/

#ifndef ADC_H
#define ADC_H

#ifdef __cplusplus
  extern "C" {
#endif

#include "HALru.h"

#define   ADC_PIN_SETTED                                2
#define   ERROR_ADC_PIN_NOT_SETTED                      12

#define   ADC_PIN_UNSETTED                              3
#define   ERROR_ADC_PIN_NOT_UNSETTED                    13

#define   ADC_SAFE_SAMPLING_RATE                        0

#define   ADC0                                          0
#define   ADC1                                          1
#define   ADC2                                          2
#define   ADC3                                          3
#define   ADC4                                          4
#define   ADC5                                          5
#define   ADC6                                          6
#define   ADC7                                          7

#if defined(ARDUINO_AVR_MEGA2560) || defined(ARDUINO_AVR_MEGA)
  #define   ADC8                                        8
  #define   ADC9                                        9
  #define   ADC10                                       10
  #define   ADC11                                       11
  #define   ADC12                                       12
  #define   ADC13                                       13
  #define   ADC14                                       14
  #define   ADC15                                       15
#else
  #define   ADC_INTERNAL_TEMPERATURE_SENSOR             8
#endif

#define   ADC_PS_2                                      1
#define   ADC_PS_4                                      2
#define   ADC_PS_8                                      3
#define   ADC_PS_16                                     4
#define   ADC_PS_32                                     5
#define   ADC_PS_64                                     6
#define   ADC_PS_128                                    7

#define   ADC_AUTO_TRIGGER_MODE                         0
#define   ADC_MANUAL_TRIGGER_MODE                       1

#define   ADC_REFERENCE_VOLTAGE_AREF                    0
#define   ADC_REFERENCE_VOLTAGE_VCC                     1

#if defined(ARDUINO_AVR_MEGA2560) || defined(ARDUINO_AVR_MEGA)
  #define   ADC_REFERENCE_VOLTAGE_INTERNAL_1V1          2
  #define   ADC_REFERENCE_VOLTAGE_INTERNAL_2V56         3
#else
  #define   ADC_REFERENCE_VOLTAGE_INTERNAL_1V1          3
#endif

#define   ADC_RESOLUTION_8BITS                          0
#define   ADC_RESOLUTION_10BITS                         1

#define   vSetADCReferenceVoltage(ui8ReferenceMode)     vCopyBits(ADMUX, REFS0, ui8ReferenceMode, 0, 2)

#define   vSetADCResolution(ui8Resolution)              if (ui8Resolution == ADC_RESOLUTION_8BITS){\
                                                          vSetBit(ADMUX, ADLAR);\
                                                        }\
                                                        else{\
                                                          vEraseBit(ADMUX, ADLAR);\
                                                        }

#define   vEnableADC()                                  vSetBit(ADCSRA, ADEN);\
                                                        vSetBit(ADCSRA, ADIE);\
                                                        vSetBit(ADCSRA, ADSC)

#define   vDisableADC()                                 vEraseBit(ADCSRA, ADEN);\
                                                        vEraseBit(ADCSRA, ADIE);\
                                                        vEraseBit(ADCSRA, ADSC)

#define   vIgnoreADCRequest()                           vEraseBit(ADCSRA, ADIF)

#define   vSetADCPrescaler(ui8ADCPrescaler)             vCopyBits(ADCSRA, ADPS0, ui8ADCPrescaler, 0, 3)

#define   vSetADCTriggerMode(ui8ADCMode)                if(ui8ADCMode == ADC_AUTO_TRIGGER_MODE){\
                                                          vSetBit(ADCSRA, ADATE);\
                                                        }\
                                                        else{\
                                                          vEraseBit(ADCSRA, ADATE);\
                                                        }

#define   vStarADConvertion()                           vSetBit(ADCSRA, ADSC)
#define   vStopADConvertion()                           vEraseBit(ADCSRA, ADSC)

#define   ui8GetADCValue(ui8ADCInput)                   (uint8_t) ui16GetADCValue(ui8ADCInput)

#define   vADCInit()                                    vSetADCReferenceVoltage(ADC_REFERENCE_VOLTAGE_VCC);\
                                                        vSetADCSamplingRate(ADC_SAFE_SAMPLING_RATE);\
                                                        vSetADCTriggerMode(ADC_AUTO_TRIGGER_MODE);\
                                                        vSetADCResolution(ADC_RESOLUTION_10BITS)

void vSetADCSamplingRate(uint32_t ui32SamplingRate);
void vAttachADCInterrupt(uint8_t ui8ADCInput, isr_pfunc_t vInterruptFunction, void* vpArgument);
void vDettachADCInterrupt(uint8_t ui8ADCInput);
uint16_t ui16GetADCValue(uint8_t ui8ADCInput);
void vSelectADCInput(uint8_t ui8ADCInput);

#ifdef __cplusplus
  }
#endif

#endif
