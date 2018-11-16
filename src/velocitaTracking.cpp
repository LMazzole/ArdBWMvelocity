#include "velocityTracking.h"

Velocitytracking::Velocitytracking(){
}

void Velocitytracking::ISRLB1(){
  DEBUG_PRINT("ISRLB1() - ");
 passingtimeLB1=micros(); //Auflösung auf 4 micros
  DEBUG_PRINTLN(passingtimeLB1);
}

void Velocitytracking::ISRLB2(){
  DEBUG_PRINT("ISRLB2() - ");
 passingtimeLB2=micros();
  DEBUG_PRINTLN(passingtimeLB2);
 LBinterrupted=true;
}

double Velocitytracking::calculate_velocity_ms(){
  DEBUG_PRINTLN("calculate_velocity_ms() ");
long passedTimeinUS = passingtimeLB2-passingtimeLB1;
double passedTimeinS=0.0;
double velocityms=0.0;
  DEBUG_PRINT("passingtimeLB1: ");
  DEBUG_PRINTLN(passingtimeLB1);
  DEBUG_PRINT("passingtimeLB2: ");
  DEBUG_PRINTLN(passingtimeLB2);
  DEBUG_PRINT("passedTimeinUS: ");
  DEBUG_PRINTLN(passedTimeinUS);

if (passedTimeinUS > 0 && (passingtimeLB1!=oldpassingtimeLB1) && (passingtimeLB2!=oldpassingtimeLB2)) {
    DEBUG_PRINT("Zeit [us]: ");
    DEBUG_PRINTLN(passedTimeinUS);
  passedTimeinS=passedTimeinUS/1000000.0;
    DEBUG_PRINT("Zeit [s]: ");
    DEBUG_PRINTLN(passedTimeinS);
  velocityms=sensordistance/passedTimeinS;
    DEBUG_PRINT("Geschwindigkeit [m/s]: ");
    DEBUG_PRINTLN(velocityms);
}
else{
    DEBUG_PRINT("Error, Negativ Time or same passingtime: ");
    DEBUG_PRINTLN(passedTimeinUS);
  velocityms=0;
}
oldpassingtimeLB1=passingtimeLB1;
oldpassingtimeLB2=passingtimeLB2;
return velocityms;
}

double Velocitytracking::calculate_velocity_ms(unsigned long passingduration){
  DEBUG_PRINTLN("calculate_velocity_ms(unsigned long passingduration) ");
double passedTimeinS=0.0;
double velocityms=0.0;
  DEBUG_PRINT("Zeit [us]: ");
  DEBUG_PRINTLN(passingduration);
passedTimeinS=passingduration/1000000.0;
  DEBUG_PRINT("Zeit [s]: ");
  DEBUG_PRINTLN(passedTimeinS);
velocityms=ballwidth/passedTimeinS;
  DEBUG_PRINT("Geschwindigkeit [m/s]: ");
  DEBUG_PRINTLN(velocityms);
  return velocityms;
}
