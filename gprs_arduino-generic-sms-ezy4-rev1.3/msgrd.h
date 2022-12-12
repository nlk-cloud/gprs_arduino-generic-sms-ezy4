// Serial inputs basics is taken from https://forum.arduino.cc/t/serial-input-basics-updated/382007/3


void MsgRd()
{
    entiremsg="";
    flag=0;
    //ReadCharacterbyCharacter();
 //Serial.println("entered msgrd");    
    ReadString();
    //ReadCharAppendtoString();
  
 
entiremsg.toUpperCase();


if(flag==1)

{

  ReadSenderno(); // Read the sender's no
   
  


if ( entiremsg.indexOf("SET USER")>0)
      {  
          SetUser0();        // setting initial user. No number will follow this command
      }


  else

    {
      
      if((sender==user0)&&(firstsender==firstuser0))
      {
         if(entiremsg.indexOf("ADD ADMIN")>0)
            {
                InclAdmin(); 
            }   

      }
    if((sender==admin)&&(firstsender==firstadmin))
        {
          if(entiremsg.indexOf("ADD USER")>0)
        
            {
             InclUser();             
            }      

        if(entiremsg.indexOf("DEL USER")>0)
            {
               Deluser();               
            }
            
        if(entiremsg.indexOf("USER LIST")>0)
        {
            response=10;
            SendResponse();
        }
        
        if(entiremsg.indexOf("CHANGE")>0)    
        {
          if(entiremsg.indexOf("DELAY")>0)
          {
          setdelay();    //setinputflag.h
          }
        else
        {
          ReadAssignioNames();
        }      
        }
        
                    
         if(entiremsg.indexOf("INPUT STATUS")>0)       
        {
          InputStatus();  // status.h
        }        

       if(entiremsg.indexOf("OUTPUT STATUS")>0)       
        {
          outputstatus(); // status.h
        }    

        if((entiremsg.indexOf("LATCHING")>0)||(entiremsg.indexOf("NONLATCHING")>0))    
        {
          latchnonlatch(); // setinputflag.h
        }

        if((entiremsg.indexOf("OPEN")>0)||(entiremsg.indexOf("CLOSED")>0))    
        {
          openclose();    //setinputflag.h
        }

        if((entiremsg.indexOf("LINK")>0)&&(entiremsg.indexOf("TO")>0))    
        {
          linkinout();    //setinputflag.h
        }
        
        if(entiremsg.indexOf("LINK OUTPUTS")>0)   
        {
          linkout();    //setoutputflag.h
        }
        
        if((entiremsg.indexOf("CLEAR")>0)&&(entiremsg.indexOf("LINK")>0))    
        {
          if (entiremsg.indexOf("OUTPUTS")>0)
          {
            linkout();
          }
          else
          {
            clearlink();    //setinputflag.h
          }
          
        }
        if((entiremsg.indexOf("LINK STATUS")>0))    
        {
          linkstatus();    //setinputflag.h
        }
        
        if((entiremsg.indexOf("RESET")>0))    
        {
          if((entiremsg.indexOf("FACTORY RESET")>0))
         {
          factoryreset(); //resetreboot.h
         }
          else
          {
          resetlatchedinput();    //setinputflag.h
          }
        }

        if((entiremsg.indexOf("REBOOT")>0)) 
        {
          reboot(); //resetreboot.h
        }
      
        if((entiremsg.indexOf("ALERTS")>0))    
        {
          alerts();    //setinputflag.h
        }

        if(entiremsg.indexOf("MESSAGE")>0)
        {
          setinputonoffmessages(); //setinputflag.h
        }

        if((entiremsg.indexOf("SET")>0)&&(entiremsg.indexOf("ON TIME TO")>0))
        {
          turnandsetoutput();
        }

      if((entiremsg.indexOf("TURN")>0))
        {
          if((entiremsg.indexOf("ALIVE TEXT")>0))
          {
            alivetext(); //resetreboot.h
          }
          else
          {
          turnandsetoutput();         
          }       
        }
/*
      if((entiremsg.indexOf("CHANGE BATTERY")>0))
      {
       setbatterytrigger(); //resetreboot.h
      }

      if(entiremsg.indexOf("LINK BATTERY")>0)
      {
       linkbatteryalarm(); //resetreboot.h
      }

      if(entiremsg.indexOf("CLEAR BATTERY")>0)
      {
       clearbatterylink(); //resetreboot.h
      }

      if((entiremsg.indexOf("VOLTAGE STATUS")>0))
      {
       batteryvoltage(); //resetreboot.h
      }
  */       
        if(entiremsg.indexOf("VER")>0)
        {
            response=12;
            SendResponse();
        }        

        if(entiremsg.indexOf("SIG")>0)
        {
            signalstrength(); //resetreboot.h
        }  
        }
}   

//userflag=false;
//numflag=false;
flag=0;
sender=0;
firstsender=0;
firstnumtoincldel=0;
numtoincldel=0;
//newData = false;     
//delay(5000); // delay before it starts new cycle

}

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
