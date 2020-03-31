// Receives communication from a Processing sketch via Serial port
//
// Serial commands are:

// "L[0-9]" -> L and number of steps, for example: L100 (moves left stepper 100steps)
// "R[0-9]" -> R and number of steps, for example: R100 (moves right stepper 100steps)
// "u" -> servo pen up
// "d" -> servo pen down

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

String inputString = "";         // a String to hold incoming data
bool stringComplete = false;  // whether the string is complete

Servo myservo;  // create servo object to control a servo

Adafruit_MotorShield AFMStop(0x60); // Default address, no jumpers

// Connect two steppers with 200 steps per revolution (1.8 degree)
// to the top shield
Adafruit_StepperMotor *myStepperRight = AFMStop.getStepper(200, 1);
Adafruit_StepperMotor *myStepperLeft = AFMStop.getStepper(200, 2);

long initialHome = -1;

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
  penUp();

  stepperLeft.setMaxSpeed(50.0);
  stepperLeft.setAcceleration(20.0);
  //stepperLeft.moveTo(); // Schwenkarm 7250 steps mit MICROSTEP, 880 INTERLEAVE, ~420 SINGLE

  stepperRight.setMaxSpeed(1000.0);
  stepperRight.setAcceleration(1000.0);
  
  Serial.begin(115200);

  //  pinMode(endStopTop, INPUT_PULLUP);
  //  pinMode(endStopBottom, INPUT_PULLUP);
  //  attachInterrupt(digitalPinToInterrupt(endStopBottom), stopStepperLeft, FALLING);
  //  attachInterrupt(digitalPinToInterrupt(endStopTop), stopStepperRight, FALLING);

  //  Serial.println("Homing completed");
  //  stepperLeft.setCurrentPosition(0);
  //  stepperRight.setCurrentPosition(0);

}

void loop()
{

  if (stepperLeft.distanceToGo() != 0 || stepperRight.distanceToGo() != 0) {
    stepperLeft.run();
    stepperRight.run();
  }

  if (stringComplete) {
    Serial.println(inputString);
    // clear the string:
    // \r return character ASCII 13 and \n newline character ASCII 10
    if (inputString == "u\r\n") {
      penUp();
    }
    else if (inputString == "d\r\n") {
      penDown();
    }
    // is character L part of the string?
    if (inputString.indexOf("L") != -1) {
      // split the number from string
      int motorPos = inputString.substring(1,inputString.length()).toInt();
      Serial.print("move left motor: " );
      Serial.println(motorPos);
      stepperLeft.moveTo(motorPos);
    }
    if (inputString.indexOf("R") != -1) {
      int motorPos = inputString.substring(1,inputString.length()).toInt();
      Serial.print("move right motor: " );
      Serial.println(motorPos);
      stepperRight.moveTo(motorPos);
    }

    inputString = "";
    stringComplete = false;
  }
}

void penDown() {
  Serial.println("Moving pen down ...");
  for (int pos = myservo.read(); pos >= 30; pos -= 2) {
    myservo.write(pos);
    delay(10);
  }
}


void penUp() {
  for (int pos = myservo.read(); pos <= 150; pos += 2) {
    myservo.write(pos);
    delay(10);
  }
  Serial.println("Moving pen up ...");

}

void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag so the main loop can
    // do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    }
  }
}
