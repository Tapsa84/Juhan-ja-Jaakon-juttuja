#include <ArduinoJson.h>
#include <DueFlashStorage.h>

int lastPass = 0;
int waitTime = 1000UL;

String* inputString = new String();
boolean stringComplete = false;

String defaultSettings = "{\"waitTime\":10000, \"vtyt\":121}";
JsonObject* settings;

DueFlashStorage dueFlashStorage;

/**
 * example json string: {"waitTime":10000}
 */
JsonObject& parseJson(String &jsonString) {
  char json[200];
  jsonString.toCharArray(json, 200);
  StaticJsonBuffer<200> jsonBuffer;
  return jsonBuffer.parseObject(json);
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
    *inputString += inChar;
    if (inChar == '\n') {
      stringComplete = true;
    }
  }
}

bool isCodeRunningForTheFirstTime() {
  uint8_t codeRunningForTheFirstTime = dueFlashStorage.read(0);
  return codeRunningForTheFirstTime == 255;
}

void writeSettingsToFlash(String &settings) {
  Serial.println("write settings to Flash.");
  int len = settings.length() + 1;
  byte jsonBytes[len];
  settings.getBytes(jsonBytes, len);
  //String *s = &settings;
  //memcpy(jsonBytes, s, sizeof(s));
  Serial.print("settings: ");
  Serial.print(settings);
  Serial.print(", => json: ");
  Serial.println((char *)jsonBytes);
  
  dueFlashStorage.write(4, jsonBytes, len);
  Serial.println("wrote settings to Flash.");
}

void readSettingsFromFlash() {
  Serial.println("read settings from Flash.");
  Serial.print("flash 1: ");
  int a = 4;
  uint8_t empty = 255;
  do {
    char json = dueFlashStorage.read(a);
    Serial.print(json);
    a++;
    empty = dueFlashStorage.read(a);
  } while (empty != 255);
  // todo:
  //return ;
}

void handleSettings(String &jsonString) {
  settings = &parseJson(jsonString);
  waitTime = (*settings)["waitTime"];
  
  Serial.print(" - wait time set to: ");
  Serial.println(waitTime);

  // todo: write
  writeSettingsToFlash(jsonString);
}

void setup() {
  Serial.begin(9600);
  // reserve 200 bytes for the inputString:
  inputString->reserve(200);
  if (isCodeRunningForTheFirstTime()) {
    Serial.println("code running for first time");
    handleSettings(defaultSettings);
    dueFlashStorage.write(0, 0);
  }
  handleSettings(defaultSettings);

  readSettingsFromFlash();
  
}

void loop() {
  if (stringComplete) {
    handleSettings(*inputString);

    delete inputString;
    inputString = new String();
    stringComplete = false;
  }
  if (oncePerWaitTime()) {
    Serial.print("once per wait time. ");
    Serial.print("flash 0 value:");
    Serial.println(dueFlashStorage.read(0));
  }
}

