#include "Arduino.h"
#include "buzzer.h"
#include "limit_switch.h"
#define T_buzzer 500
int temp_;

Buzzer :: Buzzer(){}
   
void Buzzer::init(int pin){
    pinMode(pin,OUTPUT);
    _pin=pin;
}
void Buzzer::on(){
    temp_=ls.readLS2();
    if (temp_==1){
    digitalWrite(_pin,HIGH);
    delay(T_buzzer);
    digitalWrite(_pin,LOW);
    delay(T_buzzer);
    }
}

Buzzer buzzer= Buzzer();

