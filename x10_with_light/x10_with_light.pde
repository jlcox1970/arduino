/* Arduino Interface to the PSC05 X10 Receiver.                       BroHogan 3/24/09
 * SETUP: X10 PSC05/TW523 RJ11 to Arduino (timing for 60Hz)
 * - RJ11 pin 1 (BLK) -> Pin 2 (Interrupt 0) = Zero Crossing
 * - RJ11 pin 2 (RED) -> GND
 * - RJ11 pin 3 (GRN) -> Pin 4 = Arduino receive
 * - RJ11 pin 4 (YEL) -> Pin 5 = Arduino transmit (via X10 Lib)
 * NOTES:
 * - Must detach interrup when transmitting with X10    Lib 
 */

#include "Arduino.h"                  // this is needed to compile with Rel. 0013
#include <x10.h>                       // X10 lib is used for transmitting X10
#include <x10constants.h>              // X10 Lib constants
#define RPT_SEND 1                     // how many times transmit repeats if noisy set higher
#include <psc05.h>                     // constants for PSC05 X10 Receiver
#define TRANS_PIN      5               // YEL pin 4 of PSC05
#define RCVE_PIN       4               // GRN pin 3 of PSC05
#define ZCROSS_PIN     2               // BLK pin 1 of PSC05
#define LED_PIN        13              // for testing. Blinks with command received
#define control_pin    9  
//#define temp_pin       A5
//#define ref_vcc        A4
#define DC_TRANS_PIN	8				// DC Bus data out
#define DC_RCVE_PIN	6				// DC Bus data in
#define DC_CLOCK_PIN	7				// DC_Bus Clock

volatile unsigned long mask; // MSB first - bit 12 - bit 0
volatile unsigned int X10BitCnt = 0; // counts bit sequence in frame
volatile unsigned int ZCrossCnt = 0; // counts Z crossings in frame
volatile unsigned long rcveBuff; // holds the 13 bits received in a frame
volatile boolean X10rcvd = false; // true if a new frame has been received
boolean newX10 = false; // both the unit frame and the command frame received
byte houseCode, unitCode, cmndCode; // current house, unit, and command code
byte startCode; // only needed for testing - sb B1110 (14)
boolean myX10D1; // my state
boolean myX10B1; // my state
int tempValue = 0;
int ref_val = 0;
int count = 0;
byte out_put;
int old_val = 0;
int pause = 0;
int brightness = 0; 					// how bright the LED is
int fadeAmount = 255 / 8; 				// how many points to fade the LED by
float bit_val;
int temp_pin = A5;
int ref_vcc = A4;


x10 SendX10 = x10(ZCROSS_PIN, TRANS_PIN);// set up a x10 library instance:

#include "Parse_frame.h"
#include "x10_check_rcvr.h"
#include "debug.h"
#include "setup.h"




void loop() {
	ref_val = analogRead(temp_pin);
	bit_val = analogRead(ref_vcc) / 1.7;


	tempValue = (ref_val * 0.0048876) * 10;
	count = tempValue - 4;
	if (pause == 1000) {
//		Serial.print("bit value  ");
//		Serial.print(bit_val, DEC);
//		Serial.println("");
			if (old_val < tempValue - 2) {
			newX10 = true;
			unitCode = 16;
			houseCode = 65;
			cmndCode = STATUS_REQUEST;
		}
		if (old_val > tempValue + 2) {
			newX10 = true;
			unitCode = 16;
			houseCode = 65;
			cmndCode = STATUS_REQUEST;
		}
		pause  = 0;
	}
	pause = pause + 1;
	if (newX10) { 								// received a new command
		X10_Debug(); 							// print out the received command
		newX10 = false;
		if (unitCode == 1 && houseCode == 65) {
			if (cmndCode == ON) {
				detachInterrupt(0); 			// must detach interrupt before sending
				SendX10.write(D, UNIT_5, RPT_SEND);
				SendX10.write(D, ON, RPT_SEND);
				myX10D1 = ON;
				attachInterrupt(0, Check_Rcvr, CHANGE);// re-attach interrupt
			}
			if (cmndCode == OFF) {
				detachInterrupt(0); 			// must detach interrupt before sending
				SendX10.write(D, UNIT_5, RPT_SEND);
				SendX10.write(D, OFF, RPT_SEND);
				myX10D1 = OFF;
				attachInterrupt(0, Check_Rcvr, CHANGE);// re-attach interrupt
			}
			if (cmndCode == STATUS_REQUEST) {
				detachInterrupt(0); 			// must detach interrupt before sending
				SendX10.write(A, UNIT_1, RPT_SEND);
				SendX10.write(A, myX10D1, RPT_SEND);
				attachInterrupt(0, Check_Rcvr, CHANGE);// re-attach interrupt
			}
		}
		if (unitCode == 1 && houseCode == 66) {
			if (cmndCode == ON) {
				analogWrite(control_pin, 255);
				brightness = 255;
				myX10B1 = ON;
			}
			if (cmndCode == OFF) {
				analogWrite(control_pin, 0);
				brightness = 0;
				myX10B1 = OFF;
			}
			if (cmndCode == DIM) {
				brightness = brightness - fadeAmount;
				if (brightness <= 0)
					brightness = fadeAmount;
				analogWrite(control_pin, brightness);
			}
			if (cmndCode == BRIGHT) {
				brightness = brightness + fadeAmount;
				if (brightness >= 255)
					brightness = 255;
				analogWrite(control_pin, brightness);
			}
			if (cmndCode == STATUS_REQUEST) {
				detachInterrupt(0); 					// must detach interrupt before sending
				SendX10.write(B, UNIT_1, RPT_SEND);
				SendX10.write(B, myX10B1, RPT_SEND);
				attachInterrupt(0, Check_Rcvr, CHANGE);	// re-attach interrupt
			}
		}
		if (unitCode == 16 && houseCode == 65) {
//			if (cmndCode == STATUS_REQUEST) {
                        if (cmndCode == PRE_SET_DIM) {
                        Serial.print("Temp requested for ");
				Serial.print(houseCode, DEC);
				Serial.println("");
				SendX10.x10temp(houseCode, unitCode, count, 2);
				attachInterrupt(0, Check_Rcvr, CHANGE);	// re-attach interrupt
				pause = 0;
			}
		}
		Serial.print("temp sensor input ");
		Serial.print(tempValue, DEC);
		Serial.print("       ");
		Serial.print("old value = ");
		Serial.print(old_val, DEC);
		Serial.print("       ");
		Serial.print("ref value = ");
		Serial.print(analogRead(ref_vcc), DEC);
		Serial.println("");
		old_val = tempValue;
	}
}
