#ifndef RESOURCECONTROLLER_H_
#define RESOURCECONTROLLER_H_
#include <Arduino.h>
#include "DoorsSwitchesDriver.h"
#include "CalendarOutputDriver.h"
#include "Common.h"
//#include <Adafruit_LittleFS.h>
//#include <InternalFileSystem.h>
#include <RTClib.h>


class ResourceController
{
    public:
    ResourceController(CalendarOutputDriver& calDriver, DoorsSwitchesDriver& doorsDriver, RTC_PCF8523& rtc) 
    : mCalOutDriver(calDriver), mDoorsSwDriver(doorsDriver), mRtc(rtc), mState(INACTIVE), mDoorTriggered(0)
    {
        debugLogger.log(mDEBUGSTR1, __FUNCTION__);
        // Subscribe to sw event
        //mDoorsSwDriver.subscribe_to_sw_event(this);
        mLocalMapOfOpened[0] = 255;  // Set first not used position to default value
        
    }

    void begin()
    {
        debugLogger.log(mDEBUGSTR1, __FUNCTION__);
        // Begin the drivers
        debugLogger.log("HERE3");
        //mCalOutDriver.begin();
        debugLogger.log("HERE4");
        mDoorsSwDriver.begin();
        debugLogger.log("HERE");
        mRtc.begin();
        debugLogger.log("HERE2");
        // Begin the RTC if we can't it is a critical error
        if (! mRtc.begin())
        {
            debugLogger.log("Couldn't find RTC");
            while (1);
        }
        // Initialize the RTC with compilation date for now, the date will be updated over serial
        if (!mRtc.initialized())
        {
            mRtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
        }
        debugLogger.log("%s: mDoorTriggered\n", mDoorTriggered);

    }

    void execute();

    private:
    bool should_calendar_start();
    //void parse_stored_doors_file();
    void copy_file_to_local();
    bool check_if_all_prev_opened(uint8_t day);
    void parse_serial_command();

    enum DayDoorsSate {NOT_OPENED = 0, OPENED = 1};
    enum OperStates {INACTIVE, ACTIVE};
    
    OperStates mState;

    CalendarOutputDriver& mCalOutDriver;
    DoorsSwitchesDriver& mDoorsSwDriver;
    RTC_PCF8523& mRtc;
    //File& mFile;

    DateTime mDateTime;

    static const uint8_t NUMOFDAYS = 25;
    uint8_t mDoorTriggered;
    uint8_t mLocalMapOfOpened[NUMOFDAYS] = {0};
    uint8_t mBuffer[NUMOFDAYS] = {0};
    
    const char* mFILENAME = "/ChristmasDays2.txt";
    const char* mDEBUGSTR1 = "Resource Controller: %s\n";
    const char* mDEBUGSTR2 = "Error Reading Time\n";
    
    static const uint8_t mSTARTOFFILE = 0;
    

};


#endif // DEBUG
