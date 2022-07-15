void ReadSmsTxt()
{
index = entiremsg.indexOf('\n');
index2 = entiremsg.indexOf('\n', index+1); // look for second newline character from the next position of the first newline character
//Serial.print("index is");
//Serial.println(index);
smstxt1 = entiremsg.substring((index2 + 1), (index2 + 4));//.toInt();
smstxt1.toUpperCase();
smstxt2 = entiremsg.substring((index2 + 5), (index2 + 10));//.toInt();
smstxt2.toUpperCase();

Serial.print("sub_Sub_str is :");
Serial.println(smstxt1);
Serial.println(smstxt2);
}