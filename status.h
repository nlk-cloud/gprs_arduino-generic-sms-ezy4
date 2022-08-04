void msgbuild(uint8_t tmp, String &nameofinput)
{
  Serial.println("Entered msg build");
    if(tmp==1)
    {
        msg=msg+nameofinput+" is ON, " ;
    }    
    else
    if(tmp==0)
    {
        msg=msg+nameofinput+" is OFF, " ;
    }
}

void InputStatus()
{   uint8_t inputstatusflag=0;
    uint8_t temp=0;
    msg="currently ";
    //int k;
Serial.println("Entered input status");
    temp = digitalRead(input0);
    Serial.print("temp is");
    Serial.println(temp);
    msgbuild(temp,inputname[0]);
    temp = digitalRead(input1);
    msgbuild(temp,inputname[1]);
    temp = digitalRead(input2);
    msgbuild(temp,inputname[2]);
    temp = digitalRead(input3);
    msgbuild(temp,inputname[3]);
    response=16;
    SendResponse();
/*
    if(temp==1)
    {
        msg=msg+inputname[0]+"is ON" ;
    }    
    else
    if(temp==0)
    {
        msg=msg+inputname[0]+"is OFF" ;
    }

*/
    
    /*temp = digitalRead(input0);
    inputstatusflag= inputstatusflag | temp;
    temp = digitalRead(inputname[1]);
    inputstatusflag= inputstatusflag | (temp<<1);
    temp = digitalRead(inputname[2]);
    inputstatusflag= inputstatusflag | (temp<<2);
    temp = digitalRead(inputname[3]);
    inputstatusflag= inputstatusflag | (temp<<3);*/
    
}
