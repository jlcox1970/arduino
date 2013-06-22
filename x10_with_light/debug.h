void X10_Debug() {
        Serial.println("Debug");
	Serial.print("SC-");
	Serial.print(startCode, BIN);
	Serial.print(" HOUSE-");
	Serial.print(houseCode, DEC);
	Serial.print(" UNIT-");
	Serial.print(unitCode, DEC);
	Serial.print(" CMND");
	Serial.print(cmndCode, DEC);
	if (cmndCode == ON)
		Serial.print(" (ON)");
	if (cmndCode == OFF)
		Serial.print(" (OFF)");
	if (cmndCode == DIM)
		Serial.print(" (DIM)");
	if (cmndCode == BRIGHT)
		Serial.print(" (BRIGHT)");
	if (cmndCode == EXTENDED_DATA)
		Serial.print(" (EXT_DATA)");
	if (cmndCode == STATUS_REQUEST)
		Serial.print(" (STATUS_REQUEST)");
	if (cmndCode == PRE_SET_DIM)
		Serial.print(" (PRE_SET_DIM)");
	if (cmndCode == PRE_SET_DIM2)
		Serial.print(" (PRE_SET_DIM2)");
	Serial.println("");
}
