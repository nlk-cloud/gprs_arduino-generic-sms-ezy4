void Deluser()
{
  uint8_t m;
Serial.println("Entered DEL");
 from=10;
 to=23;              
 ReadVerifyNumber(); // to extract the number to be deleted
 if ((!(flag &(0x08)))&& (sender==admin))
 {  
  CheckifUserExists();
  if(!(flag&(0x04)))
      {
        response=7;
        SendResponse();
      }
  if((flag&(0x04)) && (usercnt==1))  // there is only one user and hence cannot delete
    {
      response=8;
      SendResponse();
    }    
 if((flag&(0x04)) && (usercnt>1))
    {      
        //Identify the position from where the number has to be deleted
        for(m=0;m<5;m++)

        {
            if((numtoincldel==usernum9digits[m])&&(firstnumtoincldel==usernum3digits[m]))
            {
                usernum3digits[m]=0;
                usernum9digits[m]=0;
                offsetaddr=12+(m*6);
                writeUnsignedIntIntoEEPROM(offsetaddr,0);
                offsetaddr=14+(m*6);
                WriteLongIntoEEPROM(offsetaddr,0);
                break;
            }
        }    
        
        usercnt=usercnt-1;
        response=9;
        SendResponse();
    
    }
      
   } 
   
}
