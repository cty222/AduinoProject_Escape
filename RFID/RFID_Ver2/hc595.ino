void hc595_init(uchar resetPin, uchar serPin, uchar clkPin, uchar rclk)
{
  pinMode(resetPin,OUTPUT); 
  digitalWrite(resetPin, LOW);
  
  pinMode(serPin,OUTPUT); 
  digitalWrite(serPin, LOW);
  
  pinMode(clkPin,OUTPUT); 
  digitalWrite(clkPin, LOW);
  
  pinMode(rclk,OUTPUT); 
  digitalWrite(rclk, LOW);
}

void hc595_write(uchar data ,uchar serPin, uchar clkPin)
{
#if 0 
  boolean ptr[8];
  int i =0;
  for(i=0;i<8;i++)
  {
    ptr[i] = (data >> i)&0x01;
  }
  for(i=0;i<8;i++)
  {
  digitalWrite(serPin, *(ptr+i));
  digitalWrite(clkPin, HIGH);
  digitalWrite(clkPin, LOW);
  digitalWrite(serPin, LOW);
  }
#else if
  digitalWrite(LedLatch, LOW);
  shiftOut(16, 18, MSBFIRST, 0x00);
  digitalWrite(LedLatch, HIGH);
#endif
}

void hc595_display(uchar rclk)
{
  digitalWrite(rclk, HIGH);
  digitalWrite(rclk, LOW);
}

void hc595_Reset(uchar resetPin)
{
  digitalWrite(resetPin, LOW);
  digitalWrite(resetPin, HIGH);
}
