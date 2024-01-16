#include "GridClass.h"


GridClass::GridClass()
{
	
}


void GridClass::PublishtotalActiveEnergy(){

  char topic[100];
  snprintf(topic, sizeof(topic), "e6|{L1: %f, L2: %f, L3: %f}",(ActiveEnergy->ActiveEnergy_A)*EnergyConversionConstant,(ActiveEnergy->ActiveEnergy_B)*EnergyConversionConstant,(ActiveEnergy->ActiveEnergy_C)*EnergyConversionConstant);
  if (WiFi.status() == WL_CONNECTED){
  mqttClient.beginMessage(root_Topic);
  mqttClient.print(topic);
  mqttClient.endMessage();
  PublishdateObserved("e23");
  }else{
      myQueue.push(topic);
    }
  
}

void GridClass::PublishtotalReactiveEnergy(){

  char topic[100];
  snprintf(topic, sizeof(topic), "e7|{L1: %f, L2: %f, L3: %f}",(ReactiveEnergy->ReactiveEnergy_A)*EnergyConversionConstant,(ReactiveEnergy->ReactiveEnergy_B)*EnergyConversionConstant,(ReactiveEnergy->ReactiveEnergy_C)*EnergyConversionConstant);
  if (WiFi.status() == WL_CONNECTED){
  mqttClient.beginMessage(root_Topic);
  mqttClient.print(topic);
  mqttClient.endMessage();
  PublishdateObserved("e23");
  }else{
      myQueue.push(topic);
    }
  
}