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

    float weightedMeasurement(float left, float right, float mid, float el, float er){
      float sum = left+right;
      float n_l = (left/sum)*2.0;
      float n_r = (right/sum)*2.0;
      float w = n_l - n_r;
      return w;
      // float sum = left + right + el + er;
    
      // // Normalize the values to a scale of -2 to 2, accounting for all sensors
      // float n_el = (el / sum) * 4.0; // More weight if we need sharper turn
      // float n_left = (left / sum) * 4.0;
      // float n_right = (right / sum) * 4.0;
      // float n_er = (er / sum) * 4.0; // More weight if we need sharper turn
      
      // // Calculate the weighted measurement
      // float w = (el + n_left) - (n_right + er);
      
      // // Detect if a 90-degree turn might be needed
      // if(mid < 1000 && left < 1000 && right < 1000){
      //     // All middle sensors detect white, potential 90-degree turn
      //   if(el > 1500){
      //       // Far left sensor detects the line, suggesting a turn to the left
      //     w = -2; // Arbitrary strong left turn signal
      //   } else if(er > 1500){
      //       // Far right sensor detects the line, suggesting a turn to the right
      //     w = 2; // Arbitrary strong right turn signal
      //   }
      // }
      
      // return w;

    }

    // W -> when -ve robot should turn right.
    // -> when +ve, robot should turn left.
    // Test w values in terms of 

    void weigthedAvg(float w, float left, float mid, float right, float el, float er){
      float BiasPWM = 30;
      float MaxTurnPwm = 20;
      float w_avg = (left+mid+right)/3;
      Serial.print("EL: ");
      Serial.println(el);
      Serial.print("ER: ");
      Serial.println(er);
      float leftPWM, rightPWM;
      // use 0.01 for the most smooth operation.
      // if(w<-0.1 || w>0.1){
      if((left<leftThreshold && right>rightThreshold) || (left>leftThreshold && right<rightThreshold)){
        // w = weightedMeasurement(left, right, mid);
        leftPWM = BiasPWM - (w*MaxTurnPwm);
        rightPWM = BiasPWM + (w*MaxTurnPwm);
        // Serial.print("leftPWM: ");
        // Serial.println(leftPWM);
        // Serial.print("rightPWM: ");
        // Serial.println(rightPWM);
        motors.setMotorPower(leftPWM,rightPWM);
      }else if(mid>1000){
        motors.setMotorPower(20,20);
      }
      else if(w_avg<1000){
        // motors.turnRight(20);
        // Cannot use this directly. Switching directions this quickly screws 
        // up the weighted controller
        Serial.print("EL: ");
        Serial.println(el);
        Serial.print("ER: ");
        Serial.println(er);
        if(el<1600 && er>1500){
          // turn R
          motors.setMotorPower(20,0);
        }else if(el>1600 && er<1500){
          // turn L
          motors.setMotorPower(0,20);
        }
        // Adjust for strong turn signals specifically for 90-degree turns.
      // else if (w == -2) { // Strong signal for sharp left turn.
      //   leftPWM = 0;
      //   rightPWM = BiasPWM + MaxTurnPwm; // Turn in place or sharp turn.
      // }else if (w == 2) { // Strong signal for sharp right turn.
      //   leftPWM = BiasPWM + MaxTurnPwm;
      //   rightPWM = 0; // Turn in place or sharp turn.
      }else{
        motors.stop();
      }
    }

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