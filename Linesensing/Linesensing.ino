#include "linesensor.h"
#include "motors.h"
// #include "bangbang.h"
#include "encoders.h"
#include "fsm.h"
#define CPR 358.3

LineSensor_c line_sensors;
Motors_c motors;
FSM_c fsm;
// BangBangController_c b2;

long prev_count_e0 = 0;
long prev_count_e1 = 0;
float x_i = 0.0;
float theta_i = 0.0;
void updateKinematics(){
  // Calc the diff between current and prev encoder counts.
  long delta_count_e0 = count_e0 - prev_count_e0;
  long delta_count_e1 = count_e1 - prev_count_e1;

  // Update the prev enc counts
  prev_count_e0 = count_e0;
  prev_count_e1 = count_e1;

  float time_diff=0; 
  // 2r = 32mm, l = 96mm
  float r = 17;
  float l = 90;
  float dpc = (2*PI)/CPR; // Coeffcient for Distance travelled per count

  float x_r = dpc*r*(delta_count_e0+delta_count_e1)/2;
  float y_r = 0;
  float theta_r = dpc*r*(delta_count_e0-delta_count_e1)/l;

  x_i += x_r*cos(theta_i);
  theta_i += theta_r;
}

void setup() {
  // Set some initial pin modes and states
  line_sensors.initialise();
  motors.initialise();
  // Start Serial, wait to connect, print a debug message.
  // b2.initialise();
  fsm.initialise();
  // Setup encoders, when the robot is powered on.
  setupEncoder0();
  setupEncoder1();
  Serial.begin(9600);
  delay(1500);
  Serial.println("***RESET***");
}

unsigned long prev_time = 0;
unsigned long timestep = 100;

void loop() {
  // Abstract this in a controller file.
  float left = line_sensors.readLineSensor(line_sensors.ls_pins[1]);
  float mid = line_sensors.readLineSensor(line_sensors.ls_pins[2]);
  float right = line_sensors.readLineSensor(line_sensors.ls_pins[3]);
  // float w_avg =  (left+mid+right)/3;
  fsm.run();
  /* TODO: 
    3. Write a bool lineDetected() function to confirm robot is on black line.
    4. Using it allow robot to start off anywhere and find the black line, 
       and then allow calling the bang bang controller.
  */

  float leftThreshold = 1000;
  float rightThreshold = 1000;

  // b2.run(left,right, leftThreshold, rightThreshold, w_avg);

  // Serial.print("left: ");
  // Serial.print(left);
  // Serial.print(" Mid:");
  // Serial.println(mid);
  // Serial.print(" Right: ");
  // Serial.print(right);
  // Serial.print(" W Avg:");
  // Serial.println(w_avg);
  // Serial.println("***");
  // delay(500);
  // Serial.println("Count:");
  // Encoder 0 is for the right tyre.
  // Encoder 1 is for the left tyre
  // Serial.println(count_e0);
  // Serial.print("Left En Count: ");
  // Serial.println(count_e1);

  // TODO: initialize forward drive and verify distance meseaurement
  // using kinematics. 
  // motors.turnRight(30);
  unsigned long current_time = millis();
  if(current_time-prev_time>=timestep){
    updateKinematics();
    prev_time = current_time;
  }
  Serial.print("distance travelled: ");
  Serial.println(x_i);
  Serial.print("Angle rotated: ");
  Serial.println(theta_i);
} 