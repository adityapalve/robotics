#ifndef _FSM_H
#define _FSM_H
#include "linesensor.h"
#include "kinematics.h"
#include "bangbang.h"
#include "kinematics.h"
#include "serial.h"
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
#define STATE_GAP 12
#define STATE_REACHED_HOME 13
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
      float bp_l;
      float bp_r;
    };

    sensorData updateSensors(){
      sensorData sensor_data;
      sensor_data.el = line_sensors.readLineSensor(line_sensors.ls_pins[0]);
      sensor_data.left = line_sensors.readLineSensor(line_sensors.ls_pins[1]);
      sensor_data.mid = line_sensors.readLineSensor(line_sensors.ls_pins[2]);
      sensor_data.right = line_sensors.readLineSensor(line_sensors.ls_pins[3]);
      sensor_data.er = line_sensors.readLineSensor(line_sensors.ls_pins[4]);
      // Using time diff approach
      sensor_data.bp_l = line_sensors.readBumpSensor(line_sensors.bs_pins[0]);
      sensor_data.bp_r = line_sensors.readBumpSensor(line_sensors.bs_pins[1]);
      // Using ADC.
      // sensor_data.bp_l = line_sensors.readBumpSensor_analog(line_sensors.bs_pins[0]);
      // sensor_data.bp_r = line_sensors.readBumpSensor_analog(line_sensors.bs_pins[1]);
      return sensor_data;
    }

    void updateState(sensorData sensor_data){
      if(sensor_data.left<leftThreshold && sensor_data.right<RIGHT_THRESHOLD && sensor_data.mid<RIGHT_THRESHOLD && state == STATE_DRIVE_STR){
        state=STATE_TURN_LEFT;
      }else if(sensor_data.left>LEFT_THRESHOLD && sensor_data.right>RIGHT_THRESHOLD && sensor_data.mid > 1000 && state == STATE_FIND){
        state=STATE_FOUND_LINE;
      }else if(sensor_data.mid > 1000 && sensor_data.el < 1400 && sensor_data.er<1500 && state == STATE_TURN_LEFT){
        state = STATE_FOLLOW_LINE;
      }else if(sensor_data.mid<LEFT_THRESHOLD && sensor_data.left<800 && sensor_data.right<800 && sensor_data.er<1000 
        && state == STATE_FOLLOW_LINE 
        && (kinematics.disp<300 || kinematics.disp>570)){
        state = STATE_DEADEND;
      }else if((sensor_data.left>LEFT_THRESHOLD 
        || sensor_data.right>LEFT_THRESHOLD 
        || sensor_data.el>1400 
        || sensor_data.er > 1400)
        && state ==  STATE_GAP){
          state = STATE_FOLLOW_LINE;
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
      if(timer==0){
        timer = millis();
      }
      if(kinematics.disp>1200){ // was 1180
        motors.stop();
        state = STATE_RETURN_HOME;
      }else{
        if (kinematics.disp>500 && kinematics.disp<1200){ // was 500
            state = STATE_GAP;
        }else{
          if(millis()-timer>DURATION){
          motors.stop();
          timer = 0;
          state = STATE_REDISCOVER_LINE; 
         }else{
            state = STATE_FOLLOW_LINE;
          }
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

    float FINAL_ANGLE; 
    bool heading(){
      float x = kinematics.x_i;
      float y = kinematics.y_i;
      float theta_final = kinematics.theta_i;
      float offset = 12*(PI/180);
      // float offset = 0;
      float targetAngle = (3*PI)/2 - atan2(-x,-y) + offset;
      // float targetAngle = PI/2;
      FINAL_ANGLE = targetAngle;
      
      float tolerance = 2*(PI/180);

      if(abs(targetAngle-theta_final)>tolerance){
        motors.turnRight(20);
        kinematics.update();
        return false;
      }else{
        motors.stop();
        return true;
      }
    }

    float min_d = 1000000;
    bool go_home_c(){
      // Implement a controller to make it run straight.
      kinematics.update();
      // long current_count = kinematics.prev_count_e0;
      float homeDistance = kinematics.disp;
      // float number_of_counts = (CPR/(2*PI*17.2))*homeDistance;
      // Serial.print("number_of_counts:");
      // Serial.println(number_of_counts);
      // Serial.print("Diff:");
      // Serial.println(current_count-count_before_home);
      // if (abs(current_count-count_before_home)<number_of_counts){
      //   return false;
      // }else{
      //   return true;
      // }
      if (homeDistance<min_d){
        min_d = homeDistance;
        return false;
      }else if(homeDistance>(min_d+40)) 
      // && homeDistance>600)
      {
        motors.stop();
        return true;
      }
    }

    void r_drive(){
      float w_coeff = 1.095;
      float leftpwm = 20;
      float rpwm = 20;
      leftpwm = leftpwm*w_coeff;
      motors.setMotorPower(leftpwm,rpwm);
    }

    void dirCorrect(){
      kinematics.update();
      float currHeading = kinematics.theta_i;
      // float f_angle = 3*PI/2 - atan2(-kinematics.x_i,-kinematics.y_i);
      if ((currHeading-FINAL_ANGLE)>PI){
        currHeading = currHeading-2*PI;
      }
      // if (currHeading>PI){
      //   currHeading -= 
      // }
      if(currHeading<FINAL_ANGLE){
        motors.setMotorPower(32,20);
        // while(abs(currHeading-FINAL_ANGLE)>tolerance){
        //   motors.turnRight(20);
        // }
      }else if(currHeading>FINAL_ANGLE){
        // while(abs(currHeading-FINAL_ANGLE)>tolerance){
        //   motors.turnLeft(20*1.095);
        // }
        motors.setMotorPower(20*1.095,35);
      }
    }
    
    // bool go_home(float d_home){
    //   kinematics.update();
    //   Serial.print("Final Angle: ");
    //   Serial.print(FINAL_ANGLE);
    //   float d_curr = kinematics.disp;
    //   float distance_travelled = abs(d_home - d_curr); 
    //   Serial.print("d_home: ");
    //   Serial.println(d_home);
    //   Serial.print("distance travelled: ");
    //   // Serial.println(distance_travelled);
    //   Serial.print("Displacement: ");
    //   Serial.println(d_curr);
    //   // 1445 total distance
    //   if(distance_travelled<d_home){
    //     // motors.setMotorPower(20,20);
    //     gap();
    //     return false;
    //   }else{
    //     motors.stop();
    //     return true;
    //   }
    // }
    bool h_flag = false;
    bool d_flag = false;
    long count_before_home;
    void returnHome(){
      // // find current angle.
      // // change heading towards that angle.
      // make the robot go there.
      while(h_flag == false){
        h_flag = heading();
      }
      kinematics.update();

      // float x_home = kinematics.x_i;
      // float y_home = kinematics.y_i;
      // Serial.print("D_HOME ;");
      // Serial.print(d_home);
      // delay(500);
      // Serial.print();
      // d_flag = true;
      // unsigned prevTime = 0;
      // unsigned tstep = 100;
      count_before_home = kinematics.prev_count_e0;
      while(d_flag == false){
        Serial.print("ITS STUCK HERE");
        d_flag = go_home_c();
        // r_drive();
        dirCorrect();
      }
      state = STATE_REACHED_HOME;
    }

    void gap(){
      float w_coeff = 1.095;
      float leftpwm = 20;
      float rpwm = 20;
      leftpwm = leftpwm*w_coeff;
      motors.setMotorPower(leftpwm,rpwm);
    }

    void run(){
      sensorData data = updateSensors();
      kinematics.update();
      // log(data);
      // updateState(data);
      // Serial.print("State: ");
      // Serial.println(state);
      state = 100;
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
      }else if(state == STATE_GAP){
        gap();
        // motors.stop();
      }else if(state == STATE_REACHED_HOME){
        // reached home.
        // Serial.print("FINAL ANGLE: ");
        // Serial.println(FINAL_ANGLE);
        motors.stop();
      }else {
        // Serial.print("SYSTEM ERROR, unknown state: ");
        // Serial.println(state);
        // log(data);
        // Serial.print("Left BP: ");
        // Serial.println(data.bp_l);
        // Serial.print("Right BP: ");
        // Serial.println(data.bp_r);
        Serial.print("Readings: ");
        Serial.print(data.bp_l);
        Serial.print(",");
        Serial.println(data.bp_r);

        if (data.bp_l<1500 && data.bp_r<1500){
          driveStr();
        // }else if (data.bp_l>800 && data.bp_r>800){
        //   motors.stop();
        // }
        }else{
          motors.stop();
        }
        // delay(200);
        // Serial.print("LS Mid: ");
        // Serial.println(data.mid);
      }
    }

    void log(sensorData data){
      // Serial.print("Left: ");
      // Serial.println(data.left);
      // Serial.print("Right: ");
      // Serial.println(data.right);
      // Serial.print("Mid: ");
      // Serial.println(data.mid);
      // Serial.print("EL: ");
      // Serial.println(data.el);
      // Serial.print("ER: ");
      // Serial.println(data.er);
      // float w = controller.weightedMeasurement(data.left, data.right, data.mid, data.el, data.er);
      // Serial.print("W: ");
      // Serial.println(w);
      // controller.weigthedAvg(w, data.left, data.mid, data.right, data.el, data.er);
      // Serial.print("STATEE: ");
      // Serial.println(state);
    }
};

#endif