#include <Arduino.h>
#include <WiFi.h>
#include <Firebase_ESP_Client.h>

// Provide the token generation process info.
#include <addons/TokenHelper.h>

// Provide the RTDB payload printing info and other helper functions.
#include <addons/RTDBHelper.h>

/* 1. Define the WiFi credentials */
#define WIFI_SSID "COC123"
#define WIFI_PASSWORD "ooooo123"

/* 2. Define the API Key */
#define API_KEY "AIzaSyCFcMdUr5eVTGJnEZfWD1YjRYXrV0Tyg_Y"

/* 3. Define the RTDB URL */
#define DATABASE_URL "https://esp32-aircontroller-default-rtdb.asia-southeast1.firebasedatabase.app/"  //<databaseName>.firebaseio.com or <databaseName>.<region>.firebasedatabase.app

/* 4. Define the user Email and password that alreadey registerd or added in your project */
#define USER_EMAIL "aleefrock12345@gmail.com"
#define USER_PASSWORD "!12345"

// Define Firebase Data object
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;
unsigned long sendDataPrevMillis = 0;

#define peoplePin 13
#define schedulePin 25

unsigned long detectDurations;
unsigned long longDetectMillis;
int peopleStatusPrevious = LOW;
unsigned long detectMillis;
bool statusAirFirebase;
bool statusAir = false;
int people;

void setup() {
  Serial.begin(115200);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();
  Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);

  /* Assign the api key (required) */
  config.api_key = API_KEY;

  /* Assign the user sign in credentials */
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;

  /* Assign the RTDB URL (required) */
  config.database_url = DATABASE_URL;

  /* Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback;  // see addons/TokenHelper.h
  Firebase.reconnectNetwork(true);

  // Since v4.4.x, BearSSL engine was used, the SSL buffer need to be set.
  // Large data transmission may require larger RX buffer, otherwise connection issue or data read time out can be occurred.
  fbdo.setBSSLBufferSize(4096 /* Rx buffer size in bytes from 512 - 16384 */, 1024 /* Tx buffer size in bytes from 512 - 16384 */);

  // Limit the size of response payload to be collected in FirebaseData
  fbdo.setResponseSize(2048);

  Firebase.begin(&config, &auth);

  Firebase.setDoubleDigits(5);

  config.timeout.serverResponse = 10 * 1000;

  if (Firebase.ready()) {
    if (Firebase.RTDB.setBool(&fbdo, F("/DeviceStatus/AirConditioner"), statusAir)) {
      Serial.print("Update status success: ");
      Serial.println(statusAir);
    } else {
      Serial.println(fbdo.errorReason().c_str());
    }
  }

  pinMode(peoplePin, INPUT_PULLUP);
  pinMode(schedulePin, INPUT_PULLUP);
}

void UpdateData() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi disconnected. Reconnecting...");
    WiFi.reconnect();
    while (WiFi.status() != WL_CONNECTED) {
      delay(1000);
      Serial.print(".");
    }
    Serial.println("Connected to WiFi");
    Firebase.reconnectWiFi(true);
  } else {
    if (Firebase.ready() && (millis() - sendDataPrevMillis > 15000 || sendDataPrevMillis == 0)) {
      sendDataPrevMillis = millis();
      if (Firebase.RTDB.getBool(&fbdo, F("/DeviceStatus/AirConditioner"), &statusAirFirebase)) {
        if (statusAirFirebase != statusAir) {
          if (Firebase.RTDB.setBool(&fbdo, F("/DeviceStatus/AirConditioner"), statusAir)) {
            Serial.print("Update status success: ");
            Serial.println(statusAir);
          } else {
            Serial.println(fbdo.errorReason().c_str());
          }
        }
      } else {
        Serial.println(fbdo.errorReason().c_str());
      }

      if (Firebase.RTDB.setBool(&fbdo, F("/Human/Detection"), !people)) {
        Serial.print("Detection : ");
        Serial.println(!people);
      } else {
        Serial.println(fbdo.errorReason().c_str());
      }
    }
  }
}

void loop() {
  int schedule = digitalRead(schedulePin);
  /* ตรวจคน */
  people = digitalRead(peoplePin);
  if (people == 1 && statusAir) {
    /* เมื่อไม่เจอคน */
    while (true) {
      /* คนไม่อยู่นานตามเวลาไหม */
      detectMillis = millis();
      if (peopleStatusPrevious == LOW) {
        longDetectMillis = detectMillis;
        peopleStatusPrevious = HIGH;
      }
      detectDurations = detectMillis - longDetectMillis;
      if (people == 1 && peopleStatusPrevious == HIGH && detectDurations >= 60 * 1000) {
        statusAir = false;
        peopleStatusPrevious = LOW;
        break;
      }
      /* ตรวจคน */
      people = digitalRead(peoplePin);
      if (people == 0) {
        peopleStatusPrevious = LOW;
        break;
      }
      UpdateData();
    }
  } else if (people == 0 && !statusAir && schedule == LOW) {
    /* เมื่อเจอคน */
    while (true) {
      /* คนอยู่นานตามเวลาไหม */
      detectMillis = millis();
      if (peopleStatusPrevious == LOW) {
        longDetectMillis = detectMillis;
        peopleStatusPrevious = HIGH;
      }
      detectDurations = detectMillis - longDetectMillis;
      if (people == 0 && peopleStatusPrevious == HIGH && detectDurations >= 60 * 1000) {
        statusAir = true;
        peopleStatusPrevious = LOW;
        break;
      }
      int schedule = digitalRead(schedulePin);
      /* ตรวจคน */
      people = digitalRead(peoplePin);
      if (people == 1 || schedule == HIGH) {
        peopleStatusPrevious = LOW;
        break;
      }
      UpdateData();
    }
  }
  if (statusAir && schedule == HIGH) {
    statusAir = false;
  }
  UpdateData();
}
