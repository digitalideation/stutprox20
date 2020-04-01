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

// added new line
// Some aditional libraries
#include <Wire.h>
#include <Adafruit_MotorShield.h>

// global variables - mechanis
long positionL=0; // remember the position of stepper L
long positionR=0; // remember the position of stepper R
long loops=0; // remember number of loops
const int maxL = 240; // some simple safety to not crash the left Stepper

// global variables for general controll / "stop the sketch"
bool moveMotors = true;

// global variables - pattern
int p_ratio = 30; // ratio between steps for R and L - smaller value result in flatter spiral - ch



// the software-Side of the Motorshield, that uses ic2 to communicate 
Adafruit_MotorShield AFMStop(0x60); // Default address, no jumpers

// Connect two steppers with 200 steps per revolution (1.8 degree)
// to the top shield
Adafruit_StepperMotor *adafStepperL = AFMStop.getStepper(200, 2); // adafruit Stepper Left M3 & M4 
Adafruit_StepperMotor *adafStepperR = AFMStop.getStepper(200, 1); // adafruit Stepper Right M1 & M2

// some more easy functions to run each stepper one step, include some safety to not run over maxL or below 0
#define stepping DOUBLE
// you can change these to DOUBLE or INTERLEAVE or MICROSTEP!
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
  // skip all stuff in the loop if the pattern is finished..
  if (!moveMotors) return;

  // the logic of the pattern
  // RIGHT MOTOR;
  // do a step with the right Motor - turn the postcard-table continous
  stepRfw();
  // LEFT MOTOR:
  // do a step with the left Motor - but less often
  // change p_ratio at the top of the sketch to change
  if (loops % p_ratio == 0)
  {
    stepLfw();
    // stop the motors at the boarder of the postcard
    if (positionL >= maxL)
    {
       stopMotors();
    }
  }
  //
  // count the loops
  loops++;
  // don t move to fast... 
  delay(5); // not less then 5, otherwise the software is faster then the real world mechanics
}
