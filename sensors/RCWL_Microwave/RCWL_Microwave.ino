int inputPin = 26;               // choose the input pin (for Radar sensor)
int motionState = LOW;          // we start, assuming no motion detected
int val = 0;                    // variable for reading the pin status
 
void setup() {
  pinMode(inputPin, INPUT);     // declare sensor as input
 
  Serial.begin(115200);
}
 
void loop() {
  val = digitalRead(inputPin);  // read input value
  
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