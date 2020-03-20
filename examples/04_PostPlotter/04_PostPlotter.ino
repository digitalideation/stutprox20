// Shows how to run two Steppers at once with varying speeds
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
boolean triggerPen = false;

Servo myservo;  // create servo object to control a servo

int val;

Adafruit_MotorShield AFMStop(0x60); // Default address, no jumpers

// Connect two steppers with 200 steps per revolution (1.8 degree)
// to the top shield
Adafruit_StepperMotor *myStepperRight = AFMStop.getStepper(200, 1);
Adafruit_StepperMotor *myStepperLeft = AFMStop.getStepper(200, 2);

// you can change these to DOUBLE or INTERLEAVE or MICROSTEP!
// wrappers for the first motor!
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

  stepperLeft.setMaxSpeed(2.0);
  stepperLeft.setAcceleration(200.0);
  stepperLeft.moveTo(900);

  stepperRight.setMaxSpeed(10.0);
  stepperRight.setAcceleration(200.0);
  stepperRight.moveTo(100000);

  Serial.begin(115200);
  pinMode(endStopTop, INPUT_PULLUP);
  pinMode(endStopBottom, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(endStopBottom), stopStepperLeft, RISING);
  attachInterrupt(digitalPinToInterrupt(endStopTop), stopStepperRight, RISING);

  penUp();
  penDown();
}

void loop()
{
  // Change direction at the limits
  //stepperLeft.moveTo();

  if (endSwitch == false) {
    stepperLeft.run();
    stepperRight.run();
  }
  Serial.println(stepperLeft.currentPosition() % 25);
  if (stepperLeft.currentPosition() % 25 == 0) {
    triggerPen = !triggerPen;
    if (triggerPen) {
      //penUp();
      Serial.println("pen up");
    }
    else {
      Serial.println("pen down");
      //penDown();
    }
  }
}

void penDown() {
  myservo.write(30);
}
void penUp() {
  myservo.write(150);
}

void stopStepperLeft() {
  Serial.println("endswitch bottom");
  endSwitch = true;
  stepperLeft.stop();
  Serial.println(endSwitch);
  penUp();
}

void stopStepperRight() {
  Serial.println("endswitch top");
  endSwitch = true;
  stepperLeft.stop();
  Serial.println(endSwitch);
  penUp();
}
