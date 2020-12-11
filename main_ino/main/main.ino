/*program untuk menjalankan stepper motor*/
/*halfstep mode, externally wiring */
#include "Arduino.h"
#include "buzzer.h"
#include "led_strip.h"
#include "limit_switch.h"
#include "stepper17.h"

#include <EEPROM.h>
#include <Wire.h> //library I2C
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2);
#include <SoftwareSerial.h> //UART communication protocol
/*protokol komunikasi UART */
SoftwareSerial Serial1(2,3); //RX-TX
/*------------API Pin Wiring -------------------*/
/*Buzzer pin*/
#define buzzer_pin 11
/*driver pin*/
#define en_pin 10
#define dir_pin 8
#define step_pin 9
/*LED STRIP 12VDC pin*/
#define led_strip_pin 6
/*Limit switch pin*/
#define LS1 12
#define LS2 13

/*  I2C Communication Protocol
 SCL->A5
 SDA->A4
*/
extern int Ts=750;
int increment=150;//15cm assumsi
/*--Deskripsi variabel sensor laser*/
int dist;
int strength;
int check;
int uart[9];
int i;
const int HEADER=0x59;


/*----variable serial komunikasi-----*/
long baca_parse=0;
float angka_f;
int addr=0;

/*----variable buzzer--------------*/
int T_buzzer=500;

/*---positioning variable--- */
int error_flag = 0;
float curr_position = 0;
float target_position = 0;
float upper_error_limit = 0;
float lower_error_limit = 0;
int _hole;

/*--Intensitas Cahaya--*/
int BH1750address=0x23;
byte  buff[2];
int registerRead=2;
int baca_address1750;
uint16_t value=0;

void setup_komunikasi(){
  Wire.begin(); // set komunikasi I2C protokol
  Serial.begin(9600);//set baud rate Serial monitor
  Serial1.begin(115200); // set bauda rate komunikasi UART 
}

//*fungsi reset softwarely*//
void(*reset_kata_faris)(void)=0;


/*inisiasi*/
void setup(){
  setup_komunikasi();
  lcd.init();
  lcd.backlight();
  lcd.setCursor(3,0);//(kolom,baris)
  lcd.print("TA192001019");
  lcd.setCursor(0,1);
  lcd.print("NitrogenDetector");
  delay(5000);
  lcd.clear();
  stepper.init(en_pin,dir_pin,step_pin);
  stepper.disable();// biar gak getar2 dulu stepper motornya
  led_strip.init(led_strip_pin);
  led_strip.on();
  ls.init(LS1,LS2);
  buzzer.init(buzzer_pin);
  lcd.setCursor(1,0);
  lcd.print("Homing...");
  delay(1000);
  ls.endstop_LS();
  lcd.clear();
  lcd.setCursor(1,0);
  lcd.print("Operating..");
  delay(500);
  stepper.move_cam(-300,Ts); //setpoint gerak ke 30cm.
  lcd.clear();
  _hole=0;
  cek_hole_state();

}

/*------program utama-------*/
void loop(){
  BH1750_Init(BH1750address);
  delay(200);
  baca_address1750=BH1750_Read(BH1750address);
  konversi_lux();
  //baca_laser();
  print_lcd();
  stepper.disable();
  
  while (_hole<15){
    stepper.enable();
    stepper.move_cam(increment,Ts);
    stepper.disable();
    //cekjarak feedback
    /*
      if jarak gak sesuai maka panggil
      reset_kata_faris(); //buat reset ke awal
    */
    //panggil fungsi take picture
    buzzer.on();
    EEPROM.update(addr,_hole);
    _hole++;
    BH1750_Init(BH1750address);
    delay(200);
    baca_address1750=BH1750_Read(BH1750address);
    konversi_lux();
    //baca_laser();
    print_lcd();
  }

  /*
    if(Serial.available()>0){
        baca_parse=Serial.parseFloat();
        angka_f=baca_parse;
        //menggerakan kamera
        stepper.enable();
        stepper.move_cam(angka_f,Ts);
        buzzer.on();
        stepper.disable();
  }
  */
}

/*-----------I2C intensitas cahaya---------------------*/
/**
  @brief : membuat fungsi BH1750_Read(int address)
  @param : membaca register BH1750
  @note  : -

**/
uint32_t BH1750_Read(int address)
{
  int i=0;
  Wire.beginTransmission(address);
  Wire.requestFrom(address,2);
  while(Wire.available()){
    buff[i]=Wire.read();
    i++;
  }
  Wire.endTransmission();
  return i;
}
/**
  @brief : mmembuat fungsi BH1750_init()
  @param : ini komunikasi I2C 
  @note  : -

**/
void BH1750_Init(int address){
  Wire.beginTransmission(address);
  Wire.write(0x10);
  Wire.endTransmission();
}

/**
  @brief : membuat fungsi konversi_lux()
  @param : comparing pembacaan register dan perhitungan lux
  @note  : -

**/
void konversi_lux(){
  if (registerRead== baca_address1750 ){
    value=((buff[0]<<8)|buff[1])/1.2;
    //cetak_lux();
  }
}

void print_lcd(){
  lcd.setCursor(0,0);
  lcd.print("Lux: ");
  lcd.setCursor(5,0);
  lcd.print(value);
  lcd.setCursor(0,1);
  lcd.print("Posisi: ");
  lcd.setCursor(8,1);
  lcd.print(dist);
  delay(1000);
  lcd.clear();
}
/**
  @brief : membuat fungsi baca_laser()
  @param : membaca nilai pada komunikai serial UART dalam 9 byte hexa
  @note  : - byte (1 & 2 : berupa frame header)
           - byte (3 & 4 : berupa jarak dalam hexa)

**/
void baca_laser(){
   if (Serial1.available()){
    if(Serial1.read()==HEADER){
      uart[0]=HEADER;
      if(Serial1.read()==HEADER){
        uart[1]=HEADER;
        for(i=2;i<9;i++){
            uart[i]=Serial1.read();
        }
        check=uart[0]+uart[1]+uart[2]+uart[3]+uart[4]+uart[5]+uart[6]+uart[7];  
        if(uart[8]==(check&0xff)){
          dist=uart[2]+uart[3]*256;
          strength=uart[4]+uart[5]*256;
          Serial.print("jarak : ");
          Serial.println(dist);
          delay(100);
          
        }
       }
    } 
  }
}


void cek_hole_state(){
  int hole_state=EEPROM.read(addr);
  if(hole_state>0){
    int pindah=hole_state*increment;
    stepper.enable();
    stepper.move_cam(pindah,Ts);
    stepper.disable();
    _hole=hole_state;
  }
}
