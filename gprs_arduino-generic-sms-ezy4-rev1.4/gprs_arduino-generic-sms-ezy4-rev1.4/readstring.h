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

void ReadNamefromCommand(const String &command, char from, char to, const String &name)
{
    uint8_t p;
    uint8_t position = 0;
    uint8_t position2 = 0;
    uint8_t position3 = 0;
    name = "";
    position = entiremsg.indexOf(command);


    // Read the name of the input
    if (position != -1)
    {
        position2 = entiremsg.indexOf(from, position + 1);
        position3 = entiremsg.indexOf(to, position2 + 1);
        name = entiremsg.substring((position2 + 1), (position3));
        Serial.print("name is");
        Serial.println(name);
        Serial.println(name.length());
    }
}
