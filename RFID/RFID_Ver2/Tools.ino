void showCardNum(uchar *serNum)
{
    Serial.println("The card's number is : ");
    Serial.print(serNum[0],HEX);
    Serial.print(" , ");
    Serial.print(serNum[1],HEX);
    Serial.print(" , ");
    Serial.print(serNum[2],HEX);
    Serial.print(" , ");
    Serial.print(serNum[3],HEX);
    Serial.print(" , ");
    Serial.print(serNum[4],HEX);
    Serial.println(" ");
}

void checkAns()
{
  if(false)
  {
  pinMode(Locker0,OUTPUT); 
  digitalWrite(Locker0, LOW);  
  pinMode(Locker1,OUTPUT); 
  digitalWrite(Locker1, LOW);
  }
}


uchar CardNumA0[][5] = {0x9D,0x34,0x41,0x0a,0xe2,
                        0x9D,0xa7,0x5F,0x0a,0x6f,
                        0x5D,0x37,0x5a,0x0a,0x3a,};
uchar CardNumA1[][5] = {0x0d,0x20,0x5c,0x0a,0x7b,
                     0xdd,0x85,0x73,0x0a,0x21,
                     0xbd,0xbe,0x5a,0x0a,0x53,};

void checkID(uchar csPin)
{
  int i=0;
  switch(csPin)
  {
    case CS0_PIN:
    case CS1_PIN:
    case CS2_PIN:
      for(i=0;i<sizeof(CardNumA0)/5;i++)
      {
        showCardNum(&CardNumA0[i][0]);
      }
      break;
    case CS3_PIN:
    case CS4_PIN:
    case CS5_PIN:
      for(i=0;i<sizeof(CardNumA1)/5;i++)
      {
        showCardNum(&CardNumA1[i][0]);
      }
      break;
  }
}
