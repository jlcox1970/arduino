void Check_Rcvr() { 					// ISR - called when zero crossing (on CHANGE)
	/*if (DC_CLOCK_PIN == 0)
		digitalWrite(DC_CLOCK_PIN, HIGH);
	else
		digitalWrite(DC_CLOCK_PIN, LOW);
*/
	if (X10BitCnt == 0) { 				// looking for new frame
		delayMicroseconds(OFFSET_DELAY); // wait for bit
		if (digitalRead(RCVE_PIN))
			return; 					// still high - no start bit - get out
		if ( myX10P1 == 1){
			digitalWrite(LED2_PIN, HIGH); 	// indicate you got something
		}
		rcveBuff = 0;
		mask = 0x1000; 					// bitmask with bit 12 set
		rcveBuff = rcveBuff | mask; 	// sets bit 12 (highest)
		mask = mask >> 1; 				// move bit down in bit mask
		X10BitCnt = 1; 					// inc the bit count
		ZCrossCnt = 1; 					// need to count zero crossings too
		return;
	}

	// Begins here if NOT the first bit . . .

	ZCrossCnt++; 						// inc the zero crossing count

	// after SC (first 4 bits) ignore the pariety bits - so only read odd crossings

	if (X10BitCnt < 5 || (ZCrossCnt & 0x01)) { // if it's an odd # zero crossing
		delayMicroseconds(OFFSET_DELAY); // wait for bit
		if (!digitalRead(RCVE_PIN))
			rcveBuff = rcveBuff | mask; // got a 1 set the bit, else skip and leave it 0
		mask = mask >> 1; 				// move bit down in bit mask
		X10BitCnt++;

		if (X10BitCnt == 13) { 			// done with frame after 13 bits
			for (byte i = 0; i < 5; i++)
				delayMicroseconds(HALF_CYCLE_DELAY); // need this
			X10rcvd = true; 			// a new frame has been received
			if ( myX10P1 == 1){
						digitalWrite(LED2_PIN, LOW);
			}
			X10BitCnt = 0;
			Parse_Frame(); 				// parse out the house & unit code and command
		}
	}
}
