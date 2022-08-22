
uint8_t settimeinflags()
{
    uint8_t p;
    
if (toname.length()==0)
    {
        outputstatusflag[p]=outputstatusflag[p]&(0x00);
        msg = "SInce no  ON time was specified, the output will remain ON, if turned ON ";
    }
    else
    {
        for (p = 0; p < 4; p = p + 1)
        {
            if (fromname == outputname[p])
            {
                        
                if (entiremsg.indexOf("DAYS") > 0)
                {
                    outputstatusflag[p]=outputstatusflag[p]|(0x03); // sets last two bits to 11
                    outputontime[p] = (toname.toInt());
                    msg = " The " + outputname[p] + "ON time has been set to " + toname + " DAYS";
                    break;
                }
                
                if (entiremsg.indexOf("HRS") > 0)
                {
                    outputstatusflag[p]=outputstatusflag[p]|(0x02); // sets last but one bit to one
                    outputstatusflag[p]=outputstatusflag[p]&(0xFE); // resets last  bit to 0
                    outputontime[p] = (toname.toInt());
                    msg = " The " + outputname[p] + "ON time has been set to " + toname + " HRS";
                    break;
                }
                
                if (entiremsg.indexOf("MIN") > 0)
                {
                    outputstatusflag[p]=outputstatusflag[p]|(0x01); // sets last bit to one
                    outputstatusflag[p]=outputstatusflag[p]&(0xFD); // resets last but one bit to 0
                    outputontime[p] = (toname.toInt());
                    msg = " The " + outputname[p] + "ON time has been set to " + toname + " MIN";
                    break;
                }
                else
                { 
                    msg="Sorry. You can only set  mins, hrs or days. Please check and try again.";
                }
            }
        }
    }
    return(p);
}

//**** use this subroutine only to drive outputs high. By default, all outputs will be at 0. If this is
// used to drive 0 also, then after the specified time, that output will go to its compliment i.e., it will
// come back to 1***/
void driveoutputonoff( uint8_t r, uint8_t status)
{
    uint8_t m,t,lnkedoutput;
     
     digitalWrite(outchannel[r],status);
    
    if(((outputstatusflag[r]&(0x04))>>2)== 1) // check if this output is linked
    { 
        lnkedoutput=(outputstatusflag[r]&(0x18))>>3; // extract the information about which output it is linked to
         digitalWrite(outchannel[lnkedoutput],status); 
    }
   
    
    m=outputstatusflag[r]&(0x03); // check the units of delay - mins or hrs or days
   // Serial.print("m(output delay units) is ");
   // Serial.println(m);
    //Serial.println(!status);
    //Serial.println(~status);
    switch (m)
    {
        case 0:    
       // Serial.println("entered case 0");
        break;

    // time specified is in mins
        case 1:
        for(t=0;t<outputontime[r];t=t+1)
        {
            //Serial.println("t is");
            //Serial.println(t);
            delay(60000); // each min=60sec or 60000millisecs
        }
        digitalWrite(outchannel[r],!status);
         if((outputstatusflag[r]&(0x04))== 1) 
        {
            digitalWrite(outchannel[lnkedoutput],!status);
        }
        outputstatusflag[r] = EEPROM.read(offsetaddr);
        //outputstatusflag[lnkedoutput] = EEPROM.read(752+lnkedoutput*1);
        break;


    // time specified is in hrs
        case 2:
        for(t=0;t<outputontime[r];t=t+1)
        {
            delay(3600000); // each 1hour=3600sec or 3600000millisecs
        }
        digitalWrite(outchannel[r],!status);
         if((outputstatusflag[r]&(0x04))== 1) 
        {
            digitalWrite(outchannel[lnkedoutput],!status);
        }
        outputstatusflag[r] = EEPROM.read(offsetaddr);
        //outputstatusflag[lnkedoutput] = EEPROM.read(752+lnkedoutput*1);
        break;

    // time specified is in days
        case 3:
        for(t=0;t<outputontime[r];t=t+1)
        {
            delay(3600000); // each 1hour=3600sec or 3600000millisecs
        }
        digitalWrite(outchannel[r],!status);
         if((outputstatusflag[r]&(0x04))== 1) 
        {
            digitalWrite(outchannel[lnkedoutput],!status);
        }
        outputstatusflag[r] = EEPROM.read(offsetaddr);
        //outputstatusflag[lnkedoutput] = EEPROM.read(752+lnkedoutput*1);
        break;

    }
}



void turnandsetoutput()
{
     uint8_t q,n;
    // String
    fromname="";
    toname="";
    
    if(entiremsg.indexOf("SET")>0)
    {
    kiwrd = "SET";
    ReadNamefromCommand(kiwrd, ' ', ' ', fromname); // which output is stored in fromname
    kiwrd = "TO";
    ReadNamefromCommand(kiwrd, ' ', ' ', toname); // reead and store the time in toname
    q=settimeinflags();
    offsetaddr = 752 + (q * 1);
    EEPROM.write(offsetaddr, outputstatusflag[q]);
    outputstatusflag[q] = EEPROM.read(offsetaddr);
    offsetaddr = 756 + (q * 1);
    EEPROM.write(offsetaddr, outputontime[q]);
    outputontime[q] = EEPROM.read(offsetaddr);
    }
    else if(entiremsg.indexOf("TURN")>0)
    {
        Serial.println("entered turn");
    kiwrd = "TURN";
    ReadNamefromCommand(kiwrd, ' ', ' ', fromname);
    kiwrd = "FOR";
    ReadNamefromCommand(kiwrd, ' ', ' ', toname);
    q=settimeinflags(); // here dont write into EEPROM since this is a one time event
    Serial.print("q is");
    Serial.println(q);
   
    if(entiremsg.indexOf("ON")>0)
    {
       n=1; 
    }
    else
    if(entiremsg.indexOf("OFF")>0)
    {
       n=0; 
    }
    driveoutputonoff(q,n);  
    
    }
    

   response=16;
   SendResponse(); 
   
}
   
// Sets and clears links between outputs    
void linkout()
{
    uint8_t u,v;
    if (entiremsg.indexOf("CLEAR")>0)
    {
        for(u=0;u<4;u=u+1)
        {
            outputstatusflag[u]=outputstatusflag[u]&(0xE3);
            offsetaddr = 752 + (u * 1);
            EEPROM.write(offsetaddr, outputstatusflag[u]);
            outputstatusflag[u] = EEPROM.read(offsetaddr); 
        }
        msg="All linked outputs have been cleared";
    }

    else
    {
        fromname="";
        toname="";
        Serial.println("entered else of linkout");
        kiwrd = "OUTPUTS";
        ReadNamefromCommand(kiwrd, ' ', ' ', fromname); // which output is linked in fromname
        kiwrd = "AND";
      
        ReadNamefromCommand(kiwrd,' ','\r', toname); // read and store the output no. which is linked in toname
        u=fromname.toInt()-1; // internal output names start from 0. hence subtracting 1
        v=toname.toInt()-1; // internal output names start from 0. hence subtracting 1
        Serial.println("u and v are: ");
        Serial.println(u);
        Serial.println(v);
        v=((v&0x03)<<3); //Take to name bits to bit position 4th and 5th 
        Serial.println(v);
        //v=(v<<3);
        //Serial.println(v);
        v= v|(0x04); // set bit 2 to 1, to indicate this output is linked
        Serial.println(v);
        Serial.println(outputstatusflag[u]);
        outputstatusflag[u]=outputstatusflag[u]|(v);
        Serial.println(outputstatusflag[u]);
        if(u<4)
        {
        offsetaddr = 752 + (u * 1);
        EEPROM.write(offsetaddr, outputstatusflag[u]);
        outputstatusflag[u] = EEPROM.read(offsetaddr); 
        msg="The outputs have been linked";
        }
        else if(u>=4)
        { msg="The outputs have NOT been linked. Please check and send again";}
    }

    response=16;
    SendResponse();
}