#include "ADE9153AAPI.h"
#include "defined.h"

extern volatile uint8_t EnergySamples;

extern volatile PowerRegs* Power;
extern volatile EnergyRegs* Energy;

extern volatile RMSRegs* RMS;
extern volatile PQRegs* PQ;

void IRAM_ATTR PowertimerISR() ;

void IRAM_ATTR EnergytimerISR() ;

void IRAM_ATTR CFtimerISR() ;

void IRAM_ATTR THDtimerISR() ;

void Interrupts_setup();

