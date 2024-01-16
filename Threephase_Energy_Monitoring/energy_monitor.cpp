#include "energy_monitor.h"


#define EEPROM_SIZE 512

#define CounterA_add 0

#define CounterB_add 4

#define CounterC_add 8

#define MeasureStartTime_add 12


volatile ActivePowerRegs* ActivePower;
volatile ReactivePowerRegs* ReactivePower;
volatile ApparentPowerRegs* ApparentPower;

volatile ActiveEnergyperH* ActiveEnergy;
volatile ReactiveEnergyperH* ReactiveEnergy;

volatile VoltageRMSRegs* VoltageRMS;
volatile CurrentRMSRegs* CurrentRMS;
volatile PowerFactorRegs* PowerFactor;
volatile AngleRegs* Angle;
volatile PeriodRegs* Frequency;

volatile CurrentTHDRegs* CurrentTHD;
volatile VoltageTHDRegs* VoltageTHD;

volatile uint8_t EnergySamples;

const char root_Topic[]="/ul/EnergyMonitoring/monitor001/attrs";


EnergyMonitorClass::EnergyMonitorClass()
{
	
}


void EnergyMonitorClass::PublishdateObserved(char* sub_Topic){

  char buf[21];
  char topic[100];
    strftime(buf, 21, "%Y-%m-%dT%H:%M:%SZ", &timeinfo);
    snprintf(topic, sizeof(topic), "%s|%s", sub_Topic,buf);
    mqttClient.beginMessage(root_Topic);
    mqttClient.print(topic);
    mqttClient.endMessage();
}

void EnergyMonitorClass::PublishphaseType(char* sub_Topic,char* type){

    char topic[100];
    snprintf(topic, sizeof(topic), "%s|%s", sub_Topic,type);
    mqttClient.beginMessage(root_Topic);
    mqttClient.print(topic);
    mqttClient.endMessage();
}



void EnergyMonitorClass::PublishActivePower(char* sub_Topic){

    char topic[100];
    snprintf(topic, sizeof(topic), "%s|{L1:%f, L2:%f, L3: %f}", sub_Topic,(ActivePower->ActivePowerReg_A)*PowerConversionConstant,(ActivePower->ActivePowerReg_B)*PowerConversionConstant,(ActivePower->ActivePowerReg_C)*PowerConversionConstant);
    if (WiFi.status() == WL_CONNECTED){
    mqttClient.beginMessage(root_Topic);
    mqttClient.print(topic);
    mqttClient.endMessage();
    PublishtotalActivePower("e13");
    PublishdateObserved("e23");
    }else{
      myQueue.push(topic);
    }
}

void EnergyMonitorClass::PublishtotalActivePower(char* sub_Topic){

  char topic[100];
  snprintf(topic, sizeof(topic), "%s|%f", sub_Topic,((ActivePower->ActivePowerReg_A)+(ActivePower->ActivePowerReg_B)+(ActivePower->ActivePowerReg_C))*PowerConversionConstant);
  mqttClient.beginMessage(root_Topic);
  mqttClient.print(topic);
  mqttClient.endMessage();
  
}

void EnergyMonitorClass::PublishReactivePower(char* sub_Topic){

    char topic[100];
    snprintf(topic, sizeof(topic), "%s|{L1:%f, L2:%f, L3:%f}", sub_Topic,(ReactivePower->ReactivePowerReg_A)*PowerConversionConstant,(ReactivePower->ReactivePowerReg_B)*PowerConversionConstant,(ReactivePower->ReactivePowerReg_C)*PowerConversionConstant);
    if (WiFi.status() == WL_CONNECTED){
    mqttClient.beginMessage(root_Topic);
    mqttClient.print(topic);
    mqttClient.endMessage();
    PublishtotalReactivePower("e14");
    PublishdateObserved("e23");
    }else{
      myQueue.push(topic);
    }
}

void EnergyMonitorClass::PublishtotalReactivePower(char* sub_Topic){

  char topic[100];
  snprintf(topic, sizeof(topic), "%s|%f", sub_Topic, ((ReactivePower->ReactivePowerReg_A)+(ReactivePower->ReactivePowerReg_B)+(ReactivePower->ReactivePowerReg_C))*PowerConversionConstant);
  mqttClient.beginMessage(root_Topic);
  mqttClient.print(topic);
  mqttClient.endMessage();
  
}

void EnergyMonitorClass::PublishApparentPower(char* sub_Topic){
    char topic[100];
    snprintf(topic, sizeof(topic), "%s|{L1:%f, L2:%f, L3:%f}", sub_Topic,(ApparentPower->ApparentPowerReg_A)*PowerConversionConstant,(ApparentPower->ApparentPowerReg_B)*PowerConversionConstant,(ApparentPower->ApparentPowerReg_C)*PowerConversionConstant);
    if (WiFi.status() == WL_CONNECTED){
    mqttClient.beginMessage(root_Topic);
    mqttClient.print(topic);
    mqttClient.endMessage();
    PublishtotalApparentPower("e15");
    PublishdateObserved("e23");
    }else{
      myQueue.push(topic);
    }
}

void EnergyMonitorClass::PublishtotalApparentPower(char* sub_Topic){

  char topic[100];
  snprintf(topic, sizeof(topic), "%s|%f", sub_Topic,((ApparentPower->ApparentPowerReg_A)+(ApparentPower->ApparentPowerReg_B)+(ApparentPower->ApparentPowerReg_C))*PowerConversionConstant);
  mqttClient.beginMessage(root_Topic);
  mqttClient.print(topic);
  mqttClient.endMessage();
  
}



void EnergyMonitorClass::PublishpowerFactor(char* sub_Topic){

    char topic[100];
    snprintf(topic, sizeof(topic), "%s|{L1:%f, L2:%f, L3:%f}", sub_Topic,(PowerFactor->PowerFactorValue_A),(PowerFactor->PowerFactorValue_B),(PowerFactor->PowerFactorValue_C));
    if (WiFi.status() == WL_CONNECTED){
    mqttClient.beginMessage(root_Topic);
    mqttClient.print(topic);
    mqttClient.endMessage();
    PublishdateObserved("e23");
    }else{
      myQueue.push(topic);
    }
}

void EnergyMonitorClass::PublishdisplacementpowerFactor(char* sub_Topic){

    char topic[100];
    snprintf(topic, sizeof(topic), "%s|{L1:%f, L2:%f, L3:%f}", sub_Topic, cos(Angle->AngleValue_VA_IA),cos(Angle->AngleValue_VB_IB),cos(Angle->AngleValue_VC_IC));
    if (WiFi.status() == WL_CONNECTED){
    mqttClient.beginMessage(root_Topic);
    mqttClient.print(root_Topic);
    mqttClient.endMessage();
    PublishdateObserved("e23");
    }else{
      myQueue.push(topic);
    }
    
}

void EnergyMonitorClass::Publishvoltage(char* sub_Topic){
    //Phase voltage
    char topic[100];
    snprintf(topic, sizeof(topic), "%s|{L1:%f, L2:%f, L3:%f}", sub_Topic,(VoltageRMS->VoltageRMSReg_A)*VoltageConversionConstant,(VoltageRMS->VoltageRMSReg_B)*VoltageConversionConstant,(VoltageRMS->VoltageRMSReg_C)*VoltageConversionConstant);
    if (WiFi.status() == WL_CONNECTED){
    mqttClient.beginMessage(root_Topic);
    mqttClient.print(topic);
    mqttClient.endMessage();
    PublishdateObserved("e23");
    }else{
      myQueue.push(topic);
    }
}


void EnergyMonitorClass::Publishcurrent(char* sub_Topic){
    char topic[100];
    snprintf(topic, sizeof(topic), "%s|{L1:%f, L2:%f, L3:%f}", sub_Topic,(CurrentRMS->CurrentRMSReg_A)*CurrentConversionConstant,(CurrentRMS->CurrentRMSReg_B)*CurrentConversionConstant,(CurrentRMS->CurrentRMSReg_C)*CurrentConversionConstant);
    if (WiFi.status() == WL_CONNECTED){
    mqttClient.beginMessage(root_Topic);
    mqttClient.print(topic);
    mqttClient.endMessage();
    PublishdateObserved("e23");
    }else{
      myQueue.push(topic);
    }
}

void EnergyMonitorClass::PublishvoltagePhaseToPhase(char* sub_Topic){
    //Line voltage
    char topic[100];
    snprintf(topic, sizeof(topic), "%s|{L12: %f, L23: %f, L31: %f}", sub_Topic,((VoltageRMS->VoltageRMSReg_A)-(VoltageRMS->VoltageRMSReg_B))*VoltageConversionConstant,((VoltageRMS->VoltageRMSReg_B)-(VoltageRMS->VoltageRMSReg_C))*VoltageConversionConstant,((VoltageRMS->VoltageRMSReg_C)-(VoltageRMS->VoltageRMSReg_A))*VoltageConversionConstant);
    if (WiFi.status() == WL_CONNECTED){
    mqttClient.beginMessage(root_Topic);
    mqttClient.print(topic);
    mqttClient.endMessage();
    PublishdateObserved("e23");
    }else{
      myQueue.push(topic);
    }
}

void EnergyMonitorClass::Publishfrequency(char* sub_Topic){
 
  char topic[100];
  float freq;
  if (abs((Frequency->FrequencyValue_A)-50)>=abs((Frequency->FrequencyValue_B)-50)){
    if (abs((Frequency->FrequencyValue_A)-50)>=abs((Frequency->FrequencyValue_C)-50)){
        freq=Frequency->FrequencyValue_A;
    }else{
      freq=Frequency->FrequencyValue_C;
    }
  }else{
      freq=Frequency->FrequencyValue_B;
  }
   snprintf(topic, sizeof(topic), "%s|%f", sub_Topic,freq);
   if (WiFi.status() == WL_CONNECTED){
  mqttClient.beginMessage(root_Topic);
  mqttClient.print(topic);
  mqttClient.endMessage();
  PublishdateObserved("e23");
  }else{
      myQueue.push(topic);
    }
}

void EnergyMonitorClass::PublishvoltageTHD(char* sub_Topic){

    char topic[100];
    snprintf(topic, sizeof(topic), "%s|{L1: %f, L2: %f, L3: %f}", sub_Topic,(VoltageTHD->VoltageTHDValue_A)*VoltageConversionConstant,(VoltageTHD->VoltageTHDValue_B)*VoltageConversionConstant,(VoltageTHD->VoltageTHDValue_C)*VoltageConversionConstant);
    if (WiFi.status() == WL_CONNECTED){
    mqttClient.beginMessage(root_Topic);
    mqttClient.print(topic);
    mqttClient.endMessage();
    PublishdateObserved("e23");
    }else{
      myQueue.push(topic);
    }
}

void EnergyMonitorClass::PublishcurrentTHD(char* sub_Topic){

    char topic[100];
    snprintf(topic, sizeof(topic), "%s|{L1: %f, L2: %f, L3: %f}", sub_Topic,(CurrentTHD->CurrentTHDValue_A)*CurrentConversionConstant,(CurrentTHD->CurrentTHDValue_B)*CurrentConversionConstant,(CurrentTHD->CurrentTHDValue_C)*CurrentConversionConstant);
    if (WiFi.status() == WL_CONNECTED){
    mqttClient.beginMessage(root_Topic);
    mqttClient.print(root_Topic);
    mqttClient.endMessage();
    PublishdateObserved("e23");
    }else{
     myQueue.push(topic);
    }
}

void EnergyMonitorClass::handleWifiStatus(wl_status_t* WifiStatus){
    if (WiFi.status()!= *WifiStatus){
    if (WiFi.status() != WL_CONNECTED){
    digitalWrite(2, HIGH);
    Change_timers_config(timerFactorChanger);
    //reconnect to WIFI
    WiFi.reconnect();
    delay(100);
  
  }else{
    digitalWrite(2, LOW);
    checkBroker();
    send_data(myQueue);
    Change_timers_config(1);
  }
  *WifiStatus=WiFi.status();
  }
}

void EnergyMonitorClass::checkBroker(){
  if (!mqttClient.connected()){
    mqttClient.connect(broker,port);
  }
}

void EnergyMonitorClass::InitEnergyCounter(){
  if(MeasureStartTime()=="NOW"){
  EEPROM.writeFloat(CounterA_add,0);
  EEPROM.commit();
  EEPROM.writeFloat(CounterB_add,0);
  EEPROM.commit();
  EEPROM.writeFloat(CounterC_add,0);
  EEPROM.commit();
  }
}

void EnergyMonitorClass::StoreCountedEnergy(){
  float_t CounterA=EEPROM.readFloat(CounterA_add);
  EEPROM.writeFloat(CounterA_add,CounterA+((ActiveEnergy->ActiveEnergy_A)*EnergyConversionConstant));
  EEPROM.commit();
  float_t CounterB=EEPROM.readFloat(CounterB_add);
  EEPROM.writeFloat(CounterB_add,CounterB+((ActiveEnergy->ActiveEnergy_B)*EnergyConversionConstant));
  EEPROM.commit();
  float_t CounterC=EEPROM.readFloat(CounterC_add);
  EEPROM.writeFloat(CounterC_add,CounterC+((ActiveEnergy->ActiveEnergy_C)*EnergyConversionConstant));
  EEPROM.commit();
}

float_t EnergyMonitorClass::ReadEnergyCounter(uint8_t phase){

  if (phase==1){
    return(EEPROM.readFloat(CounterA_add));
  }else if(phase==2){
    return(EEPROM.readFloat(CounterB_add));
  }else if (phase==3){
    return(EEPROM.readFloat(CounterC_add));
  }
}




void EnergyMonitorClass::send_data(std::queue<String>& myQueue){
  char topic[100];
  float factor;
  String str;
  while (!myQueue.empty()) {
   str=myQueue.front();
   Serial.println("///////STORED////////");   
    mqttClient.beginMessage(root_Topic);
    mqttClient.print(str);
    mqttClient.endMessage();
   myQueue.pop();
   Serial.println("[APP] Free memory: " + String(esp_get_free_heap_size()) + " bytes");
  }
  
}

void EnergyMonitorClass::Change_timers_config(unsigned int Factor){
  // Detach the existing ISR
  timerDetachInterrupt(Powertimer);
  timerAttachInterrupt(Powertimer, &PowertimerISR, true);
  timerAlarmWrite(Powertimer, Factor*PowertimerFactor, true);      
  timerAlarmEnable(Powertimer);

  timerDetachInterrupt(Energytimer);
  timerAttachInterrupt(Energytimer, &EnergytimerISR, true);
  timerAlarmWrite(Energytimer, Factor*AccumulatedPowertimerFactor, true);      
  timerAlarmEnable(Energytimer);

  timerDetachInterrupt(CFtimer);
  timerAttachInterrupt(CFtimer, &CFtimerISR, true);
  timerAlarmWrite(CFtimer, Factor*VCFtimerFactor, true);      
  timerAlarmEnable(CFtimer);

  timerDetachInterrupt(THDtimer);
  timerAttachInterrupt(THDtimer, &THDtimerISR, true);
  timerAlarmWrite(THDtimer, Factor*THDtimerFactor, true);      
  timerAlarmEnable(THDtimer);
}

String EnergyMonitorClass::MeasureStartTime(){
  String Value=EEPROM.readString(MeasureStartTime_add);
  if(!(Value.charAt(Value.length()-1) == 'Z')){
    get_Time(&timeinfo);
    char str[21];
    strftime(str, 21, "%Y-%m-%dT%H:%M:%SZ", &timeinfo);
    EEPROM.writeString(MeasureStartTime_add,str);
    return "NOW";
  }
  return Value;
}

void EnergyMonitorClass::energy_meter_setup()
{

  EnergySamples=0;

  ActivePower= new ActivePowerRegs;
  ReactivePower= new ReactivePowerRegs;
  ApparentPower= new ApparentPowerRegs;

  ActiveEnergy= new ActiveEnergyperH;
  ade9000.InitActiveEnergy(ActiveEnergy);

  ReactiveEnergy= new ReactiveEnergyperH;
  ade9000.InitReactiveEnergy(ReactiveEnergy);

  CurrentRMS= new CurrentRMSRegs; 
  VoltageRMS= new VoltageRMSRegs; 

  PowerFactor= new PowerFactorRegs;
  Frequency= new PeriodRegs; 

  CurrentTHD = new CurrentTHDRegs;
  VoltageTHD = new VoltageTHDRegs;

  EEPROM.begin(EEPROM_SIZE);
  InitEnergyCounter();

  Interrupts_setup();
  

}

