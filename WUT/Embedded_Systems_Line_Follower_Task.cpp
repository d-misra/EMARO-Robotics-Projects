#include "mbed.h"
/*
Objective:  Make a robot that follows a black line, using an IR sensor array.
Locomotion: L298N motor driver I.C with PWM speed control.
Sensors: Analog I.R proximity sensors.
Power supply: 9V DC.

Algorihtm: if line detect = yes:
                go ahead
            else:
                check right
                    if line detect:
                        go ahead
                    else 
                        check left:
                            if line detect :
                                go ahead

Motor control:    Direc      L    R
                    right    0     1
                    Left     1     0
                    Fwd      1     1
                    back     0     0
                    stop     -     -
                    
Analog sensors:  4
Digital sensors: 1

D -> D -> A <- D <- D 

*/

// motor pin setup
DigitalOut m1(PB_5); // motor 1 a
PwmOut speed_1(PB_4); // motor 1  b
DigitalOut led1(LED1);
Serial pc(SERIAL_TX, SERIAL_RX);
PwmOut speed_2(PB_10); // motor 2 a
DigitalOut m4(PA_8); // motor 2 b

/// sensor's setup
AnalogIn left_1(PA_1);
AnalogIn left_2(PA_0);

AnalogIn right_1(PA_4);
AnalogIn right_2(PB_0);

DigitalOut myled(LED1);
DigitalIn center(PA_9);
///

void back(float speed){
    
   m1 = 1;
   m4 = 1;
   speed_1 = speed;
   speed_2 = speed;
   pc.printf("back\n");
    }

void front(float speed){
    m1 = 0;
    m4 = 0;
    speed_1 = speed;
    speed_2 = speed;
    pc.printf("front\n");
    }
    
void left(float speed){
    m1 = 1;
    m4 = 0;
    speed_1 = speed;
    speed_2 = speed;
    pc.printf("left\n");
}

void right(float speed){
    m1 = 0;
    m4 = 1;
    speed_1 = speed;
    speed_2 = speed;   
    pc.printf("right\n");
}  

void robot_stop(){
    m1 = 0;
    m4 = 0;
    speed_1 = 0;
    speed_2 = 0;
}

void read_sensor_array(){
      if(center){
                  
              front(0.5);   
                    
                 }
       else{
                   if(right_1 >= 0.5 && left_1 < 0.5)
                            right(0.2);
                   if(left_1 >= 0.5 && right_1 < 0.5)
                            left(0.2);        
                     }          
                     
}
   
int main(){
     while(1){
     read_sensor_array();    
    }
}
