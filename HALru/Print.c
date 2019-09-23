#include "Print.h"
#include <stdio.h>

print_t vStoredSendByteFunction = NULL;                             /*!< print_t pointer type. */
volatile uint8_t* ui8pStoredOutput = NULL;                          /*!< volatile 8-bit integer pointer type. */

int iStoredSendByteFunction(char cChar, FILE *fpStream);            /*!< void type function. */

//! Function: Print Output Function Attacher
/*!
  Attaches a function to use with printf.
  \param ui8pOutput is a volatile 8-bit pointer integer. It's the hardware communication group (USART_X, SPI_X, I2C_X).
  \param vSendByteFunction is a print_t. It's the send-a-byte address function for hardware communication group.
*/
void vAttachPrintOutput(volatile uint8_t* ui8pOutput, print_t vSendByteFunction){
  if (vStoredSendByteFunction == NULL && ui8pStoredOutput == NULL){
    fdevopen(&iStoredSendByteFunction, 0);
  }
  if (vStoredSendByteFunction == NULL && vSendByteFunction != NULL && ui8pOutput != NULL){
    vStoredSendByteFunction = vSendByteFunction;
    ui8pStoredOutput = ui8pOutput;
  }
}

//! Function: Print Output Function Detacher
/*!
  Detaches a function of printf.
*/
void vDetachPrintOutput(void){
  vStoredSendByteFunction = NULL;
}

//! Function: Printf Callback
/*!
  Printf callback function.
  \param cChar is a char. It's the character that will be sent.
  \param fpStream is a FILE pointer.
*/
int iStoredSendByteFunction(char cChar, FILE *fpStream){
  if (vStoredSendByteFunction != NULL && ui8pStoredOutput != NULL){
    vStoredSendByteFunction(ui8pStoredOutput, cChar);
  }
  return 0;
}
