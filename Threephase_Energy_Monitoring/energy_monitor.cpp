#include "energy_monitor.h"


#define EEPROM_SIZE 512

#define CounterA_add 0

#define CounterB_add 4

#define CounterC_add 8

#define MeasureStartTime_add 12



extern hw_timer_t * Powertimer; 
extern hw_timer_t * Energytimer;
extern hw_timer_t * CFtimer;
extern hw_timer_t * THDtimer;

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

int StoreAddress;

volatile uint8_t EnergySamples;

extern struct tm timeinfo;

extern WiFiClient wifiClient;
extern MqttClient mqttClient;

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

void EnergyMonitorClass::PublishtotalActiveEnergy(char* sub_Topic){

  char topic[100];
  snprintf(topic, sizeof(topic), "%s/totalActiveEnergy%s", root_Topic, sub_Topic);
  mqttClient.beginMessage(topic);
  mqttClient.print(topic);
  mqttClient.print((ActiveEnergy->ActiveEnergy_A)*EnergyConversionConstant);
  mqttClient.endMessage();
  mqttClient.beginMessage(topic);
  mqttClient.print((ActiveEnergy->ActiveEnergy_B)*EnergyConversionConstant);
  mqttClient.endMessage();
  mqttClient.beginMessage(topic);
  mqttClient.print((ActiveEnergy->ActiveEnergy_C)*EnergyConversionConstant);
  mqttClient.endMessage();
  PublishdateObserved("dateObserved");
  
}

void EnergyMonitorClass::PublishtotalReactiveEnergy(char* sub_Topic){

  char topic[100];
  snprintf(topic, sizeof(topic), "%s/totalReactiveEnergy%s", root_Topic, sub_Topic);
  mqttClient.beginMessage(topic);
  mqttClient.print(topic);
  mqttClient.print((ReactiveEnergy->ReactiveEnergy_A)*EnergyConversionConstant);
  mqttClient.endMessage();
  mqttClient.beginMessage(topic);
  mqttClient.print((ReactiveEnergy->ReactiveEnergy_B)*EnergyConversionConstant);
  mqttClient.endMessage();
  mqttClient.beginMessage(topic);
  mqttClient.print((ReactiveEnergy->ReactiveEnergy_C)*EnergyConversionConstant);
  mqttClient.endMessage();
  PublishdateObserved("dateObserved");
  
}



void EnergyMonitorClass::PublishActivePower(char* sub_Topic){

    char topic[100];
    snprintf(topic, sizeof(topic), "%s/%s/L1", root_Topic, sub_Topic);
    mqttClient.beginMessage(topic);
    mqttClient.print((ActivePower->ActivePowerReg_A)*PowerConversionConstant);
    mqttClient.endMessage();
    snprintf(topic, sizeof(topic), "%s/%s/L2", root_Topic, sub_Topic);
    mqttClient.beginMessage(topic);
    mqttClient.print((ActivePower->ActivePowerReg_B)*PowerConversionConstant);
    mqttClient.endMessage();
    snprintf(topic, sizeof(topic), "%s/%s/L3", root_Topic, sub_Topic);
    mqttClient.beginMessage(topic);
    mqttClient.print((ActivePower->ActivePowerReg_C)*PowerConversionConstant);
    mqttClient.endMessage();
    PublishtotalActivePower("totalActivePower");
    PublishdateObserved("dateObserved");
}

void EnergyMonitorClass::PublishtotalActivePower(char* sub_Topic){

  char topic[100];
  snprintf(topic, sizeof(topic), "%s/%s", root_Topic, sub_Topic);
  mqttClient.beginMessage(topic);
  mqttClient.print(((ActivePower->ActivePowerReg_A)+(ActivePower->ActivePowerReg_B)+(ActivePower->ActivePowerReg_C))*PowerConversionConstant);
  mqttClient.endMessage();
  
}

void EnergyMonitorClass::PublishReactivePower(char* sub_Topic){

    char topic[100];
    snprintf(topic, sizeof(topic), "%s/%s/L1", root_Topic, sub_Topic);
    mqttClient.beginMessage(topic);
    mqttClient.print((ReactivePower->ReactivePowerReg_A)*PowerConversionConstant);
    mqttClient.endMessage();
    snprintf(topic, sizeof(topic), "%s/%s/L2", root_Topic, sub_Topic);
    mqttClient.beginMessage(topic);
    mqttClient.print((ReactivePower->ReactivePowerReg_B)*PowerConversionConstant);
    mqttClient.endMessage();
    snprintf(topic, sizeof(topic), "%s/%s/L3", root_Topic, sub_Topic);
    mqttClient.beginMessage(topic);
    mqttClient.print((ReactivePower->ReactivePowerReg_C)*PowerConversionConstant);
    mqttClient.endMessage();
    PublishtotalReactivePower("totalReactivePower");
    PublishdateObserved("dateObserved");
}

void EnergyMonitorClass::PublishtotalReactivePower(char* sub_Topic){

  char topic[100];
  snprintf(topic, sizeof(topic), "%s/%s", root_Topic, sub_Topic);
  mqttClient.beginMessage(topic);
  mqttClient.print(((ReactivePower->ReactivePowerReg_A)+(ReactivePower->ReactivePowerReg_B)+(ReactivePower->ReactivePowerReg_C))*PowerConversionConstant);
  mqttClient.endMessage();
  
}

void EnergyMonitorClass::PublishApparentPower(char* sub_Topic){
    char topic[100];
    snprintf(topic, sizeof(topic), "%s/%s/L1", root_Topic, sub_Topic);
    mqttClient.beginMessage(topic);
    mqttClient.print((ApparentPower->ApparentPowerReg_A)*PowerConversionConstant);
    mqttClient.endMessage();
    snprintf(topic, sizeof(topic), "%s/%s/L2", root_Topic, sub_Topic);
    mqttClient.beginMessage(topic);
    mqttClient.print((ApparentPower->ApparentPowerReg_B)*PowerConversionConstant);
    mqttClient.endMessage();
    snprintf(topic, sizeof(topic), "%s/%s/L3", root_Topic, sub_Topic);
    mqttClient.beginMessage(topic);
    mqttClient.print((ApparentPower->ApparentPowerReg_C)*PowerConversionConstant);
    mqttClient.endMessage();
    PublishtotalApparentPower("totalApparentPower");
    PublishdateObserved("dateObserved");
}

void EnergyMonitorClass::PublishtotalApparentPower(char* sub_Topic){

  char topic[100];
  snprintf(topic, sizeof(topic), "%s/%s", root_Topic, sub_Topic);
  mqttClient.beginMessage(topic);
  mqttClient.print(((ApparentPower->ApparentPowerReg_A)+(ApparentPower->ApparentPowerReg_B)+(ApparentPower->ApparentPowerReg_C))*PowerConversionConstant);
  mqttClient.endMessage();
  
}



void EnergyMonitorClass::PublishpowerFactor(char* sub_Topic){

    char topic[100];
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
}

void EnergyMonitorClass::PublishdisplacementpowerFactor(char* sub_Topic){

    char topic[100];
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
}

void EnergyMonitorClass::Publishvoltage(char* sub_Topic){

    char topic[100];
    snprintf(topic, sizeof(topic), "%s/%s/L1", root_Topic, sub_Topic);
    mqttClient.beginMessage(topic);
    mqttClient.print((VoltageRMS->VoltageRMSReg_A)*VoltageConversionConstant);
    mqttClient.endMessage();
    snprintf(topic, sizeof(topic), "%s/%s/L2", root_Topic, sub_Topic);
    mqttClient.beginMessage(topic);
    mqttClient.print((VoltageRMS->VoltageRMSReg_B)*VoltageConversionConstant);
    mqttClient.endMessage();
    snprintf(topic, sizeof(topic), "%s/%s/L3", root_Topic, sub_Topic);
    mqttClient.beginMessage(topic);
    mqttClient.print((VoltageRMS->VoltageRMSReg_C)*VoltageConversionConstant);
    mqttClient.endMessage();
    PublishdateObserved("dateObserved");
}


void EnergyMonitorClass::Publishcurrent(char* sub_Topic){
    char topic[100];
    snprintf(topic, sizeof(topic), "%s/%s/L1", root_Topic, sub_Topic);
    mqttClient.beginMessage(topic);
    mqttClient.print((CurrentRMS->CurrentRMSReg_A)*CurrentConversionConstant);
    mqttClient.endMessage();
    snprintf(topic, sizeof(topic), "%s/%s/L2", root_Topic, sub_Topic);
    mqttClient.beginMessage(topic);
    mqttClient.print((CurrentRMS->CurrentRMSReg_B)*CurrentConversionConstant);
    mqttClient.endMessage();
    snprintf(topic, sizeof(topic), "%s/%s/L3", root_Topic, sub_Topic);
    mqttClient.beginMessage(topic);
    mqttClient.print((CurrentRMS->CurrentRMSReg_C)*CurrentConversionConstant);
    mqttClient.endMessage();
    snprintf(topic, sizeof(topic), "%s/%s/N", root_Topic, sub_Topic);
    mqttClient.beginMessage(topic);
    mqttClient.print((CurrentRMS->CurrentRMSReg_N)*CurrentConversionConstant);
    mqttClient.endMessage();
    PublishdateObserved("dateObserved");
}

void EnergyMonitorClass::PublishvoltagePhaseToPhase(char* sub_Topic){

    char topic[100];
    snprintf(topic, sizeof(topic), "%s/%s/L1", root_Topic, sub_Topic);
    mqttClient.beginMessage(topic);
    mqttClient.print((VoltageRMS->VoltageRMSReg_A)*VoltageConversionConstant);
    mqttClient.endMessage();
    snprintf(topic, sizeof(topic), "%s/%s/L2", root_Topic, sub_Topic);
    mqttClient.beginMessage(topic);
    mqttClient.print((VoltageRMS->VoltageRMSReg_B)*VoltageConversionConstant);
    mqttClient.endMessage();
    snprintf(topic, sizeof(topic), "%s/%s/L3", root_Topic, sub_Topic);
    mqttClient.beginMessage(topic);
    mqttClient.print((VoltageRMS->VoltageRMSReg_C)*VoltageConversionConstant);
    mqttClient.endMessage();
    PublishdateObserved("dateObserved");
}

void EnergyMonitorClass::Publishfrequency(char* sub_Topic){
 
  char topic[100];
  float farestdist;
  farestdist=max(abs((Frequency->FrequencyValue_A)-50),abs((Frequency->FrequencyValue_B)-50));
  farestdist=max(abs((Frequency->FrequencyValue_C)-50),farestdist);
  snprintf(topic, sizeof(topic), "%s/%s", root_Topic, sub_Topic);
  mqttClient.beginMessage(topic);
  mqttClient.print(farestdist+50);
  mqttClient.endMessage();
  PublishdateObserved("dateObserved");
}

void EnergyMonitorClass::PublishvoltageTHD(char* sub_Topic){

    char topic[100];
    snprintf(topic, sizeof(topic), "%s/%s/L1", root_Topic, sub_Topic);
    mqttClient.beginMessage(topic);
    mqttClient.print((VoltageTHD->VoltageTHDValue_A)*VoltageConversionConstant);
    mqttClient.endMessage();
    snprintf(topic, sizeof(topic), "%s/%s/L2", root_Topic, sub_Topic);
    mqttClient.beginMessage(topic);
    mqttClient.print((VoltageTHD->VoltageTHDValue_B)*VoltageConversionConstant);
    mqttClient.endMessage();
    snprintf(topic, sizeof(topic), "%s/%s/L3", root_Topic, sub_Topic);
    mqttClient.beginMessage(topic);
    mqttClient.print((VoltageTHD->VoltageTHDValue_C)*VoltageConversionConstant);
    mqttClient.endMessage();
    PublishdateObserved("dateObserved");
}

void EnergyMonitorClass::PublishcurrentTHD(char* sub_Topic){

    char topic[100];
    snprintf(topic, sizeof(topic), "%s/%s/L1", root_Topic, sub_Topic);
    mqttClient.beginMessage(topic);
    mqttClient.print((CurrentTHD->CurrentTHDValue_A)*CurrentConversionConstant);
    mqttClient.endMessage();
    snprintf(topic, sizeof(topic), "%s/%s/L2", root_Topic, sub_Topic);
    mqttClient.beginMessage(topic);
    mqttClient.print((CurrentTHD->CurrentTHDValue_B)*CurrentConversionConstant);
    mqttClient.endMessage();
    snprintf(topic, sizeof(topic), "%s/%s/L3", root_Topic, sub_Topic);
    mqttClient.beginMessage(topic);
    mqttClient.print((CurrentTHD->CurrentTHDValue_C)*CurrentConversionConstant);
    mqttClient.endMessage();
    PublishdateObserved("dateObserved");
}



void EnergyMonitorClass::InitEnergyCounter(){
  EEPROM.writeFloat(CounterA_add,0);
  EEPROM.commit();
  EEPROM.writeFloat(CounterB_add,0);
  EEPROM.commit();
  EEPROM.writeFloat(CounterC_add,0);
  EEPROM.commit();
}

void EnergyMonitorClass::StoreCountedEnergy(){
  float_t CounterA=EEPROM.readFloat(CounterA_add);
  EEPROM.writeFloat(CounterA_add,CounterA+((ActiveEnergy->ActiveEnergy_A)*EnergyConversionConstant));
  float_t CounterB=EEPROM.readFloat(CounterB_add);
  EEPROM.writeFloat(CounterB_add,CounterB+((ActiveEnergy->ActiveEnergy_B)*EnergyConversionConstant));
  float_t CounterC=EEPROM.readFloat(CounterC_add);
  EEPROM.writeFloat(CounterC_add,CounterC+((ActiveEnergy->ActiveEnergy_C)*EnergyConversionConstant));
}

void EnergyMonitorClass::ReadEnergyCounter(){
  float_t CounterA=EEPROM.readFloat(CounterA_add);
  float_t CounterB=EEPROM.readFloat(CounterB_add);
  float_t CounterC=EEPROM.readFloat(CounterC_add);
  Serial.println(CounterA);
  Serial.println(CounterB);
  Serial.println(CounterC);
}



String EnergyMonitorClass::Read_String_From_ESP32(int* address)
{
    if(*address<=EEPROM_SIZE){
    String Value=EEPROM.readString(*address);
    *address+=Value.length();
    return Value;
    }else{
      Serial.println("outofrange");
    }
}

void EnergyMonitorClass::Write_String_in_ESP32(int* address, String str)
{
    if(*address+str.length()<=EEPROM_SIZE){ 
    EEPROM.writeString(*address,str);
    EEPROM.commit();
    *address+=str.length();
    }else{
      Serial.println("outofrange");
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

void EnergyMonitorClass::Change_Powertimer_config(unsigned int Factor){
  // Detach the existing ISR
  timerDetachInterrupt(Powertimer);
  timerAttachInterrupt(Powertimer, &PowertimerISR, true);
  timerAlarmWrite(Powertimer, Factor*PowertimerFactor, true);      
  timerAlarmEnable(Powertimer);
}

void EnergyMonitorClass::Change_Energytimer_config(unsigned int Factor){
  timerDetachInterrupt(Energytimer);
  timerAttachInterrupt(Energytimer, &EnergytimerISR, true);
  timerAlarmWrite(Energytimer, Factor*AccumulatedPowertimerFactor, true);      
  timerAlarmEnable(Energytimer);
}

void EnergyMonitorClass::Change_CFtimer_config(unsigned int Factor){
  timerDetachInterrupt(CFtimer);
  timerAttachInterrupt(CFtimer, &CFtimerISR, true);
  timerAlarmWrite(CFtimer, Factor*VCFtimerFactor, true);      
  timerAlarmEnable(CFtimer);
}

String EnergyMonitorClass::MeasureStartTime(){
  String Value=EEPROM.readString(MeasureStartTime_add);
  if(!(Value.charAt(Value.length()-1) == 'Z')){
    get_Time(&timeinfo);
    char str[64];
    strftime(str, 64, "%Y-%m-%dT%H:%M:%SZ", &timeinfo);
    EEPROM.writeString(MeasureStartTime_add,str);
    return str;
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
  
  StoreAddress=12;
  EEPROM.begin(EEPROM_SIZE);
  InitEnergyCounter();

  Interrupts_setup();
  

}
