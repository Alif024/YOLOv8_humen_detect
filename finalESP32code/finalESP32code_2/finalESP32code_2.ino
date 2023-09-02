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
int peopleStatusPrevious = LOW;
unsigned long detectMillis;

TaskHandle_t Task1;

void setup() {
  pinMode(peoplePin, INPUT_PULLUP);
  pinMode(schedulePin, INPUT_PULLUP);
  pinMode(electricWallFanPin, OUTPUT);
  pinMode(ventilationFanPin, OUTPUT);
  pinMode(airConditionerPin, OUTPUT);

  xTaskCreatePinnedToCore(
    Task1code, /* Function to implement the task */
    "Task1",   /* Name of the task */
    10000,     /* Stack size in words */
    NULL,      /* Task input parameter */
    1,         /* Priority of the task */
    &Task1,    /* Task handle. */
    0);        /* Core where the task should run */
  delay(500);
}

void Task1code(void* parameter) {
  static int status_airConditioner;
  static int status_electricWallFan;
  static int status_ventilationFan;
  static int status_persons;
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  // Initialize Firebase
  Firebase.begin(FIREBASE_HOST, FIREBASE_API_KEY);
  for (;;) {
    if (WiFi.status() != WL_CONNECTED) {
      delay(5000);
    } else if (WiFi.status() == WL_CONNECTED) {
      // Read pin status
      status_airConditioner = digitalRead(airConditionerPin);
      status_electricWallFan = digitalRead(electricWallFanPin);
      status_ventilationFan = digitalRead(ventilationFanPin);
      status_persons = digitalRead(peoplePin);

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
  static unsigned long detectDurations;
  static unsigned long longDetectMillis;

  /* ตรวจคน */
  int people = digitalRead(peoplePin);
  if (people == 1 && state1) {
    /* เมื่อไม่เจอคน */
    while (true) {
      /* ตรวจคาบ */
      int schedule = digitalRead(schedulePin);
      if (schedule == 0 && !scheduleState) {
        scheduleState = true;
        state1 = false;
        state2 = true;
        peopleStatusPrevious = LOW;
        break;
      } else if (schedule == 1 && scheduleState) {
        scheduleState = false;
      }

      /* คนไม่อยู่นานตามเวลาไหม */
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
      /* ตรวจคน */
      int people = digitalRead(peoplePin);
      if (people == 0) {
        peopleStatusPrevious = LOW;
        break;
      }
    }
  } else if (people == 0 && state1) {
    /* เมื่อเจอคน */
    while (true) {
      /* ตรวจคาบ */
      int schedule = digitalRead(schedulePin);
      if (schedule == 0 && !scheduleState) {
        scheduleState = true;
        state1 = false;
        state2 = true;
        peopleStatusPrevious = LOW;
        break;
      } else if (schedule == 1 && scheduleState) {
        scheduleState = false;
      }

      /* คนอยู่นานตามเวลาไหม */
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
      /* ตรวจคน */
      int people = digitalRead(peoplePin);
      if (people == 1) {
        peopleStatusPrevious = LOW;
        break;
      }
    }
  }
}

void taskState2() {
  static unsigned long detectDurations;
  static unsigned long longDetectMillis;

  /* ตรวจคน */
  int people = digitalRead(peoplePin);
  if (people == 1 && state2) {
    /* เมื่อไม่เจอคน */
    while (true) {
      /* ตรวจคาบ */
      int schedule = digitalRead(schedulePin);
      if (schedule == 1 && scheduleState) {
        scheduleState = false;
        state1 = true;
        state2 = false;
        peopleStatusPrevious = LOW;
        break;
      }

      /* คนไม่อยู่นานตามเวลาไหม */
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
      /* ตรวจคน */
      int people = digitalRead(peoplePin);
      if (people == 0) {
        peopleStatusPrevious = LOW;
        break;
      }
    }
  } else if (people == 0 && state2) {
    /* เมื่อเจอคน */
    while (true) {
      /* ตรวจคาบ */
      int schedule = digitalRead(schedulePin);
      if (schedule == 1 && scheduleState) {
        scheduleState = false;
        state1 = true;
        state2 = false;
        peopleStatusPrevious = LOW;
        break;
      }

      /* คนอยู่นานตามเวลาไหม */
      detectMillis = millis();
      if (peopleStatusPrevious == LOW) {
        longDetectMillis = detectMillis;
        peopleStatusPrevious = HIGH;
      }
      detectDurations = detectMillis - longDetectMillis;
      if (people == 0 && peopleStatusPrevious == HIGH && detectDurations >= 5000) {
        digitalWrite(ventilationFanPin, HIGH);
        digitalWrite(electricWallFanPin, HIGH);
        state2 = false;
        state3 = true;
        peopleStatusPrevious = LOW;
        break;
      }
      /* ตรวจคน */
      int people = digitalRead(peoplePin);
      if (people == 1) {
        peopleStatusPrevious = LOW;
        break;
      }
    }
  }
}

void taskState3() {
  static unsigned long delay;
  static unsigned long detectDurations;
  static unsigned long longDetectMillis;

  /* ตรวจคน */
  int people = digitalRead(peoplePin);
  if (people == 1 && state2) {
    /* เมื่อไม่เจอคน */
    while (true) {
      /* ตรวจคาบ */
      int schedule = digitalRead(schedulePin);
      if (schedule == 1 && scheduleState) {
        scheduleState = false;
        state1 = true;
        state3 = false;
        peopleStatusPrevious = LOW;
        break;
      }

      /* คนไม่อยู่นานตามเวลาไหม */
      detectMillis = millis();
      if (peopleStatusPrevious == LOW) {
        longDetectMillis = detectMillis;
        peopleStatusPrevious = HIGH;
      }
      detectDurations = detectMillis - longDetectMillis;
      if (people == 1 && peopleStatusPrevious == HIGH && detectDurations >= 20000) {
        state2 = true;
        state3 = false;
        digitalWrite(electricWallFanPin, LOW);
        digitalWrite(ventilationFanPin, LOW);
        peopleStatusPrevious = LOW;
        break;
      }
      /* ตรวจคน */
      int people = digitalRead(peoplePin);
      if (people == 0) {
        peopleStatusPrevious = LOW;
        break;
      }
    }
  } else if (people == 0 && state2) {
    /* เมื่อเจอคน */
    while (true) {
      /* ตรวจคาบ */
      int schedule = digitalRead(schedulePin);
      if (schedule == 1 && scheduleState) {
        scheduleState = false;
        state1 = true;
        state3 = false;
        peopleStatusPrevious = LOW;
        break;
      }

      /* คนอยู่นานตามเวลาไหม */
      detectMillis = millis();
      if (peopleStatusPrevious == LOW) {
        longDetectMillis = detectMillis;
        peopleStatusPrevious = HIGH;
      }
      detectDurations = detectMillis - longDetectMillis;
      if (people == 0 && peopleStatusPrevious == HIGH && detectDurations >= 10000) {
        digitalWrite(ventilationFanPin, LOW);
        digitalWrite(electricWallFanPin, HIGH);
        digitalWrite(airConditionerPin, HIGH);
        state3 = false;
        state4 = true;
        /* หน่วงเวลา */
        delay = millis();
        while (true) {
          if (millis() - delay >= 10000) {
            break;
          }
        }
        peopleStatusPrevious = LOW;
        break;
      }
      /* ตรวจคน */
      int people = digitalRead(peoplePin);
      if (people == 1) {
        peopleStatusPrevious = LOW;
        break;
      }
    }
  }
}

void taskState4() {
  static unsigned long detectDurations;
  static unsigned long longDetectMillis;

  /* ตรวจคาบ */
  int schedule = digitalRead(schedulePin);
  if (schedule == 1 && scheduleState) {
    scheduleState = false;
  }

  /* ตรวจคน */
  int people = digitalRead(peoplePin);

  /* คนไม่อยู่นานตามเวลาไหม */
  detectMillis = millis();
  if (peopleStatusPrevious == LOW) {
    longDetectMillis = detectMillis;
    peopleStatusPrevious = HIGH;
  }
  detectDurations = detectMillis - longDetectMillis;
  if (people == 1 && peopleStatusPrevious == HIGH && detectDurations >= 20000) {
    state1 = true;
    state4 = false;
    digitalWrite(ventilationFanPin, LOW);
    digitalWrite(airConditionerPin, LOW);
    digitalWrite(electricWallFanPin, LOW);
    peopleStatusPrevious = LOW;
  }
  if (people == 0 && state4) {
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