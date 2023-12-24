#include "Interrupts.h"

hw_timer_t * Powertimer = NULL; 
hw_timer_t * Energytimer = NULL;
hw_timer_t * CFtimer = NULL;
hw_timer_t * THDtimer = NULL;

volatile SemaphoreHandle_t powerSemaphore;
volatile SemaphoreHandle_t AccumulationSemaphore;
volatile SemaphoreHandle_t energySemaphore;
volatile SemaphoreHandle_t cfSemaphore;
volatile SemaphoreHandle_t THDSemaphore;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;


void IRAM_ATTR PowertimerISR() {
  
  portENTER_CRITICAL_ISR(&timerMux);
  
  ///////  POWER
  
  
  portEXIT_CRITICAL_ISR(&timerMux);
  xSemaphoreGiveFromISR(powerSemaphore, NULL);
  
}


void IRAM_ATTR EnergytimerISR() {

    portENTER_CRITICAL_ISR(&timerMux);
    EnergySamples++;
    if (EnergySamples*AccumulatedPowertimerFactor>=EnergytimerFactor){
    EnergySamples=0;
    xSemaphoreGiveFromISR(energySemaphore, NULL);
    }
    portEXIT_CRITICAL_ISR(&timerMux);
    xSemaphoreGiveFromISR(AccumulationSemaphore, NULL);
}

void IRAM_ATTR CFtimerISR() {
  
  portENTER_CRITICAL_ISR(&timerMux);
  
  
  portEXIT_CRITICAL_ISR(&timerMux);
  xSemaphoreGiveFromISR(cfSemaphore, NULL);
}

void IRAM_ATTR THDtimerISR() {
  
  portENTER_CRITICAL_ISR(&timerMux);

  
  
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
