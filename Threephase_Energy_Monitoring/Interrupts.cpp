#include "Interrupts.h"

hw_timer_t * Powertimer = NULL; 
hw_timer_t * Energytimer = NULL;
hw_timer_t * CFtimer = NULL;
hw_timer_t * THDtimer = NULL;

volatile SemaphoreHandle_t powerSemaphore;
volatile SemaphoreHandle_t energySemaphore;
volatile SemaphoreHandle_t cfSemaphore;
volatile SemaphoreHandle_t THDSemaphore;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

extern volatile uint8_t EnergySamples;
extern volatile ActivePowerRegs* ActivePower;
extern volatile ReactivePowerRegs* ReactivePower;
extern volatile ApparentPowerRegs* ApparentPower;

extern volatile ActiveEnergyperH* ActiveEnergy;
extern volatile ReactiveEnergyperH* ReactiveEnergy;

extern volatile VoltageRMSRegs* VoltageRMS;
extern volatile CurrentRMSRegs* CurrentRMS;
extern volatile PowerFactorRegs* PowerFactor;
extern volatile PeriodRegs* Frequency;

extern volatile CurrentTHDRegs* CurrentTHD;
extern volatile VoltageTHDRegs* VoltageTHD;



void IRAM_ATTR PowertimerISR() {
  
  portENTER_CRITICAL_ISR(&timerMux);
  
  ///////  POWER
  ade9000.ReadActivePowerRegs(ActivePower);
  ade9000.ReadReactivePowerRegs(ReactivePower);
  ade9000.ReadApparentPowerRegs(ApparentPower);
  
  portEXIT_CRITICAL_ISR(&timerMux);
  xSemaphoreGiveFromISR(powerSemaphore, NULL);
  
}


void IRAM_ATTR EnergytimerISR() {

    portENTER_CRITICAL_ISR(&timerMux);
    ///////  ENERGY  Read energy register with reset mode page(21)
    ade9000.AccumulateActiveEnergy(ActiveEnergy);
    ade9000.AccumulateReactiveEnergy(ReactiveEnergy);
    EnergySamples++;
    if (EnergySamples*AccumulatedPowertimerFactor>=EnergytimerFactor){
    EnergySamples=0;
    xSemaphoreGiveFromISR(energySemaphore, NULL);
    }
    portEXIT_CRITICAL_ISR(&timerMux);
}

void IRAM_ATTR CFtimerISR() {
  
  portENTER_CRITICAL_ISR(&timerMux);
  
  ade9000.ReadCurrentRMSRegs(CurrentRMS);
  ade9000.ReadVoltageRMSRegs(VoltageRMS);
  ade9000.ReadPowerFactorRegsnValues(PowerFactor);
  ade9000.ReadPeriodRegsnValues(Frequency);
  
  portEXIT_CRITICAL_ISR(&timerMux);
  xSemaphoreGiveFromISR(cfSemaphore, NULL);
}

void IRAM_ATTR THDtimerISR() {
  
  portENTER_CRITICAL_ISR(&timerMux);

  ade9000.ReadVoltageTHDRegsnValues(VoltageTHD);
  ade9000.ReadCurrentTHDRegsnValues(CurrentTHD);
  
  portEXIT_CRITICAL_ISR(&timerMux);
  xSemaphoreGiveFromISR(THDSemaphore, NULL);
  
}

void Interrupts_setup(){
  // Timer 1: 1 second interval
  Powertimer = timerBegin(0, 80, true);                //Begin timer with 1 MHz frequency (80MHz/80) = 1 microsecond
  timerAttachInterrupt(Powertimer, &PowertimerISR, true);   //Attach the interrupt to Timer1
  timerAlarmWrite(Powertimer, PowertimerFactor, true);      //Initialize the timer
  timerAlarmEnable(Powertimer); 

  // Timer 2: 106s interval
  Energytimer = timerBegin(1, 80, true);                //Begin timer with 1 MHz frequency (80MHz/80)
  timerAttachInterrupt(Energytimer, &EnergytimerISR, true);   //Attach the interrupt to Timer1
  timerAlarmWrite(Energytimer, AccumulatedPowertimerFactor, true);      //Initialize the timer
  timerAlarmEnable(Energytimer); 

   // Timer 3: 15 minutes interval
  CFtimer = timerBegin(2, 80, true);                //Begin timer with 1 MHz frequency (80MHz/80) = 1 microsecond
  timerAttachInterrupt(CFtimer, &CFtimerISR, true);   //Attach the interrupt to Timer1
  timerAlarmWrite(CFtimer, VCFtimerFactor, true);      //Initialize the timer
  timerAlarmEnable(CFtimer); 

  // Timer 4: 15 minutes interval
  THDtimer = timerBegin(3, 80, true);                //Begin timer with 1 MHz frequency (80MHz/80) = 1 microsecond
  timerAttachInterrupt(THDtimer, &THDtimerISR, true);   //Attach the interrupt to Timer1
  timerAlarmWrite(THDtimer, THDtimerFactor, true);      //Initialize the timer
  timerAlarmEnable(THDtimer); 
}
