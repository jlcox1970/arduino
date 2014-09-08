void X10_Debug() {
	Serial.print("SC-");
	Serial.print(startCode, BIN);
	Serial.print(" HOUSE-");
	Serial.print(houseCode, DEC);
	Serial.print(" UNIT-");
	Serial.print(unitCode, DEC);
	Serial.print(" CMND");
	Serial.print(cmndCode, DEC);
	if (cmndCode == ALL_UNITS_OFF)
		Serial.print(" (ALL_UNITS_OFF)");
	if (cmndCode == ALL_LIGHTS_ON)
		Serial.print(" (ALL_LIGHTS_ON)");
	if (cmndCode == ON)
		Serial.print(" (ON)");
	if (cmndCode == OFF)
		Serial.print(" (OFF)");
	if (cmndCode == DIM)
		Serial.print(" (DIM)");
	if (cmndCode == BRIGHT)
		Serial.print(" (BRIGHT)");
	if (cmndCode == ALL_LIGHTS_OFF)
		Serial.print(" (ALL_LIGHTS_OFF)");
	if (cmndCode == EXTENDED_CODE)
		Serial.print(" (EXTENDED_CODE)");
	if (cmndCode == HAIL_REQUEST)
		Serial.print(" (HAIL_REQUEST)");
	if (cmndCode == HAIL_ACKNOWLEDGE)
		Serial.print(" (HAIL_ACKNOWLEDGE)");
	if (cmndCode == PRE_SET_DIM)
		Serial.print(" (PRE_SET_DIM)");
	if (cmndCode == PRE_SET_DIM2)
		Serial.print(" (PRE_SET_DIM2)");
	if (cmndCode == EXTENDED_DATA)
		Serial.print(" (EXTENDED_DATA)");
	if (cmndCode == STATUS_ON)
		Serial.print(" (STATUS_ON)");
	if (cmndCode == STATUS_OFF)
		Serial.print(" (STATUS_OFF)");
	if (cmndCode == STATUS_REQUEST)
		Serial.print(" (STATUS_REQUEST)");
	Serial.println("");
}
