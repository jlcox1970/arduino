/* Arduino Interface to the PSC05 X10 Receiver.                       BroHogan 3/24/09
 * SETUP: X10 PSC05/TW523 RJ11 to Arduino (timing for 60Hz)
 * - RJ11 pin 1 (BLK) -> Pin 2 (Interrupt 0) = Zero Crossing
 * - RJ11 pin 2 (RED) -> GND
 * - RJ11 pin 3 (GRN) -> Pin 4 = Arduino receive
 * - RJ11 pin 4 (YEL) -> Pin 5 = Arduino transmit (via X10 Lib)
 * NOTES:
 * - Must detach interrup when transmitting with X10    Lib 
 */

#include <x10.h>                // X10 lib is used for transmitting X10
#include <x10constants.h>       // X10 Lib constants
#include "DHT.h"
#include <psc05.h>              // constants for PSC05 X10 Receiver

#define RPT_SEND 		1       // how many times transmit repeats if noisy set higher
#define dataPin        PD0      // YEL pin 4 of PSC05
#define RCVE_PIN       PD1      // GRN pin 3 of PSC05
#define ZCROSS_PIN     PD2      // BLK pin 1 of PSC05
#define LED_PIN        9        // Green LED
#define LED2_PIN       10		// Blue LED
#define LED3_PIN       13		// Red LED

#define control_pin    13  
/*
#define DC_TRANS_PIN	8		// DC Bus data out
#define DC_RCVE_PIN 	6		// DC Bus data in
#define DC_CLOCK_PIN	7		// DC_Bus Clock
*/
#define DHTPIN 	       	PD3		// what pin we're connected to for DHT22
#define DHTTYPE 		DHT22  	// DHT 22  (AM2302)
#define RELAY_1 		PD7
#define RELAY_2			PD6
#define RELAY_3			PD5
#define RELAY_4			PD4

volatile unsigned long mask; 		// MSB first - bit 12 - bit 0
volatile unsigned int X10BitCnt = 0; 	// counts bit sequence in frame
volatile unsigned int ZCrossCnt = 0; 	// counts Z crossings in frame
volatile unsigned long rcveBuff; 	// holds the 13 bits received in a frame
volatile boolean X10rcvd = false; 	// true if a new frame has been received
boolean newX10 	= false; 		// both the unit frame and the command frame received
boolean newX10temp 	= false; 		// both the unit frame and the command frame received
byte houseCode, unitCode, cmndCode, stateCode, send_HC, send_UC; 	// current house, unit, and command code
byte startCode; 			// only needed for testing - sb B1110 (14)
bool myX10D1, myX10B1; 	// my state
volatile int tempValue 	= 0;
int count 	= 0;
byte out_put;
int old_val 	= 0;
int count_pause = 0;
int brightness 	= 0; 			// how bright the LED is
int fadeAmount 	= 255 / 8; 		// how many points to fade the LED by
int sendtemp   	= 0;
int x10_write  	= 0;
float t, h;
int check_time  = 4;			//delay for tempurature check
int pause_reset = 5;
int small_timer = 20000;
int small_count = 0 ;
bool sent = false;

x10 SendX10 = x10(ZCROSS_PIN, dataPin);	// set up a x10 library instance:
DHT dht(DHTPIN, DHTTYPE);

#include "debug.h"
#include "Parse_frame.h"
#include "x10_check_rcvr.h"
#include "setup.h"

void loop() {
	small_count = small_count +1 ;
	if ( small_count >= small_timer) {
		count_pause = count_pause + 1;
		small_count = 0 ;
		Serial.print(" ");
	}
	delay(1);
	if (newX10) { 								// received a new command
		Serial.println(".");
		X10_Debug(); 							// print out the received command
		newX10 = false;
		if (unitCode == 1 && houseCode == HOUSE_A) {
			send_HC  = HC_A;
			send_UC = UNIT_1;
			if (cmndCode == ON) {
				detachInterrupt(1); 			// must detach interrupt before sending
				SendX10.write(HOUSE_B, UNIT_5, RPT_SEND);
				SendX10.write(HOUSE_B, ON, RPT_SEND);
				attachInterrupt(1, Check_Rcvr, CHANGE);// re-attach interrupt
			}
			if (cmndCode == OFF) {
				detachInterrupt(1); 			// must detach interrupt before sending
				SendX10.write(HOUSE_B, UNIT_5, RPT_SEND);
				SendX10.write(HOUSE_B, OFF, RPT_SEND);
				attachInterrupt(1, Check_Rcvr, CHANGE);// re-attach interrupt
			}
			if (cmndCode == STATUS_REQUEST) {
				x10_write = 1;
			}			
		}
		// Relay 1
		if (unitCode == 2 && houseCode == HOUSE_A) {
			send_HC  = HC_A;
			send_UC = UNIT_2;
			if (cmndCode == ON) {
				digitalWrite(RELAY_1, 1);
			}
			if (cmndCode == OFF) {
				digitalWrite(RELAY_1, 0);
			}
			if (cmndCode == STATUS_REQUEST) {
				x10_write = 1;
			}			
		}	

		// Relay 2
		if (unitCode == 3 && houseCode == HOUSE_A) {
			send_HC  = HC_A;
			send_UC = UNIT_3;
			if (cmndCode == ON) {
				digitalWrite(RELAY_2, 1);
				Serial.println(" Turn on A3");
			}
			if (cmndCode == OFF) {
				digitalWrite(RELAY_2, 0);
			}
			if (cmndCode == STATUS_REQUEST) {
				x10_write = 1;
			}	
			unitCode = 99;
		}
		
		// Relay 4
		if (unitCode == 5 && houseCode == HOUSE_A) {
			send_HC  = HC_A;
			send_UC = UNIT_5;
			if (cmndCode == ON) {
				digitalWrite(RELAY_4, 1);
			}
			if (cmndCode == OFF) {
				digitalWrite(RELAY_4, 0);
			}
			if (cmndCode == STATUS_REQUEST) {
				x10_write = 1;
			}			
		}
		// Relay 3
		if (unitCode == 4 && houseCode == HOUSE_A) {
			send_HC  = HC_A;
			send_UC = UNIT_4;
			if (cmndCode == ON) {
				digitalWrite(RELAY_3, 1);
				Serial.println(" Turn on A4");
			}
			if (cmndCode == OFF) {
				digitalWrite(RELAY_3, 0);
			}
			if (cmndCode == STATUS_REQUEST) {
				x10_write = 1;
			}			
		}
		if (unitCode == 1 && houseCode == HOUSE_B ) {
			send_HC=HC_B;
			send_UC = UNIT_1;
			if (cmndCode == ON) {
				analogWrite(control_pin, 255);
				brightness = 255;
				myX10B1 = ON;
				stateCode = STATUS_ON;
			}
			if (cmndCode == OFF) {
				analogWrite(control_pin, 0);
				brightness = 0;
				myX10B1 = OFF;
				stateCode = STATUS_OFF;
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
				delay (250);
				x10_write = 1;
			}
		}
		if ((unitCode == 5 && houseCode == 77 && cmndCode == PRE_SET_DIM ) || (unitCode == 5 && houseCode == HOUSE_A && cmndCode == STATUS_REQUEST )) {
			send_HC = HC_A;
			send_UC = UNIT_5;
			sendtemp = 1;
			newX10temp = true;
			delay(10);
		}
	}
	if ( newX10 == false ) {
		if ( count_pause == check_time && sent == false) { 
			Serial.print("Pause =");
			Serial.print(count_pause);
			Serial.println("----");
			tempValue = (int)dht.readTemperature(); 
			count = tempValue - 4;
			Serial.print(" tempValue:");
			Serial.println(tempValue);
			newX10temp = true;
			unitCode = 5;
			houseCode = HOUSE_A;
			cmndCode = STATUS_REQUEST;
			sent = true;	
		}
	}
	if (newX10temp){
                if (unitCode == 5 && houseCode == HOUSE_A) {
                        if (cmndCode == STATUS_REQUEST) {
                                sendtemp = 1;
                            	newX10temp = false;
                            	send_HC = HC_A;
                        }
                }
	}
	if ( x10_write == 1 && newX10 == false) {
		//delay (1);
		Serial.print("status request for ");
		Serial.print(send_HC, BIN);
		Serial.print(" unit:");
		Serial.print(unitCode, BIN);
		Serial.print(" stateCode:");
		Serial.print(stateCode, BIN);
		Serial.println();
        detachInterrupt(1);                                     // must detach interrupt before sending
//	    SendX10.write(send_HC, unitCode, RPT_SEND);
        SendX10.write(send_HC, stateCode, RPT_SEND);
	    attachInterrupt(1, Check_Rcvr, CHANGE); 				// re-attach interrupt
		x10_write = 0;
	}
	if ( sendtemp == 1  && newX10 == false ) {
        detachInterrupt(1);                                     // must detach interrupt before sending
	    SendX10.x10temp(send_HC, unitCode, count,1);
        attachInterrupt(1, Check_Rcvr, CHANGE); 				// re-attach interrupt
	    attachInterrupt(0, Check_Rcvr, CHANGE); 				// re-attach interrupt
	    old_val = tempValue;
        sendtemp = 0;
	}
   	if ( count_pause >= pause_reset ){
   	   		Serial.println("reset counter loop");
   	   		count_pause = 0;
       		sent = false;
   	}
}
