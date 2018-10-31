#define MYDEBUG
#include "debug.h"
// #include "pindefine.h"
#include <Arduino.h>

volatile unsigned long passingtimeLB1 = 0;
volatile unsigned long passingtimeLB2 = 0;
unsigned long oldpassingtimeLB1=0;
unsigned long oldpassingtimeLB2=0;
volatile bool LBinterrupted=false;

float sensordistance= 1; //m

#define lightbarrier1 (2) // Interrupt on Pin 1 or 2 (UNO)
#define lightbarrier2 (3)

void ISRLB1() {
  DEBUG_PRINTLN("ISRLB1");
  // detachInterrupt(digitalPinToInterrupt(lightbarrier1));
 passingtimeLB1=micros(); //Auflösung auf 4 micros
 DEBUG_PRINTLN(passingtimeLB1);
  // attachInterrupt(digitalPinToInterrupt(lightbarrier1), ISRtacho, FALLING);
}

void ISRLB2() {
  DEBUG_PRINTLN("ISRLB2");
  // detachInterrupt(digitalPinToInterrupt(lightbarrier2));
 passingtimeLB2=micros();
 DEBUG_PRINTLN(passingtimeLB2);
 LBinterrupted=true;
  // attachInterrupt(digitalPinToInterrupt(lightbarrier2), ISRtacho, FALLING);
}

void setup() {
  Serial.begin(9600);                // Set serial monitor baud rate
 // Interrupt Pins for  Uno, Nano, Mini: 2, 3
  attachInterrupt(digitalPinToInterrupt(lightbarrier1), ISRLB1, FALLING);
  attachInterrupt(digitalPinToInterrupt(lightbarrier2), ISRLB2, FALLING);
  DEBUG_PRINTLN("Arduino Initialise");
}

void loop() {
  delay(5);
  if (LBinterrupted==true) {
      LBinterrupted=false;
      float temp = passingtimeLB2-passingtimeLB1;
      if (temp > 0 && (passingtimeLB1!=oldpassingtimeLB1)) { //&& passingtimeLB2!=oldpassingtimeLB2
        DEBUG_PRINT("Zeit [us]: ");
        DEBUG_PRINTLN(temp);

        DEBUG_PRINT("Zeit [s]: ");
        DEBUG_PRINTLN(temp/1000000);

        DEBUG_PRINT("Geschwindigkeit [m/s]: ");
        DEBUG_PRINTLN(sensordistance/(temp/1000000));
      }
      else{
        DEBUG_PRINT("Error, Negativ Time: ");
        DEBUG_PRINTLN(temp);
      }
      oldpassingtimeLB1=passingtimeLB1;
      oldpassingtimeLB2=passingtimeLB2;
      passingtimeLB1=0;
      passingtimeLB2=0;
  }
}
