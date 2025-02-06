#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

const char* ssid = "AL- Tarek Social";  // Replace with your network SSID
const char* password = "Meez242#";  // Replace with your network password
const char* serverUrl = "http://192.168.1.19:5000/data/ESP32_2"; // Replace with your server's IP address

void setup() {
  Serial.begin(115200);
  delay(10);

  // Connect to WiFi
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    // Specify request destination
    http.begin(serverUrl);

    // Specify content-type header
    http.addHeader("Content-Type", "application/json");

    // Create JSON object
    StaticJsonDocument<200> doc;
    //doc["client_id"] = "ESP32_1";  // Set the client ID

    // Add custom key-value pairs
    doc["temperature"] = 25.5;     // Example data
    doc["humidity"] = 60;          // Example data
    
    doc["status"] = "OK";          // Example status message

    // Serialize JSON object to string
    String jsonString;
    serializeJson(doc, jsonString);

    // Send POST request
    int httpResponseCode = http.POST(jsonString);

    // Check the response code
    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.println("HTTP Response code: " + String(httpResponseCode));
      Serial.println("Response: " + response);
    } else {
      Serial.println("Error on sending POST: " + String(httpResponseCode));
    }
    // End the HTTP connection
    http.end();
  }
  
  delay(5000); // Send a request every 5 seconds
}
