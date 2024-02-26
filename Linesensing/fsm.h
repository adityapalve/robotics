#ifndef _FSM_H
#define _FSM_H
#include "linesensor.h"
#include "kinematics.h"
#include "bangbang.h"
#include "kinematics.h"
using namespace std;

#define STATE_INITIAL 0
#define STATE_FOUND_LINE 1
#define STATE_DRIVE_FORWARDS 2
#define STATE_JOIN_LINE 3 
#define STATE_FOLLOW_LINE 4
#define STATE_TURN_180 5
#define STATE_REDISCOVER_LINE 6 
#define STATE_RETURN_HOME 7 
#define STATE_TURN_LEFT 8
#define STATE_DRIVE_STR 9
#define STATE_DEADEND 10
#define STATE_FIND 11
#define LEFT_THRESHOLD 1000
#define RIGHT_THRESHOLD 1000
#define DURATION 300
LineSensor_c line_sensors;
// Motors_c motors;
Kinematics_c kinematics;
BangBangController_c controller;

class FSM_c{
  public:
    FSM_c(){

    }
    int state;

    void initialise(){
      line_sensors.initialise();
      // motors.initialise();
      // TODO:
      kinematics.initialise();
      controller.initialise();
      state = STATE_INITIAL;
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
      sensor_data.el = line_sensors.readLineSensor(line_sensors.ls_pins[0]);
      sensor_data.left = line_sensors.readLineSensor(line_sensors.ls_pins[1]);
      sensor_data.mid = line_sensors.readLineSensor(line_sensors.ls_pins[2]);
      sensor_data.right = line_sensors.readLineSensor(line_sensors.ls_pins[3]);
      sensor_data.er = line_sensors.readLineSensor(line_sensors.ls_pins[4]);
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
    bool end;

    void updateState(sensorData sensor_data){
      if(sensor_data.left<leftThreshold && sensor_data.right<RIGHT_THRESHOLD && sensor_data.mid<RIGHT_THRESHOLD && state == STATE_DRIVE_STR){
        state=STATE_TURN_LEFT;
      }else if(sensor_data.left>LEFT_THRESHOLD && sensor_data.right>RIGHT_THRESHOLD && sensor_data.mid > 1000 && state == STATE_FIND){
        state=STATE_FOUND_LINE;
      }else if(sensor_data.mid > 1000 && sensor_data.el < 1400 && sensor_data.er<1500 && state == STATE_TURN_LEFT){
        state = STATE_FOLLOW_LINE;
      }else if(sensor_data.mid<LEFT_THRESHOLD && sensor_data.left<800 && sensor_data.right<800 && sensor_data.er<1000 && state == STATE_FOLLOW_LINE && (kinematics.disp<350 || kinematics.disp>900)){
        state = STATE_DEADEND;
      }
    }

    void driveForwards(){
      motors.moveForward(20);
      state = STATE_FIND;
      delay(800);
    }

    void driveStr(){
      motors.moveForward(20);
    }

    void foundLineBeep(){
      motors.stop();
      state = STATE_DRIVE_STR;
    }

    void followLine(sensorData data){
      float w = controller.weightedMeasurement(data.left, data.right, data.mid, data.el, data.er);
      // Serial.print("W: ");
      // Serial.println(w);
      controller.weigthedAvg(w, data.left, data.mid, data.right, data.el, data.er);
    }

    void joinLine(){
      motors.turnLeft(20);
    }

    void calibration(){
      // can we call initialise here to set everything 0.
      // otherwise set everything 0 urself.
      state = STATE_DRIVE_FORWARDS;
    }
    unsigned long timer = 0; 
    void dead(){
      if(kinematics.disp>1000){
        motors.stop();
        Serial.print("IT REACHED INSIDE THE IFFFFFF");
        state = STATE_RETURN_HOME;
      }else{
        if(timer==0){
          timer = millis();
        }
        if(millis()-timer>DURATION){
          motors.stop();
          timer = 0;
          // can this be join line again?
          state = STATE_REDISCOVER_LINE; 
        }else{
          state = STATE_FOLLOW_LINE;
        }
      }
    }

    void findLine(){
      sensorData d = updateSensors();
      if(d.mid<LEFT_THRESHOLD){
        motors.turnLeft(20);
      }else{
        state = STATE_FOLLOW_LINE;
      }
    }

    void returnHome(){
      // find current angle.
      // change heading towards that angle.
      motors.stop();
      // make the robot go there.
    }

    void run(){
      sensorData data = updateSensors();
      kinematics.update();
      // log(data);
      updateState(data);
      Serial.print("State: ");
      Serial.print(state);
      state = 100;
      // state = STATE_RETURN_HOME;
      if(state == STATE_INITIAL){
        // Set everything to 0
        calibration();        
      }else if(state == STATE_DRIVE_FORWARDS){
        // make it drive straight.
       driveForwards();
      }else if(state == STATE_FOUND_LINE){
        foundLineBeep();
        // state = STATE_FOLLOW_LINE;
      }else if(state == STATE_FOLLOW_LINE){
        followLine(data);
      }else if(state == STATE_TURN_LEFT){
        joinLine(); 
      }else if(state== STATE_DRIVE_STR){
        driveStr(); 
      }else if(state == STATE_DEADEND){
        dead();
      }else if(state == STATE_FIND){
        driveStr();
      }else if(state == STATE_REDISCOVER_LINE){
        findLine();
      } else if(state == STATE_RETURN_HOME){
        returnHome();
      }else {
        // Serial.print("SYSTEM ERROR, unknown state: ");
        // Serial.println(state);
        // motors.stop();
        followLine(data);
      }
    }

    void log(sensorData data){
      Serial.print("Left: ");
      Serial.println(data.left);
      Serial.print("Right: ");
      Serial.println(data.right);
      Serial.print("Mid: ");
      Serial.println(data.mid);
      Serial.print("EL: ");
      Serial.println(data.el);
      Serial.print("ER: ");
      Serial.println(data.er);
      // float w = controller.weightedMeasurement(data.left, data.right, data.mid, data.el, data.er);
      // Serial.print("W: ");
      // Serial.println(w);
      // controller.weigthedAvg(w, data.left, data.mid, data.right, data.el, data.er);

      Serial.print("STATEE: ");
      Serial.println(state);
    }
};

#endif