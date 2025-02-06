#include <WiFi.h>

const char* ssid = "Ghuzy";
const char* password = "01066709440@AHMAG";

String massage = "Hello client" ;

WiFiServer server(80);

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi.");
      Serial.print("IP add: ");
    Serial.println(WiFi.localIP());
  server.begin();
}

void loop() {
  WiFiClient client = server.available();
  if (client) {
    Serial.println("Client connected.");

    while (client.connected()) {
      if (client.available()) {
        String request = client.readStringUntil('\r');
        Serial.println("Received: " + request);

        // Process the request and prepare a response
        String response = "Response from ESP32: " + request;

        // Send the response back to the client
        client.print(massage);
        client.flush();
        Serial.println("Sent: " + massage);
      }
    }

    client.stop();
    Serial.println("Client disconnected.");
  }
}
