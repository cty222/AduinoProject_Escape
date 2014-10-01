static uchar ledMaping[]= {LED0_PIN, LED1_PIN, LED2_PIN, LED3_PIN, LED4_PIN, LED5_PIN};

void Led_display(uchar LedNum,boolean flag)
{
  #if 1
    digitalWrite(ledMaping[CS0_PIN-LedNum], flag);
 #else
 #endif
}

void Led_Init()
{
#if 1
  int i=0;
  for(i=0;i<sizeof(ledMaping);i++)
  {
      pinMode(ledMaping[i],OUTPUT); 
      digitalWrite(ledMaping[i], HIGH);
  }
#else
  hc595_init(LedReset, LedDataOut, LedClock, LedLatch);
#endif
}
