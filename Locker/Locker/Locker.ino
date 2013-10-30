#define slotPin0 7
#define slotPin1 6
#define slotPin2 5
#define slotPin3 4

#define unLockPin 2
#define resetPin 3

#define ledLight0 9
#define ledLight1 10
#define ledLight2 11
#define ledLight3 12
#define ErrorLight 13

#define uint_8 unsigned char

void setup()
{
  Serial.begin(9600);

  pinMode(unLockPin,OUTPUT);
  
  pinMode(slotPin0,INPUT);
  pinMode(slotPin1,INPUT);
  pinMode(slotPin2,INPUT);
  pinMode(slotPin3,INPUT);
  pinMode(resetPin,INPUT);
  
  pinMode(ledLight0,OUTPUT);
  pinMode(ledLight1,OUTPUT);
  pinMode(ledLight2,OUTPUT);
  pinMode(ledLight3,OUTPUT);
  pinMode(ErrorLight,OUTPUT);
  
  Initialize();
}


void loop()
{

  uint_8 ans[] = {4,3,2,1};
  uint_8 checkBtn[] = {0,0,0,0};
  int count = 1;
  
  while(reset(checkBtn,&count))
  {
    if(recordBtnFunction(checkBtn,&count))
    {
      if(!strncmp((char *)checkBtn,(char *)ans,sizeof(checkBtn)))
      {
            Serial.println("Unlock !!");
            digitalWrite(unLockPin,LOW);
            while(reset(checkBtn,&count))
            {
              unlockSigh();
            }
      }
      else
      {
            int i=0;
            for(i=0;i<4;i++)
              Serial.print(checkBtn[i]);
            while(reset(checkBtn,&count))
            {
              ErrorSigh();
              reset(checkBtn,&count);
            }
      }
    }
     delay(100);
  }
}


void Initialize()
{
  digitalWrite(ledLight0,LOW);
  digitalWrite(ledLight1,LOW);
  digitalWrite(ledLight2,LOW);
  digitalWrite(ledLight3,LOW);
  digitalWrite(ErrorLight,LOW);
  digitalWrite(unLockPin,HIGH);
}

boolean reset(uint_8 *checkBtn,int *count)
{
     if(digitalRead(resetPin)!=1)
     {
       Serial.println("reset!!");
       memset(checkBtn,'\0',4);
       *count=1;
       Initialize();
      return false;
     }
     return true;
}

/*Off 紀錄*/
boolean recordBtnFunction(uint_8 *checkBtn,int *count)
{
 
     if((*count)>4)
     {
       return true;
     }
     //Serial.println("recordBtnFunction!!");
    if(digitalRead(slotPin0)!=1)
    {
      if(checkBtn[0]==0)
      {
        Serial.println("btn 0 on");
        digitalWrite(ledLight0,HIGH);
        checkBtn[0] = (*count);
        (*count)++;
      }
    }
    if(digitalRead(slotPin1)!=1)
    {
      if(checkBtn[1]==0)
      {
        Serial.println("btn 1 on");
        digitalWrite(ledLight1,HIGH);
        checkBtn[1] = (*count);
        (*count)++;
      }
    }
    if(digitalRead(slotPin2)!=1)
    {
      if(checkBtn[2]==0)
      {
        Serial.println("btn 2 on");
        digitalWrite(ledLight2,HIGH);
        checkBtn[2] = (*count);
        (*count)++;
      }
    }
    if(digitalRead(slotPin3)!=1)
    {
      if(checkBtn[3]==0)
      {
        Serial.println("btn 3 on");
        digitalWrite(ledLight3,HIGH);
        checkBtn[3] = (*count);
        (*count)++;
      }
    }
    return false;
}

void unlockSigh()
{
  digitalWrite(ledLight0,HIGH);
  digitalWrite(ledLight1,HIGH);
  digitalWrite(ledLight2,HIGH);
  digitalWrite(ledLight3,HIGH);
  delay(100);
  digitalWrite(ledLight0,LOW);
  digitalWrite(ledLight1,LOW);
  digitalWrite(ledLight2,LOW);
  digitalWrite(ledLight3,LOW);
  delay(100);
}

void ErrorSigh()
{
  digitalWrite(ErrorLight,HIGH);
  delay(100);
  digitalWrite(ErrorLight,LOW);
  delay(100); 
}
