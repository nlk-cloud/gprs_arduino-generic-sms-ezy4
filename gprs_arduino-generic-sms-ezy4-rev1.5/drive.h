void rdinpdriveout(uint8_t m, uint8_t a) // m is the input no. and a is the latch status
{
    uint8_t t[4]={0,0,0,0};
    uint8_t n;
    unsigned int inpdlyonly[4]={30,30,30,30}; // This variable stores the delay only without the units of min or sec ( the MSB is made 0)
  
    



 //inpondly[m]= inputondelay[m]&(0x7f); // Remove the MSB and take the delay only
        
    if(inpdlystarted[m]==0)
    {
         t[m]=digitalRead(inchannel[m]);
         previnpval[m]=t[m];
      
         if( t[m]!= ((inputstatusflag[m])&(0x01))) //input has changed state from its default
            {  
              Serial.println("entered changed if");  
             if(t[m]==1)
                {
                   Serial.println("entered ON delay");
                    offsetaddr=231+(m*66);
                    ReadCharFromEprom(offsetaddr,&msg ); // load the on msg. DOnt send it yet
                    //delay(inputondelay[m]*1000); // wait untill ON delay
                    dlyofinp[m]= inputondelay[m];
                    inpdlystarted[m]=1;
                    currtimeinp[m]=rtc.now();
                   // timeprint(currtime[m]);
                   //Serial.print("input delay started ");
                  //Serial.println(inpdly[m]);
                  // Serial.print(" ");
                  // Serial.println(inpondly[m]);
                  // Serial.println(inpdlystarted[m]);
                  // Serial.println(inpdlyattained[m]);
                   //Serial.print("m is");
                   //Serial.println(m);
                }
                else if (t[m]==0)
                {    Serial.println("entered off delay");
                    offsetaddr=492+(m*66);
                    ReadCharFromEprom(offsetaddr,&msg ); //load OFF msg. Dont send it yet
                    //delay(inputoffdelay[m]*1000); // wait untill OFF delay
                    dlyofinp[m]= inputoffdelay[m];
                    inpdlystarted[m]=1;
                    currtimeinp[m]=rtc.now();
                    
                }
            }

         else if(t[m] == ((inputstatusflag[m])&(0x01))) // input has not changed from its default state 
            {
                //Serial.println("entered last else - 1");
                if(((inputstatusflag[m]&(0x04))>>2)==1) // input is linked to output
                {
                //Serial.println("entered last else - 2"); 
                n=((inputstatusflag[m]&(0x18))>>3); // extracting the output no. to which it is linked
                
                if(((outputstatusflag[n]&(0x40))>>6)==1) //output is already not being driven because it is in its delay
                {
                //  Serial.println("entered last else - 3");
                  //Setting the status of output to 0
                  outputstatusflag[n]=(outputstatusflag[n]&(0xBF));
                  offsetaddr = 752 + (n * 1);
                  EEPROM.write(offsetaddr, outputstatusflag[n]);
                  outputstatusflag[n] = EEPROM.read(offsetaddr);
                  //Serial.print("outputstatusflag is:");
                  //Serial.println( outputstatusflag[n]);
                }
                  outputstarted[n]=1;   // Flag to say that output has started
                  driveoutputonoff(n);
                //digitalWrite(outchannel[n],0);
                }
              }
   }
      
  
 

 else if(inpdlystarted[m]==1)
        {
          //Serial.print("m is");
          //Serial.println(m);
         
            inpdlyonly[m]= (dlyofinp[m]&(0x7f)); // Remove the MSB bit since it was only indicating units ie., min and store it 
                // in a different variable, since we dont want to lose the min information
            // Serial.print("inpdly is");
           //Serial.println(dlyofinp[m]);
           //Serial.println(inpdlyonly[m]);
            
            if(((dlyofinp[m]&(0x80))>>7)==1)     // delay is in min
            {
                inpdlyattained[m]=dly(currtimeinp[m],0,0,inpdlyonly[m],0); //timekeeping.h   
            }

            else if(((dlyofinp[m]&(0x80))>>7)==0)     // delay is in sec
            {
              inpdlyattained[m]=dly(currtimeinp[m],0,0,0,inpdlyonly[m]); //timekeeping.h      
            }
        }
        

        

if(inpdlyattained[m]==1)
        { 
          Serial.print("Input delay attained ");
          Serial.print(dlyofinp[m]);
          Serial.println(inpdlyonly[m]);
          
            //timeprint(currtime[m]);
           
            t[m]=digitalRead(inchannel[m]); // Read the input again after a delay
          
            if ( t[m]==previnpval[m]) // Implies the switch is real and not noise
            {
                Serial.println("entered change is real if"); 
                if(a==1) // input is latching type. Hence we have to set the latch bit
                {
                   Serial.println("setting the latch bit");
                   inputstatusflag[m]=inputstatusflag[m] |(0x20); // set the latch bit
                   offsetaddr = 211 + (m * 8);
                   EEPROM.write(offsetaddr, inputstatusflag[m]);
                   inputstatusflag[m] = EEPROM.read(offsetaddr);
                }
                if((((inputstatusflag[m])&(0x40))>>6)==1) // check if alerts are set on and whether to send ON msg or off msg
                {
                    inpdlystarted[m]=0;
                    inpdlyattained[m]=0;
                    Serial.println("sending msg if alerts is ON");
                    response=17;
                    SendResponse(); // sendresponse.h (send sms)
                 }               
                if (((inputstatusflag[m]&0x04)>>2)==1) // checking if the input is linked to an output
                {
                    Serial.println("setting the output if output is linked");
                    n=((inputstatusflag[m]&(0x18))>>3); // extracting the output no. to which it is linked
                    //Serial.println(n);
                    
                    //setting the status of the output
                    
                    outputstatusflag[n]=outputstatusflag[n]|(0x40); // setting the status bit of output
                    offsetaddr = 752 + (n * 1);
                    EEPROM.write(offsetaddr, outputstatusflag[n]);
                    outputstatusflag[n] = EEPROM.read(offsetaddr);

                    // Reading the output on time from memory
                     offsetaddr = 756 + (n * 1);
                     outputontime[n] = EEPROM.read(offsetaddr);

                    // Setting the start time of the output
                    setnwriterdepromoutputontimetonow(n);  // set, write and read ( from EEPROM) the start time for the output delay
                    //currtimeoutput[m]=rtc.now(); // start time for the output delay
                    
                    outputstarted[n]=1; // As long as outputstarted =1, the output should be driven
                    driveoutputonoff(n); // setoutputflag.h
                    
                 }

           }
                inpdlystarted[m]=0;
                inpdlyattained[m]=0;
                             
           }
          
  }





void action()
{
    uint8_t k,q;
   
     
    for(k=0;k<4;k++)
    {
//Serial.print("outputstarted value is" );
  //             Serial.println(outputstarted[k]);
        //drive outputs if they were driven
        
         driveoutputonoff(k); // setoutputflag.h
        
        // Now check the inputs
        
        q=(((inputstatusflag[k])&(0x02))>>1); // extract the type of input - latching or non-latching
       
         if((((inputstatusflag[k])&(0x02))>>1)==1) // check is the input type is latching
         {         
             //Serial.println("entered latch if");   
            if((inputstatusflag[k]&(0x20))==0) // Read the input and set the output, only when latch is reset 
            {
               // Serial.println("entered latch while");
                rdinpdriveout(k,q); // this file only
            }
        
         }

        else if(((inputstatusflag[k])&(0x02))==0)  
        {
           
            rdinpdriveout(k,q);    
            
        }

    }

    //check battery voltage
   // checkbatteryvolt(); // resetreboot.h
   

    
}
