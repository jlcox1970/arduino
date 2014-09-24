int set_AIO(byte cmndCode, int AIO, int state){
	if (cmndCode == ON) {
		analogWrite(AIO, 255);
		brightness = 255;
		return 1;
	}
	if (cmndCode == OFF) {
		analogWrite(AIO, 0);
		brightness = 0;
		return 0;
	}
	if (cmndCode == DIM) {
		brightness = brightness - fadeAmount;
		if (brightness <= 0)
			brightness = fadeAmount;
		analogWrite(AIO, brightness);
	}
	if (cmndCode == BRIGHT) {
		brightness = brightness + fadeAmount;
		if (brightness >= 255)
			brightness = 255;
		analogWrite(AIO, brightness);
	}
	if (cmndCode == STATUS_REQUEST) {
		if ( state == 1)
			stateCode = STATUS_ON;
		if ( state == 0)
			stateCode = STATUS_OFF;
		
		x10_write = 1;
	}
	return state;
}
