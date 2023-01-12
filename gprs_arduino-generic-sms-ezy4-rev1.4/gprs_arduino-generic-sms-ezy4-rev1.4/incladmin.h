void InclAdmin()
{
    Serial.println("Entered incladmin");  
    from=11;
    //to=24;
   
   //Read the number to include or delete  
    ReadVerifyNumber(); // if number is faulty numflag is set
   
    if (((flag&(0x08))==false) && (sender==user0))
      {           
        offsetaddr=6;
          writeUnsignedIntIntoEEPROM(offsetaddr,firstnumtoincldel); // writes the cuntry code and the first digit into EEPROM
          firstadmin=readUnsignedIntFromEEPROM(offsetaddr); // Reads these 3 digits into firstuser0 from EEPROM

          offsetaddr=8;
          WriteLongIntoEEPROM(offsetaddr,numtoincldel); // writes the remaining 9 digits of the no. into EEPROM
          admin=ReadLongFromEEPROM(offsetaddr);
          
          response=5;
          SendResponse(); 
               
      }
}
