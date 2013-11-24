#define HallRecievePin0      7
#define HallRecievePin1      6
#define HallRecievePin2      5
#define HallRecievePin3      4
#define HallRecievePin4      3
#define HallRecievePin5      2

#define LED_SWITCH0 8
#define LED_SWITCH1 9
#define LED_SWITCH2 10
#define LED_SWITCH3 11
#define LED_SWITCH4 12
#define LED_CORRECT 13

#define LOCKER_SWITCH 14

void setup()
{
  Serial.begin(9600);
  
  pinMode(HallRecievePin0,INPUT);
  pinMode(HallRecievePin1,INPUT);
  pinMode(HallRecievePin2,INPUT);
  pinMode(HallRecievePin3,INPUT);
  pinMode(HallRecievePin4,INPUT);
  pinMode(HallRecievePin5,INPUT);

  pinMode(LED_SWITCH0,OUTPUT);
  pinMode(LED_SWITCH1,OUTPUT);
  pinMode(LED_SWITCH2,OUTPUT);
  pinMode(LED_SWITCH3,OUTPUT);
  pinMode(LED_SWITCH4,OUTPUT);
  pinMode(LED_CORRECT,OUTPUT);
  
  pinMode(LOCKER_SWITCH,OUTPUT);

  reset();
}

unsigned char res[] = {0x00,0x00,0x00,0x00,0x00};
unsigned char ans[] = {0x01,0x02,0x03,0x04,0x05};

unsigned  char pinRecieve[] = {HallRecievePin0,
                               HallRecievePin1,
                               HallRecievePin2,
                               HallRecievePin3,
                               HallRecievePin4,
                               };
unsigned char pinLed[] = {
  LED_SWITCH0,
  LED_SWITCH1,
  LED_SWITCH2,
  LED_SWITCH3,
  LED_SWITCH4,
};

void loop()
{
  Serial.println("HallRecievePin0 Detecting...");
  
  unsigned char count = 0x01;
  while(1)
  {
    int i =0;
    for(i=0;i<sizeof(pinRecieve);i++)
    {
      if((res[i]==0x00)&(!digitalRead(pinRecieve[i])))
      {
        res[i] = count;
        digitalWrite(pinLed[i],HIGH);
        Serial.print("i: ");
        Serial.println(i);
        count ++;
      }
    }
    if(count == (sizeof(pinRecieve)+1))
    {
      count = 0x01;
      boolean checkFlag = checkAns();
      Serial.println(checkFlag);
      if(checkFlag)
      {
        digitalWrite(LOCKER_SWITCH,LOW);
        digitalWrite(LED_CORRECT,HIGH);
        while(1)
        {
          delay(1000);
        }
      }
      else
      {
        LED_Error();
        memset(res,'\0',sizeof(res));
        reset();
      }
    }
    
  }
}

boolean checkAns(){
   if(memcmp(res,ans,sizeof(res)))
   {
     return false;
   }
   return true;
}

void reset()
{
  digitalWrite(LED_SWITCH0,LOW);
  digitalWrite(LED_SWITCH1,LOW);
  digitalWrite(LED_SWITCH2,LOW);
  digitalWrite(LED_SWITCH3,LOW);
  digitalWrite(LED_SWITCH4,LOW);
  digitalWrite(LED_CORRECT,LOW);

  digitalWrite(LOCKER_SWITCH,HIGH);
}

void LED_Error()
{
  int i =0;
  for(i=0;i<3;i++)
  {
    digitalWrite(LED_SWITCH0,HIGH);
    digitalWrite(LED_SWITCH1,HIGH);
    digitalWrite(LED_SWITCH2,HIGH);
    digitalWrite(LED_SWITCH3,HIGH);
    digitalWrite(LED_SWITCH4,HIGH);
    delay(600);
    digitalWrite(LED_SWITCH0,LOW);
    digitalWrite(LED_SWITCH1,LOW);
    digitalWrite(LED_SWITCH2,LOW);
    digitalWrite(LED_SWITCH3,LOW);
    digitalWrite(LED_SWITCH4,LOW);
    delay(600);
  }
}
