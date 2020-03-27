# Studio Prototyping Extended 2020

March 2020: This modul started in the first week after the Covid-19 lockdown in Switzerland. Hence, workshop is done entirely via remote-channels. 

## Installation

### Hardware setup

We are using the [Arduino Shield V2 motorshield](https://github.com/adafruit/Adafruit_Motor_Shield_V2_Library).

### Software install

Either to manual install by downloading the zip file and put it into the library folder of your Arduino installation. Otherwise use the library manager directly from the Arduino IDE. Make sure that following libraries are installed:

  - [Adafruit Motor Shield Rev2 Library](https://github.com/adafruit/Adafruit_Motor_Shield_V2_Library)
  - [Accelstepper](http://www.airspayce.com/mikem/arduino/AccelStepper/)

I am not using the Arduino IDE for development but instead [Visual Studio Code](https://code.visualstudio.com/) with the [Arduino extension](https://marketplace.visualstudio.com/items?itemName=vsciot-vscode.vscode-arduino). You can upload your sketches directly from your text editor.

## Steppers

[Halfstep, double, and microstepping?](https://www.rs-online.com/designspark/stepper-motors-and-drives-what-is-full-step-half-step-and-microstepping)

## Servos

  - [Video "germainsplaining" a servo:](https://www.youtube.com/watch?v=yQMcr3PNxV8&feature=youtu.be)
  - Further instructions in Ilias Modulunterlagen
  
## External interrupts 

Read this [excellent guide](https://www.allaboutcircuits.com/technical-articles/using-interrupts-on-arduino/) on why we need external interrupts. The UNO board we are using has two external interrupts on pin 2 and 3. The official Arduino website also has good documentation on how to use [interrupt service routines](https://www.arduino.cc/reference/en/language/functions/external-interrupts/attachinterrupt/).

But those external interrupts are only working with digital input signals, i. e. buttons.

If you want to read an analog sensor and trigger the external interrupt once a certain threshold is reached, you'll have to work with analog comparator interrupts. An easy tutorial can be found [here](https://github.com/tardate/LittleArduinoProjects/tree/master/playground/AnalogComparator).

The reference voltage in the example above is created with a simple voltage divider, but you can also just use a PWM output from the Arduino to generate the desired reference voltage.

There's also [library](https://github.com/leomil72/analogComp) for Analog Comparator if none of the above satisfies you. 



