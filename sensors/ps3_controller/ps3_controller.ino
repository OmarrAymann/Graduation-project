#include <Ps3Controller.h>
int leftX;
int leftY;
int rightX;
int rightY;
void notify()
{
    if( Ps3.data.button.cross ){
        Serial.println("Pressing the cross button");
    }

    if( Ps3.data.button.square ){
        Serial.println("Pressing the square button");
    }

    if( Ps3.data.button.triangle ){
        Serial.println("Pressing the triangle button");
    }

    if( Ps3.data.button.circle ){
        Serial.println("Pressing the circle button");
    }
    if( Ps3.data.button.up ){
        Serial.println("Pressing the up button");
    }
    if( Ps3.data.button.right ){
        Serial.println("Pressing the right button");
    }
    if( Ps3.data.button.down ){
        Serial.println("Pressing the down button");
    }
    if( Ps3.data.button.left ){
        Serial.println("Pressing the left button");
    }
    leftX = (Ps3.data.analog.stick.lx);
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
    Serial.println(rightY);
}

void onConnect(){
  Serial.println("Connected!.");
}

void setup()
{
    Serial.begin(115200);
    Ps3.attach(notify);
    Ps3.attachOnConnect(onConnect);
    Ps3.begin("00:26:5C:12:DE:1E");
    Serial.println("Ready.");
}

void loop()
{
}
