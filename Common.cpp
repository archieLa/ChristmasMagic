#include "Common.h"

SerialOutputDevice debugToSerial;
SimpleDebug<50> debugLogger(debugToSerial);