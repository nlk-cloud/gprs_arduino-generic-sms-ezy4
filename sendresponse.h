// As per the manual response from MODEM is not consdered at any stage to take a decision
// Same is implemented in this code too
// Response should be set before calling this function.
//Response = 0 => response for matching (existing) User number
// Response = 1 => response for Add User command
// Response = 2 => response for no + symbol before the number
// Response = 3 => response for no alphabets or symbols in the number
// Response = 4 => response for successfully setting the user0 ( set user0 command )
// Response = 5 => response for setting the admin
// Response = 6 => response if there are no 13 characters in the numtoincldel
void SendResponse()
{
switch (response)
{
// response for duplicate user addition
case 0:   
     msg= numtoincldel+" Already active. Pls check and try again";
     sendto=sender;
     SendMessage();
     delay(20000);     
     flag=false;
break;

// response for Adding user
case 1:   
    msg= "you are added to user list";
    sendto=numtoincldel;
    SendMessage();
    delay(30000);
    /*
    if (replyfrommodem.indexOf("ERROR")>0)
    { 
      Serial.println("entered error if");
      msg="Pls check the number to be included or deleted";
      sendto=sender;
      SendMessage();
      delay(20000);
    }
    else
*/         
    {
    //Serial.println("entered error else");
    msg=numtoincldel+ " User added to user list";
    sendto=sender;
    SendMessage(); //sending msg to the admin  
    delay(20000);
    }  
break;

//response for checking for + sign in the no.
case 2:
  msg="the number should have + with international code";
  sendto=sender;
  SendMessage();  
  delay(20000);  
break;

//response for checking for symbols in no.
case 3:
  msg="there should be no letters or symbols (except +) in the number. Please check."; 
  sendto=sender;
  SendMessage(); 
  delay(20000);
  flag=false;  
break;

//response for adding user0
case 4:
if(flag==true)
  {
    Serial.println("entered sendresponse");
    msg="You have been added successfully to the user list";
    sendto=sender;
    SendMessage();
    delay(20000);
    flag=false;
  }  
break;

//response for adding admin
case 5:   
    //Serial.println("entered case5");
    msg= " you are added as admin";
    sendto=numtoincldel;
    SendMessage();
    delay(30000);
    /*
    if (replyfrommodem.indexOf("ERROR")>0)
    { 
      Serial.println("entered error if");
      msg="Pls check the number to be included or deleted";
      sendto=sender;
      SendMessage();
      delay(20000);      
    }
    else
    */     
    {
    //Serial.println("entered 2nd case 5");
    msg=numtoincldel+ " added as admin";
    sendto=sender;
    SendMessage(); //sending msg to the admin  
    delay(20000);    
    }  
break;

case 6:
      msg="Pls check the number to be included or deleted";
      sendto=sender;
      SendMessage();
      delay(20000);
break;

}

}
