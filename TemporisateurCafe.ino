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
int RELAY_PIN = 6; // Command of relay
int Button_PIN = 5; // Button read
int DispClkPin = 3; // TMS163 Display
int DispDataPin = 2; // TMS163 Display
int adjustPin = 1; // analog pin A1,  0-5V read input to adjust countdown
// Improvement ?: Add potentiometer read to adjust timing

const long countDownDurationNominal = 60000; //CountDown duration in millis
long countDownCorrection = 0;
long countDownDurationCorrected;

/*
 HEX Display countdown
 */
//Measured temperature
const int CLK = DispClkPin; //Set the CLK pin connection to the display
const int DIO1 = DispDataPin; //Set the DIO pin connection to the display
TM1637Display displayCountDown(CLK, DIO1); //set up the 4-Digit Display1.

int buttonState;
unsigned long duration = 0;
unsigned long startTime;
long remainingTime;
int i = 0;
int adjustmentValue = 0;

//The setup function is called once at startup of the sketch
void setup() {
	pinMode(RELAY_PIN, PIN_OUTPUT);	// set relay pin to output
	digitalWrite(RELAY_PIN, 0); //Set relay to OFF
	pinMode(Button_PIN, PIN_INPUT); // set button pin to input
	displayCountDown.setBrightness(0x0a); //set the display to maximum brightness
	Serial.begin(9600);			// initialize serial communications at 9600 bps:
	digitalWrite(RELAY_PIN, LOW);
	//pinMode(adjustPin, PIN_INPUT);
}

// The loop function is called in an endless loop
void loop() {
	Serial.println();
	digitalWrite(RELAY_PIN, LOW);
	buttonState = digitalRead(Button_PIN);
	adjustmentValue = analogRead(adjustPin);
	// Amélioration: faire une moyenne de lecture
	//adjustmentValue = 800;
	Serial.print(" - adjustmentValue:");
	Serial.print(adjustmentValue);
	countDownCorrection = 117 * (512 - adjustmentValue); //1024 = 120'000 millis sec
	countDownDurationCorrected = countDownDurationNominal + countDownCorrection;
	Serial.print(" - correction:");
	Serial.print(countDownCorrection);
	if (countDownDurationCorrected <= 0) {
		Serial.print("-- min reach limiting correction to 5000");
		countDownDurationCorrected = 10000;
	}
	Serial.print("-- 0-5V value:");
	Serial.print(adjustmentValue);
	// print out the state of the button:
	Serial.print(buttonState);
	displayCountDown.showNumberDec(countDownDurationCorrected/1000);

	if (buttonState == 1) {
		Serial.print(" - starting the countdown");
		startTime = millis();
		duration = 0;
		i = 0;
		remainingTime = countDownDurationCorrected;
		do {
			Serial.println();
			digitalWrite(RELAY_PIN, HIGH);
			Serial.print(" - countdown is going on:");
			Serial.print(remainingTime);
			duration = millis() - startTime;
			displayCountDown.showNumberDec(remainingTime/1000);
			//remainingTime = countDownDurationNominal - duration;
			remainingTime = remainingTime - duration;

		} while (remainingTime >= 0);

	}

}
