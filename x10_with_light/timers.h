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
			if ( debug == 1){
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

int timer_dec(int lastCheck,int HC, int UC){
	//UC=UC;
	int state = myX10[HC][UC];
	if ( state != 0){
		if (seconds() - lastCheck > timeVal ){
			lastCheck = seconds();
			myX10[HC][UC] = set_timer(DIM, Control[HC][UC], myX10[HC][UC], HC, UC);
			if (debug2 == 1){
				Serial.print("DIM HC:");
				Serial.print(HC, DEC);
				Serial.print(" UC:");
				Serial.print(UC, DEC);
				Serial.print(" state :");
				Serial.println(myX10[HC][UC]);
			}
				
		}
	}
	return lastCheck;
}