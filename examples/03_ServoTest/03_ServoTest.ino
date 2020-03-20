/*
 Controlling a servo position using a potentiometer (variable resistor)
 by Michal Rinott <http://people.interaction-ivrea.it/m.rinott>

 modified on 8 Nov 2013
 by Scott Fitzgerald
 http://www.arduino.cc/en/Tutorial/Knob
*/

#include <Servo.h>

Servo myservo;  // create servo object to control a servo

int potpin = 0;  // analog pin used to connect the potentiometer
int val = 150;    // variable to read the value from the analog pin

void setup() {
  Serial.begin(115200);
  myservo.attach(10);  // attaches the servo on pin 9 to the servo object
  //myservo.write(0);   
}

void loop() {
  // myservo.write(90);
  Serial.println(val);
  val += 0;    // scale it to use it with the servo (value between 0 and 180)
  myservo.write(val);                  // sets the servo position according to the scaled value
  delay(15);                           // waits for the servo to get there
 if (val % 180 == 0) val = 0;
}
