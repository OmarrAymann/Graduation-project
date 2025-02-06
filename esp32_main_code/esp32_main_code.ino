#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <DHT.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <Adafruit_BMP085.h>
//#include <QMC5883LCompass.h>
#include <Melopero_AMG8833.h>
#include <PS4Controller.h>

#define IN1_PIN 23  // The ESP32 pin GPIO27 connected to the IN1 pin L298N
#define IN2_PIN 19  // The ESP32 pin GPIO26 connected to the IN2 pin L298N
#define IN3_PIN 18  // The ESP32 pin GPIO25 connected to the IN3 pin L298N
#define IN4_PIN 4   // The ESP32 pin GPIO33 connected to the IN4 pin L2

// WiFi credentials
const char* ssid = "Ghuzy";
const char* password = "01066709440@AHMAG";
const char* serverUrl = "http://192.168.1.5:5000/data/ESP32_2";

// DHT11 sensor
#define DHTPIN 25
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// MQ2 and Microwave sensors
#define MQ2pin 35
#define Microwave_Pin 34

// IMU sensors
Adafruit_MPU6050 mpu;
Adafruit_BMP085 bmp;
//QMC5883LCompass compass;

// AMG8833 sensor
Melopero_AMG8833 amg8833_sensor;

int motionState = LOW;
int val = 0;
int sensorValue;

// Timing variables
unsigned long previousMillis = 0;
const long interval = 200;  // Interval for sending data (in milliseconds)

void setup() {
  Serial.begin(115200);
  delay(10);
  pinMode(IN1_PIN, OUTPUT);
  pinMode(IN2_PIN, OUTPUT);
  pinMode(IN3_PIN, OUTPUT);
  pinMode(IN4_PIN, OUTPUT);
  PS4.begin("1c:e6:1d:87:6d:37");
  Serial.println("Ready.");
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

  // Initialize sensors
  pinMode(MQ2pin, INPUT);
  pinMode(Microwave_Pin, INPUT);
  dht.begin();

  initializeMPU6050();
  mpu.setI2CBypass(true);
  //initializeQMC5883L();
  initializeBMP180();

  // Initialize AMG8833 sensor
  Wire.begin();
  amg8833_sensor.initI2C();

  Serial.print("Resetting AMG8833 sensor ... ");
  int statusCode = amg8833_sensor.resetFlagsAndSettings();
  Serial.println(amg8833_sensor.getErrorDescription(statusCode));

  Serial.print("Setting FPS for AMG8833 ... ");
  statusCode = amg8833_sensor.setFPSMode(FPS_MODE::FPS_10);
  Serial.println(amg8833_sensor.getErrorDescription(statusCode));
}

void loop() {
  microwave();
  mq2();
  DHT();
  move_car();
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    if (WiFi.status() == WL_CONNECTED) {
      HTTPClient http;
      http.begin(serverUrl);
      http.addHeader("Content-Type", "application/json");

      StaticJsonDocument<1000> doc;
      float temperature = dht.readTemperature();
      float humidity = dht.readHumidity();
      int motion = digitalRead(Microwave_Pin);
      int smoke = digitalRead(MQ2pin);

      sensors_event_t a, g, temp;
      mpu.getEvent(&a, &g, &temp);

      doc["temperature"] = temperature;
      doc["humidity"] = humidity;
      doc["motion"] = (motion == HIGH) ? "active" : "inactive";
      doc["smoke"] = (smoke == HIGH) ? "clear" : "detected";
      doc["status"] = "OK";

      doc["acceleration_x"] = a.acceleration.x;
      doc["acceleration_y"] = a.acceleration.y;
      doc["acceleration_z"] = a.acceleration.z;
      doc["gyro_x"] = g.gyro.x;
      doc["gyro_y"] = g.gyro.y;
      doc["gyro_z"] = g.gyro.z;
      //doc["mpu_temp"] = temp.temperature;

      /*float qmc_x, qmc_y, qmc_z, qmc_a;
      compass.read();
      qmc_x = compass.getX();
      qmc_y = compass.getY();
      qmc_z = compass.getZ();
      qmc_a = compass.getAzimuth();
      doc["qmc_x"] = qmc_x;
      doc["qmc_y"] = qmc_y;
      doc["qmc_z"] = qmc_z;
      doc["qmc_azimuth"] = qmc_a;*/

      //doc["bmp_temp"] = bmp.readTemperature();
      doc["bmp_pressure"] = bmp.readPressure();
      //doc["bmp_altitude"] = bmp.readAltitude();
      //doc["bmp_sealevel_pressure"] = bmp.readSealevelPressure();

      amg8833_sensor.updateThermistorTemperature();
      amg8833_sensor.updatePixelMatrix();
      doc["amg8833_thermistor_temp"] = amg8833_sensor.thermistorTemperature;

      for (int x = 0; x < 8; x++) {
        for (int y = 0; y < 8; y++) {
          String key = "amg8833_pixel_" + String(x) + "_" + String(y);
          doc[key] = amg8833_sensor.pixelMatrix[y][x];
        }
      }

      String jsonString;
      serializeJson(doc, jsonString);

      int httpResponseCode = http.POST(jsonString);

      if (httpResponseCode > 0) {
        String response = http.getString();
        Serial.println("HTTP Response code: " + String(httpResponseCode));
        Serial.println("Response: " + response);
      } else {
        Serial.println("Error on sending POST: " + String(httpResponseCode));
      }

      http.end();
    }
  }
}

void microwave() {
  val = digitalRead(Microwave_Pin);
  if (val == HIGH) {
    Serial.println("Motion active!");
  } else {
    Serial.println("No Motion!");
  }
}

void mq2() {
  sensorValue = digitalRead(MQ2pin);
  Serial.print("Digital Output: ");
  Serial.print(sensorValue);

  if (sensorValue == HIGH) {
    Serial.println("  |  Smoke: -");
  } else {
    Serial.println("  |  Smoke: Detected!");
  }
}

void DHT() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  float f = dht.readTemperature(true);

  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  float hif = dht.computeHeatIndex(f, h);
  float hic = dht.computeHeatIndex(t, h, false);

  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.print(F("°C "));
  Serial.print(f);
  Serial.print(F("°F  Heat index: "));
  Serial.print(hic);
  Serial.print(F("°C "));
  Serial.print(hif);
  Serial.println(F("°F"));
}

/*void initializeQMC5883L() {
  compass.init();
  compass.setCalibrationOffsets(-336.00, -179.00, 47.00);
  compass.setCalibrationScales(1.05, 0.94, 1.02);
}*/

void initializeMPU6050() {
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1)
      ;
  }
  Serial.println("MPU6050 Found!");

  mpu.setAccelerometerRange(MPU6050_RANGE_16_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
}

void initializeBMP180() {
  if (!bmp.begin()) {
    Serial.println("Could not find a valid BMP180 sensor, check wiring!");
    while (1)
      ;
  }
  Serial.println("BMP180 Found!");
}
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

void move_car() {
  if (PS4.Cross()) {
    Serial.println("Pressing the cross button");
  } else if (PS4.Square()) {
    Serial.println("Pressing the square button");
  }

  else if (PS4.Triangle()) {
    Serial.println("Pressing the triangle button");
  }

  else if (PS4.Circle()) {
    Serial.println("Pressing the circle button");
  } else if (PS4.Up()) {
    Serial.println("Pressing the up button");
    CAR_moveForward();
  } else if (PS4.Right()) {
    Serial.println("Pressing the right button");
    CAR_turnRight();
  } else if (PS4.Down()) {
    Serial.println("Pressing the down button");
    CAR_moveBackward();
  } else if (PS4.Left()) {
    Serial.println("Pressing the left button");
    CAR_turnLeft();
  } else {
    CAR_stop();
  }
}
