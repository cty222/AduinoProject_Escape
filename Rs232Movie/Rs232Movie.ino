void setup()
{
  Serial.begin(115200);
}
unsigned char Nak[] = { 0x55, 0x00, 0x33 };
unsigned char Ark[] = { 0x55, 0x01, 0x33 };

unsigned char testCommand1[] = { 0x55, 0x11, 0x33 };
unsigned char testCommand2[] = { 0x55, 0x12, 0x33 };
unsigned char testCommand3[] = { 0x55, 0x13, 0x33 };


void loop()
{
  while(1)
  {

    unsigned char buf[3];
    memset(buf,'\0',sizeof(buf));
    while(1)
    {
       if (Serial.available() > 0) {
         Serial.readBytes((char *)&buf[0],sizeof(unsigned char));
        if(buf[0]!=Ark[0])
        {
          Serial.println(buf[0]);
          continue;
        }
        Serial.readBytes((char *)&buf[1],sizeof(unsigned char));
        Serial.readBytes((char *)&buf[2],sizeof(unsigned char));
        if(buf[2]!=Ark[2])
        {
          memset(buf,'\0',sizeof(buf));
          continue;
        }
        else
        {
          break;
        }
       }
    }
    Serial.write(Ark,sizeof(Ark));
    delay(500);
    
    
    for(;;)
    {
      Serial.write(testCommand1,sizeof(testCommand1));
      delay(5000);
      Serial.write(testCommand2,sizeof(testCommand2));
      delay(5000);
      Serial.write(testCommand3,sizeof(testCommand3));
      delay(5000);
    }
    delay(10);
  }
}
