 // checks if the user already exists and if yes, sends response and sets uer flag
 void CheckifUserExists()
 {
 for(i=0;i<5; i++)
          {
            if(numtoincldel==recipientNumber[i])
            {userflag=true;}
            else
            {continue;}
          }

  if (userflag==true)
   {
      response=0;
      SendResponse();
    }

 }