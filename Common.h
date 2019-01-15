#ifndef _COMMON_H_
#define _COMMON_H_
#include "SimpleDebug.h"


// Used for callbacks
class Subscriber
{
    public:
    Subscriber() {}
    virtual ~Subscriber() {}
    virtual void handle_callback(uint8_t msg) = 0;
};

extern SimpleDebug<50> debugLogger;
//#define DEBUG

#endif // !_COMMON_H_

