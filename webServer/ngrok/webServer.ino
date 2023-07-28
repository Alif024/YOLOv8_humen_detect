#include <WiFi.h>
#include <ESPAsyncWebServer.h>

// Replace with your network credentials
const char* ssid = "ONE_2.4GHz";
const char* password = "91919191";

// GPIO pin for the button
const int personsPin = 13;
const int airConditionerPin = 12;
const int electricWallFanPin = 14;
const int ventilationFanPin = 27;

// Create an instance of the server
AsyncWebServer server(80);

// HTML content to serve the webpage
const char* indexHtml = R"(
<!DOCTYPE html>
<html>
<head>
    <title>ESP32 Device Status</title>
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
</head>
<body>
    <h1>ESP32 Device Status</h1>
    <p>Persons: <span id="persons">Loading...</span></p>
    <p>Air Conditioner: <span id="air-conditioner">Loading...</span></p>
    <p>Electric Wall Fan: <span id="electric-wall-fan">Loading...</span></p>
    <p>Ventilation Fan: <span id="ventilation-fan">Loading...</span></p>
</body>
</html>
)";

void setup() {
    Serial.begin(115200);
    pinMode(personsPin, INPUT_PULLUP);
    pinMode(airConditionerPin, INPUT_PULLUP);
    pinMode(electricWallFanPin, INPUT_PULLUP);
    pinMode(ventilationFanPin, INPUT_PULLUP);

    // Connect to Wi-Fi
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Connecting to WiFi...");
    }
    Serial.println("Connected to WiFi");

    // Print the server's IP address to the Serial Monitor
    Serial.print("Server IP address: ");
    Serial.println(WiFi.localIP());

    // Route for root / web page
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(200, "text/html", indexHtml);
    });

    // Route for getting button status
    server.on("/status", HTTP_GET, [](AsyncWebServerRequest *request) {
        int personsState = digitalRead(personsPin);
        int airConditionerState = digitalRead(airConditionerPin);
        int electricWallFanState = digitalRead(electricWallFanPin);
        int ventilationFanState = digitalRead(ventilationFanPin);

        String statusJson = "{\"persons\":\"" + String(personsState == HIGH ? "present" : "absent") +
                            "\",\"air_conditioner\":\"" + String(airConditionerState == HIGH ? "on" : "off") +
                            "\",\"electric_wall_fan\":\"" + String(electricWallFanState == HIGH ? "on" : "off") +
                            "\",\"ventilation_fan\":\"" + String(ventilationFanState == HIGH ? "on" : "off") + "\"}";

        request->send(200, "application/json", statusJson);
    });

    // Start the server
    server.begin();
    Serial.println("Web server started");
}

void loop() {
    // No need to call server.handleClient() in the loop for AsyncWebServer
}
