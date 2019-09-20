#include "USART.h"
#include <stdio.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include <EmbeddedTools.h>
#include "./Configs.h"
#include "./Interrupts.h"

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
  volatile hal_isr_t isrDataRegisterEmpty[4] = {{NULL, NULL}};
  volatile hal_isr_t isrRXComplete[4] = {{NULL, NULL}};
  volatile hal_isr_t isrTXComplete[4] = {{NULL, NULL}};
  volatile uint16_t ui16ActivedUSARTs = 0;
  #define   usrGetUSARTNumber(usrGroup)                       usrGroup == USART_0 ? 0 :\
                                                              usrGroup == USART_1 ? 1 :\
                                                              usrGroup == USART_2 ? 2 : 3
#else
  volatile hal_isr_t isrDataRegisterEmpty = {NULL, NULL};
  volatile hal_isr_t isrRXComplete = {NULL, NULL};
  volatile hal_isr_t isrTXComplete = {NULL, NULL};
  volatile uint8_t ui8ActivedUSARTs = 0;
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
  #if defined(ARDUINO_AVR_MEGA2560) || defined(ARDUINO_AVR_MEGA)
    vSetBit(ui16ActivedUSARTs, (3*(usrGetUSARTNumber(ui8pGroup)) + ui8InterruptionType));
  #else
    vSetBit(ui8ActivedUSARTs, ui8InterruptionType);
  #endif
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
  #if defined(ARDUINO_AVR_MEGA2560) || defined(ARDUINO_AVR_MEGA)
    vEraseBit(ui16ActivedUSARTs, (3*(usrGetUSARTNumber(ui8pGroup)) + ui8InterruptionType));
  #else
    vEraseBit(ui8ActivedUSARTs, ui8InterruptionType);
  #endif
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
  Sends USART data.
  \param ui8pGroup is a volatile 8-bit pointer integer. It's the USART group (USART_X).
  \param ui8Data is a 8-bit integer.
*/
void vUSARTSendByte(volatile uint8_t* ui8pGroup, uint8_t ui8Data){
  uint8_t ui8UCSRB = *regUCSRB(ui8pGroup);
  while (!(*regUCSRA(ui8pGroup) & (1 << UDREn)));
  *regUCSRB(ui8pGroup) &= 31;
  vEraseBit(SREG, 7);
  (*regUDR(ui8pGroup)) = ui8Data;
  vSetBit(SREG, 7);
  *regUCSRB(ui8pGroup) = ui8UCSRB;
}

//! Function: USART Interrupt Attacher
/*!
  Attaches an USART interruption function.
  \param ui8pGroup is a volatile 8-bit pointer integer. It's the USART group (USART_X).
  \param ui8InterruptionType is a 8-bit integer. It's the interruption type.
  \param vInterruptFunction is a function pointer. It's the callback interruption.
  \param vpArgument is a void pointer. It's the callback argument.
*/
void vAttachUSARTInterrupt(volatile uint8_t* ui8pGroup, uint8_t ui8InterruptionType, void (*vInterruptFunction)(void*), void* vpArgument){
  #if defined(ARDUINO_AVR_MEGA2560) || defined(ARDUINO_AVR_MEGA)
    uint8_t ui8USARTNumber = usrGetUSARTNumber(ui8pGroup);
  #endif
  switch(ui8InterruptionType){

    case RX_COMPLETE:
    {
      #if defined(ARDUINO_AVR_MEGA2560) || defined(ARDUINO_AVR_MEGA)
        isrRXComplete[ui8USARTNumber].vInterruptFunction = vInterruptFunction;
        isrRXComplete[ui8USARTNumber].vpArgument = vpArgument;
      #else
        isrRXComplete.vInterruptFunction = vInterruptFunction;
        isrRXComplete.vpArgument = vpArgument;
      #endif
      break;
    }

    case TX_COMPLETE:
    {
      #if defined(ARDUINO_AVR_MEGA2560) || defined(ARDUINO_AVR_MEGA)
        isrTXComplete[ui8USARTNumber].vInterruptFunction = vInterruptFunction;
        isrTXComplete[ui8USARTNumber].vpArgument = vpArgument;
      #else
        isrTXComplete.vInterruptFunction = vInterruptFunction;
        isrTXComplete.vpArgument = vpArgument;
      #endif
      break;
    }

    case DATA_EMPTY:
    {
      #if defined(ARDUINO_AVR_MEGA2560) || defined(ARDUINO_AVR_MEGA)
        isrDataRegisterEmpty[ui8USARTNumber].vInterruptFunction = vInterruptFunction;
        isrDataRegisterEmpty[ui8USARTNumber].vpArgument = vpArgument;
      #else
        isrDataRegisterEmpty.vInterruptFunction = vInterruptFunction;
        isrDataRegisterEmpty.vpArgument = vpArgument;
      #endif
      break;
    }

  }
}

//! Function: USART Interrupt Dettacher
/*!
  Dettaches USART interruption function.
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
    if (isrRXComplete[0].vInterruptFunction != NULL){
      vEraseBit(*regUCSRB(USART_0), RXCIEn);
      isrRXComplete[0].vInterruptFunction(isrRXComplete[0].vpArgument);
      if (ui8ReadBit(ui16ActivedUSARTs, 0) == 1){
        vSetBit(*regUCSRB(USART_0), RXCIEn);
      }
    }
  }

  ISR(USART0_TX_vect){
    if (isrTXComplete[0].vInterruptFunction != NULL){
      vEraseBit(*regUCSRB(USART_0), TXCIEn);
      isrTXComplete[0].vInterruptFunction(isrTXComplete[0].vpArgument);
      if (ui8ReadBit(ui16ActivedUSARTs, 1) == 1){
        vSetBit(*regUCSRB(USART_0), TXCIEn);
      }
    }
  }

  ISR(USART0_UDRE_vect){
    if (isrDataRegisterEmpty[0].vInterruptFunction != NULL){
      vEraseBit(*regUCSRB(USART_0), UDREn);
      isrDataRegisterEmpty[0].vInterruptFunction(isrDataRegisterEmpty[0].vpArgument);
      if (ui8ReadBit(ui16ActivedUSARTs, 2) == 1){
        vSetBit(*regUCSRB(USART_0), UDREn);
      }
    }
  }

  ISR(USART1_RX_vect){
    if (isrRXComplete[1].vInterruptFunction != NULL){
      vEraseBit(*regUCSRB(USART_1), RXCIEn);
      isrRXComplete[1].vInterruptFunction(isrRXComplete[1].vpArgument);
      if (ui8ReadBit(ui16ActivedUSARTs, 3) == 1){
        vSetBit(*regUCSRB(USART_1), RXCIEn);
      }
    }
  }

  ISR(USART1_TX_vect){
    if (isrTXComplete[1].vInterruptFunction != NULL){
      vEraseBit(*regUCSRB(USART_1), TXCIEn);
      isrTXComplete[1].vInterruptFunction(isrTXComplete[1].vpArgument);
      if (ui8ReadBit(ui16ActivedUSARTs, 4) == 1){
        vSetBit(*regUCSRB(USART_1), TXCIEn);
      }
    }
  }

  ISR(USART1_UDRE_vect){
    if (isrDataRegisterEmpty[1].vInterruptFunction != NULL){
      vEraseBit(*regUCSRB(USART_1), UDREn);
      isrDataRegisterEmpty[1].vInterruptFunction(isrDataRegisterEmpty[1].vpArgument);
      if (ui8ReadBit(ui16ActivedUSARTs, 5) == 1){
        vSetBit(*regUCSRB(USART_1), UDREn);
      }
    }
  }

  ISR(USART2_RX_vect){
    if (isrRXComplete[2].vInterruptFunction != NULL){
      vEraseBit(*regUCSRB(USART_2), RXCIEn);
      isrRXComplete[2].vInterruptFunction(isrRXComplete[2].vpArgument);
      if (ui8ReadBit(ui16ActivedUSARTs, 6) == 1){
        vSetBit(*regUCSRB(USART_2), RXCIEn);
      }
    }
  }

  ISR(USART2_TX_vect){
    if (isrTXComplete[2].vInterruptFunction != NULL){
      vEraseBit(*regUCSRB(USART_2), TXCIEn);
      isrTXComplete[2].vInterruptFunction(isrTXComplete[2].vpArgument);
      if (ui8ReadBit(ui16ActivedUSARTs, 7) == 1){
        vSetBit(*regUCSRB(USART_2), TXCIEn);
      }
    }
  }

  ISR(USART2_UDRE_vect){
    if (isrDataRegisterEmpty[2].vInterruptFunction != NULL){
      vEraseBit(*regUCSRB(USART_2), UDREn);
      isrDataRegisterEmpty[2].vInterruptFunction(isrDataRegisterEmpty[2].vpArgument);
      if (ui8ReadBit(ui16ActivedUSARTs, 8) == 1){
        vSetBit(*regUCSRB(USART_2), UDREn);
      }
    }
  }

  ISR(USART3_RX_vect){
    if (isrRXComplete[3].vInterruptFunction != NULL){
      vEraseBit(*regUCSRB(USART_3), RXCIEn);
      isrRXComplete[3].vInterruptFunction(isrRXComplete[3].vpArgument);
      if (ui8ReadBit(ui16ActivedUSARTs, 9) == 1){
        vSetBit(*regUCSRB(USART_3), RXCIEn);
      }
    }
  }

  ISR(USART3_TX_vect){
    if (isrTXComplete[3].vInterruptFunction != NULL){
      vEraseBit(*regUCSRB(USART_3), TXCIEn);
      isrTXComplete[3].vInterruptFunction(isrTXComplete[3].vpArgument);
      if (ui8ReadBit(ui16ActivedUSARTs, 10) == 1){
        vSetBit(*regUCSRB(USART_3), TXCIEn);
      }
    }
  }

  ISR(USART3_UDRE_vect){
    if (isrDataRegisterEmpty[3].vInterruptFunction != NULL){
      vEraseBit(*regUCSRB(USART_3), UDREn);
      isrDataRegisterEmpty[3].vInterruptFunction(isrDataRegisterEmpty[3].vpArgument);
      if (ui8ReadBit(ui16ActivedUSARTs, 11) == 1){
        vSetBit(*regUCSRB(USART_3), UDREn);
      }
    }
  }
#else
  ISR(USART_RX_vect){
    if (isrRXComplete.vInterruptFunction != NULL){
      vEraseBit(*regUCSRB(USART_0), RXCIEn);
      isrRXComplete.vInterruptFunction(isrRXComplete.vpArgument);
      if (ui8ReadBit(ui8ActivedUSARTs, 0) == 1){
        vSetBit(*regUCSRB(USART_0), RXCIEn);
      }
    }
  }

  ISR(USART_TX_vect){
    if (isrTXComplete.vInterruptFunction != NULL){
      vEraseBit(*regUCSRB(USART_0), TXCIEn);
      isrTXComplete.vInterruptFunction(isrTXComplete.vpArgument);
      if (ui8ReadBit(ui8ActivedUSARTs, 1) == 1){
        vSetBit(*regUCSRB(USART_0), TXCIEn);
      }
    }
  }

  ISR(USART_UDRE_vect){
    if (isrDataRegisterEmpty.vInterruptFunction != NULL){
      vEraseBit(*regUCSRB(USART_0), UDREn);
      isrDataRegisterEmpty.vInterruptFunction(isrDataRegisterEmpty.vpArgument);
      if (ui8ReadBit(ui8ActivedUSARTs, 2) == 1){
        vSetBit(*regUCSRB(USART_0), UDREn);
      }
    }
  }
#endif