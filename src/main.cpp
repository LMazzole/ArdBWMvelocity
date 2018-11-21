#define MYDEBUG
#include "debug.h"
#include <Arduino.h>

//===========Global Variabel===========
volatile unsigned long passingtimeLB1 = 0;
volatile unsigned long passingtimeLB11 = 0;
volatile unsigned long passingtimeLB12 = 0;
volatile unsigned long passingtimeLB2 = 0;
volatile unsigned long passingtimeLB21 = 0;
volatile unsigned long passingtimeLB22 = 0;
unsigned long oldpassingtimeLB1=0;
unsigned long oldpassingtimeLB2=0;
unsigned long passingdurationLB1 = 0;
unsigned long passingdurationLB2 = 0;
volatile bool LBinterrupted=false;

const double sensordistance= 10.1*1000; //um
const double ballwidth= 72.0*1000; //um -> 72mm

//===========Pindefine===========
// #define lightbarrier1 (2) // Interrupt on Pin 2 or 3 (UNO)
// #define lightbarrier2 (3)
const byte lightbarrier1 = 2;
const byte lightbarrier2 = 3;

//===========Functionprototype===========
void ISRLB1();
void ISRLB2();
double calculate_velocity_ms();
double calculate_velocity_ms(unsigned long passingduration,double distance);
int countingvar = 0;

void setup() {
  Serial.begin(9600);                // Set serial monitor baud rate
 // Interrupt Pins for  Uno, Nano, Mini: 2, 3
  attachInterrupt(digitalPinToInterrupt(lightbarrier1), ISRLB1, CHANGE);
  attachInterrupt(digitalPinToInterrupt(lightbarrier2), ISRLB2, CHANGE);
    DEBUG_PRINTLN("Arduino Initialise");
}

void loop() {
  // delay(500); //Update only once per Second
  passingdurationLB1 = pulseIn(lightbarrier1, LOW,200);
  passingdurationLB2 = pulseIn(lightbarrier2, LOW,200);
  DEBUG_PRINTLN(calculate_velocity_ms(passingdurationLB1,ballwidth)*3.6);
  DEBUG_PRINTLN(calculate_velocity_ms(passingdurationLB2,ballwidth)*3.6);
  // velocityTracking.calculate_velocity_ms(passingdurationLB1)*3.6;
  DEBUG_PRINTLN(calculate_velocity_ms(passingtimeLB12-passingtimeLB11,ballwidth)*3.6);
  DEBUG_PRINTLN(calculate_velocity_ms(passingtimeLB22-passingtimeLB21,ballwidth)*3.6);
  DEBUG_PRINTLN(calculate_velocity_ms(passingtimeLB21-passingtimeLB11,sensordistance)*3.6);
  DEBUG_PRINTLN(calculate_velocity_ms(passingtimeLB22-passingtimeLB12,sensordistance)*3.6);

  if (LBinterrupted){
      LBinterrupted=false;

      double velocitykmh=calculate_velocity_ms()*3.6;
      if(velocitykmh!=0){
      countingvar+=1;
      Serial.print(countingvar);
      Serial.print("\t");
      Serial.print("Geschwindigkeit [km/h]:");
      Serial.print("\t");
      Serial.println(velocitykmh);
    }
 }
}

void ISRLB1() {
  DEBUG_PRINT("ISRLB1() - ");
  passingtimeLB1=micros();
  if(digitalRead(lightbarrier1)){ //Direct Port Acces would be faster
    passingtimeLB12=passingtimeLB1;
    DEBUG_PRINTLN(passingtimeLB12);
  }
  else{
    passingtimeLB11=passingtimeLB1; //Auflösung auf 4 micros
    DEBUG_PRINTLN(passingtimeLB11);
  }
}

void ISRLB2() {
  DEBUG_PRINT("ISRLB2() - ");
  passingtimeLB2=micros();
  if(digitalRead(lightbarrier2)){
    passingtimeLB22=passingtimeLB2;
    DEBUG_PRINTLN(passingtimeLB22);
  }
  else{
    passingtimeLB21=passingtimeLB2; //Auflösung auf 4 micros
    DEBUG_PRINTLN(passingtimeLB21);
  }
 LBinterrupted=true;
}

double calculate_velocity_ms(){
    DEBUG_PRINTLN("calculate_velocity_ms() ");
  long passedTimeinUS = passingtimeLB21-passingtimeLB11;
  double velocityms=0.0;
    DEBUG_PRINT("passingtimeLB11: ");
    DEBUG_PRINTLN(passingtimeLB11);
    DEBUG_PRINT("passingtimeLB21: ");
    DEBUG_PRINTLN(passingtimeLB21);
    DEBUG_PRINT("passedTimeinUS: ");
    DEBUG_PRINTLN(passedTimeinUS);

  if (passedTimeinUS > 0 && (passingtimeLB11!=oldpassingtimeLB1) && (passingtimeLB21!=oldpassingtimeLB2)) {
      DEBUG_PRINT("Zeit [us]: ");
      DEBUG_PRINTLN(passedTimeinUS);
      DEBUG_PRINT("Zeit [s]: ");
      DEBUG_PRINTLN(passedTimeinUS/1000000.0);
    velocityms=sensordistance/passedTimeinUS;
      DEBUG_PRINT("Geschwindigkeit [m/s]: ");
      DEBUG_PRINTLN(velocityms);
  }
  else{
      DEBUG_PRINT("Error, Negativ Time or same passingtime: ");
      DEBUG_PRINTLN(passedTimeinUS);
    velocityms=0;
  }
  oldpassingtimeLB1=passingtimeLB11;
  oldpassingtimeLB2=passingtimeLB21;
  return velocityms;
}

double calculate_velocity_ms(unsigned long duration,double distance){ //us,um
    DEBUG_PRINTLN("calculate_velocity_ms(unsigned long passingduration,double distance) ");
  double velocityms=0.0;
    DEBUG_PRINT("Zeit [us]: ");
    DEBUG_PRINTLN(duration);
    DEBUG_PRINT("Zeit [s]: ");
    DEBUG_PRINTLN(duration/1000000.0);
    DEBUG_PRINT("Distanz [um]: ");
    DEBUG_PRINTLN(distance);
    DEBUG_PRINT("Distanz [m]: ");
    DEBUG_PRINTLN(distance/1000000.0);
  velocityms=distance/duration;
    DEBUG_PRINT("Geschwindigkeit [m/s]: ");
    DEBUG_PRINTLN(velocityms);
  return velocityms;
}
