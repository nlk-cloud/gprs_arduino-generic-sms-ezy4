//For setting the bit to 1, OR it with that bit 1 and all others 0
//For resetting or setting a bit to 0, AND it with that bit 0 and all other bits 1

void LatchandUnlatch()
{
   uint8_t p;
   uint8_t position=0;
   uint8_t position2=0;
   uint8_t position3=0;
   latchflag=0;    //ionameflag is the 2nd bit from LSB. To reset it to 0 and not disturb other bits, AND the flag with FD ie., 1111 1101
   
    position = entiremsg.indexOf("MAKE");
     if(position != -1)
     {
        position2 = entiremsg.indexOf(' ', position+1); 
        position3 = entiremsg.indexOf(' ', position2+1);
        fromname=entiremsg.substring((position2+1), (position3)); 
        Serial.print("fromname is");
        Serial.println(fromname);
     }

  if(entiremsg.indexOf("NONLATCHING")>0)
     {
            for (p=0;p<4,p++)
            {
                if(fromname==inputname[p])
                {
                    latchflag=latchflag&(0xFE << p);
                    msg="The input "+inputname[p]+" is now a nonlatching input";
                    response=16;
                    SendResponse();
                    break;
        
                }
            }
     }

     else
     {   
     
     if(entiremsg.indexOf("LATCHING")>0)
     {
            for (p=0;p<4,p++)
            {
                if(fromname==inputname[p])
                {
                    latchflag=latchflag|(0x01 << p);
                    msg="The input "+inputname[p]+" is now a latching input";
                    response=16;
                    SendResponse();
                    break;
        
                }
            }
     }
     }


}