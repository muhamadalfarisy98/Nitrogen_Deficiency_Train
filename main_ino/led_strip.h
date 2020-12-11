#ifndef LED_STRIP_H
#define LED_STRIP_H
//prevent problematics call by user

class Led_strip{
    private:
        int _ledstrip_pin;
    public:
        Led_strip();
/*------------------LED STRIP-----------------------------*/
/**
  @brief : Inisialisasi pin led strip()
  @param : mendeklarasikan pin 5 sebagai pin OUTPUT 
  @note  : saat diberi high transistor akan bekerja sebagai switch
**/
    void init(int pin);

/**
  @brief : seting kondisi on/off switch led strip
  @param : instruksi menyala dan mematikan led strip
  @note  : 
**/
    void on();
};
extern Led_strip led_strip;

#endif