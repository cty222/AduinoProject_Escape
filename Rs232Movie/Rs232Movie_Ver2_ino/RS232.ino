unsigned char Nak[] = { 0x55, 0x00, 0x33 };
unsigned char Ark[] = { 0x55, 0x01, 0x33 };

void linkToComputer()
{
    unsigned char buf[3];
    memset(buf,'\0',sizeof(buf));
    while(1)
    {
       if (Serial.available() > 0) {
         Serial.readBytes((char *)&buf[0],sizeof(unsigned char));
        if(buf[0]!=Ark[0])
        {
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
}


