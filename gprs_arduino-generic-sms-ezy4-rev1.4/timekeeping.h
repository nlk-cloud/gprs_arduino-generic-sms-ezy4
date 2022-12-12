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
ReadNamefromCommand(kiwrd, ':', ':', min);
ReadNamefromCommand(kiwrd, ':', '+', sec);

kiwrd =month;
ReadNamefromCommand(kiwrd,'/', ',', date);

rtc.adjust(DateTime(year.toInt(),month.toInt(),date.toInt(), hour.toInt(), min.toInt(), sec.toInt()));

}


boolean dly(DateTime starttime,uint8_t dys,uint8_t hrs, uint8_t mns, uint8_t scs )
{

 //Serial.print("mns is");
 //Serial.println(mns);
 //Serial.print("scs is");
 //Serial.println(scs);
 DateTime future = (starttime + TimeSpan(dys,hrs,mns,scs));
   // timeprint(starttime);
   // timeprint(future);
    //delay(10000);
    //timeprint(rtc.now());
    if(rtc.now()== future)
    {
        return(1);
    }
else
{
      return(0);
}
}
