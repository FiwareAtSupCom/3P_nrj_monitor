#include <WiFi.h>
#include <SPI.h>
#include  "ADE9000RegMap.h"
#include "ADE9000API.h"
#include "Interrupts.h"
#include "energy_monitor.h"
#include "SolarClass.h"
#include "GridClass.h"
#include "OTA.h"

/*Basic initializations*/
ADE9000Class ade9000;
SolarClass monitor;

#define SPI_SPEED 5000000     //SPI Speed
#define CS_PIN 5 //8-->Arduino Zero. 16-->ESP8266 
#define ADE9000_RESET_PIN 14 //Reset Pin on HW
#define PM_1 12              //PM1 Pin: 4-->Arduino Zero. 15-->ESP8266 

/*Function Decleration*/
void readRegisterData(void);
void readResampledData(void);
void resetADE9000(void);

/*WiFi settings*/
const char* ssid = "ssid";
const char* password = "**********";

/*Static address of broker*/
const char broker[]="192.168.1.16";
int port =1883;

wl_status_t WifiStatus;

struct tm timeinfo;
extern volatile SemaphoreHandle_t powerSemaphore;
extern volatile SemaphoreHandle_t AccumulationSemaphore;
extern volatile SemaphoreHandle_t energySemaphore;
extern volatile SemaphoreHandle_t cfSemaphore;
extern volatile SemaphoreHandle_t THDSemaphore;


std::queue<struct dataNode> myQueue;

WiFiClient wifiClient;
MqttClient mqttClient(wifiClient);


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

	if ((ade9000.SPI_Read_16(ADDR_RUN) != 0x01)) {
    Serial.println("ADE9153A Shield not detected. Plug in Shield and reset the Arduino");
    while ((ade9000.SPI_Read_16(ADDR_RUN) != 0x01)) {     //Hold until arduino is reset
      delay(1000);
    }
  }
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
  AccumulationSemaphore=xSemaphoreCreateBinary();
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

  monitor.handleWifiStatus(&WifiStatus);

  monitor.checkBroker();

  ArduinoOTA.handle();
  get_Time(&timeinfo);
  if (xSemaphoreTake(powerSemaphore, 0) == pdTRUE){
    ade9000.ReadActivePowerRegs(ActivePower);
    ade9000.ReadReactivePowerRegs(ReactivePower);
    ade9000.ReadApparentPowerRegs(ApparentPower);
    monitor.PublishActivePower("ActivePower");
    monitor.PublishReactivePower("ReactivePower");
    monitor.PublishApparentPower("ApparentPower");
  }
  if (xSemaphoreTake(AccumulationSemaphore, 0) == pdTRUE){
    ///////  ENERGY  Read energy register with reset mode page(21)
    ade9000.AccumulateActiveEnergy(ActiveEnergy);
    ade9000.AccumulateReactiveEnergy(ReactiveEnergy);
  }
  if (xSemaphoreTake(energySemaphore, 0) == pdTRUE){
    monitor.StoreCountedEnergy();
    monitor.PublishtotalActiveEnergy("totalActiveEnergy"); 
    ade9000.InitActiveEnergy(ActiveEnergy);
    monitor.PublishtotalReactiveEnergy("totalReactiveEnergy");
    ade9000.InitReactiveEnergy(ReactiveEnergy);
  } 
  if (xSemaphoreTake(cfSemaphore, 0) == pdTRUE){
    ade9000.ReadCurrentRMSRegs(CurrentRMS);
    ade9000.ReadVoltageRMSRegs(VoltageRMS);
    ade9000.ReadPowerFactorRegsnValues(PowerFactor);
    ade9000.ReadPeriodRegsnValues(Frequency);
    monitor.PublishpowerFactor("powerFactor"); 
    monitor.Publishvoltage("voltage");
    monitor.Publishcurrent("current");
    monitor.Publishfrequency("frequency");
  } 
  if (xSemaphoreTake(THDSemaphore, 0) == pdTRUE){
    
  }

  disconnectWifi();
  
  //Serial.println(monitor.ReadEnergyCounter());
  //Serial.println(monitor.MeasureStartTime());
  
  yield();

}

void disconnectWifi(void){
  // If button is pressed
  if (digitalRead(0) == LOW) {
    
    if (WiFi.status() == WL_CONNECTED) {
      
      Serial.println("wifi OFF");
      WiFi.disconnect();
      delay(100);
    }
  }
}



void resetADE9000(void)
{
 digitalWrite(ADE9000_RESET_PIN, LOW);
 delay(50);
 digitalWrite(ADE9000_RESET_PIN, HIGH);
 delay(1000);
 Serial.println("Reset Done");
}
