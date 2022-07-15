// Serial inputs basics is taken from https://forum.arduino.cc/t/serial-input-basics-updated/382007/3


void MsgRd()
{
    entiremsg="";
    newData = false;
    //ReadCharacterbyCharacter();
 //Serial.println("entered msgrd");    
    ReadString();
    //ReadCharAppendtoString();
  
 

if(newData==true)

{

  ReadSenderno(); // Read the sender's no
  ReadSmsTxt(); ////Read the sms txt 

 if ((smstxt1=="SET") && (smstxt2=="USER0")) // setting initial user. No number will follow this command
      {  
          SetUser0();        
      }

  else
    {
     Serial.println("entered msgrd else");
      // Read the number to include or delete  
      ReadVerifyNumber(); // if number is faulty numflag is set
  
      if ((numflag==false) && (smstxt1=="ADD"))
      {
        Incl();
      }  
    }      
  
}   

userflag=false;
numflag=false;
sender="";
newData = false;     
//delay(5000); // delay before it starts new cycle

}
/*else
if( entiremsg != "")
{
  msg="pls check ur comnd again";
  sendto=sender;
  SendMessage();    
}
        sender="";
        entiremsg="";

}

*/
/*

 if (smstxt=="ADD")
 {
    if (count==arraysize)
    { msg= "Array is full. Del first";
        //Serial.print("msg is set to ");
        //Serial.println(msg);
        //Serial.print("sender is set to ");
        //Serial.println(sendto);
      SendMessage();}
      else 

      if (count<arraysize)
      {
        j=0;
        while( (recipientNumber[j] != "\0") && (j< arraysize) )  // count only holds the n. of valid phone no.s in the array.
        // But we have to scan the entire array. Hence i<arraysize and not i<count
        {
          j=j+1;
        }
Serial.print("j is "); 
Serial.println(j);        
        recipientNumber[j] = numtoincldel;
Serial.print("num is "); 
Serial.println(recipientNumber[j]);        
        count=count+1;
        
        //delay(5000);
        msg="included";
        SendMessage();
        //delay(1000); 
        msg="added to the list";
        sendto=numtoincldel;
        Serial.print("about to send msg to ");
        SendMessage();
        //delay(1000);
        
            
      }
 }
 if (smstxt=="DEL")
{
for(i=0; i<arraysize; i++) //count only holds the n. of valid phone no.s in the array.
        // But we have to scan the entire array to check for the no. to be deleted. Hence i<arraysize and not i<count
{
if (recipientNumber[i] == numtoincldel)  
{
recipientNumber[i] = "\0"; 
count=count-1;
msg="deleted";
sendto=admin;
SendMessage();
}
else continue;

}  

}
Serial.print("count is  ");
Serial.println(count);
for(i=0; i<arraysize; i++) //count only holds the n. of valid phone no.s in the array.
        // But we have to scan the entire array. Hence i<arraysize and not i<count
        {
            //Serial.print("msg is set to ");
        Serial.println(recipientNumber[i]);
 
        }
 
*/

