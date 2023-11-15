#include "energy_meter.h"
#include <WiFi.h>

#define PowertimerFactor  1000 //1000 microsec
#define EnergytimerFactor  100000 //100 seconds
#define VCFtimerFactor  900000 //15 minutes


#define PowerConversionConstant 3.6097 //(mWATT/mVAR/mVA)/LSB

#define EnergyConversionConstant 1.0268 //( WATTHr/ VARHr/ VAHr)/LSB

#define VoltageConversionConstant 13.4225

#define CurrentConversionConstant 2.0036 

#define EEPROM_SIZE 512

#define PowerId 0x0A

#define EnergyId 0x0B

#define PowerfactorId 0x0C

#define type "Grid"

uint8_t nb;
uint32_t SentEnergy;

hw_timer_t * Powertimer = NULL; 
hw_timer_t * Energytimer = NULL;
hw_timer_t * VCFtimer = NULL;

ActivePowerRegs* ActivePower;
uint32_t* ActiveEnergy;
VoltageRMSRegs* VoltageRMS;
CurrentRMSRegs* CurrentRMS;
PowerFactorRegs* PowerFactor;
int StoreAddress;

TimeDate* timeinfo;

extern char FIWARE_DEVICE[] ;



void IRAM_ATTR PowertimerISR() {
   // Create a JSON object
  DynamicJsonDocument jsonDoc(200); // Adjust the size as needed
  String jsonStr;
  ///////  POWER
    ade9000.ReadActivePowerRegs(ActivePower);
    get_Time(timeinfo);
    if(WiFi.status() != WL_CONNECTED){
  
    EEPROM.writeString(StoreAddress,timeinfo->timeStamp);
    EEPROM.writeString(StoreAddress,timeinfo->dayStamp);
    Store_In_ESP32(&StoreAddress,(ActivePower->ActivePowerReg_C)*PowerConversionConstant);
    Store_In_ESP32(&StoreAddress,(ActivePower->ActivePowerReg_B)*PowerConversionConstant);
    Store_In_ESP32(&StoreAddress,(ActivePower->ActivePowerReg_A)*PowerConversionConstant);
    Store_In_ESP32(&StoreAddress,PowerId);
    
    }else{
    jsonDoc["id"] = FIWARE_DEVICE;
    jsonDoc["type"] = type;
    jsonDoc["ActivePowerReg_A"] = (ActivePower->ActivePowerReg_A)*PowerConversionConstant;
    jsonDoc["ActivePowerReg_B"] = (ActivePower->ActivePowerReg_B)*PowerConversionConstant;
    jsonDoc["ActivePowerReg_C"] = (ActivePower->ActivePowerReg_C)*PowerConversionConstant;
    jsonDoc["dayStamp"]= timeinfo->dayStamp;
    jsonDoc["timeStamp"]= timeinfo->timeStamp;
    // Print measures
    Serial.println((ActivePower->ActivePowerReg_A)*PowerConversionConstant);
    Serial.println((ActivePower->ActivePowerReg_B)*PowerConversionConstant);
    Serial.println((ActivePower->ActivePowerReg_C)*PowerConversionConstant);
    Serial.println(timeinfo->dayStamp);
    Serial.println(timeinfo->timeStamp);
     // Serialize the JSON object to a string
    serializeJson(jsonDoc, jsonStr);
    postMeasures(jsonStr);
  }
}

void IRAM_ATTR EnergytimerISR() {

    ///////  ENERGY  Read energy register with reset mode page(21)
    ade9000.ReadAccumulatedActiveRegs(ActiveEnergy);
    SentEnergy+=*ActiveEnergy;
    if (nb==36){
     // Create a JSON object
    DynamicJsonDocument jsonDoc(200); // Adjust the size as needed
    String jsonStr;
    get_Time(timeinfo);

    if(WiFi.status() != WL_CONNECTED){
    
    EEPROM.writeString(StoreAddress,timeinfo->timeStamp);
    EEPROM.writeString(StoreAddress,timeinfo->dayStamp);
    Store_In_ESP32(&StoreAddress,SentEnergy*EnergyConversionConstant);
    Store_In_ESP32(&StoreAddress,EnergyId);
    
    }else{
    jsonDoc["id"] = FIWARE_DEVICE;
    jsonDoc["type"] = type;
    jsonDoc["AccumulatedActiveEnergy"] = SentEnergy*EnergyConversionConstant;
    jsonDoc["dayStamp"]= timeinfo->dayStamp;
    jsonDoc["timeStamp"]= timeinfo->timeStamp;
    // Print measure
    Serial.println(SentEnergy*EnergyConversionConstant);
    Serial.println(timeinfo->dayStamp);
    Serial.println(timeinfo->timeStamp);
    
    serializeJson(jsonDoc, jsonStr);
    postMeasures(jsonStr);
    }
    Store_Energy_In_Flash(SentEnergy);
    nb=0;
    SentEnergy=0;
    if (((timeinfo->dayStamp).equals("28")) && ((timeinfo->timeStamp).equals("01:00:00")<0)){
      Send_Energy_From_Flash();
    }
    }
}

void IRAM_ATTR VCFtimerISR() {
  
   // Create a JSON object
  DynamicJsonDocument jsonDoc(200); // Adjust the size as needed
  String jsonStr;
  ///////  Voltage Current PowerFactor
    
    ade9000.ReadVoltageRMSRegs(VoltageRMS);
    ade9000.ReadCurrentRMSRegs(CurrentRMS);
    ade9000.ReadPowerFactorRegsnValues(PowerFactor);
    get_Time(timeinfo);
    if(WiFi.status() != WL_CONNECTED){
    
    EEPROM.writeString(StoreAddress,timeinfo->timeStamp);
    EEPROM.writeString(StoreAddress,timeinfo->dayStamp);
    Store_In_ESP32(&StoreAddress,PowerFactor->PowerFactorValue_C);
    Store_In_ESP32(&StoreAddress,PowerFactor->PowerFactorValue_B);
    Store_In_ESP32(&StoreAddress,PowerFactor->PowerFactorValue_A);
    Store_In_ESP32(&StoreAddress,PowerfactorId);
    
    }else{
    jsonDoc["id"] = FIWARE_DEVICE;
    jsonDoc["type"] = type;

    jsonDoc["VoltageRMS_A"] = (VoltageRMS->VoltageRMSReg_A)*VoltageConversionConstant;
    jsonDoc["VoltageRMS_B"] = (VoltageRMS->VoltageRMSReg_B)*VoltageConversionConstant;
    jsonDoc["VoltageRMS_C"] = (VoltageRMS->VoltageRMSReg_C)*VoltageConversionConstant;

    jsonDoc["CurrentRMS_A"] = (CurrentRMS->CurrentRMSReg_A)*CurrentConversionConstant;
    jsonDoc["CurrentRMS_B"] = (CurrentRMS->CurrentRMSReg_B)*CurrentConversionConstant;
    jsonDoc["CurrentRMS_C"] = (CurrentRMS->CurrentRMSReg_C)*CurrentConversionConstant;

    jsonDoc["PowerFactor_A"] = (PowerFactor->PowerFactorValue_A);
    jsonDoc["PowerFactor_B"] = (PowerFactor->PowerFactorValue_B);
    jsonDoc["PowerFactor_C"] = (PowerFactor->PowerFactorValue_C);

    jsonDoc["dayStamp"]= timeinfo->dayStamp;
    jsonDoc["timeStamp"]= timeinfo->timeStamp;
    
    // Print measures
    Serial.println((VoltageRMS->VoltageRMSReg_A)*PowerConversionConstant);
    Serial.println((VoltageRMS->VoltageRMSReg_B)*PowerConversionConstant);
    Serial.println((VoltageRMS->VoltageRMSReg_C)*PowerConversionConstant);

    Serial.println((CurrentRMS->CurrentRMSReg_A)*CurrentConversionConstant);
    Serial.println((CurrentRMS->CurrentRMSReg_B)*CurrentConversionConstant);
    Serial.println((CurrentRMS->CurrentRMSReg_C)*CurrentConversionConstant);

    Serial.println(PowerFactor->PowerFactorValue_A);
    Serial.println(PowerFactor->PowerFactorValue_B);
    Serial.println(PowerFactor->PowerFactorValue_C);

    Serial.println(timeinfo->dayStamp);
    Serial.println(timeinfo->timeStamp);
     // Serialize the JSON object to a string
    serializeJson(jsonDoc, jsonStr);
    postMeasures(jsonStr);
  }
}


void Store_In_ESP32(int* address ,float value)
{
    if(sizeof(value)+*address<=EEPROM_SIZE){
    EEPROM.writeFloat(*address,value);
    EEPROM.commit();
    *address+=sizeof(float); 
    }else{
      Serial.println("EEPROM overflow");
    }
}

float Read_From_ESP32(int* address)
{
    if(*address<=EEPROM_SIZE){
    float Value=EEPROM.readFloat(*address);
    EEPROM.writeFloat(*address,0);
    *address-=sizeof(float); 
    return Value;
    }else{
      Serial.println("outofrange");
    }
}

String Read_String_From_ESP32(int* address)
{
    if(*address<=EEPROM_SIZE){
    String Value=EEPROM.readString(*address);
    EEPROM.writeFloat(*address,0);
    *address-=sizeof(Value); 
    return Value;
    }else{
      Serial.println("outofrange");
    }
}


void Store_Energy_In_Flash(float value)
{
    float StoredValue=EEPROM.readFloat(0);
    float TotalEnergy= StoredValue+value;
    EEPROM.writeFloat(0,TotalEnergy); 
    EEPROM.commit();
}

void Send_Energy_From_Flash()
{
    float StoredValue=EEPROM.readFloat(0);
    DynamicJsonDocument jsonDoc(200); // Adjust the size as needed
    String jsonStr;
    jsonDoc["Totalconsumption"]= StoredValue;
    // Serialize the JSON object to a string
    serializeJson(jsonDoc, jsonStr);
    postMeasures(jsonStr);
}

void Send_Stocked_Data(){
  
  if (StoreAddress!=4){
     DynamicJsonDocument jsonDoc(200); // Adjust the size as needed
     String jsonStr;
     uint8_t id;
      id = EEPROM.read(StoreAddress);
      StoreAddress-=1; 
      if (id == 0x0A){
        jsonDoc["ActivePowerReg_A"] = Read_From_ESP32(&StoreAddress);
        jsonDoc["ActivePowerReg_B"] = Read_From_ESP32(&StoreAddress);
        jsonDoc["ActivePowerReg_C"] = Read_From_ESP32(&StoreAddress);
        jsonDoc["dayStamp"]= Read_String_From_ESP32(&StoreAddress);
        jsonDoc["timeStamp"]= Read_String_From_ESP32(&StoreAddress);
      }else if (id == 0x0B) {
        jsonDoc["AccumulatedActiveEnergy"] = Read_From_ESP32(&StoreAddress);
        jsonDoc["dayStamp"]= Read_String_From_ESP32(&StoreAddress);
        jsonDoc["timeStamp"]= Read_String_From_ESP32(&StoreAddress);
      }else {
        jsonDoc["PowerFactor_A"] = Read_From_ESP32(&StoreAddress);
        jsonDoc["PowerFactor_B"] = Read_From_ESP32(&StoreAddress);
        jsonDoc["PowerFactor_C"] = Read_From_ESP32(&StoreAddress);

        jsonDoc["dayStamp"]= Read_String_From_ESP32(&StoreAddress);
        jsonDoc["timeStamp"]= Read_String_From_ESP32(&StoreAddress);
      }
      
    // Serialize the JSON object to a string
      serializeJson(jsonDoc, jsonStr);
      postMeasures(jsonStr);
  }
}

void Change_Powertimer_config(unsigned int timerFactor){

  timerAlarmWrite(Powertimer, timerFactor, true);      
  timerAlarmEnable(Powertimer);
}

void Change_Energytimer_config(unsigned int timerFactor){

  timerAlarmWrite(Energytimer, timerFactor, true);      
  timerAlarmEnable(Energytimer);
}



void energy_meter_setup()
{

  nb=0;
  SentEnergy=0;

  ActivePower= new ActivePowerRegs;
  *ActiveEnergy=0;
  VoltageRMS= new VoltageRMSRegs;
  CurrentRMS= new CurrentRMSRegs;
  PowerFactor= new PowerFactorRegs;
  timeinfo= new TimeDate;
  
  StoreAddress=4;
  EEPROM.begin(EEPROM_SIZE);

  // Timer 1: 1 second interval
  Powertimer = timerBegin(0, 80, true);                //Begin timer with 1 MHz frequency (80MHz/80) = 1 microsecond
  timerAttachInterrupt(Powertimer, &PowertimerISR, true);   //Attach the interrupt to Timer1
  timerAlarmWrite(Powertimer, PowertimerFactor, true);      //Initialize the timer
  timerAlarmEnable(Powertimer); 


  // Timer 2: 106s interval
  Energytimer = timerBegin(1, 80, true);                //Begin timer with 1 MHz frequency (80MHz/80)
  timerAttachInterrupt(Energytimer, &EnergytimerISR, true);   //Attach the interrupt to Timer1
  timerAlarmWrite(Energytimer, EnergytimerFactor, true);      //Initialize the timer
  timerAlarmEnable(Energytimer); 

   // Timer 3: 15 minutes interval
  VCFtimer = timerBegin(0, 80, true);                //Begin timer with 1 MHz frequency (80MHz/80) = 1 microsecond
  timerAttachInterrupt(VCFtimer, &VCFtimerISR, true);   //Attach the interrupt to Timer1
  timerAlarmWrite(VCFtimer, VCFtimerFactor, true);      //Initialize the timer
  timerAlarmEnable(VCFtimer); 
  

}

