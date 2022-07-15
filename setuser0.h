void SetUser0()
{
    if(user0=="") // checking if user0 in local RAM is empty
          {
          addrOffsetuser0=0;
          addrOffsetadmin=WriteEprom(addrOffsetuser0, sender);    //write into the EEPROM and generate addr offset for the admin         
          ReadEprom(addrOffsetuser0,&user0 ); // reads the EEPROM into user0 string
          response=4;
          SendResponse();
          }
}
