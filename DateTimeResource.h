#ifndef _DATETIMERESOURCE_H_
#define _DATETIMERESOURCE_H_

#include "RTCLib.h"
#include "Common.h"
#include <Arduino.h>

class DateTimeResIf
{
    public:
        virtual boolean get_current_date(DateTime& date) = 0;
        virtual boolean check_if_aval() const = 0;
        virtual void handle_command(Common::CommandContext context, void* msg);
        virtual ~DateTimeResIf() {}
    protected:
        virtual void set_current_date(const DateTime& date) = 0;
        virtual void increment_date() = 0;
        virtual void reset_without_date() = 0;
        virtual void reset_with_date(const DateTime& date) = 0;
    private:
      virtual void set_to_unavail();
};

class DateTimeResSim : public DateTimeResIf 
{
    public:
        DateTimeResSim(const DateTime& date) : mDateTime(date), mAvail(true) {}
        boolean get_current_date(DateTime& date);
        boolean check_if_aval() const;
    private:
        void set_current_date(const DateTime& date);
        void increment_date();
        void reset_without_date();
        void reset_with_date(const DateTime& date);
        void set_to_unavail();
        bool mAvail;    
        DateTime mDateTime;   
};

class DateTimeResRtc : public DateTimeResIf
{
    public:
        DateTimeResRtc(const DateTime& date);
        boolean get_current_date(DateTime& date);
        boolean check_if_aval() const;
    private:
        void set_current_date(const DateTime& date);
        void increment_date();
        void reset_without_date();
        void reset_with_date(const DateTime& date);
        DateTime mDateTime;
        RTC_PCF8523 mRtc;
};


#endif // !_DATETIMERESOURCE_H_
