#include "./USART.h"

/*!
  Prototype public "methods".
*/
static void vPublicSetDefaultMode(usart_t* usrObjectUSART, uint32_t ui32BaudRate);                                                  /*!< void "method". */
static void vPublicEnableRX(usart_t* usrObjectUSART);                                                                               /*!< void "method". */
static void vPublicEnableTX(usart_t* usrObjectUSART);                                                                               /*!< void "method". */
static void vPublicDisableRX(usart_t* usrObjectUSART);                                                                              /*!< void "method". */
static void vPublicDisableTX(usart_t* usrObjectUSART);                                                                              /*!< void "method". */
static void vPublicSetDataSize(usart_t* usrObjectUSART, uint8_t ui8DataSize);                                                       /*!< void "method". */
static void vPublicAsynchronousMode(usart_t* usrObjectUSART);                                                                       /*!< void "method". */
static void vPublicSynchronousMode(usart_t* usrObjectUSART);                                                                        /*!< void "method". */
static void vPublicMasterSPIMode(usart_t* usrObjectUSART);                                                                          /*!< void "method". */
static void vPublicNoParityMode(usart_t* usrObjectUSART);                                                                           /*!< void "method". */
static void vPublicPairMode(usart_t* usrObjectUSART);                                                                               /*!< void "method". */
static void vPublicOddMode(usart_t* usrObjectUSART);                                                                                /*!< void "method". */
static void vPublicFallingEdgeMode(usart_t* usrObjectUSART);                                                                        /*!< void "method". */
static void vPublicRisingEdgeMode(usart_t* usrObjectUSART);                                                                         /*!< void "method". */
static void vPublicSetStopBit(usart_t* usrObjectUSART, uint8_t ui8StopBit);                                                         /*!< void "method". */
static void vPublicSetBaudRate(usart_t* usrObjectUSART, uint32_t ui32BaudRate);                                                     /*!< void "method". */
static void vPublicWrite(usart_t* usrObjectUSART, const char cData);                                                                /*!< void "method". */
static uint8_t ui8PublicRead(usart_t* usrObjectUSART);                                                                              /*!< uint8_t "method". */
static void vPublicPrint(usart_t* usrObjectUSART, const char* cpString, ...);                                                       /*!< void "method". */
static void vPublicPrintln(usart_t* usrObjectUSART, const char* cpString, ...);                                                     /*!< void "method". */
static void vPublicAttachRXInterrupt(usart_t* usrObjectUSART, void (*vInterruptFunction)(void*), void* vpArgument);                 /*!< void "method". */
static void vPublicAttachTXInterrupt(usart_t* usrObjectUSART, void (*vInterruptFunction)(void*), void* vpArgument);                 /*!< void "method". */
static void vPublicAttachEmptyInterrupt(usart_t* usrObjectUSART, void (*vInterruptFunction)(void*), void* vpArgument);              /*!< void "method". */
static void vPublicDetachRXInterrupt(usart_t* usrObjectUSART);                                                                     /*!< void "method". */
static void vPublicDetachTXInterrupt(usart_t* usrObjectUSART);                                                                     /*!< void "method". */
static void vPublicDetachEmptyInterrupt(usart_t* usrObjectUSART);                                                                  /*!< void "method". */
static void vPublicEnableRXInterrupt(usart_t* usrObjectUSART);                                                                      /*!< void "method". */
static void vPublicEnableTXInterrupt(usart_t* usrObjectUSART);                                                                      /*!< void "method". */
static void vPublicEnableEmptyInterrupt(usart_t* usrObjectUSART);                                                                   /*!< void "method". */
static void vPublicDisableRXInterrupt(usart_t* usrObjectUSART);                                                                     /*!< void "method". */
static void vPublicDisableTXInterrupt(usart_t* usrObjectUSART);                                                                     /*!< void "method". */
static void vPublicDisableEmptyInterrupt(usart_t* usrObjectUSART);                                                                  /*!< void "method". */

/*!
  USART object "constructor".
*/
const usart_manager_t USART PROGMEM = {
  .vWrite = &vPublicWrite,
  .ui8Read = &ui8PublicRead,
  .vPrint = &vPublicPrint,
  .vPrintln = &vPublicPrintln,
  .RXInterrupt = {
    .vAttach = &vPublicAttachRXInterrupt,
    .vDetach = &vPublicDetachRXInterrupt,
    .vEnable = &vPublicEnableRXInterrupt,
    .vDisable = &vPublicDisableRXInterrupt
  },
  .TXInterrupt = {
    .vAttach = &vPublicAttachTXInterrupt,
    .vDetach = &vPublicDetachTXInterrupt,
    .vEnable = &vPublicEnableTXInterrupt,
    .vDisable = &vPublicDisableTXInterrupt
  },
  .EmptyInterrupt = {
    .vAttach = &vPublicAttachEmptyInterrupt,
    .vDetach = &vPublicDetachEmptyInterrupt,
    .vEnable = &vPublicEnableEmptyInterrupt,
    .vDisable = &vPublicDisableEmptyInterrupt
  },
  .Configurations = {
    .vSetDefaultMode = &vPublicSetDefaultMode,
    .vEnableRX = &vPublicEnableRX,
    .vEnableTX = &vPublicEnableTX,
    .vDisableRX = &vPublicDisableRX,
    .vDisableTX = &vPublicDisableTX,
    .vSetDataSize = &vPublicSetDataSize,
    .vSetAsynchronousMode = &vPublicAsynchronousMode,
    .vSetSynchronousMode = &vPublicSynchronousMode,
    .vSetMasterSPIMode = &vPublicMasterSPIMode,
    .vSetNoParityMode = &vPublicNoParityMode,
    .vSetPairMode = &vPublicPairMode,
    .vSetOddMode = &vPublicOddMode,
    .vSetFallingEdgeMode = &vPublicFallingEdgeMode,
    .vSetRisingEdgeMode = &vPublicRisingEdgeMode,
    .vSetStopBit = &vPublicSetStopBit,
    .vSetBaudRate = &vPublicSetBaudRate
  }
};

//! Public Method: Default Mode Setter
/*!
  Sets USART default mode.
  \param usrObjectUSART is a usart_t "object".
  \param ui32BaudRate is a 32-bit integer. It's the USART baud rate.
*/
static void vPublicSetDefaultMode(usart_t* usrObjectUSART, uint32_t ui32BaudRate){
  vUSARTInit(usrObjectUSART->ui8pUSARTGroup);
  vSetUSARTBaudRate(usrObjectUSART->ui8pUSARTGroup, ui32BaudRate);
}

//! Public Method: RX Enabler
/*!
  Enables RX for a USART comunication.
  \param usrObjectUSART is a usart_t "object".
*/
static void vPublicEnableRX(usart_t* usrObjectUSART){
  vEnableRX(usrObjectUSART->ui8pUSARTGroup);
}

//! Public Method: TX Enabler
/*!
  Enables TX for a USART comunication.
  \param usrObjectUSART is a usart_t "object".
*/
static void vPublicEnableTX(usart_t* usrObjectUSART){
  vEnableTX(usrObjectUSART->ui8pUSARTGroup);
}

//! Public Method: RX Disabler
/*!
  Disables RX for a USART comunication.
  \param usrObjectUSART is a usart_t "object".
*/
static void vPublicDisableRX(usart_t* usrObjectUSART){
  vDisableRX(usrObjectUSART->ui8pUSARTGroup);
}

//! Public Method: TX Disabler
/*!
  Disables TX for a USART comunication.
  \param usrObjectUSART is a usart_t "object".
*/
static void vPublicDisableTX(usart_t* usrObjectUSART){
  vDisableTX(usrObjectUSART->ui8pUSARTGroup);
}

//! Public Method: USART Data Size Setter
/*!
  Sets USART send/receive data size.
  \param usrObjectUSART is a usart_t "object".
  \param ui8DataSize is a 8-bit unsigned integer. Its the size of USART data.
*/
static void vPublicSetDataSize(usart_t* usrObjectUSART, uint8_t ui8DataSize){
  vSetUSARTDataSize(usrObjectUSART->ui8pUSARTGroup, ui8DataSize);
}

//! Public Method: USART Asynchronous Mode Setter
/*!
  Configures an USART to asynchronous mode.
  \param usrObjectUSART is a usart_t "object".
*/
static void vPublicAsynchronousMode(usart_t* usrObjectUSART){
  vSetUSARTSync(usrObjectUSART->ui8pUSARTGroup, ASYNCHRONOUS);
}

//! Public Method: USART Synchronous Mode Setter
/*!
  Configures an USART to synchronous mode.
  \param usrObjectUSART is a usart_t "object".
*/
static void vPublicSynchronousMode(usart_t* usrObjectUSART){
  vSetUSARTSync(usrObjectUSART->ui8pUSARTGroup, SYNCHRONOUS);
}

//! Public Method: USART Master SPI Mode Setter
/*!
  Configures an USART to master SPI mode.
  \param usrObjectUSART is a usart_t "object".
*/
static void vPublicMasterSPIMode(usart_t* usrObjectUSART){
  vSetUSARTSync(usrObjectUSART->ui8pUSARTGroup, SPI_MASTER);
}

//! Public Method: USART No Parity Mode Setter
/*!
  Configures an USART to no parity mode.
  \param usrObjectUSART is a usart_t "object".
*/
static void vPublicNoParityMode(usart_t* usrObjectUSART){
  vSetUSARTParity(usrObjectUSART->ui8pUSARTGroup, NO_PARITY);
}

//! Public Method: USARTPair Setter
/*!
  Configures an USART to pair mode.
  \param usrObjectUSART is a usart_t "object".
*/
static void vPublicPairMode(usart_t* usrObjectUSART){
  vSetUSARTParity(usrObjectUSART->ui8pUSARTGroup, PAIR);
}

//! Public Method: USART Odd Setter
/*!
  Configures an USART to odd mode.
  \param usrObjectUSART is a usart_t "object".
*/
static void vPublicOddMode(usart_t* usrObjectUSART){
  vSetUSARTParity(usrObjectUSART->ui8pUSARTGroup, ODD);
}

//! Public Method: USART Falling Edge Mode Setter
/*!
  Configures an USART to falling edge mode.
  \param usrObjectUSART is a usart_t "object".
*/
static void vPublicFallingEdgeMode(usart_t* usrObjectUSART){
  vSetUSARTActivationEdge(usrObjectUSART->ui8pUSARTGroup, FALLING_EDGE_TX);
}

//! Public Method: USART Rising Edge Mode Setter
/*!
  Configures an USART to rising edge mode.
  \param usrObjectUSART is a usart_t "object".
*/
static void vPublicRisingEdgeMode(usart_t* usrObjectUSART){
  vSetUSARTActivationEdge(usrObjectUSART->ui8pUSARTGroup, RISING_EDGE_TX);
}

//! Public Method: USART Stop Bit Setter
/*!
  Sets USART stop bit.
  \param usrObjectUSART is a usart_t "object".
  \param ui8StopBit is a 8-bit unsigned integer. Its USART stop bit.
*/
static void vPublicSetStopBit(usart_t* usrObjectUSART, uint8_t ui8StopBit){
  vSetUSARTStopBit(usrObjectUSART->ui8pUSARTGroup, ui8StopBit);
}

//! Public Method: USART Baud Rate Setter
/*!
  Sets USART baud rate.
  \param usrObjectUSART is a usart_t "object".
  \param ui32BaudRate is a 32-bit integer. It's the USART baud rate.
*/
static void vPublicSetBaudRate(usart_t* usrObjectUSART, uint32_t ui32BaudRate){
  vSetUSARTBaudRate(usrObjectUSART->ui8pUSARTGroup, ui32BaudRate);
}

//! Public Method: USART Data Sender
/*!
  Sends USART data.
  \param usrObjectUSART is a usart_t "object".
  \param ui8Data is a 8-bit integer.
*/
static void vPublicWrite(usart_t* usrObjectUSART, const char cData){
  vUSARTSendByte(usrObjectUSART->ui8pUSARTGroup, cData);
}

//! Public Method: Receive USART Byte
/*!
  Gets received USART data.
  \param usrObjectUSART is a usart_t "object".
  \return Returns USART data.
*/
static uint8_t ui8PublicRead(usart_t* usrObjectUSART){
  return ui8USARTReceiveByte(usrObjectUSART->ui8pUSARTGroup);
}

//! Public Method: USART Printer
/*!
  Prints USART data (likes printf standard C function).
  \param usrObjectUSART is a usart_t "object".
  \param cpString is a 8-bit integer pointer. It's the string that will be sent.
*/
static void vPublicPrint(usart_t* usrObjectUSART, const char* cpString, ...){
  va_list vaArguments;
  va_start(vaArguments, cpString);
  vAttachPrintOutput((void*) usrObjectUSART->ui8pUSARTGroup, (void (*)(void*, uint8_t)) &vUSARTSendByte);
  vprintf(cpString, vaArguments);
  vDetachPrintOutput();
  va_end(vaArguments);
}

//! Public Method: USART Printer
/*!
  Prints USART data (likes printf standard C function with end line at string end).
  \param usrObjectUSART is a usart_t "object".
  \param cpString is a 8-bit integer pointer. It's the string that will be sent.
*/
static void vPublicPrintln(usart_t* usrObjectUSART, const char* cpString, ...){
  va_list vaArguments;
  va_start(vaArguments, cpString);
  vAttachPrintOutput((void*) usrObjectUSART->ui8pUSARTGroup, (void (*)(void*, uint8_t)) &vUSARTSendByte);
  vprintf(cpString, vaArguments);
  vDetachPrintOutput();
  vUSARTSendByte(usrObjectUSART->ui8pUSARTGroup, '\r');
  vUSARTSendByte(usrObjectUSART->ui8pUSARTGroup, '\n');
  va_end(vaArguments);
}

//! Function: USART RX Interrupt Attacher
/*!
  Attaches an USART RX interruption function.
  \param usrObjectUSART is a usart_t "object".
  \param vInterruptFunction is a function pointer. It's the callback interruption.
  \param vpArgument is a void pointer. It's the callback argument.
*/
static void vPublicAttachRXInterrupt(usart_t* usrObjectUSART, void (*vInterruptFunction)(void*), void* vpArgument){
  vAttachUSARTInterrupt(usrObjectUSART->ui8pUSARTGroup, RX_COMPLETE, vInterruptFunction, vpArgument);
}

//! Function: USART TX Interrupt Attacher
/*!
  Attaches an USART TX interruption function.
  \param usrObjectUSART is a usart_t "object".
  \param vInterruptFunction is a function pointer. It's the callback interruption.
  \param vpArgument is a void pointer. It's the callback argument.
*/
static void vPublicAttachTXInterrupt(usart_t* usrObjectUSART, void (*vInterruptFunction)(void*), void* vpArgument){
  vAttachUSARTInterrupt(usrObjectUSART->ui8pUSARTGroup, TX_COMPLETE, vInterruptFunction, vpArgument);
}

//! Function: USART Empty Interrupt Attacher
/*!
  Attaches an USART Empty interruption function.
  \param usrObjectUSART is a usart_t "object".
  \param vInterruptFunction is a function pointer. It's the callback interruption.
  \param vpArgument is a void pointer. It's the callback argument.
*/
static void vPublicAttachEmptyInterrupt(usart_t* usrObjectUSART, void (*vInterruptFunction)(void*), void* vpArgument){
  vAttachUSARTInterrupt(usrObjectUSART->ui8pUSARTGroup, DATA_EMPTY, vInterruptFunction, vpArgument);
}

//! Function: USART RX Interrupt Detacher
/*!
  Detaches USART RX interruption function.
  \param usrObjectUSART is a usart_t "object".
*/
static void vPublicDetachRXInterrupt(usart_t* usrObjectUSART){
  vDetachUSARTInterrupt(usrObjectUSART->ui8pUSARTGroup, RX_COMPLETE);
}

//! Function: USART TX Interrupt Detacher
/*!
  Detaches USART TX interruption function.
  \param usrObjectUSART is a usart_t "object".
*/
static void vPublicDetachTXInterrupt(usart_t* usrObjectUSART){
  vDetachUSARTInterrupt(usrObjectUSART->ui8pUSARTGroup, TX_COMPLETE);
}

//! Function: USART Empty Interrupt Detacher
/*!
  Detaches USART Empty interruption function.
  \param usrObjectUSART is a usart_t "object".
*/
static void vPublicDetachEmptyInterrupt(usart_t* usrObjectUSART){
  vDetachUSARTInterrupt(usrObjectUSART->ui8pUSARTGroup, DATA_EMPTY);
}

//! Function: USART RX Interrupt Enabler
/*!
  Enables USART RX interruption.
  \param usrObjectUSART is a usart_t "object".
*/
static void vPublicEnableRXInterrupt(usart_t* usrObjectUSART){
  vEnableUSARTInterrupt(usrObjectUSART->ui8pUSARTGroup, RX_COMPLETE);
}

//! Function: USART TX Interrupt Enabler
/*!
  Enables USART TX interruption.
  \param usrObjectUSART is a usart_t "object".
*/
static void vPublicEnableTXInterrupt(usart_t* usrObjectUSART){
  vEnableUSARTInterrupt(usrObjectUSART->ui8pUSARTGroup, TX_COMPLETE);
}

//! Function: USART Emtpy Interrupt Enabler
/*!
  Enables USART Emtpy interruption.
  \param usrObjectUSART is a usart_t "object".
*/
static void vPublicEnableEmptyInterrupt(usart_t* usrObjectUSART){
  vEnableUSARTInterrupt(usrObjectUSART->ui8pUSARTGroup, DATA_EMPTY);
}

//! Function: USART RX Interrupt Disabler
/*!
  Disables USART RX interruption.
  \param usrObjectUSART is a usart_t "object".
*/
static void vPublicDisableRXInterrupt(usart_t* usrObjectUSART){
  vDisableUSARTInterrupt(usrObjectUSART->ui8pUSARTGroup, RX_COMPLETE);
}

//! Function: USART TX Interrupt Disabler
/*!
  Disables USART TX interruption.
  \param usrObjectUSART is a usart_t "object".
*/
static void vPublicDisableTXInterrupt(usart_t* usrObjectUSART){
  vDisableUSARTInterrupt(usrObjectUSART->ui8pUSARTGroup, TX_COMPLETE);
}

//! Function: USART Empty Interrupt Disabler
/*!
  Disables USART Empty interruption.
  \param usrObjectUSART is a usart_t "object".
*/
static void vPublicDisableEmptyInterrupt(usart_t* usrObjectUSART){
  vDisableUSARTInterrupt(usrObjectUSART->ui8pUSARTGroup, DATA_EMPTY);
}
