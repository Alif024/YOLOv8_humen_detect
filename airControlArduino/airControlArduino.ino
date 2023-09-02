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
int peopleStatusPrevious = LOW;
unsigned long detectMillis;

/* arduino uno */
#define peopleLED 3
#define state1LED 8
#define state2LED 9
#define state3LED 10
#define state4LED 11

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
  static unsigned long detectDurations;
  static unsigned long longDetectMillis;

  /* ตรวจคน */
  int people = digitalRead(peoplePin);

  /* arduino uno */
  if (people == 0) {
    digitalWrite(peopleLED, HIGH);
  } else {
    digitalWrite(peopleLED, LOW);
  }

  if (people == 1 && state1) {
    /* เมื่อไม่เจอคน */
    while (true) {
      /* ตรวจคาบ */
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
    /* เมื่อเจอคน */
    while (true) {
      /* ตรวจคาบ */
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
  static unsigned long detectDurations;
  static unsigned long longDetectMillis;

  /* ตรวจคน */
  int people = digitalRead(peoplePin);

  /* arduino uno */
  if (people == 0) {
    digitalWrite(peopleLED, HIGH);
  } else {
    digitalWrite(peopleLED, LOW);
  }

  if (people == 1 && state2) {
    /* เมื่อไม่เจอคน */
    while (true) {
      /* ตรวจคาบ */
      int schedule = digitalRead(schedulePin);
      if (schedule == 1 && scheduleState) {
        scheduleState = false;
        state1 = true;
        state2 = false;

        /* arduino uno */
        digitalWrite(state1LED, HIGH);
        digitalWrite(state2LED, LOW);

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
    /* เมื่อเจอคน */
    while (true) {
      /* ตรวจคาบ */
      int schedule = digitalRead(schedulePin);
      if (schedule == 1 && scheduleState) {
        scheduleState = false;
        state1 = true;
        state2 = false;

        /* arduino uno */
        digitalWrite(state1LED, HIGH);
        digitalWrite(state2LED, LOW);

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

        /* arduino uno */
        digitalWrite(state2LED, LOW);
        digitalWrite(state3LED, HIGH);

        peopleStatusPrevious = LOW;
        break;
      }
      /* ตรวจคน */
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

void taskState3() {
  static unsigned long delay;
  static unsigned long detectDurations;
  static unsigned long longDetectMillis;

  /* ตรวจคน */
  int people = digitalRead(peoplePin);

  /* arduino uno */
  if (people == 0) {
    digitalWrite(peopleLED, HIGH);
  } else {
    digitalWrite(peopleLED, LOW);
  }

  if (people == 1 && state3) {
    /* เมื่อไม่เจอคน */
    while (true) {
      /* ตรวจคาบ */
      int schedule = digitalRead(schedulePin);
      if (schedule == 1 && scheduleState) {
        scheduleState = false;
        state1 = true;
        state3 = false;

        /* arduino uno */
        digitalWrite(state1LED, HIGH);
        digitalWrite(state3LED, LOW);

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

        /* arduino uno */
        digitalWrite(state2LED, HIGH);
        digitalWrite(state3LED, LOW);

        digitalWrite(electricWallFanPin, LOW);
        digitalWrite(ventilationFanPin, LOW);
        peopleStatusPrevious = LOW;
        break;
      }
      /* ตรวจคน */
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
    /* เมื่อเจอคน */
    while (true) {
      /* ตรวจคาบ */
      int schedule = digitalRead(schedulePin);
      if (schedule == 1 && scheduleState) {
        scheduleState = false;
        state1 = true;
        state3 = false;

        /* arduino uno */
        digitalWrite(state1LED, HIGH);
        digitalWrite(state3LED, LOW);

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

        /* arduino uno */
        digitalWrite(state3LED, LOW);
        digitalWrite(state4LED, HIGH);

        /* หน่วงเวลา */
        delay = millis();
        while (true) {
          /* arduino uno */
          int people = digitalRead(peoplePin);
          if (people == 0) {
            digitalWrite(peopleLED, HIGH);
          } else {
            digitalWrite(peopleLED, LOW);
          }
          if (millis() - delay >= 10000) {
            break;
          }
        }
        peopleStatusPrevious = LOW;
        break;
      }
      /* ตรวจคน */
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

  /* arduino uno */
  if (people == 0) {
    digitalWrite(peopleLED, HIGH);
  } else {
    digitalWrite(peopleLED, LOW);
  }

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

    /* arduino uno */
    digitalWrite(state1LED, HIGH);
    digitalWrite(state4LED, LOW);

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
