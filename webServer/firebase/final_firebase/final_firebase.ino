#include <Arduino.h>
#include <WiFi.h>
#include <FirebaseESP32.h>

// Replace with your network credentials
#define ssid "ONE_2.4GHz"
#define password "91919191"

// Replace with your Firebase project credentials
#define FIREBASE_HOST "https://esp32-website-default-rtdb.asia-southeast1.firebasedatabase.app"
#define FIREBASE_API_KEY "AIzaSyAZw4zsGLxItXGbpNqqf44teLAUv1EZE48"

// Replace with the path to your pin status data on Firebase
#define FIREBASE_PIN_STATUS_PATH "/pin_status"

#define schedulePin 25
#define finishSchedulePin 26
#define personsPin 13
#define airConditionerPin 12
#define electricWallFanPin 33
#define ventilationFanPin 27

TaskHandle_t Task1;  // Task handle for Core 0
TaskHandle_t Task2;  // Task handle for Core 1

/* state_status */
bool state1 = true;
bool state2 = false;
bool state3 = false;
bool state4 = false;
bool state5 = false;

/* taskState2 */
unsigned long classTime;
bool classTimeState = true;

void core1Task(void* pvParameters) {
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
  }

  // Initialize Firebase
  Firebase.begin(FIREBASE_HOST, FIREBASE_API_KEY);
  static unsigned long delay;
  delay = millis();
  while (true) {
    // Read pin status
    int status_airConditioner = digitalRead(airConditionerPin);
    int status_electricWallFan = digitalRead(electricWallFanPin);
    int status_ventilationFan = digitalRead(ventilationFanPin);
    int status_persons = digitalRead(personsPin);

    // Update pin status data to Firebase
    FirebaseData data;
    Firebase.set(data, FIREBASE_PIN_STATUS_PATH "/pin1", status_airConditioner == HIGH);
    Firebase.setBool(data, FIREBASE_PIN_STATUS_PATH "/pin2", status_electricWallFan == HIGH);
    Firebase.setBool(data, FIREBASE_PIN_STATUS_PATH "/pin3", status_ventilationFan == HIGH);
    Firebase.setBool(data, FIREBASE_PIN_STATUS_PATH "/pin4", status_persons == HIGH);
  }
}

void setup() {
  // Create the task for Core 1
  xTaskCreatePinnedToCore(core1Task, "Core1Task", 10000, NULL, 1, &Task2, 1);

  // Initialize the LED pin as an output
  pinMode(personsPin, INPUT_PULLUP);
  pinMode(schedulePin, INPUT_PULLUP);
  pinMode(finishSchedulePin, INPUT_PULLUP);
  pinMode(electricWallFanPin, OUTPUT);
  pinMode(airConditionerPin, OUTPUT);
  pinMode(ventilationFanPin, OUTPUT);
}

void taskState1() {
  int scheduleState = digitalRead(schedulePin);
  if (scheduleState == LOW && state1) {
    state1 = false;
    state2 = true;
    classTimeState = true;
  }
}

void taskState2() {
  static unsigned long detectDurations;
  static unsigned long detectLongMillis;
  static int detectStatePrevious = LOW;
  static unsigned long detectMillis;
  if (classTimeState && state2) {
    classTime = millis();
    classTimeState = false;
  }
  if (!classTimeState && state2) {
    if (millis() - classTime >= 20000) {
      state1 = true;
      state2 = false;
      digitalWrite(ventilationFanPin, LOW);
      digitalWrite(electricWallFanPin, LOW);
    } else {
      while (true) {
        detectMillis = millis();
        int detectState = digitalRead(personsPin);
        if (detectState == HIGH && detectStatePrevious == LOW) {
          detectLongMillis = detectMillis;
          detectStatePrevious = HIGH;
        }
        detectDurations = detectMillis - detectLongMillis;
        if (detectState == HIGH && detectDurations >= 5000) {
          digitalWrite(ventilationFanPin, LOW);
          digitalWrite(electricWallFanPin, LOW);
          detectStatePrevious = LOW;
          break;
        }
        if (detectState == LOW && detectStatePrevious == HIGH) {
          digitalWrite(ventilationFanPin, HIGH);
          digitalWrite(electricWallFanPin, HIGH);
          state2 = false;
          state3 = true;
          detectStatePrevious = LOW;
          break;
        }
        if (detectState == LOW && detectStatePrevious == LOW) {
          digitalWrite(ventilationFanPin, HIGH);
          digitalWrite(electricWallFanPin, HIGH);
          state2 = false;
          state3 = true;
          break;
        }
      }
    }
  }
}

void taskState3() {
  static unsigned long detectDurations;
  static unsigned long detectLongMillis;
  static int detectStatePrevious = LOW;
  static unsigned long detectMillis;
  static unsigned long detectionTime;
  static bool detectionTimeState = true;
  static unsigned long delayTime;
  if (detectionTimeState && state3) {
    detectionTime = millis();
    detectionTimeState = false;
  }
  if (!detectionTimeState && state3) {
    if (millis() - detectionTime >= 10000) {
      digitalWrite(ventilationFanPin, LOW);
      digitalWrite(airConditionerPin, HIGH);
      delayTime = millis();
      while (true) {
        if (millis() - delayTime >= 5000) {
          state3 = false;
          state4 = true;
          detectionTimeState = true;
          break;
        }
      }
    } else {
      while (true) {
        detectMillis = millis();
        int detectState = digitalRead(personsPin);
        if (detectState == HIGH && detectStatePrevious == LOW) {
          detectLongMillis = detectMillis;
          detectStatePrevious = HIGH;
        }
        detectDurations = detectMillis - detectLongMillis;
        if (detectState == HIGH && detectDurations >= 5000) {
          state2 = true;
          state3 = false;
          detectionTimeState = true;
          detectStatePrevious = LOW;
          break;
        }
        if (detectState == LOW && detectStatePrevious == HIGH) {
          detectStatePrevious = LOW;
          break;
        }
        if (detectState == LOW && detectStatePrevious == LOW) {
          break;
        }
      }
    }
  }
}

void taskState4() {
  static unsigned long detectDurations;
  static unsigned long detectLongMillis;
  static int detectStatePrevious = LOW;
  static unsigned long detectMillis;
  detectMillis = millis();
  int detectState = digitalRead(personsPin);
  if (detectState == HIGH && detectStatePrevious == LOW) {
    detectLongMillis = detectMillis;
    detectStatePrevious = HIGH;
  }
  detectDurations = detectMillis - detectLongMillis;
  if (detectState == HIGH && detectDurations >= 5000) {
    digitalWrite(electricWallFanPin, LOW);
    digitalWrite(ventilationFanPin, LOW);
    digitalWrite(airConditionerPin, LOW);
    state1 = true;
    state4 = false;
    detectStatePrevious = LOW;
  }
  if (detectState == LOW && detectStatePrevious == HIGH) {
    detectStatePrevious = LOW;
  }

  int finishScheduleState = digitalRead(finishSchedulePin);
  if (finishScheduleState == LOW && state4) {
    digitalWrite(airConditionerPin, LOW);
    digitalWrite(ventilationFanPin, HIGH);
    state4 = false;
    state5 = true;
    detectStatePrevious = LOW;
  }
}

void taskState5() {
  static unsigned long detectDurations;
  static unsigned long detectLongMillis;
  static int detectStatePrevious = LOW;
  static unsigned long detectMillis;
  detectMillis = millis();
  int detectState = digitalRead(personsPin);
  if (detectState == HIGH && detectStatePrevious == LOW) {
    detectLongMillis = detectMillis;
    detectStatePrevious = HIGH;
  }
  detectDurations = detectMillis - detectLongMillis;
  if (detectState == HIGH && detectDurations >= 5000) {
    digitalWrite(electricWallFanPin, LOW);
    digitalWrite(ventilationFanPin, LOW);
    state1 = true;
    state5 = false;
    detectStatePrevious = LOW;
  }
  if (detectState == LOW && detectStatePrevious == HIGH) {
    detectStatePrevious = LOW;
  }
  int scheduleState = digitalRead(schedulePin);
  if (scheduleState == LOW && state5) {
    state2 = true;
    state5 = false;
    classTimeState = true;
    detectStatePrevious = LOW;
  }
}

void loop() {
  while (state1) {
    taskState1();
  }
  while (state2) {
    taskState2();
  }
  while (state3) {
    taskState3();
  }
  while (state4) {
    taskState4();
  }
  while (state5) {
    taskState5();
  }
}