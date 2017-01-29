#ifndef DEBUG_H
#define DEBUG_H

#include <Arduino.h>
#include "Config.h"

#ifdef DEBUG

#define DEBUG(x) Serial.print(millis());Serial.print(": ");Serial.println(x); delay(10);
#define DEBUG2(x, y) Serial.print(x); Serial.println(y); delay(10);
#define DEBUG3(x, y, z) Serial.print(x); Serial.print(y); Serial.println(z); delay(10);

#else

#define DEBUG(x) ;
#define DEBUG2(x, y) ;
#define DEBUG3(x, y, z) ;

#endif

#endif
