//#define MYDEBUG
#include "debug.h"
// #include "pindefine.h"
#include <Arduino.h>

//===========Global Variabel===========
volatile unsigned long passingtimeLB1 = 0;
volatile unsigned long passingtimeLB2 = 0;
unsigned long oldpassingtimeLB1=0;
unsigned long oldpassingtimeLB2=0;
volatile bool LBinterrupted=false;

const double sensordistance= 0.101; //m
// double sensordistance=0.785398163;

//===========Pindefine===========
#define lightbarrier1 (2) // Interrupt on Pin 2 or 3 (UNO)
#define lightbarrier2 (3)

//===========Functionprototype===========
void ISRLB1();
void ISRLB2();
double calculate_velocity_ms();


void setup() {
  Serial.begin(9600);                // Set serial monitor baud rate
 // Interrupt Pins for  Uno, Nano, Mini: 2, 3
  attachInterrupt(digitalPinToInterrupt(lightbarrier1), ISRLB1, FALLING);
  attachInterrupt(digitalPinToInterrupt(lightbarrier2), ISRLB2, FALLING);

  DEBUG_PRINTLN("Arduino Initialise");
}

void loop() {
  delay(1000); //Update only once per Second
  if (LBinterrupted==true) {
      LBinterrupted=false;

      double velocitykmh=calculate_velocity_ms()*3.6;
      if(velocitykmh!=0){
      Serial.print("Geschwindigkeit [km/h]:");
      Serial.print('\t');
      Serial.println(velocitykmh);
    }
 }
}

void ISRLB1() {
 // detachInterrupt(digitalPinToInterrupt(lightbarrier1));
 DEBUG_PRINT("ISRLB1() - ");
 passingtimeLB1=micros(); //Auflösung auf 4 micros
 // delay(1);
 DEBUG_PRINTLN(passingtimeLB1);
 // attachInterrupt(digitalPinToInterrupt(lightbarrier2), ISRLB2, FALLING);
}

void ISRLB2() {
// detachInterrupt(digitalPinToInterrupt(lightbarrier2));
 DEBUG_PRINT("ISRLB2() - ");
  // detachInterrupt(digitalPinToInterrupt(lightbarrier2));
 passingtimeLB2=micros();
 // delay(1);
 DEBUG_PRINTLN(passingtimeLB2);
 LBinterrupted=true;
  // attachInterrupt(digitalPinToInterrupt(lightbarrier2), ISRtacho, FALLING);
  // attachInterrupt(digitalPinToInterrupt(lightbarrier1), ISRLB1, FALLING);
}

double calculate_velocity_ms(){
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
  // passingtimeLB1=0;
  // passingtimeLB2=0;
  return velocityms;
}

// void test_calculate_velocity() {
//   unsigned long start = millis();
//   cout << "millis() test start: " << start << endl;
//
//   }
//   unsigned long end = millis();
//   cout << "End of test - duration: " << end - start << "ms" << endl;
// }
