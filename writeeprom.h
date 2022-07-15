int WriteEprom(int addrOffset, const String &strToWrite)
{

// Add to the EEPROM
Serial.println(" Entered EEPROM write");
Serial.println(strToWrite);
Serial.println(addrOffset);
  byte len = strToWrite.length();
  Serial.println(len);
  EEPROM.write(addrOffset, len);
  for (i = 0; i < len; i++)
  {
    EEPROM.write(addrOffset + 1 + i, strToWrite[i]);
  }
  
  return addrOffset + 1 + len;

}
  
  /*
    //epromadd+=sizeof(numtoincldel);
    EEPROM.put(epromadd, numtoincldel);
    //Serial.println(epromadd);
    
 // Send msg to the number that was included   
    msg="Added to the list";
    sendto=numtoincldel;
    SendMessage();//sending msg to the no which was included


// Send msg to admin
    if (replyfrommodem =="ERROR")
    { 
      msg="check the number to be included or deleted";
      sendto=sender;
      SendMessage();
    }
else     
{
  msg=msg1;
  sendto=sender;
SendMessage(); //sending msg to the admin  
}
//Serial.print(" size of");
//Serial.println(sizeof(numtoincldel));
//EEPROM.get(epromadd, num);
//Serial.print(" Eprom stored string is ");
//Serial.println(num);

delay(3000);

}*/