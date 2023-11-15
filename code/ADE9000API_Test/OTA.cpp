#include "OTA.h"

 void OTA_setup(){

  // Hostname defaults to esp3232-[MAC]
   ArduinoOTA.setHostname("Grid/Solar");

  // No authentication by default
   ArduinoOTA.setPassword("admin");

   delay(1000);

   ArduinoOTA.begin();
}

