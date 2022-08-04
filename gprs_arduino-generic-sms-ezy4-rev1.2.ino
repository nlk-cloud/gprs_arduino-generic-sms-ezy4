// this code aims at replicating all the functions listed in ezy- sms-8 but for 4 inputs and 4 outputs using Arduino uno

// Rev 1.2 - Trying to reduce the dnamic RAM usage in this version by reducing the size of variables, strings etc 
#include <SoftwareSerial.h>
SoftwareSerial mySerial(50, 51);
#include <EEPROM.h>

#define Version 1.2
#define input0 22
#define input1 23
#define input2 24
#define input3 A0

#define output0 26
#define output1 27
#define output2 28
#define output3 29

String fromname;
String toname;
String entiremsg;
// the names given by the user are stored in these arrays. However, inside the program, they are referred to as input0, input1 and so on
// THe idea is to read the name, compare it with this the name in the input name array, read the index and using that index 
// do the processing
String inputname[4];//={ "\0", "\0", "\0", "\0" };
String outputname[4]={ "\0", "\0", "\0", "\0" };
String stringtemp;
String msg;
String kiwrd;

int firstnumtoincldel; // variables starting with first indicate first 3 digits of the no.
int firstadmin;
int firstuser0;
//int firstsendto;
int firstsender;
int usernum3digits[5];
int offsetaddr;

long numtoincldel=0;
long admin=0; 
long user0=0;
//long sendto=0;
long sender=0;
long usernum9digits[5]={0,0,0,0,0};


uint8_t flag=0; // this will be 8 bits. Each bit represents the below flags 
//boolean numflag = false; - MSB
//boolean userflag = false;
//boolean ionameflag=false;
//boolean newData; - LSB


unsigned int inputondelay[4]={30,30,30,30}; // contains delay in secs
unsigned int inputoffdelay[4]={30,30,30,30}; // contains delay in secs


//uint8_t latchreset=0; // stores the latch reset status of all the inputs
uint8_t instatusrd; //status of the inputs read and stored here. LSB is the status of input0, next bit is the status of input1 and so on
uint8_t instatusdefault=0; // default status of all 4 inputs. LSB is for input 0 and so on 
uint8_t outstatusrd; // status of the outputs is read and stored here. LSB is the status of output0, next bt output1 and so on
uint8_t i;
uint8_t response; //if response=0 implies response to add cmd, response=1 for del response
uint8_t usercnt=0;//holds the live count of the number of users
uint8_t from, to; // the starting position to read the number from and the ending position to read the number to
//uint8_t latchflag=0; // LSB is flag for input0 and MSB for input3. If set latch is on
uint8_t inputstatusflag[4]={0,0,0,0};
// Each input has one status flag. THe seetings of each flag are:
// Bit                whatit indicates
//  7                              
//  6               Input texts ON if 1 Input texts OFF if 0
//  5               
//  4               WHich output is linked to this input ( upperr bit of that output)
//  3               WHich output is linked to this input ( lower bit of that output)
//  2               Link input to output if 1 else inputs and outputs are not linked
//  1               Input Latching (if 1 ) and Input Non-Latching ( if 0)
//  0               normally open(default logic 0, when it is 0) or closed (default logic 1 when it is 1) - 




#include "readwriteEEPROM.h"
//#include "readusrlistfrmeprom.h"
///#include "readcharappendtostring.h"
#include "readstring.h"
//#include "modemresponse.h"
#include "sendmsg.h"
///#include "writelongintoEEPROM.h"
#include "setupeeprom.h"
#include "sendresponse.h"
#include "setupinputflag.h"
#include "status.h"
//#include "linklatch.h"
#include "readassignionames.h"
#include "checkifuserexists.h"
#include "setuser0.h"
#include "readverifynumber.h"
#include "incluser.h"
#include "incladmin.h"
#include "deluser.h"
#include "readsenderno.h"
//#include "readsmstxt.h"
//#include "readcharacterbycharacter.h"
#include "msgrd.h"





void setup() {
  Serial.begin(9600);  // Setting the baud rate of Serial Monitor (Arduino)
  mySerial.begin(9600);   // Setting the baud rate of GSM Module  
  
  //mySerial.println("AT+CFUN=0");
  //delay(100);
  //mySerial.println("AT+CFUN=1");
  //delay(1000);
  //mySerial.println("ATE0&W"); // Turning the echo off in modem  
mySerial.println("AT+CMGDA=\"DEL ALL\""); // Delete all the messages
delay(1000);
mySerial.println("AT+CNMI=2,2,0,0,0"); // Deliver the SMS content 
delay(1000);
mySerial.println("AT+CMGR=1");
delay(1000);

Serial.println("started");
//CLearing EEPROM


//ClearEEPROM();


  delay(100);

// Write input and output names to EEPROM for the first time
//WriteIONamesfirstime(); //setupeeprom.h

// Write Input flags into EEPROM
//WriteInputFlagsfirsttime(); //setupeeprom.h

// Write delays into EEPROM
//WriteInputDelaysfirsttime(); //setupeeprom.h

// Write input on messages into EEPROM
//ReadioNamesfrmEprom(); // write input on msg uses the IO names. Hence we have to read the io names first.
//WriteInputONmsgfirsttime(); //setupeeprom.h

// Write input on messages into EEPROM
//WriteInputOFFmsgfirsttime(); //setupeeprom.h

// Reading from EEPROM
ReadUsersandAdmin();
ReadioNamesfrmEprom(); //setupeeprom.h
ReadInputStatusFlag(); //setupeeprom.h
ReadInputDelays();//setupeeprom.h

inputonmsgprint(); //setupeeprom.h
inputoffmsgprint(); //setupeeprom.h

pinMode(input0, INPUT);
pinMode(input1, INPUT);
pinMode(input2, INPUT);
pinMode(input3, INPUT);

pinMode(output0, OUTPUT);
pinMode(output1, OUTPUT);
pinMode(output2, OUTPUT);
pinMode(output3, OUTPUT);

// set the initial status of the inputs
for(i=0;i<4;i=i+1)
{
  if ((inputstatusflag[i]&0x01)==0)
  {
    instatusdefault=(instatusdefault&(0x00<<i)); // normally open
  }
  else
  if ((inputstatusflag[i]&0x01)==1)
  {
    instatusdefault=(instatusdefault|(0x01<<i)); // normally closed
  }
 }
}



void loop() {

  MsgRd();
/*for(i=0;i<4;i++)
{
while((latchreset&(0x01<<i))==0)
{
  stringtemp="input"+String(i);
  instatusrd=(instatusrd&(digitalRead(stringtemp)<<i));
  if((instatusrd&(0x01<<i))!=(instatusdefault&(0x01<<i))
  {
    delay(delaymin[i]*60+delaysec[i]);
    msg="
  }
}
} 
*/
}

// EEPROM MAPPING
// Address  what
// 0-1      International code and one digit(MSB) of the phone no. of user0
// 2-5      Remaining 9 digits of the phine no. of user0
// 6-7      International code and one digit(MSB) of the phone no. of admin
// 8-11     Remaining 9 digits of the phine no. of admin
// 12-13      International code and one digit(MSB) of the phone no. of user1
// 14-17      Remaining 9 digits of the phine no. of user1
// 18-19     International code and one digit(MSB) of the phone no. of user2
// 20-23      Remaining 9 digits of the phine no. of user2
// 24-25      International code and one digit(MSB) of the phone no. of user3
// 26-29      Remaining 9 digits of the phine no. of user3
// 30-31      International code and one digit(MSB) of the phone no. of user4
// 32-35      Remaining 9 digits of the phine no. of user4
// 36-37      International code and one digit(MSB) of the phone no. of user5
// 38-41      Remaining 9 digits of the phine no. of user5
// 42-62        input1 name ( 20 bytes for 20 characters and 1 byte to store the length of the string) 
// 63-83        input2 name ( Max 20 characters) 
// 84-104       input3 name ( Max 20 characters) 
// 105-125      input4 name ( Max 20 characters) 
// 126-146      output1 name ( Max 20 characters) 
// 147-167      output2 name ( Max 20 characters) 
// 168-188      output3 name ( Max 20 characters) 
// 189-209      output4 name ( Max 20 characters) 
// 211          Input Status flag of input 1
// 212          Input Status flag of input 2
// 213          Input Status flag of input 3
// 214          Input Status flag of input 4
//215-216       Inputondelay[0]
//217-218       Inputondelay[1]
//219-220       Inputondelay[2]
//221-222       Inputondelay[3]
//223-224       Inputoffdelay[0]
//225-226       Inputoffdelay[1]
//227-228       Inputoffdelay[2]
//228-230       Inputoffdelay[3]
//231-296       INput0 on message ( 6 words(10 characters per word)= 60chars +5 spaces between the words= 65chars ( or 65 bytes) and , 1 byte for storing the length of the string = )
//297-361       INput1 on message ( 6 words, 5 spaces between the words and 10 characters per word)
//362-426       INput2 on message ( 6 words, 5 spaces between the words and 10 characters per word)
//427-491       INput3 on message ( 6 words, 5 spaces between the words and 10 characters per word)
//492-556       INput0 off message ( 6 words, 5 spaces between the words and 10 characters per word)
//557-621       INput1 off message ( 6 words, 5 spaces between the words and 10 characters per word)
//622-686       INput2 off message ( 6 words, 5 spaces between the words and 10 characters per word)
//687-751       INput3 off message ( 6 words, 5 spaces between the words and 10 characters per word)
