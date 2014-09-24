int set_DIO(byte cmndCode, int DIO, int state){
	if (cmndCode == ON) {
		digitalWrite(DIO, 1);
		lastSec = currentSec;
		stateCode = STATUS_ON;
		return 1;
	}
	if (cmndCode == OFF) {
		digitalWrite(DIO, 0);
		stateCode = STATUS_OFF;
		return 0;
	}
	if (cmndCode == STATUS_REQUEST) {
		x10_write = 1;
	}
	return state;
}
