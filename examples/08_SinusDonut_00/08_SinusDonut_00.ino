// Shows how to run two Steppers at once with varying speeds
//
// Requires the Adafruit_Motorshield v2 library
//   https://github.com/adafruit/Adafruit_Motor_Shield_V2_Library

/* ATTENTION **********************************
 *
 * before you run this sketch, make sure the left Motor is wired currectly, moving from 0 Forward
 * test this by starting the sketch first tim with a middle position for the left motor
 * 
 * 
*/

// Some aditional libraries
#include <Wire.h>
#include <Adafruit_MotorShield.h>

// global variables - mechanis
long positionL=0; // remember the position of stepper L
long positionR=0; // remember the position of stepper R
long loops=0; // remember number of loops
const int maxL = 230; // some simple safety to not crash the left Stepper
const int oneTurnR = 1400; // steps to rotate the postcard-table

// global variables for general controll / "stop the sketch"
bool moveMotors = true;



// the software-Side of the Motorshield, that uses ic2 to communicate 
Adafruit_MotorShield AFMStop(0x60); // Default address, no jumpers

// Connect two steppers with 200 steps per revolution (1.8 degree)
// to the top shield
Adafruit_StepperMotor *adafStepperL = AFMStop.getStepper(200, 2); // adafruit Stepper Left
Adafruit_StepperMotor *adafStepperR = AFMStop.getStepper(200, 1); // adafruit Stepper Right

// some more easy functions to run each stepper one step, include some safety to not run over maxL or below 0
#define stepping DOUBLE
// you can change these to DOUBLE or INTERLEAVE or MICROSTEP!
// wrappers for the first motor!
// stepper Left Fore Ward
void stepLfw() {
  // make sure L doesn t move to high
  if (positionL <= maxL && moveMotors)
  {
    adafStepperL->onestep(FORWARD, stepping);
    positionL++;
  }
}
// stepper Left Back Ward
void stepLbw() {
  // make sure L doesn t move to low
  if (positionL >= 0 && moveMotors)
  {
    adafStepperL->onestep(BACKWARD, stepping);
    positionL--;
  }  
}

// wrappers for the second motor!
// stepper Right Fore Ward
void stepRfw() {
  // no safety question for the position - rotates endless
  if (moveMotors)
  {
    adafStepperR->onestep(FORWARD, stepping);
    positionR++;
  }
}
// stepper Right Back Ward
void stepRbw() {
  // no safety question for the position - rotates endless
  if (moveMotors)
  {
    adafStepperR->onestep(BACKWARD, stepping);
    positionR--;
  }
}

void stopMotors()
{
  moveMotors = false;
  adafStepperR->release();
  adafStepperL->release();
}

void setup()
{
  Serial.begin(9600);
  // manual - homeing - see note at top, check Motor-Direction at first run / middle position
  Serial.println("Please Set Plotter to 0-0");
  // wait one second for example to turn on the power supply of the motor
  delay(2000);
  AFMStop.begin(); // Start the top shield
}

void loop()
{
  // *****(1)*****check for stop
  // arduion has no stop, end of sketch so we have to return the loop already here...
  // skip all stuff in the loop if the pattern is finished..
  if (!moveMotors) return;
  // *****(2-0)*****
  // the logic of the pattern
  // *****(2-1)*****
  // RIGHT MOTOR ****************
  // do a step with the right Motor - turn the postcard-table continous
  stepRfw();
  // *****(2-2)*****
  // LEFT MOTOR CALCULATION
  // do some fancy calculation for the second axis
  // some sinus movement - the formular could be in one line ... but let s do it step by step:
  //    first calculate value inside the sin() function
  //    in this example do 3.02 sin-waves per turn of Postcard 
  float x = (PI * 2.0 * loops * 3.02) /oneTurnR; // you re very free to change this 
  //
  // calculate a factor that is between 0 .. 1
  float factor = (sin(x) + 1) *0.5;
  float lminf = 0.0; // also nice: 0.5; // minimal percentage - change between 0..1
  //old version keep maximum constant
  //float lmaxf = 1.0; // maximum percentage - change between 0..1
  //new version decrease lmaxf after each turn
  float lmaxf = 1.0 - floor(loops / oneTurnR) * 0.025;
  // take lower and higher value into account
  factor = lminf + factor * (lmaxf - lminf);
  // the position we would love to go to...
  int targetL = factor * maxL; // this will truncate the float to an int
  // Serial.println(targetL); // some supervision 
  //
  // *****(2-3)*****
  // END OF PATTERN TEST
  // some simple test if we have to stop
  if (lmaxf < 0.05)
  {
    stopMotors();
    return;
  }
  // *****(2-4)*****
  // LEFT MOTOR CALCULATION
  // only do steps if position does not fit
  // this might cause unexpected behavior for very small p_ratio values
  if (positionL < targetL)
  {
    stepLfw();
  }
  if (positionL > targetL)
  {
    stepLbw();
  }
  // don t move to fast... 
  delay(3); // not less then 3, otherwise the software is faster then the real world mechanics

  // *****(2-5)*****
  // COUNT THE LOOPS
  loops++;
  
}

// if you do not like so many comments as above... same stuff, less comments:
void loopNoComments()
{
  if (!moveMotors) return;
  // RIGHT MOTOR continous steps
  stepRfw();
  // *****(2-2)*****
  // LEFT MOTOR
  float x = (PI * 2.0 * loops * 3.02) /oneTurnR; // inside sin() map 3.02 sin waves to oneTurnR
  float factor = (sin(x) + 1) *0.5; // sin mapped to 0..1
  float lminf = 0.0; // also nice: 0.5; // minimal percentage - change between 0..1
  float lmaxf = 1.0 - floor(loops / oneTurnR) * 0.025; // getting smaller each turn
  factor = lminf + factor * (lmaxf - lminf);
  int targetL = factor * maxL; // this will truncate the float to an int
  // end of pattern ?
  if (lmaxf < 0.05)
  {
    stopMotors();
    return;
  }
  // Left Motor stepping
  if (positionL < targetL)
  {
    stepLfw();
  }
  if (positionL > targetL)
  {
    stepLbw();
  }
  // don t move to fast... 
  delay(3); // not less then 3, otherwise the software is faster then the real world mechanics
  loops++;
}
