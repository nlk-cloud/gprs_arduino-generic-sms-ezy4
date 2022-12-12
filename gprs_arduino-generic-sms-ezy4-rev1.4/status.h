void InputStatus()
{   
    uint8_t a;
    msg="currently the ";
    //Serial.println("Entered input status");
 
 for(a=0;a<4;a=a+1)
 {
  if((inputstatusflag[a]&(0x02))==0)
  {
    msg=msg+"non-latching input ";
   }
   else 
   {
    msg=msg+"latching input ";
   }

  msg=msg+inputname[a]+" is ";
   if((digitalRead(inchannel[a]))==0)
   {
    msg=msg+"OFF ";
   }
   else if((digitalRead(inchannel[a]))==1)
   {
    msg=msg+"ON ";
   }

   if((inputstatusflag[a]&(0x20))>>5==1)
   {
    msg=msg+" and Latched";
   }
   msg=msg+",";
 }
 response=16;
 SendResponse();
}
 
 void outputstatus()
 {
  uint8_t a;
    msg="currently  ";
    //Serial.println("Entered input status");
 
 for(a=0;a<4;a=a+1)
 {
  msg=msg+outputname[a]+" is ";
 
  if( (digitalRead(outchannel[a]))==0)
  {
    msg=msg+"OFF, ";
  }
  else if( (digitalRead(outchannel[a]))==1)
  {
    msg=msg+"ON, ";
  }
 }
 
  response=16;
 SendResponse();
  }

 
 
   
