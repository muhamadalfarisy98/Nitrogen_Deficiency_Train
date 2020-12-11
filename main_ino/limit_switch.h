#ifndef LIMIT_SWITCH_H
#define LIMIT_SWITCH_H



class Limit_switch{
    private:
      int _limitswitch_pin1;
      int _limitswitch_pin2;
    public:
        Limit_switch();
/**
  @brief : membuat program initLimitSwitch()
  @param : inisialisasi pin yang digunakan sebagai input PULLUP
  @note  : menggunakan resistansi internal mikrokontroler
       sehingga pembacaan berupa active-low
**/
        void init(int pin1,int pin2);
/**
  @brief : membuat program readLS1()
  @param : membaca keluaran pin digital kondisi LS1
  @note  : nilai keluaran berupa high low (1-0)
**/
        uint32_t readLS1();
/**
  @brief : membuat program readLS2()
  @param : membaca keluaran pin digital kondisi LS2
  @note  : nilai keluaran berupa high low (1-0)
**/
        uint32_t readLS2();
/**
  @brief : membuat program cekLS()
  @param : membaca keluaran pin digital kondisi LS1
  @note  : nilai keluaran berupa high low (1-0)
**/
    void cek_LS();

    void endstop_LS();
};
extern Limit_switch ls;//deklare an instance


#endif
