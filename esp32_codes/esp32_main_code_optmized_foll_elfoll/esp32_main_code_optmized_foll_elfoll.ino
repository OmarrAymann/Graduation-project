#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <DHT.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <Adafruit_BMP085.h>
#include <Melopero_AMG8833.h>
#include <Ps3Controller.h>

// Pin definitions
#define IN1_PIN 23  
#define IN2_PIN 19  
#define IN3_PIN 18  
#define IN4_PIN 4   
#define DHTPIN 25
#define DHTTYPE DHT11
#define MQ2pin 35
#define Microwave_Pin 34

// WiFi credentials
const char ssid[] PROGMEM = "Ghuzy";
const char password[] PROGMEM = "01066709440@AHMAG";
const char serverUrl[] PROGMEM = "http://192.168.1.5:5000/data/ESP32_2";
const char thermalServerUrl[] PROGMEM = "http://192.168.1.5:5000/thermal_data/ESP32_2";

DHT dht(DHTPIN, DHTTYPE);
Adafruit_MPU6050 mpu;
Adafruit_BMP085 bmp;
Melopero_AMG8833 amg8833_sensor;

unsigned long previousMillis = 0;
const unsigned int interval = 200;

void onConnect(){
  Serial.println("Connected!.");
}

void notify()
{
    if( Ps3.data.button.cross ){
        //Serial.println("Pressing the cross button");
    }

    /*if( Ps3.data.button.square ){
        Serial.println("Pressing the square button");
    }

    if( Ps3.data.button.triangle ){
        Serial.println("Pressing the triangle button");
    }

    if( Ps3.data.button.circle ){
        Serial.println("Pressing the circle button");
    }*/
    else if( Ps3.data.button.up ){
        //Serial.println("Pressing the up button");
        CAR_moveForward();
    }
    else if( Ps3.data.button.right ){
        //Serial.println("Pressing the right button");
        CAR_turnRight();
    }
    else if( Ps3.data.button.down ){
        //Serial.println("Pressing the down button");
        CAR_moveBackward();
    }
    else if( Ps3.data.button.left ){
        //Serial.println("Pressing the left button");
        CAR_turnLeft();
    }
    else{
      CAR_stop();
    }
    /*leftX = (Ps3.data.analog.stick.lx);
    Serial.print("leftX:");
    Serial.println(leftX);
    leftY = (Ps3.data.analog.stick.ly);
    Serial.print("leftY:");
    Serial.println(leftY);
    rightX = (Ps3.data.analog.stick.rx);
    Serial.print("rightX:");
    Serial.println(rightX);
    rightY = (Ps3.data.analog.stick.ry);
    Serial.print("rightY:");
    Serial.println(rightY);*/
}

void setup() {
  Serial.begin(115200);
  delay(10);

  pinMode(IN1_PIN, OUTPUT);
  pinMode(IN2_PIN, OUTPUT);
  pinMode(IN3_PIN, OUTPUT);
  pinMode(IN4_PIN, OUTPUT);
  
  Ps3.attach(notify);
  Ps3.attachOnConnect(onConnect);
  Ps3.begin("48:f1:7f:c2:ab:ea");
  Serial.println("Ready.");
  
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected\nIP address: ");
  Serial.println(WiFi.localIP());

  pinMode(MQ2pin, INPUT);
  pinMode(Microwave_Pin, INPUT);
  dht.begin();

  initializeSensors();
}

void loop() {
  readSensors();
  //moveCar();

  if (millis() - previousMillis >= interval) {
    previousMillis = millis();
    sendData();
    sendThermalData();
  }
}

void initializeSensors() {
  if (!mpu.begin()) {
    //Serial.println("Failed to find MPU6050 chip");
    while (1);
  }
  mpu.setAccelerometerRange(MPU6050_RANGE_16_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
  
  if (!bmp.begin()) {
    //Serial.println("Could not find a valid BMP180 sensor, check wiring!");
    while (1);
  }

  Wire.begin();
  amg8833_sensor.initI2C();
  amg8833_sensor.resetFlagsAndSettings();
  amg8833_sensor.setFPSMode(FPS_MODE::FPS_10);
}

void readSensors() {
  if (digitalRead(Microwave_Pin) == HIGH) {
    //Serial.println("Motion active!");
  } else {
    //Serial.println("No Motion!");
  }

  int mq2State = digitalRead(MQ2pin);
  //Serial.print("Digital Output: ");
  //Serial.print(mq2State);
  if (mq2State == HIGH) {
    //Serial.println("  |  Smoke: -");
  } else {
    //Serial.println("  |  Smoke: Detected!");
  }

  float h = dht.readHumidity();
  float t = dht.readTemperature();
  /*if (!isnan(h) && !isnan(t)) {
    float hic = dht.computeHeatIndex(t, h, false);
    Serial.print("Humidity: ");
    Serial.print(h);
    Serial.print("%  Temperature: ");
    Serial.print(t);
    Serial.print("°C  Heat index: ");
    Serial.print(hic);
    Serial.println("°C");
  } else {
    Serial.println("Failed to read from DHT sensor!");
  }*/
}

void sendData() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(serverUrl);
    http.addHeader("Content-Type", "application/json");

    StaticJsonDocument<512> doc;
    doc["temperature"] = dht.readTemperature();
    doc["humidity"] = dht.readHumidity();
    doc["motion"] = digitalRead(Microwave_Pin) == HIGH ? "active" : "inactive";
    doc["smoke"] = digitalRead(MQ2pin) == HIGH ? "clear" : "detected";
    //doc["status"] = "OK";

    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);
    doc["acceleration_x"] = a.acceleration.x;
    doc["acceleration_y"] = a.acceleration.y;
    doc["acceleration_z"] = a.acceleration.z;
    //doc["gyro_x"] = g.gyro.x;
    //doc["gyro_y"] = g.gyro.y;
    //doc["gyro_z"] = g.gyro.z;
    //doc["mpu_temp"] = temp.temperature;

    //doc["bmp_temp"] = bmp.readTemperature();
    doc["bmp_pressure"] = bmp.readPressure();
    //doc["bmp_altitude"] = bmp.readAltitude();
    //doc["bmp_sealevel_pressure"] = bmp.readSealevelPressure();

    String jsonString;
    serializeJson(doc, jsonString);
    int httpResponseCode = http.POST(jsonString);

    if (httpResponseCode > 0) {
      Serial.println("HTTP Response code: " + String(httpResponseCode));
    } else {
      Serial.println("Error on sending POST: " + String(httpResponseCode));
    }

    http.end();
  }
}
void sendThermalData() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(thermalServerUrl);
    http.addHeader("Content-Type", "application/json");

    StaticJsonDocument<512> doc;
    
    // Update the thermistor temperature and pixel matrix
    amg8833_sensor.updateThermistorTemperature();
    amg8833_sensor.updatePixelMatrix();

    // Add thermistor temperature to JSON document
    doc["thermistor_temp"] = amg8833_sensor.thermistorTemperature;

    // Create a nested array for pixel data
    JsonArray pixels = doc.createNestedArray("pixels");
    for (int x = 0; x < 8; x++) {
      for (int y = 0; y < 8; y++) {
        pixels.add(amg8833_sensor.pixelMatrix[y][x]);
      }
    }

    // Serialize JSON document to string
    String jsonString;
    serializeJson(doc, jsonString);

    // Send the POST request
    int httpResponseCode = http.POST(jsonString);

    // Print HTTP response code
    if (httpResponseCode > 0) {
      Serial.println("HTTP Response code: " + String(httpResponseCode));
    } else {
      Serial.println("Error on sending POST: " + String(httpResponseCode));
    }

    // End HTTP connection
    http.end();
  }
}


/*void moveCar() {
  if (PS4.Up()) {
    CAR_moveForward();
  } else if (PS4.Right()) {
    CAR_turnRight();
  } else if (PS4.Down()) {
    CAR_moveBackward();
  } else if (PS4.Left()) {
    CAR_turnLeft();
  } else {
    CAR_stop();
  }
}*/

void CAR_moveForward() {
  digitalWrite(IN1_PIN, HIGH);
  digitalWrite(IN2_PIN, LOW);
  digitalWrite(IN3_PIN, HIGH);
  digitalWrite(IN4_PIN, LOW);
}

void CAR_moveBackward() {
  digitalWrite(IN1_PIN, LOW);
  digitalWrite(IN2_PIN, HIGH);
  digitalWrite(IN3_PIN, LOW);
  digitalWrite(IN4_PIN, HIGH);
}

void CAR_turnLeft() {
  digitalWrite(IN1_PIN, HIGH);
  digitalWrite(IN2_PIN, LOW);
  digitalWrite(IN3_PIN, LOW);
  digitalWrite(IN4_PIN, HIGH);
}

void CAR_turnRight() {
  digitalWrite(IN1_PIN, LOW);
  digitalWrite(IN2_PIN, HIGH);
  digitalWrite(IN3_PIN, HIGH);
  digitalWrite(IN4_PIN, LOW);
}

void CAR_stop() {
  digitalWrite(IN1_PIN, LOW);
  digitalWrite(IN2_PIN, LOW);
  digitalWrite(IN3_PIN, LOW);
  digitalWrite(IN4_PIN, LOW);
}