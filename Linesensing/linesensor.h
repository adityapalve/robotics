// this #ifndef stops this file
// from being included mored than
// once by the compiler. 
#ifndef _LINESENSOR_H
#define _LINESENSOR_H

# define EMIT_PIN    11    // Documentation says 11.
# define LS_LEFT_PIN 12    // Complete for DN1 pin
# define LS_MIDLEFT_PIN A0 // DN2
# define LS_MID_PIN A2 // DN3
# define LS_MIDRIGHT_PIN A3 // DN4
# define LS_RIGHT_PIN A4 // DN5 
// Class to operate the linesensor(s).
class LineSensor_c {
  public:
    // Constructor, must exist.
    LineSensor_c() {

    } 
    int ls_pins[5] = {LS_LEFT_PIN, 
                      LS_MIDLEFT_PIN, 
                      LS_MID_PIN, 
                      LS_MIDRIGHT_PIN, 
                      LS_RIGHT_PIN
                      };

    void initialize(){
      pinMode( EMIT_PIN, INPUT ); // Set EMIT as an input (off)
      pinMode( LS_LEFT_PIN, INPUT ); // Set line sensor pin to input
    }
    /*
      Return the result, the time elapsed to conduct the sensor reading.
    */
    float readLineSensor(int pin_number){
      pinMode(EMIT_PIN, OUTPUT);
      digitalWrite(EMIT_PIN, HIGH);
      pinMode(pin_number,OUTPUT);
      digitalWrite(pin_number, HIGH);
      delayMicroseconds(10);
      pinMode(pin_number,INPUT);
      unsigned long start_time = micros();
      while(digitalRead(pin_number)==HIGH){
        // till pin has volatge
      }
      unsigned long end_time = micros();
      pinMode(EMIT_PIN, INPUT);
      digitalWrite(EMIT_PIN, LOW);
      unsigned long elapsed_time = end_time - start_time;
      
      return (float)elapsed_time;
    }
  };

#endif

/*
tests
  //  Set the infra-red LEDs to be enabled,
  //  to transmit infra-red light to reflect off the work surface.
  pinMode( EMIT_PIN, OUTPUT );
  digitalWrite(EMIT_PIN, HIGH);

  //  Set the sensor measurement pin to an OUTPUT with state HIGH.
  //  This will produce a positive voltage to charge the capacitor.
  pinMode(LS_LEFT_PIN, OUTPUT);
  digitalWrite( LS_LEFT_PIN, HIGH );
  //  Wait for 10microseconds for the capacitor to charge to full.
  delayMicroseconds(10);

  // Set the sensor measurement pin to an INPUT. 
  // The capacitor we have charged now begins to discharge.
  pinMode( LS_LEFT_PIN, INPUT);

  // Capture the current time in microseconds ( tstart ), 
  // representing the beginning of the sensor reading process.
  unsigned long start_time = micros();
  while(digitalRead(LS_LEFT_PIN)==HIGH){

  }
  unsigned long end_time = micros();
  pinMode(EMIT_PIN, INPUT);
  digitalWrite(EMIT_PIN, LOW);
  unsigned long elapsed_time =  end_time - start_time;
  Serial.println( "ELAPSED TIME:");
  Serial.println((float)elapsed_time);
*/


