void ReadCharacterbyCharacter()
{
    /*
const byte numChars = 100;
char receivedChars[numChars];   // an array to store the received data





//void loop() {
//    recvWithEndMarker();
  //  showNewData();
//}

//void recvWithEndMarker() {
    static byte ndx = 0;
//    char endMarker = '\n';
  int newlinecnt=0;
    char rc;
  
//Serial.println(receivedChars);
        //Serial.println(entiremsg);  
//if(mySerial.available()>0)
{    
//Serial.println("entered if");
    while (mySerial.available() > 0 && newData == false) {
//Serial.println("entered while");
        rc = mySerial.read();
        Serial.print(" rc is ");
        Serial.println(rc);
                
        Serial.print(" newlinecnt is ");
        Serial.println(newlinecnt);  
        if(rc=='\n')
        {
          newlinecnt=newlinecnt+1;
        }

        if (newlinecnt < 4) {
            receivedChars[ndx] = rc;
            ndx++;
            if (ndx >= numChars) {
                ndx = numChars - 1;
            }
        }
        else {
            receivedChars[ndx] = '\0'; // terminate the string
            ndx = 0;
            newData = true;
        }
    }
    entiremsg=String(receivedChars);
    
}
//Serial.print("newData is");
//Serial.println(newData);
//void showNewData() {
  //  if (newData == true) {
    //    Serial.print("This just in ... ");
     //   Serial.println(receivedChars);
      //  Serial.println(entiremsg);
      //  newData = false;
    //}
//}
Serial.println(entiremsg);
}
*/
}