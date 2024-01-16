#include "energy_monitor.h"

extern volatile ActiveEnergyperH* ActiveEnergy;
extern volatile ReactiveEnergyperH* ReactiveEnergy;
extern WiFiClient wifiClient;
extern MqttClient mqttClient;
extern const char root_Topic[];


class GridClass : public EnergyMonitorClass{

  public:

    GridClass();

    void PublishtotalActiveEnergy();

    void PublishtotalReactiveEnergy();

};