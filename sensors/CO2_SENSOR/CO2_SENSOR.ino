#define MQ2pin 34

int sensorValue;  //variable to store sensor value

void setup() {
	Serial.begin(115200); // sets the serial port to 9600
	Serial.println("MQ2 warming up!");\
  pinMode(MQ2pin, INPUT);
	//delay(20000); // allow the MQ2 to warm up
}

void loop() {
	sensorValue = digitalRead(MQ2pin); // read digital output pin
	Serial.print("Digital Output: ");
	Serial.print(sensorValue);
	
	// Determine the status
	if (sensorValue ==1) {
		Serial.println("  |  Smoke: -");
	} else {
		Serial.println("  |  Smoke: Detected!");
	}
	
	delay(500); // wait 2s for next reading
}