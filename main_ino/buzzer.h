#ifndef BUZZER_H
#define BUZZER_H

class Buzzer{
    private:
      int _pin;
    public:
        Buzzer();
 /*--------------------BUZZER------------------------------*/
/**
  @brief : membuat fungsi init_buzzer()
  @param : inisialisasi penempatan pin buzzer sebagai output
  @note  : -
**/
        void init(int pin);
/**
  @brief : membuat fungsi buzzer_on()
  @param : mengaktifkan fungsi buzzer disaat yang diinginkan
  @note  : -
**/
        void on();
     
};
extern Buzzer buzzer;//deklare an instance


#endif
