#ifndef STEPPER17_H
#define STEPPER17_H


class Stepper17{
    private:
      int _step_pin;
      int _dir_pin;
      int _en_pin;

    public:
        Stepper17();
/**
  @brief : membuat program init_stepper_pin()
  @param : melakukan init pin stepper motor sebagai OUTPUT pin
  @note  : -
**/
        void init(int pin1,int pin2,int pin3);
/**
  @brief : membuat program step_move()
  @param : melakukan gerakkan stepper tiap sinyal step
  @note  : -
**/
        void move(uint32_t dir,uint32_t Ts);
/**
  @brief : membuat program move_cam()
  @param : melakukan gerakkan stepper tiap sinyal step
  @note  : menggunakan optimasi kurva pergerakan akselarasi dan deselerasi profile secara real-time
            - 60% konstan speed
            - 20% akselerasi speed
            - 20% deselerasi speed
            - plen sebagai lebar sinyal yang digunakan generate pulsa step
**/
        void move_cam(float x, uint32_t plen);
/**
  @brief : memanggil program step_enable()
  @param : membuat driver dan stepper motor enable
  @note  : -

**/
    void enable();
/**
  @brief : memanggil program step_disable()
  @param : membuat driver dan stepper motor disable (lemas)
  @note  : -

**/
    void disable();
};
extern Stepper17 stepper;//deklare an instance


#endif
