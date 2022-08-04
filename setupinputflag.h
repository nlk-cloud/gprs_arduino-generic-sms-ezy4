void ReadNamefromCommand(const String &command, char from, char to, const String &name)
{
    uint8_t p;
    uint8_t position = 0;
    uint8_t position2 = 0;
    uint8_t position3 = 0;
    name = "";
    position = entiremsg.indexOf(command);

    // Read the name of the input
    if (position != -1)
    {
        position2 = entiremsg.indexOf(from, position + 1);
        position3 = entiremsg.indexOf(to, position2 + 1);
        name = entiremsg.substring((position2 + 1), (position3));
        Serial.print("name is");
        Serial.println(name);
        Serial.println(name.length());
    }
}
/*
void ReadNamefromCommand(const String &command)
{
    uint8_t p;
    uint8_t position=0;
    uint8_t position2=0;
    uint8_t position3=0;
    fromname="";
    position = entiremsg.indexOf(command);

    // Read the name of the input
     if(position != -1)
     {
        position2 = entiremsg.indexOf(' ', position+1);
        position3 = entiremsg.indexOf(' ', position2+1);
        fromname=entiremsg.substring((position2+1), (position3));
        Serial.print("fromname is");
        Serial.println(fromname);
        Serial.println(fromname.length());
     }

     position = entiremsg.indexOf("TO");
     if(position != -1)
     {
        position2 = entiremsg.indexOf(' ', position+1);
        position3 = entiremsg.indexOf('\n', position2+1);
        toname=entiremsg.substring((position2+1), ((position3)-1));
        Serial.print("to name is");
        Serial.println(toname);
        Serial.println(toname.length());
     }

}
*/
// Resets the bit(s) and returns the index of the input whose bit is reset
int resetbit(int andwith)
{
    uint8_t a;
    for (a = 0; a < 4; a++)
    {

        if (fromname == inputname[a])
        {

            if (((inputstatusflag[a]) & (~(andwith))) == 0)
            {
                a = 4;
                break;
            }
            else if (((inputstatusflag[a]) & (~(andwith))) != 0)
            {
                inputstatusflag[a] = inputstatusflag[a] & (andwith);
                offsetaddr = 211 + (a * 8);
                EEPROM.write(offsetaddr, inputstatusflag[a]);
                inputstatusflag[a] = EEPROM.read(offsetaddr);
                break;
            }
        }
    }
    return (a);
}

// sets the bit(s) and returns the index of the input whose bit is set
uint8_t setbit(int orwith)
{
    uint8_t b;
    for (b = 0; b < 4; b++)
    {

        if (fromname == inputname[b])
        {

            // Check if the bit is already set
            if (((inputstatusflag[b]) & (orwith)) != 0)
            {
                b = 4;
                break;
            }
            else if (((inputstatusflag[b]) & (orwith)) == 0)
            {
                inputstatusflag[b] = inputstatusflag[b] | (orwith);
                offsetaddr = 211 + (b * 8);
                EEPROM.write(offsetaddr, inputstatusflag[b]);
                inputstatusflag[b] = EEPROM.read(offsetaddr);
                break;
            }
        }
    }

    return (b);
}

void latchnonlatch()
{
    uint8_t p;
    // String
    kiwrd = "MAKE";
    ReadNamefromCommand(kiwrd, ' ', ' ', fromname); // input name will be read here
    if (entiremsg.indexOf("NONLATCHING") > 0)
    {

        Serial.println("Entered non latching if");
        p = resetbit(0xFD);
        if (p == 4)
        {
            msg = "Either the name is not matching or the input is already a nonlatching input. Please check.";
        }
        else
        {
            msg = "The input " + inputname[p] + " is now a nonlatching input";
        }
        response = 16;
        SendResponse();
    }
    else

        if (entiremsg.indexOf("LATCHING") > 0)
    {
        p = setbit(0x02);
        if (p == 4)
        {
            msg = "Either the name is not matching or the input is already a latching input. Please check.";
        }
        else
        {
            msg = "The input " + inputname[p] + " is now a latching input";
        }
        response = 16;
        SendResponse();
    }
}
// sets bit for normally open (0) or normally closed (1)
void openclose()
{
    uint8_t p;
    // String
    kiwrd = "MAKE";
    ReadNamefromCommand(kiwrd, ' ', ' ', fromname); // input name will be read here
    if (entiremsg.indexOf("OPEN") > 0)
    {
        p = resetbit(0xFE);
        if (p == 4)
        {
            msg = "Either the name is not matching or the input is already a normally open input. PLease check.";
        }
        else
        {
            msg = "The input " + inputname[p] + " is now a normally open input";
        }
        response = 16;
        SendResponse();
    }
    else
    {
        p = setbit(0x01);
        if (p == 4)
        {
            msg = "Either the name is not matching or the input is already a normally closed input. Please check";
        }
        else
        {
            msg = "The input " + inputname[p] + " is now a normally closed input";
        }
        response = 16;
        SendResponse();
    }
}

void linkinout()
{
    byte p, q;

    // String
    kiwrd = "LINK";
    ReadNamefromCommand(kiwrd, ' ', ' ', fromname);
    kiwrd = "TO";
    ReadNamefromCommand(kiwrd, ' ', '\r', toname);

    for (q = 0; q < 4; q = q + 1)
    {
        if (toname == outputname[q])
        {
            //        Serial.println(q);
            //        Serial.print(" q after shifting left ");
            //        Serial.println(q<<3,HEX);
            //      Serial.println((q<<3)|(0x04));
            // the 3rd bit is set since link is on and the output bits are set in the next two bita)
            p = setbit((q << 3) | (0x04));
            if (p == 4)
            {
                msg = "Either the name is not matching or the input is already linked to the same output. please check";
            }
            else
            {
                msg = "The input " + inputname[p] + " is linked to " + outputname[q];
            }
            response = 16;
            SendResponse();
        }
    }
}

void clearlink()
{
    uint8_t p, q;
    // String
    kiwrd = "CLEAR";
    ReadNamefromCommand(kiwrd, ' ', ' ', fromname);
    p = resetbit(0xE3); // resetting the link bit to 0 and the output bits to 00
    if (p == 4)
    {
        msg = "Either the name is not matching or the input link is already cleared. Please check.";
    }
    else
    {
        msg = "The link of " + inputname[p] + " is now cleared";
    }
    response = 16;
    SendResponse();
}

void linkstatus()
{
    uint8_t p, q;
    msg = "";
    for (p = 0; p < 4; p = p + 1)
    {
        // Extract link bit and check if it is linked or not
        q = ((inputstatusflag[p] & (0x04)) >> 2);
        if (q == 1)
        {
            // Extract the bits indicating the output to which it is linked
            q = ((inputstatusflag[p] & (0x18)) >> 3);
            // Serial.println((inputstatusflag[p]&(0x18)),BIN);
            // Serial.println("value of q is");
            // Serial.println(q);
            msg = msg + inputname[p] + " is linked to " + outputname[q] + ", ";
        }
    }
    if (msg == "") // implies no inputs are linked
    {
        msg = " No inputs are linked to any outputs";
    }

    response = 16;
    SendResponse();
}

void resetlatchedinput()
{
    uint8_t p;
    // String
    kiwrd = "RESET";
    ReadNamefromCommand(kiwrd, ' ', '\r', fromname);
    // Serial.println("Entered reset");
    p = resetbit(0xDF);
    // Serial.println(p);
    if (p == 4)
    {
        msg = fromname + " latch is not currently set so does not need clearing";
    }
    else
    {
        msg = fromname + " is no longer in a latched state and will begin to be monitored again";
    }
    response = 16;
    SendResponse();
}

void alerts()
{
    uint8_t p;
    if (entiremsg.indexOf("ON") > 0)
    {
        for (p = 0; p < 4; p = p + 1)
        {
            inputstatusflag[p] = inputstatusflag[p] | (0x40);
            offsetaddr = 211 + (p * 8);
            EEPROM.write(offsetaddr, inputstatusflag[p]);
            inputstatusflag[p] = EEPROM.read(offsetaddr);
        }
        msg = "Alerts have been enabled";
        response = 16;
        SendResponse();
    }

    if (entiremsg.indexOf("OFF") > 0)
    {
        for (p = 0; p < 4; p = p + 1)
        {
            if (entiremsg.indexOf("EXC LINKS") > 0)
            {
                // alerts off and linked outputs if any will remain same - 1011 1111
                inputstatusflag[p] = inputstatusflag[p] & (0xBF);
                msg = " Alerts have been disabled but linked outputs will still work";
            }
            else
            {
                // alerts off and linked outputs will not work - 1010 0011
                inputstatusflag[p] = inputstatusflag[p] & (0xA3);
                msg = "Alerts have been disabled";
            }
            offsetaddr = 211 + (p * 8);
            EEPROM.write(offsetaddr, inputstatusflag[p]);
            inputstatusflag[p] = EEPROM.read(offsetaddr);
        }

        response = 16;
        SendResponse();
    }
}

void setdelay()
{
    /**** since one input can have both off delay and on delay, which means
     * we need one byte to store on delay and another to store off delay. or atleast
     * 6 bytes for each. Even if we make the input statusflag as int, we will not get that many bytes.
     * Hence, it is better to add new variables to store the delays*******/

    uint8_t p, n;
    // String
    kiwrd = "CHANGE";
    ReadNamefromCommand(kiwrd, ' ', ' ', fromname);
    for (p = 0; p < 4; p = p + 1)
    {
        if (fromname == inputname[p])
        {
            kiwrd = "TO";
            ReadNamefromCommand(kiwrd, ' ', ' ', toname);
            if ((!(toname.toInt()) > 1 && (toname.toInt()) < 99))
            {
                msg = "The delay value is not inputted properly. Hence the delay has not been changed";
                break;
            }
            if (entiremsg.indexOf("ON") > 0)
            {
                if (entiremsg.indexOf("MIN") > 0)
                {
                    inputondelay[p] = (toname.toInt()) * 60;
                    msg = " The " + inputname[p] + "ON delay has been set to " + toname + " MIN";
                }
                else if (entiremsg.indexOf("SEC") > 0)
                {
                    inputondelay[p] = (toname.toInt());
                    msg = " The " + inputname[p] + "ON delay has been set to " + toname + " SEC";
                }
                offsetaddr = 215 + (n * 2);
                writeUnsignedIntIntoEEPROM(offsetaddr, inputondelay[n]);
                inputondelay[n] = readUnsignedIntFromEEPROM(offsetaddr);
            }

            if (entiremsg.indexOf("OFF") > 0)
            {
                if (entiremsg.indexOf("MIN") > 0)
                {
                    inputoffdelay[p] = (toname.toInt()) * 60;
                    msg = " The " + inputname[p] + "OFF delay has been set to " + toname + " MIN";
                }
                else if (entiremsg.indexOf("SEC") > 0)
                {
                    inputoffdelay[p] = (toname.toInt());
                    msg = " The " + inputname[p] + "OFF delay has been set to " + toname + " SEC";
                }
                offsetaddr = 223 + (n * 2);
                writeUnsignedIntIntoEEPROM(offsetaddr, inputoffdelay[n]);
                inputoffdelay[n] = readUnsignedIntFromEEPROM(offsetaddr);
            }
        }
        response = 16;
        SendResponse();
        break;
    }
}

void setinputonoffmessages()
{
    uint8_t n;
    if (entiremsg.indexOf("OFFMESSAGE") > 0)
    {
        kiwrd = "SSAGE"; // since we want to extract the position of 2nd E in the word message
        ReadNamefromCommand(kiwrd,'E', ' ', fromname);
        n = fromname.toInt();
        Serial.println(fromname.toInt());
        kiwrd = fromname;
        ReadNamefromCommand(kiwrd,' ', '\r', toname);
        offsetaddr = 492 + ((n - 1) * 66);
        WriteCharIntoEprom(offsetaddr, toname);
    }
}