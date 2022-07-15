// this code aims at replicating all the functions listed in ezy- sms-8 but for 4 inputs and 4 outputs using Arduino uno


#include <SoftwareSerial.h>
SoftwareSerial mySerial(9, 10);
#include <EEPROM.h>

String entiremsg;
String smstxt1;
String smstxt2;
String numtoincldel;
String admin; 
String user0;
String sendto;
String msg;
String sender;
String replyfrommodem;
String num;
String msg1;
String recipientNumber[] = { "+919611839500", "\0", "\0", "\0","\0" };


boolean flag = false;
boolean newData;

int index,index2;
int admincnt=0;
int position, position2;
//int epromadd=0;
int i;
int newlinecnt;
//int nwlncntexpected; // expected newline cnt in the message that is received
char rc;
int addrOffsetuser0;
int addrOffsetadmin;
int addrOffsetuser1;
int addrOffsetuser2;
int addrOffsetuser3;
int addrOffsetuser4;
int addrOffsetuser5;
int response; //if response=0 implies response to add cmd, response=1 for del response

#include "readcharappendtostring.h"
#include "readstring.h"
#include "readeprom.h"
#include "modemresponse.h"
#include "sendmsg.h"
#include "writeeprom.h"
#include "sendresponse.h"
#include "checkifuserexists.h"
#include "setuser0.h"
#include "incl.h"
#include "readsenderno.h"
#include "readsmstxt.h"
#include "readverifynumber.h"
//#include "readcharacterbycharacter.h"
#include "msgrd.h"


// 


void setup() {
  Serial.begin(9600);  // Setting the baud rate of Serial Monitor (Arduino)
  mySerial.begin(9600);   // Setting the baud rate of GSM Module  
  
  //mySerial.println("AT+CFUN=0");
  //delay(100);
  //mySerial.println("AT+CFUN=1");
  //delay(1000);
  //mySerial.println("ATE0&W"); // Turning the echo off in modem  
//mySerial.println("AT+CMGDA="+\"+DEL ALL+\""); // Delete all the messages
//delay(1000);
mySerial.println("AT+CNMI=2,2,0,0,0"); // Deliver the SMS content 
delay(1000);
mySerial.println("AT+CMGR=1");
delay(1000);

Serial.println("started");
//CLearing EEPROM
/*
for ( i = 0 ; i < EEPROM.length() ; i++) {
    EEPROM.write(i, 0);
  }  
*/

  delay(100);

// Reading from EEPROM
addrOffsetuser0=0;
addrOffsetadmin=ReadEprom(addrOffsetuser0,&user0 );
addrOffsetuser1=ReadEprom(addrOffsetadmin,&admin );
addrOffsetuser2=ReadEprom(addrOffsetuser1,&recipientNumber[0] );
addrOffsetuser3=ReadEprom(addrOffsetuser2,&recipientNumber[1] );
addrOffsetuser4=ReadEprom(addrOffsetuser3,&recipientNumber[2] );
addrOffsetuser5=ReadEprom(addrOffsetuser4,&recipientNumber[3] );
ReadEprom(addrOffsetuser5,&recipientNumber[4] );
Serial.print(" User0 is:  ");
Serial.println(user0);
Serial.print(" admin is:  ");
Serial.println(admin);

for (int j=0;j<5; j=j+1)
{
Serial.print(" RecipientNumber[j] is:  ");
Serial.println(recipientNumber[j]);
}
/*
//writing into EPROM

for (int j=0;j<5; j=j+1)
{

recipientNumber[j] = "+919611839500";
  
}

numtoincldel="+919611839500";
addrOffsetuser0=0;
addrOffsetadmin=WriteEprom(addrOffsetuser0,numtoincldel );
Serial.println(addrOffsetadmin);
admin=WriteEprom(addrOffsetadmin, numtoincldel);
Serial.println(addrOffsetuser1);
addrOffsetuser2=WriteEprom(addrOffsetuser1, numtoincldel);
Serial.println(addrOffsetuser2);
addrOffsetuser3=WriteEprom(addrOffsetuser2, numtoincldel);
Serial.println(addrOffsetuser3);
addrOffsetuser4=WriteEprom(addrOffsetuser3, numtoincldel);
Serial.println(addrOffsetuser4);
addrOffsetuser5=WriteEprom(addrOffsetuser4, numtoincldel); 
Serial.println(addrOffsetuser5);
WriteEprom(addrOffsetuser5, numtoincldel);
*/
}

void loop() {

  MsgRd();

}

// EEPROM MAPPING
// Address  what
// 0-13        Set User ( 13 byts for +91XXXXXXXXXX and one byte for storing the length )
// 14-27       Admin
// 28-41       User1
// 42-55       User2
// 56-69       User3
// 70-83       User4
// 84-97       User5