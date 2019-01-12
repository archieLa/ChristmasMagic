#ifndef _COMMON_H_
#define _COMMON_H_
#include "SimpleDebug.h"
namespace Common
{
    enum CommandContext {DATE_TIME_RESET, DATE_TIME_RESET_WITH_DATE, DATE_TIME_INCREMENT_DATE, DATE_TIME_SET_DATE, DATE_TIME_SET_UNAVAIL, DOOR_OPENED};
    enum ChristmasDoors {DOOR_1 = 1, DOOR_2, D00R_3, DOOR_4, DOOR_5, DOOR_6, DOOR_7, DOOR_8,
    DOOR_9, DOOR_10, DOOR_11, DOOR_12, DOOR_13, DOOR_14, DOOR_15, DOOR_16, DOOR_17,
    DOOR_18, DOOR_19, DOOR_20, DOOR_21, DOOR_22, DOOR_23, DOOR_24};

}

// Used for callbacks
class Subscriber
{
    public:
    Subscriber();
    virtual ~Subscriber();
    virtual void handle_callback(Common::CommandContext context, void* msg) = 0;
};

extern SimpleDebug<50> debugLogger;
#define DEBUG

#endif // !_COMMON_H_

