 // checks if the user already exists and if yes, sends response and sets user flag
 // FOr setting the bit to 1, OR it with that bit 1 and all others 0
//For resetting or setting a bit to 0, AND it with that bit 0 and all other bits 1
 void CheckifUserExists()
 {
  uint8_t j;
  flag=flag&(0xFB);    //userflag is the 3rd bit from LSB. To reset it or set it to 0 and not disturb other bits, AND the flag with FB ie., 1111 1011
 for(j=0;j<5; j++)
          {
            if((numtoincldel==usernum9digits[j])&&(firstnumtoincldel==usernum3digits[j]))
            {
              flag=(flag|(0x04));
            }
            else
            {continue;}
          }
          Serial.print("flag is ");
          Serial.println(flag);
 }
