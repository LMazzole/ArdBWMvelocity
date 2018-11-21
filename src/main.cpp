// #define MYDEBUG
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

const double sensordistance= 101.0*1000; //um
const double ballwidth= 72.0*1000; //um -> 72mm

double velocitykmh[6];
unsigned long passingtime[6];
//===========Pindefine===========
// #define lightbarrier1 (2) // Interrupt on Pin 2 or 3 (UNO)
// #define lightbarrier2 (3)
const byte lightbarrier1 = 2;
const byte lightbarrier2 = 3;

//===========Functionprototype===========
void ISRLB1();
void ISRLB2();
// double calculate_velocity_ms();
double calculate_velocity_ms(unsigned long passingduration,double distance);
int countingvar = 0;

void setup() {
  Serial.begin(9600);                // Set serial monitor baud rate
 // Interrupt Pins for  Uno, Nano, Mini: 2, 3
  attachInterrupt(digitalPinToInterrupt(lightbarrier1), ISRLB1, CHANGE);
  attachInterrupt(digitalPinToInterrupt(lightbarrier2), ISRLB2, CHANGE);
    DEBUG_PRINTLN("Arduino Initialise");
      Serial.println("");
      Serial.print("Iter");
      Serial.print("\t");
      Serial.print("Bez");
      Serial.print("\t");
      Serial.print("Puls LS 1");
      Serial.print("\t");
      Serial.print("Puls LS 2");
      Serial.print("\t");
      // Serial.print("Int LS 12");
      // Serial.print("\t");
      Serial.print("Int LS 1");
      Serial.print("\t");
      Serial.print("Int LS 2");
      Serial.print("\t");
      Serial.print("Int LS 11-21");
      Serial.print("\t");
      Serial.print("Int LS 12-22");
      Serial.println("\t");
}

void loop() {
  //delay(500); //Update only once per Second
  passingdurationLB1 = pulseIn(lightbarrier1, LOW);
    DEBUG_PRINTLN(passingdurationLB1);
  passingdurationLB2 = pulseIn(lightbarrier2, LOW);
    DEBUG_PRINTLN(passingdurationLB2);

  if (LBinterrupted){
    LBinterrupted=false;
    noInterrupts();
      passingtime[0]=passingdurationLB1;
      passingtime[1]=passingdurationLB2;
      passingtime[2]=passingtimeLB11;
      passingtime[3]=passingtimeLB12;
      passingtime[4]=passingtimeLB21;
      passingtime[5]=passingtimeLB22;
    interrupts();

     velocitykmh[0]=calculate_velocity_ms(passingtime[0],ballwidth)*3.6;
     velocitykmh[1]=calculate_velocity_ms(passingtime[1],ballwidth)*3.6;
     velocitykmh[2]=calculate_velocity_ms(passingtime[3]-passingtime[2],ballwidth)*3.6;
     velocitykmh[3]=calculate_velocity_ms(passingtime[5]-passingtime[4],ballwidth)*3.6;
     velocitykmh[4]=calculate_velocity_ms(passingtime[4]-passingtime[2],sensordistance)*3.6;
     velocitykmh[5]=calculate_velocity_ms(passingtime[5]-passingtime[3],sensordistance)*3.6;


      countingvar+=1;
      Serial.print(countingvar);
      Serial.print("\t");
      Serial.print("Geschwindigkeit [km/h]:");
      Serial.print("\t");
      Serial.print(velocitykmh[0]);
      Serial.print("\t");
      Serial.print(velocitykmh[1]);
      Serial.print("\t");
      Serial.print(velocitykmh[2]);
      Serial.print("\t");
      Serial.print(velocitykmh[3]);
      Serial.print("\t");
      Serial.print(velocitykmh[4]);
      Serial.print("\t");
      Serial.println(velocitykmh[5]);
    DEBUG_PRINTLN("===================================================");
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
    LBinterrupted=true;
  }
  else{
    passingtimeLB21=passingtimeLB2; //Auflösung auf 4 micros
    DEBUG_PRINTLN(passingtimeLB21);
  }
}


double calculate_velocity_ms(unsigned long duration,double distance){ //us,um
    DEBUG_PRINTLN("calculate_velocity_ms(unsigned long passingduration,double distance) ");
  double velocityms=0.0;
  velocityms=distance/duration;
    DEBUG_PRINT("Geschwindigkeit [m/s]: ");
    DEBUG_PRINTLN(velocityms);
  return velocityms;
}
