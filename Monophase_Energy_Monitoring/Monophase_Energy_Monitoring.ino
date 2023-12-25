#define ARM_MATH_CM0PLUS

#include <SPI.h>
#include <WiFi.h>
#include  "ADE9153A.h"
#include "ADE9153AAPI.h"
#include "Interrupts.h"
#include "energy_monitor.h"
#include "SolarClass.h"
#include "GridClass.h"
#include "OTA.h"

/* Basic initializations */
#define SPI_SPEED 1000000     //SPI Speed
#define CS_PIN 5              //8-->Arduino Zero. 15-->ESP8266 
#define ADE9153A_RESET_PIN 14  //On-board Reset Pin
#define USER_INPUT 12          //On-board User Input Button Pin

ADE9153AClass ade9153A;
SolarClass monitor;

void resetADE9153A(void);
void disconnectWifi(void);

/*************************/

const char* ssid = "TOPNETF15A40D9";
const char* password = "98228782";

wl_status_t WifiStatus;

struct tm timeinfo;
extern volatile SemaphoreHandle_t powerSemaphore;
extern volatile SemaphoreHandle_t AccumulationSemaphore;
extern volatile SemaphoreHandle_t energySemaphore;
extern volatile SemaphoreHandle_t cfSemaphore;
extern volatile SemaphoreHandle_t THDSemaphore;

extern volatile EnergyRegs* Energy;
std::queue<struct dataNode> myQueue;

WiFiClient wifiClient;
MqttClient mqttClient(wifiClient);

const char broker[]="192.168.1.16";
int port =1883;

/*************************/


void setup() {
  /* Pin and serial monitor setup */
  //pinMode(USER_INPUT, INPUT);
  //pinMode(ADE9153A_RESET_PIN, OUTPUT);
  //digitalWrite(ADE9153A_RESET_PIN, HIGH);  
  Serial.begin(115200);

  resetADE9153A();            //Reset ADE9153A for clean startup
  delay(1000);
  /*SPI initialization and test*/
  bool commscheck = ade9153A.SPI_Init(SPI_SPEED,CS_PIN); //Initialize SPI
  if (!commscheck) {
    Serial.println("ADE9153A Shield not detected. Plug in Shield and reset the Arduino");
    while (!commscheck) {     //Hold until arduino is reset
      delay(1000);
    }
  }
  
  ade9153A.SetupADE9153A(); //Setup ADE9153A according to ADE9153AAPI.h
  /* Read and Print Specific Register using ADE9153A SPI Library */
  Serial.println(String(ade9153A.SPI_Read_32(REG_VERSION_PRODUCT), HEX)); // Version of IC
  ade9153A.SPI_Write_32(REG_AIGAIN, -268435456); //AIGAIN to -1 to account for IAP-IAN swap
  delay(500); 

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
    ade9153A.ReadPowerRegs(Power);
    monitor.PublishActivePower("ActivePower");
    monitor.PublishReactivePower("ReactivePower");
    monitor.PublishApparentPower("ApparentPower");
  }
  if (xSemaphoreTake(AccumulationSemaphore, 0) == pdTRUE){
    ///////  ENERGY  Read energy register with reset mode page(21)
    ade9153A.AccumulateActiveEnergy(Energy);
    ade9153A.AccumulateReactiveEnergy(Energy);
  }
  if (xSemaphoreTake(energySemaphore, 0) == pdTRUE){
    monitor.StoreCountedEnergy();
    monitor.PublishtotalActiveEnergy("totalActiveEnergy"); 
    ade9153A.InitActiveEnergy(Energy);
    monitor.PublishtotalReactiveEnergy("totalReactiveEnergy");
    ade9153A.InitReactiveEnergy(Energy);
  } 
  if (xSemaphoreTake(cfSemaphore, 0) == pdTRUE){
    ade9153A.ReadRMSRegs(RMS);
    ade9153A.ReadPQRegs(PQ);
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


void resetADE9153A(void)
{
 digitalWrite(ADE9153A_RESET_PIN, LOW);
 delay(100);
 digitalWrite(ADE9153A_RESET_PIN, HIGH);
 delay(1000);
 Serial.println("Reset Done");
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


