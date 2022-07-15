void Incl()
{
    //flag=false;
    //if(numflag==false)
    {
        if (smstxt2=="ADMIN") // setting admin 
          {  
            addrOffsetuser1=WriteEprom(addrOffsetadmin, numtoincldel); 
            ReadEprom(addrOffsetadmin,&admin ); 
            response=5;
            SendResponse();
        } 

        else
        {
          CheckifUserExists();

          
          if(userflag==false) // by this time, number is good and user doesnt exist. so goahead and add the number    
          {
            if ((smstxt2=="USER1")&&(sender==admin))
                {
        
                    addrOffsetuser2=WriteEprom(addrOffsetuser1, numtoincldel); 
                    ReadEprom(addrOffsetuser1,&recipientNumber[0] ); // reads the EEPROM into recipient number array
          // EPROM read and write happens in character by character of the string. But we need ot handle strings
          // Hence the string array ( recipient array ) should be kept uptodate so that it has the live status of the EEPROM
          // Hence, after every EEPROM write ( add or del), the EEPROM is read and contents are stored in the string array
                    response=1;
                    SendResponse();
                }
                
            if ((smstxt2=="USER2")&&(sender==admin))
        
                {
                    addrOffsetuser3=WriteEprom(addrOffsetuser2, numtoincldel);          
                    ReadEprom(addrOffsetuser2,&recipientNumber[1] );
                    response=1;
                    SendResponse();
                }

            if ((smstxt2=="USER3")&&(sender==admin))     
                {    
                    addrOffsetuser4=WriteEprom(addrOffsetuser3, numtoincldel);
                    ReadEprom(addrOffsetuser3,&recipientNumber[2] );
                    response=1;
                    SendResponse();        
                }

            if ((smstxt2=="USER4")&&(sender==admin))
                { 
                    addrOffsetuser5=WriteEprom(addrOffsetuser4, numtoincldel); 
                    ReadEprom(addrOffsetuser4,&recipientNumber[3] );
                    response=1;
                    SendResponse();   
                }
            if ((smstxt2=="USER5")&&(sender==admin))
                {
                    WriteEprom(addrOffsetuser5, numtoincldel); 
                    ReadEprom(addrOffsetuser5,&recipientNumber[4] );
                    response=1;
                    SendResponse();

        
                }
          }
    }
    }          
}