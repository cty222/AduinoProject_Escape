#include <SPI.h>
#include "reg.h"

#define CS3_PIN 6
#define CS2_PIN 7
#define CS1_PIN 8
#define CS0_PIN 9

#define RST3_PIN 5
#define RST2_PIN 4
#define RST1_PIN 3
#define RST0_PIN 2

#define SS_PIN 10
#define MAX_LEN 16

void setup()
{
  Serial.begin(9600);
  SPI.begin();
   
  pinMode(SS_PIN,OUTPUT); 
  digitalWrite(SS_PIN, HIGH);
  pinMode(RST3_PIN,OUTPUT); 
  digitalWrite(RST3_PIN, HIGH);
  pinMode(RST2_PIN,OUTPUT); 
  digitalWrite(RST2_PIN, HIGH);
  pinMode(RST1_PIN,OUTPUT); 
  digitalWrite(RST1_PIN, HIGH);
  pinMode(RST0_PIN,OUTPUT); 
  digitalWrite(RST0_PIN, HIGH);
    
  pinMode(CS0_PIN,OUTPUT); 
  digitalWrite(CS0_PIN, LOW);
  pinMode(CS1_PIN,OUTPUT); 
  digitalWrite(CS1_PIN, LOW);
  
  pinMode(CS2_PIN,OUTPUT); 
  digitalWrite(CS2_PIN, LOW);
  pinMode(CS3_PIN,OUTPUT); 
  digitalWrite(CS3_PIN, LOW);

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
    
    MFRC522_Init(CS2_PIN);
    searchCard(CS2_PIN);
    
    MFRC522_Init(CS3_PIN);
    searchCard(CS3_PIN);
    delay(50);
  }
}

void searchCard(uchar csPin)
{
  uchar status;
  uchar str[16];
  status = MFRC522_Request(PICC_REQIDL, str,csPin);
  if (status == MI_OK)
  {
    Serial.println("Find out a card ");
#if 0
    Serial.print(str[0],BIN);
    Serial.print(" , ");
    Serial.print(str[1],BIN);
    Serial.println(" ");
#endif
  }
  
  status = MFRC522_Anticoll(str,csPin);
  memcpy(serNum, str, 5);
  if (status == MI_OK)
  {
#if 1
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
    if(!strncmp((char *)serNum,(char *)CardNumA0,5))
    {
      Serial.println("A0");
    }
    if(!strncmp((char *)serNum,(char *)CardNumA1,5))
    {
      Serial.println("A1");
    }
    else if(!strncmp((char *)serNum,(char *)CardNumA2,5))
    {
      Serial.println("A2");
    }
    else if(!strncmp((char *)serNum,(char *)CardNumA3,5))
    {
      Serial.println("A3");
    }
    else if(!strncmp((char *)serNum,(char *)CardNumA4,5))
    {
      Serial.println("A4");
    }
    else if(!strncmp((char *)serNum,(char *)CardNumA5,5))
    {
      Serial.println("A5");
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

