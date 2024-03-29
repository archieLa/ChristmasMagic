#ifndef _DOORSSWITCHESDRIVER_H_
#define _DOORSSWITCHESDRIVER_H_
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_MCP23X17.h>
#include "Common.h"


class DoorsSwitchesDriver 
{
    public:
    DoorsSwitchesDriver(uint8_t addr1 = 0x20, uint8_t addr2 = 0x21, uint8_t intPin1 = 30, uint8_t intPin2 = 31,
    bool useInterrupts = false) : mADDR1(addr1), mADDR2(addr2), mINTPIN1(intPin1), mINTPIN2(intPin2), mPreviousSwitchesState1(0)
    ,mCurrentSwitchesState1(0), mPreviousSwitchesState2(0), mCurrentSwitchesState2(0)
    ,mLastDoorNumberTriggered(0)
    {
        debugLogger.log(mDEBUGSTR1, __FUNCTION__);
        // Initialize pin on the microcontroller
        pinMode(mINTPIN1, INPUT);
        pinMode(mINTPIN2, INPUT);
        
        // Initializes pin on mcp chips
        // Set up all the pins except for last one  - connected to MCP2
        for (int i = 0; i < 15; i++)
        {
            mcp.pinMode(i, INPUT_PULLUP);
            //mcp.pullUp(i, HIGH);  // turn on a 100K pullup internally
        }
        // Set up pins used on the second mcp
        for (int i = 0; i < 15; i++)
        {
            mcp2.pinMode(i, INPUT_PULLUP);
            //mcp2.pullUp(i, HIGH);  // turn on a 100K pullup internally
        }

        //for (int i = 8; i < 13; i++)
        //{
        //    mcp2.pinMode(i, INPUT);
        //    mcp2.pullUp(i, HIGH);  // turn on a 100K pullup internally
        //    mcp2.setupInterruptPin(i, RISING); 
        //}

        // Due to hardware issue I had one switch is not connected to first mcp
        // but added to the second on pin 12
        //mcp2.pinMode(12, INPUT);
        //mcp2.pullUp(12, HIGH);
        //mcp2.setupInterruptPin(12, RISING);


    }
    
    void begin()
    {
        debugLogger.log(mDEBUGSTR1, __FUNCTION__);
        mcp.begin_I2C(mADDR1);
        mcp2.begin_I2C(mADDR2);
        //debugLogger.log("MCP gpios are: %d\n", mcp.readGPIOAB());
        //debugLogger.log("MCP2 gpios are %d\n", mcp2.readGPIOAB());
        // attach interrupt functions
    }

    // Returns 0 if no door was opened
    uint8_t find_which_door_opened();
    
    private:
    
    enum MCPS {ONE, TWO};

    uint8_t get_switch_state(uint16_t swNum, uint16_t combinedSwitches)
    {
        
        uint8_t switchState = ((combinedSwitches >> swNum) & 0x01);
        //debugLogger.log(mDEBUGSTR1, __FUNCTION__);
        //debugLogger.log("%s: switchState is %d\n", __FUNCTION__, switchState);
        //debugLogger.log("%s: Passed switch num %d\n", __FUNCTION__, swNum);
        //debugLogger.log("%s: Passed combined switches num %d\n", __FUNCTION__, combinedSwitches);
        return switchState;
    }

    bool did_door_sw_opened(uint8_t doorSwNumber, MCPS whichMcp)
    {
        debugLogger.log(mDEBUGSTR1, __FUNCTION__);
        bool ret = false;
        // If previous state is 0 and current is 1 there was a rising edge door opened
        if (whichMcp == ONE)
        {
            if (!get_switch_state(doorSwNumber, mPreviousSwitchesState1) &&
            get_switch_state(doorSwNumber, mCurrentSwitchesState1))
            {
                ret = true;
            }
        }
        else
        {
            if (!get_switch_state(doorSwNumber, mPreviousSwitchesState2) && get_switch_state(doorSwNumber, mCurrentSwitchesState2))
            {
                ret = true;
            }
        }

        return ret;      
    }
    
    uint16_t mPreviousSwitchesState1;
    uint16_t mCurrentSwitchesState1;
    uint16_t mPreviousSwitchesState2;
    uint16_t mCurrentSwitchesState2;
    
    
    Adafruit_MCP23X17 mcp;
    Adafruit_MCP23X17 mcp2;    
    
    Subscriber* mSubscriber;

    bool mInterrStopped;
    bool mUseInterrupts;

    uint8_t mLastDoorNumberTriggered;

    const uint8_t mADDR1;
    const uint8_t mADDR2;
    const uint8_t mINTPIN1;
    const uint8_t mINTPIN2;    

    const char* mDEBUGSTR1 = "Door SW Driver: %s\n";
    const char* mDEBUGSTR2 = "Door SW Driver: %s, door #: %d\n"; 

    static const uint8_t mMCP1_PIN_TO_DOOR[15];
    static const uint8_t mMCP2_PIN_TO_DOOR[13];    
    
    
};


#endif
