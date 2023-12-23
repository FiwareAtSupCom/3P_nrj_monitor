#include "ADE9000API.h"
#include "defined.h"

void IRAM_ATTR PowertimerISR() ;

void IRAM_ATTR EnergytimerISR() ;

void IRAM_ATTR CFtimerISR() ;

void IRAM_ATTR THDtimerISR() ;

void Interrupts_setup();

