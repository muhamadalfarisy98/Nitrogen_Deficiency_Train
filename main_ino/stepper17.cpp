#include "Arduino.h"
#include "stepper17.h"

/*-------Step Pulse definition-----------------*/
#define SPMM 10 //10 Step per mm , mode half step
//extern int Ts=750;
/*Direction motor*/
#define X_DIR_POS 0
#define X_DIR_NEG 1
/*
  MS1 | MS2 | MS3 | MC_STEP       | Resolusi step/rev  |  step angle
  L     L     L     Full step       200                   1.8 
  H     L     L     Half step       400                   0.9
  L     H     L     Quarter step    800                   0.45
  H     H     L     Eight step      1600                  0.225
  H     H     H     16 step
//-->>  1rev ~ 4cm
*/

/*------Deskripsi Pin Driver----------
  EN --> Active Low
  Pin RST dan Sleep --> Active low , perlu di short dirangkaian
*/

//define constructor
Stepper17:: Stepper17(){}
   
void Stepper17::init(int pin1,int pin2,int pin3){
    pinMode(pin1,OUTPUT);
    pinMode(pin2,OUTPUT);
    pinMode(pin3,OUTPUT);
    _step_pin=pin1;
    _dir_pin=pin2;
    _en_pin=pin3;
}

void Stepper17::move(uint32_t dir, uint32_t Ts){
    digitalWrite(_dir_pin,dir); //dir:0 ccw, dir:1 cw
    digitalWrite(_step_pin,HIGH);
    delayMicroseconds(Ts);
    digitalWrite(_step_pin,LOW);
    delayMicroseconds(Ts);

}

void Stepper17::move_cam(float x, uint32_t plen){
    uint32_t x_dir;
    uint32_t x_step;
    uint32_t x_accel_step;
    uint32_t i=0;
    float c= 0.0f;
    uint32_t plen_t;

     /*checking arah putaran*/
    if (x>0){
        x_dir=X_DIR_POS;
    }else{
        x=-x;
        x_dir=X_DIR_NEG;
    }
    /*--Optimasi gerakkan stepper motor secara real-time-----*/
    x_step=(uint32_t)(x*SPMM); 
    x_accel_step=20*x_step/100;
    //ref :https://www.embedded.com/generate-stepper-motor-speed-profiles-in-real-time/

    for(i=0;i<x_accel_step;i++){
        if (i==0)
            c=0.676*2*plen/(sqrtf(1+x_accel_step)-sqrtf(x_accel_step));
        else 
            c-=(2*c)/(4*i+1);
        plen_t=(uint32_t)(c/2);
        move(x_dir,plen_t);
    }
    
    c=2*(float)plen;
    //konstan speed 60%
    for (i=x_accel_step;i< x_step-x_accel_step; i++){
        move(x_dir,plen);
    }

        //deselerasi 20%
    for (i=x_step-x_accel_step;i<x_step;i++){
        c+=(2*c)/(4*(i-x_step)+1);
        plen_t=(uint32_t)c/2;
        move(x_dir,plen_t);
    }
}

void Stepper17 ::enable(){
    digitalWrite(_en_pin,LOW);
}

void Stepper17::disable(){
   digitalWrite(_en_pin,HIGH);
}



Stepper17 stepper= Stepper17(); //deklare instance
