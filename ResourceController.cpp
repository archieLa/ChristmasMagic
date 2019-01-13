#include "ResourceController.h"

void ResourceController::execute()
{   

    debugLogger.log(mDEBUGSTR1, __FUNCTION__);    
    
    // See if new date is programmed
    parse_serial_command();
    
    switch (mState)
    {
        case INACTIVE:
        {
            
            // Not interrupt action should be taken in inactive state
            mInterruptTriggered = false;
            mDoorsSwDriver.stop_interrupts();      
            mDateTime = mRtc.now();

            if (should_calendar_start())
            {
                mCalOutDriver.trigg_start_calendar();
                // Parse the stored files to the array to see if any of the door have been saved
                parse_stored_doors_file();
                // If file parsed and any doors were open turn them to red
                for (uint8_t i = 0; i < NUMOFDAYS; i++)
                {
                    if (mLocalMapOfOpened[i] && i != 0)
                    {
                        mCalOutDriver.trigg_correct_day(i);    
                    }
                }
                mState = ACTIVE;
            }
            mDoorsSwDriver.start_interrupts();       
        }
        case ACTIVE:
        {
            mDoorsSwDriver.stop_interrupts();
            if (mInterruptTriggered)
            {
                mDateTime = mRtc.now();
                
                uint8_t day = mDateTime.day();
                // Trying to open future day or not all previous door opened yet
                if (day < mDoorTriggered || !check_if_all_prev_opened(mDoorTriggered))
                {
                    mCalOutDriver.trigg_incorrect_day(mDoorTriggered);
                }
                else  // Correct day and previous doors opened 
                {
                    // Proceed only if we didn't trigger that day yet
                    if (!mLocalMapOfOpened[mDoorTriggered])
                    {
                        mCalOutDriver.trigg_correct_day(mDoorTriggered);
                        mLocalMapOfOpened[mDoorTriggered] = OPENED;
                        // Find if final day (dec 24th) was opened
                        if (mDoorTriggered == 24)
                        {
                            mCalOutDriver.trigg_end_calendar();
                            // Clear all the door to not opened
                            for (uint8_t i = 1; i <NUMOFDAYS; i++)
                            {
                                mLocalMapOfOpened[i] = NOT_OPENED;
                            } 
                            mState = INACTIVE;
                            mDoorTriggered = 0;                            
                        }
                        // Save copy of doors opened to the file if file opens
                        if (mFile.open(mFILENAME, FILE_WRITE))
                        {
                            mFile.seek(mSTARTOFFILE);
                            mFile.write(mLocalMapOfOpened, sizeof(mLocalMapOfOpened));
                        }
                    }
                }       
            }
            mInterruptTriggered = false;    // Reset the interrupt
            mDoorsSwDriver.start_interrupts();
        }
    }
}


void ResourceController::handle_callback(void* msg)
{
    debugLogger.log(mDEBUGSTR1, __FUNCTION__);
    mDoorTriggered = *(reinterpret_cast<uint8_t*>(msg));
    mInterruptTriggered = true;
}


bool ResourceController::should_calendar_start()
{
    debugLogger.log(mDEBUGSTR1, __FUNCTION__);
    uint8_t month = mDateTime.month();
    uint8_t day = mDateTime.day();
    return (month == 12 && day == 1) ? true : false;
}

void ResourceController::parse_stored_doors_file()
{
    debugLogger.log(mDEBUGSTR1, __FUNCTION__);
    // If flash file open we will copy the content to our array
    if (mFile.open(mFILENAME, FILE_READ))
    {
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
            DateTime temp(year, month, day, hour, minute, second);
            mRtc.adjust(temp);
        }
    }

}