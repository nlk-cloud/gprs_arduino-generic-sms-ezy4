//https://roboticsbackend.com/arduino-store-int-into-eeprom/#:~:text=Write%20Int%20into%20EEPROM,-void%20writeIntIntoEEPROM(int&text=For%20the%20first%20byte%2C%20we,right%2C%20we%20simply%20obtain%2001000111.
void WriteLongIntoEEPROM(int address, long number)
{ 
  EEPROM.write(address, (number >> 24) & 0xFF);
  EEPROM.write(address + 1, (number >> 16) & 0xFF);
  EEPROM.write(address + 2, (number >> 8) & 0xFF);
  EEPROM.write(address + 3, number & 0xFF);
}

long ReadLongFromEEPROM(int address)
{
  return ((long)EEPROM.read(address) << 24) +
         ((long)EEPROM.read(address + 1) << 16) +
         ((long)EEPROM.read(address + 2) << 8) +
         (long)EEPROM.read(address + 3);
}


void writeUnsignedIntIntoEEPROM(int address, unsigned int number)
{ 
  EEPROM.write(address, number >> 8);
  EEPROM.write(address + 1, number & 0xFF);
}


unsigned int readUnsignedIntFromEEPROM(int address)
{
  return (EEPROM.read(address) << 8) + EEPROM.read(address + 1);
}

int WriteCharIntoEprom(int addrOffset, const String &strToWrite)
{
int k;
// Add to the EEPROM
//Serial.println(" Entered EEPROM write");
//Serial.println(strToWrite);
//Serial.println(addrOffset);
  byte len = strToWrite.length();
  //Serial.println(len);
  EEPROM.write(addrOffset, len);
  for (k = 0; k < len; k++)
  {
    EEPROM.write(addrOffset + 1 + k, strToWrite[k]);
  }
  
  return addrOffset + 1 + len;

}

int ReadCharFromEprom(int addrOffset, String *strToRead)
{
int m;
  int newStrLen = EEPROM.read(addrOffset);
  char data[newStrLen + 1];
  for ( m = 0; m < newStrLen; m++)
  {
    data[m] = EEPROM.read(addrOffset + 1 + m);
  }
  data[newStrLen] = '\0'; // !!! NOTE !!! Remove the space between the slash "/" and "0" (I've added a space because otherwise there is a display bug)
  *strToRead = String(data);
  return addrOffset + 1 + newStrLen;

}
