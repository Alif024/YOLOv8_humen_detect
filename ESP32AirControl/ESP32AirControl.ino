#include <TridentTD_LineNotify.h>
#define detectPin 13
#define schedulePin 12
#define finishSchedulePin 14
#define personCheck 22
#define fanControl 23
#define airControl 21
#define ventilationFan 19
#define SSID "COC123"                                             //ชื่อ Wifi
#define PASSWORD "ooooo123"                                       //รหัส Wifi
#define LINE_TOKEN "ATPPSfvi2OV0oJ54BKAuSneb3vb8BcFzPjrma6mK43L"  //Line Token

bool state1 = true;
bool state2 = false;
bool state3 = false;
bool state4 = false;
bool state5 = false;

unsigned long timeScheduleMillis;
bool timeScheduleCount = true;
unsigned long timeDetectMillis;
bool timeDetectCount = true;

void setup() {
  WiFi.begin(SSID, PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(400);
  }
  LINE.setToken(LINE_TOKEN);
  char buf[16];
  sprintf(buf, "%d.%d.%d.%d (IP for Webserver coming soon...)", WiFi.localIP()[0], WiFi.localIP()[1], WiFi.localIP()[2], WiFi.localIP()[3]);
  LINE.notify(buf);
  pinMode(detectPin, INPUT_PULLUP);
  pinMode(schedulePin, INPUT_PULLUP);
  pinMode(finishSchedulePin, INPUT_PULLUP);
  pinMode(personCheck, OUTPUT);
  pinMode(fanControl, OUTPUT);
  pinMode(airControl, OUTPUT);
  pinMode(ventilationFan, OUTPUT);
}

void State1() {
  uint8_t schedule = digitalRead(schedulePin);
  if (schedule == 0) {
    state1 = false;
    state2 = true;
    timeScheduleCount = true;
    LINE.notify("ห้องว่างแล้ว");
  }
}

void State2() {
  static unsigned long detectDurations;
  static unsigned long detectLongMillis;
  static uint8_t detectStatePrevious = LOW;
  static unsigned long detectMillis;
  if (timeScheduleCount) {
    timeScheduleMillis = millis();
    timeScheduleCount = false;
  }
  if (!timeScheduleCount && millis() - timeScheduleMillis > 7200000) {
    timeScheduleCount = true;
    digitalWrite(fanControl, LOW);
    digitalWrite(ventilationFan, LOW);
    state1 = true;
    state2 = false;
  }
  detectMillis = millis();
  uint8_t detectState = digitalRead(detectPin);
  if (detectState == 0) {
    digitalWrite(fanControl, HIGH);
    digitalWrite(ventilationFan, HIGH);
    state2 = false;
    state3 = true;
    timeDetectCount = true;
    LINE.notify("คนเข้ามาใช้ห้องเรียนแล้ว");
  }

  if (detectState == 1 && detectStatePrevious == LOW) {
    detectLongMillis = detectMillis;
    detectStatePrevious = HIGH;
  }
  detectDurations = detectMillis - detectLongMillis;
  if (detectState == 1 && detectDurations >= 60000) {
    digitalWrite(fanControl, LOW);
    digitalWrite(ventilationFan, LOW);
  }
  if (detectState == 0 && detectStatePrevious == HIGH) {
    detectStatePrevious = LOW;
  }
}

void State3() {
  static unsigned long detectDurations;
  static unsigned long detectLongMillis;
  static uint8_t detectStatePrevious = LOW;
  static unsigned long detectMillis;
  if (timeDetectCount) {
    timeDetectMillis = millis();
    timeDetectCount = false;
  }
  if (!timeDetectCount && millis() - timeDetectMillis > 300000) {
    digitalWrite(ventilationFan, LOW);
    digitalWrite(airControl, HIGH);
    delay(300000);
    state3 = false;
    state4 = true;
  }

  detectMillis = millis();
  uint8_t detectState = digitalRead(detectPin);
  if (detectState == 1 && detectStatePrevious == LOW) {
    detectLongMillis = detectMillis;
    detectStatePrevious = HIGH;
  }
  detectDurations = detectMillis - detectLongMillis;
  if (detectState == 1 && detectDurations >= 60000) {
    digitalWrite(fanControl, LOW);
    digitalWrite(ventilationFan, LOW);
    state2 = true;
    state3 = false;
    timeDetectCount = true;
  }
  if (detectState == 0 && detectStatePrevious == HIGH) {
    detectStatePrevious = LOW;
  }
}

void State4() {
  static unsigned long detectDurations;
  static unsigned long detectLongMillis;
  static uint8_t detectStatePrevious = LOW;
  static unsigned long detectMillis;
  detectMillis = millis();
  uint8_t detectState = digitalRead(detectPin);
  if (detectState == 1 && detectStatePrevious == LOW) {
    detectLongMillis = detectMillis;
    detectStatePrevious = HIGH;
  }
  detectDurations = detectMillis - detectLongMillis;
  if (detectState == 1 && detectDurations >= 60000) {
    digitalWrite(fanControl, LOW);
    digitalWrite(airControl, LOW);
    state1 = true;
    state4 = false;
    LINE.notify("ห้องว่างแล้ว");
  }
  if (detectState == 0 && detectStatePrevious == HIGH) {
    detectStatePrevious = LOW;
  }
  uint8_t finishSchedule = digitalRead(finishSchedulePin);
  if (finishSchedule == 0) {
    digitalWrite(airControl, LOW);
    digitalWrite(ventilationFan, HIGH);
    state4 = false;
    state5 = true;
  }
}

void State5() {
  static unsigned long detectDurations;
  static unsigned long detectLongMillis;
  static uint8_t detectStatePrevious = LOW;
  static unsigned long detectMillis;
  detectMillis = millis();
  uint8_t detectState = digitalRead(detectPin);
  if (detectState == 1 && detectStatePrevious == LOW) {
    detectLongMillis = detectMillis;
    detectStatePrevious = HIGH;
  }
  detectDurations = detectMillis - detectLongMillis;
  if (detectState == 1 && detectDurations >= 60000) {
    digitalWrite(fanControl, LOW);
    digitalWrite(ventilationFan, LOW);
    state1 = true;
    state5 = false;
    LINE.notify("ห้องว่างแล้ว");
  }
  if (detectState == 0 && detectStatePrevious == HIGH) {
    detectStatePrevious = LOW;
  }

  uint8_t schedule = digitalRead(schedulePin);
  if (schedule == 0) {
    state2 = true;
    state5 = false;
    timeScheduleCount = true;
    LINE.notify("คนมาใช้ห้องต่อจากคาบที่แล้ว");
  }
}

void loop() {
  while (state1) {
    State1();
  }
  while (state2) {
    State2();
  }
  while (state3) {
    State3();
  }
  while (state4) {
    State4();
  }
  while (state5) {
    State5();
  }
}
