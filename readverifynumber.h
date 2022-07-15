// Reads the number to be included, checks it, and if there is some problem, sends SMS as well as sets numflag
void ReadVerifyNumber()
{
     numtoincldel= entiremsg.substring((index2 + 11), (index2 + 24));
     Serial.print("numtoincl or del is set to ");
    Serial.println(numtoincldel);

if((numtoincldel!="")&&(sender!="")) 
{  
    if(numtoincldel[0]!='+') // checking if the first chaaracter is +
        {
          response=2; 
          SendResponse();
          numflag=true; // set numflag if the first digit is not +
        }
    
    Serial.print("length of num to incl is");
    Serial.println(numtoincldel.length());
    if((numtoincldel.length()==13)) // checking for 13 characters 

    {   
      for(i=1;i<numtoincldel.length();i++) // checking if 12 characters are digits 
        {
            if(isdigit(numtoincldel[i]))
              {
                continue;
              }
            else {
              numflag=true; // set numflag if the number has characters other than digits
                break;
              }
        }
      if(numflag==true)
      {
        response=3;
      SendResponse();
      }
    }
    else 
    {
      numflag=true; // set num flag if there are no 13 characters
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