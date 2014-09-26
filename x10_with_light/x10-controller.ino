/* Arduino Interface to the PSC05 X10 Receiver.                       BroHogan 3/24/09
 * modified by Jason Cox
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
bool newX10 		= false; 		// both the unit frame and the command frame received
bool newX10temp 	= false; 		// both the unit frame and the command frame received
bool autoreport 	= false;		// setting to change if its an auto report
byte houseCode, unitCode, cmndCode, stateCode[16][16], send_HC, send_UC, firstHouse; 	// current house, unit, and command code
byte startCode; 					// only needed for testing - sb B1110 (14)
bool debug, debug2;
int myX10DIM[16][16] ;
byte typeCode[16][16] ;
int myX10[16] [16] ;
//int myX10Timers[16][16]; 	// my state
long lastrun[16][16];
volatile int tempValue 	= 0;
int count 	= 0;
byte out_put;
int old_val 	= 0;
int count_pause = 0;
//int brightness 	= 0; 			// how bright the LED is
int fadeAmount 	= 32; 				// how many points to fade the LED by
int sendtemp   	= 0;
int x10_write  	= 0;
float t, h;
int check_time  = 1;				//delay for tempurature check
int pause_reset = 5;
int small_timer = 20000;
int small_count = 0 ;
bool sent = false;
int lastMinute;
int HC, UC, hc ,uc;
int null_out = 999;
int state;
int dummy[16];
long maxTime = 300;
int timeVal = 20; // should be 20
int dim_repeat = 5;

//vars for timer
long currentSec;
long lastSec;
long setTimeSec = 10;

x10 SendX10 = x10(ZCROSS_PIN, dataPin);	// set up a x10 library instance:
DHT dht(DHTPIN, DHTTYPE);

#include "devices.h"
#include "x10-set-timer.h"
#include "timers.h"
#include "debug.h"
#include "Parse_frame.h"
#include "x10_check_rcvr.h"
#include "setup.h"
#include "x10-send.h"
#include "x10-set-dio.h"
#include "x10-set-aio.h"
#include "x10-set-null.h"


void loop() {
	debug = myX10[15][0];
	debug2 = myX10[15][1];
	//calculate current uptime in seconds
	currentSec = seconds();
	//lastSec = check_timer ( setTimeSec, lastSec, 0, 2 );
	

	
	
	if (hc < 16){
		if (uc < 16){
			uc++;
		} else {
			hc ++;
			uc = 0;
		}
	} else {
		hc = 0;
	}
	
	if (typeCode[hc][uc] == 3){
		if ((stateCode[hc][uc] != OFF ) || (stateCode[hc][uc] != STATUS_OFF)) {
			dummy[uc] = timer_dec (dummy[uc], hc, uc);
		}
	}

	
	if ( minutes() - lastMinute >= check_time){
		if (debug == 1){
			minute_debug();
		}
		count_pause = check_time;
		lastMinute = minutes();
		sent = false;
	}
	
	if (newX10) { 								// received a new command
		X10_Debug(); 							// print out the received command
		newX10 = false;
		HC = houseCode - 65;					// set to 0 starting for the array lookups
		UC = unitCode - 1;						// set to 0 starting for the array lookups
		send_HC = House[HC];
		send_UC = Unit[UC];
		if (typeCode[HC][UC] == 1){			// null device ie. debug switch
			myX10[HC][UC] = set_NULL(cmndCode, Control[HC][UC], myX10[HC][UC]);
		} else if  (typeCode[HC][UC] == 2){	// dimmer output
			myX10[HC][UC] = set_AIO(cmndCode, Control[HC][UC], myX10[HC][UC]);
			if ( debug == 1){
				state = myX10[HC][UC];
				X10_debug_aio();
			}
		} else if (typeCode[HC][UC] == 3){     // digital timer based on dimmer
			myX10[HC][UC] = set_timer(cmndCode, Control[HC][UC], myX10[HC][UC], HC, UC);
			if ( debug == 1){
				state = myX10[HC][UC];
				X10_debug_aio();
			}
		} else {								//relay control without timer 
			myX10[HC][UC] = set_DIO(cmndCode, Control[HC][UC], myX10[HC][UC]);
		}
		if ((unitCode == 5 && houseCode == 77 && cmndCode == PRE_SET_DIM ) || (unitCode == 5 && houseCode == HOUSE_A && cmndCode == STATUS_REQUEST )) {
			send_HC = HC_A;
			send_UC = UNIT_5;
			sendtemp = 1;
			newX10temp = true;
		}
		X10_debug_notify();
	} else {
		HC=hc;
		UC=uc;
	}
	if ( newX10 == false && x10_write == 0) {
		if ( count_pause == check_time && sent == false) { 
			tempValue = (int)dht.readTemperature(); 
			count = tempValue - 4;
			if (debug == 1){
				X10_write_temp_debug();
			}
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
	if ( x10_write == 1 && newX10 == false ) {
		if (debug == 1){
			X10_write_debug();
		}

		if (typeCode[HC][UC] == 3 && stateCode[HC][UC] == DIM ){
			for (int repeat ; repeat < dim_repeat ; repeat++){
				x10_write = 1;
				x10write(autoreport, HC, UC , RPT_SEND);
			}
			if (stateCode[HC][UC] == OFF)
				stateCode[HC][UC] = STATUS_OFF;
		} else {
			x10write(autoreport, HC, UC , RPT_SEND);
			if (stateCode[HC][UC] == OFF)
				stateCode[HC][UC] = STATUS_OFF;
		}
	}
	
	if ( sendtemp == 1  && newX10 == false ) {
		if (old_val != tempValue)
			x10temp(send_HC,unitCode,count);
	    old_val = tempValue;
        sendtemp = 0;
	}
   	if ( count_pause >= pause_reset ){
   	   		count_pause = 0;
       		sent = false;
   	}
}

	