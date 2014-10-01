#include <SPI.h>
#include "reg.h"

#define CS1_PIN 8
#define CS0_PIN 9

#define RST1_PIN 3
#define RST0_PIN 2

#define Locker_Pin 14

#define SS_PIN 10
#define MAX_LEN 16

void setup()
{
  Serial.begin(9600);
  SPI.begin();
   
  pinMode(SS_PIN,OUTPUT); 
  digitalWrite(SS_PIN, HIGH);
  pinMode(RST1_PIN,OUTPUT); 
  digitalWrite(RST1_PIN, HIGH);
  pinMode(RST0_PIN,OUTPUT); 
  digitalWrite(RST0_PIN, HIGH);
    
  pinMode(CS0_PIN,OUTPUT); 
  digitalWrite(CS0_PIN, LOW);
  pinMode(CS1_PIN,OUTPUT); 
  digitalWrite(CS1_PIN, LOW);
  
  pinMode(Locker_Pin,OUTPUT); 
  digitalWrite(Locker_Pin, LOW);
}

uchar serNum[5];

void loop()
{
  while(1)
  {
    MFRC522_Init(CS0_PIN);
    searchCard(CS0_PIN);
    
    MFRC522_Init(CS1_PIN);
    searchCard(CS1_PIN);
    
    delay(50);
  }
}

//第一種 假結局
uchar CardNumA6[][5] = {
  0x95 , 0xA0 , 0xC7 , 0xDC , 0x2E,
  0x8C , 0x7A , 0x7C , 0x35 , 0xBF,
  0x8C , 0x62 , 0x9D , 0x35 , 0x46,
  0x8C , 0x47 , 0x3 , 0x35 , 0xFD,
  0x8C , 0x59 , 0x8F , 0x35 , 0x6F,
};

//第二種 真結局
uchar CardNumA12[][5] = {
  0x7C , 0xDF , 0x7A , 0x35 , 0xEC,
  0x8C , 0xED , 0xEE , 0x35 , 0xBA,
  0x8C , 0xDA , 0xBC , 0x35 , 0xDF,
  0x8C , 0x71 , 0xC7 , 0x35 , 0xF,
};

void searchCard(uchar csPin)
{
  uchar status;
  uchar str[16];
  status = MFRC522_Request(PICC_REQIDL, str,csPin);
  if (status == MI_OK)
  {
    Serial.println("Find out a card ");
  }
  
  status = MFRC522_Anticoll(str,csPin);
  memcpy(serNum, str, 5);
  if (status == MI_OK)
  {
#if 0
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
#else
    int i=0;
    if(csPin==CS0_PIN)
    {
      for(i=0;i<sizeof(CardNumA6)/5;i++)
      {
        if(!strncmp((char *)serNum,(char *)&CardNumA6[i][0],5))
        {
          //door unlock
          Serial.println("bad ans");
          digitalWrite(Locker_Pin, HIGH);

        }
      }
    }
    else
    {
      for(i=0;i<sizeof(CardNumA12)/5;i++)
      {
        if(!strncmp((char *)serNum,(char *)&CardNumA12[i][0],5))
        {
            //door unlock
            Serial.println("true ans");
            digitalWrite(Locker_Pin, HIGH);
        }
      }
    }
#endif
  }
 
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

