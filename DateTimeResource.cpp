#include "DateTimeResource.h"


/*  Date Time Resource Interface */

void DateTimeResIf::handle_callback(Common::CommandContext context, void* msg)
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
            // This should never happen but
            // safe programming is the name of the game
            if (date != nullptr)
            {
                reset_with_date(*date);
            }
            break;
        }
        case Common::DATE_TIME_INCREMENT_DATE:
        {
            increment_date();
            break;
        }
        case Common::DATE_TIME_SET_DATE:
        {
            if (date != nullptr)
            {
            set_current_date(*date);
            }
            break;
        }
        case Common::DATE_TIME_SET_UNAVAIL:
        {
            set_to_unavail();
            break;
        }
    }
}

void DateTimeResIf::set_to_unavail()
{
    // Done so that classes inheritting from this abstract class
    // But not having a need or option to 
}

/* End */

/* Date time Resouce Simulation */

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

/* End */

/* Date Time Resouce Real Time Clock */

DateTimeResRtc::DateTimeResRtc(const DateTime& date)
{
    mDateTime = date;
    mRtc.begin();
    // Adjust in constructor only if not previously done
    if (!mRtc.initialized())
    {    
        mRtc.adjust(mDateTime);
    }
}

boolean DateTimeResRtc::get_current_date(DateTime& date)
{
    boolean returnStatus = false;
    
    if (mRtc.initialized())
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

void DateTimeResRtc::set_current_date(const DateTime& date)
{
    mDateTime = date;
    mRtc.adjust(mDateTime);
}

void DateTimeResRtc::increment_date()
{
    mDateTime = mDateTime + TimeSpan(1,0,0,0);
    mRtc.adjust(mDateTime);
}

void DateTimeResRtc::reset_without_date()
{
    mRtc.adjust(mDateTime);
}

void DateTimeResRtc::reset_with_date(const DateTime& date)
{
    mDateTime = date;
    mRtc.adjust(mDateTime);
}

/* End */
