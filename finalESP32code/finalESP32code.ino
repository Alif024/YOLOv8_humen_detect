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

#define schedulePin 25
#define peoplePin 13
#define airConditionerPin 32
#define electricWallFanPin 33
#define ventilationFanPin 27

bool state1 = true;
bool state2 = false;
bool state3 = false;
bool state4 = false;

bool scheduleState = false;

TaskHandle_t Task1;

void setup() {
  pinMode(peoplePin, INPUT_PULLUP);
  pinMode(schedulePin, INPUT_PULLUP);
  pinMode(electricWallFanPin, OUTPUT);
  pinMode(ventilationFanPin, OUTPUT);
  pinMode(airConditionerPin, OUTPUT);

  xTaskCreatePinnedToCore(
    Task1, /* Function to implement the task */
    "Task1",   /* Name of the task */
    10000,     /* Stack size in words */
    NULL,      /* Task input parameter */
    1,         /* Priority of the task */
    &Task1,    /* Task handle. */
    0);        /* Core where the task should run */
  delay(500);
}

void Task1code(void* parameter) {
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  // Initialize Firebase
  Firebase.begin(FIREBASE_HOST, FIREBASE_API_KEY);
  for (;;) {
    if (WiFi.status() != WL_CONNECTED) {
      delay(1000);
    } else if (WiFi.status() == WL_CONNECTED) {
      // Read pin status
      int status_airConditioner = digitalRead(airConditionerPin);
      int status_electricWallFan = digitalRead(electricWallFanPin);
      int status_ventilationFan = digitalRead(ventilationFanPin);
      int status_persons = digitalRead(peoplePin);

      // Update pin status data to Firebase
      FirebaseData data;
      Firebase.setBool(data, FIREBASE_PIN_STATUS_PATH "/pin1", status_airConditioner == HIGH);
      Firebase.setBool(data, FIREBASE_PIN_STATUS_PATH "/pin2", status_electricWallFan == HIGH);
      Firebase.setBool(data, FIREBASE_PIN_STATUS_PATH "/pin3", status_ventilationFan == HIGH);
      Firebase.setBool(data, FIREBASE_PIN_STATUS_PATH "/pin4", status_persons == HIGH);
    }
  }
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
      if (people == 0 && peopleStatusPrevious == HIGH && detectDurations >= 5000) {
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
    scheduleState = false;
    state1 = true;
    state2 = false;
    peopleStatusPrevious = LOW;
  } else {
    int people = digitalRead(peoplePin);
    if (people == 1 && state2) {
      /* when the person cannot be found */
      while (true) {
        int schedule = digitalRead(schedulePin);
        if (schedule == 1 && scheduleState) {
          scheduleState = false;
          state1 = true;
          state2 = false;
          peopleStatusPrevious = LOW;
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
        if (schedule == 1 && scheduleState) {
          scheduleState = false;
          state1 = true;
          state2 = false;
          peopleStatusPrevious = LOW;
          break;
        }
        detectMillis = millis();
        if (peopleStatusPrevious == LOW) {
          longDetectMillis = detectMillis;
          peopleStatusPrevious = HIGH;
        }
        detectDurations = detectMillis - longDetectMillis;
        if (people == 0 && detectDurations >= 5000) {
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
  int schedule = digitalRead(schedulePin);
  if (schedule == 1 && scheduleState && state3) {
    scheduleState = false;
    state1 = true;
    state3 = false;
    peopleStatusPrevious = LOW;
  } else {
    int people = digitalRead(peoplePin);
    if (people == 1 && state3) {
      /* when the person cannot be found */
      while (true) {
        int schedule = digitalRead(schedulePin);
        if (schedule == 1 && scheduleState) {
          scheduleState = false;
          state1 = true;
          state3 = false;
          peopleStatusPrevious = LOW;
          break;
        }
        detectMillis = millis();
        if (peopleStatusPrevious == LOW) {
          longDetectMillis = detectMillis;
          peopleStatusPrevious = HIGH;
        }
        detectDurations = detectMillis - longDetectMillis;
        if (people == 1 && detectDurations >= 20000) {
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
          state1 = true;
          state3 = false;
          peopleStatusPrevious = LOW;
          break;
        }
        detectMillis = millis();
        if (peopleStatusPrevious == LOW) {
          longDetectMillis = detectMillis;
          peopleStatusPrevious = HIGH;
        }
        detectDurations = detectMillis - longDetectMillis;
        if (people == 0 && detectDurations >= 5000) {
          digitalWrite(ventilationFanPin, LOW);
          digitalWrite(electricWallFanPin, HIGH);
          digitalWrite(airConditionerPin, HIGH);

          state3 = false;
          state4 = true;
          static unsigned long delay;
          delay = millis();
          while (true) {
            if (millis() - delay >= 10000) {
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