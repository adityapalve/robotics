#include "motors.h"

#define LED_PIN 13  // Pin to activate the orange LED
#define L_PWM_PIN 10
#define L_DIR_PIN 16
#define R_PWM_PIN 9
#define R_DIR_PIN 15

#define FWD HIGH
#define REV LOW

void moveForward(){
  analogWrite(L_PWM_PIN, 30);
  analogWrite(R_PWM_PIN, 30);  
}

void moveBackward(){
  analogWrite(L_DIR_PIN, 255);
  analogWrite(R_DIR_PIN, 255);    
  analogWrite(L_PWM_PIN, 20);
  analogWrite(R_PWM_PIN, 20);
  }
  
void turnRight(){
  analogWrite(L_PWM_PIN, 30);
  analogWrite(R_PWM_PIN, 30);
  analogWrite(L_DIR_PIN, 0);
  analogWrite(R_DIR_PIN, 255);
}

void turnLeft(){
  analogWrite(L_PWM_PIN, 0);
  analogWrite(R_PWM_PIN, 30);
}


// put your setup code here, to run once:
void setup() {
  Serial.begin(9600);
  delay(1000);
  Serial.println("***RESET***");
  // Set pin as an output
  pinMode(L_PWM_PIN, OUTPUT);
  pinMode(R_PWM_PIN, OUTPUT);
  pinMode(L_DIR_PIN, OUTPUT);
  pinMode(R_DIR_PIN, OUTPUT);
}

int time = 0;
unsigned long myTime;
void loop() {
  Serial.print("Time: ");
  myTime = millis();
//  Setting the motor power at 30 i.e, 8% duty-cycle.
  if (time<2){
    moveBackward();
    time++;
  }
  else{
    turnRight();
    time++;
  }
  Serial.println(myTime);
  delay(1000);
}
