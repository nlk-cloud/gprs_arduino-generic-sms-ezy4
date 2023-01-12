// Reads the number to be included, checks it, and if there is some problem, sends SMS as well as sets numflag
// from and to should be set before entering this function
// FOr setting the bit to 1, OR it with that bit 1 and all others 0
//For resetting or setting a bit to 0, AND it with that bit 0 and all other bits 1
void ReadVerifyNumber()
{
    uint8_t index,index2;
    String stringtemp1="";
    flag=flag&0xF7; //Numflag is the 4th bit from LSB. To reset it or set it to 0 and not disturb other bits, AND the flag with F7
    index = entiremsg.indexOf('\n');
    index2 = entiremsg.indexOf('\n', index+1);
    to=entiremsg.indexOf('\n', index2+1);
    stringtemp1= entiremsg.substring((index2 + from), (to));
    //Serial.println("stringtemp is");
    //Serial.println(stringtemp1);
    if((sender!=0)) 
{  
  // check if the number begins with a +
    if((stringtemp1.indexOf('+'))==-1) // checking if the first chaaracter is +
        {
          response=2; 
          SendResponse();
          flag=flag|(0x08); // set numflag if the first digit is not +
        }

Serial.print( "Stringtemp length is:" );
Serial.println(stringtemp1.length());
//Serial.println(stringtemp1.indexOf('+'));
  if((stringtemp1.indexOf('+'))!=-1)
  { 
    if((stringtemp1.length())!=14)
  
    {
      Serial.println("entered num length is not 14");
      response=6;
      SendResponse();
      flag=flag|(0x08);; // set num flag if there are no 14 characters - 12 no.s, + char and carriage return
    }
   
    if(((stringtemp1).length())==14)  

    {   
      for(i=1;i<(stringtemp1.length()-1);i++) // checking if 12 characters are digits. First char is +, last char is carriage return. So, we should check for in between characters
        {
            if(isdigit(stringtemp1[i]))
              {
                //Serial.print(" i is");
                //Serial.println(i);
                continue;
              }
            else 
              {
               flag=flag|(0x08); // set numflag if the number has characters other than digits
               response=3;
               SendResponse();
               break;
              }
        }
 //Serial.print("flag ");
 //Serial.println(flag);
      //if((flag&(0x08))== 8)
     // {
       // Serial.println("Entered if flag &");
       
      //}
    }

  }
}

// if flag is not set, it means number is good. Now, lets extract the number to an int and move on
if((flag&(0x08))== 0)
{

    firstnumtoincldel= (stringtemp1.substring(1, 4)).toInt();
    numtoincldel= (stringtemp1.substring(4, 14)).toInt();
    Serial.print("numtoincl or del is set to ");
    Serial.println(firstnumtoincldel);
    Serial.println(numtoincldel);   
    
}

}




void ReadSenderno()
{
  uint8_t pos=0;
  uint8_t pos2=0;
  pos = entiremsg.indexOf("+CMT");
     if(pos != -1)
        {
          pos2 = entiremsg.indexOf('+', pos+1); 

          if (pos2==-1)
          {
            response=2;
            SendResponse();
          }
          
          //Serial.print("position is ");
          //Serial.println(pos);
          firstsender = (entiremsg.substring((pos2+1), (pos2+4))).toInt();
          sender= (entiremsg.substring((pos2+4), (pos2+13))).toInt();
        }
Serial.print("sender is ");
Serial.print(firstsender);
Serial.println(sender);
}
