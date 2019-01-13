#ifndef _DOORSSWITCHESDRIVER_H_
#define _DOORSSWITCHESDRIVER_H_
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_MCP23017.h>
#include "Common.h"

class DoorsSwitchesDriver 
{
    public:
    DoorsSwitchesDriver(uint8_t addr1 = 0, uint8_t addr2 = 1, uint8_t intPin1 = 30, uint8_t intPin2 = 31) :
    mADDR1(addr1), mADDR2(addr2), mINTPIN1(intPin1), mINTPIN2(intPin2), mInterrStopped(true)
    {
        debugLogger.log(mDEBUGSTR1, __FUNCTION__);
        // Initialize pin on the microcontroller
        pinMode(mINTPIN1, INPUT);
        pinMode(mINTPIN2, INPUT);
        // Set ups interr on mcp chips
        mcp.setupInterrupts(true,false,LOW);
        mcp2.setupInterrupts(true,false,LOW);
        // Initializes pin on mcp chips
        // Set up all the pins on the first mcp
        for (int i = 0; i < 15; i++)
        {
            mcp.pinMode(i, INPUT);
            mcp.pullUp(i, HIGH);  // turn on a 100K pullup internally
            mcp.setupInterruptPin(i, RISING);     
        }
        // Set up pins used on the second mcp
        for (int i = 4; i < 8; i++)
        {
            mcp2.pinMode(i, INPUT);
            mcp2.pullUp(i, HIGH);  // turn on a 100K pullup internally
            mcp2.setupInterruptPin(i, RISING); 
        }

        for (int i = 8; i < 12; i++)
        {
            mcp2.pinMode(i, INPUT);
            mcp2.pullUp(i, HIGH);  // turn on a 100K pullup internally
            mcp2.setupInterruptPin(i, RISING); 
        }

        // Due to hardware issue I had one switch is not connected to first mcp
        // but added to the second on pin 12
        mcp2.pinMode(12, INPUT);
        mcp2.pullUp(12, HIGH);
        mcp2.setupInterruptPin(12, RISING);

        // attach interrupt functions
        attachInterrupt(digitalPinToInterrupt(mINTPIN1), DoorsSwitchesDriver::handleDoorSwInterrupt, FALLING);
        attachInterrupt(digitalPinToInterrupt(mINTPIN2), DoorsSwitchesDriver::handleDoorSwInterrupt2, FALLING);
        mInterrStopped = false;
    }
    
    void begin()
    {
        debugLogger.log(mDEBUGSTR1, __FUNCTION__);
        mcp.begin(mADDR1);
        mcp2.begin(mADDR2);
    }

    void stop_interrupts()
    {
        debugLogger.log(mDEBUGSTR1, __FUNCTION__);
        if (!mInterrStopped)
        {
            detachInterrupt(digitalPinToInterrupt(mINTPIN1));
            detachInterrupt(digitalPinToInterrupt(mINTPIN2));
            mInterrStopped = true;
        }
    }

    void start_interrupts()
    {
        debugLogger.log(mDEBUGSTR1, __FUNCTION__);
        if (mInterrStopped)
        {
            attachInterrupt(digitalPinToInterrupt(mINTPIN1), handleDoorSwInterrupt, FALLING);
            attachInterrupt(digitalPinToInterrupt(mINTPIN2), handleDoorSwInterrupt2, FALLING); 
            mInterrStopped = false;  
        }
    }

    void subscribe_to_sw_event(Subscriber* subscriber)
    {
        debugLogger.log(mDEBUGSTR1, __FUNCTION__);
        mSubscriber = subscriber;
        mItself = this;
    }

    void handle_interrupt(uint8_t mcpNumber);

    static void handleDoorSwInterrupt();
    static void handleDoorSwInterrupt2();
    
    private:

    Adafruit_MCP23017 mcp;
    Adafruit_MCP23017 mcp2;    
    
    Subscriber* mSubscriber;

    bool mInterrStopped;

    uint8_t mLastDoorNumberTriggered;

    const uint8_t mADDR1;
    const uint8_t mADDR2;
    const uint8_t mINTPIN1;
    const uint8_t mINTPIN2;    

    const char* mDEBUGSTR1 = "Door SW Driver: %s\n";
    const char* mDEBUGSTR2 = "Door SW Driver: %s, door #: %d\n"; 

    static const uint8_t mMCP1_PIN_TO_DOOR[15];
    static const uint8_t mMCP2_PIN_TO_DOOR[13];    
    
    static DoorsSwitchesDriver* mItself;
    
};


#endif
