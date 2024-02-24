#ifndef _BANGBANG_H
#define _BANGBANG_H
#include "motors.h"
// #include "linesensor.h"

#define leftThreshold 1000
#define rightThreshold 1000

// LineSensor_c line_sensors;
Motors_c motors;

class BangBangController_c{
  public: 
    BangBangController_c(){

    }

    void initialise(){
      motors.initialise();
      // line_sensors.initialise();
    }

    float weightedMeasurement(float left, float right, float mid){
      float sum = left+right;
      float n_l = (left/sum)*2.0;
      float n_r = (right/sum)*2.0;
      float w = n_l - n_r;
      return w;
    }

    // W -> when -ve robot should turn right.
    // -> when +ve, robot should turn left.
    // Test w values in terms of 

    void weigthedAvg(float w, float left, float mid, float right){
      float BiasPWM = 30;
      float MaxTurnPwm = 20;
      // float w = 0.0;
      // float leftPWM = 0;
      // float rightPWM = 0;
      // readLineSensors() -> Get the sensor updates.
      // use 0.01 for the most smooth operation.
      // if(w<-0.1 || w>0.1){
      if((left<leftThreshold && right>rightThreshold) || (left>leftThreshold && right<rightThreshold)){
        // w = weightedMeasurement(left, right, mid);
        float leftPWM = BiasPWM - (w*MaxTurnPwm);
        float rightPWM = BiasPWM + (w*MaxTurnPwm);
        Serial.print("leftPWM: ");
        Serial.println(leftPWM);
        Serial.print("rightPWM: ");
        Serial.println(rightPWM);
        // setMotorPower(leftPWM, rightPWM); set power at this step.
        motors.setMotorPower(leftPWM,rightPWM);
      }else if(mid>1000){
        motors.setMotorPower(20,20);
      }
      else{
        // motors.turnRight(20);
        motors.stop();
      }
    }

    // void controller(){
    //   float left = line_sensors.readLineSensor(line_sensors.ls_pins[1]);
    //   float mid = line_sensors.readLineSensor(line_sensors.ls_pins[2]);
    //   float right = line_sensors.readLineSensor(line_sensors.ls_pins[3]);
    //   // For running the bangbang controller.
    //   run(left, right, 1000, 1000, w_avg)
      
    //   // For testing the W funtion.
    //   float w = weightedMeasurement(left,right, mid); 
    //   Serial.print("Weight Measurement Coef: ");
    //   Serial.println(w);
    // }

    void run(float left,float right, float mid){
    float w_avg =  (left+mid+right)/3;
      if(left<leftThreshold && right>rightThreshold){
        Serial.println("TURN RIGHT COMMAND");
        // time_start = micros();
        motors.turnRight(20);
        delay(20);
        motors.moveForward(20);
        // time_end = micros();
        // Serial.println(time_end-time_start);
      }else if (right<rightThreshold && left>leftThreshold){
        Serial.print("TURN LEFT COMMAND");
        motors.turnLeft(20);
        delay(20);
        motors.moveForward(20);
      }else if(w_avg<1000){
        Serial.print("its WHITE turn RIGHT");
        motors.turnRight(20);
        // delay(20);
        // motors.moveForward(20);
      }else{
        Serial.print("MOVE FORWARD");
        motors.moveForward(20);
        // motors.stop();
      }
    }

};

/*Notes:
1. Therefore, we could decide the maximum power or rate that the robot could turn (e.g., a PWM value of 100), and then allow a the weighted-measurement of the line  
W  to scale this. Therefore, the more the robot was off the line, the more it would turn. 
Which way to turn, direction, would be implicitly captured by the sign of the value  W

Allowing the W to the scale the Max values is fine, but we will have to keep in mind that the min value from the scaling doesnt hit the deadband. in our case for eg: 20pwm 
Similarly to handle negative values, I will have to check(call a flip in the dir of the motor).

2. 
*/

#endif