// To check if a flag is set, AND it with that bit 1 and all others 0 and put it in if directly. DOnt store it back in the flag. THis way, flag will be not be affected

void InclUser()
{
  uint8_t k;
  Serial.println("Entered incl user");
    Serial.println(usercnt);
    if(usercnt==5)
    {
      response=11;
      SendResponse();                      
    }                         
    else
    {
      from=11;
      //to=24;

     // Read the number to include or delete  
      ReadVerifyNumber(); // if number is faulty numflag is set
      
      if(!(flag&0x08)) // if number is not faulty, then check if user exists
      {
        CheckifUserExists();

      }
        if (flag&(0x04))
        {
            response=0;
            SendResponse();
        }
        if(!(flag &(0x0C))) // proceed if both numflag and user flag are not set
       {
                       
          if ((sender==admin)&&(firstsender==firstadmin))
            {
            for(k=0;k<5;k++)
            {

                if((usernum3digits[k]==0)&&(usernum9digits[k]==0))
                {
                    offsetaddr=12+(k*6);
                    writeUnsignedIntIntoEEPROM(offsetaddr,firstnumtoincldel);
                    usernum3digits[k]=readUnsignedIntFromEEPROM(offsetaddr);
                    offsetaddr=14+(k*6);
                    WriteLongIntoEEPROM(offsetaddr,numtoincldel);
                    usernum9digits[k]=ReadLongFromEEPROM(offsetaddr);
                                       
                    usercnt=usercnt+1;
                    response=1;
                    SendResponse();
                    break;

                }
            }
          }
    }
    
             
  }
}


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
