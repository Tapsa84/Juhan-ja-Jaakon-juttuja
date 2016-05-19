#include <DcMotor.h>

const int LED = 13;

class PumpController {
  private:
    DcMotor *pump;
    int lastPass = 0;
    const int sec10 = 1000UL * 10;

  public: 
    PumpController(DcMotor *pump) {
      this->pump = pump;
    }

    bool oncePer10Sec() {
      if (millis() > lastPass + sec10) {
        this->lastPass = millis();
        return true;
      }
      return false;
    }

    // non-blocking
    void tick() {
      if (this->oncePer10Sec()) {
        Serial.println("once per 10 sec...");
        this->pump->toggle();
      } 
    }
};

// motor1
const int IN1 = 5;
const int IN2 = 4;
// motor2
const int IN3 = 3;
const int IN4 = 2;

PumpController *controller;

void setup() {
  Serial.begin(9600);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  
  pinMode(LED, OUTPUT);

  DcMotor *m = new DcMotor(IN1, IN2);
  controller = new PumpController(m);
}

void loop() {
  controller->tick();
}
