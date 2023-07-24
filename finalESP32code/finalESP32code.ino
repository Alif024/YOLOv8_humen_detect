#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <TridentTD_LineNotify.h>

// Replace with your network credentials
const char* SSID = "ONE_2.4GHz";
const char* PASSWORD = "91919191";
const char* LINE_TOKEN = "w9QTrv5PZYiGjVFYKbhfpb0JeXtcf6Q9wibiQCwBh5Q";  // Line Token
char buf[19];

const int schedulePin = 25;
const int finishSchedulePin = 26;
const int personsPin = 13;
const int airConditionerPin = 12;
const int electricWallFanPin = 14;
const int ventilationFanPin = 27;

// Create an instance of the server
AsyncWebServer server(80);

const char* indexHtml = R"(
<!DOCTYPE html>
<html>

<head>
    <meta charset='UTF-8'>
    <meta name='viewport' content='width=device-width, initial-scale=1.0'>
    <title>Document</title>
    <script>
        // Function to update the device statuses
        function updateDeviceStatus() {
            fetch("/status")
                .then(response => response.json())
                .then(data => {
                    document.getElementById("persons").innerHTML = data.persons;
                    document.getElementById("air-conditioner").innerHTML = data.air_conditioner;
                    document.getElementById("electric-wall-fan").innerHTML = data.electric_wall_fan;
                    document.getElementById("ventilation-fan").innerHTML = data.ventilation_fan;
                });
        }

        // Periodically update the device statuses every 2 seconds
        setInterval(updateDeviceStatus, 2000);
    </script>
    <style>
        table {
            font-family: arial, sans-serif;
            border-collapse: collapse;
            width: 100%;
        }

        td,
        th {
            border: 1px solid black;
            text-align: left;
            padding: 8px;
            text-align: center;
        }

        tr:nth-child(odd) {
            background-color: #FF7F50;
        }

        .item1 {
            grid-area: header;
        }

        .item1-1 {
            grid-area: section1;
        }

        .item1-2 {
            grid-area: section2;
        }

        .item1-3 {
            grid-area: section3;
        }

        .item1-4 {
            grid-area: section4;
        }

        .item1-5 {
            grid-area: section5;
        }

        .item1-6 {
            grid-area: section6;
        }

        .item1-7 {
            grid-area: section7;
        }

        .item1-8 {
            grid-area: section8;
        }

        .item1-9 {
            grid-area: section9;
        }

        .item1-10 {
            grid-area: section10;
        }

        .item1-11 {
            grid-area: section11;
        }

        .item2-1 {
            grid-area: menu-1;
        }

        .item2-2 {
            grid-area: menu-2;
        }

        .item2-3 {
            grid-area: menu-3;
        }

        .item2-4 {
            grid-area: menu-4;
        }

        .item2-5 {
            grid-area: menu-5;
        }

        .item-main-mon-1 {
            grid-area: main1-mon;
        }

        .item-main-mon-2 {
            grid-area: main2-mon;
        }

        .item-main-mon-3 {
            grid-area: main3-mon;
        }

        .item-main-mon-4 {
            grid-area: main4-mon;
        }

        .item-right-mon-1 {
            grid-area: right1-mon;
        }

        .item-right-mon-2 {
            grid-area: right2-mon;
        }

        .item-right-mon-3 {
            grid-area: right3-mon;
        }

        .item-right-mon-4 {
            grid-area: right4-mon;
        }

        .item-right-mon-5 {
            grid-area: right5-mon;
        }

        .item6 {
            grid-area: break;
        }

        .item-main-tue-1 {
            grid-area: main1-tue;
        }

        .item-main-tue-2 {
            grid-area: main2-tue;
        }

        .item-main-tue-3 {
            grid-area: main3-tue;
        }

        .item-main-tue-4 {
            grid-area: main4-tue;
        }

        .item-right-tue-1 {
            grid-area: right1-tue;
        }

        .item-right-tue-2 {
            grid-area: right2-tue;
        }

        .item-right-tue-3 {
            grid-area: right3-tue;
        }

        .item-right-tue-4 {
            grid-area: right4-tue;
        }

        .item-right-tue-5 {
            grid-area: right5-tue;
        }

        .item-main-wed-1 {
            grid-area: main1-wed;
        }

        .item-main-wed-2 {
            grid-area: main2-wed;
        }

        .item-main-wed-3 {
            grid-area: main3-wed;
        }

        .item-main-wed-4 {
            grid-area: main4-wed;
        }

        .item-right-wed-1 {
            grid-area: right1-wed;
        }

        .item-right-wed-2 {
            grid-area: right2-wed;
        }

        .item-right-wed-3 {
            grid-area: right3-wed;
        }

        .item-right-wed-4 {
            grid-area: right4-wed;
        }

        .item-right-wed-5 {
            grid-area: right5-wed;
        }

        .item-main-thu-1 {
            grid-area: main1-thu;
        }

        .item-main-thu-2 {
            grid-area: main2-thu;
        }

        .item-main-thu-3 {
            grid-area: main3-thu;
        }

        .item-main-thu-4 {
            grid-area: main4-thu;
        }

        .item-right-thu-1 {
            grid-area: right1-thu;
        }

        .item-right-thu-2 {
            grid-area: right2-thu;
        }

        .item-right-thu-3 {
            grid-area: right3-thu;
        }

        .item-right-thu-4 {
            grid-area: right4-thu;
        }

        .item-right-thu-5 {
            grid-area: right5-thu;
        }

        .item-main-fri-1 {
            grid-area: main1-fri;
        }

        .item-main-fri-2 {
            grid-area: main2-fri;
        }

        .item-main-fri-3 {
            grid-area: main3-fri;
        }

        .item-main-fri-4 {
            grid-area: main4-fri;
        }

        .item-right-fri-1 {
            grid-area: right1-fri;
        }

        .item-right-fri-2 {
            grid-area: right2-fri;
        }

        .item-right-fri-3 {
            grid-area: right3-fri;
        }

        .item-right-fri-4 {
            grid-area: right4-fri;
        }

        .item-right-fri-5 {
            grid-area: right5-fri;
        }

        .grid-container {
            display: grid;
            grid-template-areas: 'header header header header header header header header header header header'
                'section1 section2 section3 section4 section5 section6 section7 section8 section9 section10 section11'
                'menu-1 main1-mon main2-mon main3-mon main4-mon break right1-mon right2-mon right3-mon right4-mon right5-mon'
                'menu-2 main1-tue main2-tue main3-tue main4-tue break right1-tue right2-tue right3-tue right4-tue right5-tue'
                'menu-3 main1-wed main2-wed main3-wed main4-wed break right1-wed right2-wed right3-wed right4-wed right5-wed'
                'menu-4 main1-thu main2-thu main3-thu main4-thu break right1-thu right2-thu right3-thu right4-thu right5-thu'
                'menu-5 main1-fri main2-fri main3-fri main4-fri break right1-fri right2-fri right3-fri right4-fri right5-fri';
            gap: 1px;
            padding: 1px;
        }

        .grid-container>div {
            background-color: #FAC898;
            text-align: center;
            padding: 10px 0;
            font-size: small;
        }
    </style>
</head>

<body>
    <div class='grid-container' style='background-color: black; font-family: arial, sans-serif;'>
        <div class='item1' style='color: white; background-color: #FF7F50; font-size: larger;'><b>ตารางเรียน</b></div>
        <div class='item2-1'>จันทร์</div>
        <div class='item2-2'>อังคาร</div>
        <div class='item2-3'>พุธ</div>
        <div class='item2-4'>พฤหัสบดี</div>
        <div class='item2-5'>ศุกร์</div>
        <div class='item1-1' style='color: white;background-color: #FF7F50;'>วัน/เวลา</div>
        <div class='item1-2' style='color: white;background-color: #FF7F50;'>08.15 - 09.15</div>
        <div class='item1-3' style='color: white;background-color: #FF7F50;'>09.15 - 10.15</div>
        <div class='item1-4' style='color: white;background-color: #FF7F50;'>10.15 - 11.15</div>
        <div class='item1-5' style='color: white;background-color: #FF7F50;'>11.15 - 12.15</div>
        <div class='item1-6' style='color: white;background-color: #FF7F50;'>12.15 - 13.15</div>
        <div class='item1-7' style='color: white;background-color: #FF7F50;'>13.15 - 14.15</div>
        <div class='item1-8' style='color: white;background-color: #FF7F50;'>14.15 - 15.15</div>
        <div class='item1-9' style='color: white;background-color: #FF7F50;'>15.15 - 16.15</div>
        <div class='item1-10' style='color: white;background-color: #FF7F50;'>16.15 - 17.15</div>
        <div class='item1-11' style='color: white;background-color: #FF7F50;'>17.15 - 18.15</div>
        <div class='item-main-mon-1'>คาบ</div>
        <div class='item-main-mon-2'>คาบ</div>
        <div class='item-main-mon-3'>คาบ</div>
        <div class='item-main-mon-4'>คาบ</div>
        <div class='item-right-mon-1'>คาบ</div>
        <div class='item-right-mon-2'>คาบ</div>
        <div class='item-right-mon-3'>คาบ</div>
        <div class='item-right-mon-4'>คาบ</div>
        <div class='item-right-mon-5'>คาบ</div>
        <div class='item-main-tue-1'>คาบ</div>
        <div class='item-main-tue-2'>คาบ</div>
        <div class='item-main-tue-3'>คาบ</div>
        <div class='item-main-tue-4'>คาบ</div>
        <div class='item-right-tue-1'>คาบ</div>
        <div class='item-right-tue-2'>คาบ</div>
        <div class='item-right-tue-3'>คาบ</div>
        <div class='item-right-tue-4'>คาบ</div>
        <div class='item-right-tue-5'>คาบ</div>
        <div class='item-main-wed-1'>คาบ</div>
        <div class='item-main-wed-2'>คาบ</div>
        <div class='item-main-wed-3'>คาบ</div>
        <div class='item-main-wed-4'>คาบ</div>
        <div class='item-right-wed-1'>คาบ</div>
        <div class='item-right-wed-2'>คาบ</div>
        <div class='item-right-wed-3'>คาบ</div>
        <div class='item-right-wed-4'>คาบ</div>
        <div class='item-right-wed-5'>คาบ</div>
        <div class='item-main-thu-1'>คาบ</div>
        <div class='item-main-thu-2'>คาบ</div>
        <div class='item-main-thu-3'>คาบ</div>
        <div class='item-main-thu-4'>คาบ</div>
        <div class='item-right-thu-1'>คาบ</div>
        <div class='item-right-thu-2'>คาบ</div>
        <div class='item-right-thu-3'>คาบ</div>
        <div class='item-right-thu-4'>คาบ</div>
        <div class='item-right-thu-5'>คาบ</div>
        <div class='item-main-fri-1'>คาบ</div>
        <div class='item-main-fri-2'>คาบ</div>
        <div class='item-main-fri-3'>คาบ</div>
        <div class='item-main-fri-4'>คาบ</div>
        <div class='item-right-fri-1'>คาบ</div>
        <div class='item-right-fri-2'>คาบ</div>
        <div class='item-right-fri-3'>คาบ</div>
        <div class='item-right-fri-4'>คาบ</div>
        <div class='item-right-fri-5'>คาบ</div>
        <div class='item6' style='background-color: #ffd6af;'><a style='margin-left: 10px;'></a></div>
    </div>
    <div>
        <h3 style='text-align: center; padding-top: 10px;'>แถบแสดงสถานะต่างๆ</h3>
        <table style='background-color: #FAC898;'>
            <tr>
                <td style='color: aliceblue; font-size: large;'><b>air conditioner</td>
                <td style='color: aliceblue; font-size: large;'><b>electric wall fan</td>
                <td style='color: aliceblue; font-size: large;'><b>ventilation fan</td>
                <td style='color: aliceblue; font-size: large;'><b>persons</td>
            </tr>
            <tr>
                <td>
                    <p><span id="air-conditioner">Loading...</span></p>
                </td>
                <td>
                    <p><span id="electric-wall-fan">Loading...</span></p>
                </td>
                <td>
                    <p><span id="ventilation-fan">Loading...</span></p>
                </td>
                <td>
                    <p><span id="persons">Loading...</span></p>
                </td>
            </tr>
        </table>
    </div>
</body>

</html>
)";

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
  Serial.begin(115200);
  pinMode(personsPin, INPUT_PULLUP);
  pinMode(schedulePin, INPUT_PULLUP);
  pinMode(finishSchedulePin, INPUT_PULLUP);
  pinMode(electricWallFanPin, OUTPUT);
  pinMode(airConditionerPin, OUTPUT);
  pinMode(ventilationFanPin, OUTPUT);

  WiFi.begin(SSID, PASSWORD);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(1000);
        Serial.println("Connecting to WiFi...");
    }
    Serial.println("Connected to WiFi");
    LINE.setToken(LINE_TOKEN);
    sprintf(buf, "%d.%d.%d.%d", WiFi.localIP()[0], WiFi.localIP()[1], WiFi.localIP()[2], WiFi.localIP()[3]);
    LINE.notify(buf);

    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send(200, "text/html", indexHtml); });

    // Route for getting button status
    server.on("/status", HTTP_GET, [](AsyncWebServerRequest *request)
              {
    int personsState = digitalRead(personsPin);
    int airConditionerState = digitalRead(airConditionerPin);
    int electricWallFanState = digitalRead(electricWallFanPin);
    int ventilationFanState = digitalRead(ventilationFanPin);

    String statusJson = "{\"persons\":\"" + String(personsState == LOW ? "Present" : "Absent") + 
                        "\",\"air_conditioner\":\"" + String(airConditionerState == HIGH ? "ON" : "OFF") + 
                        "\",\"electric_wall_fan\":\"" + String(electricWallFanState == HIGH ? "ON" : "OFF") + 
                        "\",\"ventilation_fan\":\"" + String(ventilationFanState == HIGH ? "ON" : "OFF") + "\"}";

    request->send(200, "application/json", statusJson); });

    // Start the server
    server.begin();
    Serial.println("Web server started");
}

void taskState1() {
  uint8_t scheduleState = digitalRead(schedulePin);
  if (scheduleState == LOW && state1) {
    state1 = false;
    state2 = true;
    classTimeState = true;
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
      digitalWrite(ventilationFanPin, LOW);
      digitalWrite(electricWallFanPin, LOW);
    } else {
      while (true) {
        detectMillis = millis();
        uint8_t detectState = digitalRead(personsPin);
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
        uint8_t detectState = digitalRead(personsPin);
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
  static uint8_t detectStatePrevious = LOW;
  static unsigned long detectMillis;
  detectMillis = millis();
  uint8_t detectState = digitalRead(personsPin);
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

  uint8_t finishScheduleState = digitalRead(finishSchedulePin);
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
  static uint8_t detectStatePrevious = LOW;
  static unsigned long detectMillis;
  detectMillis = millis();
  uint8_t detectState = digitalRead(personsPin);
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
  uint8_t scheduleState = digitalRead(schedulePin);
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
