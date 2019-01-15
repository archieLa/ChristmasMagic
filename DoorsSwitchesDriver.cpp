#include "DoorsSwitchesDriver.h"


const uint8_t DoorsSwitchesDriver::mMCP1_PIN_TO_DOOR[15] = {3, 17, 23, 8, 20, 10, 1, 22, 21, 7, 5, 9, 
                                                            24, 13, 19};
const uint8_t DoorsSwitchesDriver::mMCP2_PIN_TO_DOOR[13] = {255, 255, 255, 255, 12, 6, 11, 18, 15, 4,
                                                            16, 2, 14};

DoorsSwitchesDriver* DoorsSwitchesDriver::mItself = NULL;

void DoorsSwitchesDriver::handleDoorSwInterrupt()
{
    debugLogger.log("MCP1_Interrupt\n");
    if (mItself)
    {
    mItself->handle_interrupt(1);
    }
    else
    {
        debugLogger.log("Callback not set up\n");
    }
}

void DoorsSwitchesDriver::handleDoorSwInterrupt2()
{
    debugLogger.log("MCP2_Interrupt\n");
    if (mItself)
    {
        mItself->handle_interrupt(2);
    }
    else
    {
        debugLogger.log("Callback not set up\n");
    }
}

void DoorsSwitchesDriver::handle_interrupt(uint8_t mcpNumber)
{
    debugLogger.log(mDEBUGSTR1, __FUNCTION__);
    
    uint8_t pin;

    if (mcpNumber == 1)
    {
        pin = mcp.getLastInterruptPin();
        mLastDoorNumberTriggered = mMCP1_PIN_TO_DOOR[pin];
        while (pin != MCP23017_INT_ERR) 
        {
            mcp.digitalRead(pin);
            pin = mcp.getLastInterruptPin();
        }
    }
    else
    {
        pin = mcp2.getLastInterruptPin();
        mLastDoorNumberTriggered = mMCP2_PIN_TO_DOOR[pin];
        while (pin != MCP23017_INT_ERR) 
        {
            mcp2.digitalRead(pin);
            pin = mcp2.getLastInterruptPin();
        }
    }

    debugLogger.log(mDEBUGSTR2, __FUNCTION__, mLastDoorNumberTriggered);
    
    if (mSubscriber == NULL)
    {
        debugLogger.log("No callback object");           
    }
    else
    {
        mSubscriber->handle_callback(mLastDoorNumberTriggered);
    }

}


uint8_t DoorsSwitchesDriver::find_which_door_opened()
{
    
    debugLogger.log(mDEBUGSTR1, __FUNCTION__);
    delay(10);
    mCurrentSwitchesState1 = mcp.readGPIOAB();
    delay(10);
    mCurrentSwitchesState2 = mcp2.readGPIOAB();
    
    debugLogger.log("Gpios read from MCP1 %d", mCurrentSwitchesState1);
    debugLogger.log("Gpios read from MCP2 %d", mCurrentSwitchesState2);
    debugLogger.log("Prev GPIOS MCP1 %d", mPreviousSwitchesState1);
    debugLogger.log("Prev GPIOS MCP2 %d", mPreviousSwitchesState2);
    uint8_t doorNumber = 0;
    uint8_t skip_second_check = false;

    // Check MCP1 
    for (uint8_t i = 0; i < 15; i++)
    {
        // Right now only one switche can indicate so we return as soon as we find one
        if (did_door_sw_opened(i, ONE))
        {
            debugLogger.log("%s: MCP1 Checks\n", __FUNCTION__, i);
            debugLogger.log("%s: door switch number %d was opened\n", __FUNCTION__, i);
            doorNumber = mMCP1_PIN_TO_DOOR[i];
            skip_second_check = true;
            break;
        }
    }

    // Check MCP2
    for (uint8_t i = 4; i < 13; i++)
    {
        // Right now only one switch can indicate so we return as soon as we find one
        if (did_door_sw_opened(i, TWO))
        {
            debugLogger.log("%s: MCP2Checks\n", __FUNCTION__, i);
            debugLogger.log("%s: door switch number %d was opened\n", __FUNCTION__, i);
            doorNumber = mMCP2_PIN_TO_DOOR[i];
            break;
        }
    }    
    mPreviousSwitchesState1 = mCurrentSwitchesState1;
    mPreviousSwitchesState2 = mCurrentSwitchesState2;
    return doorNumber;
}

