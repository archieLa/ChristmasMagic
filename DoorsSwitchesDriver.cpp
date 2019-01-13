#include "DoorsSwitchesDriver.h"


const uint8_t DoorsSwitchesDriver::mMCP1_PIN_TO_DOOR[15] = {17, 3, 23, 8, 10, 20, 1, 22, 21, 7, 5, 9, 
                                                            24, 13, 19};
const uint8_t DoorsSwitchesDriver::mMCP2_PIN_TO_DOOR[13] = {255, 255, 255, 255, 6, 12, 11, 18, 15, 4,
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
        mSubscriber->handle_callback(reinterpret_cast<void*>(mLastDoorNumberTriggered));
    }

}
