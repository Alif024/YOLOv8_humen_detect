const int schedulePin = 25;
const int peoplePin = 13;
const int airConditionerPin = 12;
const int electricWallFanPin = 14;
const int ventilationFanPin = 27;

bool state1 = true;
bool state2 = false;
bool state3 = false;
bool state4 = false;
bool state5 = false;

bool scheduleState = false;

void setup() {
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
            if (millis() - delay >= 60000) {
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
    state1 = true;
    state4 = false;
    digitalWrite(ventilationFanPin, HIGH);
    digitalWrite(airConditionerPin, LOW);

    peopleStatusPrevious = LOW;
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
