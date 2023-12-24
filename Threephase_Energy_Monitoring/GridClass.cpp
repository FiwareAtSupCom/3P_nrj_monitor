#include "GridClass.h"


GridClass::GridClass()
{
	
}


void GridClass::PublishtotalActiveEnergy(char* sub_Topic){

  char topic[100];
  snprintf(topic, sizeof(topic), "%s/Energy/%sImport/L1", root_Topic, sub_Topic);
  mqttClient.beginMessage(topic);
  mqttClient.print(topic);
  mqttClient.print((ActiveEnergy->ActiveEnergy_A)*EnergyConversionConstant);
  mqttClient.endMessage();
  snprintf(topic, sizeof(topic), "%s/Energy/%sImport/L2", root_Topic, sub_Topic);
  mqttClient.beginMessage(topic);
  mqttClient.print((ActiveEnergy->ActiveEnergy_B)*EnergyConversionConstant);
  mqttClient.endMessage();
  snprintf(topic, sizeof(topic), "%s/Energy/%sImport/L3", root_Topic, sub_Topic);
  mqttClient.beginMessage(topic);
  mqttClient.print((ActiveEnergy->ActiveEnergy_C)*EnergyConversionConstant);
  mqttClient.endMessage();
  PublishdateObserved("dateObserved");
  
}

void GridClass::PublishtotalReactiveEnergy(char* sub_Topic){

  char topic[100];
  snprintf(topic, sizeof(topic), "%s/Energy/%sImport/L1", root_Topic, sub_Topic);
  mqttClient.beginMessage(topic);
  mqttClient.print(topic);
  mqttClient.print((ReactiveEnergy->ReactiveEnergy_A)*EnergyConversionConstant);
  mqttClient.endMessage();
  snprintf(topic, sizeof(topic), "%s/Energy/%sImport/L2", root_Topic, sub_Topic);
  mqttClient.beginMessage(topic);
  mqttClient.print((ReactiveEnergy->ReactiveEnergy_B)*EnergyConversionConstant);
  mqttClient.endMessage();
  snprintf(topic, sizeof(topic), "%s/Energy/%sImport/L3", root_Topic, sub_Topic);
  mqttClient.beginMessage(topic);
  mqttClient.print((ReactiveEnergy->ReactiveEnergy_C)*EnergyConversionConstant);
  mqttClient.endMessage();
  PublishdateObserved("dateObserved");
  
}