#include "RealTime.h"


const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 3600;
const int   daylightOffset_sec = 3600;

void Time_setup(){
  
  // Init and get the time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer,NULL);
}


void get_Time(TimeDate* timeinfo){
  struct tm timei;
  if(!getLocalTime(&timei)){
    return;
  }
  timeinfo->dayStamp="%d-%B-%Y";
  timeinfo->timeStamp= "%H:%M:%S";

}