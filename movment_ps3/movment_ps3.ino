#include <PS4Controller.h>

#define IN1_PIN 23  // The ESP32 pin GPIO27 connected to the IN1 pin L298N
#define IN2_PIN 19  // The ESP32 pin GPIO26 connected to the IN2 pin L298N
#define IN3_PIN 18  // The ESP32 pin GPIO25 connected to the IN3 pin L298N
#define IN4_PIN 4  // The ESP32 pin GPIO33 connected to the IN4 pin L298N

void move_car()
{
    if( PS4.Cross() ){
        Serial.println("Pressing the cross button");
    }
    else if( PS4.Square() ){
        Serial.println("Pressing the square button");
    }

    else if( PS4.Triangle() ){
        Serial.println("Pressing the triangle button");
    }

    else if( PS4.Circle() ){
        Serial.println("Pressing the circle button");
    }
    else if( PS4.Up() ){
        Serial.println("Pressing the up button");
        CAR_moveForward();
    }
    else if( PS4.Right() ){
        Serial.println("Pressing the right button");
        CAR_turnRight();
    }
    else if( PS4.Down() ){
        Serial.println("Pressing the down button");
        CAR_moveBackward();
    }
    else if( PS4.Left() ){
        Serial.println("Pressing the left button");
        CAR_turnLeft();
    }
    else{
    CAR_stop();
    }
}

void setup()
{
    Serial.begin(115200);
    pinMode(IN1_PIN, OUTPUT);
    pinMode(IN2_PIN, OUTPUT);
    pinMode(IN3_PIN, OUTPUT);
    pinMode(IN4_PIN, OUTPUT);
    PS4.begin("1c:e6:1d:87:6d:37");
    Serial.println("Ready.");
}

void loop()
{
  move_car();
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

