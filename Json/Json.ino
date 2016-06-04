#include <ArduinoJson.h>

int lastPass = 0;
int waitTime = 1000UL;

String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete

void parseJson() {
  // for debugging, use: char json[] = "{\"sensor\":\"gps\",\"time\":1351824120,\"data\":[48.756080,2.302038]}";
  // actual input string needs to be without double quotes: {"waitTime":10000}
  char json[200];
  inputString.toCharArray(json, 200);
  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& settings = jsonBuffer.parseObject(json);

  // for debugging:
  //const char* sensor    = settings["sensor"];
  //long        time      = settings["time"];
  //double      latitude  = settings["data"][0];
  //double      longitude = settings["data"][1];
  waitTime = settings["waitTime"];
  
  Serial.print(" - wait time set to: ");
  Serial.println(waitTime);
}

int getMillis() {
  // milliseconds timer rolls every 50 days or so.
  // TODO: what should happen when timer rolls ?
  return millis();
}

bool oncePerWaitTime() {
  if (getMillis() > lastPass + waitTime) {
    lastPass = getMillis();
    return true;
  }
  return false;
}

void serialEvent() {
  while (Serial.available()) {
    char inChar = (char)Serial.read();
    inputString += inChar;
    if (inChar == '\n') {
      stringComplete = true;
    }
  }
}

void setup() {
  Serial.begin(9600);
  // reserve 200 bytes for the inputString:
  inputString.reserve(200);
}

void loop() {
  if (stringComplete) {
    parseJson();
    inputString = "";
    stringComplete = false;
  }
  if (oncePerWaitTime()) {
    Serial.println("once per wait time");
  }
}

