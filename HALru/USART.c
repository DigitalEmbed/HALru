#include "USART.h"
#include <avr/interrupt.h>
#include <stdlib.h>
#include <stdio.h>

//! Macro: USART Macros
/*!
  This macros are for facilitate the use of this library.
*/
#define     regUCSRA(usrGroup)                                (usrGroup)
#define     regUBRRL(usrGroup)                                ((volatile uint8_t*) usrGroup + 4)
#define     regUBRRH(usrGroup)                                ((volatile uint8_t*) usrGroup + 5)

//! Declarations: Private USART Declarations
/*!
  This macros and variables are for facilitate the use of this library.
*/
#if defined(ARDUINO_AVR_MEGA2560) || defined(ARDUINO_AVR_MEGA)
  isr_usart_t isrDataRegisterEmpty[4] = {NULL};
  isr_usart_t isrRXComplete[4] = {NULL};
  isr_usart_t isrTXComplete[4] = {NULL};
  args_usart_t argDataRegisterEmpty[4] = {NULL};
  args_usart_t argRXComplete[4] = {NULL};
  args_usart_t argTXComplete[4] = {NULL};
  #define   usrGetUSARTNumber(usrGroup)                       usrGroup == USART_0 ? 0 :\
                                                              usrGroup == USART_1 ? 1 :\
                                                              usrGroup == USART_2 ? 2 : 3
#else
  isr_usart_t isrDataRegisterEmpty = NULL;
  isr_usart_t isrRXComplete = NULL;
  isr_usart_t isrTXComplete = NULL;
  args_usart_t argDataRegisterEmpty = NULL;
  args_usart_t argRXComplete = NULL;
  args_usart_t argTXComplete = NULL;
  #define   usrGetUSARTNumber(usrGroup)                       0
#endif

//! Function: USART Initializer
/*!
  Initialize a USART.
  \param ui8pGroup is a volatile 8-bit pointer integer. It's the USART group (USART_X).
*/
void vUSARTInit(volatile uint8_t* ui8pGroup){
  *regUCSRA(ui8pGroup) = 0;
  *regUCSRB(ui8pGroup) = 0;
  *regUCSRC(ui8pGroup) = 0;
  *regUBRRL(ui8pGroup) = 0;
  *regUBRRH(ui8pGroup) = 0;
  vSetUSARTSync(ui8pGroup, ASYNCHRONOUS);
  vSetUSARTActivationEdge(ui8pGroup, RISING);
  vSetUSARTParity(ui8pGroup, NO_PARITY);
  vSetUSARTStopBit(ui8pGroup, 1);
  vSetUSARTDataSize(ui8pGroup, 8);
  vEnableTX(ui8pGroup);
  vEnableRX(ui8pGroup);
  vSetUSARTBaudRate(ui8pGroup, 9600);
}

//! Function: USART Interrupt Enabler
/*!
  Enable a USART interruption.
  \param ui8pGroup is a volatile 8-bit pointer integer. It's the USART group (USART_X).
  \param ui8InterruptionType is a 8-bit integer. It's the interruption type.
*/
void vEnableUSARTInterrupt(volatile uint8_t* ui8pGroup, uint8_t ui8InterruptionType){
  switch(ui8InterruptionType){
    case RX_COMPLETE:
    {
      vSetBit(*regUCSRB(ui8pGroup), RXCIEn);
    }
    break;

    case TX_COMPLETE:
    {
      vSetBit(*regUCSRB(ui8pGroup), TXCIEn);
    }
    break;

    case DATA_EMPTY:
    {
      vSetBit(*regUCSRB(ui8pGroup), UDREn);
    }
    break;
  }
}

//! Function: USART Interrupt Disabler
/*!
  Disable a USART interruption.
  \param ui8pGroup is a volatile 8-bit pointer integer. It's the USART group (USART_X).
  \param ui8InterruptionType is a 8-bit integer. It's the interruption type.
*/
void vDisableUSARTInterrupt(volatile uint8_t* ui8pGroup, uint8_t ui8InterruptionType){
  switch(ui8InterruptionType){
    case RX_COMPLETE:
    {
      vEraseBit(*regUCSRB(ui8pGroup), RXCIEn);
    }
    break;

    case TX_COMPLETE:
    {
      vEraseBit(*regUCSRB(ui8pGroup), TXCIEn);
    }
    break;

    case DATA_EMPTY:
    {
      vEraseBit(*regUCSRB(ui8pGroup), UDREn);
    }
    break;
  }
}

//! Function: USART Baud Rate Setter
/*!
  Set USART baud rate.
  \param ui8pGroup is a volatile 8-bit pointer integer. It's the USART group (USART_X).
  \param ui32BaudRate is a 32-bit integer. It's the USART baud rate.
*/
void vSetUSARTBaudRate(volatile uint8_t* ui8pGroup, uint32_t ui32BaudRate){
  uint8_t ui8Sync = (*regUCSRC(ui8pGroup) >> UMSELn0) & 3;
  if (ui8Sync == ASYNCHRONOUS){
    ui32BaudRate = (ui32BaudRate >> 1);
    vSetBit(*regUCSRA(ui8pGroup), U2Xn);
  }
  else{
    vEraseBit(*regUCSRA(ui8pGroup), U2Xn);
  }
  uint16_t ui16UBRR = DEVICE_CLOCK_HZ/(ui32BaudRate << 4) - 1;
  vCopyBits(*regUBRRH(ui8pGroup), 0, ui16UBRR, 8, 4);
  vCopyByte(*regUBRRL(ui8pGroup), 0, ui16UBRR, 0);
}

//! Function: USART Data Sender
/*!
  Send data on a USART.
  \param ui8pGroup is a volatile 8-bit pointer integer. It's the USART group (USART_X).
  \param ui8Data is a 8-bit integer.
*/
void vUSARTSendByte(volatile uint8_t* ui8pGroup, uint8_t ui8Data){
  uint8_t ui8UCSRB = *regUCSRB(ui8pGroup);
  while (!(*regUCSRA(ui8pGroup) & (1 << UDREn)));
  *regUCSRB(ui8pGroup) &= 31;
  vDisableAllInterrupts();
  (*regUDR(ui8pGroup)) = ui8Data;
  vEnableAllInterrupts();
  *regUCSRB(ui8pGroup) = ui8UCSRB;
}

//! Function: USART Interrupt Attacher
/*!
  Attach a USART interruption function.
  \param ui8pGroup is a volatile 8-bit pointer integer. It's the USART group (USART_X).
  \param ui8InterruptionType is a 8-bit integer. It's the interruption type.
  \param vInterruptFunction is a function pointer. It's the callback interruption.
  \param vpArgument is a void pointer. It's the callback argument.
*/
void vAttachUSARTInterrupt(volatile uint8_t* ui8pGroup, uint8_t ui8InterruptionType, isr_usart_t vInterruptFunction, void* vpArgument){
  #if defined(ARDUINO_AVR_MEGA2560) || defined(ARDUINO_AVR_MEGA)
    uint8_t ui8USARTNumber = usrGetUSARTNumber(ui8pGroup);
  #endif
  switch(ui8InterruptionType){

    case RX_COMPLETE:
    {
      #if defined(ARDUINO_AVR_MEGA2560) || defined(ARDUINO_AVR_MEGA)
        isrRXComplete[ui8USARTNumber] = vInterruptFunction;
        argRXComplete[ui8USARTNumber] = vpArgument;
      #else
        isrRXComplete = vInterruptFunction;
        argRXComplete = vpArgument;
      #endif
      break;
    }

    case TX_COMPLETE:
    {
      #if defined(ARDUINO_AVR_MEGA2560) || defined(ARDUINO_AVR_MEGA)
        isrTXComplete[ui8USARTNumber] = vInterruptFunction;
        argTXComplete[ui8USARTNumber] = vpArgument;
      #else
        isrTXComplete = vInterruptFunction;
        argTXComplete = vpArgument;
      #endif
      break;
    }

    case DATA_EMPTY:
    {
      #if defined(ARDUINO_AVR_MEGA2560) || defined(ARDUINO_AVR_MEGA)
        isrDataRegisterEmpty[ui8USARTNumber] = vInterruptFunction;
        argDataRegisterEmpty[ui8USARTNumber] = vpArgument;
      #else
        isrDataRegisterEmpty = vInterruptFunction;
        argDataRegisterEmpty = vpArgument;
      #endif
      break;
    }

  }
}

//! Function: USART Interrupt Dettacher
/*!
  Dettach a USART interruption function.
  \param ui8pGroup is a volatile 8-bit pointer integer. It's the USART group (USART_X).
  \param ui8InterruptionType is a 8-bit integer. It's the interruption type.
*/
void vDettachUSARTInterrupt(volatile uint8_t* ui8pGroup, uint8_t ui8InterruptionType){
  vAttachUSARTInterrupt(ui8pGroup, ui8InterruptionType, NULL, NULL);
}

//! Callbacks: USART Interruptions
/*!
  Callbacks of USART hardware interruptions.
*/
#if defined(ARDUINO_AVR_MEGA2560) || defined(ARDUINO_AVR_MEGA)
  ISR(USART0_RX_vect){
    if (isrRXComplete[0] != NULL){
      vDisableUSARTInterrupt(USART_0, RX_COMPLETE);
      isrRXComplete[0](argRXComplete[0]);
      vEnableUSARTInterrupt(USART_0, RX_COMPLETE);
    }
  }

  ISR(USART0_TX_vect){
    if (isrTXComplete[0] != NULL){
      vDisableUSARTInterrupt(USART_0, TX_COMPLETE);
      isrTXComplete[0](argTXComplete[0]);
      vEnableUSARTInterrupt(USART_0, TX_COMPLETE);
    }
  }

  ISR(USART0_UDRE_vect){
    if (isrDataRegisterEmpty[0] != NULL){
      vDisableUSARTInterrupt(USART_0, DATA_EMPTY);
      isrDataRegisterEmpty[0](argDataRegisterEmpty[0]);
      vEnableUSARTInterrupt(USART_0, DATA_EMPTY);
    }
  }

  ISR(USART1_RX_vect){
    if (isrRXComplete[1] != NULL){
      vDisableUSARTInterrupt(USART_1, RX_COMPLETE);
      isrRXComplete[1](argRXComplete[1]);
      vEnableUSARTInterrupt(USART_1, RX_COMPLETE);
    }
  }

  ISR(USART1_TX_vect){
    if (isrTXComplete[1] != NULL){
      vDisableUSARTInterrupt(USART_1, RX_COMPLETE);
      isrTXComplete[1](argTXComplete[1]);
      vEnableUSARTInterrupt(USART_1, RX_COMPLETE);
    }
  }

  ISR(USART1_UDRE_vect){
    if (isrDataRegisterEmpty[1] != NULL){
      vDisableUSARTInterrupt(USART_1, DATA_EMPTY);
      isrDataRegisterEmpty[1](argDataRegisterEmpty[1]);
      vEnableUSARTInterrupt(USART_1, DATA_EMPTY);
    }
  }

  ISR(USART2_RX_vect){
    if (isrRXComplete[2] != NULL){
      vDisableUSARTInterrupt(USART_2, RX_COMPLETE);
      isrRXComplete[2](argRXComplete[2]);
      vEnableUSARTInterrupt(USART_2, RX_COMPLETE);
    }
  }

  ISR(USART2_TX_vect){
    if (isrTXComplete[2] != NULL){
      vDisableUSARTInterrupt(USART_2, RX_COMPLETE);
      isrTXComplete[2](argTXComplete[2]);
      vEnableUSARTInterrupt(USART_2, RX_COMPLETE);
    }
  }

  ISR(USART2_UDRE_vect){
    if (isrDataRegisterEmpty[2] != NULL){
      vDisableUSARTInterrupt(USART_2, DATA_EMPTY);
      isrDataRegisterEmpty[2](argDataRegisterEmpty[2]);
      vEnableUSARTInterrupt(USART_2, DATA_EMPTY);
    }
  }

  ISR(USART3_RX_vect){
    if (isrRXComplete[3] != NULL){
      vDisableUSARTInterrupt(USART_3, RX_COMPLETE);
      isrRXComplete[3](argRXComplete[3]);
      vEnableUSARTInterrupt(USART_3, RX_COMPLETE);
    }
  }

  ISR(USART3_TX_vect){
    if (isrTXComplete[3] != NULL){
      vDisableUSARTInterrupt(USART_3, RX_COMPLETE);
      isrTXComplete[3](argTXComplete[3]);
      vEnableUSARTInterrupt(USART_3, RX_COMPLETE);
    }
  }

  ISR(USART3_UDRE_vect){
    if (isrDataRegisterEmpty[3] != NULL){
      vDisableUSARTInterrupt(USART_3, DATA_EMPTY);
      isrDataRegisterEmpty[3](argDataRegisterEmpty[3]);
      vEnableUSARTInterrupt(USART_3, DATA_EMPTY);
    }
  }
#else
  ISR(USART_RX_vect){
    if (isrRXComplete != NULL){
      vDisableUSARTInterrupt(USART_0, RX_COMPLETE);
      isrRXComplete(argRXComplete);
      vEnableUSARTInterrupt(USART_0, RX_COMPLETE);
    }
  }

  ISR(USART_TX_vect){
    if (isrTXComplete != NULL){
      vDisableUSARTInterrupt(USART_0, TX_COMPLETE);
      isrTXComplete(argTXComplete);
      vEnableUSARTInterrupt(USART_0, TX_COMPLETE);
    }
  }

  ISR(USART_UDRE_vect){
    if (isrDataRegisterEmpty != NULL){
      vDisableUSARTInterrupt(USART_0, DATA_EMPTY);
      isrDataRegisterEmpty(argDataRegisterEmpty);
      vEnableUSARTInterrupt(USART_0, DATA_EMPTY);
    }
  }
#endif
