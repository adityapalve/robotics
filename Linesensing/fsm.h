#ifndef _FSM_H
#define _FSM_H
#include "linesensor.h"
#include "motors.h"
#include "kinematics.h"
#include <string.h>
using namespace std;

#define STATE_INITIAL 0
#define STATE_FOUND_LINE 1
#define STATE_DRIVE_FORWARDS 2
#define STATE_JOIN_LINE 3 
#define STATE_FOLLOW_LINE 4
#define STATE_TURN_180 5
#define STATE_TURN_UNTIL_LINE 6 
#define STATE_TURN_HOME 7 

#define LEFT_THRESHOLD 1000
#define RIGHT_THRESHOLD 1000

LineSensor_c line_sensors;
Motors_c motors;
Kinematics_c kinematics;

class FSM_c{
  public:
    FSM_c(){

    }
    int state;

    void initialise(){
      line_sensors.initialise();
      motors.initialise();
      // TODO:
      // kinematics.initialise();
    }

    struct sensorData{
      float el;
      float left;
      float mid;
      float right;
      float er;
    };

    sensorData updateSensors(){
      sensorData sensor_data;
      sensor_data.left = line_sensors.readLineSensor(line_sensors.ls_pins[1]);
      sensor_data.left = line_sensors.readLineSensor(line_sensors.ls_pins[2]);
      sensor_data.right = line_sensors.readLineSensor(line_sensors.ls_pins[3]);
      return sensor_data;
    }
    /*
    STATES: 
    1. Initial State
    2. Drive Straight
    3. Join Line 
    4. Follow Line
    5. Rediscover Line

    Meta States: 
    1. Drive Straight
    2. Turn 180
    3. Turn until line
    4. Turn to Home (for later)
    
    Transition States: 
    S3 -> S4:  when on line.
    S4 -> S5: when all white sensor data(line==false).
      - S4 -> MS2: (line == false)
      - MS2 -> MS2: (angleRotated<180)
    S5 -> S4: when on line.
      - MS2 -> S4: (found line || angleRotated===180).
    
    */

    void updateState(sensorData sensor_data){
    //  Line sensors <- left, mid, right (float 0-2000).
    // 
    if(sensor_data.left<LEFT_THRESHOLD && sensor_data.right>RIGHT_THRESHOLD){
      state=
    }

    }

    void driveForwards(){

    }

    void foundLineBeep(){

    }
    void calibration(){

    }

    void run(){
      sensorData data = updateSensors();
      updateState(data);

      if(state == STATE_INITIAL){

        calibration();
        
      }else if(state == STATE_DRIVE_FORWARDS){

       driveForwards(); 

      }else if(state == STATE_FOUND_LINE){

        foundLineBeep();

      }else{
        Serial.print("SYSTEM ERROR, unknown state: ");
        Serial.println(state);
      }
    }
};

#endif