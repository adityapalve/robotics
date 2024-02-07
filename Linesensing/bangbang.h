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
};

#endif