//! HALru Version 1.0b
/*!
  This code file was written by Jorge Henrique Moreira Santana and is under
  the GNU GPLv3 license. All legal rights are reserved.

  Permissions of this copyleft license are conditioned on making available
  complete source code of licensed works and modifications under the same
  license or the GNU GPLv3. Copyright and license notices must be preserved.
  Contributors provide an express grant of patent rights. However, a larger
  work using the licensed work through interfaces provided by the licensed
  work may be distributed under different terms and without source code for
  the larger work.

  * Permissions:
    -> Commercial use;
    -> Modification;
    -> Distribuition;
    -> Patent Use;
    -> Private Use;

  * Limitations:
    -> Liability;
    -> Warranty;

  * Conditions:
    -> License and copyright notice;
    -> Disclose source;
    -> State changes;
    -> Same license (library);

  For more informations, check the LICENSE document. If you want to use a
  commercial product without having to provide the source code, send an email
  to jorge_henrique_123@hotmail.com to talk.
*/

#ifndef Nested_H
#define Nested_H

#ifdef __cplusplus
  extern "C" {
#endif

#include "HALru.h"

//! Macro: Nested Macros
/*!
  This macros are for facilitate the use of this library.
*/
#define   NestedMode                          {\
                                                vEnableAllInterrupts();\
                                                static uint8_t ui8FlagNestedModeActived = 1;\
                                                if (ui8FlagNestedModeActived != 1){\
                                                  return;\
                                                }\
                                                ui8FlagNestedModeActived = 0;\
                                                {

#define   EndNestedMode                         }\
                                                ui8FlagNestedModeActived = 1;\
                                                vDisableAllInterrupts();\
                                              }

#ifdef __cplusplus
  }
#endif

#endif
