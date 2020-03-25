// Homing stepper motor with end switches using external interrupts
// Studio Prototying Extended, March 2020 
//
// Read more about external interrupts here: https://www.arduino.cc/reference/en/language/functions/external-interrupts/attachinterrupt/?setlang=it
//
// Requires the Adafruit_Motorshield v2 library
//   https://github.com/adafruit/Adafruit_Motor_Shield_V2_Library
// And AccelStepper with AFMotor support
//   https://github.com/adafruit/AccelStepper

#include <Wire.h>
#include <AccelStepper.h>
#include <Adafruit_MotorShield.h>
#include <Servo.h>

const byte endStopTop = 3;
const byte endStopBottom = 2;

boolean endSwitch = false;

long initialHome = -1;

float stepperLeftSpeed = 150.0;
float stepperRightSpeed = 150.0;
float stepperLeftAcceleration = 500.0;
float stepperRightAcceleration = 500.0;

int stepperLeftPosMax = 880;
int stepperLeftPosMin = 20;

Servo myservo;  // create servo object to control a servo

Adafruit_MotorShield AFMStop(0x60); // Default address, no jumpers

// Connect two steppers with 200 steps per revolution (1.8 degree)
// to the top shield
Adafruit_StepperMotor *myStepperRight = AFMStop.getStepper(200, 1);
Adafruit_StepperMotor *myStepperLeft = AFMStop.getStepper(200, 2);


// you can change these to SINGLE, DOUBLE, INTERLEAVE or MICROSTEP!
// wrappers for the both motors
#define stepping INTERLEAVE
void forwardstep1() {
  myStepperLeft->onestep(FORWARD, stepping);
}
void backwardstep1() {
  myStepperLeft->onestep(BACKWARD, stepping);
}
// wrappers for the second motor!
void forwardstep2() {
  myStepperRight->onestep(FORWARD, stepping);
}
void backwardstep2() {
  myStepperRight->onestep(BACKWARD, stepping);
}

AccelStepper stepperLeft(forwardstep1, backwardstep1);
AccelStepper stepperRight(forwardstep2, backwardstep2);

void setup()
{
  AFMStop.begin(); // Start the top shield

  myservo.attach(10);
  penUp();

  stepperLeft.setMaxSpeed(stepperLeftSpeed);
  stepperLeft.setAcceleration(stepperLeftAcceleration);
  //stepperLeft.moveTo(); // Schwenkarm 7250 steps mit MICROSTEP, 880 INTERLEAVE, ~420 SINGLE

  stepperRight.setMaxSpeed(stepperRightSpeed);
  stepperRight.setAcceleration(stepperRightAcceleration);
  //stepperRight.moveTo(2750); // 22400 for full circle MICROSTEP, 2750 INTERLEAVE, ~1350 SINGLE
  //stepperRight.moveTo(22400);

  Serial.begin(115200);

  pinMode(endStopTop, INPUT_PULLUP);
  pinMode(endStopBottom, INPUT_PULLUP);

  // External interrupts
  attachInterrupt(digitalPinToInterrupt(endStopBottom), stopStepperLeftBottom, FALLING);
  attachInterrupt(digitalPinToInterrupt(endStopTop), stopStepperLeftTop, FALLING);

  homingSteppers();

  penDown();
}

void loop()
{

  if (endSwitch == false) {
    stepperLeft.run();
    stepperRight.run();
  }

}

void penDown() {
  Serial.println("(I) Moving pen down ...");
  for (int pos = myservo.read(); pos >= 30; pos -= 2) {
    myservo.write(pos);
    delay(15);
  }
}


void penUp() {
  for (int pos = myservo.read(); pos <= 150; pos += 2) {
    myservo.write(pos);
    delay(15);
  }
  Serial.println("(I) Moving pen up ...");

}

void stopStepperLeftBottom() {
  Serial.println("(W) Endswitch bottom");
  endSwitch = true;
  stepperLeft.stop();
  penUp();
}

void stopStepperLeftTop() {
  Serial.println("(W) Endswitch top");
  endSwitch = true;
  stepperLeft.stop();
  penUp();
}

void homingSteppers() {
  Serial.println("(I) Homing stepperLeft ... ");
  while (digitalRead(endStopTop) == HIGH) {
    stepperLeft.moveTo(initialHome);
    initialHome--;
    stepperLeft.runSpeed();

    Serial.print("(I) Homing stepperLeft steps: ");
    Serial.println(stepperLeft.currentPosition());
  }
  Serial.println("(I) Homing completed");
  stepperLeft.setCurrentPosition(0);

  Serial.println("(I) Moving out of home: start");
  // move outside of Zero Home Position
  stepperLeft.moveTo(20);
  while (stepperLeft.distanceToGo() != 0) {
    stepperLeft.run();
  }
  Serial.println("(I) Moving out of home: done");
  Serial.println("(I) Setting stepperLeft new Zero-Position: done");
  stepperLeft.setCurrentPosition(0);
  stepperRight.setCurrentPosition(0);
  endSwitch = false;
}
