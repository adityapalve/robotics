#include "linesensor.h"
#include "motors.h"
#include "bangbang.h"

LineSensor_c line_sensors;
// Motors_c motors;
BangBangController_c b2;

void setup() {
  // Set some initial pin modes and states
  line_sensors.initialize();
  motors.initialise();
  // Start Serial, wait to connect, print a debug message.
  b2.initialise();
  Serial.begin(9600);
  delay(1500);
  Serial.println("***RESET***");
}

void loop() {
  // Abstract this in a controller file.
  float left = line_sensors.readLineSensor(line_sensors.ls_pins[1]);
  float mid = line_sensors.readLineSensor(line_sensors.ls_pins[2]);
  float right = line_sensors.readLineSensor(line_sensors.ls_pins[3]);
  float w_avg =  (left+mid+right)/3;

  /* TODO: 
    3. Write a bool lineDetected() function to confirm robot is on black line.
    4. Using it allow robot to start off anywhere and find the black line, and then allow calling the bang bang controller.
  */

  float leftThreshold = 1000;
  float rightThreshold = 1000;
  int time_start, time_end;

  b2.run(left,right, leftThreshold, rightThreshold, w_avg);

  Serial.print("left: ");
  Serial.print(left);
  Serial.print(" Mid:");
  Serial.print(mid);
  Serial.print(" Right: ");
  Serial.print(right);
  Serial.print(" W Avg:");
  Serial.println(w_avg);
  Serial.println("***");
  // delay(500);
} 
