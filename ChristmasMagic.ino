#include <RTClib.h>
#include "Common.h"
#include "DateTimeResource.h"



void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:
      DateTimeResSim dateTime(DateTime(0));
    DateTimeResIf* something = &dateTime;

}
