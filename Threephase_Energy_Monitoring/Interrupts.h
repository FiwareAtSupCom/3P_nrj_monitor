#include "ADE9000API.h"
#include "defined.h"

extern uint8_t EnergySamples;
extern ActivePowerRegs* ActivePower;
extern ReactivePowerRegs* ReactivePower;
extern ApparentPowerRegs* ApparentPower;

extern ActiveEnergyperH* ActiveEnergy;
extern ReactiveEnergyperH* ReactiveEnergy;

extern VoltageRMSRegs* VoltageRMS;
extern CurrentRMSRegs* CurrentRMS;
extern PowerFactorRegs* PowerFactor;
extern AngleRegs* Angle;
extern PeriodRegs* Frequency;

extern CurrentTHDRegs* CurrentTHD;
extern VoltageTHDRegs* VoltageTHD;

void IRAM_ATTR PowertimerISR() ;

void IRAM_ATTR EnergytimerISR() ;

void IRAM_ATTR CFtimerISR() ;

void IRAM_ATTR THDtimerISR() ;

void Interrupts_setup();

