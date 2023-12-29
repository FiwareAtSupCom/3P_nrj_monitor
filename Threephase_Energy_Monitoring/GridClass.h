#include "energy_monitor.h"

extern ActiveEnergyperH* ActiveEnergy;
extern ReactiveEnergyperH* ReactiveEnergy;
extern WiFiClient wifiClient;
extern MqttClient mqttClient;
extern const char root_Topic[];


class GridClass : public EnergyMonitorClass{

  public:

    GridClass();

    void PublishtotalActiveEnergy(char* sub_Topic);

    void PublishtotalReactiveEnergy(char* sub_Topic);

};