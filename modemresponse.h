// Before entering this function, the nwlncntexpected value should be set, replyfrommodem should be cleared


void ModemResponse()
{
  //String collect = "";
  long prev = millis();
  int tst=0;
  //newlinecnt=1;
  rc="";

//long now=millis();
 //Serial.println(millis()-prev);
while(((millis()-prev) < 30000)&&(tst==0))
{
    
  while((mySerial.available()>0)) //&&(newlinecnt<=nwlncntexpected))
  {  

  rc = mySerial.read();
      // if ( rc=='/n')
      //{
      //  newlinecnt=newlinecnt+1;  
      //}              
      replyfrommodem=replyfrommodem+rc;
      tst=1;
  }      
  
  //Serial.println(collect);
    //if ( (collect.indexOf(replyfrommodem) >= 0) || ( collect=="ERROR") ) 
    //{  // full response obtained. Now, we can exit
    //  replyfrommodem=collect;
    //  tst=1;
    //}
  //else { continue;}
   
//}
}

Serial.print("Reply from Modem is ");
Serial.println(replyfrommodem);
delay(10000);

  //if ( collect.indexOf(replyfrommodem) == -1 ) 
    //{  
    //  Serial.println("No response from network");
    //}

    //Serial.println(collect);
}

/*while( 1 ) {
delay(1);  // slow things down a  bit
//char c = (char) sim800l.read();
//collect += c;
collect=mySerial.readString();
if ( millis() - prev > 2000 ) {  // get out if no response in 2 secs
Serial.println("No response from network");
break;
}

if ( collect.indexOf(replyfrommodem) >= 0 ) {  // full response obtained
break;
}
}
*/
