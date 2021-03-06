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

#ifndef USART_H
#define USART_H

#ifdef __cplusplus
  extern "C" {
#endif

#include <stdint.h>
#include <avr/io.h>
#include "./Configs.h"

//! Macro: USART Macros
/*!
  These macros are for facilitate the use of this library.
*/
#define     USART_0                                           &UCSR0A

#if defined(ARDUINO_AVR_MEGA2560) || defined(ARDUINO_AVR_MEGA)
  #define     USART_1                                         &UCSR1A
  #define     USART_2                                         &UCSR2A
  #define     USART_3                                         &UCSR3A
#endif

#define     regUCSRB(usrGroup)                                ((volatile uint8_t*) usrGroup + 1)
#define     regUCSRC(usrGroup)                                ((volatile uint8_t*) usrGroup + 2)
#define     regUDR(usrGroup)                                  ((volatile uint8_t*) usrGroup + 6)

#define     MPCMn                                             0
#define     U2Xn                                              1
#define     UPEn                                              2
#define     DORn                                              3
#define     FEn                                               4
#define     UDREn                                             5
#define     TXCn                                              6
#define     RXCn                                              7

#define     TXB8n                                             0
#define     RXB8n                                             1
#define     UCSZn2                                            2
#define     TXENn                                             3
#define     RXENn                                             4
#define     UDRIEn                                            5
#define     TXCIEn                                            6
#define     RXCIEn                                            7

#define     UCPOLn                                            0
#define     UCSZn0                                            1
#define     UCSZn1                                            2
#define     USBSn                                             3
#define     UPMn0                                             4
#define     UPMn1                                             5
#define     UMSELn0                                           6
#define     UMSELn1                                           7

#define     ASYNCHRONOUS                                      0
#define     SYNCHRONOUS                                       1
#define     SPI_MASTER                                        3

#define     RISING_EDGE_TX                                    0
#define     FALLING_EDGE_RX                                   0
#define     RISING_EDGE_RX                                    1
#define     FALLING_EDGE_TX                                   1

#define     NO_PARITY                                         0
#define     PAIR                                              2
#define     ODD                                               3

#define     SERIAL_INITIALIZED                                1
#define     SERIAL_NOT_INITIALIZED                            0

#define     RX_COMPLETE                                       0
#define     TX_COMPLETE                                       1
#define     DATA_EMPTY                                        2

//! Macro: Enable RX
/*!
  Enables RX for an USART comunication.
  \param usrGroup is an undefined parameter. It's the gpio group pin (USART_X).
*/
#define     vEnableRX(usrGroup)                               vSetBit(*regUCSRB(usrGroup), RXENn)

//! Macro: Disable RX
/*!
  Disables RX for an USART comunication.
  \param usrGroup is an undefined parameter. It's the gpio group pin (USART_X).
*/
#define     vDisableRX(usrGroup)                              vEraseBit(*regUCSRB(usrGroup), RXENn)

//! Macro: Enable TX
/*!
  Enables TX for an USART comunication.
  \param usrGroup is an undefined parameter. It's the gpio group pin (USART_X).
*/
#define     vEnableTX(usrGroup)                               vSetBit(*regUCSRB(usrGroup), TXENn)

//! Macro: Disable TX
/*!
  Disables TX for an USART comunication.
  \param usrGroup is an undefined parameter. It's the gpio group pin (USART_X).
*/
#define     vDisableTX(usrGroup)                              vEraseBit(*regUCSRB(usrGroup), TXENn)

//! Macro: Set USART Data Size
/*!
  Sets size of USART send/receive data.
  \param usrGroup is an undefined parameter. It's the gpio group pin (USART_X).
  \param ui8DataSize is an undefined parameter. Its the size of USART data.
*/
#define     vSetUSARTDataSize(usrGroup, ui8DataSize)          if (ui8DataSize > 4 && ui8DataSize < 9) {\
                                                                vEraseBit(*regUCSRB(usrGroup), UCSZn2);\
                                                                vSetBits(*regUCSRC(usrGroup), UCSZn0, (ui8DataSize - 5));\
                                                              }\
                                                              else {\
                                                                vSetBit(*regUCSRB(usrGroup), UCSZn2);\
                                                                vSetBits(*regUCSRC(usrGroup), UCSZn0, 3);\
                                                              }

//! Macro: Set USART Parity
/*!
  Sets parity of USART.
  \param usrGroup is an undefined parameter. It's the gpio group pin (USART_X).
  \param ui8Parity is an undefined parameter. Its USART parity.
*/
#define     vSetUSARTParity(usrGroup, ui8Parity)              if (ui8Parity == NO_PARITY || ui8Parity == PAIR || ui8Parity == ODD) {\
                                                                vSetBits(*regUCSRC(usrGroup), UPMn0, ui8Parity);\
                                                              }

//! Macro: Set USART Sync
/*!
  Sets sync of USART.
  \param usrGroup is an undefined parameter. It's the gpio group pin (USART_X).
  \param ui8Sync is an undefined parameter. Its the USART sync.
*/
#define     vSetUSARTSync(usrGroup, ui8Sync)                  if (ui8Sync == SYNCHRONOUS || ui8Sync == ASYNCHRONOUS || ui8Sync == SPI_MASTER) {\
                                                                vSetBits(*regUCSRC(usrGroup), UMSELn0, ui8Sync);\
                                                              }

//! Macro: Set USART Stop Bit
/*!
  Sets stop bit of USART.
  \param usrGroup is an undefined parameter. It's the gpio group pin (USART_X).
  \param ui8StopBit is an undefined parameter. Its USART stop bit.
*/
#define     vSetUSARTStopBit(usrGroup, ui8StopBit)            if (ui8StopBit == 2) {\
                                                                vSetBit(*regUCSRC(usrGroup), USBSn);\
                                                              }\
                                                              else {\
                                                                vEraseBit(*regUCSRC(usrGroup), USBSn);\
                                                              }

//! Macro: Set USART Activatio Edge
/*!
  Sets activation edge of USART.
  \param usrGroup is an undefined parameter. It's the gpio group pin (USART_X).
  \param ui8Edge is an undefined parameter. Its USART activation edge.
*/
#define     vSetUSARTActivationEdge(usrGroup, ui8Edge)        if (((*regUCSRC(usrGroup) >> UMSELn0) & 3) == ASYNCHRONOUS) {\
                                                                vEraseBit(*regUCSRC(usrGroup), UCPOLn);\
                                                              }\
                                                              else {\
                                                                if (ui8Edge == RISING_EDGE_TX){\
                                                                  vEraseBit(*regUCSRC(usrGroup), UCPOLn);\
                                                                }\
                                                                else{\
                                                                  vSetBit(*regUCSRC(usrGroup), UCPOLn);\
                                                                }\
                                                              }

//! Macro: Receive USART Byte
/*!
  Gets received USART data.
  \param usrGroup is an undefined parameter. It's the gpio group pin (USART_X).
  \return Returns USART data.
*/
#define     ui8USARTReceiveByte(usrGroup)                     *regUDR(usrGroup)

void vUSARTInit(volatile uint8_t* ui8pGroup);                                                                                                 /*!< void type function. */
void vEnableUSARTInterrupt(volatile uint8_t* ui8pGroup, uint8_t ui8InterruptionType);                                                         /*!< void type function. */
void vDisableUSARTInterrupt(volatile uint8_t* ui8pGroup, uint8_t ui8InterruptionType);                                                        /*!< void type function. */
void vSetUSARTBaudRate(volatile uint8_t* ui8pGroup, uint32_t ui32BaudRate);                                                                   /*!< void type function. */
void vUSARTSendByte(volatile uint8_t* ui8pGroup, uint8_t ui8Data);                                                                            /*!< void type function. */
void vAttachUSARTInterrupt(volatile uint8_t* ui8pGroup, uint8_t ui8InterruptionType, void (*vInterruptFunction)(void*), void* vpArgument);    /*!< void type function. */
void vDetachUSARTInterrupt(volatile uint8_t* ui8pGroup, uint8_t ui8InterruptionType);                                                         /*!< void type function. */

#ifdef __cplusplus
  }
#endif

#endif
