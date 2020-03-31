import processing.serial.*;          // imports serial library
import controlP5.*;                  // imports controlp5 library

// create instances for classes we are going to use
ControlP5 cp5;
Slider Stepper1;
Slider Stepper2;
CheckBox servo;

// Create instance from Serial class
Serial myPort;  

// Some variables
float stepperLeftValue;
float stepperRightValue;

float stepperLeftMax = 880;
float stepperRightMax = 2750 ;

boolean servoPos = false;
String inByte = "";    // Incoming serial data

void setup() 
{
  size(950, 500);

  cp5 = new ControlP5(this);                               

  // setup two sliders
  Stepper1 = cp5.addSlider("stepperLeft").setRange(0,stepperLeftMax ).setValue(0).setPosition(110, 174).setSize(270, 25).setNumberOfTickMarks(270).setLabelVisible(true).setColorForeground(color(102, 102, 102)).setColorBackground(color(255, 153, 0)).setColorActive(color(102, 102, 102));
  Stepper2 = cp5.addSlider("stepperRight").setRange(0, stepperRightMax).setValue(0).setPosition(110, 224).setSize(270, 25).setNumberOfTickMarks(270).setLabelVisible(true).setColorForeground(color(102, 102, 102)).setColorBackground(color(255, 153, 0)).setColorActive(color(102, 102, 102));

  // setup checkbox for servo
  servo = cp5.addCheckBox("checkBox")
    .setPosition(110, 110)
    .setSize(40, 40)
    .setItemsPerRow(3)
    .setSpacingColumn(30)
    .setSpacingRow(20)
    .setColorForeground(color(102, 102, 102))
    .setColorBackground(color(255, 153, 0))
    .setColorActive(color(102, 102, 102))
    .addItem("pen down", 0);

  // List all serial ports available   
  for (int i = 0; i<Serial.list().length; i++) {
    println("["+i+"]"+Serial.list()[i]);
  }

  // check your serial port from the list -- CHANGE NUMBER ACCORDING TO YOUR SERIAL PORT 
  String portName = Serial.list()[3];

  // make sure your Arduino sketch has the same baud rate 115200!
  myPort = new Serial(this, portName, 115200);
}  

void draw() 
{
  background(color(22, 22, 22));  
  
  // only send values to serial port if they've changed
  if (stepperLeftValue != Stepper1.getValue()) {
    myPort.write("L"+  Stepper1.getValue()+"\r\n");
  }
  if (stepperRightValue != Stepper2.getValue()) {
    myPort.write("R"+  Stepper2.getValue()+"\r\n");
  }
  if (servoPos != servo.getState(0)) {
    if (servoPos == true) myPort.write("u\r\n");
    if (servoPos == false) myPort.write("d\r\n");
  }
  stepperLeftValue = Stepper1.getValue();
  stepperRightValue = Stepper2.getValue();
  servoPos = servo.getState(0);

  text("Received from Arduino: " + inByte, 110, 430);
}

// Function is being called if there's a new Serial event
void serialEvent(Serial myPort) {
  inByte = myPort.readStringUntil(10);
}
