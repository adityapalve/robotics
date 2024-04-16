// this #ifndef stops this file
// from being included mored than
// once by the compiler.
#ifndef _LINESENSOR_H
#define _LINESENSOR_H

# define CENTER_LS A2
# define LEFT_LS A0
# define RIGHT_LS A3
# define WIDE_LEFT_LS A11
# define WIDE_RIGHT_LS A4
# define EMIT_PIN 11
# define LEFT_BUMPER A6
# define RIGHT_BUMPER 5


// Max number of pins to be used:
# define NUM_LS_PINS 5
# define NUM_BUMPER_PINS 2

// We can create an array of sensors to check
int ls_pin[ NUM_LS_PINS ] = {WIDE_LEFT_LS, LEFT_LS, CENTER_LS, RIGHT_LS, WIDE_RIGHT_LS};

int bumper_pins[ NUM_BUMPER_PINS ] = {RIGHT_BUMPER, LEFT_BUMPER};

int bumper_value[2];

// A function to read a line sensor
// - input which line sensor
// - outputs the readings the of the chosen line sensor




// Class to operate the linesensor(s).
class LineSensor_c {
  public:
    float ls_reading[NUM_LS_PINS];

    int bumper[2] = {LEFT_BUMPER, RIGHT_BUMPER};

    // Constructor, must exist.
    LineSensor_c() {

    }

    void Initialise() {
      // SENSOR PIN
      
      pinMode(CENTER_LS, INPUT);
      pinMode(LEFT_LS, INPUT);
      pinMode(RIGHT_LS, INPUT);
      pinMode(WIDE_RIGHT_LS, INPUT);
      pinMode(WIDE_LEFT_LS, INPUT);


      IREmitOff();
    }


    

    void IREmitOn() {
      // EMITTER PIN
      pinMode(EMIT_PIN, OUTPUT);
      digitalWrite(EMIT_PIN, HIGH);

    }

    void IR_BUMPER_EMIT_INIT(){
      pinMode(EMIT_PIN, OUTPUT);
      digitalWrite(EMIT_PIN, LOW);
      pinMode(LEFT_BUMPER, INPUT_PULLUP);                  
      pinMode(RIGHT_BUMPER, INPUT_PULLUP);
    }               

    void IR_BUMPER_ON(){
      digitalWrite(LEFT_BUMPER, LOW);
      digitalWrite(RIGHT_BUMPER, LOW);
    }
    void IR_BUMPER_OFF(){
      digitalWrite(LEFT_BUMPER, HIGH);
      digitalWrite(RIGHT_BUMPER, HIGH);
    }

    void read_bumper(){
//      for (int i = 0; i < 2 ; i++){
//        bumper_value[i] = analogRead(bumper[i]);
        bumper_value[0] = analogRead(LEFT_BUMPER);
        bumper_value[1] = analogRead(RIGHT_BUMPER);
      }


    void IREmitOff() {
      pinMode(EMIT_PIN, INPUT);
    }

    void read_bumper_discrete(){
          // Using which to determine index through each sensor
      IR_BUMPER_ON();

      int which;

      // we will count how many sensors have finished
      // their reading (transitions from high to low).
      int count;

      // Charging the capacitors:
      for ( which = 0; which < 1 ; which++ ) {
        pinMode(bumper_pins[ which ], OUTPUT);
        digitalWrite(bumper_pins[ which ], HIGH);
      }
      // small delay to charge
      delayMicroseconds(10);

      // Now the pin can be set to INPUT since the capacitor is charged
      for ( which = 0; which < 1 ; which++ ) {
        pinMode(bumper_pins[ which ], INPUT);
      }
      // Recording the start time
      unsigned long start_time;
      start_time = micros();

      // Multiple sensors require multiple storage places

      unsigned long end_time_ls[ 1]; // 3 PLACES CONSECUTIVELY
      for ( which = 0; which < 1 ; which++ ) {
        end_time_ls[ which ] = 0;
      }


      // Can use a boolean variable to establish completion
      bool done = false; // Start with false

      count = 1; // starts at 3 and we count down
      // This allows for the while loop to be broken once
      // the reading are completed.


      // Use timeout routine as before
      // Loop to read process until all 3 line sensors
      // have completed their reading
      while ( done == false ) {

        for ( which = 0; which < 1 ; which++ ) {
          //
          if (end_time_ls[ which ] == 0) {
            if (digitalRead( bumper_pins[ which ] ) == LOW) {
              end_time_ls[ which ] = micros();

              // sensor has finished, decrease count.
              count = count - 1;

            }
          }
          if (count == 0) {
            done = true;
          } // if all sensors read
        } // for each sensor
      } // while not all done
      // save readings for each sensor:
      for ( which = 0; which < 1 ; which++ ) {
        unsigned long elapsed_time;
        elapsed_time = end_time_ls[ which ] - start_time;

        ls_reading[ which ] = (float)elapsed_time;


      }

      IR_BUMPER_OFF();
      
      
    }

    void parallelLineSensorRead() {
      // Using which to determine index through each sensor
      IREmitOn();

      int which;

      // we will count how many sensors have finished
      // their reading (transitions from high to low).
      int count;

      // Charging the capacitors:
      for ( which = 0; which < NUM_LS_PINS ; which++ ) {
        pinMode(ls_pin[ which ], OUTPUT);
        digitalWrite(ls_pin[ which ], HIGH);
      }
      // small delay to charge
      delayMicroseconds(10);

      // Now the pin can be set to INPUT since the capacitor is charged
      for ( which = 0; which < NUM_LS_PINS ; which++ ) {
        pinMode(ls_pin[ which ], INPUT);
      }
      // Recording the start time
      unsigned long start_time;
      start_time = micros();

      // Multiple sensors require multiple storage places

      unsigned long end_time_ls[ NUM_LS_PINS]; // 3 PLACES CONSECUTIVELY
      for ( which = 0; which < NUM_LS_PINS ; which++ ) {
        end_time_ls[ which ] = 0;
      }


      // Can use a boolean variable to establish completion
      bool done = false; // Start with false

      count = NUM_LS_PINS; // starts at 3 and we count down
      // This allows for the while loop to be broken once
      // the reading are completed.


      // Use timeout routine as before
      // Loop to read process until all 3 line sensors
      // have completed their reading
      while ( done == false ) {

        for ( which = 0; which < NUM_LS_PINS ; which++ ) {
          //
          if (end_time_ls[ which ] == 0) {
            if (digitalRead( ls_pin[ which ] ) == LOW) {
              end_time_ls[ which ] = micros();

              // sensor has finished, decrease count.
              count = count - 1;

            }
          }
          if (count == 0) {
            done = true;
          } // if all sensors read
        } // for each sensor
      } // while not all done
      // save readings for each sensor:
      for ( which = 0; which < NUM_LS_PINS ; which++ ) {
        unsigned long elapsed_time;
        elapsed_time = end_time_ls[ which ] - start_time;

        ls_reading[ which ] = (float)elapsed_time;


      }

      IREmitOff();

    } // end of parallel read

    bool lineDetected() {
      parallelLineSensorRead();
      if ( ls_reading[1] < 1100 && ls_reading[2] < 1200 && ls_reading[3] < 1000 ) {
        return false;
      }
      else {
        return true;
      }
    }


    
};



#endif
