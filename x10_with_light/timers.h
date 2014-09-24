int seconds(){
	int count_seconds = millis() / 1000;
	return count_seconds;
}

int minutes (){
	int count_minutes = millis() / 60000;
	return count_minutes;
}

int check_timer(int timer_expire, int lastCheck, byte HC, byte UC, bool state, int DIO){
	if ( state == 1){
		if (seconds() - lastCheck > timer_expire){
			lastCheck = seconds();
			send_HC = HC;
			send_UC = UC;
			stateCode = OFF;
			autoreport = true;
			digitalWrite(DIO, 0 );
			myX10A2 = 0;
			x10_write = 1;
		}
	}
	return lastCheck;
}