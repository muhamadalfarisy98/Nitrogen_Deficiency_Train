#include "Arduino.h"
#include "limit_switch.h"
#include "stepper17.h"
int a;
int Ts1=750;
//define constructor
Limit_switch:: Limit_switch(){}
   
void Limit_switch::init(int pin1,int pin2){
    pinMode(pin1,INPUT_PULLUP);
    pinMode(pin2,INPUT_PULLUP);
    _limitswitch_pin2=pin2;
    _limitswitch_pin1=pin1;
}
uint32_t Limit_switch::readLS1(){
    return digitalRead(_limitswitch_pin1);
}
uint32_t Limit_switch::readLS2(){
    return digitalRead(_limitswitch_pin2);
}

void Limit_switch ::cek_LS(){
    int LimitS1,LimitS2;
    LimitS1=readLS1();
    LimitS2=readLS2();
    if((LimitS1==LimitS2) && (LimitS1==0)){
        a= LimitS1;
    }else{
        a= 1;
  }
}

void Limit_switch::endstop_LS(){
    cek_LS();
    while (a){
        stepper.enable();
        stepper.move(0,Ts1);  
        cek_LS();
    }
}
Limit_switch ls= Limit_switch(); //deklare instance
