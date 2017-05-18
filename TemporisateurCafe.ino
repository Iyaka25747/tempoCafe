// Temporisateur Machine à Café

#include "Arduino.h"
#include "TM1637Display.h"

/*
Hardware Pin declaration
*/

#define PIN_INPUT 0
#define PIN_OUTPUT 1
#define LOW 0
#define HIGH 1
int RELAY_PIN = 6;// Command of relay
int Button_PIN = 5; // Button read
int DispClkPin = 3; // TMS163 Display
int DispDataPin = 2; // TMS163 Display
// Improvement ?: Add potentiometer read to adjust timing

const long countDownDuration = 5000; //CountDown duration in millis

/*
 HEX Display countdown
 */
//Measured temperature
const int CLK = DispClkPin; //Set the CLK pin connection to the display
const int DIO1 = DispDataPin; //Set the DIO pin connection to the display
TM1637Display displayCountDown(CLK, DIO1); //set up the 4-Digit Display1.

int buttonState;
unsigned long duration = 0;
int i = 0;
unsigned long  remainingTime;

//The setup function is called once at startup of the sketch
void setup() {
	pinMode(RELAY_PIN, PIN_OUTPUT);	// set relay pin to output
	digitalWrite(RELAY_PIN, 0); //Set relay to OFF
	pinMode(Button_PIN, PIN_INPUT); // set button pin to input
	displayCountDown.setBrightness(0x0a); //set the display to maximum brightness
	Serial.begin(9600);			// initialize serial communications at 9600 bps:
	digitalWrite(RELAY_PIN, LOW);
}

// The loop function is called in an endless loop
void loop() {
	digitalWrite(RELAY_PIN, LOW);
	buttonState = digitalRead(Button_PIN);

	// print out the state of the button:
	Serial.println(buttonState);
	displayCountDown.showNumberDec(countDownDuration);

	if (buttonState == 1) {
		Serial.println("starting the countdown");

		unsigned long startTime = millis();
		duration = 0;
		i = 0;
		do {
			digitalWrite(RELAY_PIN, HIGH);
			Serial.println("countdown is going on");
			duration = millis() - startTime;
			displayCountDown.showNumberDec(remainingTime);
			remainingTime = countDownDuration - duration;
		} while (duration < countDownDuration);

	}


}
