/*
  Moves the Servo attached to pin 10 from 0 to 180 degrees and backwards
  Gordan Savicic, March 2020
*/

#include <Servo.h>

Servo myservo;  // create servo object to control a servo

int val = 0;
int counter = 1;

void setup() {
  Serial.begin(115200);
  myservo.attach(10);  // attaches the servo on pin 10
}

void loop() {
  val += counter;
  Serial.println(val);
  myservo.write(val); // sets the servo position according to the scaled value
  delay(15);  // waits for the servo to get there
  if (val > 179 || val < 1) counter = -counter;
}
