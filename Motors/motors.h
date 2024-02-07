// this #ifndef stops this file
// from being included mored than
// once by the compiler. 
#ifndef _MOTORS_H
#define _MOTORS_H



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

    }

    // Write a function to operate
    // your motor(s)
    // ...

    
};



#endif

/*
Motor sketch file.
# define L_PWM_PIN 10
# define L_DIR_PIN 16
# define R_PWM_PIN 9
# define R_DIR_PIN 15

# define FWD HIGH
# define REV LOW

void setup(){
  pinMode(L_PWM_PIN,OUTPUT);
  pinMode(R_PWM_PIN,OUTPUT);
  pinMode(L_DIR_PIN,OUTPUT);
  pinMode(R_DIR_PIN,OUTPUT);

  // Start serial
  Serial.begin(9600);
  delay(1000);
  Serial.println("***RESET***");
  }

void loop(){
  analogWrite(L_DIR_PIN,REV);
  analogWrite(R_DIR_PIN,REV);
  // setting the power level for motors.
  analogWrite(L_PWM_PIN, 30);
  analogWrite(R_PWM_PIN, 30);
  
  delay(5);
  }
 */
