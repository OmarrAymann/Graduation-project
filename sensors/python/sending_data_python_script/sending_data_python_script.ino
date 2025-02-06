#include <WiFi.h>
uint8_t dataArray[] = {1, 2, 3, 4, 5};
const char* ssid = "Ghuzy";
const char* password = "01066709440@AHMAG";
const char* host = "192.168.1.5";  // IP address of the Python server
WiFiClient client;

void setup() {
    Serial.begin(115200);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Connecting to WiFi...");
    }
    Serial.println("Connected to WiFi");
    Serial.print("IP add: ");
    Serial.println(WiFi.localIP());
}

void loop() {
    if (!client.connect(host, 12345)) {
        Serial.println("Connection failed.");
        return;
    }
    Serial.println("Sending data...");
    client.write(dataArray, sizeof(dataArray));
    int x= client.read(); 
    client.stop();
    delay(1000);
}