#include <WiFi.h>
#include "time.h"
#include "sntp.h"
#include <ESP32Time.h>

extern struct tm timeinfo;

void Time_setup() ;

void printLocalTime();

void get_Time(struct tm* time);
