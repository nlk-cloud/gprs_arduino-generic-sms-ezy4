
uint8_t settimeinflags()
{
    uint8_t p;
    

    
        for (p = 0; p < 4; p = p + 1)
        {
          
            if (fromname == outputname[p])
            {
              
         
                    if (toname.length()==0)
                       {
                        
                          outputstatusflag[p]=outputstatusflag[p]|(0x40); // setting the status bit of output
                          msg = "SInce no  ON time was specified, the output will remain ON, if turned ON ";
                          break;
                        }

                        else
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

// outputstarted flag should be set before coming to this function
// The logic to which the output should be driven, is taken from the output status flag in this function

void driveoutputonoff( uint8_t r )
{
    uint8_t p,t,lnkedoutput,status;

    status = (outputstatusflag[r]>>6);

     if((outputstarted[r]==1)&&(outputdlyattained[r]==0)||(outputstatusflag[r]>>6))
     {
      digitalWrite(outchannel[r],status);

      
    
      if(((outputstatusflag[r]&(0x04))>>2)== 1) // check if this output is linked to any other output
      { 
            lnkedoutput=(outputstatusflag[r]&(0x18))>>3; // extract the information about which output it is linked to
            digitalWrite(outchannel[lnkedoutput],status); 
      }
      
    }

   p=outputstatusflag[r]&(0x03); // check the units of delay - mins or hrs or days
   
   switch (p)
    {
     // No time is specified. It is just 'TURN ON' or 'TURN OFF'
        case 0:    
        outputdlyattained[r]=0;
        break;

    // time specified is in mins
        case 1:
        
         outputdlyattained[r]=dly(currtimeoutput[r],0,0,outputontime[r],0); //timekeeping.h 
               
        break;


    // time specified is in hrs
        case 2:
        
          outputdlyattained[r]=dly(currtimeoutput[r],0,outputontime[r],0,0); //timekeeping.h 
        
        break;

    // time specified is in days
        case 3:
        
          outputdlyattained[r]=dly(currtimeoutput[r],outputontime[r],0,0,0); //timekeeping.h 
        
        break;

    }

if(outputdlyattained[r]==1)
        {
          outputstarted[r]=0;
         
          outputstatusflag[r]=(outputstatusflag[r]&(0xBF));
          offsetaddr = 752 + (r * 1);
          EEPROM.write(offsetaddr, outputstatusflag[r]);
          outputstatusflag[r] = EEPROM.read(offsetaddr);
          
          digitalWrite(outchannel[r],!status);
         if((outputstatusflag[r]&(0x04))== 1) // write the compliment in the linked output as well
        {
          outputstatusflag[lnkedoutput]=(outputstatusflag[lnkedoutput]&(0xBF));
          offsetaddr = 752 + (lnkedoutput * 1);
          EEPROM.write(offsetaddr, outputstatusflag[lnkedoutput]);
          outputstatusflag[lnkedoutput] = EEPROM.read(offsetaddr);
            
          digitalWrite(outchannel[lnkedoutput],!status);
        }
        outputstatusflag[r] = EEPROM.read(offsetaddr);
        outputstatusflag[lnkedoutput] = EEPROM.read(752+lnkedoutput*1);
        outputdlyattained[r]=0;
        }

}

void turnandsetoutput()
{
     uint8_t q,n,m, tmp=0;
    // String
    fromname="";
    toname="";
    msg="";
    if(entiremsg.indexOf("SET")>0)
    {
    kiwrd = "SET";
    ReadNamefromCommand(kiwrd, ' ', ' ', fromname); // which output is stored in fromname
    kiwrd = "TO";
    ReadNamefromCommand(kiwrd, ' ', ' ', toname); // reead and store the time in toname
    q=settimeinflags();
    if(q==4)
    {msg="This name doesnt exist. Please check";}
    else
    {
    offsetaddr = 752 + (q * 1);
    EEPROM.write(offsetaddr, outputstatusflag[q]);
    outputstatusflag[q] = EEPROM.read(offsetaddr);
    offsetaddr = 756 + (q * 1);
    EEPROM.write(offsetaddr, outputontime[q]);
    outputontime[q] = EEPROM.read(offsetaddr);
    }
    
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

if(q==4)
    {
      msg="This name doesnt exist. Please check";
      tmp=1;
     }
     else
     {
        // check if the output is linked to any input 
        for( m=0;m<4;m++)
        {
              if (((inputstatusflag[m]&0x04)>>2)==1) // checking if the input is linked to an output
          { 
              if(q==(inputstatusflag[m]&(0x18))>>3) // checking if the linked input is linked to this particular output
              {
                msg="This output is linked to an input. Please delink the output first";
                tmp=1;
                break;
              }
          }
        }
    
    
   
    if(tmp==0) // continue only of the output is not linked
    {
        if(entiremsg.indexOf("ON")>0)
      {
      
        offsetaddr = 752 + (q * 1);
        EEPROM.write(offsetaddr, outputstatusflag[q]);
        outputstatusflag[q] = EEPROM.read(offsetaddr);
        outputstarted[q]=1;   // Flag to say that output has started
        driveoutputonoff(q);
        
        
      }
        else
        if(entiremsg.indexOf("OFF")>0)
      {
          outputstatusflag[q]=(outputstatusflag[q]&(0xBF));
          offsetaddr = 752 + (q * 1);
          EEPROM.write(offsetaddr, outputstatusflag[q]);
          outputstatusflag[q] = EEPROM.read(offsetaddr);
          //Serial.print("outputstatusflag is:");
          //Serial.println( outputstatusflag[q]);

          outputstarted[q]=1;   // Flag to say that output has started
          driveoutputonoff(q);
          msg="The output has been turned off";
      }
     
    }
    }
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
