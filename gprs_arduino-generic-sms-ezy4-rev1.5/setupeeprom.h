

// Clear EEPROM

void ClearEEPROM()
{
for ( offsetaddr = 0 ; offsetaddr < EEPROM.length() ; offsetaddr++) // cannot use i here since i is defined as unsigned int. So it can go upto 255 only
{ 
    EEPROM.write(offsetaddr, 0);
  //Serial.println("entered eprom clear");
  }  
}


// Write IO names

void WriteIONamesfirstime()
{
  uint8_t n;
offsetaddr=42;

for(n=0;n<4;n=n+1)
{
  
offsetaddr=42+(n*21);
//stringtemp="INPUT";  
//stringtemp=stringtemp+i;
WriteCharIntoEprom(offsetaddr,"INPUT"+String(n+1));

}

offsetaddr=126;
for(n=0;n<4;n=n+1)
{
offsetaddr=126+(n*21);
//stringtemp="OUTPUT"; 
//stringtemp=stringtemp+(i);
//WriteEprom(offsetaddr,stringtemp);
WriteCharIntoEprom(offsetaddr,"OUTPUT"+String(n+1));
}
}

// Write Input flags

void WriteInputFlagsfirsttime()
{
  uint8_t n;
  offsetaddr=211;
  for(n=0;n<4;n=n+1)
{
  offsetaddr=211+(n*1);
  //EEPROM.write(offsetaddr,inputstatusflag[n]);
  EEPROM.write(offsetaddr,0);
}
  
}


// Write input delays

void WriteInputDelaysfirsttime()
{
  uint8_t n;
  offsetaddr=215;
  for(n=0;n<4;n=n+1)
{
  offsetaddr=215+(n*2);
  writeUnsignedIntIntoEEPROM(offsetaddr,30);
}

 for(n=0;n<4;n=n+1)
{
  offsetaddr=223+(n*2);
  writeUnsignedIntIntoEEPROM(offsetaddr,30);
}
  //Serial.println("Completed writing input status flag");
}


// Write input ON message

void WriteInputONmsgfirsttime()
{
    uint8_t n;

for(n=0;n<4;n=n+1)
{
offsetaddr=231+(n*66);
WriteCharIntoEprom(offsetaddr,inputname[n]+" has turned on" );

}
}


// Write Input off message

void WriteInputOFFmsgfirsttime()
{
    uint8_t n;

for(n=0;n<4;n=n+1)
{
  
offsetaddr=492+(n*66);
WriteCharIntoEprom(offsetaddr,inputname[n]+" has turned off" );

}
}

// Write output flags

void WriteOutputFlagsfirsttime()
{
  uint8_t n;
 
  for(n=0;n<4;n=n+1)
{
  offsetaddr=752+(n*1);
  EEPROM.write(offsetaddr,0);
}

}

// Write output ON time

void WriteOutputOntime()
{
  uint8_t n;
  //offsetaddr=756;
  
  for(n=0;n<4;n=n+1)
{
  offsetaddr=756+(n*1);
  EEPROM.write(offsetaddr,0);
}
  
}

// Write battery trigger voltage

void Writebatterytrigger()
{
  offsetaddr=760;
WriteCharIntoEprom(offsetaddr,"11.5" );
}

// Write alive text time 

void Writealivetxttimefirsttime()
{
  offsetaddr=765;
  EEPROM.write(offsetaddr,0);
}

void Writecurrtimeoutputfirsttime()
{

uint8_t n;

for(n=0;n<4;n=n+1)    // Repeat the loop for each of the outputs
{
         offsetaddr = 766 + (n * 6);
         EEPROM.write(offsetaddr,0);     // Write year
         EEPROM.write((offsetaddr+1),0); // Write month
         EEPROM.write((offsetaddr+2),0); // write day
         EEPROM.write((offsetaddr+3),0); // Write hour
         EEPROM.write((offsetaddr+4),0); // Write min
         EEPROM.write((offsetaddr+5),0); // Write sec
}
}
/***** Read ****/

void ReadUsersandAdmin()
{  
  uint8_t n;

// Read User0
offsetaddr=0;
firstuser0=readUnsignedIntFromEEPROM(offsetaddr);
offsetaddr=2;
user0=ReadLongFromEEPROM(offsetaddr);

// Read admin
offsetaddr=6;
firstadmin=readUnsignedIntFromEEPROM(offsetaddr);
offsetaddr=8;
admin=ReadLongFromEEPROM(offsetaddr);

// Read users
offsetaddr=12;
for(n=0;n<5;n++)
{
offsetaddr=12+(n*6);
usernum3digits[n]=readUnsignedIntFromEEPROM(offsetaddr);
offsetaddr=14+(n*6);
usernum9digits[n]=ReadLongFromEEPROM(offsetaddr);
if((usernum3digits[n]!=0)&&(usernum9digits[n]!=0))
{
  usercnt=usercnt+1;
}
}

Serial.print(" User0 is:  ");
Serial.print(firstuser0);
Serial.println(user0);
Serial.print(" admin is:  ");
Serial.print(firstadmin);
Serial.println(admin);
for ( n=0;n<5; n=n+1)
{
Serial.print(" user[i] is:  ");
Serial.print(usernum3digits[n]);
Serial.println(usernum9digits[n]);
}
}

void ReadioNamesfrmEprom()
{
  uint8_t n;
// Read input names
    for(n=0;n<4;n=n+1)
    {
        offsetaddr=42+(n*21);
        ReadCharFromEprom(offsetaddr,&inputname[n] );
        Serial.print(inputname[n]);
        Serial.print(" ");
        Serial.println(inputname[n].length());        

    }

// REad output names
    for(n=0;n<4;n=n+1)
    {
        offsetaddr=126+(n*21);
        ReadCharFromEprom(offsetaddr,&outputname[n] );
        Serial.print(outputname[n]);
        Serial.print(" ");
        Serial.println(outputname[n].length()); 
    }


}

// Read input status flags from EEPROM

void ReadInputStatusFlag()
{
  uint8_t n;
  for(n=0;n<4;n=n+1)
  {
    offsetaddr=211+(n*1);
    inputstatusflag[n]=EEPROM.read(offsetaddr);
    Serial.print("The status flag of ");
    Serial.print(inputname[n]);
    Serial.print(" ");
    Serial.println(inputstatusflag[n]);
  }
}

// read input on and off delays

void ReadInputDelays()
{
  uint8_t n;
  offsetaddr=215;
  for(n=0;n<4;n=n+1)
{
  offsetaddr=215+(n*2);
  inputondelay[n]=readUnsignedIntFromEEPROM(offsetaddr);
  Serial.print("The ON delay of ");
  Serial.print(inputname[n]);
  Serial.print(" ");
  Serial.println(inputondelay[n]);
}

 for(n=0;n<4;n=n+1)
{
  offsetaddr=223+(n*2);
  inputoffdelay[n]=readUnsignedIntFromEEPROM(offsetaddr);
  Serial.print("The OFF delay of ");
  Serial.print(inputname[n]);
  Serial.print(" ");
  Serial.println(inputoffdelay[n]);
}
  //Serial.println("Completed writing input status flag");
}

void inputonmsgprint()
{
  uint8_t n;

    for(n=0;n<4;n=n+1)
    {
        offsetaddr=231+(n*66);
        ReadCharFromEprom(offsetaddr,&stringtemp );
        Serial.print(inputname[n]);
        Serial.println(" On message is ");
        Serial.println(stringtemp);        
    }

}

void inputoffmsgprint()
{
  uint8_t n;

    for(n=0;n<4;n=n+1)
    {
        offsetaddr=492+(n*66);
        ReadCharFromEprom(offsetaddr,&stringtemp );
        Serial.print(inputname[n]);
        Serial.println(" OFF message is ");
        Serial.println(stringtemp);        
    }

}

// Read output flags

void ReadOutputFlags()
{
  uint8_t n;
  
  for(n=0;n<4;n=n+1)
{
  offsetaddr=752+(n*1);
  outputstatusflag[n]=EEPROM.read(offsetaddr);
    Serial.print("The status flag of ");
    Serial.print(outputname[n]);
    Serial.print(" ");
    Serial.println(outputstatusflag[n]);
}

}

// Read output on time

void ReadOutputONTime()
{
  uint8_t n;
  
  for(n=0;n<4;n=n+1)
{
  offsetaddr=756+(n*1);
  outputontime[n]=EEPROM.read(offsetaddr);
  Serial.print("The ON delay of ");
  Serial.print(outputname[n]);
   Serial.print(" ");
  Serial.println(outputontime[n]);
}

}

// Read battery trigger voltage

void ReadBatteryTrigger()
{
  offsetaddr=760;
  ReadCharFromEprom(offsetaddr,&stringtemp );
  Serial.print(" Battery Trigger voltage is ");
  Serial.println(stringtemp);  
  
}

// Read alive text time

void Readalivetxttime()
{
  offsetaddr=765;
  alivetxttime=EEPROM.read(offsetaddr);
  Serial.print(" Alive Text Time is ");
  Serial.println(alivetxttime); 
}

Readcurrtimeoutput()
{
  uint8_t n;
  for(n=0;n<4;n=n+1) // repeat the loop for all 4 outputs
  {
  offsetaddr = 766 + (n * 6);
  currtimeoutput[n]=DateTime((int(EEPROM.read(offsetaddr))+(0x7D0)),EEPROM.read(offsetaddr+1),EEPROM.read(offsetaddr+2), EEPROM.read(offsetaddr+3), EEPROM.read(offsetaddr+4), EEPROM.read(offsetaddr+5)); // Read the year, convert it into int and add 2000
  timeprint(currtimeoutput[n]);
  }
}
