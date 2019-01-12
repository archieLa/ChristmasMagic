#include <RTClib.h>
//#include "Common.h"
//#include "DateTimeResource.h"
//#include "SimpleDebug.h"
#include "CalendarOutputDriver.h"
#include "Common.h"


CalendarOutputDriver outCalDriver;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  #ifdef DEBUG
  debugLogger.enable();  
  #else if
  debugLogger.disable();
  #endif

  outCalDriver.begin();

}

void loop() {
  // put your main code here, to run repeatedly:
  
  
  if (Serial.available())
  {
    char incomingCommand;
    incomingCommand = Serial.read();
    
    switch (incomingCommand)
    {
      case 'a':
      {
        int switchNum = Serial.parseInt();
        outCalDriver.trigg_correct_day(switchNum);
        break;
      }
      case 'b':
      {
        int switchNum = Serial.parseInt();
        outCalDriver.trigg_incorrect_day(switchNum);
        break; 
      }
      case 'c':
      {
        outCalDriver.trigg_start_calendar();
        break;
      }
      case 'd':
      {
        outCalDriver.trigg_end_calendar();
        break;       
      }
      default:
      {
        Serial.println("oppps");
        break;
      }

    }

    delay(1000);

  }
}
