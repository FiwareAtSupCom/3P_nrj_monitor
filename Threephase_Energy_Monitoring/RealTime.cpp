#include "RealTime.h"


const char* ntpServer1 = "pool.ntp.org";
const char* ntpServer2 = "time.nist.gov";
const long  gmtOffset_sec = 3600;
const int   daylightOffset_sec = 3600;

//ESP32Time rtc;
ESP32Time rtc(3600);
extern struct tm timeinfo;

void Time_setup()
{
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer1, ntpServer2);
}

void get_Time(struct tm* timeinfo)
{
  if(!getLocalTime(timeinfo)){
    return;
  }
  
}

