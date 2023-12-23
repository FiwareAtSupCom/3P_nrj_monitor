#include <WiFi.h>
#include <SPI.h>
#include  "ADE9000RegMap.h"
#include "ADE9000API.h"
#include "Interrupts.h"
#include "energy_monitor.h"
#include "OTA.h"

/*Basic initializations*/
ADE9000Class ade9000;
EnergyMonitorClass monitor;
#define SPI_SPEED 5000000     //SPI Speed
#define CS_PIN 8 //8-->Arduino Zero. 16-->ESP8266 
#define ADE9000_RESET_PIN 5 //Reset Pin on HW
#define PM_1 4              //PM1 Pin: 4-->Arduino Zero. 15-->ESP8266 

/*Function Decleration*/
void readRegisterData(void);
void readResampledData(void);
void resetADE9000(void);

/*WiFi settings*/
const char* ssid = "ssid";
const char* password = "********";

wl_status_t WifiStatus;

struct tm timeinfo;
extern volatile SemaphoreHandle_t powerSemaphore;
extern volatile SemaphoreHandle_t energySemaphore;
extern volatile SemaphoreHandle_t cfSemaphore;
extern volatile SemaphoreHandle_t THDSemaphore;

extern int StoreAddress;
extern volatile ActiveEnergyperH* ActiveEnergy;
extern volatile ReactiveEnergyperH* ReactiveEnergy;
std::queue<struct dataNode> myQueue;

WiFiClient wifiClient;
MqttClient mqttClient(wifiClient);

/*Static address of broker*/
const char broker[]="192.168.1.10";
int port =1883;

extern volatile ActivePowerRegs* ActivePower;
extern volatile ReactivePowerRegs* ReactivePower;
extern volatile ApparentPowerRegs* ApparentPower;

extern volatile ActiveEnergyperH* ActiveEnergy;

extern volatile VoltageRMSRegs* VoltageRMS;
extern volatile CurrentRMSRegs* CurrentRMS;
extern volatile PowerFactorRegs* PowerFactor;
extern volatile PeriodRegs* Frequency;

extern volatile CurrentTHDRegs* CurrentTHD;
extern volatile VoltageTHDRegs* VoltageTHD;

void setup() 
{
  Serial.begin(115200);
  pinMode(PM_1, OUTPUT);    //Set PM1 pin as output 
  digitalWrite(PM_1, LOW);   //Set PM1 select pin low for PSM0 mode
  pinMode(ADE9000_RESET_PIN, OUTPUT);
  digitalWrite(ADE9000_RESET_PIN, HIGH); 
  void resetADE9000(); 
  delay(1000);
  ade9000.SPI_Init(SPI_SPEED,CS_PIN); //Initialize SPI
  ade9000.SetupADE9000();             //Initialize ADE9000 registers according to values in ADE9000API.h
  ade9000.SPI_Write_16(ADDR_RUN,0x1); //Set RUN=1 to turn on DSP. Uncomment if SetupADE9000 function is not used
  Serial.print("RUN Register: ");
  Serial.println(ade9000.SPI_Read_16(ADDR_RUN),HEX);
   // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) { 
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
  WifiStatus=WiFi.status();

  while (!mqttClient.connect(broker,port)){
    Serial.println("Mqtt connection failed! Error code =");
    Serial.println(mqttClient.connectError());
  }
  Serial.println("Connected to MQTT Broker");
  

    powerSemaphore = xSemaphoreCreateBinary();
    energySemaphore = xSemaphoreCreateBinary();
    cfSemaphore = xSemaphoreCreateBinary();
    THDSemaphore = xSemaphoreCreateBinary();
  //button
  pinMode(0, INPUT);
  pinMode(2, OUTPUT);
  //OTA
  OTA_setup();
  Time_setup();
  while(sntp_get_sync_status()!=SNTP_SYNC_STATUS_COMPLETED){
    Serial.println("synchronizing time");
    delay(1000);
  }
  monitor.energy_meter_setup(); 
  
}

void loop() {

  if (WiFi.status()!= WifiStatus){
    if (WiFi.status() != WL_CONNECTED){
    /*wifi reconnect*/
    WiFi.reconnect();
    digitalWrite(2, HIGH);
    monitor.Change_timers_config(3);
  
  }else{
    digitalWrite(2, LOW);
    if (!mqttClient.connected()){
    mqttClient.connect(broker,port);
    }
     monitor.send_data(myQueue);
     monitor.Change_timers_config(1);
  }
  WifiStatus=WiFi.status();
  }
  /*verify connection with the broker*/
  if (!mqttClient.connected()){
    mqttClient.connect(broker,port);
  }
  ArduinoOTA.handle();
  get_Time(&timeinfo);
  if (xSemaphoreTake(powerSemaphore, 0) == pdTRUE){
    if (WiFi.status() == WL_CONNECTED){
      monitor.PublishActivePower("ActivePower");
    }else{
       monitor.store_data("ActivePower/L1",&(ActivePower->ActivePowerReg_A),myQueue);
       monitor.store_data("ActivePower/L2",&(ActivePower->ActivePowerReg_B),myQueue);
       monitor.store_data("ActivePower/L3",&(ActivePower->ActivePowerReg_C),myQueue);
       
    } 
  }
  if (xSemaphoreTake(energySemaphore, 0) == pdTRUE){
    if (WiFi.status() == WL_CONNECTED){
    monitor.PublishtotalActiveEnergy(ImportorExport); 
    monitor.StoreCountedEnergy();
    ade9000.InitActiveEnergy(ActiveEnergy);
    monitor.PublishtotalReactiveEnergy(ImportorExport );
    ade9000.InitReactiveEnergy(ReactiveEnergy);
    }else{
       monitor.store_data(ImportorExport,&(ActiveEnergy->ActiveEnergy_A),myQueue);
       monitor.store_data(ImportorExport,&(ActiveEnergy->ActiveEnergy_B),myQueue);
       monitor.store_data(ImportorExport,&(ActiveEnergy->ActiveEnergy_C),myQueue);
    } 
  } 
  if (xSemaphoreTake(cfSemaphore, 0) == pdTRUE){
    if (WiFi.status() == WL_CONNECTED){
    monitor.PublishpowerFactor("powerFactor"); 
    monitor.Publishvoltage("voltage");
    monitor.Publishcurrent("current");
    monitor.Publishfrequency("frequency");
    }else{
       monitor.store_data("powerFactor/L1",&(PowerFactor->PowerFactorValue_A),myQueue);
       monitor.store_data("powerFactor/L2",&(PowerFactor->PowerFactorValue_B),myQueue);
       monitor.store_data("powerFactor/L3",&(PowerFactor->PowerFactorValue_C),myQueue);
       monitor.store_data("voltage/L1",&(VoltageRMS->VoltageRMSReg_A),myQueue);
       monitor.store_data("voltage/L2",&(VoltageRMS->VoltageRMSReg_B),myQueue);
       monitor.store_data("voltage/L3",&(VoltageRMS->VoltageRMSReg_C),myQueue);
    } 
  } 
  if (xSemaphoreTake(THDSemaphore, 0) == pdTRUE){
    if (WiFi.status() == WL_CONNECTED){
    monitor.PublishvoltageTHD("voltageTHD");
    monitor.PublishcurrentTHD("currentTHD");
    }else{
       monitor.store_data("voltageTHD/L1",&(VoltageTHD->VoltageTHDValue_A),myQueue);
       monitor.store_data("voltageTHD/L2",&(VoltageTHD->VoltageTHDValue_B),myQueue);
       monitor.store_data("voltageTHD/L3",&(VoltageTHD->VoltageTHDValue_C),myQueue);
       monitor.store_data("currentTHD/L1",&(CurrentTHD->CurrentTHDValue_A),myQueue);
       monitor.store_data("currentTHD/L2",&(CurrentTHD->CurrentTHDValue_B),myQueue);
       monitor.store_data("currentTHD/L3",&(CurrentTHD->CurrentTHDValue_C),myQueue);
    } 
  }

  // If button is pressed
  if (digitalRead(0) == LOW) {
    
    if (WiFi.status() == WL_CONNECTED) {
      
      Serial.println("wifi OFF");
      WiFi.disconnect();
      delay(1000);
    }else{
      Serial.println("wifi ON");
      WiFi.reconnect();
      delay(1000);
    }
  }
  
  yield();

}



void resetADE9000(void)
{
 digitalWrite(ADE9000_RESET_PIN, LOW);
 delay(50);
 digitalWrite(ADE9000_RESET_PIN, HIGH);
 delay(1000);
 Serial.println("Reset Done");
}
