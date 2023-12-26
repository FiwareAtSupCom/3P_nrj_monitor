#ifndef ENERGY_MONITOR_H
#define ENERGY_MONITOR_H



#include "ADE9153AAPI.h"
#include "RealTime.h"
#include "Interrupts.h"
#include "defined.h"
#include <WiFi.h>
#include <EEPROM.h>
#include <ArduinoMqttClient.h>
#include <queue>

extern const char broker[];
extern int port;

extern hw_timer_t * Powertimer; 
extern hw_timer_t * Energytimer;
extern hw_timer_t * CFtimer;
extern hw_timer_t * THDtimer;

extern struct tm timeinfo;

extern WiFiClient wifiClient;
extern MqttClient mqttClient;

extern std::queue<struct dataNode> myQueue;

extern const char root_Topic[];

struct dataNode
 {
	char* topic;
  volatile void* data; 	
  char time[21];
 }; 

class EnergyMonitorClass
{
  private:

    void PublishtotalActivePower(char* sub_Topic);

    void PublishtotalReactivePower(char* sub_Topic);

    void PublishtotalApparentPower(char* sub_Topic);

    void store_data(char* sub_Topic,volatile void* data,std::queue<struct dataNode>& myQueue);


	public:
		EnergyMonitorClass();

    void handleWifiStatus(wl_status_t* WifiStatus);

    void checkBroker();

    void InitEnergyCounter();

    void StoreCountedEnergy();

    float_t ReadEnergyCounter();

    void send_data(std::queue<struct dataNode>& myQueue);

    void Change_timers_config(unsigned int Factor);

    String MeasureStartTime();

    void energy_meter_setup();

    void PublishdateObserved(char* sub_Topic);

    void PublishphaseType(char* sub_Topic,char* type);

    virtual void PublishtotalActiveEnergy(char* sub_Topic)=0;

    virtual void PublishtotalReactiveEnergy(char* sub_Topic)=0;

    void PublishActivePower(char* sub_Topic);

    void PublishReactivePower(char* sub_Topic);

    void PublishApparentPower(char* sub_Topic);

    void PublishpowerFactor(char* sub_Topic);

    void PublishdisplacementpowerFactor(char* sub_Topic);

    void Publishvoltage(char* sub_Topic);

    void Publishcurrent(char* sub_Topic);

    void Publishfrequency(char* sub_Topic);


};

#endif // ENERGY_MONITOR_H
