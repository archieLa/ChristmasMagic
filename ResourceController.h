#ifndef RESOURCECONTROLLER_H_
#define RESOURCECONTROLLER_H_
#include <Arduino.h>
#include "DoorsSwitchesDriver.h"
#include "CalendarOutputDriver.h"
#include "Common.h"
#include <Bluefruit_FileIO.h>

class ResourceController : public Subscriber
{
    public:
    ResourceController(CalendarOutputDriver& calDriver, DoorsSwitchesDriver& doorsDriver) 
    : mCalOutDriver(calDriver), mDoorsSwDriver(doorsDriver), mState(NON_ACTIVE)
    {
        debugLogger.log(mDEBUGSTR1, __FUNCTION__);
        // Subscribe to sw event
        mDoorsSwDriver.subscribe_to_sw_event(this);
    }

    void begin()
    {
        debugLogger.log(mDEBUGSTR1, __FUNCTION__);
        // Begin the drivers
        mCalOutDriver.begin();
        mDoorsSwDriver.begin();
    }

    void execute();

    void handle_callback(Common::CommandContext context, void* msg);


    private:
    
    enum OperStates {NON_ACTIVE, ACTIVE};
    OperStates mState;
    const char* mDEBUGSTR1 = "Resource Controller: %s\n";
    CalendarOutputDriver& mCalOutDriver;
    DoorsSwitchesDriver& mDoorsSwDriver;



};


#endif // DEBUG