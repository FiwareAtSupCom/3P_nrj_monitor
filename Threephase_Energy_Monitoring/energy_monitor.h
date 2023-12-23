#include "ADE9000API.h"
#include "RealTime.h"
#include "Interrupts.h"
#include "defined.h"
#include <WiFi.h>
#include <EEPROM.h>
#include <ArduinoMqttClient.h>
#include <queue>


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


	public:
		EnergyMonitorClass();

    ///

    String Read_String_From_ESP32(int* address);

    void Write_String_in_ESP32(int* address, String str);

    void InitEnergyCounter();

    void StoreCountedEnergy();

    void ReadEnergyCounter();

    void store_data(char* sub_Topic,volatile void* data,std::queue<struct dataNode>& myQueue);

    void send_data(std::queue<struct dataNode>& myQueue);

    void Change_timers_config(unsigned int Factor);

    String MeasureStartTime();

    void energy_meter_setup();

    ///

    void PublishdateObserved(char* sub_Topic);

    void PublishphaseType(char* sub_Topic,char* type);

    void PublishtotalActiveEnergy(char* sub_Topic);

    void PublishtotalReactiveEnergy(char* sub_Topic);

    void PublishActivePower(char* sub_Topic);

    void PublishReactivePower(char* sub_Topic);

    void PublishApparentPower(char* sub_Topic);

    void PublishpowerFactor(char* sub_Topic);

    void PublishdisplacementpowerFactor(char* sub_Topic);

    void Publishvoltage(char* sub_Topic);

    void Publishcurrent(char* sub_Topic);

    void PublishvoltagePhaseToPhase(char* sub_Topic);

    void Publishfrequency(char* sub_Topic);

    void PublishvoltageTHD(char* sub_Topic);

    void PublishcurrentTHD(char* sub_Topic);

};
