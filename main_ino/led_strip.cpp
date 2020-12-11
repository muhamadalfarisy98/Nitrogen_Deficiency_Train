#include "Arduino.h"
#include "led_strip.h"


//define constructor
Led_strip :: Led_strip(){}
   
void Led_strip::init(int pin){
    pinMode(pin,OUTPUT);
    _ledstrip_pin=pin;
}
void Led_strip::on(){
    digitalWrite(_ledstrip_pin,HIGH);
}

Led_strip led_strip= Led_strip(); //deklare instance
