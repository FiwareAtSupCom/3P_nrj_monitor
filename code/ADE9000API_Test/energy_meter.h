#include "ADE9000API.h"
#include <ArduinoJson.h>
#include "Fiware.h"
#include <EEPROM.h>
#include "RealTime.h"

void Store_In_ESP32(int* address ,float value);

float Read_From_ESP32(int* address);

String Read_String_From_ESP32(int* address);

void Store_Energy_In_Flash(float value);

void Send_Energy_From_Flash();

void Send_Stocked_Data();

void Change_Powertimer_config(unsigned int timerFactor);

void Change_Energytimer_config(unsigned int timerFactor);

void energy_meter_setup();