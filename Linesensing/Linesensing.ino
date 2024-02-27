#include "encoders.h"
#include "fsm.h"
#define CPR 358.3

FSM_c fsm;


void setup() {
  fsm.initialise();
  // Setup encoders, when the robot is powered on.
  Serial.begin(9600);
  delay(1500);
  Serial.println("***RESET***");

}


void loop() {
  fsm.run();
} 