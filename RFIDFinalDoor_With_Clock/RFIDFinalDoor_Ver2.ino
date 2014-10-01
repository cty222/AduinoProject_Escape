#include <SPI.h>
#include "reg.h"

//========================================
//=============修改聲音===================
#define StopBeep  2
#define StartBeep 1
#define ResetBeep 4
//========================================

#define CS0_PIN 9
#define CS1_PIN 8

#define TimeSig0_Pin 14
#define TimeSig1_Pin 15
#define TimeSig2_Pin 16
#define TimeSig3_Pin 17

#define Btn0_Pin 18
#define Btn1_Pin 19

#define AlarmSig0 6
#define AlarmSig1 5

#define Locker_Pin 7

#define SS_PIN 10
#define MAX_LEN 16


void setup()
{
  Serial.begin(9600);
  SPI.begin();
   
  pinMode(SS_PIN,OUTPUT); 
  digitalWrite(SS_PIN, HIGH);
    
  pinMode(CS0_PIN,OUTPUT); 
  digitalWrite(CS0_PIN, HIGH);
  pinMode(CS1_PIN,OUTPUT); 
  digitalWrite(CS1_PIN, HIGH);
  
  pinMode(TimeSig0_Pin,OUTPUT);
  digitalWrite(TimeSig0_Pin, HIGH);
  pinMode(TimeSig1_Pin,OUTPUT);
  digitalWrite(TimeSig1_Pin, HIGH);
  pinMode(TimeSig2_Pin,OUTPUT);
  digitalWrite(TimeSig2_Pin, HIGH);
  pinMode(TimeSig3_Pin,OUTPUT);
  digitalWrite(TimeSig3_Pin, HIGH);
  
  pinMode(Btn0_Pin,INPUT);
  pinMode(Btn1_Pin,INPUT);
  
  pinMode(Locker_Pin,OUTPUT); 
  digitalWrite(Locker_Pin, HIGH);
}

uchar serNum[5];
boolean startFlag = false;
boolean resetFlag = false;
boolean lockFlag  = true;

void loop()
{  
  Serial.println("Waiting for signal to start...");
  
  Initialize ();
  
  //1. 直到按鈕按下才開始 
  while( !startFlag )
  {
    btnSignal(false);
    delay(100);
  }
  
  Serial.println("start!!");
   
  //2. 主要功能
  while(lockFlag)
  {
    //2.1 如果有按過按鈕會反映
    btnSignal(false);
    
    if (startFlag)
    {
      MFRC522_Init(CS0_PIN);
      searchCard(CS0_PIN);
    
      MFRC522_Init(CS1_PIN);
      searchCard(CS1_PIN);
    }
    
    delay(50);
  }
  
  //3 重新開始
  while (!resetFlag)
  {
    resetFlag = digitalRead(Btn0_Pin) | digitalRead(Btn1_Pin);
    delay(50);
  }
  playBeepVoice(ResetBeep);
  delay(1000);
}

void Initialize ()
//
// 重設全域變數
//
{
  startFlag = false;
  resetFlag = false;
  lockFlag = true;
  digitalWrite(Locker_Pin, HIGH);
}

boolean btnSignal(boolean forceFlag)
//
//forceFlag是強制啟用功能的flag
//
{
  //1. 判斷是否有被按下
  boolean res = digitalRead(Btn0_Pin) | digitalRead(Btn1_Pin);
  
  //2. 是否強制啟動
  res |= forceFlag;
  
  //3. 根據res & forceFlag去判斷是否發出啟動/停止訊號
  if(res)
  {
    //3.1 改變全域旗標狀態 
    startFlag = !startFlag;
    
    //3.2. 發出聲音
    if (startFlag)
    {  
      playBeepVoice(StartBeep);
    }
    else
    {
      playBeepVoice(StopBeep);
    }
    
    //3.3. 送出訊號
    signalToTimeMachine(TimeSig0_Pin);
    signalToTimeMachine(TimeSig1_Pin);
    signalToTimeMachine(TimeSig2_Pin);
    signalToTimeMachine(TimeSig3_Pin);
    
  }
  
  // 4. 回傳是否動作過
  return res;
}

void signalToTimeMachine(int timeNum)
//
// 發訊號給時鐘
//
{
    digitalWrite(timeNum,LOW);
    delay(100);
    digitalWrite(timeNum,HIGH);
}

void playBeepVoice(int times)
//
// 嗶嗶聲音
//
{
    int i;
    for (i=0;i< times; i++)
    {
      analogWrite(AlarmSig0,0xf0);
      delay(200);
      digitalWrite(AlarmSig0,LOW);
      delay(100);
    }
    
    for (i=0;i<times;i++)
    {
      analogWrite(AlarmSig1,0xf0);
      delay(200);
      digitalWrite(AlarmSig1,LOW);
      delay(100);
    }
}

//第一種 假結局
uchar CardNumA6[][5] = {
  0x95 , 0xA0 , 0xC7 , 0xDC , 0x2E,
  0x8C , 0x7A , 0x7C , 0x35 , 0xBF,
  0x8C , 0x62 , 0x9D , 0x35 , 0x46,
  0x8C , 0x47 , 0x3 , 0x35 , 0xFD,
  0x8C , 0x59 , 0x8F , 0x35 , 0x6F,
  0x8C , 0xA9 , 0x1F , 0x35 , 0xF,
};

//第二種 真結局
uchar CardNumA12[][5] = {
  0x7C , 0xDF , 0x7A , 0x35 , 0xEC,
  0x8C , 0xED , 0xEE , 0x35 , 0xBA,
  0x8C , 0xDA , 0xBC , 0x35 , 0xDF,
  0x8C , 0x71 , 0xC7 , 0x35 , 0xF,
  0x8C , 0xA9 , 0x1F , 0x35 , 0xF,
};

void searchCard(uchar csPin)
//
// 掃描卡片
//
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
          //在時間會走的情況才會動作
          if(startFlag)
          {
            Serial.println("bad ans");
            digitalWrite(Locker_Pin, LOW);
            lockFlag = false;
            btnSignal(true);
          }
        }
      }
    }
    else if(csPin==CS1_PIN)
    {
      for(i=0;i<sizeof(CardNumA12)/5;i++)
      {
        if(!strncmp((char *)serNum,(char *)&CardNumA12[i][0],5))
        {
            //door unlock
            //在時間會走的情況才會動作
            if(startFlag)
            {
              Serial.println("true ans");
              digitalWrite(Locker_Pin, LOW);
              lockFlag = false;
              btnSignal(true);
            }
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

