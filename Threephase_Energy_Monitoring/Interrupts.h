#include "ADE9000API.h"
#include "defined.h"

extern volatile uint8_t EnergySamples;
extern volatile ActivePowerRegs* ActivePower;
extern volatile ReactivePowerRegs* ReactivePower;
extern volatile ApparentPowerRegs* ApparentPower;

extern volatile ActiveEnergyperH* ActiveEnergy;
extern volatile ReactiveEnergyperH* ReactiveEnergy;

extern volatile VoltageRMSRegs* VoltageRMS;
extern volatile CurrentRMSRegs* CurrentRMS;
extern volatile PowerFactorRegs* PowerFactor;
extern volatile AngleRegs* Angle;
extern volatile PeriodRegs* Frequency;

extern volatile CurrentTHDRegs* CurrentTHD;
extern volatile VoltageTHDRegs* VoltageTHD;

void IRAM_ATTR PowertimerISR() ;

void IRAM_ATTR EnergytimerISR() ;

void IRAM_ATTR CFtimerISR() ;

void IRAM_ATTR THDtimerISR() ;

void Interrupts_setup();

