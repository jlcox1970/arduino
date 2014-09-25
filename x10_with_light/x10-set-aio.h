int set_AIO(byte cmndCode, int AIO, int state){
	if (cmndCode == ON) {
		state = 254;
		stateCode[HC][UC] = STATUS_ON;
	}
	if (cmndCode == OFF) {
		stateCode[HC][UC] = STATUS_OFF;
		state = 0;
	}
	if (cmndCode == DIM) {
		state = state  - fadeAmount;
		if  ( state <= 0)
			state = fadeAmount;
		stateCode[HC][UC] = STATUS_ON;
	}
	if (cmndCode == BRIGHT) {
		state = state + fadeAmount;
		if ( state >= 254 )
			state = 254;
		stateCode[HC][UC] = STATUS_ON;
	}
	if (cmndCode == STATUS_REQUEST) {
		autoreport = false;
		x10_write = 1;
	}

	analogWrite(AIO, state);
	return state;
}
