
#define resetPin 18

#define btn0    17
#define btn1    16
#define btn2    15
#define btn3    14

#define locker  6
#define btnLED0 8
#define btnLED1 9
#define btnLED2 10
#define btnLED3 11
#define LED_Red 12
#define LED_Green 13


void setup()
{
  Serial.begin(9600);
  
  pinMode(btn0,OUTPUT);
  pinMode(btn1,OUTPUT);
  pinMode(btn2,OUTPUT);
  pinMode(btn3,OUTPUT);
  pinMode(resetPin,OUTPUT);
  
  pinMode(locker,OUTPUT);
  pinMode(btnLED0,OUTPUT);
  pinMode(btnLED1,OUTPUT);
  pinMode(btnLED2,OUTPUT);
  pinMode(btnLED3,OUTPUT);
  pinMode(LED_Red,OUTPUT);
  pinMode(LED_Green,OUTPUT);
  
  reset();

}

static unsigned char selectNum[] = {0,0,0,0,0,0,0,0};
static unsigned char AnsNum[] = {3,1,2,3,4,1,2,3};
static unsigned char count = 1;
boolean unlockFlag = false;

static unsigned char lastNum = 0;

void loop()
{
  while(1)
  {
    if(checkReset())
    {
      Serial.println("Reset!!");
      reset();
    }
    if(!unlockFlag)
    {
      btnLEDCheck(btn0,btnLED0);
      btnLEDCheck(btn1,btnLED1);
      btnLEDCheck(btn2,btnLED2);
      btnLEDCheck(btn3,btnLED3);
      if(count >sizeof(AnsNum))
      {
        unlockFlag=checkAns();
        checkUnlock(unlockFlag);
      }
    }
    delay(10);
  }
}

void btnLEDCheck(unsigned int btnNum,unsigned int btnLedNum)
{ 
    digitalWrite(btnNum,HIGH);
    if(digitalRead(btnNum)==LOW)
    {
      Serial.print("btn: ");
      Serial.println(18-btnNum);
      digitalWrite(btnLedNum,HIGH);
      while(digitalRead(btnNum)==LOW)
      {
        delay(50);
      }
      selectNum[count-1] = 18 - btnNum;
      count++;
      digitalWrite(btnLedNum,LOW);
    }
    
}

boolean checkReset()
{
  digitalWrite(resetPin,HIGH);
  if(digitalRead(resetPin)==LOW)
  {
    return true;
  }
  return false;
}

boolean checkAns()
{
  int i =0;
  for(i=0;i<sizeof(AnsNum);i++)
  {
    Serial.print(selectNum[i]);
    Serial.print("  ");
    Serial.println(AnsNum[i]);
  }
  if(!memcmp(selectNum,AnsNum, sizeof(AnsNum)))
  {
    return true;
  }
  return false;
}

void checkUnlock(boolean flag)
{
  if(flag)
  {
    digitalWrite(LED_Green,HIGH);
    digitalWrite(LED_Red,LOW);
    digitalWrite(locker,LOW);
  }
  else
  {
    redLedFlash();
    reset();
  }
}

void redLedFlash()
{
  #define falshTime  200
  unsigned int i =0;
  for(i=0;i<8;i++)
  {
    digitalWrite(LED_Red,LOW);
    delay(falshTime);
    digitalWrite(LED_Red,HIGH);
    delay(falshTime);
  }
}



void reset()
{
  digitalWrite(locker,HIGH);
  digitalWrite(btnLED0,LOW);
  digitalWrite(btnLED1,LOW);
  digitalWrite(btnLED2,LOW);
  digitalWrite(btnLED3,LOW);
  digitalWrite(LED_Red,LOW);
  digitalWrite(LED_Green,LOW);
  
  count = 1;
  memset(selectNum,'\0',sizeof(selectNum));
  unlockFlag = false;
}
