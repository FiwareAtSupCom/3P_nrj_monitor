#include "GridClass.h"


GridClass::GridClass()
{
	
}


void GridClass::PublishtotalActiveEnergy(char* sub_Topic){

  char topic[100];
  snprintf(topic, sizeof(topic), "%s/Energy/%sImport/L1", root_Topic, sub_Topic);
  mqttClient.beginMessage(topic);
  mqttClient.print(topic);
  mqttClient.print((Energy->ActiveEnergyValue)*EnergyConversionConstant);
  mqttClient.endMessage();
  snprintf(topic, sizeof(topic), "%s/Energy/%sImport/L2", root_Topic, sub_Topic);
  mqttClient.beginMessage(topic);
  mqttClient.print((Energy->ActiveEnergyValue)*EnergyConversionConstant);
  mqttClient.endMessage();
  snprintf(topic, sizeof(topic), "%s/Energy/%sImport/L3", root_Topic, sub_Topic);
  mqttClient.beginMessage(topic);
  mqttClient.print((Energy->ActiveEnergyValue)*EnergyConversionConstant);
  mqttClient.endMessage();
  PublishdateObserved("dateObserved");
  
}

void GridClass::PublishtotalReactiveEnergy(char* sub_Topic){

  char topic[100];
  snprintf(topic, sizeof(topic), "%s/Energy/%sImport/L1", root_Topic, sub_Topic);
  mqttClient.beginMessage(topic);
  mqttClient.print(topic);
  mqttClient.print((Energy->FundReactiveEnergyValue)*EnergyConversionConstant);
  mqttClient.endMessage();
  snprintf(topic, sizeof(topic), "%s/Energy/%sImport/L2", root_Topic, sub_Topic);
  mqttClient.beginMessage(topic);
  mqttClient.print((Energy->FundReactiveEnergyValue)*EnergyConversionConstant);
  mqttClient.endMessage();
  snprintf(topic, sizeof(topic), "%s/Energy/%sImport/L3", root_Topic, sub_Topic);
  mqttClient.beginMessage(topic);
  mqttClient.print((Energy->FundReactiveEnergyValue)*EnergyConversionConstant);
  mqttClient.endMessage();
  PublishdateObserved("dateObserved");
  
}