// this code aims at replicating all the functions listed in ezy- sms-8 but for 4 inputs and 4 outputs using Arduino uno

// Rev 1.2 - Trying to reduce the dnamic RAM usage in this version by reducing the size of variables, strings etc 
#include <SoftwareSerial.h>
SoftwareSerial mySerial(50, 51);
#include <EEPROM.h>
void(* resetFunc) (void) = 0;

#define Version 1.2
/*#define input0 22
#define input1 23
#define input2 24
#define input3 25

#define output0 26
#define output1 27
#define output2 28
#define output3 29 */

#define battery A0 

// Input channels
uint8_t inchannel[4]={22,23,24,25};

// Output channels
uint8_t outchannel[4]={26,27,28,29};

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

unsigned long prev=0;


uint8_t flag=0; // this will be 8 bits. Each bit represents the below flags 
//boolean numflag = false; - MSB
//boolean userflag = false;
//boolean ionameflag=false;
//boolean newData; - LSB


unsigned int inputondelay[4]={30,30,30,30}; // contains delay in secs
unsigned int inputoffdelay[4]={30,30,30,30}; // contains delay in secs
uint8_t outputontime[4]={0,0,0,0}; // whatever 2 digit on time has been entered, will be stored here. whether it is min, days etc will be stored in status flag



//uint8_t latchreset=0; // stores the latch reset status of all the inputs
uint8_t instatusrd; //status of the inputs read and stored here. LSB is the status of input0, next bit is the status of input1 and so on
//uint8_t instatusdefault=0; // default status of all 4 inputs. LSB is for input 0 and so on 
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
//  5               Latch resset
//  4               WHich output is linked to this input ( upperr bit of that output)
//  3               WHich output is linked to this input ( lower bit of that output)
//  2               Link input to output if 1 else inputs and outputs are not linked
//  1               Input Latching (if 1 ) and Input Non-Latching ( if 0)
//  0               normally open(default logic 0, when it is 0) or closed (default logic 1 when it is 1) - 

uint8_t outputstatusflag[4]={0,0,0,0};

// Each output has one status flag. THe settings of each flag are:
// Bit                whatit indicates
//  7                              
//  6               
//  5               1 - output is linked to battery alarm. 0 - link to battery alarim is off               
//  4              (bit4 and bit3) - indicate the outputs to which the current output is linked 
//  3               
//  2              0- Linking is OFF, 1 - Linking is ON 
//  1              (bit0 and bit1)00 - output will be ON or OFF untill the next command
//  0              01- output will be on for specified time and the time is in mins
//                 10 - output will be on for specified time and the time is in hours
//                 11 - output will be on for specified time and the time is in days

uint8_t alivetxttime=0;
//Alive text information
// Bit            What it indicates
// 7              Bits 1 to 7 store the count of hours or days after which alive text has to be sent
// 6              00 indicates no alive text 
// 5
// 4
// 3
// 2
// 1
// 0              0 - implies the number in the abovebits is for hours 1 - implies the number in above bits is days



#include "readwriteEEPROM.h"
//#include "readusrlistfrmeprom.h"
///#include "readcharappendtostring.h"
#include "readstring.h"
#include "sendmsg.h"
#include "setupeeprom.h"
#include "sendresponse.h"
#include "setupinputflag.h"
#include "setoutputflag.h"
#include "status.h"
#include "readassignionames.h"
//#include "checkifuserexists.h" //moved to readassignionames.h
//#include "setuser0.h" //moved to incluser.h
#include "readverifynumber.h"
#include "incluser.h"
#include "incladmin.h"
#include "deluser.h"
//#include "readsenderno.h" //moved to readverifynumber.h
//#include "readsmstxt.h"
//#include "readcharacterbycharacter.h"
#include "resetreboot.h"
#include "drive.h"
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

//Write output status flag for the first time
//WriteOutputFlagsfirsttime();

//Write output ON TIME for the first time
//WriteOutputOntime();

// Write battery trigger voltage for the first time
//Writebatterytrigger();

// Write alive text time for the first time
//Writealivetxttimefirsttime();

// Reading from EEPROM
ReadUsersandAdmin();
ReadioNamesfrmEprom(); //setupeeprom.h
ReadInputStatusFlag(); //setupeeprom.h
ReadInputDelays();//setupeeprom.h
ReadOutputFlags();//setupeeprom.h
ReadOutputONTime();
ReadBatteryTrigger();
Readalivetxttime();

inputonmsgprint(); //setupeeprom.h
inputoffmsgprint(); //setupeeprom.h

/*pinMode(input0, INPUT);
pinMode(input1, INPUT);
pinMode(input2, INPUT);
pinMode(input3, INPUT);

pinMode(output0, OUTPUT);
pinMode(output1, OUTPUT);
pinMode(output2, OUTPUT);
pinMode(output3, OUTPUT);*/

pinMode(battery, INPUT);
for(i=0;i<4;i=i+1)
{
  pinMode(inchannel[i], INPUT);
  pinMode(outchannel[i],OUTPUT);
}
// set the initial status of the inputs
/*for(i=0;i<4;i=i+1)
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
 */
}



void loop() {
 
while(mySerial.available()<=0)
  {
    action(); // drive.h
  }
  
MsgRd();

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
// 752          output Status flag of output 1
// 753          output Status flag of output 2
// 754          output Status flag of output 3
// 755          output Status flag of output 4
// 756          outputontime[0]
// 757          outputontime[1]
// 758          outputontime[2]
// 759          outputontime[3]
// 760-764      Battery trigger voltage - 3 characters for reading value, one for decimal point and anther for the length of the string
// 765          ALive text information
