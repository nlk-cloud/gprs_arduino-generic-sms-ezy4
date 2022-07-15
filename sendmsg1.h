// set 'sendto' and 'msg' variables before calling this function
// SInce this application doesnt require time based operation, decided to give a fixed delay and also store 
// the response from MODEM in replyfrommodem variable so that we can check if there is some error in the 
//phone no. entered
void SendMessage()
{
  
  //Serial.println(" started sending");
  mySerial.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
  delay(1000);
// SInce this is only setting text mode command, not reading the reply from modem here
 //replyfrommodem=""; // before calling Modemresponse, ensure that there is nothing stored in replyfrommodem
//  nwlncntexpected=3; // since we are expecting 3 lines of response from modem
 //ModemResponse();
  //if (replyfrommodem.indexOf("OK")>0); // if Modem replies with ok, continue to the next command
    
  {
        mySerial.println("AT+CMGS=\""+sendto+"\"\r");  // send next command to modem
        delay(1000);
   //     replyfrommodem="";
    //    ModemResponse();

        mySerial.println(msg);// The SMS text you want to send
        //mySerial.println("check the code");
        // Reading the Modem respose here (after AT+CMGS and msg) and once again after ctrl+Z seems to be working 
        //nwlncntexpected=4; // instead of reading it only once after CTRL+Z
        delay(100);
        replyfrommodem="";
        ModemResponse();
        
        mySerial.println((char)26);// ASCII code of CTRL+Z
        delay(1000);
        //replyfrommodem="";
        //nwlncntexpected=8;
        //ModemResponse();
           
    
  }
}  
  
