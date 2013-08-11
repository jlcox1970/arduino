
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
}

