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
