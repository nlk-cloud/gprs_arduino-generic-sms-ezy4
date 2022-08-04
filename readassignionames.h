//For setting the bit to 1, OR it with that bit 1 and all others 0
//For resetting or setting a bit to 0, AND it with that bit 0 and all other bits 1

void ReadAssignioNames()
{
  uint8_t m;
  flag=flag&(0xFD);
  //String 
  kiwrd="CHANGE";
  ReadNamefromCommand(kiwrd,' ',' ',fromname);
  Serial.print("From name is");
  Serial.println(fromname);
  kiwrd="TO";
  ReadNamefromCommand(kiwrd,' ','\r',toname); // an sms is terminated with '/r' followed by '/n' when we press enter key. Hence we look for '\r' to identify the substring 
  Serial.print("To name is");
  Serial.println(toname);
   /*uint8_t p;
   uint8_t position=0;
   uint8_t position2=0;
   uint8_t position3=0;
       //ionameflag is the 2nd bit from LSB. To reset it to 0 and not disturb other bits, AND the flag with FD ie., 1111 1101
   
    position = entiremsg.indexOf("CHANGE");
     if(position != -1)
     {
        position2 = entiremsg.indexOf(' ', position+1); 
        position3 = entiremsg.indexOf(' ', position2+1);
        fromname=entiremsg.substring((position2+1), (position3)); 
        Serial.print("fromname is");
        Serial.println(fromname);
     }

    position = entiremsg.indexOf("TO");
     if(position != -1)
     {
        position2 = entiremsg.indexOf(' ', position+1); 
        position3 = entiremsg.indexOf('\n', position2+1);
        toname=entiremsg.substring((position2+1), (position3)); 
        Serial.print("to name is");
        Serial.println(toname);
     }
*/
// Name should not be more than 20 characters
     if(toname.length()>20)
     {
      flag=flag|(0x02);
      //ionameflag=true;
      }

// Name should not contain spaces
     if(toname.indexOf(" ") >0)
     {
      flag=flag|(0x02);
      //ionameflag=true;
     }

//Name should not contain _
      if(toname.indexOf("_") >0)
     {
       flag=flag|(0x02);
     // ionameflag=true;
     }
    
Serial.print("IO flag is:");
Serial.println(flag);
      if((flag&(0x02))==0)
      //if(!(flag&(0x02)))  // If name flag is not set, then continue and write in EEPROM
        { 
          
         //offsetaddr=42;
         for(m=0;m<4;m=m+1)
         {
         // Serial.print("m is");
         // Serial.println(m);
         // Serial.println(inputname[m]);
            if(fromname==inputname[m])
            {
              offsetaddr=42+(m*21);
              
               WriteCharIntoEprom(offsetaddr,toname);
               ReadCharFromEprom(offsetaddr,&inputname[m] );
               //Serial.print("input is:");
               //Serial.println(inputname[m]);
               response=13;
               SendResponse();
               break;
            }
           
         }

      //Serial.print("m is");
      //Serial.println(m);

        if(m==4) // m=4 implies, the from name was not found in input names
        {
         //offsetaddr=122;
         for(m=0;m<4;m=m+1)
         {
          
            if(fromname==outputname[m])
            {
               offsetaddr=126+(m*21);
               WriteCharIntoEprom(offsetaddr,toname);
               ReadCharFromEprom(offsetaddr,&outputname[m] );
               //Serial.print("output is:");
               //Serial.println(outputname[p]);
               response=13;
               SendResponse();
               break;
            }
            
         }
        }
         
         if(m==4) // m=4 implies the from name was not found in output names either

         {
          response=15;
          SendResponse();
         }
         }
         
   

     if (flag&0x02)
     {
      response=14;
      SendResponse();
     }
}
