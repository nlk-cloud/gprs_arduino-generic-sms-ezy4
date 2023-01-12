void factoryreset()
{
    //CLearing EEPROM

Serial.println(" Entered factory reset");
ClearEEPROM();


  delay(100);

// Write input and output names to EEPROM for the first time
WriteIONamesfirstime(); //setupeeprom.h

// Write Input flags into EEPROM
WriteInputFlagsfirsttime(); //setupeeprom.h

// Write delays into EEPROM
WriteInputDelaysfirsttime(); //setupeeprom.h

// Write input on messages into EEPROM
ReadioNamesfrmEprom(); // write input on msg uses the IO names. Hence we have to read the io names first.
WriteInputONmsgfirsttime(); //setupeeprom.h

// Write input on messages into EEPROM
WriteInputOFFmsgfirsttime(); //setupeeprom.h

//Write output status flag for the first time
WriteOutputFlagsfirsttime();

//Write output ON TIME for the first time
WriteOutputOntime();

// Write battery trigger voltage for the first time
Writebatterytrigger();

// Write alive text time for the first time
Writealivetxttimefirsttime();

// Write the output start time for the first time
Writecurrtimeoutputfirsttime();



ReadUsersandAdmin();
ReadioNamesfrmEprom(); //setupeeprom.h
ReadInputStatusFlag(); //setupeeprom.h
ReadInputDelays();//setupeeprom.h
ReadOutputFlags();//setupeeprom.h
ReadOutputONTime();
ReadBatteryTrigger();
Readalivetxttime();
Readcurrtimeoutput();

inputonmsgprint(); //setupeeprom.h
inputoffmsgprint(); //setupeeprom.h

// Setting the time in rtc
setnetworktime();

msg=" The unit has been restored to factory settings and all memory has been cleared";
response=16;
SendResponse();

}

void reboot()
{
  
  msg=" The module is now resetting";
  response=16;
  SendResponse();
  resetFunc(); // reset for arduino  
  //Serial.begin(9600);  // Setting the baud rate of Serial Monitor (Arduino)
  //mySerial.begin(9600);   // Setting the baud rate of GSM Module  
  
  mySerial.println("AT+CFUN=0");
  delay(100);
  mySerial.println("AT+CFUN=1");
  delay(1000);
  mySerial.println("ATE0&W"); // Turning the echo off in modem  
  mySerial.println("AT+CMGDA=\"DEL ALL\""); // Delete all the messages
  delay(1000);
  mySerial.println("AT+CNMI=2,2,0,0,0"); // Deliver the SMS content 
  delay(1000);
  mySerial.println("AT+CMGR=1");
  delay(1000);

// Setting the time in rtc
setnetworktime();

 msg=" The module is now resetting";
 response=16;
 SendResponse();

}


void signalstrength()
{
  uint8_t q, count=0;
  
  mySerial.println("AT+CSQ"); // check signal quality
  delay(1000);
  while(((entiremsg.indexOf("+CSQ")>0)||(count>30))==0) // wait for 30secs or untill +CSQ comes 
  {
    flag=0;
    ReadString();
    delay(1000);
    count=count+1;
  }
  kiwrd = "+CSQ:";
  ReadNamefromCommand(kiwrd, ' ', ',', fromname);
  q=fromname.toInt();
  q=100*q/30 ; //converting the signal strength to %
  msg="currently the signal strngth is "+String(q)+"%";
  response=16;
  SendResponse();
  flag=0;
}


void batteryvoltage()
{
  msg=" Battery is currently "+String(digitalRead(battery))+" volts";
  response=16;
  SendResponse();
}

void linkbatteryalarm()
{ uint8_t q;
  kiwrd = "to";
  ReadNamefromCommand(kiwrd, ' ', '\r', fromname);
  for(q=0;q<4;q++)
  {
    if(outputname[q]==fromname)
    {
        outputstatusflag[q]=outputstatusflag[q]|(0x20); // set the battery link bit to 1
        offsetaddr = 752 + (q * 1);
        EEPROM.write(offsetaddr, outputstatusflag[q]);
        outputstatusflag[q] = EEPROM.read(offsetaddr); 
        msg="The battery alarm has been linked to "+outputname[q];
    }
  }
  response=16;
  SendResponse();

}

void setbatterytrigger()
{
  uint8_t q;
  kiwrd = "to";
  ReadNamefromCommand(kiwrd, ' ', '\r', fromname); // store the volatge in fromname;
  WriteCharIntoEprom(760, fromname);
  msg=" The low voltage alarm has been set to "+fromname+"volts";
 

  if(fromname.length()==0)
  { 
    msg=" Since no value was specified, voltage alarm has been set to 11.5 Volts";

  }
  if((fromname.toFloat() < 8) || (fromname.toFloat()>30))
  {
    msg=" The limit value is out of range. Hence the voltage alarm has been set to 11.5 Volts";
  }

   response=16;
  SendResponse();
}

  void clearbatterylink()
{ uint8_t q;
  
  for(q=0;q<4;q++)
  {
        outputstatusflag[q]=outputstatusflag[q]&(0xDF); // reset the battery link bit to 1
        offsetaddr = 752 + (q * 1);
        EEPROM.write(offsetaddr, outputstatusflag[q]);
        outputstatusflag[q] = EEPROM.read(offsetaddr); 
       
  }
   msg="The battery alarm output link has been removed.";
  response=16;
  SendResponse();
}
  

void checkbatteryvolt()
{
  uint8_t t;
   float q,r,avgreading; 
   String settrigger;
   
   ReadCharFromEprom(760, &settrigger);
   q=analogRead(battery);
   
     if( (millis()-prev)/1000 > 60)
{
  r=analogRead(battery);
  avgreading=(q+r)/2;
  prev=millis();
  if ( avgreading<= settrigger.toFloat())
  {
    msg="Battery voltage low";
    response=17;
    SendResponse();

  for(t=0;t<4;t++)
  {
     offsetaddr = 752 + (t * 1);
     outputstatusflag[t] = EEPROM.read(offsetaddr); 
     if((outputstatusflag[t]&(0x20))==32) // battery link has been set in this output. Hence drive this output
     {
      digitalWrite(outchannel[t],1);
     }
  }
  }

}
}


void alivetext()
{
  uint8_t k;
   offsetaddr=765;
  if (entiremsg.indexOf("OFF")>0)
  {
   
    EEPROM.write(offsetaddr,0);
    alivetxttime=EEPROM.read(offsetaddr);
    msg="Alive text has been turned off";
  }

   if (entiremsg.indexOf("ON")>0)
  {
    kiwrd = "EVERY";
    ReadNamefromCommand(kiwrd, ' ', ' ', fromname);
    k= (fromname.toInt());
    // MSB =1 => days and MSB=0 => hrs, remaining 7 bits are used to store 2 digit time in hrs or days
    if (entiremsg.indexOf("HRS")>0)
    {
      k=(k|(0x00));
     
    }
    else
    if (entiremsg.indexOf("DAYS")>0)
    {
      k=(k|(0x80));
    }
    Serial.print("alive txt time is");
    Serial.println(k);
    EEPROM.write(offsetaddr,k);
    alivetxttime=EEPROM.read(offsetaddr);
    setnextalivetxttime();
    msg="Alive text has been turned on";
  }
response =18;
SendResponse();
  
}
