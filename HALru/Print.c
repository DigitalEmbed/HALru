#include "Print.h"
#include <stdio.h>

print_t vStoredSendByteFunction = NULL;
volatile uint8_t* ui8pStoredOutput = NULL;

int iStoredSendByteFunction(char cChar, FILE *fpStream){
  if (vStoredSendByteFunction != NULL && ui8pStoredOutput != NULL){
    vStoredSendByteFunction(ui8pStoredOutput, cChar);
  }
  return 0;
}

void vAttachPrintOutput(volatile uint8_t* ui8pOutput, print_t vSendByteFunction){
  if (vStoredSendByteFunction == NULL && ui8pStoredOutput == NULL){
    fdevopen(&iStoredSendByteFunction, 0);
  }
  if (vStoredSendByteFunction == NULL && vSendByteFunction != NULL && ui8pOutput != NULL){
    vStoredSendByteFunction = vSendByteFunction;
    ui8pStoredOutput = ui8pOutput;
  }
}

void vDettachPrintOutput(void){
  vStoredSendByteFunction = NULL;
}
