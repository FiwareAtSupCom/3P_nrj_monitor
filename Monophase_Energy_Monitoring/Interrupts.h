#include "ADE9153AAPI.h"
#include "defined.h"

extern uint8_t EnergySamples;

extern PowerRegs* Power;
extern EnergyRegs* Energy;

extern RMSRegs* RMS;
extern PQRegs* PQ;

void IRAM_ATTR PowertimerISR() ;

void IRAM_ATTR EnergytimerISR() ;

void IRAM_ATTR CFtimerISR() ;

void IRAM_ATTR THDtimerISR() ;

void Interrupts_setup();

