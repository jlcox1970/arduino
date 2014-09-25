/*
$Rev: 41 $
$Author: jason $
$Date: 2012-06-02 16:25:55 +1000 (Sat, 02 Jun 2012) $
*/



void setup() {
	attachInterrupt(1, Check_Rcvr, CHANGE);// (pin 2) trigger zero cross
	Serial.begin(9600);
	pinMode(LED_PIN, OUTPUT); 			// onboard LED
	pinMode(control_pin, OUTPUT);
	pinMode(RCVE_PIN, INPUT); 			// receive X10 commands - low = 1
    pinMode(dataPin, OUTPUT);
    pinMode(ZCROSS_PIN, INPUT); 		// zero crossing - 60 Hz square wave
	digitalWrite(RCVE_PIN, HIGH); 		// set 20K pullup (low active signal)
	digitalWrite(ZCROSS_PIN, HIGH); 	// set 20K pullup (low active signal)
/*
 * 
 	pinMode(DC_RCVE_PIN, INPUT);
	pinMode(DC_TRANS_PIN, OUTPUT);
	pinMode(DC_CLOCK_PIN, OUTPUT);
	digitalWrite(DC_TRANS_PIN, HIGH);
	digitalWrite(DC_CLOCK_PIN, HIGH);
*/
	dht.begin();
	for ( int i = 0 ; i < 16 ; i ++){
		for ( int j = 0 ; j < 16 ; j++ ){
			stateCode [i][j] = STATUS_OFF;
		}
	}
	typeCode[15][0] = 99;  // setup dull device control for debug on house P unit 1
	typeCode[0][0]= 98;    // on board led with dimmer
	
	// relay outputs
	pinMode(RELAY_1, OUTPUT);
	pinMode(RELAY_2, OUTPUT);
	pinMode(RELAY_3, OUTPUT);
	pinMode(RELAY_4, OUTPUT);
			
}
