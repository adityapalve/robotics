// this #ifndef stops this file
// from being included more than
// once by the compiler. 
#ifndef _KINEMATICS_H
#define _KINEMATICS_H
#define CPR 358.3

// Class to track robot position.
class Kinematics_c {
  public:
  
    // Constructor, must exist.
    Kinematics_c() {

    } 
    void initialise(){
      setupEncoder0();
      setupEncoder1();
    }

    long prev_count_e0 = 0;
    long prev_count_e1 = 0;
    float x_i = 0.0;
    float theta_i = 0.0;
    float y_i = 0.0;
    float disp = 0.0;

    float displacement(float x, float y){
      float d = sqrt(x*x + y*y);
      return d;
    }

    void updateKinematics(){
      // Calc the diff between current and prev encoder counts.
      long delta_count_e0 = count_e0 - prev_count_e0;
      long delta_count_e1 = count_e1 - prev_count_e1;

      // Update the prev enc counts
      prev_count_e0 = count_e0;
      prev_count_e1 = count_e1;

      float time_diff=0; 
      // 2r = 32mm, l = 96mm
      float r = 17;
      float l = 85; // l = 90mm was using.
      float dpc = (2*PI)/CPR; // Coeffcient for Distance travelled per count

      float x_r = dpc*r*(delta_count_e0+delta_count_e1)/2;
      float y_r = 0;
      float theta_r = dpc*r*(delta_count_e0-delta_count_e1)/l;

      x_i += x_r*cos(theta_i);
      y_i += x_r*sin(theta_i);
      theta_i += theta_r;
      if(theta_i<0){
        theta_i += (2*PI);
      }else if(theta_i>(2*PI)){
        theta_i -= (2*PI);
      }
    }
    // Use this function to update
    // your kinematics
    unsigned long prev_time = 0;
    unsigned long timestep = 10;
    void update() {
      // Serial.println("Count:");
      // Encoder 0 is for the right tyre.
      // Encoder 1 is for the left tyre
      // Serial.println(count_e0);
      // Serial.print("Left En Count: ");
      // Serial.println(count_e1);

      // TODO: initialize forward drive and verify distance meseaurement
      // using kinematics. 
      // motors.turnRight(30);
      unsigned long current_time = millis();
      if(current_time-prev_time>=timestep){
        updateKinematics();
        prev_time = current_time;
      }
      disp = displacement(x_i, y_i);
      Serial.print("distance travelled: ");
      Serial.println(x_i);
      Serial.print("Displacement: ");
      Serial.println(disp);
      Serial.print("y: ");
      Serial.println(y_i);
      Serial.print("Angle rotated: ");
      Serial.println(theta_i*(180/PI));
    }

};



#endif
