#ifndef DEBUG_H_
#define DEBUG_H_

#ifdef MYDEBUG
 #define DEBUG_PRINT(x)        Serial.print (x)
 #define DEBUG_PRINTDEC(x)     Serial.print (x, DEC)
 #define DEBUG_PRINTLN(x)      Serial.println (x)
#else
 #define DEBUG_PRINT(x)
 #define DEBUG_PRINTDEC(x)
 #define DEBUG_PRINTLN(x)
#endif

#endif /* DEBUG_H_ */
