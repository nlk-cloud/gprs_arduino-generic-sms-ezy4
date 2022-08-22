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
// Response =7 => response for user doesnot exist. Hence cannot be deleted
// Response =8 => response for cannot delete the only user
// Response =9 => response for successful deletion of the user
// Response =10 => response for userlist
// Response =11 => Response for trying to add more than 5 users 
// Response =12 => Response for version
// Response =13 => Response for changing i/o names
// Response =14 => response for IO name not valid
// Response =15 => Response if the from name doesnt match with any of the existing input or output names
// Response =16 => Response to 'Input Status Command
//               => Response for latching and unlatching inputs. Msg is set in calling function
// Response 17 => Sending response to all users

void SendResponse()
{
  uint8_t y;
switch (response)
{
// response for duplicate user addition
case 0:   
     msg= '+'+String(firstnumtoincldel)+String(numtoincldel)+" Already active. Pls check and try again";
     //sendto=sender;
     stringtemp='+'+String(firstsender)+String(sender);
     SendMessage();
     delay(20000);     
break;

// response for Adding user
case 1:   
    msg= "you are added to user list";
    //sendto=numtoincldel;
    stringtemp='+'+String(firstnumtoincldel)+String(numtoincldel);
    SendMessage();
    delay(30000);
          
    {
    //Serial.println("entered error else");
    msg='+'+String(firstnumtoincldel)+String(numtoincldel)+ " User added to user list";
    //sendto=sender;
    stringtemp='+'+String(firstsender)+String(sender);
    SendMessage(); //sending msg to the admin  
    delay(20000);
    }  
break;

//response for checking for + sign in the no.
case 2:
  msg="the number should have + with international code";
  stringtemp='+'+String(firstsender)+String(sender);
  SendMessage(); 
  delay(20000);  
break;

//response for checking for symbols in no.
case 3:
  msg="there should be no letters or symbols (except +) in the number. Please check."; 
  //sendto=sender;
  stringtemp='+'+String(firstsender)+String(sender);
  SendMessage(); 
  delay(20000);  
break;

//response for adding user0
case 4:
    Serial.println("entered sendresponse");
    msg="You have been added successfully to the user list";
    //sendto=sender;
    stringtemp='+'+String(firstsender)+String(sender);
    SendMessage();
    delay(20000);
break;

//response for adding admin
case 5:   
    //Serial.println("entered case5");
    msg= " you are added as admin";
   // sendto=numtoincldel;
    stringtemp='+'+String(firstnumtoincldel)+String(numtoincldel);
    SendMessage();
    delay(30000);
       
    {
    Serial.println("entered 2nd case 5");
    msg='+'+String(firstnumtoincldel)+String(numtoincldel)+ " added as admin";
    //sendto=sender;
    stringtemp='+'+String(firstsender)+String(sender);
    SendMessage(); //sending msg to the admin  
    delay(20000);    
    }  
break;

case 6:
      msg="Pls check the number to be included or deleted";
      //sendto=sender;
      stringtemp='+'+String(firstsender)+String(sender);
      SendMessage();
      delay(20000);
break;

// response for non-existent user deletion
case 7:   
     msg= '+'+String(firstnumtoincldel)+String(numtoincldel)+" is not in user list. Pls check and try again";
     //sendto=sender;
     stringtemp='+'+String(firstsender)+String(sender);
     SendMessage();
     delay(20000);     
break;

// There is only one user and cannot be deleted
case 8:   
     msg= "Cannot delete the only user";
     //sendto=sender;
     stringtemp='+'+String(firstsender)+String(sender);
     SendMessage();
     delay(20000);     
break;

// User has been deleted
case 9:   
  Serial.println("Enetered case 9");
     msg= '+'+String(firstnumtoincldel)+String(numtoincldel)+ " has been deleted from the user list";
     //sendto=sender;
     stringtemp='+'+String(firstsender)+String(sender);
     SendMessage();
     delay(20000);     
break;

// User list
case 10:   
     
    msg="";
    for(i=0;i<5;i++)
    {
      msg='+'+String(usernum3digits[i])+String(usernum9digits[i])+","+msg;
    }
     msg= "User List is: " + msg;
//Serial.print("msg is: ");
//Serial.println(msg);
     //sendto=sender;
     stringtemp='+'+String(firstsender)+String(sender);
     SendMessage();
     delay(20000);     
break;

//response for trying to add more than 5 users
case 11:
Serial.println("Entered case 11");
  msg="sorry only 5 users allowed";
  //sendto=sender;
  stringtemp='+'+String(firstsender)+String(sender);
  SendMessage();  
  delay(20000);  
break;

// Version message
case 12:
  msg="Metriq SMS-2 Version is "+ String(Version);
  //sendto=sender;
  stringtemp='+'+String(firstsender)+String(sender);
  SendMessage();  
  delay(20000);  
break;

// Response for changing the io name

case 13:
      msg="The input/Output name has been changed to"+toname;
      //sendto=sender;
      stringtemp='+'+String(firstsender)+String(sender);
      SendMessage();
      delay(20000);
break;

// Response if the to name has more than 20 characters or space or underscore
case 14:
      msg="Sorry. this name is not allowed. Please check and try again";
      //sendto=sender;
      stringtemp='+'+String(firstsender)+String(sender);
      SendMessage();
      delay(20000);
break;

// Response if the from name doesnt match with any of the existing input or output names
case 15:
      msg="Sorry. this name does not exist. Please check and try again";
      //sendto=sender;
      stringtemp='+'+String(firstsender)+String(sender);
      SendMessage();
      delay(20000);
break;

//Generic Response. Message is already set whne it comes here 
case 16:
      // msg is set in inputstatus() or LatchandUnlatch()
      stringtemp='+'+String(firstsender)+String(sender);
      SendMessage();
      delay(20000);
      break;

// sending message all 5 users. Mag is already set, when it comes here
case 17:
     stringtemp='+'+String(firstadmin)+String(admin);
     SendMessage();
     delay(20000);
     for(y=0;y<5;y++)
     { stringtemp='+'+String(usernum3digits[y])+String(usernum9digits[y]);
      SendMessage();
      delay(20000);
     }
      break;
     }
      
}
