// Install the LowPower library for optional sleeping support.
// See loop() function comments for details on usage.
//#include <LowPower.h>

#include <Wire.h>
#include <Adafruit_MCP23017.h>
#include "RTClib.h"

Adafruit_MCP23017 mcp;
Adafruit_MCP23017 mcp2;
RTC_PCF8523 rtc;

// Interrupts from the MCP will be handled by this PIN
byte arduinoIntPin1 = 30;
byte arduinoIntPin2 = 31;

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

void setup(){

  Serial.begin(9600);
  Serial.println("MCP23007 Interrupt Test and RTC");
  
  //Wire.begin();
  pinMode(arduinoIntPin1, INPUT);
  pinMode(arduinoIntPin2, INPUT);
  Serial.println("WTF!!!!");
  
  mcp.begin();      // use default address 0
  mcp2.begin(1);     // second device with address of 1
  
  if (! rtc.begin()) 
  {
    Serial.println("Couldn't find RTC");
  }

  
  // We mirror INTA and INTB, so that only one line is required between MCP and Arduino for int reporting
  // The INTA/B will not be Floating 
  // INTs will be signaled with a LOW
  //mcp.setupInterrupts(true,false,LOW);
  //mcp2.setupInterrupts(true,false,LOW);

  Serial.println("Here");
  for (int i = 0; i < 15; i++)
  {
      mcp.pinMode(i, INPUT);
      mcp.pullUp(i, HIGH);  // turn on a 100K pullup internally
      //mcp.setupInterruptPin(i, RISING);     
  }

  for (int i = 4; i < 8; i++)
  {
      mcp2.pinMode(i, INPUT);
      mcp2.pullUp(i, HIGH);  // turn on a 100K pullup internally
      //mcp2.setupInterruptPin(i, RISING); 
  }

  mcp2.pinMode(12, INPUT);
  mcp2.pullUp(12, HIGH);
  //mcp2.setupInterruptPin(12, RISING);
  
  for (int i = 8; i < 12; i++)
  {
      mcp2.pinMode(i, INPUT);
      mcp2.pullUp(i, HIGH);  // turn on a 100K pullup internally
      //mcp2.setupInterruptPin(i, RISING); 
  }


  Serial.println("Right before the issue");
  if (! rtc.initialized()) 
  {
    Serial.println("RTC is NOT running!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  Serial.println("RTC initalized");
  Serial.println("Here");
  //attachInterrupt(digitalPinToInterrupt(arduinoIntPin1), handleInterrupt, FALLING);
  //attachInterrupt(digitalPinToInterrupt(arduinoIntPin2), handleInterrupt2, FALLING);
  
}


void handleInterrupt(){

  static int number_of_entry = 0;
  number_of_entry++;
  Serial.println(number_of_entry);
  
  // Get more information from the MCP from the INT
  uint8_t pin = mcp.getLastInterruptPin();
  uint8_t val = mcp.getLastInterruptPinValue();
  
  Serial.println("First multiplexer");
  Serial.print("Pin that triggered the interrupt was: ");
  Serial.println(pin);
  Serial.print("And the last interrupt pin value was: ");
  Serial.println(val);
  
  while (pin != MCP23017_INT_ERR) 
  {
    mcp.digitalRead(pin);
    pin = mcp.getLastInterruptPin();
  }
  
  }

  void handleInterrupt2(){

  static int number_of_entry = 0;
  number_of_entry++;
  Serial.println(number_of_entry);
  // Get more information from the MCP from the INT
  uint8_t pin = mcp2.getLastInterruptPin();
  uint8_t val = mcp2.getLastInterruptPinValue();
  
  Serial.println("Second multiplexer");
  Serial.print("Pin that triggered the interrupt was: ");
  Serial.println(pin);
  Serial.print("And the last interrupt pin value was: ");
  Serial.println(val);

  while (pin != MCP23017_INT_ERR)
  {
    mcp2.digitalRead(pin);
    pin = mcp2.getLastInterruptPin();
  }    


}


void loop(){
Serial.println("Here");
DateTime now = rtc.now();    
Serial.print(now.year(), DEC);
Serial.print('/');
Serial.print(now.month(), DEC);
Serial.print('/');
Serial.print(now.day(), DEC);
Serial.print(" (");
Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);
Serial.print(") ");
Serial.print(now.hour(), DEC);
Serial.print(':');
Serial.print(now.minute(), DEC);
Serial.print(':');
Serial.print(now.second(), DEC);
Serial.println();
Serial.println(mcp.readGPIOAB());
Serial.println(mcp2.readGPIOAB());
delay(50);
}
