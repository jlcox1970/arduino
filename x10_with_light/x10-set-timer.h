int set_timer(byte cmndCode, int DIO, int state, int HC, int UC){
	if (cmndCode == ON) {
		state = maxTime;
		stateCode[HC][UC] = STATUS_ON;
	}
	if (cmndCode == OFF) {
		stateCode[HC][UC] = STATUS_OFF;
		state = 0;
	}
	if (cmndCode == DIM) {
		
		if  ( state <= timeVal + 1){
			state = 0;
			stateCode[HC][UC] = OFF;
			autoreport = true;
			x10_write = 1;
		} else {
			state = state  - timeVal;
			stateCode[HC][UC] = DIM;
			autoreport = true;
			x10_write = 1;
		}
	}
	if (cmndCode == BRIGHT) {
		state = state + timeVal;
		if ( state >= 254 )
			state = 254;
		stateCode[HC][UC] = STATUS_ON;
	}
	if (cmndCode == STATUS_REQUEST) {
		autoreport = false;
		x10_write = 1;
	}
	if (stateCode[HC][UC] == STATUS_ON || stateCode[HC][UC] == DIM ){
		digitalWrite(DIO,1);
	} else {
		digitalWrite(DIO,0);
	}
	return state;
}
