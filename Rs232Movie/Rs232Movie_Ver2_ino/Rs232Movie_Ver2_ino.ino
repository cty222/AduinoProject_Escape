#include <SPI.h>
#include "reg.h"
#include "cardNums.h"

#define CS0_PIN 9

#define SS_PIN 10
#define MAX_LEN 16

void setup()
{
  Serial.begin(115200);
  SPI.begin();
   
  pinMode(SS_PIN,OUTPUT); 
  digitalWrite(SS_PIN, HIGH);
    
  pinMode(CS0_PIN,OUTPUT); 
  digitalWrite(CS0_PIN, LOW);

}

uchar serNum[5];

unsigned char sendCmd[] = { 0x55, 0xff, 0x33 };

void loop()
{
  MFRC522_Init(CS0_PIN);
  linkToComputer();
  uchar tmp;
  while(1)
  {
       tmp = searchCard(CS0_PIN);
       if(tmp!=0xff)
       {
           sendCmd[1]=tmp;
           Serial.write(sendCmd,sizeof(sendCmd));
           delay(100);
       }
       delay(100);
  }
}

uchar cmdArray[] = {0x10,0x11,0x12,0x13,0x14,0x15,
                    0x16,0x17,0x18,0x19,0x1d,0x1b,
                    0x1c};

uchar searchCard(uchar csPin)
{
  uchar cmd = 0xff;
  uchar status;
  uchar str[16];
  status = MFRC522_Request(PICC_REQIDL, str,csPin);
  status = MFRC522_Anticoll(str,csPin);
  memcpy(serNum, str, 5);
  if (status == MI_OK)
  {
    int i=0;
    for(i=0;i<sizeof(CardNumA0)/5;i++)
    {
      if(!strncmp((char *)serNum,(char *)&CardNumA0[i][0],5))
      {
          cmd = cmdArray[0];
      }
    }
    for(i=0;i<sizeof(CardNumA1)/5;i++)
    {
      if(!strncmp((char *)serNum,(char *)&CardNumA1[i][0],5))
      {
          cmd = cmdArray[1];
      }
    }
    for(i=0;i<sizeof(CardNumA2)/5;i++)
    {
      if(!strncmp((char *)serNum,(char *)&CardNumA2[i][0],5))
      {
          cmd = cmdArray[2];
      }
    }
    for(i=0;i<sizeof(CardNumA3)/5;i++)
    {
      if(!strncmp((char *)serNum,(char *)&CardNumA3[i][0],5))
      {
          cmd = cmdArray[3];
      }
    }
    for(i=0;i<sizeof(CardNumA4)/5;i++)
    {
      if(!strncmp((char *)serNum,(char *)&CardNumA4[i][0],5))
      {
          cmd = cmdArray[4];
      }
    }
    for(i=0;i<sizeof(CardNumA5)/5;i++)
    {
      if(!strncmp((char *)serNum,(char *)&CardNumA5[i][0],5))
      {
          cmd = cmdArray[5];
      }
    }
    for(i=0;i<sizeof(CardNumA6)/5;i++)
    {
      if(!strncmp((char *)serNum,(char *)&CardNumA6[i][0],5))
      {
          cmd = cmdArray[6];
      }
    }
    for(i=0;i<sizeof(CardNumA7)/5;i++)
    {
      if(!strncmp((char *)serNum,(char *)&CardNumA7[i][0],5))
      {
          cmd = cmdArray[7];
      }
    }
    for(i=0;i<sizeof(CardNumA8)/5;i++)
    {
      if(!strncmp((char *)serNum,(char *)&CardNumA8[i][0],5))
      {
          cmd = cmdArray[8];
      }
    }
    for(i=0;i<sizeof(CardNumA9)/5;i++)
    {
      if(!strncmp((char *)serNum,(char *)&CardNumA9[i][0],5))
      {
          cmd = cmdArray[9];
      }
    }
    for(i=0;i<sizeof(CardNumA10)/5;i++)
    {
      if(!strncmp((char *)serNum,(char *)&CardNumA10[i][0],5))
      {
          cmd = cmdArray[10];
      }
    }
    for(i=0;i<sizeof(CardNumA11)/5;i++)
    {
      if(!strncmp((char *)serNum,(char *)&CardNumA11[i][0],5))
      {
          cmd = cmdArray[11];
      }
    }
    for(i=0;i<sizeof(CardNumA12)/5;i++)
    {
      if(!strncmp((char *)serNum,(char *)&CardNumA12[i][0],5))
      {
          cmd = cmdArray[12];
      }
    }
  }
  return cmd;
}

uchar MFRC522_Anticoll(uchar *serNum, uchar csPin)
{
uchar status;
uchar i;
uchar serNumCheck=0;
uint unLen;
ClearBitMask(Status2Reg, 0x08,csPin); //TempSensclear
ClearBitMask(CollReg,0x80,csPin); //ValuesAfterColl
Write_MFRC522(BitFramingReg, 0x00,csPin); //TxLastBists = BitFramingReg[2..0]
serNum[0] = PICC_ANTICOLL;
serNum[1] = 0x20;
status = MFRC522_ToCard(PCD_TRANSCEIVE, serNum, 2, serNum, &unLen,csPin);
if (status == MI_OK)
{
//校验卡序列号
for (i=0; i<4; i++)
{
serNumCheck ^= serNum[i];
}
if (serNumCheck != serNum[i])
{
status = MI_ERR;
}
}
//SetBitMask(CollReg, 0x80); //ValuesAfterColl=1
return status;
}

