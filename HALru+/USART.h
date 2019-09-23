//! winTerface Version 1.0b
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

#ifndef USART_Class_H
#define USART_Class_H

#ifdef __cplusplus
  extern "C" {
#endif

#include "HALru.h"
#include <avr/pgmspace.h>

//! Macro: gpio_t "object constructor"
/*!
  "Constructes" a gpio_t "object".
  \param ui8pGroup is a volatile 8-bit pointer integer. It's the USART group (USART_X).
  \return Returns the object.
*/
#define     NewUSART(ui8pUSARTGroup)                          {ui8pUSARTGroup}

//! Type Definition: usart_t
/*!
  This is a "class" of usart_t type.
*/
typedef struct {
  volatile uint8_t* ui8pUSARTGroup;                                                                     /*!< volatile uint8_t pointer type. */
} usart_t;

//! Type Definition: isr_usart_manager_t
/*!
  This is a "class" of isr_usart_manager_t type.
*/
typedef struct{
  void (*vAttach)(usart_t* usrObjectUSART, void (*vInterruptFunction)(void*), void* vpArgument);        /*!< void "method". */
  void (*vDetach)(usart_t* usrObjectUSART);                                                             /*!< void "method". */
  void (*vEnable)(usart_t* usrObjectUSART);                                                             /*!< void "method". */
  void (*vDisable)(usart_t* usrObjectUSART);                                                            /*!< void "method". */
} isr_usart_manager_t;

//! Type Definition: usart_configs_manager_t
/*!
  This is a "class" of usart_configs_manager_t type.
*/
typedef struct{
  void (*vSetDefaultMode)(usart_t* usrObjectUSART, uint32_t ui32BaudRate);                              /*!< void "method". */
  void (*vEnableRX)(usart_t* usrObjectUSART);                                                           /*!< void "method". */
  void (*vEnableTX)(usart_t* usrObjectUSART);                                                           /*!< void "method". */
  void (*vDisableRX)(usart_t* usrObjectUSART);                                                          /*!< void "method". */
  void (*vDisableTX)(usart_t* usrObjectUSART);                                                          /*!< void "method". */
  void (*vSetDataSize)(usart_t* usrObjectUSART, uint8_t ui8DataSize);                                   /*!< void "method". */
  void (*vSetAsynchronousMode)(usart_t* usrObjectUSART);                                                /*!< void "method". */
  void (*vSetSynchronousMode)(usart_t* usrObjectUSART);                                                 /*!< void "method". */
  void (*vSetMasterSPIMode)(usart_t* usrObjectUSART);                                                   /*!< void "method". */
  void (*vSetNoParityMode)(usart_t* usrObjectUSART);                                                    /*!< void "method". */
  void (*vSetPairMode)(usart_t* usrObjectUSART);                                                        /*!< void "method". */
  void (*vSetOddMode)(usart_t* usrObjectUSART);                                                         /*!< void "method". */
  void (*vSetFallingEdgeMode)(usart_t* usrObjectUSART);                                                 /*!< void "method". */
  void (*vSetRisingEdgeMode)(usart_t* usrObjectUSART);                                                  /*!< void "method". */
  void (*vSetStopBit)(usart_t* usrObjectUSART, uint8_t ui8StopBit);                                     /*!< void "method". */
  void (*vSetBaudRate)(usart_t* usrObjectUSART, uint32_t ui32BaudRate);                                 /*!< void "method". */
} usart_configs_manager_t;

//! Type Definition: usart_manager_t
/*!
  This is a "class" of usart_manager_t type.
*/
typedef struct {
  void (*vWrite)(usart_t* usrObjectUSART, const char cData);                                            /*!< void "method". */
  uint8_t (*ui8Read)(usart_t* usrObjectUSART);                                                          /*!< uint8_t "method". */
  void (*vPrint)(usart_t* usrObjectUSART, const char* cpString, ...);                                   /*!< void "method". */
  void (*vPrintln)(usart_t* usrObjectUSART, const char* cpString, ...);                                 /*!< void "method". */
  isr_usart_manager_t RXInterrupt;                                                                      /*!< RXInterrupt manager "object". */
  isr_usart_manager_t TXInterrupt;                                                                      /*!< TXInterrupt manager "object". */
  isr_usart_manager_t EmptyInterrupt;                                                                   /*!< EmptyInterrupt manager "object". */
  usart_configs_manager_t Configurations;                                                               /*!< Configurations manager "object". */
} usart_manager_t;

extern const usart_manager_t USART PROGMEM;                                                             /*!< USART manager "object". */

#ifdef __cplusplus
  }
#endif

#endif
