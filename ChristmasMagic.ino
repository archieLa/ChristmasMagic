#include "Common.h"
#include <RTClib.h>
#include "SimpleDebug.h"
#include "CalendarOutputDriver.h"
#include "DoorsSwitchesDriver.h"
#include "ResourceController.h"

DoorsSwitchesDriver inDoorSwDriver;
CalendarOutputDriver outCalDriver;
RTC_PCF8523 rtc;
//File storageFile(InternalFS);
ResourceController resController(outCalDriver, inDoorSwDriver, rtc);


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  #ifdef DEBUG
  debugLogger.enable();  
  #else if
  debugLogger.disable();
  #endif

  //InternalFS.begin();
  resController.begin();

}

void loop() {
  resController.execute();
  debugLogger.log("Out of resource controller\n");
  delay(250);
  
}
