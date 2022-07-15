// set 'sendto' and 'msg' variables before calling this function
// SInce this application doesnt require time based operation, decided to give a fixed delay 
// Decided to ditch modemresponse since it is very unpredictable
void SendMessage()
{
  mySerial.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
  delay(1000);
  mySerial.println("AT+CMGS=\""+sendto+"\"\r");  // send next command to modem
  delay(1000);
  mySerial.println(msg);// The SMS text you want to send
  delay(100);
  mySerial.println((char)26);// ASCII code of CTRL+Z
  delay(1000);
}  
  
