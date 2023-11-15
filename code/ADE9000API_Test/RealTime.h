#include "Arduino.h"
#include <time.h>
#include <string>

   struct TimeDate
 {
  String DateTime;
 }; 

void Time_setup() ;

void get_Time(TimeDate* timeinfo);