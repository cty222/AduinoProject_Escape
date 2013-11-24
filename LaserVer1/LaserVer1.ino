#define RedLightRecievePin    5
#define LaserRecieve2Pin      4
#define LaserRecieve1Pin      3
#define LaserPowerPin      2

int count = 0;

void setup()
{
  Serial.begin(9600);
  
  pinMode(RedLightRecievePin,INPUT);
  pinMode(LaserRecieve1Pin,INPUT);
  pinMode(LaserRecieve2Pin,INPUT);
  pinMode(LaserPowerPin,OUTPUT);
    
  reset();
}

boolean unlockFlag = false;
boolean redLightFlag = false;
boolean alertFlag = false;
boolean alertStopFlag =false;

void loop()
{
  Serial.println("RedLight Detecting...");
  while(1)
  {
    /*digitalWrite(LaserRecieve1Pin,LOW);
    Serial.print("RedLightRecievePin: ");
    Serial.println(digitalRead(RedLightRecievePin));
    delay(100); 
    Serial.print("LaserRecieve1Pin: ");
    Serial.println(digitalRead(LaserRecieve1Pin));
    digitalWrite(LaserPowerPin,HIGH);
    delay(100); 
    continue;*/
    if(!unlockFlag)
    {
      while(!redLightFlag)
      {
        //Serial.println("Step 1");
        digitalWrite(RedLightRecievePin,LOW);
        if(digitalRead(RedLightRecievePin)>0)
        {
          delay(200);
          //Serial.println("Step 2");
          digitalWrite(RedLightRecievePin,LOW);
          if(digitalRead(RedLightRecievePin)>0)
          {
             //Serial.println("Step 3");
             redLightFlag = true;
             digitalWrite(LaserPowerPin,HIGH);
          }
        }
        delay(100);
      }
      
      
      //Serial.println("Step 4");
      digitalWrite(LaserRecieve1Pin,LOW);
      alertFlag|=digitalRead(LaserRecieve1Pin);
      //alertFlag|=digitalRead(LaserRecieve2Pin);  
      checkAlert(); 
      
      alertFlag = false;
      unlockFlag = checkClose();
      delay(100);    
    }
    else
    {
      Serial.println("End!!");
    }
    delay(100);
  }
}

void reset()
{
      digitalWrite(RedLightRecievePin,LOW);
      digitalWrite(LaserRecieve1Pin,LOW);
      digitalWrite(LaserRecieve2Pin,LOW);
      digitalWrite(LaserPowerPin,LOW);
}

void checkAlert()
{
  if(alertFlag)
  {
    if(!alertStopFlag)
    {
      Serial.println("Alert!!");
      alertStopFlag = true;
    }
  }
  else
  {
    alertStopFlag = false;
    Serial.println("Alert Reset");
  }
}

boolean checkClose()
{
  return false;
}

