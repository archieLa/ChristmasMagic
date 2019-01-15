#ifndef _SIMPLEDEBUG_H_
#define _SIMPLEDEBUG_H_
#include <Arduino.h>
#include <stdarg.h>

// Super simple implementation of interface to be able to log debug statements/actions
// For example you could use serial output LED or other.
// Simply write a class that inherits from DebugOutputDevice and implement write function
// How you implement write is up to you. When done defining that class simply pass referece to SimpleDebug

class DebugOutputDevice 
{
    public:
      virtual void write(const char *stringToWrite) = 0;
      virtual ~DebugOutputDevice() {}
};

// I don't like to assume but in this case I am assuming that Serial begin has been called
class SerialOutputDevice : public DebugOutputDevice 
{
     public: 
      void write(const char *stringToWrite)
      {
        Serial.write(stringToWrite);
      }
};


template <int MAXDEBUGSTRINGSIZE>
class SimpleDebug 
{
    public:
        
        SimpleDebug(DebugOutputDevice& device) : mDevice(device), mEnabled(true)
        {}            
        
        void enable()
        {
            mEnabled = true;
        }

        void disable()
        {
            mEnabled = false;
        }

        void log(const char * formatString, ...)
        {   
            if (mEnabled)
            {
                va_list args;
                va_start(args, formatString);             
                if (vsnprintf(mBuffer, MAXDEBUGSTR, formatString, args) < 0)
                {
                    mDevice.write(ERRORSTR);
                }
                else
                {
                    mDevice.write(mBuffer);
                }
                va_end(args);
            }
        }

    private:
        // Size of the output string is limited to 50 characters
        char mBuffer[MAXDEBUGSTRINGSIZE];
        DebugOutputDevice& mDevice;
        bool mEnabled;
        const int8_t MAXDEBUGSTR = MAXDEBUGSTRINGSIZE;
        const char* ERRORSTR = "SimpleDebugErr: Failed encoding";


};

#endif // !_SIMPLESERIALDEBUG_H_
