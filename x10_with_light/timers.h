int seconds(){
	int count_seconds = millis() / 1000;
	return count_seconds;
}

int minutes (){
	int count_minutes = millis() / 60000;
	return count_minutes;
}

int check_timer(int timer_expire, int lastCheck, int HC, int UC ){
	UC = UC - 1;
	
	int state = myX10[HC][UC];  //get state from array

	
	if ( state == 1){
		if (seconds() - lastCheck > timer_expire){
			if ( myX10P1 == 1){
				Serial.print ("HC");
				Serial.print (HC);
				Serial.print ("UC");
				Serial.print (UC + 1);
				Serial.println(" timer expired");
			}
			lastCheck = seconds();
			send_HC = House[HC];
			send_UC = Unit[UC];
			stateCode[HC][UC] = OFF;
			autoreport = true;
			digitalWrite(Control[HC][UC], 0 );
			myX10[HC][UC] = 0;
			x10_write = 1;
		}
	}
	return lastCheck;
}