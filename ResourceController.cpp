#include "ResourceController.h"

void ResourceController::execute()
{   

    debugLogger.log(mDEBUGSTR1, __FUNCTION__);    
    
    // See if new date is programmed
    parse_serial_command();
    debugLogger.log("Current state is: %d\n", mState);
    mDateTime = mRtc.now();

    switch (mState)
    {
        case INACTIVE:
        {
            
            debugLogger.log("%s: State is inactive\n", __FUNCTION__);    
            debugLogger.log("Current year is: %d\n", mDateTime.year());
            debugLogger.log("Current month is: %d\n", mDateTime.month());
            debugLogger.log("Current day is: %d\n", mDateTime.day());

            if (should_calendar_start())
            {
                debugLogger.log("%s: Calendar should start\n", __FUNCTION__);
                mCalOutDriver.trigg_start_calendar();
                // Parse the stored files to the array to see if any of the door have been saved
                //parse_stored_doors_file();
                // If file parsed and any doors were open turn them to red
                for (uint8_t i = 0; i < NUMOFDAYS; i++)
                {
                    if (mLocalMapOfOpened[i] && i != 0)
                    {
                        mCalOutDriver.trigg_correct_day(i);    
                    }
                }
                debugLogger.log("%s: This should not happen currently\n");
                mState = ACTIVE;
            } 
            break;      
        }
        case ACTIVE:
        {
            debugLogger.log("%s: State is active\n", __FUNCTION__);
            uint8_t day = mDateTime.day();
            debugLogger.log("Current day is %d\n", day);
            // Find if any door opened
            mDoorTriggered = mDoorsSwDriver.find_which_door_opened();
            debugLogger.log("%s: current door trigger %d\n", mDoorTriggered);
            if (mDoorTriggered)
            {
                // Trying to open future day or not all previous door opened yet
                if (day < mDoorTriggered || !check_if_all_prev_opened(mDoorTriggered))
                {
                    debugLogger.log("%s: Incorrect door should be triggered\n", __FUNCTION__);
                    mCalOutDriver.trigg_incorrect_day(mDoorTriggered);
                }
                else  // Correct day and previous doors opened 
                {
                    // Proceed only if we didn't trigger that day yet
                    if (!mLocalMapOfOpened[mDoorTriggered])
                    {
                        debugLogger.log("%s: Day not processed yet\n", __FUNCTION__);
                        mCalOutDriver.trigg_correct_day(mDoorTriggered);
                        mLocalMapOfOpened[mDoorTriggered] = OPENED;
                        // Find if final day (dec 24th) was opened
                        if (mDoorTriggered == 24)
                        {
                            debugLogger.log("%s: December 24th!\n", __FUNCTION__);
                            mCalOutDriver.trigg_end_calendar();
                            // Clear all the door to not opened
                            for (uint8_t i = 1; i <NUMOFDAYS; i++)
                            {
                                mLocalMapOfOpened[i] = NOT_OPENED;
                            } 
                            debugLogger.log("%s: Determined to be true\n", __FUNCTION__);
                            mState = INACTIVE;
                            mDoorTriggered = 0;                            
                        }
                        // Save copy of doors opened to the file if file opens
                        //if (mFile.open(mFILENAME, FILE_WRITE))
                        //{
                        //    debugLogger.log("%s: File opened, saving map of doors\n", __FUNCTION__);
                        //    mFile.seek(mSTARTOFFILE);
                        //    mFile.write(mLocalMapOfOpened, sizeof(mLocalMapOfOpened));
                        //}
                    }
                }       
            }
        break;
        }
    }
}


void ResourceController::handle_callback(uint8_t msg)
{
    debugLogger.log(mDEBUGSTR1, __FUNCTION__);
    mDoorTriggered = msg;
    debugLogger.log("RC %s, door %d\n", __FUNCTION__, mDoorTriggered);
    mInterruptTriggered = true;
}


bool ResourceController::should_calendar_start()
{
    debugLogger.log(mDEBUGSTR1, __FUNCTION__);
    uint8_t month = mDateTime.month();
    uint8_t day = mDateTime.day();
    if (month == 12 && day == 1)
    {
        debugLogger.log("%s: Determined to be true\n", __FUNCTION__);
        return true;
    }
    else
    {
        debugLogger.log("%s: Determined to be false\n", __FUNCTION__);
        return false;
    }
    
}

void ResourceController::parse_stored_doors_file()
{
    debugLogger.log(mDEBUGSTR1, __FUNCTION__);
    // If flash file open we will copy the content to our array
    if (mFile.open(mFILENAME, FILE_READ))
    {
        debugLogger.log("%s: File opened for reading\n", __FUNCTION__);
        // Copy the contents
        int bytesRead = mFile.read(mBuffer, sizeof(mBuffer));
        // Only copy if we got all bytes
        if (bytesRead == NUMOFDAYS)
        {
            copy_file_to_local();
        }
    }
}

void ResourceController::copy_file_to_local()
{
    debugLogger.log(mDEBUGSTR1, __FUNCTION__);
    for (uint8_t i = 0; i < NUMOFDAYS; i++)
    {
        mLocalMapOfOpened[i] = mBuffer[i];
    }
}


bool ResourceController::check_if_all_prev_opened(uint8_t day)
{
    debugLogger.log(mDEBUGSTR1, __FUNCTION__);
    bool status = false;
    
    
    if (day == 1)
    {
        status = true;
    }    
    else
    {
        for (uint8_t i = day - 1; i >0; i--)
        {
            if (mLocalMapOfOpened[i])
            {
                status = true;
            }
            else
            {
                break;
            }
        }
    }

    return status;
}

void ResourceController::parse_serial_command()
{
    debugLogger.log(mDEBUGSTR1, __FUNCTION__);

    while (Serial.available())
    {
        char c = Serial.read();
        if (c == 'd')
        {
            uint16_t year = Serial.parseInt();
            uint8_t month = Serial.parseInt();
            uint8_t day = Serial.parseInt();
            uint8_t hour = Serial.parseInt();
            uint8_t minute = Serial.parseInt();
            uint8_t second = Serial.parseInt();
            debugLogger.log("Received following time:\n");
            debugLogger.log("%d/", year);
            debugLogger.log("%d/", month);
            debugLogger.log("%d/ ", day);
            debugLogger.log("%d:", hour);
            debugLogger.log("%d:", minute);
            debugLogger.log("%d\n", second);
            DateTime temp(year, month, day, hour, minute, second);
            mRtc.adjust(temp);

        }
    }

}