#ifndef _BANGBANG_H
#define _BANGBANG_H
#include "motors.h"
// #include "linesensor.h"

// LineSensor_c sensors;
Motors_c motors;

class BangBangController_c{
  public: 
    BangBangController_c(){

    }

    void initialise(){
      motors.initialise();
    }

    void run(float left,float right,float leftThreshold,float rightThreshold, float w_avg){
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
    float weightedMeasurement(float left, float right, float mid){
      float sum = left+right;
      float n_l = (left/sum)*2.0;
      float n_r = (right/sum)*2.0;
      float w = n_l - n_r;
      return w;
    }

    void weigthedAvg(float left, float right, float mid){
      float BiasPWM = 30;
      float MaxTurnPwm = 50;
      float w = 0;
      float leftPWM = 0;
      float rightPWM = 0;
      // readLineSensors() -> Get the sensor updates.
      if(Online==true){
        w = weightedMeasurement(left, right, mid); 
        leftPWM = BiasPWM + (w*MaxTurnPwm);
        rightPWM = BiasPWM - (w*MaxTurnPwm);
        // setMotorPower(leftPWM, rightPWM); set power at this step.
      }else{
        // setMotorPower(0.0,0.0);
      }
    }
};

#endif