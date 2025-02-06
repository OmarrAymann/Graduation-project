#include <DHT.h>

#define DHTPIN 32     // Digital pin connected to the DHT sensor
// Feather HUZZAH ESP8266 note: use pins 3, 4, 5, 12, 13 or 14 --
// Pin 15 can work but DHT must be disconnected during program upload.

// Uncomment whatever type you're using!
#define DHTTYPE DHT11   // DHT 11
//#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
//#define DHTTYPE DHT21   // DHT 21 (AM2301)
DHT dht(DHTPIN, DHTTYPE);

#define MQ2pin 34
#define Microwave_Pin 35               // choose the input pin (for Radar sensor)

int motionState = LOW;          // we start, assuming no motion detected
int val = 0;                    // variable for reading the pin status

int sensorValue;  //variable to store sensor value

void setup() {
	Serial.begin(115200); // sets the serial port to 9600
	Serial.println("MQ2 warming up!");\
  pinMode(MQ2pin, INPUT);
  pinMode(Microwave_Pin, INPUT);     // declare sensor as input
  dht.begin();
	//delay(20000); // allow the MQ2 to warm up
}
void loop() {
  microwave();
  mq2();
  DHT();
 delay(200);
}

void microwave() {
  val = digitalRead(Microwave_Pin);  // read input value
  
  if (val == HIGH) {  // check if the input is HIGH
    //if (motionState == LOW) {
      Serial.println("Motion active!"); // print on output change
      //motionState = HIGH;
    //}
  }
  else {
    //if (motionState == HIGH) {
      Serial.println("No Motion !");  // print on output change
      //motionState = LOW;
    //}
  }
}

void mq2() {
	sensorValue = digitalRead(MQ2pin); // read digital output pin
	Serial.print("Digital Output: ");
	Serial.print(sensorValue);
	
	// Determine the status
	if (sensorValue ==1) {
		Serial.println("  |  Smoke: -");
	} else {
		Serial.println("  |  Smoke: Detected!");
	}
	//delay(500); // wait 2s for next reading
}
void DHT() {
  // Wait a few seconds between measurements.
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  // Compute heat index in Fahrenheit (the default)
  float hif = dht.computeHeatIndex(f, h);
  // Compute heat index in Celsius (isFahreheit = false)
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