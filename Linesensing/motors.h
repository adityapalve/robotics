// this #ifndef stops this file
// from being included mored than
// once by the compiler. 
#ifndef _MOTORS_H
#define _MOTORS_H

#define LED_PIN 13  // Pin to activate the orange LED
#define L_PWM_PIN 10
#define L_DIR_PIN 16
#define R_PWM_PIN 9
#define R_DIR_PIN 15

#define FWD HIGH
#define REV LOW


// Class to operate the motor(s).
class Motors_c {
  public:

    // Constructor, must exist.
    Motors_c() {

    } 

    // Use this function to 
    // initialise the pins and 
    // state of your motor(s).
    void initialise() {
      pinMode(L_PWM_PIN, OUTPUT);
      pinMode(R_PWM_PIN, OUTPUT);
      pinMode(L_DIR_PIN, OUTPUT);
      pinMode(R_DIR_PIN, OUTPUT);
    }
    
    void setMotorPower(float leftPWM,float rightPWM){
      if(leftPWM<0){
        analogWrite(L_DIR_PIN, 255);
      }else{ 
        analogWrite(L_DIR_PIN, 0);
      }

      if(rightPWM<0){
        analogWrite(R_DIR_PIN, 255);
      }else{
        analogWrite(R_DIR_PIN, 0);
      }

      analogWrite(L_PWM_PIN, leftPWM);
      analogWrite(R_PWM_PIN, rightPWM);

    }

    void moveForward(int power){
      analogWrite(L_DIR_PIN, 0);
      analogWrite(R_DIR_PIN, 0);
      analogWrite(L_PWM_PIN, power);
      analogWrite(R_PWM_PIN, power);  
    }

    void moveBackward(int power){
      analogWrite(L_DIR_PIN, 255);
      analogWrite(R_DIR_PIN, 255);    
      analogWrite(L_PWM_PIN, power);
      analogWrite(R_PWM_PIN, power);
      }
      
    void turnRight(int power){
      analogWrite(L_PWM_PIN, power);
      analogWrite(R_PWM_PIN, power);
      analogWrite(L_DIR_PIN, 0);
      analogWrite(R_DIR_PIN, 255);
    }

    void turnLeft(int power){
      analogWrite(R_DIR_PIN, 0);
      analogWrite(L_DIR_PIN, 255);
      analogWrite(L_PWM_PIN, power);
      analogWrite(R_PWM_PIN, power);
    }

    // void turn180(int power){
    //   analogWrite(R_DIR_PIN,255);
    //   analogWrite(L_DIR_PIN, 0);
    //   analogWrite(R_PWM_PIN, power);
    //   analogWrite(L_PWM_PIN, power);
    // }
    void stop(){
      analogWrite(L_DIR_PIN, 0);
      analogWrite(R_DIR_PIN, 0);
      analogWrite(R_PWM_PIN, 0);
      analogWrite(L_PWM_PIN, 0);
    }

    
};



#endif
