#include "SolarClass.h"


SolarClass::SolarClass()
{
	
}

void SolarClass::PublishtotalActiveEnergy(char* sub_Topic){

  char topic[100];
  snprintf(topic, sizeof(topic), "%s/Energy/%sExport/L1", root_Topic, sub_Topic);
  mqttClient.beginMessage(topic);
  mqttClient.print(topic);
  mqttClient.print((Energy->ActiveEnergyValue)*EnergyConversionConstant);
  mqttClient.endMessage();
  snprintf(topic, sizeof(topic), "%s/Energy/%sExport/L2", root_Topic, sub_Topic);
  mqttClient.beginMessage(topic);
  mqttClient.print((Energy->ActiveEnergyValue)*EnergyConversionConstant);
  mqttClient.endMessage();
  snprintf(topic, sizeof(topic), "%s/Energy/%sExport/L3", root_Topic, sub_Topic);
  mqttClient.beginMessage(topic);
  mqttClient.print((Energy->ActiveEnergyValue)*EnergyConversionConstant);
  mqttClient.endMessage();
  PublishdateObserved("dateObserved");
  
}

void SolarClass::PublishtotalReactiveEnergy(char* sub_Topic){

  char topic[100];
  snprintf(topic, sizeof(topic), "%s/Energy/%sExport/L1", root_Topic, sub_Topic);
  mqttClient.beginMessage(topic);
  mqttClient.print(topic);
  mqttClient.print((Energy->FundReactiveEnergyValue)*EnergyConversionConstant);
  mqttClient.endMessage();
  snprintf(topic, sizeof(topic), "%s/Energy/%sExport/L2", root_Topic, sub_Topic);
  mqttClient.beginMessage(topic);
  mqttClient.print((Energy->FundReactiveEnergyValue)*EnergyConversionConstant);
  mqttClient.endMessage();
  snprintf(topic, sizeof(topic), "%s/Energy/%sExport/L3", root_Topic, sub_Topic);
  mqttClient.beginMessage(topic);
  mqttClient.print((Energy->FundReactiveEnergyValue)*EnergyConversionConstant);
  mqttClient.endMessage();
  PublishdateObserved("dateObserved");
  
}