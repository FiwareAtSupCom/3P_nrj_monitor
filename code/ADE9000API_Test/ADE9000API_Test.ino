#include <WiFi.h>
#include "energy_meter.h"
#include <SPI.h>
#include  "ADE9000RegMap.h"
#include "ADE9000API.h"
#include "Fiware.h"
#include <esp_task_wdt.h>
#include "OTA.h"
//#include "ADE9000CalibrationInputs.h"

/*Basic initializations*/
ADE9000Class ade9000;
#define SPI_SPEED 5000000     //SPI Speed
#define CS_PIN 8 //8-->Arduino Zero. 16-->ESP8266 
#define ADE9000_RESET_PIN 5 //Reset Pin on HW
#define PM_1 4              //PM1 Pin: 4-->Arduino Zero. 15-->ESP8266 

#define PowertimerFactor  1000 //1000 microsec
#define EnergytimerFactor  100000 //100 seconds
#define VCFtimerFactor  900000 //15 minutes


/*Function Decleration*/
void readRegisterData(void);
void readResampledData(void);
void resetADE9000(void);

/*WiFi settings*/
const char* ssid = "WiFi-EVENT";
const char* password = "Sup2023@Ev..";

wl_status_t WifiStatus;

const int wdtTimeout = 3000; 


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
  while (WiFi.status() != WL_CONNECTED) { /// limited attempts!!!!
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
  WifiStatus=WiFi.status();
  energy_meter_setup(); 
  // Configure the watchdog timer with a 10-second timeout
  esp_task_wdt_init(10, true);
  // Enable the watchdog timer
  esp_task_wdt_add(NULL);
  //OTA
  OTA_setup();
}

void loop() {

  // Reset the watchdog timer to prevent a reset
  esp_task_wdt_reset();
  if (WiFi.status()!= WifiStatus){
    if (WiFi.status() != WL_CONNECTED){
    //WiFi.begin(ssid, password);
    //delay(1000);
    Change_Powertimer_config(2*PowertimerFactor);
  }else{
    Send_Stocked_Data();
    Change_Powertimer_config(PowertimerFactor);
  }
  }
  
  ArduinoOTA.handle();
  delay(10000);
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







