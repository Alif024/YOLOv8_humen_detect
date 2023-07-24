const int personPin = 0;
const int schedulePin = 1;
const int finishSchedulePin = 2;
const int personLED = 3;
const int electricWallFanPin = 4;
const int airConditionerPin = 5;
const int ventilationFanPin = 6;
const int state1Pin = 8;
const int state2Pin = 9;
const int state3Pin = 10;
const int state4Pin = 11;
const int state5Pin = 12;

/* state_status */
bool state1 = true;
bool state2 = false;
bool state3 = false;
bool state4 = false;
bool state5 = false;

/* taskState2 */
unsigned long classTime;
bool classTimeState = true;

void setup() {
  pinMode(personPin, INPUT_PULLUP);
  pinMode(schedulePin, INPUT_PULLUP);
  pinMode(finishSchedulePin, INPUT_PULLUP);
  pinMode(electricWallFanPin, OUTPUT);
  pinMode(airConditionerPin, OUTPUT);
  pinMode(ventilationFanPin, OUTPUT);
  pinMode(personLED, OUTPUT);
  pinMode(state1Pin, OUTPUT);
  pinMode(state2Pin, OUTPUT);
  pinMode(state3Pin, OUTPUT);
  pinMode(state4Pin, OUTPUT);
  pinMode(state5Pin, OUTPUT);
  digitalWrite(state1Pin, HIGH);
  digitalWrite(state2Pin, LOW);
  digitalWrite(state3Pin, LOW);
  digitalWrite(state4Pin, LOW);
  digitalWrite(state5Pin, LOW);
}

void taskState1() {
  uint8_t scheduleState = digitalRead(schedulePin);
  if (scheduleState == LOW && state1) {
    state1 = false;
    state2 = true;
    digitalWrite(state1Pin, LOW);
    digitalWrite(state2Pin, HIGH);
    classTimeState = true;
  }
  uint8_t personState = digitalRead(personPin);
  if (personState == LOW) {
    digitalWrite(personLED, HIGH);
  } else {
    digitalWrite(personLED, LOW);
  }
}

void taskState2() {
  static unsigned long detectDurations;
  static unsigned long detectLongMillis;
  static uint8_t detectStatePrevious = LOW;
  static unsigned long detectMillis;
  if (classTimeState && state2) {
    classTime = millis();
    classTimeState = false;
  }
  if (!classTimeState && state2) {
    if (millis() - classTime >= 20000) {
      state1 = true;
      state2 = false;
      digitalWrite(state1Pin, HIGH);
      digitalWrite(state2Pin, LOW);
      digitalWrite(ventilationFanPin, LOW);
      digitalWrite(electricWallFanPin, LOW);
    } else {
      while (true) {
        detectMillis = millis();
        uint8_t detectState = digitalRead(personPin);
        if (detectState == LOW) {
          digitalWrite(personLED, HIGH);
        } else {
          digitalWrite(personLED, LOW);
        }
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
          digitalWrite(state2Pin, LOW);
          digitalWrite(state3Pin, HIGH);
          detectStatePrevious = LOW;
          break;
        }
        if (detectState == LOW && detectStatePrevious == LOW) {
          digitalWrite(ventilationFanPin, HIGH);
          digitalWrite(electricWallFanPin, HIGH);
          state2 = false;
          state3 = true;
          digitalWrite(state2Pin, LOW);
          digitalWrite(state3Pin, HIGH);
          break;
        }
      }
    }
    uint8_t personState = digitalRead(personPin);
    if (personState == LOW) {
      digitalWrite(personLED, HIGH);
    } else {
      digitalWrite(personLED, LOW);
    }
  }
}

void taskState3() {
  static unsigned long detectDurations;
  static unsigned long detectLongMillis;
  static uint8_t detectStatePrevious = LOW;
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
      digitalWrite(state3Pin, LOW);
      digitalWrite(state4Pin, HIGH);
      delayTime = millis();
      while (true) {
        if (millis() - delayTime >= 5000) {
          state3 = false;
          state4 = true;
          detectionTimeState = true;
          break;
        }
        uint8_t personState = digitalRead(personPin);
        if (personState == LOW) {
          digitalWrite(personLED, HIGH);
        } else {
          digitalWrite(personLED, LOW);
        }
      }
    } else {
      while (true) {
        detectMillis = millis();
        uint8_t detectState = digitalRead(personPin);
        if (detectState == LOW) {
          digitalWrite(personLED, HIGH);
        } else {
          digitalWrite(personLED, LOW);
        }
        if (detectState == HIGH && detectStatePrevious == LOW) {
          detectLongMillis = detectMillis;
          detectStatePrevious = HIGH;
        }
        detectDurations = detectMillis - detectLongMillis;
        if (detectState == HIGH && detectDurations >= 5000) {
          state2 = true;
          state3 = false;
          digitalWrite(state2Pin, HIGH);
          digitalWrite(state3Pin, LOW);
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
  static uint8_t detectStatePrevious = LOW;
  static unsigned long detectMillis;
  detectMillis = millis();
  uint8_t detectState = digitalRead(personPin);
  if (detectState == LOW) {
    digitalWrite(personLED, HIGH);
  } else {
    digitalWrite(personLED, LOW);
  }
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
    digitalWrite(state1Pin, HIGH);
    digitalWrite(state4Pin, LOW);
    detectStatePrevious = LOW;
  }
  if (detectState == LOW && detectStatePrevious == HIGH) {
    detectStatePrevious = LOW;
  }

  uint8_t finishScheduleState = digitalRead(finishSchedulePin);
  if (finishScheduleState == LOW && state4) {
    digitalWrite(airConditionerPin, LOW);
    digitalWrite(ventilationFanPin, HIGH);
    state4 = false;
    state5 = true;
    digitalWrite(state4Pin, LOW);
    digitalWrite(state5Pin, HIGH);
    detectStatePrevious = LOW;
  }
}

void taskState5() {
  static unsigned long detectDurations;
  static unsigned long detectLongMillis;
  static uint8_t detectStatePrevious = LOW;
  static unsigned long detectMillis;
  detectMillis = millis();
  uint8_t detectState = digitalRead(personPin);
  if (detectState == LOW) {
    digitalWrite(personLED, HIGH);
  } else {
    digitalWrite(personLED, LOW);
  }
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
    digitalWrite(state1Pin, HIGH);
    digitalWrite(state5Pin, LOW);
    detectStatePrevious = LOW;
  }
  if (detectState == LOW && detectStatePrevious == HIGH) {
    detectStatePrevious = LOW;
  }
  uint8_t scheduleState = digitalRead(schedulePin);
  if (scheduleState == LOW && state5) {
    state2 = true;
    state5 = false;
    digitalWrite(state2Pin, HIGH);
    digitalWrite(state5Pin, LOW);
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
