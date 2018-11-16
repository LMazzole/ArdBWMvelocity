#ifndef VELOCITYTRACKING_H_
#define VELOCITYTRACKING_H_

#define MYDEBUG

#include "Arduino.h"
#include "debug.h"

class Velocitytracking
{
  public:
    Velocitytracking();
    void ISRLB1();
    void ISRLB2();
    double calculate_velocity_ms();
    double calculate_velocity_ms(unsigned long passingduration);

  private:
    volatile unsigned long passingtimeLB1 = 0;
    volatile unsigned long passingtimeLB2 = 0;
    unsigned long oldpassingtimeLB1=0;
    unsigned long oldpassingtimeLB2=0;
    unsigned long passingdurationLB1 = 0;
    unsigned long passingdurationLB2 = 0;
    volatile bool LBinterrupted=false;
    const double sensordistance= 0.101; //m
    const double ballwidth= 0.072; //m -> 72mm
};



#endif /* VELOCITYTRACKING_H_ */
