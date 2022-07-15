void ReadString()
{
if ((mySerial.available()>0)&&(newData==false))  
    {//mySerial.flush();
      Serial.println("entered msgrd");    
      //entiremsg=mySerial.readStringUntil('\r');
      entiremsg=mySerial.readString();
      Serial.println(entiremsg);
      newData = true;
    }
}

