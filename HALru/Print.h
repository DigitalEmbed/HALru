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

#ifndef Print_H
#define Print_H

#ifdef __cplusplus
  extern "C" {
#endif

#include "HALru.h"

//! Macro: Float Numbers
/*!
  This macro is to use float types in print functions on AVR microcontrolers.
*/
#define ftoa(fNumber)   (uint16_t)(fNumber), (uint16_t)(100*(fNumber))%100
#define dtoa(fNumber)   (uint16_t)(fNumber), (uint16_t)(10000*(fNumber))%10000    

//! Type Definition: print_t
/*!
  This typedef exist for organization purpose.
*/
typedef void (*print_t)(volatile uint8_t*, uint8_t);

void vAttachPrintOutput(volatile uint8_t* ui8pOutput, print_t vSendByteFunction);     /*!< Void type function. */
void vDettachPrintOutput(void);                                                       /*!< Void type function. */

#ifdef __cplusplus
  }
#endif

#endif
