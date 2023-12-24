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
volatile PeriodRegs* Frequency;

volatile CurrentTHDRegs* CurrentTHD;
volatile VoltageTHDRegs* VoltageTHD;

volatile uint8_t EnergySamples;

const char root_Topic[]="/ul/EnergyMonitoring/areaServed/Type/phaseType/deviceID";


EnergyMonitorClass::EnergyMonitorClass()
{
	
}


void EnergyMonitorClass::PublishdateObserved(char* sub_Topic){

  char buf[21];
  char topic[100];
    strftime(buf, 21, "%Y-%m-%dT%H:%M:%SZ", &timeinfo);
    snprintf(topic, sizeof(topic), "%s/%s", root_Topic, sub_Topic);
    mqttClient.beginMessage(topic);
    mqttClient.print(buf);
    mqttClient.endMessage();
}

void EnergyMonitorClass::PublishphaseType(char* sub_Topic,char* type){

    char topic[100];
    snprintf(topic, sizeof(topic), "%s/%s", root_Topic, sub_Topic);
    mqttClient.beginMessage(topic);
    mqttClient.print(type);
    mqttClient.endMessage();
}



void EnergyMonitorClass::PublishActivePower(char* sub_Topic){

    char topic[100];
    if (WiFi.status() == WL_CONNECTED){
    snprintf(topic, sizeof(topic), "%s/Power/%s/L1", root_Topic, sub_Topic);
    mqttClient.beginMessage(topic);
    mqttClient.print((ActivePower->ActivePowerReg_A)*PowerConversionConstant);
    mqttClient.endMessage();
    snprintf(topic, sizeof(topic), "%s/Power/%s/L2", root_Topic, sub_Topic);
    mqttClient.beginMessage(topic);
    mqttClient.print((ActivePower->ActivePowerReg_B)*PowerConversionConstant);
    mqttClient.endMessage();
    snprintf(topic, sizeof(topic), "%s/Power/%s/L3", root_Topic, sub_Topic);
    mqttClient.beginMessage(topic);
    mqttClient.print((ActivePower->ActivePowerReg_C)*PowerConversionConstant);
    mqttClient.endMessage();
    PublishtotalActivePower("totalActivePower");
    PublishdateObserved("dateObserved");
    }else{
      snprintf(topic, sizeof(topic), "%s/Power/%s/L1", root_Topic, sub_Topic);
      store_data(topic,&(ActivePower->ActivePowerReg_A),myQueue);
      snprintf(topic, sizeof(topic), "%s/Power/%s/L2", root_Topic, sub_Topic);
      store_data(topic,&(ActivePower->ActivePowerReg_B),myQueue);
      snprintf(topic, sizeof(topic), "%s/Power/%s/L3", root_Topic, sub_Topic);
      store_data(topic,&(ActivePower->ActivePowerReg_C),myQueue);
    }
}

void EnergyMonitorClass::PublishtotalActivePower(char* sub_Topic){

  char topic[100];
  snprintf(topic, sizeof(topic), "%s/Power/%s", root_Topic, sub_Topic);
  mqttClient.beginMessage(topic);
  mqttClient.print(((ActivePower->ActivePowerReg_A)+(ActivePower->ActivePowerReg_B)+(ActivePower->ActivePowerReg_C))*PowerConversionConstant);
  mqttClient.endMessage();
  
}

void EnergyMonitorClass::PublishReactivePower(char* sub_Topic){

    char topic[100];
    if (WiFi.status() == WL_CONNECTED){
    snprintf(topic, sizeof(topic), "%s/Power/%s/L1", root_Topic, sub_Topic);
    mqttClient.beginMessage(topic);
    mqttClient.print((ReactivePower->ReactivePowerReg_A)*PowerConversionConstant);
    mqttClient.endMessage();
    snprintf(topic, sizeof(topic), "%s/Power/%s/L2", root_Topic, sub_Topic);
    mqttClient.beginMessage(topic);
    mqttClient.print((ReactivePower->ReactivePowerReg_B)*PowerConversionConstant);
    mqttClient.endMessage();
    snprintf(topic, sizeof(topic), "%s/Power/%s/L3", root_Topic, sub_Topic);
    mqttClient.beginMessage(topic);
    mqttClient.print((ReactivePower->ReactivePowerReg_C)*PowerConversionConstant);
    mqttClient.endMessage();
    PublishtotalReactivePower("totalReactivePower");
    PublishdateObserved("dateObserved");
    }else{
      snprintf(topic, sizeof(topic), "%s/Power/%s/L1", root_Topic, sub_Topic);
      store_data(topic,&(ReactivePower->ReactivePowerReg_A),myQueue);
      snprintf(topic, sizeof(topic), "%s/Power/%s/L2", root_Topic, sub_Topic);
      store_data(topic,&(ReactivePower->ReactivePowerReg_B),myQueue);
      snprintf(topic, sizeof(topic), "%s/Power/%s/L3", root_Topic, sub_Topic);
      store_data(topic,&(ReactivePower->ReactivePowerReg_C),myQueue);
    }
}

void EnergyMonitorClass::PublishtotalReactivePower(char* sub_Topic){

  char topic[100];
  snprintf(topic, sizeof(topic), "%s/Power/%s", root_Topic, sub_Topic);
  mqttClient.beginMessage(topic);
  mqttClient.print(((ReactivePower->ReactivePowerReg_A)+(ReactivePower->ReactivePowerReg_B)+(ReactivePower->ReactivePowerReg_C))*PowerConversionConstant);
  mqttClient.endMessage();
  
}

void EnergyMonitorClass::PublishApparentPower(char* sub_Topic){
    char topic[100];
    if (WiFi.status() == WL_CONNECTED){
    snprintf(topic, sizeof(topic), "%s/Power/%s/L1", root_Topic, sub_Topic);
    mqttClient.beginMessage(topic);
    mqttClient.print((ApparentPower->ApparentPowerReg_A)*PowerConversionConstant);
    mqttClient.endMessage();
    snprintf(topic, sizeof(topic), "%s/Power/%s/L2", root_Topic, sub_Topic);
    mqttClient.beginMessage(topic);
    mqttClient.print((ApparentPower->ApparentPowerReg_B)*PowerConversionConstant);
    mqttClient.endMessage();
    snprintf(topic, sizeof(topic), "%s/Power/%s/L3", root_Topic, sub_Topic);
    mqttClient.beginMessage(topic);
    mqttClient.print((ApparentPower->ApparentPowerReg_C)*PowerConversionConstant);
    mqttClient.endMessage();
    PublishtotalApparentPower("totalApparentPower");
    PublishdateObserved("dateObserved");
    }else{
      snprintf(topic, sizeof(topic), "%s/Power/%s/L1", root_Topic, sub_Topic);
      store_data(topic,&(ApparentPower->ApparentPowerReg_A),myQueue);
      snprintf(topic, sizeof(topic), "%s/Power/%s/L2", root_Topic, sub_Topic);
      store_data(topic,&(ApparentPower->ApparentPowerReg_B),myQueue);
      snprintf(topic, sizeof(topic), "%s/Power/%s/L3", root_Topic, sub_Topic);
      store_data(topic,&(ApparentPower->ApparentPowerReg_C),myQueue);
    }
}

void EnergyMonitorClass::PublishtotalApparentPower(char* sub_Topic){

  char topic[100];
  snprintf(topic, sizeof(topic), "%s/Power/%s", root_Topic, sub_Topic);
  mqttClient.beginMessage(topic);
  mqttClient.print(((ApparentPower->ApparentPowerReg_A)+(ApparentPower->ApparentPowerReg_B)+(ApparentPower->ApparentPowerReg_C))*PowerConversionConstant);
  mqttClient.endMessage();
  
}



void EnergyMonitorClass::PublishpowerFactor(char* sub_Topic){

    char topic[100];
    if (WiFi.status() == WL_CONNECTED){
    snprintf(topic, sizeof(topic), "%s/%s/L1", root_Topic, sub_Topic);
    mqttClient.beginMessage(topic);
    mqttClient.print((PowerFactor->PowerFactorValue_A));
    mqttClient.endMessage();
    snprintf(topic, sizeof(topic), "%s/%s/L2", root_Topic, sub_Topic);
    mqttClient.beginMessage(topic);
    mqttClient.print((PowerFactor->PowerFactorValue_B));
    mqttClient.endMessage();
    snprintf(topic, sizeof(topic), "%s/%s/L3", root_Topic, sub_Topic);
    mqttClient.beginMessage(topic);
    mqttClient.print((PowerFactor->PowerFactorValue_C));
    mqttClient.endMessage();
    PublishdateObserved("dateObserved");
    }else{
      snprintf(topic, sizeof(topic), "%s/%s/L1", root_Topic, sub_Topic);
      store_data(topic,&(PowerFactor->PowerFactorValue_A),myQueue);
      snprintf(topic, sizeof(topic), "%s/%s/L2", root_Topic, sub_Topic);
      store_data(topic,&(PowerFactor->PowerFactorValue_B),myQueue);
      snprintf(topic, sizeof(topic), "%s/%s/L3", root_Topic, sub_Topic);
      store_data(topic,&(PowerFactor->PowerFactorValue_C),myQueue);
    }
}

void EnergyMonitorClass::PublishdisplacementpowerFactor(char* sub_Topic){

    char topic[100];
    if (WiFi.status() == WL_CONNECTED){
    snprintf(topic, sizeof(topic), "%s/%s/L1", root_Topic, sub_Topic);
    mqttClient.beginMessage(topic);
    mqttClient.print((PowerFactor->PowerFactorValue_A));
    mqttClient.endMessage();
    snprintf(topic, sizeof(topic), "%s/%s/L2", root_Topic, sub_Topic);
    mqttClient.beginMessage(topic);
    mqttClient.print((PowerFactor->PowerFactorValue_B));
    mqttClient.endMessage();
    snprintf(topic, sizeof(topic), "%s/%s/L3", root_Topic, sub_Topic);
    mqttClient.beginMessage(topic);
    mqttClient.print((PowerFactor->PowerFactorValue_C));
    mqttClient.endMessage();
    PublishdateObserved("dateObserved");
    }else{
      snprintf(topic, sizeof(topic), "%s/%s/L1", root_Topic, sub_Topic);
      store_data(topic,&(PowerFactor->PowerFactorValue_A),myQueue);
      snprintf(topic, sizeof(topic), "%s/%s/L2", root_Topic, sub_Topic);
      store_data(topic,&(PowerFactor->PowerFactorValue_B),myQueue);
      snprintf(topic, sizeof(topic), "%s/%s/L3", root_Topic, sub_Topic);
      store_data(topic,&(PowerFactor->PowerFactorValue_C),myQueue);
    }
    
}

void EnergyMonitorClass::Publishvoltage(char* sub_Topic){

    char topic[100];
    if (WiFi.status() == WL_CONNECTED){
    snprintf(topic, sizeof(topic), "%s/Voltages/%s/L1", root_Topic, sub_Topic);
    mqttClient.beginMessage(topic);
    mqttClient.print((VoltageRMS->VoltageRMSReg_A)*VoltageConversionConstant);
    mqttClient.endMessage();
    snprintf(topic, sizeof(topic), "%s/Voltages/%s/L2", root_Topic, sub_Topic);
    mqttClient.beginMessage(topic);
    mqttClient.print((VoltageRMS->VoltageRMSReg_B)*VoltageConversionConstant);
    mqttClient.endMessage();
    snprintf(topic, sizeof(topic), "%s/Voltages/%s/L3", root_Topic, sub_Topic);
    mqttClient.beginMessage(topic);
    mqttClient.print((VoltageRMS->VoltageRMSReg_C)*VoltageConversionConstant);
    mqttClient.endMessage();
    PublishdateObserved("dateObserved");
    }else{
      snprintf(topic, sizeof(topic), "%s/Voltages/%s/L1", root_Topic, sub_Topic);
      store_data(topic,&(VoltageRMS->VoltageRMSReg_A),myQueue);
      snprintf(topic, sizeof(topic), "%s/Voltages/%s/L2", root_Topic, sub_Topic);
      store_data(topic,&(VoltageRMS->VoltageRMSReg_B),myQueue);
      snprintf(topic, sizeof(topic), "%s/Volatages/%s/L3", root_Topic, sub_Topic);
      store_data(topic,&(VoltageRMS->VoltageRMSReg_C),myQueue);
    }
}


void EnergyMonitorClass::Publishcurrent(char* sub_Topic){
    char topic[100];
    if (WiFi.status() == WL_CONNECTED){
    snprintf(topic, sizeof(topic), "%s/Currents/%s/L1", root_Topic, sub_Topic);
    mqttClient.beginMessage(topic);
    mqttClient.print((CurrentRMS->CurrentRMSReg_A)*CurrentConversionConstant);
    mqttClient.endMessage();
    snprintf(topic, sizeof(topic), "%s/Currents/%s/L2", root_Topic, sub_Topic);
    mqttClient.beginMessage(topic);
    mqttClient.print((CurrentRMS->CurrentRMSReg_B)*CurrentConversionConstant);
    mqttClient.endMessage();
    snprintf(topic, sizeof(topic), "%s/Currents/%s/L3", root_Topic, sub_Topic);
    mqttClient.beginMessage(topic);
    mqttClient.print((CurrentRMS->CurrentRMSReg_C)*CurrentConversionConstant);
    mqttClient.endMessage();
    snprintf(topic, sizeof(topic), "%s/Currents/%s/N", root_Topic, sub_Topic);
    mqttClient.beginMessage(topic);
    mqttClient.print((CurrentRMS->CurrentRMSReg_N)*CurrentConversionConstant);
    mqttClient.endMessage();
    PublishdateObserved("dateObserved");
    }else{
      snprintf(topic, sizeof(topic), "%s/Currents/%s/L1", root_Topic, sub_Topic);
      store_data(topic,&(CurrentRMS->CurrentRMSReg_A),myQueue);
      snprintf(topic, sizeof(topic), "%s/Currents/%s/L2", root_Topic, sub_Topic);
      store_data(topic,&(CurrentRMS->CurrentRMSReg_B),myQueue);
      snprintf(topic, sizeof(topic), "%s/Currents/%s/L3", root_Topic, sub_Topic);
      store_data(topic,&(CurrentRMS->CurrentRMSReg_C),myQueue);
    }
}

void EnergyMonitorClass::PublishvoltagePhaseToPhase(char* sub_Topic){

    char topic[100];
    if (WiFi.status() == WL_CONNECTED){
    snprintf(topic, sizeof(topic), "%s/Voltages/%s/L1", root_Topic, sub_Topic);
    mqttClient.beginMessage(topic);
    mqttClient.print((VoltageRMS->VoltageRMSReg_A)*VoltageConversionConstant);
    mqttClient.endMessage();
    snprintf(topic, sizeof(topic), "%s/Voltages/%s/L2", root_Topic, sub_Topic);
    mqttClient.beginMessage(topic);
    mqttClient.print((VoltageRMS->VoltageRMSReg_B)*VoltageConversionConstant);
    mqttClient.endMessage();
    snprintf(topic, sizeof(topic), "%s/Voltages/%s/L3", root_Topic, sub_Topic);
    mqttClient.beginMessage(topic);
    mqttClient.print((VoltageRMS->VoltageRMSReg_C)*VoltageConversionConstant);
    mqttClient.endMessage();
    PublishdateObserved("dateObserved");
    }else{
      snprintf(topic, sizeof(topic), "%s/Voltages/%s/L1", root_Topic, sub_Topic);
      store_data(topic,&(VoltageRMS->VoltageRMSReg_A),myQueue);
      snprintf(topic, sizeof(topic), "%s/Voltages/%s/L2", root_Topic, sub_Topic);
      store_data(topic,&(VoltageRMS->VoltageRMSReg_B),myQueue);
      snprintf(topic, sizeof(topic), "%s/Volatages/%s/L3", root_Topic, sub_Topic);
      store_data(topic,&(VoltageRMS->VoltageRMSReg_C),myQueue);
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
  if (WiFi.status() == WL_CONNECTED){
  snprintf(topic, sizeof(topic), "%s/%s", root_Topic, sub_Topic);
  mqttClient.beginMessage(topic);
  mqttClient.print(freq);
  mqttClient.endMessage();
  PublishdateObserved("dateObserved");
  }else{
      snprintf(topic, sizeof(topic), "%s/%s", root_Topic, sub_Topic);
      store_data(topic,&freq,myQueue);
    }
}

void EnergyMonitorClass::PublishvoltageTHD(char* sub_Topic){

    char topic[100];
    if (WiFi.status() == WL_CONNECTED){
    snprintf(topic, sizeof(topic), "%s/Voltages/%s/L1", root_Topic, sub_Topic);
    mqttClient.beginMessage(topic);
    mqttClient.print((VoltageTHD->VoltageTHDValue_A)*VoltageConversionConstant);
    mqttClient.endMessage();
    snprintf(topic, sizeof(topic), "%s/Voltages/%s/L2", root_Topic, sub_Topic);
    mqttClient.beginMessage(topic);
    mqttClient.print((VoltageTHD->VoltageTHDValue_B)*VoltageConversionConstant);
    mqttClient.endMessage();
    snprintf(topic, sizeof(topic), "%s/Voltages/%s/L3", root_Topic, sub_Topic);
    mqttClient.beginMessage(topic);
    mqttClient.print((VoltageTHD->VoltageTHDValue_C)*VoltageConversionConstant);
    mqttClient.endMessage();
    PublishdateObserved("dateObserved");
    }else{
      snprintf(topic, sizeof(topic), "%s/Voltages/%s/L1", root_Topic, sub_Topic);
      store_data(topic,&(VoltageTHD->VoltageTHDValue_A),myQueue);
      snprintf(topic, sizeof(topic), "%s/Voltages/%s/L2", root_Topic, sub_Topic);
      store_data(topic,&(VoltageTHD->VoltageTHDValue_B),myQueue);
      snprintf(topic, sizeof(topic), "%s/Volatages/%s/L3", root_Topic, sub_Topic);
      store_data(topic,&(VoltageTHD->VoltageTHDValue_C),myQueue);
    }
}

void EnergyMonitorClass::PublishcurrentTHD(char* sub_Topic){

    char topic[100];
    if (WiFi.status() == WL_CONNECTED){
    snprintf(topic, sizeof(topic), "%s/Currents/%s/L1", root_Topic, sub_Topic);
    mqttClient.beginMessage(topic);
    mqttClient.print((CurrentTHD->CurrentTHDValue_A)*CurrentConversionConstant);
    mqttClient.endMessage();
    snprintf(topic, sizeof(topic), "%s/Currents/%s/L2", root_Topic, sub_Topic);
    mqttClient.beginMessage(topic);
    mqttClient.print((CurrentTHD->CurrentTHDValue_B)*CurrentConversionConstant);
    mqttClient.endMessage();
    snprintf(topic, sizeof(topic), "%s/Currents/%s/L3", root_Topic, sub_Topic);
    mqttClient.beginMessage(topic);
    mqttClient.print((CurrentTHD->CurrentTHDValue_C)*CurrentConversionConstant);
    mqttClient.endMessage();
    PublishdateObserved("dateObserved");
    }else{
      snprintf(topic, sizeof(topic), "%s/Currents/%s/L1", root_Topic, sub_Topic);
      store_data(topic,&(CurrentTHD->CurrentTHDValue_A),myQueue);
      snprintf(topic, sizeof(topic), "%s/Currents/%s/L2", root_Topic, sub_Topic);
      store_data(topic,&(CurrentTHD->CurrentTHDValue_B),myQueue);
      snprintf(topic, sizeof(topic), "%s/Currents/%s/L3", root_Topic, sub_Topic);
      store_data(topic,&(CurrentTHD->CurrentTHDValue_C),myQueue);
    }
}

void EnergyMonitorClass::handleWifiStatus(wl_status_t* WifiStatus){
    if (WiFi.status()!= *WifiStatus){
    if (WiFi.status() != WL_CONNECTED){
    digitalWrite(2, HIGH);
    Change_timers_config(3);
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



void EnergyMonitorClass::store_data(char* sub_Topic,volatile void* data,std::queue<struct dataNode>& myQueue){
  if (esp_get_free_heap_size()>(sizeof(struct dataNode))){
    struct dataNode node;
    char Time[21];
    strftime(Time, 21, "%Y-%m-%dT%H:%M:%SZ", &timeinfo);
    node.topic=sub_Topic;
    node.data=data;
    strcpy(node.time,Time);
    myQueue.push(node);
    Serial.println("[APP] Free memory: " + String(esp_get_free_heap_size()) + " bytes");
  }
}



void EnergyMonitorClass::send_data(std::queue<struct dataNode>& myQueue){
  char topic[100];
  float factor;
  struct dataNode node;
  while (!myQueue.empty()) {
   node=myQueue.front();
   Serial.println("///////STORED////////");   
   snprintf(topic, sizeof(topic), "%s/%s", root_Topic,node.topic);
   if (strstr( node.topic, "ActivePower" ) || strstr( node.topic, "ReactivePower" )|| strstr( node.topic, "ApparentPower" ) || strstr( node.topic,"totalActiveEnergyImport") || strstr( node.topic,"totalReactiveEnergyImport") || strstr( node.topic,"totalActiveEnergyExport") || strstr( node.topic,"totalReactiveEnergyExport") ){
        factor=PowerConversionConstant;
    }else if(strstr( node.topic, "voltage" ) || strstr( node.topic, "voltagePhaseToPhase" ) || strstr( node.topic, "voltageTHD" )){
        factor=VoltageConversionConstant;
    }else if(strstr( node.topic, "current" ) || strcmp( node.topic, "currentTHD" )){
        factor=CurrentConversionConstant;
    }else{
      factor=1.0;
    }
    
   if (strstr( node.topic, "currentTHD" ) || strstr( node.topic, "voltageTHD" ) || strstr( node.topic, "powerFactor" )){
    mqttClient.beginMessage(topic);
    mqttClient.print((*(float*)node.data)*factor);
    mqttClient.endMessage();
   }else{
    mqttClient.beginMessage(topic);
    mqttClient.print((*(int32_t*)node.data)*factor);
    mqttClient.endMessage();
   }
   if (strstr(node.topic, "L3")){
    snprintf(topic, sizeof(topic), "%s/dateObserved", root_Topic);
    mqttClient.beginMessage(topic);
    mqttClient.print(node.time);
    mqttClient.endMessage();
   }
   
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

