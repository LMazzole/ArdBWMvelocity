/*
File: BWMvelocity.cpp
Author: Luca Mazzoleni
Date: 27.11.2018

HSR Hochschule Rapperswil

Description:
Programm which detects the Speed of an Object that passes two Lightbarrier.
Either with interrupt or polling.

Pythonscript for Datalogging:
https://github.com/LMazzole/ArdPyt-ReadSerial
*/
#include <Arduino.h>

//==== Defines ====
/*
To switch between the mode Debug, Datalog and Interrupt comment
or uncomment the corresponde define below. All modes are possible at the same time.
*/

// #define MYDEBUG  // Uncomment to see all DEBUG- Messages
#define MYLOG //Uncomment to see all Log-Message. Check the Pythonfile for Serialdataloggin.
#define USEINTERRUPT //Uncomment so use interrupts inseatd of polling

//Define Debug-Function
#ifdef MYDEBUG
  #define DEBUG_PRINT(x)        Serial.print (x)
  #define DEBUG_PRINTDEC(x)     Serial.print (x, DEC)
  #define DEBUG_PRINTLN(x)      Serial.println (x)
#else
  #define DEBUG_PRINT(x)
  #define DEBUG_PRINTDEC(x)
  #define DEBUG_PRINTLN(x)
#endif

//Define Log-Function for Python-Script
#ifdef MYLOG
  #define LOG_PRINT(x)        Serial.print (x)
  #define LOG_PRINTDEC(x)     Serial.print (x, DEC)
  #define LOG_PRINTLN(x)      Serial.println (x)
#else
  #define LOG_PRINT(x)
  #define LOG_PRINTDEC(x)
  #define LOG_PRINTLN(x)
#endif

//===========Global Variabel===========
const double sensordistance= 101.0*1000; //um -> 101mm
const double ballwidth= 72.0*1000; //um -> 72mm

double velocitykmh[4];
unsigned long passingtime[4];
int countingvar = 0;

#ifdef USEINTERRUPT
  volatile unsigned long passingtimeLB1 = 0;
  volatile unsigned long passingtimeLB11 = 0;
  volatile unsigned long passingtimeLB12 = 0;
  volatile unsigned long passingtimeLB2 = 0;
  volatile unsigned long passingtimeLB21 = 0;
  volatile unsigned long passingtimeLB22 = 0;
  volatile bool LBinterrupted=false;
#else
  unsigned long passingdurationLB1 = 0;
  unsigned long passingdurationLB2 = 0;
#endif

//===========Pindefine===========
// Interrupt Pins for  Uno, Nano, Mini: 2, 3
const byte lightbarrier1 = 2;
const byte lightbarrier2 = 3;

//===========Functionprototype===========
double calculate_velocity_ms(unsigned long passingduration,double distance);
void printlog(double speedinkmh[4]);
#ifdef USEINTERRUPT
  void ISRLB1();
  void ISRLB2();
#endif

void setup() {
  Serial.begin(57600);                // Set serial monitor baud rate. For Datalogging with python the baudrates need to be consistent
  #ifdef USEINTERRUPT
    attachInterrupt(digitalPinToInterrupt(lightbarrier1), ISRLB1, CHANGE);
    attachInterrupt(digitalPinToInterrupt(lightbarrier2), ISRLB2, CHANGE);
  #endif

  DEBUG_PRINTLN("Arduino Initialise");
  LOG_PRINTLN("");
  LOG_PRINT("Iter"); LOG_PRINT("\t");
  LOG_PRINT("Bez"); LOG_PRINT("\t");
  #ifdef USEINTERRUPT
    LOG_PRINT("Int LB 1"); LOG_PRINT("\t");
    LOG_PRINT("Int LB 2"); LOG_PRINT("\t");
    LOG_PRINT("Int LB 11-21"); LOG_PRINT("\t");
    LOG_PRINT("Int LB 12-22");
  #else
    LOG_PRINT("Puls LB 1"); LOG_PRINT("\t");
    LOG_PRINT("Puls LB 2");
  #endif
  LOG_PRINTLN("");
}

void loop() {
  #ifdef USEINTERRUPT
  noInterrupts(); //eliminates racecondition
    if (LBinterrupted){
        LBinterrupted=false;
        passingtime[0]=passingtimeLB11;
        passingtime[1]=passingtimeLB12;
        passingtime[2]=passingtimeLB21;
        passingtime[3]=passingtimeLB22;
      interrupts();
       velocitykmh[0]=calculate_velocity_ms(passingtime[1]-passingtime[0],ballwidth)*3.6;
       velocitykmh[1]=calculate_velocity_ms(passingtime[3]-passingtime[2],ballwidth)*3.6;
       velocitykmh[2]=calculate_velocity_ms(passingtime[2]-passingtime[0],sensordistance)*3.6;
       velocitykmh[3]=calculate_velocity_ms(passingtime[3]-passingtime[1],sensordistance)*3.6;
       printlog(velocitykmh);
     }
   interrupts();
   /*do something else*/
  #else //Polling-Mode
     passingdurationLB1 = pulseIn(lightbarrier1, LOW);
        DEBUG_PRINTLN("passingdurationLB1: ");
        DEBUG_PRINTLN(passingdurationLB1);
      if(passingdurationLB1!=0){
        passingdurationLB2 = pulseIn(lightbarrier2, LOW,500000);
           DEBUG_PRINTLN("passingdurationLB2: ");
           DEBUG_PRINTLN(passingdurationLB2);
        if(passingdurationLB2!=0){
          passingtime[0]=passingdurationLB1;
          passingtime[1]=passingdurationLB2;
          velocitykmh[0]=calculate_velocity_ms(passingtime[0],ballwidth)*3.6;
          velocitykmh[1]=calculate_velocity_ms(passingtime[1],ballwidth)*3.6;
          printlog(velocitykmh);
        }
        passingdurationLB1 = 0;
        passingdurationLB2 = 0;
      }
   #endif
 DEBUG_PRINTLN("===================END-LOOP==============================");
}

double calculate_velocity_ms(unsigned long duration,double distance){ //us,um
    DEBUG_PRINTLN("calculate_velocity_ms(unsigned long passingduration,double distance) ");
  double velocityms=0.0;
  velocityms=distance/duration;
    DEBUG_PRINT("Geschwindigkeit [m/s]: ");
    DEBUG_PRINTLN(velocityms);
  return velocityms;
}

void printlog(double speedinkmh[4]){
    DEBUG_PRINTLN("printlog(double speedinkmh[4])");
    countingvar+=1;
    LOG_PRINT(countingvar); LOG_PRINT("\t");
    LOG_PRINT("Geschwindigkeit [km/h]:"); LOG_PRINT("\t");
    LOG_PRINT(velocitykmh[0]); LOG_PRINT("\t");
    LOG_PRINT(velocitykmh[1]);
    #ifdef USEINTERRUPT
      LOG_PRINT("\t");
      LOG_PRINT(velocitykmh[2]); LOG_PRINT("\t");
      LOG_PRINT(velocitykmh[3]);
    #endif
    LOG_PRINTLN("");
}

#ifdef USEINTERRUPT
  void ISRLB1() {
      DEBUG_PRINT("ISRLB1() - ");
    passingtimeLB1=micros(); //Resolution 4 micros
    if(digitalRead(lightbarrier1)){ //Direct Port Acces would be faster
      passingtimeLB12=passingtimeLB1;
        DEBUG_PRINTLN(passingtimeLB12);
      LBinterrupted=false;
    }
    else{
      passingtimeLB11=passingtimeLB1;
        DEBUG_PRINTLN(passingtimeLB11);
      LBinterrupted=false;
    }
  }

  void ISRLB2() {
      DEBUG_PRINT("ISRLB2() - ");
    passingtimeLB2=micros(); //Resolution 4 micros
    if(digitalRead(lightbarrier2)){
      passingtimeLB22=passingtimeLB2;
        DEBUG_PRINTLN(passingtimeLB22);
      LBinterrupted=true;
    }
    else{
      passingtimeLB21=passingtimeLB2;
        DEBUG_PRINTLN(passingtimeLB21);
      LBinterrupted=false;
    }
  }
#endif
