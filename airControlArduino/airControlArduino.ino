#define peoplePin 0
#define schedulePin 1
#define electricWallFanPin 4
#define ventilationFanPin 6
#define airConditionerPin 5

bool state1 = true;
bool state2 = false;
bool state3 = false;
bool state4 = false;
bool state5 = false;

bool scheduleState = false;

/* arduino uno */
#define peopleLED 3
#define state1LED 8
#define state2LED 9
#define state3LED 10
#define state4LED 11
#define state5LED 12

void setup() {
  pinMode(peoplePin, INPUT_PULLUP);
  pinMode(schedulePin, INPUT_PULLUP);
  pinMode(electricWallFanPin, OUTPUT);
  pinMode(ventilationFanPin, OUTPUT);
  pinMode(airConditionerPin, OUTPUT);

  /* arduino uno */
  pinMode(peopleLED, OUTPUT);
  pinMode(state1LED, OUTPUT);
  pinMode(state2LED, OUTPUT);
  pinMode(state3LED, OUTPUT);
  pinMode(state4LED, OUTPUT);
  digitalWrite(state1LED, HIGH);
  digitalWrite(state2LED, LOW);
  digitalWrite(state3LED, LOW);
  digitalWrite(state4LED, LOW);
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

    /* arduino uno */
    digitalWrite(state1LED, LOW);
    digitalWrite(state2LED, HIGH);

    peopleStatusPrevious = LOW;
  } else if (schedule == 1 && scheduleState && state1) {
    scheduleState = false;
  }

  int people = digitalRead(peoplePin);

  /* arduino uno */
  if (people == 0) {
    digitalWrite(peopleLED, HIGH);
  } else {
    digitalWrite(peopleLED, LOW);
  }

  if (people == 1 && state1) {
    /* when the person cannot be found */
    while (true) {

      int schedule = digitalRead(schedulePin);
      if (schedule == 0 && !scheduleState) {
        scheduleState = true;
        state1 = false;
        state2 = true;

        /* arduino uno */
        digitalWrite(state1LED, LOW);
        digitalWrite(state2LED, HIGH);

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

      /* arduino uno */
      if (people == 0) {
        digitalWrite(peopleLED, HIGH);
      } else {
        digitalWrite(peopleLED, LOW);
      }
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

        /* arduino uno */
        digitalWrite(state1LED, LOW);
        digitalWrite(state2LED, HIGH);

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

      /* arduino uno */
      if (people == 0) {
        digitalWrite(peopleLED, HIGH);
      } else {
        digitalWrite(peopleLED, LOW);
      }
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

    /* arduino uno */
    digitalWrite(state1LED, HIGH);
    digitalWrite(state2LED, LOW);

    peopleStatusPrevious = LOW;
  } else {
    int people = digitalRead(peoplePin);
    /* arduino uno */
    if (people == 0) {
      digitalWrite(peopleLED, HIGH);
    } else {
      digitalWrite(peopleLED, LOW);
    }
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
        /* arduino uno */
        if (people == 0) {
          digitalWrite(peopleLED, HIGH);
        } else {
          digitalWrite(peopleLED, LOW);
        }
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
        if (people == 0 && detectDurations >= 5000) {
          digitalWrite(ventilationFanPin, HIGH);
          digitalWrite(electricWallFanPin, HIGH);
          state2 = false;
          state3 = true;
          /* arduino uno */
          digitalWrite(state2LED, LOW);
          digitalWrite(state3LED, HIGH);
          peopleStatusPrevious = LOW;
          break;
        }
        int people = digitalRead(peoplePin);
        /* arduino uno */
        if (people == 0) {
          digitalWrite(peopleLED, HIGH);
        } else {
          digitalWrite(peopleLED, LOW);
        }
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
    state1 = true;
    state3 = false;
    /* arduino uno */
    digitalWrite(state1LED, HIGH);
    digitalWrite(state3LED, LOW);
    peopleStatusPrevious = LOW;
  } else {
    int people = digitalRead(peoplePin);
    /* arduino uno */
    if (people == 0) {
      digitalWrite(peopleLED, HIGH);
    } else {
      digitalWrite(peopleLED, LOW);
    }
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
        if (people == 1 && detectDurations >= 20000) {
          state2 = true;
          state3 = false;
          digitalWrite(electricWallFanPin, LOW);
          digitalWrite(ventilationFanPin, LOW);
          /* arduino uno */
          digitalWrite(state2LED, HIGH);
          digitalWrite(state3LED, LOW);
          peopleStatusPrevious = LOW;
          break;
        }
        int people = digitalRead(peoplePin);
        /* arduino uno */
        if (people == 0) {
          digitalWrite(peopleLED, HIGH);
        } else {
          digitalWrite(peopleLED, LOW);
        }
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
        if (people == 0 && detectDurations >= 5000) {
          int ventilationFan = digitalRead(ventilationFanPin);
          int electricWallFan = digitalRead(electricWallFanPin);
          int airConditioner = digitalRead(airConditionerPin);
          digitalWrite(ventilationFanPin, LOW);
          digitalWrite(electricWallFanPin, HIGH);
          digitalWrite(airConditionerPin, HIGH);

          state3 = false;
          state4 = true;
          /* arduino uno */
          digitalWrite(state3LED, LOW);
          digitalWrite(state4LED, HIGH);
          static unsigned long delay;
          delay = millis();
          while (true) {
            if (millis() - delay >= 10000) {
              break;
            }
            int people = digitalRead(peoplePin);
            /* arduino uno */
            if (people == 0) {
              digitalWrite(peopleLED, HIGH);
            } else {
              digitalWrite(peopleLED, LOW);
            }
          }

          peopleStatusPrevious = LOW;
          break;
        }
        int people = digitalRead(peoplePin);
        /* arduino uno */
        if (people == 0) {
          digitalWrite(peopleLED, HIGH);
        } else {
          digitalWrite(peopleLED, LOW);
        }
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

    /* arduino uno */
    digitalWrite(state1LED, HIGH);
    digitalWrite(state4LED, LOW);
    digitalWrite(ventilationFanPin, HIGH);
    digitalWrite(airConditionerPin, LOW);

    peopleStatusPrevious = LOW;
  }

  /* when the person cannot be found */
  int people = digitalRead(peoplePin);
  /* arduino uno */
  if (people == 0) {
    digitalWrite(peopleLED, HIGH);
  } else {
    digitalWrite(peopleLED, LOW);
  }
  detectMillis = millis();
  if (people == 1 && peopleStatusPrevious == LOW) {
    longDetectMillis = detectMillis;
    peopleStatusPrevious = HIGH;
  }
  detectDurations = detectMillis - longDetectMillis;
  if (people == 1 && detectDurations >= 10000) {
    state1 = true;
    state4 = false;
    /* arduino uno */
    digitalWrite(state1LED, HIGH);
    digitalWrite(state4LED, LOW);

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
