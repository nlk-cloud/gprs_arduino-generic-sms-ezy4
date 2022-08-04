void ReadString()
{
if ((mySerial.available()>0)&&(flag==0))  
    {//mySerial.flush();
      Serial.println("entered Readstring");    
      //entiremsg=mySerial.readStringUntil('\r');
      entiremsg=mySerial.readString();
      Serial.println(entiremsg);
      flag=1;
    }
}
