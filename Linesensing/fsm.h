#ifndef _FSM_H
#define _FSM_H
#define STATE_INITIAL 0
#define STATE_FOUND_LINE 1
#define STATE_DRIVE_FORWWARDS 2

class FSM_c{
  public:
    FSM_c(){

    }
    int state;

    void initialise(){

    }

    void updateState(){

    }

    void driveForwards(){

    }

    void foundLineBeep(){

    }
    void calibration(){

    }

    void run(){
      updateState();

      if(state == STATE_INITIAL){

        calibration();
        
      }else if(state == STATE_DRIVE_FORWWARDS){

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