int set_DIO(byte cmndCode, int DIO, int state){
	if (cmndCode == ON) {
		digitalWrite(DIO, 1);
		lastSec = currentSec;
		stateCode[HC][UC] = STATUS_ON;
		return 1;
	}
	if (cmndCode == OFF) {
		digitalWrite(DIO, 0);
		stateCode[HC][UC] = STATUS_OFF;
		return 0;
	}
	if (cmndCode == STATUS_REQUEST) {
		autoreport = false;
		x10_write = 1;
	}
	return state;
}
