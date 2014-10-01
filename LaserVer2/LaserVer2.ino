#include <SPI.h>
#include "reg.h"

#define LASER_DELAY         1500
#define LASER_POWER_DELAY    200

#define CS0_PIN 9
#define SS_PIN 10
#define MAX_LEN 16

#define btn0Pin 14
#define btn1Pin 15
#define AlarmPin 8
#define LockerPin    7
#define RedLightRecievePin    6
#define LaserRecieve1Pin      5
#define LaserRecieve0Pin      4
#define LaserRecievePowerPin  3
#define LaserSendPowerPin     2

int count = 0;

void setup()
{
  Serial.begin(9600);
  SPI.begin();
  
  pinMode(RedLightRecievePin,INPUT);
  pinMode(LaserRecieve0Pin,INPUT);
  pinMode(LaserRecieve1Pin,INPUT);
  pinMode(LaserSendPowerPin,OUTPUT);
  pinMode(LaserRecievePowerPin,OUTPUT); 

  pinMode(btn0Pin,INPUT); 
  pinMode(btn1Pin,INPUT); 
  pinMode(LockerPin,OUTPUT); 
  pinMode(AlarmPin,OUTPUT); 

  pinMode(SS_PIN,OUTPUT); 
  pinMode(CS0_PIN,OUTPUT); 
  
  reset();
}

boolean unlockFlag = false;
boolean redLightFlag = false;
boolean alertFlag = false;
boolean alertStopFlag =false;

void loop()
{
  Serial.println("RedLight Detecting...");
  delay(LASER_DELAY);
  while(1)
  {
    if(!unlockFlag)
    {
      while(!redLightFlag)
      {
        Serial.println("Step 1");
        digitalWrite(RedLightRecievePin,LOW);
        if(digitalRead(RedLightRecievePin)>0)
        {
          delay(200);
          Serial.println("Step 2");
          digitalWrite(RedLightRecievePin,LOW);
          if(digitalRead(RedLightRecievePin)>0)
          {
             Serial.println("Step 3");
             redLightFlag = true;
             digitalWrite(LaserSendPowerPin,HIGH);
             digitalWrite(LaserRecievePowerPin,HIGH);
          }
        }
        delay(LASER_POWER_DELAY);
      }
      
      
      Serial.println("Step 4");
      digitalWrite(LaserRecieve0Pin,LOW);
      alertFlag|=digitalRead(LaserRecieve0Pin);
      digitalWrite(LaserRecieve1Pin,LOW);
      alertFlag|=digitalRead(LaserRecieve1Pin);  
      checkAlert(); 
      
      alertFlag = false;
      unlockFlag = checkClose();   
    }
    else
    {
      digitalWrite(LaserSendPowerPin,LOW);
      digitalWrite(LaserRecievePowerPin,LOW);
      digitalWrite(LockerPin,LOW);
      Serial.println("End!!");
      delay(100000);
    }
    delay(100);
  }
}

void reset()
{
      digitalWrite(LaserSendPowerPin,LOW);
      digitalWrite(LaserRecievePowerPin,LOW);
      digitalWrite(LockerPin,HIGH);
      digitalWrite(AlarmPin,LOW);
}

void checkAlert()
{
  if(alertFlag)
  {
    if(!alertStopFlag)
    {
      Serial.println("Alert!!");
      digitalWrite(AlarmPin,HIGH);
      alertStopFlag = true;
    }
  }
  else
  {
    alertStopFlag = false;
    digitalWrite(AlarmPin,LOW);
    Serial.println("Alert Reset");
  }
}

boolean checkClose()
{
  if(digitalRead(btn0Pin))
  {
    Serial.println("btn0Pin");
    if(digitalRead(btn1Pin))
    {
      Serial.println("btn1Pin");
      MFRC522_Init(CS0_PIN);
      if(searchCard(CS0_PIN))
      {
        Serial.println("right card");
        return true;
      }
    }
  }
  return false;
}


//第一種 假結局
uchar CardNumA5[][5] = {
  0xD5 , 0x7 , 0xC9 , 0xDC , 0xC7,
  0x8C , 0xC2 , 0x2E , 0x35 , 0x55,
  0x8C , 0x71 , 0x56 , 0x35 , 0x9E,
  0x7C , 0xFD , 0x73 , 0x35 , 0xC7,
  0x7C , 0xDA , 0xF , 0x35 , 0x9C,
  0x8C , 0xA9 , 0x1F , 0x35 , 0xF, 
};
uchar CardNumA0[][5] = {
  0x9D , 0x34 , 0x41 , 0xA , 0xE2,
  0x8C , 0x5E , 0x44 , 0x35 , 0xA3 ,
  0x8C , 0xFB , 0x18 , 0x35 , 0x5A , 
  0x8C , 0x91 , 0xCE , 0x35 , 0xE6 ,
  0x8C , 0x76 , 0x80 , 0x35 , 0x4F,
  0x8C , 0xA9 , 0x1F , 0x35 , 0xF, 
};

uchar serNum[5];
boolean searchCard(uchar csPin)
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
    int i=0;
    if(csPin==CS0_PIN)
    {
      for(i=0;i<sizeof(CardNumA0)/5;i++)
      {
        if(!strncmp((char *)serNum,(char *)&CardNumA0[i][0],5))
        {
          //door unlock
          return true;
        }
      }
    }
  }
  return false;
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

