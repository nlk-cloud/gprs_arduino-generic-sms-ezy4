void rdinpdriveout(uint8_t m, uint8_t a) // m is the input no. and a is the latch status
{
    uint8_t t, prev,n;
    //Serial.println("entered read input");
            t=digitalRead(inchannel[m]);
            prev=t;
            if( t!= ((inputstatusflag[m])&(0x01))) //input has changed state from its default
            
            {  Serial.println("entered changed if");  
                 if(t==1)
                {
                   Serial.println("enetered ON delay");
                    offsetaddr=231+(m*66);
                    ReadCharFromEprom(offsetaddr,&msg ); // load the on msg. DOnt send it yet
                    delay(inputondelay[m]*1000); // wait untill ON delay
                }
                else if (t==0)
                {    Serial.println("enetered off delay");
                    offsetaddr=492+(m*66);
                    ReadCharFromEprom(offsetaddr,&msg ); //load OFF msg. Dont send it yet
                    delay(inputoffdelay[m]*1000); // wait untill OFF delay
                }
                t=digitalRead(inchannel[m]); // Read the input again after a delay
                if ( t==prev) // Implies the switch is real and not noise
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
                        Serial.println("sending msg if alerts is ON");
                        response=17;
                        SendResponse(); // send sms
                    }               
                    if (((inputstatusflag[m]&0x04)>>2)==1) // checking if the input is linked to an output
                    {
                        Serial.println("setting the output if output is linked");
                        n=((inputstatusflag[m]&(0x18))>>3); // extracting the output no. to which it is linked
                        Serial.println(n);
                        driveoutputonoff( n, 1);
                    }

                }
            }

            
            else  if(t== ((inputstatusflag[m])&(0x01))) // input has not changed from its default state
            {
                //Serial.println("entered last else"); 
                n=(inputstatusflag[m]&(0x18))>>3; // extracting the output no. to which it is linked
                digitalWrite(outchannel[n],0);
            }
            

}





void action()
{
    uint8_t k,q;
   
     
    for(k=0;k<4;k++)
    {
         //Serial.println("k is ");
         // Serial.println(k);
        //q=(inputstatusflag[k]&(0x20))>>5; // extract the latch status
        q=(((inputstatusflag[k])&(0x02))>>1); // extract the type of input - latching or non-latching
       // Serial.println("q is ");
        //Serial.println(q);
         if((((inputstatusflag[k])&(0x02))>>1)==1) // check is the input type is latching
         {         
             //Serial.println("entered latch if");   
            if((inputstatusflag[k]&(0x20))==0) // Read the input and set the output, only when latch is reset 
            {
               // Serial.println("entered latch while");
                rdinpdriveout(k,q);
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
