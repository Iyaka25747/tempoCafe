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

const long countDownDurationNominal = 275000; //CountDown duration in millis cafe=4min35s = 275000millis
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
long duration = 0;
unsigned long startTime;
long remainingTime;
int i = 0;
int adjustmentValue = 0;
long displayValue = 0;

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
	long remainder = 0;
	Serial.println();
	digitalWrite(RELAY_PIN, LOW);
	buttonState = digitalRead(Button_PIN);
	//adjustmentValue = analogRead(adjustPin);
	adjustmentValue = readAverageInput(adjustPin);
	Serial.print(" - adj. value:");
	Serial.print(adjustmentValue);

	// The display oscille autour de la derniere valeur, on va arrondir pour stabiliser l affichage
	remainder = adjustmentValue % 10;
	Serial.print(" - remainder:");
		Serial.print(remainder);

	adjustmentValue = adjustmentValue - (adjustmentValue % 10);
	if (remainder < 5) {
		remainder = 0;
	} else {
		remainder = 10;
	}
	adjustmentValue = adjustmentValue + remainder; //fin de l arrondi
//	Serial.println();
	Serial.print(" - rounded adj. value:");
	Serial.print(adjustmentValue);

	// Limitation des valeurs du potentiomètre car il est foutu dans les hautes valeurs
	//, trop de temps pour démonter et en mettre un autre
	if (adjustmentValue > 520) {
		adjustmentValue = 520;
	}
	Serial.print(" - limited value:");
	Serial.print(adjustmentValue);
	Serial.print(" - Count down nominal: ");
	Serial.print(countDownDurationNominal);

	countDownCorrection = 260 - adjustmentValue; //520 = 120'000 millis
	countDownCorrection = countDownCorrection * 230; // Adjustment of +-60s --> 120/520*1000 =230
	Serial.print(" - correction: ");
	Serial.print(countDownCorrection);
	countDownDurationCorrected = countDownDurationNominal + countDownCorrection;

	if (countDownDurationCorrected <= 0) {
		Serial.print("-- min reach limiting correction to 5000");
		countDownDurationCorrected = 10000; // set min to 10sec
	}
	// The display oscille autour de la derniere valeur, on va arrondir pour stabiliser l affichage
		remainder = countDownDurationCorrected % 1000;
		Serial.print(" - remainder:");
			Serial.print(remainder);

			countDownDurationCorrected = countDownDurationCorrected - (countDownDurationCorrected % 1000);
		if (remainder < 500) {
			remainder = 0;
		} else {
			remainder = 1000;
		}
		countDownDurationCorrected = countDownDurationCorrected + remainder; //fin de l arrondi
	Serial.print(" - CountDown corrected: ");
	Serial.print(countDownDurationCorrected);
	// print out the state of the button:
	Serial.print(buttonState);
	displayValue = countDownDurationCorrected / 1000;
//	displayValue = displayValue - (displayValue%10);
	displayCountDown.showNumberDec(displayValue);

	if (buttonState == 1) {
		Serial.print(" - starting the countdown");
		startTime = millis();
		duration = 0;
		i = 0;
		remainingTime = countDownDurationCorrected;
		displayValue = 0;
		digitalWrite(RELAY_PIN, HIGH);
		do {
//			Serial.println();
			duration = millis() - startTime;
			remainingTime = countDownDurationCorrected - duration;
			displayValue = remainingTime / 100;
//			Serial.print(" - countdown running, duration: ");
//			Serial.print(duration);
//			Serial.print(" - remaining time: ");
//			Serial.print(remainingTime);
//			Serial.print(" - displayed value: ");
//			Serial.print(displayValue);
			displayCountDown.showNumberDec(displayValue);
			//remainingTime = countDownDurationNominal - duration;

		} while (remainingTime >= 0);

	}

}

int readAverageInput(int Pin) {

	// Smoothing input value parameters to get rid of erratic values
	// Define the number of samples to keep track of.  The higher the number,
	// the more the readings will be smoothed, but the slower the output will
	// respond to the input.
	const int numReadings = 2;  // number of samples for averaging
	//int readings[numReadings]; // array use to hold the the readings from the analog input
	int readIndex = 0;              // the index of the current reading
	int total = 0;                  // the running total, total of the array
	int average = 0;                // the computed average
	int ReadAnalogValue = 0;
	Serial.println();
	// Let's read some input values and compute the average to get rid of erratic values
	for (int thisReading = 0; thisReading < numReadings; thisReading++) {

		// subtract the last reading:
		//total = total - readings[readIndex];
		// read from the sensor:
		ReadAnalogValue = analogRead(Pin);
		//readings[thisReading] = ReadAnalogValue;
		//delay(1000);
//		Serial.println();
//		Serial.print(" - Average index: ");
//		Serial.print(thisReading);
//		Serial.print(" - Read 0-5V: ");
		//Serial.print(readings[readIndex]);
		//Serial.print(" - ");
//		Serial.print(ReadAnalogValue);
		// add the reading to the total:
		total = total + ReadAnalogValue;
		// advance to the next position in the array:
//		Serial.print(" - Total: ");
//		Serial.print(total);
		//thisReading = readIndex + 1;
		// if we're at the end of the array...
//		if (readIndex >= numReadings) {
//			// ...wrap around to the beginning:
//			readIndex = 0;
		//}
	}
	// calculate the average:
	average = total / numReadings;
	// send it to the computer as ASCII digits
	// Serial.println(average);
	//  delay(1);        // delay in between reads for stability
//	Serial.println();
//	Serial.print(" - Average computed: ");
//	Serial.print(average);
	return average;
}
