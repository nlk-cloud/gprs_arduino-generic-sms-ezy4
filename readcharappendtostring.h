void ReadCharAppendtoString()
{
    entiremsg="";
     char rc;
     if(mySerial.available()>0)
{    
Serial.println("entered if");
    while (mySerial.available() > 0 && newData == false)
     {
Serial.println("entered while");
        rc = mySerial.read();
        entiremsg=entiremsg+rc;

    }
}
Serial.println(entiremsg);
newData=true;
}