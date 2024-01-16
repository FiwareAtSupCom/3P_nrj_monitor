#include "energy_monitor.h"

extern volatile ActiveEnergyperH* ActiveEnergy;
extern volatile ReactiveEnergyperH* ReactiveEnergy;
extern WiFiClient wifiClient;
extern MqttClient mqttClient;
extern const char root_Topic[];


class SolarClass : public EnergyMonitorClass{

  public:

    SolarClass();

    void PublishtotalActiveEnergy();

    void PublishtotalReactiveEnergy();

};