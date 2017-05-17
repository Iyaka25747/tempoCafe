// Temporisateur Machine à Café et thé et sucre

#include "Arduino.h"
#include "TM1637Display.h"

#define PIN_INPUT 0
#define PIN_OUTPUT 1
#define LOW 0
#define HIGH 1
#define RELAY_PIN 6 // Command of relay
#define Button_PIN 5 // Button read

/*
HEX Display, measure and temperature setpoint
*/
//Measured temperature
const int CLK = 3; //Set the CLK pin connection to the display
const int DIO1 = 2; //Set the DIO pin connection to the display
TM1637Display displayCountDown(CLK, DIO1); //set up the 4-Digit Display1.



//The setup function is called once at startup of the sketch
void setup()
{
	pinMode(RELAY_PIN, PIN_OUTPUT);	// set relay pin to output
	pinMode(Button_PIN, PIN_INPUT);// set button pin input
	displayCountDown.setBrightness(0x0a);//set the display to maximum brightness
	Serial.begin(9600);			// initialize serial communications at 9600 bps:

}

// The loop function is called in an endless loop
void loop()
{
//Add your repeated code here
}
