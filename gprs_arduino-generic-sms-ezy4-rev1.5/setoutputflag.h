//currtimeoutput - contains the time at which output started
//


void drivelogicopandlinkedop(uint8_t outputno, bool logic )
{
 
uint8_t lnkedoutput;
      digitalWrite(outchannel[outputno],logic);
      outputstarted[outputno]=1;
      
    
      if(((outputstatusflag[outputno]&(0x04))>>2)== 1) // check if this output is linked to any other output
      { 
            lnkedoutput=(outputstatusflag[outputno]&(0x18))>>3; // extract the information about which output it is linked to
            digitalWrite(outchannel[lnkedoutput],logic); 
            outputstarted[lnkedoutput]=1;
  
      }
}





// when output is started, checks if delay is attained or not
void outputstartedaction(uint8_t b)
{
  uint8_t p;
  
if ( outputstarted[b]==1)
{
   
   p=outputstatusflag[b]&(0x03); // check the units of delay - mins or hrs or days
 
   switch (p)
    {
     // No time is specified. It is just 'TURN ON' or 'TURN OFF'
        case 0:    
          outputdlyattained[b]=0;
        break;

    // time specified is in mins
        case 1:
          outputdlyattained[b]=dly(currtimeoutput[b],0,0,outputontime[b],0); //timekeeping.h  
        break;


    // time specified is in hrs
        case 2:
           outputdlyattained[b]=dly(currtimeoutput[b],0,outputontime[b],0,0); //timekeeping.h 
        break;

    // time specified is in days
        case 3:
           outputdlyattained[b]=dly(currtimeoutput[b],outputontime[b],0,0,0); //timekeeping.h 
         break;

    }
}

}

// delay has been attained. NOw, the output has to be switched off and various memory locations have to reset
// THis function assumes that delay attained will come only in the case when output was switched on. When output is turned off, there is no delay

void delayattainedaction(uint8_t a)
{
  uint8_t lnkedop,stat;

    
 
 if((outputdlyattained[a]==1)||(outputdlyattained[a]==(-1)))
        {
          outputstarted[a]=0;
          // Write back the output start time to 0
           offsetaddr = 766 + (a * 6);
         EEPROM.write(offsetaddr,0);     // Write year
         EEPROM.write((offsetaddr+1),0); // Write month
         EEPROM.write((offsetaddr+2),0); // write day
         EEPROM.write((offsetaddr+3),0); // Write hour
         EEPROM.write((offsetaddr+4),0); // Write min
         EEPROM.write((offsetaddr+5),0); // Write sec
          
          
          Readcurrtimeoutput(); // Read the output starttime into the currtimeoutput variable ( i.e., bring back the variable value to 0
          
         if((((outputstatusflag[a])&(0x80))>> 7)==1)  // output was turned on using turn on command
         {
          outputstatusflag[a]=(outputstatusflag[a]&(0x3C)); // Make the first and last 2 bits 0 indicating output is off now and can take turn on command again        
         }
         else
         {
          outputstatusflag[a]=(outputstatusflag[a]&(0xBF)); // just turn off the output and keep other bits unchanged
         }
          offsetaddr = 752 + (a * 1);
          EEPROM.write(offsetaddr, outputstatusflag[a]);
          outputstatusflag[a] = EEPROM.read(offsetaddr);

          stat = ((outputstatusflag[a]&(0x40))>>6);
          
          digitalWrite(outchannel[a],stat);
         if((outputstatusflag[a]&(0x04))== 1) // write the compliment in the linked output as well
        {
          outputstatusflag[lnkedop]=(outputstatusflag[lnkedop]&(0xBF));
          offsetaddr = 752 + (lnkedop * 1);
          EEPROM.write(offsetaddr, outputstatusflag[lnkedop]);
          outputstatusflag[lnkedop] = EEPROM.read(offsetaddr);
            
          digitalWrite(outchannel[lnkedop],stat);
        }
        outputstatusflag[a] = EEPROM.read(offsetaddr);
        outputstatusflag[lnkedop] = EEPROM.read(752+lnkedop*1);
        outputdlyattained[a]=0;
        }

 if(outputdlyattained[a]==0)
    {
    outputstarted[a]=1;
    outputdlyattained[a]=0;
    //drivelogicopandlinkedop(r, status);
    //outputstartedaction(r); // Checks if the delay has been attained or not
    //delayattainedaction(r);// delay attained. So, write the compliment
    
   }
/*    else if (outputdlyattained[a]==(-1)) // power has come back after window time. switch off the outputs
    {
      outputstarted[a]=0;
      outputdlyattained[r]=1;
      delayattainedaction(r);
    }
 */
}

// checks if any input is linked to the output
boolean checkoutputlinkedtoinp(uint8_t outputno)
{
  uint8_t m,r=0;
  
  for( m=0;m<4;m++)
        {
              if (((inputstatusflag[m]&0x04)>>2)==1) // checking if the input is linked to an output
          { 
            Serial.println("Entered first if");
            Serial.print(" m is");
            Serial.println(m);
            Serial.println(inputstatusflag[m]);
            Serial.println((inputstatusflag[m]&(0x18)));
            Serial.println(((inputstatusflag[m]&(0x18))>>3));
            Serial.println(outputno);
              if(outputno==((inputstatusflag[m]&(0x18))>>3)) // checking if the linked input is linked to this particular output
              {
                Serial.println("Entered second if");
                r=1;
                break;
              }
          }
        }
        Serial.print("r is");
        Serial.println(r);
        return(r);
}



void setnwriterdepromoutputontimetonow(uint8_t outputno)
{
       currtimeoutput[outputno]=rtc.now();
         
        offsetaddr = 766 + (outputno * 6);
        
         EEPROM.write(offsetaddr, uint8_t((currtimeoutput[outputno].year())-(0x7D0))); // extract the year part of the total year ie., extract 23 from the year 2023 and convert it to uint8_t
         EEPROM.write((offsetaddr+1), (currtimeoutput[outputno].month())); // extract the month and write it in offsetaddr+1 
         EEPROM.write((offsetaddr+2), (currtimeoutput[outputno].day())); // extract the day and write it in offsetaddr+2
         EEPROM.write((offsetaddr+3), (currtimeoutput[outputno].hour())); // extract the hour and write it in offsetaddr+3
         EEPROM.write((offsetaddr+4), (currtimeoutput[outputno].minute())); // extract the minute and write it in offsetaddr+4
         EEPROM.write((offsetaddr+5), (currtimeoutput[outputno].second())); // extract the second and write it in offsetaddr+5
        
        
         currtimeoutput[outputno]=DateTime((int(EEPROM.read(offsetaddr))+(0x7D0)),EEPROM.read(offsetaddr+1),EEPROM.read(offsetaddr+2), EEPROM.read(offsetaddr+3), EEPROM.read(offsetaddr+4), EEPROM.read(offsetaddr+5)); // Read the year, convert it into int and add 2000
         
         timeprint(currtimeoutput[outputno]);
       
 }




uint8_t settimeinflags()
{
    uint8_t p;
    

    
        for (p = 0; p < 4; p = p + 1)
        {
          
            if (fromname == outputname[p])
            {
              
         
                    if (toname.length()==0)
                       {
                        
                          outputstatusflag[p]=(outputstatusflag[p]|(0x40)); // setting the status bit of output
                          outputontime[p] = 0;
                          msg = "SInce no  ON time was specified, the output will remain ON, if turned ON ";
                          break;
                        }

                        else
                       {
                        
                               if (entiremsg.indexOf("DAYS") > 0)
                              {
                                outputstatusflag[p]=(outputstatusflag[p]|(0x03)); // sets last two bits to 11
                                outputontime[p] = (toname.toInt());
                                msg = " The " + outputname[p] + "ON time has been set to " + toname + " DAYS";
                                break;
                              }
                
                              if (entiremsg.indexOf("HRS") > 0)
                              {
                                outputstatusflag[p]=(outputstatusflag[p]|(0x02)); // sets last but one bit to one
                                outputstatusflag[p]=(outputstatusflag[p]&(0x7E)); // resets last  bit to 0 and MSB to 0 to indicate setting time
                                outputontime[p] = (toname.toInt());
                                msg = " The " + outputname[p] + "ON time has been set to " + toname + " HRS";
                                break;
                              }
                
                            if (entiremsg.indexOf("MIN") > 0)
                            {
                              outputstatusflag[p]=(outputstatusflag[p]|(0x01)); // sets last bit to one
                              outputstatusflag[p]=(outputstatusflag[p]&(0x7D)); // resets last but one bit to 0 and MSB to 0 to indicate setting time
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
    uint8_t p,t,status;
    //boolean logic1, logic2, logic3, logic4;
    
    status = ((outputstatusflag[r]&(0x40))>>6);
    //logic1 = (((outputstarted[r]==1)&(outputdlyattained[r]==0))); // output has started and delay not attained or output was set to 1
    //logic2 = (rtc.now() < (currtimeoutput[r]+outputontime[r]));
    //logic3 = (rtc.now() = (currtimeoutput[r]+outputontime[r]))
    //logic3 = (rtc.now() > (currtimeoutput[r]+outputontime[r]));
    //logic4 = ((currtimeoutput[r].day())&(currtimeoutput[r].hour())&(currtimeoutput[r].minute())&(currtimeoutput[r].second()));
// Keep writing the same logic since the output has started, but delay has not yet been acheived

  

//Serial.print("currtimeoutput is ");
 //timeprint(currtimeoutput[0]);
  
if(status==1)  // if status is 1, implies output was switched on
{
 // Serial.println("entered status if ");
  
  if ((outputstarted[r]==1)&&(outputdlyattained[r]==0)) // check if output has started and delay has not been attained. Implies it is a normall cycle. Need to switch the output on and check for delay attained
  {
    drivelogicopandlinkedop(r, status);
    outputstartedaction(r); // Checks if the delay has been attained or not
    delayattainedaction(r);// delay attained. So, write the compliment
  }
  else if (((currtimeoutput[r].day())!=0)) //&&(rtc.now() <= (currtimeoutput[r]+outputontime[r]))) // If the above if is not satisfied, only then it will come here and check this if 
  //which means power had gone, Now, power has come back, so outputstarted and output delayattained have become 0. we have to check if power has come back
  // within the window time. so make the outputstarted=1, and check for the dly attained value
  {
    Serial.println("power combeack 1 if "); 
    
   // check if the time has crossed the outputon interval by going to outputstartedaction(r) and looking for outputdlyattained[r]
   outputstarted[r]=1;
   outputstartedaction(r); // returns the delay attained value
   Serial.print("delay attained value is ");
   Serial.println(outputdlyattained[r]);
   delayattainedaction(r); // Based on the returned value action will be taken in the delay attained function
   
}
}

  
else if(status==0) // since status=0, output was not switched on. So,need to drive 0 
  {
    drivelogicopandlinkedop(r, status);
  }

}

/*

     if(logic1==1)
     {
      if ( currtimeoutput[r]!=0) // checking if power went off before the output completed its on time
      { 
        if ( logic2==1) // checking if power has come back before the output completed its on time )
      {
      digitalWrite(outchannel[r],status);
      outputstarted[r]=1;
      
    
      if(((outputstatusflag[r]&(0x04))>>2)== 1) // check if this output is linked to any other output
      { 
            lnkedoutput=(outputstatusflag[r]&(0x18))>>3; // extract the information about which output it is linked to
            digitalWrite(outchannel[lnkedoutput],status); 
            outputstarted[lnkedoutput]=1;
      }
      
    }


    outputstartedaction(r); // Checks if the delay has been attained or not
    delayattainedaction(r);// delay attained. So, write the compliment

 if((logic3)==1) // power has comeback after the output on time delay is over
{
  delayattainedaction(r);// delay attained. So, write the complimen
}
}
*/

void turnandsetoutput()
{
     uint8_t q,n,tmp=0, temporary=0;
    // String
    fromname="";
    toname="";
    msg="";

    // SET is used to set the output ON timing, which will be used only when output is linked to input.
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
    if(checkoutputlinkedtoinp(q)==1) // output is linked to input
    {
    offsetaddr = 752 + (q * 1);
    EEPROM.write(offsetaddr, outputstatusflag[q]);
    outputstatusflag[q] = EEPROM.read(offsetaddr);
    offsetaddr = 756 + (q * 1);
    EEPROM.write(offsetaddr, outputontime[q]);
    outputontime[q] = EEPROM.read(offsetaddr);
    }
    else
    {
      msg="This output is not linked to input, Hence output ON time cannot be set";
    }
    
    }
    
    else if(entiremsg.indexOf("TURN")>0)
    {
        Serial.println("entered turn");


    kiwrd = "TURN";
    ReadNamefromCommand(kiwrd, ' ', ' ', fromname);
    kiwrd = "FOR";
    ReadNamefromCommand(kiwrd, ' ', ' ', toname);
    q=settimeinflags(); // 
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
        tmp=checkoutputlinkedtoinp(q);
        Serial.print("tmp is");
        Serial.println(tmp); 
        if(tmp==1)
        {
        msg="This output is linked to an input. Please delink the output first";    
        }       
       
    if(tmp==0) // continue only of the output is not linked
    {
        if(entiremsg.indexOf("ON")>0)
      {
        outputstatusflag[q]=(outputstatusflag[q]|(0xC0)); //Making 6th bit 1, to indicate the output has to turn on, and making 7th bit on to indicate output has turned on using turn on command
        offsetaddr = 752 + (q * 1);
        EEPROM.write(offsetaddr, outputstatusflag[q]);
        outputstatusflag[q] = EEPROM.read(offsetaddr);
        offsetaddr = 756 + (q * 1);
        EEPROM.write(offsetaddr, outputontime[q]);
        outputontime[q] = EEPROM.read(offsetaddr);

        setnwriterdepromoutputontimetonow(q);
         /*currtimeoutput[q]=rtc.now();
         
        offsetaddr = 766 + (q * 6);
        
         EEPROM.write(offsetaddr, uint8_t((currtimeoutput[q].year())-(0x7D0))); // extract the year part of the total year ie., extract 23 from the year 2023 and convert it to uint8_t
        //currtimeoutput[q].year()= ((EEPROM.read(offsetaddr)+(0x7D0)).toInt()); // Add 2000 to the year stored in EEPROM to get the whole year and then convert it to int
        // Serial.print( uint8_t((currtimeoutput[q].year())&(0x7F)), DEC);
         Serial.print( (EEPROM.read(offsetaddr)), DEC);
        //offsetaddr = 767 + (q * 6);
        EEPROM.write((offsetaddr+1), (currtimeoutput[q].month())); // extract the month and write it in offsetaddr+1 
        //temporary= EEPROM.read(offsetaddr);
        //(currtimeoutput[q]).month()=temporary;
       //currtimeoutput[q].month()= readUnsignedIntFromEEPROM(offsetaddr); //(EEPROM.read(offsetaddr)); // Read the month 
        
        //offsetaddr = 768 + (q * 6);
        EEPROM.write((offsetaddr+2), (currtimeoutput[q].day())); // extract the day and write it in offsetaddr+2
        //currtimeoutput[q].day()= EEPROM.read(offsetaddr); // Read the day

        //offsetaddr = 769 + (q * 6);
        EEPROM.write((offsetaddr+3), (currtimeoutput[q].hour())); // extract the hour and write it in offsetaddr+3
        //currtimeoutput[q].hour()= int(EEPROM.read(offsetaddr)); // Read the hour

        //offsetaddr = 770 + (q * 6);
        EEPROM.write((offsetaddr+4), (currtimeoutput[q].minute())); // extract the minute and write it in offsetaddr+4
        //currtimeoutput[q].minute()= int(EEPROM.read(offsetaddr)); // Read the minute

        //offsetaddr = 771 + (q * 6);
        EEPROM.write((offsetaddr+5), (currtimeoutput[q].second())); // extract the second and write it in offsetaddr+5
        //currtimeoutput[q].second()= int(EEPROM.read(offsetaddr)); // Read the second
        

        currtimeoutput[q]=DateTime((int(EEPROM.read(offsetaddr))+(0x7D0)),EEPROM.read(offsetaddr+1),EEPROM.read(offsetaddr+2), EEPROM.read(offsetaddr+3), EEPROM.read(offsetaddr+4), EEPROM.read(offsetaddr+5)); // Read the year, convert it into int and add 2000
         //timeprint(rtc.now());
         timeprint(currtimeoutput[q]);
         //delay(10000);
         */
        outputstarted[q]=1;   // Flag to say that output has started
        outputdlyattained[q]=0;
        driveoutputonoff(q);
        
        
      }
        else
        if(entiremsg.indexOf("OFF")>0)
      {
          //Serial.println("ENtered OFF if");
          outputstatusflag[q]=(outputstatusflag[q]&(0xBF)); //Making 6th bit 0, to indicate the output has to turn off
          offsetaddr = 752 + (q * 1);
          EEPROM.write(offsetaddr, outputstatusflag[q]);
          outputstatusflag[q] = EEPROM.read(offsetaddr);
          //Serial.print("outputstatusflag is:");
          //Serial.println( outputstatusflag[q]);

          outputstarted[q]=1;   // Flag to say that output has started
          outputdlyattained[q]=0;
          msg="The output has been turned off";
          driveoutputonoff(q);
          
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
