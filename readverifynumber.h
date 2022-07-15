void ReadVerifyNumber()
{
     numtoincldel= entiremsg.substring((index2 + 11), (index2 + 24));
     Serial.print("numtoincl or del is set to ");
    Serial.println(numtoincldel);

if((numtoincldel!="")&&(sender!="")) 
{  
    if((numtoincldel.length()==13)) // checking for 13 characters 

    {   
      if(numtoincldel[0]!='+') // checking if the first chaaracter is +
        {
          response=2; 
          SendResponse();
        }

      for(i=1;i<numtoincldel.length();i++) // checking if 12 characters are digits 
        {
            if(isdigit(numtoincldel[i]))
              {
                continue;
              }
            else {
              flag=true;
                break;
              }
        }
      if(flag==true)
      {
        response=3;
      SendResponse();
      }
    }
}
/*
else 
{
response=6;
SendResponse();
}
*/
} 