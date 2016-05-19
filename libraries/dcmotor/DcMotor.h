#ifndef DcMotor_h
#define DcMotor_h

class DcMotor {
  private:
    int pin1;
    int pin2;
    bool running;
    
  public:
    DcMotor(int pin1, int pin2);
    void on();
    void off();
    bool isOn();
    void toggle();
};
#endif