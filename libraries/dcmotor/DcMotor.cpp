#include "Arduino.h"
#include "DcMotor.h"

DcMotor::DcMotor(int pin1, int pin2) {
  this->pin1 = pin1;
  this->pin2 = pin2;
  analogWrite(pin2, 0);
}

void DcMotor::on() {
  this->running = true;
  analogWrite(pin1, 255);
}

void DcMotor::off() {
  this->running = false;
  analogWrite(pin1, 0); 
}

bool DcMotor::isOn() {
  Serial.print("is on: ");
  Serial.println(this->running);
  return this->running;
}

void DcMotor::toggle() {
  if (this->isOn()) {
    this->off();
  } else {
    this->on();
  }
}
