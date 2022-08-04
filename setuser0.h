void SetUser0()
{
    if(user0==0) // checking if user0 in local RAM is empty
          {
          offsetaddr=0;
          writeUnsignedIntIntoEEPROM(offsetaddr,firstsender); // writes the cuntry code and the first digit into EEPROM
          firstuser0=readUnsignedIntFromEEPROM(offsetaddr); // Reads these 3 digits into firstuser0 from EEPROM

          offsetaddr=2;
          WriteLongIntoEEPROM(offsetaddr,sender); // writes the remaining 9 digits of the no. into EEPROM
          user0=ReadLongFromEEPROM(offsetaddr);
                              
          response=4;
          SendResponse();
          }
}
