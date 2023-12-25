#include "energy_monitor.h"


#define EEPROM_SIZE 512

#define Counter_add 0

#define MeasureStartTime_add 4


volatile PowerRegs* Power;

volatile EnergyRegs* Energy;

volatile RMSRegs* RMS;
volatile PQRegs* PQ;

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
    mqttClient.print((Power->ActivePowerValue)*PowerConversionConstant);
    mqttClient.endMessage();
    snprintf(topic, sizeof(topic), "%s/Power/%s/L2", root_Topic, sub_Topic);
    mqttClient.beginMessage(topic);
    mqttClient.print((Power->ActivePowerValue)*PowerConversionConstant);
    mqttClient.endMessage();
    snprintf(topic, sizeof(topic), "%s/Power/%s/L3", root_Topic, sub_Topic);
    mqttClient.beginMessage(topic);
    mqttClient.print((Power->ActivePowerValue)*PowerConversionConstant);
    mqttClient.endMessage();
    PublishtotalActivePower("totalActivePower");
    PublishdateObserved("dateObserved");
    }else{
      snprintf(topic, sizeof(topic), "%s/Power/%s/L1", root_Topic, sub_Topic);
      store_data(topic,&(Power->ActivePowerValue),myQueue);
      snprintf(topic, sizeof(topic), "%s/Power/%s/L2", root_Topic, sub_Topic);
      store_data(topic,&(Power->ActivePowerValue),myQueue);
      snprintf(topic, sizeof(topic), "%s/Power/%s/L3", root_Topic, sub_Topic);
      store_data(topic,&(Power->ActivePowerValue),myQueue);
    }
}

void EnergyMonitorClass::PublishtotalActivePower(char* sub_Topic){

  char topic[100];
  snprintf(topic, sizeof(topic), "%s/Power/%s", root_Topic, sub_Topic);
  mqttClient.beginMessage(topic);
  mqttClient.print((Power->ActivePowerValue)*PowerConversionConstant);
  mqttClient.endMessage();
  
}

void EnergyMonitorClass::PublishReactivePower(char* sub_Topic){

    char topic[100];
    if (WiFi.status() == WL_CONNECTED){
    snprintf(topic, sizeof(topic), "%s/Power/%s/L1", root_Topic, sub_Topic);
    mqttClient.beginMessage(topic);
    mqttClient.print((Power->FundReactivePowerValue)*PowerConversionConstant);
    mqttClient.endMessage();
    snprintf(topic, sizeof(topic), "%s/Power/%s/L2", root_Topic, sub_Topic);
    mqttClient.beginMessage(topic);
    mqttClient.print((Power->FundReactivePowerValue)*PowerConversionConstant);
    mqttClient.endMessage();
    snprintf(topic, sizeof(topic), "%s/Power/%s/L3", root_Topic, sub_Topic);
    mqttClient.beginMessage(topic);
    mqttClient.print((Power->FundReactivePowerValue)*PowerConversionConstant);
    mqttClient.endMessage();
    PublishtotalReactivePower("totalReactivePower");
    PublishdateObserved("dateObserved");
    }else{
      snprintf(topic, sizeof(topic), "%s/Power/%s/L1", root_Topic, sub_Topic);
      store_data(topic,&(Power->FundReactivePowerValue),myQueue);
      snprintf(topic, sizeof(topic), "%s/Power/%s/L2", root_Topic, sub_Topic);
      store_data(topic,&(Power->FundReactivePowerValue),myQueue);
      snprintf(topic, sizeof(topic), "%s/Power/%s/L3", root_Topic, sub_Topic);
      store_data(topic,&(Power->FundReactivePowerValue),myQueue);
    }
}

void EnergyMonitorClass::PublishtotalReactivePower(char* sub_Topic){

  char topic[100];
  snprintf(topic, sizeof(topic), "%s/Power/%s", root_Topic, sub_Topic);
  mqttClient.beginMessage(topic);
  mqttClient.print((Power->FundReactivePowerValue)*PowerConversionConstant);
  mqttClient.endMessage();
  
}

void EnergyMonitorClass::PublishApparentPower(char* sub_Topic){
    char topic[100];
    if (WiFi.status() == WL_CONNECTED){
    snprintf(topic, sizeof(topic), "%s/Power/%s/L1", root_Topic, sub_Topic);
    mqttClient.beginMessage(topic);
    mqttClient.print((Power->ApparentPowerValue)*PowerConversionConstant);
    mqttClient.endMessage();
    snprintf(topic, sizeof(topic), "%s/Power/%s/L2", root_Topic, sub_Topic);
    mqttClient.beginMessage(topic);
    mqttClient.print((Power->ApparentPowerValue)*PowerConversionConstant);
    mqttClient.endMessage();
    snprintf(topic, sizeof(topic), "%s/Power/%s/L3", root_Topic, sub_Topic);
    mqttClient.beginMessage(topic);
    mqttClient.print((Power->ApparentPowerValue)*PowerConversionConstant);
    mqttClient.endMessage();
    PublishtotalApparentPower("totalApparentPower");
    PublishdateObserved("dateObserved");
    }else{
      snprintf(topic, sizeof(topic), "%s/Power/%s/L1", root_Topic, sub_Topic);
      store_data(topic,&(Power->ApparentPowerValue),myQueue);
      snprintf(topic, sizeof(topic), "%s/Power/%s/L2", root_Topic, sub_Topic);
      store_data(topic,&(Power->ApparentPowerValue),myQueue);
      snprintf(topic, sizeof(topic), "%s/Power/%s/L3", root_Topic, sub_Topic);
      store_data(topic,&(Power->ApparentPowerValue),myQueue);
    }
}

void EnergyMonitorClass::PublishtotalApparentPower(char* sub_Topic){

  char topic[100];
  snprintf(topic, sizeof(topic), "%s/Power/%s", root_Topic, sub_Topic);
  mqttClient.beginMessage(topic);
  mqttClient.print((Power->ApparentPowerValue)*PowerConversionConstant);
  mqttClient.endMessage();
  
}



void EnergyMonitorClass::PublishpowerFactor(char* sub_Topic){

    char topic[100];
    if (WiFi.status() == WL_CONNECTED){
    snprintf(topic, sizeof(topic), "%s/%s/L1", root_Topic, sub_Topic);
    mqttClient.beginMessage(topic);
    mqttClient.print((PQ->PowerFactorValue));
    mqttClient.endMessage();
    snprintf(topic, sizeof(topic), "%s/%s/L2", root_Topic, sub_Topic);
    mqttClient.beginMessage(topic);
    mqttClient.print((PQ->PowerFactorValue));
    mqttClient.endMessage();
    snprintf(topic, sizeof(topic), "%s/%s/L3", root_Topic, sub_Topic);
    mqttClient.beginMessage(topic);
    mqttClient.print((PQ->PowerFactorValue));
    mqttClient.endMessage();
    PublishdateObserved("dateObserved");
    }else{
      snprintf(topic, sizeof(topic), "%s/%s/L1", root_Topic, sub_Topic);
      store_data(topic,&(PQ->PowerFactorValue),myQueue);
      snprintf(topic, sizeof(topic), "%s/%s/L2", root_Topic, sub_Topic);
      store_data(topic,&(PQ->PowerFactorValue),myQueue);
      snprintf(topic, sizeof(topic), "%s/%s/L3", root_Topic, sub_Topic);
      store_data(topic,&(PQ->PowerFactorValue),myQueue);
    }
}

void EnergyMonitorClass::PublishdisplacementpowerFactor(char* sub_Topic){

    char topic[100];
    if (WiFi.status() == WL_CONNECTED){
    snprintf(topic, sizeof(topic), "%s/%s/L1", root_Topic, sub_Topic);
    mqttClient.beginMessage(topic);
    mqttClient.print(cos(PQ->AngleValue_AV_AI));
    mqttClient.endMessage();
    snprintf(topic, sizeof(topic), "%s/%s/L2", root_Topic, sub_Topic);
    mqttClient.beginMessage(topic);
    mqttClient.print(cos(PQ->AngleValue_AV_AI));
    mqttClient.endMessage();
    snprintf(topic, sizeof(topic), "%s/%s/L3", root_Topic, sub_Topic);
    mqttClient.beginMessage(topic);
    mqttClient.print(cos(PQ->AngleValue_AV_AI));
    mqttClient.endMessage();
    PublishdateObserved("dateObserved");
    }else{
      float DPF;
      DPF=cos(PQ->AngleValue_AV_AI);
      snprintf(topic, sizeof(topic), "%s/%s/L1", root_Topic, sub_Topic);
      store_data(topic,&DPF,myQueue);
      DPF=cos(PQ->AngleValue_AV_AI);
      snprintf(topic, sizeof(topic), "%s/%s/L2", root_Topic, sub_Topic);
      store_data(topic,&DPF,myQueue);
      DPF=cos(PQ->AngleValue_AV_AI);
      snprintf(topic, sizeof(topic), "%s/%s/L3", root_Topic, sub_Topic);
      store_data(topic,&DPF,myQueue);
    }
}

void EnergyMonitorClass::Publishvoltage(char* sub_Topic){

    char topic[100];
    if (WiFi.status() == WL_CONNECTED){
    snprintf(topic, sizeof(topic), "%s/Voltages/%s/L1", root_Topic, sub_Topic);
    mqttClient.beginMessage(topic);
    mqttClient.print((RMS->VoltageRMSValue)*VoltageConversionConstant);
    mqttClient.endMessage();
    snprintf(topic, sizeof(topic), "%s/Voltages/%s/L2", root_Topic, sub_Topic);
    mqttClient.beginMessage(topic);
    mqttClient.print((RMS->VoltageRMSValue)*VoltageConversionConstant);
    mqttClient.endMessage();
    snprintf(topic, sizeof(topic), "%s/Voltages/%s/L3", root_Topic, sub_Topic);
    mqttClient.beginMessage(topic);
    mqttClient.print((RMS->VoltageRMSValue)*VoltageConversionConstant);
    mqttClient.endMessage();
    PublishdateObserved("dateObserved");
    }else{
      snprintf(topic, sizeof(topic), "%s/Voltages/%s/L1", root_Topic, sub_Topic);
      store_data(topic,&(RMS->VoltageRMSValue),myQueue);
      snprintf(topic, sizeof(topic), "%s/Voltages/%s/L2", root_Topic, sub_Topic);
      store_data(topic,&(RMS->VoltageRMSValue),myQueue);
      snprintf(topic, sizeof(topic), "%s/Voltages/%s/L3", root_Topic, sub_Topic);
      store_data(topic,&(RMS->VoltageRMSValue),myQueue);
    }
}


void EnergyMonitorClass::Publishcurrent(char* sub_Topic){
    char topic[100];
    if (WiFi.status() == WL_CONNECTED){
    snprintf(topic, sizeof(topic), "%s/Currents/%s/L1", root_Topic, sub_Topic);
    mqttClient.beginMessage(topic);
    mqttClient.print((RMS->CurrentRMSValue)*CurrentConversionConstant);
    mqttClient.endMessage();
    snprintf(topic, sizeof(topic), "%s/Currents/%s/L2", root_Topic, sub_Topic);
    mqttClient.beginMessage(topic);
    mqttClient.print((RMS->CurrentRMSValue)*CurrentConversionConstant);
    mqttClient.endMessage();
    snprintf(topic, sizeof(topic), "%s/Currents/%s/L3", root_Topic, sub_Topic);
    mqttClient.beginMessage(topic);
    mqttClient.print((RMS->CurrentRMSValue)*CurrentConversionConstant);
    mqttClient.endMessage();
    snprintf(topic, sizeof(topic), "%s/Currents/%s/N", root_Topic, sub_Topic);
    mqttClient.beginMessage(topic);
    mqttClient.print((RMS->CurrentRMSValue)*CurrentConversionConstant);
    mqttClient.endMessage();
    PublishdateObserved("dateObserved");
    }else{
      snprintf(topic, sizeof(topic), "%s/Currents/%s/L1", root_Topic, sub_Topic);
      store_data(topic,&(RMS->CurrentRMSValue),myQueue);
      snprintf(topic, sizeof(topic), "%s/Currents/%s/L2", root_Topic, sub_Topic);
      store_data(topic,&(RMS->CurrentRMSValue),myQueue);
      snprintf(topic, sizeof(topic), "%s/Currents/%s/L3", root_Topic, sub_Topic);
      store_data(topic,&(RMS->CurrentRMSValue),myQueue);
      snprintf(topic, sizeof(topic), "%s/Currents/%s/N", root_Topic, sub_Topic);
      store_data(topic,&(RMS->CurrentRMSValue),myQueue);
    }
}


void EnergyMonitorClass::Publishfrequency(char* sub_Topic){
 
  char topic[100];
  if (WiFi.status() == WL_CONNECTED){
  snprintf(topic, sizeof(topic), "%s/%s", root_Topic, sub_Topic);
  mqttClient.beginMessage(topic);
  mqttClient.print(PQ->FrequencyValue);
  mqttClient.endMessage();
  PublishdateObserved("dateObserved");
  }else{
    snprintf(topic, sizeof(topic), "%s/%s", root_Topic, sub_Topic);
    store_data(topic,&(PQ->FrequencyValue),myQueue);
  }
}

void EnergyMonitorClass::PublishvoltageTHD(char* sub_Topic){

    char topic[100];
    if (WiFi.status() == WL_CONNECTED){
    snprintf(topic, sizeof(topic), "%s/Voltages/%s/L1", root_Topic, sub_Topic);
    mqttClient.beginMessage(topic);
    mqttClient.print((RMS->VoltageRMSValue)*VoltageConversionConstant);
    mqttClient.endMessage();
    snprintf(topic, sizeof(topic), "%s/Voltages/%s/L2", root_Topic, sub_Topic);
    mqttClient.beginMessage(topic);
    mqttClient.print((RMS->VoltageRMSValue)*VoltageConversionConstant);
    mqttClient.endMessage();
    snprintf(topic, sizeof(topic), "%s/Voltages/%s/L3", root_Topic, sub_Topic);
    mqttClient.beginMessage(topic);
    mqttClient.print((RMS->VoltageRMSValue)*VoltageConversionConstant);
    mqttClient.endMessage();
    PublishdateObserved("dateObserved");
    }else{
      snprintf(topic, sizeof(topic), "%s/Voltages/%s/L1", root_Topic, sub_Topic);
      store_data(topic,&(RMS->VoltageRMSValue),myQueue);
      snprintf(topic, sizeof(topic), "%s/Voltages/%s/L2", root_Topic, sub_Topic);
      store_data(topic,&(RMS->VoltageRMSValue),myQueue);
      snprintf(topic, sizeof(topic), "%s/Voltages/%s/L3", root_Topic, sub_Topic);
      store_data(topic,&(RMS->VoltageRMSValue),myQueue);
    }
}

void EnergyMonitorClass::PublishcurrentTHD(char* sub_Topic){

    char topic[100];
    if (WiFi.status() == WL_CONNECTED){
    snprintf(topic, sizeof(topic), "%s/Currents/%s/L1", root_Topic, sub_Topic);
    mqttClient.beginMessage(topic);
    mqttClient.print((RMS->CurrentRMSValue)*CurrentConversionConstant);
    mqttClient.endMessage();
    snprintf(topic, sizeof(topic), "%s/Currents/%s/L2", root_Topic, sub_Topic);
    mqttClient.beginMessage(topic);
    mqttClient.print((RMS->CurrentRMSValue)*CurrentConversionConstant);
    mqttClient.endMessage();
    snprintf(topic, sizeof(topic), "%s/Currents/%s/L3", root_Topic, sub_Topic);
    mqttClient.beginMessage(topic);
    mqttClient.print((RMS->CurrentRMSValue)*CurrentConversionConstant);
    mqttClient.endMessage();
    snprintf(topic, sizeof(topic), "%s/Currents/%s/N", root_Topic, sub_Topic);
    mqttClient.beginMessage(topic);
    mqttClient.print((RMS->CurrentRMSValue)*CurrentConversionConstant);
    mqttClient.endMessage();
    PublishdateObserved("dateObserved");
    }else{
      snprintf(topic, sizeof(topic), "%s/Currents/%s/L1", root_Topic, sub_Topic);
      store_data(topic,&(RMS->CurrentRMSValue),myQueue);
      snprintf(topic, sizeof(topic), "%s/Currents/%s/L2", root_Topic, sub_Topic);
      store_data(topic,&(RMS->CurrentRMSValue),myQueue);
      snprintf(topic, sizeof(topic), "%s/Currents/%s/L3", root_Topic, sub_Topic);
      store_data(topic,&(RMS->CurrentRMSValue),myQueue);
      snprintf(topic, sizeof(topic), "%s/Currents/%s/N", root_Topic, sub_Topic);
      store_data(topic,&(RMS->CurrentRMSValue),myQueue);
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
  EEPROM.writeFloat(Counter_add,0);
  EEPROM.commit();
  
  }
}

void EnergyMonitorClass::StoreCountedEnergy(){
  float_t Counter=EEPROM.readFloat(Counter_add);
  EEPROM.writeFloat(Counter_add,Counter+((Energy->ActiveEnergyValue)*EnergyConversionConstant));
  EEPROM.commit();
}

float_t EnergyMonitorClass::ReadEnergyCounter(){

 
  return(EEPROM.readFloat(Counter_add));

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

  Power= new PowerRegs;
  Energy= new EnergyRegs;
  ade9153A.InitActiveEnergy(Energy);
  ade9153A.InitReactiveEnergy(Energy);

  RMS= new RMSRegs;  
  PQ= new PQRegs;

  EEPROM.begin(EEPROM_SIZE);
  InitEnergyCounter();
  Interrupts_setup();
  
}


