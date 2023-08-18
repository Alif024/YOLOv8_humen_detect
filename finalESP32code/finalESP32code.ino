#include <Arduino.h>
#include <WiFi.h>
#include <FirebaseESP32.h>

// Replace with your network credentials
const char* ssid = "COC123";
const char* password = "ooooo123";

// Replace with your Firebase project credentials
#define FIREBASE_HOST "https://esp32-website-default-rtdb.asia-southeast1.firebasedatabase.app"
#define FIREBASE_API_KEY "AIzaSyAZw4zsGLxItXGbpNqqf44teLAUv1EZE48"

// Replace with the path to your pin status data on Firebase
#define FIREBASE_PIN_STATUS_PATH "/pin_status"

TaskHandle_t Task1;  // Task handle for Core 0
TaskHandle_t Task2;  // Task handle for Core 1

const int schedulePin = 25;
const int peoplePin = 13;
const int airConditionerPin = 12;
const int electricWallFanPin = 33;
const int ventilationFanPin = 27;

bool state1 = true;
bool state2 = false;
bool state3 = false;
bool state4 = false;
bool state5 = false;

bool scheduleState = false;

void core1Task(void* pvParameters) {
  while (true) {
    if (WiFi.status() != WL_CONNECTED) {
      delay(1000);
      Serial.println("Connecting to WiFi...");
    } else {
      // Read pin status
      int status_airConditioner = digitalRead(airConditionerPin);
      int status_electricWallFan = digitalRead(electricWallFanPin);
      int status_ventilationFan = digitalRead(ventilationFanPin);
      int status_persons = digitalRead(peoplePin);

      // Update pin status data to Firebase
      FirebaseData data;
      if (Firebase.set(data, FIREBASE_PIN_STATUS_PATH "/pin1", status_airConditioner == HIGH)) {
        Serial.println("Update successful!");
      } else {
        Serial.println("Update failed.");
        Serial.println(data.errorReason());
      }

      if (Firebase.setBool(data, FIREBASE_PIN_STATUS_PATH "/pin2", status_electricWallFan == HIGH)) {
        Serial.println("Update successful!");
      } else {
        Serial.println("Update failed.");
        Serial.println(data.errorReason());
      }

      if (Firebase.setBool(data, FIREBASE_PIN_STATUS_PATH "/pin3", status_ventilationFan == HIGH)) {
        Serial.println("Update successful!");
      } else {
        Serial.println("Update failed.");
        Serial.println(data.errorReason());
      }

      if (Firebase.setBool(data, FIREBASE_PIN_STATUS_PATH "/pin4", status_persons == HIGH)) {
        Serial.println("Update successful!");
      } else {
        Serial.println("Update failed.");
        Serial.println(data.errorReason());
      }
    }
  }
}

void setup() {
  Serial.begin(115200);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  // Initialize Firebase
  Firebase.begin(FIREBASE_HOST, FIREBASE_API_KEY);

  // Create the task for Core 1
  xTaskCreatePinnedToCore(core1Task, "Core1Task", 10000, NULL, 1, &Task2, 1);

  pinMode(peoplePin, INPUT_PULLUP);
  pinMode(schedulePin, INPUT_PULLUP);
  pinMode(electricWallFanPin, OUTPUT);
  pinMode(ventilationFanPin, OUTPUT);
  pinMode(airConditionerPin, OUTPUT);
}

void taskState1() {
  static int peopleStatusPrevious = LOW;
  static unsigned long detectMillis;
  static unsigned long detectDurations;
  static unsigned long longDetectMillis;

  int schedule = digitalRead(schedulePin);
  if (schedule == 0 && !scheduleState && state1) {
    scheduleState = true;
    state1 = false;
    state2 = true;
    peopleStatusPrevious = LOW;
  } else if (schedule == 1 && scheduleState && state1) {
    scheduleState = false;
  }

  int people = digitalRead(peoplePin);
  if (people == 1 && state1) {
    /* when the person cannot be found */
    while (true) {

      int schedule = digitalRead(schedulePin);
      if (schedule == 0 && !scheduleState) {
        scheduleState = true;
        state1 = false;
        state2 = true;
        peopleStatusPrevious = LOW;
        break;
      }

      detectMillis = millis();
      if (peopleStatusPrevious == LOW) {

        longDetectMillis = detectMillis;
        peopleStatusPrevious = HIGH;
      }
      detectDurations = detectMillis - longDetectMillis;
      if (people == 1 && peopleStatusPrevious == HIGH && detectDurations >= 10000) {
        digitalWrite(ventilationFanPin, LOW);
        digitalWrite(electricWallFanPin, LOW);

        peopleStatusPrevious = LOW;
        break;
      }

      int people = digitalRead(peoplePin);
      if (people == 0) {
        peopleStatusPrevious = LOW;
        break;
      }
    }
  } else if (people == 0 && state1) {
    /* when detecting a person */
    while (true) {

      int schedule = digitalRead(schedulePin);
      if (schedule == 0 && !scheduleState) {
        scheduleState = true;
        state1 = false;
        state2 = true;
        peopleStatusPrevious = LOW;
        break;
      }

      detectMillis = millis();
      if (peopleStatusPrevious == LOW) {
        longDetectMillis = detectMillis;
        peopleStatusPrevious = HIGH;
      }
      detectDurations = detectMillis - longDetectMillis;
      if (people == 0 && peopleStatusPrevious == HIGH && detectDurations >= 15000) {
        digitalWrite(ventilationFanPin, HIGH);
        digitalWrite(electricWallFanPin, HIGH);

        peopleStatusPrevious = LOW;
        break;
      }

      int people = digitalRead(peoplePin);
      if (people == 1) {
        peopleStatusPrevious = LOW;
        break;
      }
    }
  }
}

void taskState2() {
  static int peopleStatusPrevious = LOW;
  static unsigned long detectMillis;
  static unsigned long detectDurations;
  static unsigned long longDetectMillis;

  int schedule = digitalRead(schedulePin);
  if (schedule == 1 && scheduleState && state2) {
    state1 = true;
    state2 = false;
    peopleStatusPrevious = LOW;
  } else {
    int people = digitalRead(peoplePin);
    if (people == 1 && state2) {
      /* when the person cannot be found */
      while (true) {
        int schedule = digitalRead(schedulePin);
        if (schedule == 1) {
          break;
        }
        detectMillis = millis();
        if (peopleStatusPrevious == LOW) {
          longDetectMillis = detectMillis;
          peopleStatusPrevious = HIGH;
        }
        detectDurations = detectMillis - longDetectMillis;
        if (people == 1 && detectDurations >= 10000) {
          digitalWrite(ventilationFanPin, LOW);
          digitalWrite(electricWallFanPin, LOW);

          peopleStatusPrevious = LOW;
          break;
        }
        int people = digitalRead(peoplePin);
        if (people == 0) {
          peopleStatusPrevious = LOW;
          break;
        }
      }
    } else if (people == 0 && state2) {
      /* when detecting a person */
      while (true) {
        int schedule = digitalRead(schedulePin);
        if (schedule == 1) {
          break;
        }
        detectMillis = millis();
        if (peopleStatusPrevious == LOW) {
          longDetectMillis = detectMillis;
          peopleStatusPrevious = HIGH;
        }
        detectDurations = detectMillis - longDetectMillis;
        if (people == 0 && detectDurations >= 15000) {
          digitalWrite(ventilationFanPin, HIGH);
          digitalWrite(electricWallFanPin, HIGH);
          state2 = false;
          state3 = true;
          peopleStatusPrevious = LOW;
          break;
        }

        int people = digitalRead(peoplePin);
        if (people == 1) {
          peopleStatusPrevious = LOW;
          break;
        }
      }
    }
  }
}

void taskState3() {
  static int peopleStatusPrevious = LOW;
  static unsigned long detectMillis;
  static unsigned long detectDurations;
  static unsigned long longDetectMillis;
  static unsigned long delay;
  int schedule = digitalRead(schedulePin);
  if (schedule == 1 && scheduleState && state3) {
    state1 = true;
    state3 = false;
    peopleStatusPrevious = LOW;
  } else {
    int people = digitalRead(peoplePin);
    if (people == 1 && state3) {
      /* when the person cannot be found */
      while (true) {
        int schedule = digitalRead(schedulePin);
        if (schedule == 1) {
          break;
        }
        detectMillis = millis();
        if (peopleStatusPrevious == LOW) {
          longDetectMillis = detectMillis;
          peopleStatusPrevious = HIGH;
        }
        detectDurations = detectMillis - longDetectMillis;
        if (people == 1 && detectDurations >= 10000) {
          state2 = true;
          state3 = false;
          digitalWrite(electricWallFanPin, LOW);
          digitalWrite(ventilationFanPin, LOW);
          peopleStatusPrevious = LOW;
          break;
        }
        int people = digitalRead(peoplePin);
        if (people == 0) {
          peopleStatusPrevious = LOW;
          break;
        }
      }
    } else if (people == 0 && state3) {
      /* when detecting a person */
      while (true) {
        int schedule = digitalRead(schedulePin);
        if (schedule == 1) {
          break;
        }
        detectMillis = millis();
        if (peopleStatusPrevious == LOW) {
          longDetectMillis = detectMillis;
          peopleStatusPrevious = HIGH;
        }
        detectDurations = detectMillis - longDetectMillis;
        if (people == 0 && detectDurations >= 180000) {
          int ventilationFan = digitalRead(ventilationFanPin);
          int electricWallFan = digitalRead(electricWallFanPin);
          int airConditioner = digitalRead(airConditionerPin);
          digitalWrite(ventilationFanPin, LOW);
          digitalWrite(electricWallFanPin, HIGH);
          digitalWrite(airConditionerPin, HIGH);
          state3 = false;
          state4 = true;

          delay = millis();
          while (true) {
            if (millis() - delay >= 300000) {
              break;
            }
          }
          peopleStatusPrevious = LOW;
          break;
        }
        int people = digitalRead(peoplePin);
        if (people == 1) {
          peopleStatusPrevious = LOW;
          break;
        }
      }
    }
  }
}

void taskState4() {
  static int peopleStatusPrevious = LOW;
  static unsigned long detectMillis;
  static unsigned long detectDurations;
  static unsigned long longDetectMillis;
  int schedule = digitalRead(schedulePin);
  if (schedule == 1 && scheduleState && state4) {
    scheduleState = false;
  }

  /* when the person cannot be found */
  int people = digitalRead(peoplePin);
  detectMillis = millis();
  if (people == 1 && peopleStatusPrevious == LOW) {
    longDetectMillis = detectMillis;
    peopleStatusPrevious = HIGH;
  }
  detectDurations = detectMillis - longDetectMillis;
  if (people == 1 && detectDurations >= 10000) {
    state1 = true;
    state4 = false;
    digitalWrite(ventilationFanPin, LOW);
    digitalWrite(airConditionerPin, LOW);
    digitalWrite(electricWallFanPin, LOW);
    peopleStatusPrevious = LOW;
  }
  if (people == 0) {
    peopleStatusPrevious = LOW;
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
}
