void x10temp (byte HC, byte UC, int count) {
	detachInterrupt(1); 
	SendX10.x10temp(send_HC, unitCode, count,1);
    attachInterrupt(1, Check_Rcvr, CHANGE); 				// re-attach interrupt
}
void x10write(bool autoreport, byte send_HC, byte send_UC, int count){
	detachInterrupt(1);                                     // must detach interrupt before sending
	if ( autoreport == true){
		SendX10.write(send_HC, send_UC, RPT_SEND);
	}
    SendX10.write(send_HC, stateCode, RPT_SEND);
    attachInterrupt(1, Check_Rcvr, CHANGE); 				// re-attach interrupt
	x10_write = 0;
}