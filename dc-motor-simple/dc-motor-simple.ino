const int LED = 13;

class PumpMotor {
  private:
    int pin1;
    int pin2;
    bool running = false;
    
  public:
    PumpMotor(int pin1, int pin2) {
      this->pin1 = pin1;
      this->pin2 = pin2;
      analogWrite(pin2, 0);
    }
    void on() {
      this->running = true;
      analogWrite(pin1, 255);
      digitalWrite(LED, HIGH);
    }
    void off() {
      this->running = false;
      analogWrite(pin1, 0);
      digitalWrite(LED, LOW); 
    }
    bool isOn() {
      Serial.print("is on: ");
      Serial.println(this->running);
      return this->running;
    }
    void toggle() {
      if (this->isOn()) {
        this->off();
      } else {
        this->on();
      }
    }
};

class PumpController {
  private:
    PumpMotor *pump;
    int lastPass = 0;
    const int sec10 = 1000UL * 10;

  public: 
    PumpController(PumpMotor *pump) {
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

  PumpMotor *m = new PumpMotor(IN1, IN2);
  controller = new PumpController(m);
}

void loop() {
  controller->tick();
}
