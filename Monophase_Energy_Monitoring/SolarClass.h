#include "energy_monitor.h"

extern EnergyRegs* Energy;
extern WiFiClient wifiClient;
extern MqttClient mqttClient;
extern const char root_Topic[];


class SolarClass : public EnergyMonitorClass{

  public:

    SolarClass();

    void PublishtotalActiveEnergy(char* sub_Topic);

    void PublishtotalReactiveEnergy(char* sub_Topic);

};