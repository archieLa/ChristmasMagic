#include "DateTimeResource.h"

boolean DateTimeResSim::get_current_date(DateTime& date)
{
    date = mDateTime;
    return (mAvail == true) ? true : false;
}

boolean DateTimeResSim::check_if_aval() const
{
    return mAvail;
}

void DateTimeResSim::set_current_date(const DateTime& date)
{
    mDateTime = date;
}

void DateTimeResSim::increment_date()
{
    mDateTime = mDateTime + TimeSpan(1,0,0,0);
}

void DateTimeResSim::reset_without_date()
{
    mAvail = true;
}

void DateTimeResSim::reset_with_date(const DateTime& date)
{
    mAvail = true;
    mDateTime = date;
} 

void DateTimeResSim::set_to_unavail()
{
  mAvail = false;
}

void DateTimeResSim::handle_command(Common::CommandContext context, void* msg)
{
    DateTime* date = nullptr;
    if (msg != nullptr)
    {
        date = reinterpret_cast<DateTime*>(msg);
    }
    
    switch (context)
    {
        case Common::DATE_TIME_RESET:
        {
            reset_without_date();
            break;
        }
        case Common::DATE_TIME_RESET_WITH_DATE:
        {
            reset_with_date(*date);
            break;
        }
        case Common::DATE_TIME_INCREMENT_DATE:
        {
            increment_date();
            break;
        }
        case Common::DATE_TIME_SET_DATE:
        {
            set_current_date(*date);
            break;
        }
        case Common::DATE_TIME_SET_UNAVAIL:
        {
            set_to_unavail();
            break;
        }
    }
}


DateTimeResRtc::DateTimeResRtc(const DateTime& date)
{
    mDateTime = date;
    mRtc.begin();
    mRtc.adjust(mDateTime);
}

boolean DateTimeResRtc::get_current_date(DateTime& date)
{
    boolean returnStatus = false;
    
    if (initialized())
    {
        // Determine if function error out
        date = mRtc.now();
        // Function returns inverse logic so need
        // to flip it
        returnStatus = !mRtc.checkIfErrorPresent();
    }

    return returnStatus;
}

boolean DateTimeResRtc::check_if_aval()
{
    return ((mRtc.initialized()) && (mRtc.checkBatteryStatus() == RTC_PCF8523::BATT_OK) \
           && (!mRtc.checkIfErrorPresent())); 
}