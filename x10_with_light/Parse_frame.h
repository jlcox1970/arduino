
void Parse_Frame() { 					// parses the receive buffer to get House, Unit, and Cmnd
	if (rcveBuff & 0x1) { 				// last bit set so it's a command
		cmndCode = rcveBuff & 0x1F; 	// mask 5 bits 0 - 4 to get the command
		newX10 = true; 					// now have complete pair of frames
	} else { 							// last bit not set so it's a unit
		unitCode = rcveBuff & 0x1F; 	// mask 5 bits 0 - 4 to get the unit
		newX10 = false; 				// now wait for the command
		for (byte i = 0; i < 16; i++) { // use lookup table to get the actual unit #
			if (Unit[i] == unitCode) {
				unitCode = i + 1; 		// this gives Unit 1-16
				break; 					// stop search when found!
			}
		}
	}
	rcveBuff = rcveBuff >> 5; 			// shift the house code down to LSB
	houseCode = rcveBuff & 0x0F; 		// mask the last 4 bits to get the house code
	for (byte i = 0; i < 16; i++) { 	// use lookup table to get the actual command #
		if (House[i] == houseCode) {
			houseCode = i + 65; 		// this gives House 'A' - 'P'
			break; 						// stop search when found!
		}
	}
	rcveBuff = rcveBuff >> 4; 			// shift the start code down to LSB
	startCode = rcveBuff & 0x0F; 		// mask the last 4 bits to get the start code
	X10rcvd = false; 					// reset status
	if ( debug == 1){
		X10_frame_debug();
		X10_Debug();
	}
}
void Parse_Frame2() { // parses the receive buffer to get House, Unit, and Cmnd
	// Remember this is called by ISR - so STILL IN ISR! - no delays or prints!
	byte rawCmndUnit; // Command or Unit portion of rcveBuff
	byte rawHouse; // House portion of rcveBuff
	byte foundMatch = false; // set when a match is found
	bool procCmnd;
	// break the rcveBuff into it's parts . . .
	if(rcveBuff & 0x1) procCmnd = true; // last bit set so frame is a command (2nd)
	else procCmnd = false; // else frame is a unit (1st)
	rawCmndUnit = rcveBuff & 0x1F; // mask 5 bits 0 - 4 get the unit / command
	rcveBuff = rcveBuff >> 5; // shift the house code down to LSB
	rawHouse = rcveBuff & 0x0F; // mask the last 4 bits to get the house code
	rcveBuff = rcveBuff >> 4; // shift the start code down to LSB
	startCode = rcveBuff & 0x0F; // mask the last 4 bits to get the start code
	// start with the House code - SB the same for both frames
	for (byte i=0; i<16; i++){ // use lookup table to get the actual command #
		if (House[i] == rawHouse){
			houseCode = i+65; // this gives House 'A' - 'P'
			foundMatch = true;
			break; // stop search when found!
		}
	}
	 // if no match, wipe out the start byte to abort the command
	if (foundMatch == false) startCode = 0;
	// if it's the 1st frame, save off the house
	if (foundMatch == true && procCmnd == false) {
		firstHouse = houseCode;
		Serial.print(" fHC :");
		Serial.print(firstHouse,BIN);
		
	}
	// if it's the 2nd frame, compare the house code for the 2 frames
	if (foundMatch == true && procCmnd == true){
		if (houseCode != firstHouse) startCode = 0; // abort if not the same
	}
	// If it's the 1st frame process the Unit code . . .
	if (procCmnd == false){ // last bit not set so it's a unit
		newX10 = false; // now wait for the command
		foundMatch = false; // reset this before search
		for (byte i=0; i<16; i++){ // use lookup table to get the actual unit #
			if (Unit[i] == rawCmndUnit){
				unitCode = i+1; // this gives Unit 1-16
				foundMatch = true;
				break; // stop search when found!
			}
		}
		if (foundMatch == false) startCode = 0; // wipe out the start byte if no match
	}
	// If it's the 2nd frame process the Command code . . .
	if (procCmnd == true){ // last bit set so it's a command
//		if (rawCmndUnit == ON) X10.Cmnd=on; // convert commands to generic commands
//		else if (rawCmndUnit == OFF) X10.Cmnd=off;
//		else if (rawCmndUnit == DIM) X10.Cmnd=dim;
//		else if (rawCmndUnit == BRIGHT) X10.Cmnd=bright;
//		else startCode = 0; // wipe out the start byte if no match
		cmndCode = rawCmndUnit;

		Serial.print("  housecode :");
		Serial.print(houseCode, BIN);
		Serial.print("  cmndCode :");
		Serial.println(cmndCode, BIN);
		newX10 = true; // now have complete pair of frames
	}
	// if the command was set as invalid, it's not a new command
	if (startCode != B1110) newX10 = false; // final error check here
	//if (gParam[NO_P16]){ // optionally stop phantom P-16 commands
	// if (houseCode == 'P' && X10.Unit == 16) _newX10 = false; // Fix for phantom P-16 comand
	//}
}