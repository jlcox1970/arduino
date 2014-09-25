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

void X10_write_debug(){
	Serial.print("status request for ");
	Serial.print(send_HC, BIN);
	Serial.print(" unit:");
	Serial.print(send_UC, BIN);
	Serial.print(" stateCode:");
	Serial.print(stateCode[HC][UC], BIN);
	Serial.println();
	Serial.print ("send HC :");
	Serial.print (HC);
	Serial.print (" send UC :");
	Serial.println(UC);
	
}
void X10_write_temp_debug(){
	Serial.print("Pause =");
	Serial.print(count_pause);
	Serial.println("----");
	Serial.print(" tempValue:");
	Serial.println(tempValue);
	Serial.print("count pause: ");
	Serial.print(count_pause);
	Serial.print(" check time: " );
	Serial.println(check_time);
}
void X10_frame_debug(){
	Serial.print("Parse Frame :");
	Serial.println(rcveBuff, BIN);
}
void minute_debug(){
	Serial.print("tick ");
	Serial.println(minutes(), DEC);
	Serial.print("count pause: ");
	Serial.print(count_pause);
	Serial.print(" check time: " );
	Serial.println(check_time);
}
void X10_debug_notify(){
	if (unitCode == 1 && houseCode == HOUSE_P) {
		Serial.print("P1 is (Debug) ");
		Serial.print(myX10[15][0]);
		Serial.println("");
	}
}
void X10_debug_aio(){
	Serial.print("DIM value :");
	Serial.println(state, DEC);
}
