int set_NULL(byte cmndCode, int DIO, int state){
	if (cmndCode == ON) {
		stateCode[HC][UC] = STATUS_ON;
		return 1;
	}
	if (cmndCode == OFF) {
		stateCode[HC][UC] = STATUS_OFF;
		return 0;
	}
	if (cmndCode == STATUS_REQUEST) {
		autoreport = false;
		x10_write = 1;
	}
	return state;
}
