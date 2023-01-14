timeprint(DateTime timetoprint)
{
  //Serial.println("Current Date & Time: ");
    Serial.print(timetoprint.year(), DEC);
    Serial.print('/');
    Serial.print(timetoprint.month(), DEC);
    Serial.print('/');
    Serial.print(timetoprint.day(), DEC);
    Serial.print(' ');
//    Serial.print(" (");
//    Serial.print(daysOfTheWeek[timetoprint.dayOfTheWeek()]);
//    Serial.print(") ");
    Serial.print(timetoprint.hour(), DEC);
    Serial.print(':');
    Serial.print(timetoprint.minute(), DEC);
    Serial.print(':');
    Serial.print(timetoprint.second(), DEC);
    Serial.println();

}


void settime()
{

// Comment out below lines once you set the date & time.
    // Following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
	
    // Following line sets the RTC with an explicit date & time
    // for example to set January 27 2017 at 12:56 you would call:
    // rtc.adjust(DateTime(2017, 1, 27, 12, 56, 0));


}



void setnetworktime()
{
String date, month, year, sec, min, hour;
// Reading the time from network

//Command to read the GSM RTC clock : 

mySerial.println("AT+CCLK?");

//delay(5000);

ReadString();

//AT+CCLK?
//+CCLK: "22/09/09,12:26:47+22"

//Command to enable GSM RTC network time sync : 
//AT+CLTS=1

//Command to save the command settings to memory so that the time is synchronized on every boot up automatically. 
//AT&W
kiwrd ="+CCLK";
ReadNamefromCommand(kiwrd, '"', '/', year);
ReadNamefromCommand(kiwrd, '/', '/', month);
ReadNamefromCommand(kiwrd, ',', ':', hour);
kiwrd =hour;
ReadNamefromCommand(kiwrd, ':', ':', min);
kiwrd =min;
ReadNamefromCommand(kiwrd, ':', '+', sec);

kiwrd =month;
ReadNamefromCommand(kiwrd,'/', ',', date);

rtc.adjust(DateTime(year.toInt(),month.toInt(),date.toInt(), hour.toInt(), min.toInt(), sec.toInt()));
timeprint(rtc.now());
/*Serial.print(" year is");
Serial.println(year);
Serial.print(" month is");
Serial.println(month);
Serial.print(" date is");
Serial.println(date);
Serial.print(" hour is");
Serial.println(hour);
Serial.print(" min is");
Serial.println(min);
Serial.print(" sec is");
Serial.println(sec);
*/

}


int8_t dly(DateTime starttime,uint8_t dys,uint8_t hrs, uint8_t mns, uint8_t scs )
{

 //Serial.print("mns is");
 //Serial.println(mns);
 //Serial.print("scs is");
 //Serial.println(scs);
//timeprint(starttime);
 
 DateTime future = (starttime + TimeSpan(dys,hrs,mns,scs));
   //timeprint(starttime);
  // timeprint(future);
  // delay(10000);
  // timeprint(rtc.now());
    if(rtc.now()== future)
    {
        return(1);
    }
  else if(rtc.now()< future)
  {
    return(0);
  }
  else if(rtc.now()>future)
  {
      return(-1);
  }


}


void setnextalivetxttime()
{
//DateTime tmptime; 

//tmptime=rtc.now();
  if (  alivetxttime != 0)  // implies some value is set as alive txt time
 // Serial.print("ALive txt time is");
 // Serial.println(alivetxttime);
{
  if ((( (alivetxttime)&(0x80))>>7)==1)  // delay is in days
  
  {
       nextalivetxt= (rtc.now() + TimeSpan(alivetxttime,0,0,0));
   //    Serial.println( "ENtered days"); 
  }

  else if((( (alivetxttime)&(0x80))>>7)==0) // delay is in hours
  {
     nextalivetxt= (rtc.now() + TimeSpan(0,alivetxttime,0,0));
     // Serial.println( "ENtered hours");
  }
    
}

//timeprint(nextalivetxt);
}

void resettimeandalivetxt()
{
  DateTime present,timelapse;
  present=rtc.now();
  if (((present.hour())==24)&&((present.minute())==0)&&((present.second())==0))
  {
    setnetworktime();
  }

if (  alivetxttime != 0) 
{
 if (((present.hour())==nextalivetxt.hour())||((present.day())==nextalivetxt.day()))
 {
  if(sendalivemsg==0)
  {
  sendalivemsg=1;
  msg="Alive msg from GSM";
  response=18;
  SendResponse();
  setnextalivetxttime();
 }  
}
}
}
