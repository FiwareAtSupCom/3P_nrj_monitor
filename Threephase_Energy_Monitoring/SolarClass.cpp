#include "SolarClass.h"


SolarClass::SolarClass()
{
	
}

void SolarClass::PublishtotalActiveEnergy(char* sub_Topic){

  char topic[100];
  snprintf(topic, sizeof(topic), "%s/Energy/%sExport/L1", root_Topic, sub_Topic);
  mqttClient.beginMessage(topic);
  mqttClient.print(topic);
  mqttClient.print((ActiveEnergy->ActiveEnergy_A)*EnergyConversionConstant);
  mqttClient.endMessage();
  snprintf(topic, sizeof(topic), "%s/Energy/%sExport/L2", root_Topic, sub_Topic);
  mqttClient.beginMessage(topic);
  mqttClient.print((ActiveEnergy->ActiveEnergy_B)*EnergyConversionConstant);
  mqttClient.endMessage();
  snprintf(topic, sizeof(topic), "%s/Energy/%sExport/L3", root_Topic, sub_Topic);
  mqttClient.beginMessage(topic);
  mqttClient.print((ActiveEnergy->ActiveEnergy_C)*EnergyConversionConstant);
  mqttClient.endMessage();
  PublishdateObserved("dateObserved");
  
}

void SolarClass::PublishtotalReactiveEnergy(char* sub_Topic){

  char topic[100];
  snprintf(topic, sizeof(topic), "%s/Energy/%sExport/L1", root_Topic, sub_Topic);
  mqttClient.beginMessage(topic);
  mqttClient.print(topic);
  mqttClient.print((ReactiveEnergy->ReactiveEnergy_A)*EnergyConversionConstant);
  mqttClient.endMessage();
  snprintf(topic, sizeof(topic), "%s/Energy/%sExport/L2", root_Topic, sub_Topic);
  mqttClient.beginMessage(topic);
  mqttClient.print((ReactiveEnergy->ReactiveEnergy_B)*EnergyConversionConstant);
  mqttClient.endMessage();
  snprintf(topic, sizeof(topic), "%s/Energy/%sExport/L3", root_Topic, sub_Topic);
  mqttClient.beginMessage(topic);
  mqttClient.print((ReactiveEnergy->ReactiveEnergy_C)*EnergyConversionConstant);
  mqttClient.endMessage();
  PublishdateObserved("dateObserved");
  
}